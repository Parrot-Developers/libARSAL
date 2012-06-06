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

sal_socket_t sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, int protocol)
{
	sal_socket_t result = -1;
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
		result = (sal_socket_t)socket(_domain, _type, protocol);
#endif
	}

	return result;
}
