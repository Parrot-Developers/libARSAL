/**
 * @file libSAL/socket.h
 * @brief This file contains headers about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
*/
#ifndef _SOCKET_H_
#define _SOCKET_H_

struct sockaddr;

/**
 * @enum eSAL_SOCK_DOMAIN
 * @brief Socket domains
*/
typedef enum
{
	SAL_SOCK_DOMAIN_INET,
	SAL_SOCK_DOMAIN_INET6,
	SAL_SOCK_DOMAIN_MAX
} eSAL_SOCK_DOMAIN;

/**
 * @enum eSAL_SOCK_TYPE
 * @brief Socket types
*/
typedef enum
{
	SAL_SOCK_TYPE_STREAM,
	SAL_SOCK_TYPE_DGRAM,
	SAL_SOCK_TYPE_MAX
} eSAL_SOCK_TYPE;

/**
 * @fn int sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, int protocol)
 * @brief Creates an endpoint for communication and returns a descriptor.
 *
 * @param domain The communication domain
 * @param type The communication semantics.
 * @param protocol A particular protocol to be used with the socket
 * @retval On success, a file descriptor for the new socket is returned. On error, -1 is returned, and errno is set appropriately. (See errno.h)
 */
int sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, int protocol);

/**
 * @fn int sal_connect(int sockfd, const struct sockaddr *addr, int addrlen)
 * @brief Initiate a connection on a socket,
 * If the socket sock is of type SOCK_DGRAM then addr is the address to which datagrams are sent by default, and the only address from which datagrams are received.
 * If the socket is of  type  SOCK_STREAM  or  SOCK_SEQPACKET, this call attempts to make a connection to the socket that is bound to the address specified by addr.
 * @param sock The socket to connect
 * @param addr The address to connect.
 * @param addrlen The size of the addr
 * @retval On success, 0 is returned. Otherwise, -1 is returned and errno is set appropriately. (See errno.h)
 */
int sal_connect(int sockfd, const struct sockaddr *addr, int addrlen);

#endif // _SOCKET_H_
