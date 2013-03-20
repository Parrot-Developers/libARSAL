/**
 * @file libARSAL/ARSAL_Socket.c
 * @brief This file contains sources about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
 */
#include <config.h>
#include <stdlib.h>
#include <unistd.h>
#include <libARSAL/ARSAL_Socket.h>

int ARSAL_Socket_Create(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}

int ARSAL_Socket_Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return connect(sockfd, addr, addrlen);
}

ssize_t ARSAL_Socket_Sendto(int sockfd, const void *buf, size_t buflen, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
    return sendto(sockfd, buf, buflen, flags, dest_addr, addrlen);
}

ssize_t ARSAL_Socket_Send(int sockfd, const void *buf, size_t buflen, int flags)
{
    return ARSAL_Socket_Sendto(sockfd, buf, buflen, flags, NULL, 0);
}

ssize_t ARSAL_Socket_Recvfrom(int sockfd, void *buf, size_t buflen, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
    return recvfrom(sockfd, buf, buflen, flags, src_addr, addrlen);
}

ssize_t ARSAL_Socket_Recv(int sockfd, void *buf, size_t buflen, int flags)
{
    return ARSAL_Socket_Recvfrom(sockfd, buf, buflen, flags, (struct sockaddr *)NULL, (socklen_t *)NULL);
}

ssize_t ARSAL_Socket_Writev (int sockfd, const struct iovec *iov, int iovcnt)
{
    return writev (sockfd, iov, iovcnt);
}

ssize_t ARSAL_Socket_Readv (int sockfd, const struct iovec *iov, int iovcnt)
{
    return readv (sockfd, iov, iovcnt);
}

int ARSAL_Socket_Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return bind(sockfd, addr, addrlen);
}

int ARSAL_Socket_Listen(int sockfd, int backlog)
{
    return listen(sockfd, backlog);
}

int ARSAL_Socket_Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return accept(sockfd, addr, addrlen);
}

int ARSAL_Socket_Close(int sockfd)
{
    return close(sockfd);
}

int ARSAL_Socket_Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    return setsockopt(sockfd, level, optname, optval, optlen);
}
