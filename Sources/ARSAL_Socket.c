/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/**
 * @file libARSAL/ARSAL_Socket.c
 * @brief This file contains sources about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ARSAL_INCLUDE_WSA
#include <libARSAL/ARSAL_Socket.h>
#include <libARSAL/ARSAL_Print.h>
#undef ARSAL_INCLUDE_WSA

#ifdef _WIN32
#define USING_WSA 1
#define close closesocket

#include <mswsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#else

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

/* IOV_MAX should normally be defined in limits.h. in case it's not, just
 * take a standard value of 1024. It should not cause issue since we emulate
 * writev/readv */
#ifndef IOV_MAX
#define IOV_MAX 1024
#endif

/* SSIZE_MAX should normally be defined in limits.h. In case it's not, hope
 * compiler define __SIZE_MAX__ and derived SSIZE_MAX from it. */
#ifndef SSIZE_MAX
#define SSIZE_MAX (__SIZE_MAX__ >> 1)
#endif

static ssize_t writev(int sockfd, const struct iovec *iov, int iovcnt)
{
    int i;
    ssize_t wbytes = 0;
    ssize_t ret = 0;
    ssize_t sum = 0;

    if (iovcnt <= 0 || iovcnt > IOV_MAX)
    {
        errno = EINVAL;
        return -1;
    }

    /* check we don't overflow SSIZE_MAX */
    for (i = 0; i < iovcnt; i++)
    {
        if (SSIZE_MAX - sum < (ssize_t)iov[i].iov_len)
        {
            errno = EINVAL;
            return -1;
        }

        sum += iov[i].iov_len;
    }

    for (i = 0; i < iovcnt; i++)
    {
        ret = send(sockfd, iov[i].iov_base, iov[i].iov_len, 0);
        if (ret < 0)
        {
            return -1;
        }

        wbytes += ret;

        /* short write means we can't proceed to the next area so break */
        if (ret < (ssize_t)iov[i].iov_len)
        {
            break;
        }
    }

    return wbytes;
}

static ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    int i;
    ssize_t rbytes = 0;
    ssize_t ret = 0;
    ssize_t sum = 0;

    if (iovcnt <= 0 || iovcnt > IOV_MAX)
    {
        errno = EINVAL;
        return -1;
    }

    /* check we don't overflow SSIZE_MAX */
    for (i = 0; i < iovcnt; i++)
    {
        if (SSIZE_MAX - sum < (ssize_t)iov[i].iov_len)
        {
            errno = EINVAL;
            return -1;
        }

        sum += iov[i].iov_len;
    }

    for (i = 0; i < iovcnt; i++)
    {
        ret = recv(sockfd, iov[i].iov_base, iov[i].iov_len, 0);
        if (ret < 0)
        {
            return -1;
        }

        rbytes += ret;

        /* short read means we can't proceed to the next area */
        if (ret < (ssize_t)iov[i].iov_len)
        {
            break;
        }
    }

    return rbytes;
}
#endif

#define CODE(e)                                                                                    \
    case WSA##e:                                                                                   \
        code = e;                                                                                  \
        break;
#define RCODE(w, e)                                                                                \
    case w:                                                                                        \
        code = e;                                                                                  \
        break;

static inline int ARSAL_Socket_Seterrno(int ret)
{
#ifdef _WIN32
    if (ret < 0) {
        int code = WSAGetLastError();

        switch (code) {
            CODE(EINTR)
            CODE(EBADF)
            CODE(EACCES)
            CODE(EFAULT)
            CODE(EINVAL)
            CODE(EMFILE)
            CODE(EWOULDBLOCK)
            CODE(EINPROGRESS)
            CODE(EALREADY)
            CODE(ENOTSOCK)
            CODE(ECONNREFUSED)
            CODE(ETIMEDOUT)
            CODE(EHOSTUNREACH)
            CODE(ENETUNREACH)
        default:
            code = -code;
        }

        errno = code;
        return code;
    }

    return ret;
#else
    /* no-op, unix sockets already set errno */
    return ret;
#endif
}

#undef CODE
#undef RCODE

int ARSAL_Socket_Create(int domain, int type, int protocol)
{
#ifdef USING_WSA
    static int wsa_initialized = 0;
    if (!wsa_initialized) {
        WSADATA wsaData;
        /* TODO: Find a place to call WSACleanup */
        if (WSAStartup(0x0202, &wsaData) != 0) {
            /* fatal error? */
        }

        wsa_initialized = 1;
    }
#endif

    return socket(domain, type, protocol);
}

