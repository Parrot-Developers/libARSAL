/**
 * @file libSAL/socket.h
 * @brief This file contains headers about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
*/
#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <sys/types.h>

/**
 * @brief Structure used to store most addresses.
 */
struct sockaddr;

/**
 * @brief Socket address, internet style.
 */
struct sockaddr_in;

/**
 * @brief Socket domains
*/
typedef enum
{
	SAL_SOCK_DOMAIN_INET,
	SAL_SOCK_DOMAIN_INET6,
	SAL_SOCK_DOMAIN_MAX
} eSAL_SOCK_DOMAIN;

/**
 * @brief Socket types
*/
typedef enum
{
	SAL_SOCK_TYPE_STREAM,
	SAL_SOCK_TYPE_DGRAM,
	SAL_SOCK_TYPE_MAX
} eSAL_SOCK_TYPE;

/**
 * @brief Creates an endpoint for communication and returns a descriptor.
 *
 * @param domain The communication domain
 * @param type The communication semantics.
 * @param protocol A particular protocol to be used with the socket
 * @retval On success, a file descriptor for the new socket is returned. On error, -1 is returned, and errno is set appropriately. (See errno.h)
 */
int sal_socket(eSAL_SOCK_DOMAIN domain, eSAL_SOCK_TYPE type, int protocol);

/**
 * @brief Initiate a connection on a socket,
 * If the socket sock is of type SOCK_TYPE_DGRAM then addr is the address to which datagrams are sent by default, and the only address from which datagrams are received.
 * If the socket is of type SAL_SOCK_TYPE_STREAM, this call attempts to make a connection to the socket that is bound to the address specified by addr.
 *
 * @param sockfd The socket descriptor used to connect
 * @param addr The address to connect.
 * @param addrlen The size of the addr
 * @retval On success, 0 is returned. Otherwise, -1 is returned and errno is set appropriately. (See errno.h)
 */
int sal_connect(int sockfd, const struct sockaddr *addr, int addrlen);

/**
 * @brief Transmit a message on a socket
 * If sal_sendto() is used on a connection-mode (SAL_SOCK_TYPE_STREAM) socket, the arguments dest_addr and addrlen are ignored (and the error EISCONN may be returned when they are not NULL and 0)
 * Otherwise, the target address is given by dest_addr with addrlen specifying its size.
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to send
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the flags SAL_SOCK_FLAGS
 * @param dest_addr The target address
 * @param addrlen The size of the target address
 *
 * @retval On success, 0 is returned. Otherwise, -1 is returned and errno is set appropriately. (See errno.h)
 */
int sal_sendto(int sockfd, const void *buf, int buflen, int flags, const struct sockaddr *dest_addr, int addrlen);

/**
 * @brief Transmit a message on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to send
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the flags SAL_SOCK_FLAGS
 *
 * @retval On success, 0 is returned. Otherwise, -1 is returned and errno is set appropriately. (See errno.h)
 */
int sal_send(int sockfd, const void *buf, int buflen, int flags);

/**
 * @brief Receive a message on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to fill with data received
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the flags SAL_SOCK_FLAGS
 * @param src_addr The source address
 * @param addrlen The size of the source address
 *
 * @retval On success, the number of bytes received is returned. 0 is returned when the peer has performed an orderly shutdow.
 * Otherwise -1 is returned if an error occurred and errno is set appropriately. (See errno.h).
 */
int sal_recvfrom(int sockfd, void *buf, int buflen, int flags, struct sockaddr *src_addr, int *addrlen);

/**
 * @brief Receive a message on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to fill with data received
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the flags SAL_SOCK_FLAGS
 *
 * @retval On success, the number of bytes received is returned. 0 is returned when the peer has performed an orderly shutdow.
 * Otherwise -1 is returned if an error occurred and errno is set appropriately. (See errno.h).
 */
int sal_recv(int sockfd, void *buf, int buflen, int flags);

/**
 * @brief Close a socket
 *
 * @param sockfd The socket to close
 * @retval On success, 0 is returned. Otherwise, -1 is returned, and errno is set appropriately. (See errno.h)
 */
int sal_close(int sockfd);

#endif // _SOCKET_H_
