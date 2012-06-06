/**
 * @file libSAL/socket.c
 * @brief This file contains sources about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
*/
#include <config.h>
#include <libSAL/socket.h>

#if defined(HAVE_SYS_SOCKET_H)
#include <sys/socket.h>
#endif

#if defined(HAVE_NETINET_IN_H)
#include <netinet/in.h>
#endif

int sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, int protocol)
{
	int result = -1;
	int _domain = (int)SAL_SOCK_DOMAIN_MAX;
	int _type = (int)SAL_SOCK_TYPE_MAX;

	switch(domain)
	{
		case SAL_SOCK_DOMAIN_INET:
#if defined(HAVE_SYS_SOCKET_H)
			_domain = AF_INET;
#endif
			break;

		case SAL_SOCK_DOMAIN_INET6:
#if defined(HAVE_SYS_SOCKET_H)
			_domain = AF_INET6;
#endif
			break;

		default:
			break;
	}

	switch(type)
	{
		case SAL_SOCK_TYPE_STREAM:
#if defined(HAVE_SYS_SOCKET_H)
			_domain = SOCK_STREAM;
#endif
			break;

		case SAL_SOCK_TYPE_DGRAM:
#if defined(HAVE_SYS_SOCKET_H)
			_domain = SOCK_DGRAM;
#endif
			break;

		default:
			break;
	}

	if((_domain != SAL_SOCK_DOMAIN_MAX) && (_type != SAL_SOCK_TYPE_MAX))
	{
#if defined(HAVE_SYS_SOCKET_H)
		result = socket(_domain, _type, protocol);
#endif
	}

	return result;
}

int sal_connect(int sockfd, const struct sockaddr *addr, int addrlen)
{
	int result = -1;

#if defined(HAVE_SYS_SOCKET_H)
	result = connect(sockfd, addr, (socklen_t)addrlen);
#endif

	return result;
}

int sal_sendto(int sockfd, const void *buf, int buflen, int flags, const struct sockaddr *dest_addr, int addrlen)
{
	int result = -1;

#if defined(HAVE_SYS_SOCKET_H)
	result = (int)sendto(sockfd, buf, (size_t)buflen, flags, dest_addr, (socklen_t)addrlen);
#endif

	return result;
}

int sal_recvfrom(int sockfd, void *buf, int buflen, int flags, struct sockaddr *src_addr, int *addrlen)
{
	int result = -1;

#if defined(HAVE_SYS_SOCKET_H)
	result = (int)recvfrom(sockfd, buf, (size_t)buflen, flags, src_addr, (socklen_t *)addrlen);
#endif

	return result;
}

int sal_close(int sockfd)
{
	int result = -1;

#if defined(HAVE_SYS_SOCKET_H)
	result = close(sockfd);
#endif

	return result;

}
