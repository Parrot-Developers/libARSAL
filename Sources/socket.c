/**
 * @file libSAL/socket.c
 * @brief This file contains sources about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
*/
#include <config.h>
#include <stdlib.h>
#include <libSAL/socket.h>

#if defined(HAVE_SYS_SOCKET_H)
#include <sys/socket.h>
#endif

#if defined(HAVE_NETINET_IN_H)
#include <netinet/in.h>
#endif

static int sal_convert_flags(int flags)
{
	int result = 0;

	if(flags & SAL_SOCK_FLAG_DONTWAIT)
	{
#if defined(HAVE_SYS_SOCKET_H)
		result |= MSG_DONTWAIT;
#endif
	}

	if(flags & SAL_SOCK_FLAG_WAITALL)
	{
#if defined(HAVE_SYS_SOCKET_H)
		result |= MSG_WAITALL;
#endif
	}

	if(flags & SAL_SOCK_FLAG_NOSIGNAL)
	{
#if defined(HAVE_SYS_SOCKET_H)
		result |= MSG_NOSIGNAL;
#endif
	}

	return result;
}

static int sal_convert_types(eSAL_SOCK_TYPE type)
{
	int result = (int)SAL_SOCK_TYPE_MAX;

	switch(type)
	{
		case SAL_SOCK_TYPE_STREAM:
#if defined(HAVE_SYS_SOCKET_H)
			result = SOCK_STREAM;
#endif
			break;

		case SAL_SOCK_TYPE_DGRAM:
#if defined(HAVE_SYS_SOCKET_H)
			result = SOCK_DGRAM;
#endif
			break;

		default:
			break;
	}

	return result;
}

static int sal_convert_domains(eSAL_SOCK_DOMAIN domain)
{
	int result =(int)SAL_SOCK_DOMAIN_MAX;

	switch(domain)
	{
		case SAL_SOCK_DOMAIN_INET:
#if defined(HAVE_SYS_SOCKET_H)
			result = AF_INET;
#endif
			break;

		case SAL_SOCK_DOMAIN_INET6:
#if defined(HAVE_SYS_SOCKET_H)
			result = AF_INET6;
#endif
			break;

		default:
			break;
	}

	return result;
}

static int sal_convert_protocols(eSAL_SOCK_PROTOCOL protocol)
{
	int result =(int)SAL_SOCK_PROTO_MAX;

	switch(protocol)
	{
		case SAL_SOCK_PROTO_TCP:
#if defined(HAVE_SYS_SOCKET_H)
			result = IPPROTO_TCP;
#endif
			break;

		case SAL_SOCK_PROTO_UDP:
#if defined(HAVE_SYS_SOCKET_H)
			result = IPPROTO_UDP;
#endif
			break;

		default:
			break;
	}

	return result;
}

int sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, eSAL_SOCK_PROTOCOL protocol)
{
	int result = -1;
	int _domain = sal_convert_domain(domain);
	int _type = sal_convert_type(type);
	int _protocol = sal_convert_protocols(protocol);

	if( (_domain != (int)SAL_SOCK_DOMAIN_MAX) &&
		(_type != (int)SAL_SOCK_TYPE_MAX) &&
		(_protocol != (int)SAL_SOCK_PROTO_MAX))
	{
#if defined(HAVE_SYS_SOCKET_H)
		result = socket(_domain, _type, _protocol);
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
	int _flags = sal_convert_flags(flags);

#if defined(HAVE_SYS_SOCKET_H)
	result = (int)sendto(sockfd, buf, (size_t)buflen, _flags, dest_addr, (socklen_t)addrlen);
#endif

	return result;
}

int sal_send(int sockfd, const void *buf, int buflen, int flags)
{
	return sal_sendto(sockfd, buf, buflen, flags, NULL, 0);
}

int sal_recvfrom(int sockfd, void *buf, int buflen, int flags, struct sockaddr *src_addr, int *addrlen)
{
	int result = -1;
	int _flags = sal_convert_flags(flags);

#if defined(HAVE_SYS_SOCKET_H)
	result = (int)recvfrom(sockfd, buf, (size_t)buflen, _flags, src_addr, (socklen_t *)addrlen);
#endif

	return result;
}

int sal_recv(int sockfd, void *buf, int buflen, int flags)
{
	return sal_recvfrom(sockfd, buf, buflen, flags, NULL, 0);
}

int sal_close(int sockfd)
{
	int result = -1;

#if defined(HAVE_SYS_SOCKET_H)
	result = close(sockfd);
#endif

	return result;

}
