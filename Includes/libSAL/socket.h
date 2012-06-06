/**
 * @file libSAL/socket.h
 * @brief This file contains headers about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
*/
#ifndef _SOCKET_H_
#define _SOCKET_H_

typedef int sal_socket_t;

typedef enum
{
	SAL_SOCK_DOMAIN_INET,
	SAL_SOCK_DOMAIN_INET6,
	SAL_SOCK_DOMAIN_MAX
} eSAL_SOCK_DOMAIN;

typedef enum
{
	SAL_SOCK_TYPE_STREAM,
	SAL_SOCK_TYPE_DGRAM,
	SAL_SOCK_TYPE_MAX
} eSAL_SOCK_TYPE;

/**
 * @fn sal_socket_t sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, int protocol)
 * @brief Creates an endpoint for communication and returns a descriptor.
 *
 * @param domain The communication domain
 * @param type The communication semantics.
 * @param protocol A particular protocol to be used with the socket
 * @retval On success, a file descriptor for the new socket is returned. On error, -1 is returned, and errno is set appropriately. (See errno.h)
 */
sal_socket_t sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, int protocol);

#endif // _SOCKET_H_