int ARSAL_Socket_CreatePair(int *pfds)
{
    /* http://stackoverflow.com/questions/3333361/how-to-cancel-waiting-in-select-on-windows */
    struct sockaddr_in inaddr;
    struct sockaddr addr;
    pfds[0] = -1;
    pfds[1] = -1;
    int ret = 0;

    int lst = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&inaddr, 0, sizeof(inaddr));
    memset(&addr, 0, sizeof(addr));

    inaddr.sin_family = AF_INET;
    inaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    inaddr.sin_port = 0;

    if (ret == 0) {
        int yes = 1;
        ret = setsockopt(lst, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));
    }

    if (ret == 0) {
        ret = bind(lst, (struct sockaddr *)&inaddr, sizeof(inaddr));
    }

    if (ret == 0) {
        ret = listen(lst, 1);
    }

    socklen_t len = sizeof(inaddr);
    if (ret == 0) {
        ret = getsockname(lst, &addr, &len);
    }

    if (ret == 0) {
        pfds[0] = socket(AF_INET, SOCK_STREAM, 0);
        ret = connect(pfds[0], &addr, len);
        pfds[1] = accept(lst, 0, 0);
        close(lst);
    }

    if (pfds[0] == -1 || pfds[1] == -1) {
        ret = -1;
    }

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int ret;

    while (((ret = connect(sockfd, addr, addrlen)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
#ifdef _WIN32
    if (ret < 0 && WSAGetLastError() == WSAEWOULDBLOCK) {
        errno = EINPROGRESS;
    }
#endif
    return ret;
}

#ifdef _WIN32
/*
static GUID WSASendMsg_GUID = WSAID_WSASENDMSG;
static LPFN_WSASENDMSG WSASendMsg = NULL;
*/
#endif
ssize_t ARSAL_Socket_SendMsg(int sockfd, const struct msghdr *msg, int flags)
{
#ifdef _WIN32
    /*
    if (WSASendMsg == 0) {
        DWORD dwBytesReturned = 0;
        int res = WSAIoctl((SOCKET)sockfd, SIO_GET_EXTENSION_FUNCTION_POINTER, &WSASendMsg_GUID,
                           sizeof(WSASendMsg_GUID), &WSASendMsg, sizeof(WSASendMsg),
                           &dwBytesReturned, NULL, NULL);
        if (res != 0) {
            errno = ENOENT;
            return -1;
        }
    }
    */

    SOCKADDR name;
    WSAMSG wmsg;
    wmsg.name = &name;
    wmsg.namelen = sizeof(SOCKADDR);

    wmsg.lpBuffers = (LPWSABUF)alloca(sizeof(WSABUF) * msg->msg_iovlen);
    wmsg.dwBufferCount = (DWORD)msg->msg_iovlen;
    for (int i = 0; i < msg->msg_iovlen; i++) {
        wmsg.lpBuffers[i].buf = msg->msg_iov[i].iov_base;
        wmsg.lpBuffers[i].len = msg->msg_iov[i].iov_len;
    }

    wmsg.Control.buf = msg->msg_control;
    wmsg.Control.len = msg->msg_controllen;
    wmsg.dwFlags = flags;

    DWORD dwNumBytesSent = 0;
    int ret = WSASendMsg(sockfd, &wmsg, flags, &dwNumBytesSent, NULL, NULL);
    if (ret == 0) {
        ret = (int)dwNumBytesSent;
    }
#else
    ssize_t ret = sendmsg(sockfd, msg, flags);
#endif

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

ssize_t ARSAL_Socket_Sendto(int sockfd, const void *buf, size_t buflen, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
    ssize_t ret;

    while (((ret = sendto(sockfd, buf, buflen, flags, dest_addr, addrlen)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

ssize_t ARSAL_Socket_Send(int sockfd, const void *buf, size_t buflen, int flags)
{
    ssize_t res;
    int tries = 10;
    int i;
    for (i = 0; i < tries; i++)
    {
        while (((res = send(sockfd, buf, buflen, flags)) == -1) &&
               (errno == EINTR));

        if (res >= 0 || errno != ECONNREFUSED)
        {
            break;
        }
    }

    ARSAL_Socket_Seterrno(res);
    return res;
}

#ifdef _WIN32
static GUID WSARecvMsg_GUID = WSAID_WSARECVMSG;
static LPFN_WSARECVMSG WSARecvMsg = NULL;
#endif
ssize_t ARSAL_Socket_RecvMsg(int sockfd, struct msghdr *msg, int flags)
{
#if defined(_WIN32)
    if (WSARecvMsg == 0) {
        DWORD dwBytesReturned = 0;
        int res = WSAIoctl((SOCKET)sockfd, SIO_GET_EXTENSION_FUNCTION_POINTER, &WSARecvMsg_GUID,
                           sizeof(WSARecvMsg_GUID), &WSARecvMsg, sizeof(WSARecvMsg), &dwBytesReturned,
                           NULL, NULL);
        if (res != 0) {
            errno = ENOENT;
            return -1;
        }
    }

    SOCKADDR name;
    WSAMSG wmsg;
    wmsg.name = &name;
    wmsg.namelen = sizeof(SOCKADDR);

    wmsg.lpBuffers = (LPWSABUF)alloca(sizeof(WSABUF) * msg->msg_iovlen);
    wmsg.dwBufferCount = (DWORD)msg->msg_iovlen;
    for (int i = 0; i < msg->msg_iovlen; i++) {
        wmsg.lpBuffers[i].buf = msg->msg_iov[i].iov_base;
        wmsg.lpBuffers[i].len = msg->msg_iov[i].iov_len;
    }

    wmsg.Control.buf = msg->msg_control;
    wmsg.Control.len = msg->msg_controllen;
    wmsg.dwFlags = flags;

    DWORD dwNumBytesReceived = 0;
    int ret = WSARecvMsg(sockfd, &wmsg, &dwNumBytesReceived, NULL, NULL);
    if (ret == 0) {
        ret = (int)dwNumBytesReceived;

        memcpy(msg->msg_name, wmsg.name, min(wmsg.namelen, msg->msg_namelen));
        msg->msg_namelen = min(wmsg.namelen, msg->msg_namelen);
        msg->msg_controllen = wmsg.Control.len;
        msg->msg_flags = 0;
    }
#else
    ssize_t ret = recvmsg(sockfd, msg, flags);
#endif

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

ssize_t ARSAL_Socket_Recvfrom(int sockfd, void *buf, size_t buflen, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
    ssize_t ret;

    while (((ret = recvfrom(sockfd, buf, buflen, flags, src_addr, addrlen)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

ssize_t ARSAL_Socket_Recv(int sockfd, void *buf, size_t buflen, int flags)
{
    ssize_t ret;

    while (((ret = recv(sockfd, buf, buflen, flags)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

ssize_t ARSAL_Socket_Writev (int sockfd, const struct iovec *iov, int iovcnt)
{
    ssize_t ret;

    while (((ret = writev (sockfd, iov, iovcnt)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

ssize_t ARSAL_Socket_Readv (int sockfd, const struct iovec *iov, int iovcnt)
{
    ssize_t ret;

    while (((ret = readv (sockfd, iov, iovcnt)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    ssize_t ret = bind(sockfd, addr, addrlen);

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Listen(int sockfd, int backlog)
{
    ssize_t ret = listen(sockfd, backlog);

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int ret;

    while (((ret = accept(sockfd, addr, addrlen)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Close(int sockfd)
{
    int ret;

    while (((ret = close(sockfd)) == -1) &&
           (errno == EINTR));

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    int ret = setsockopt(sockfd, level, optname, optval, optlen);

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
{
    int ret = getsockopt(sockfd, level, optname, optval, optlen);

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_Getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int ret = getsockname(sockfd, addr, addrlen);

    ARSAL_Socket_Seterrno(ret);
    return ret;
}

int ARSAL_Socket_SetBlocking(int sockfd, int blocking)
{
#ifdef HAVE_WINSOCK2_H
    /* winsock2 sets nonblocking mode, so flip the param around */
    unsigned long val = blocking != 0 ? 0 : 1;
    return (ioctlsocket(sockfd, FIONBIO, &val)) != 0 ? -1 : 0;
#else
    int flags = fcntl(sockfd, F_GETFL, 0);
    flags = (blocking == 0) ? flags | O_NONBLOCK : flags & ~O_NONBLOCK;
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
}
