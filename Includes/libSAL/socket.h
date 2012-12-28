/**
 * @file libSAL/socket.h
 * @brief This file contains headers about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
 */
#ifndef _SAL_SOCKET_H_
#define _SAL_SOCKET_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * @brief Creates an endpoint for communication and returns a descriptor.
 *
 * @param domain The communication domain
 * @param type The communication semantics.
 * @param protocol A protocol to be used with the socket
 * @retval On success, a file descriptor for the new socket is returned. On error, -1 is returned, and errno is set appropriately. (See errno.h)
 */
int sal_socket(int domain, int type, int protocol);

/**
 * @brief Initiate a connection on a socket,
 * If the socket sock is of type SOCK_DGRAM then addr is the address to which datagrams are sent by default, and the only address from which datagrams are received.
 * If the socket is of type SAL_SOCK_STREAM, this call attempts to make a connection to the socket that is bound to the address specified by addr.
 *
 * @param sockfd The socket descriptor used to connect
 * @param addr The address to connect.
 * @param addrlen The size of the addr
 * @retval On success, 0 is returned. Otherwise, -1 is returned and errno is set appropriately. (See errno.h)
 */
int sal_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brief Transmit a message on a socket
 * If sal_sendto() is used on a connection-mode (SOCK_STREAM) socket, the arguments dest_addr and addrlen are ignored (and the error EISCONN may be returned when they are not NULL and 0)
 * Otherwise, the target address is given by dest_addr with addrlen specifying its size.
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to send
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the socket flags.
 * @param dest_addr The target address
 * @param addrlen The size of the target address
 *
 * @retval On success, 0 is returned. Otherwise, -1 is returned and errno is set appropriately. (See errno.h)
 */
ssize_t sal_sendto(int sockfd, const void *buf, size_t buflen, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

/**
 * @brief Transmit a message on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to send
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the socket flags
 *
 * @retval On success, 0 is returned. Otherwise, -1 is returned and errno is set appropriately. (See errno.h)
 */
ssize_t sal_send(int sockfd, const void *buf, size_t buflen, int flags);

/**
 * @brief Receive a message on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to fill with data received
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the socket flags
 * @param src_addr The source address
 * @param addrlen The size of the source address
 *
 * @retval On success, the number of bytes received is returned. 0 is returned when the peer has performed an orderly shutdow.
 * Otherwise -1 is returned if an error occurred and errno is set appropriately. (See errno.h).
 */
ssize_t sal_recvfrom(int sockfd, void *buf, size_t buflen, int flags, struct sockaddr *src_addr, socklen_t *addrlen);

/**
 * @brief Receive a message on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param buf The buffer to fill with data received
 * @param buflen The buffer size
 * @param flags The bitwise OR of zero or more of the socket flags
 *
 * @retval On success, the number of bytes received is returned. 0 is returned when the peer has performed an orderly shutdow.
 * Otherwise -1 is returned if an error occurred and errno is set appropriately. (See errno.h).
 */
ssize_t sal_recv(int sockfd, void *buf, size_t buflen, int flags);


/**
 * @brief Bind a name to a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param addr The address
 * @param addrlen The size of the address
 *
 * @retval On succes, 0 is returned. Otherwise -1 is returned if an error occurred and errno is set appropriately. (See errno.h).
 */
int sal_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brief Listen for connections on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param backlog The maximum length to which the queue of pending connections for sockfd may grow
 *
 * @retval On succes, 0 is returned. Otherwise -1 is returned if an error occurred and errno is set appropriately. (See errno.h).
 */
int sal_listen(int sockfd, int backlog);

/**
 * @brief Accept a connection on a socket
 *
 * @param sockfd The socket descriptor used to send
 * @param addr The address
 * @param addrlen The size of address
 *
 * @retval On succes, a nonnegative integer is returned. Otherwise -1 is returned if an error occurred and errno is set appropriately. (See errno.h).
 */
int sal_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
 * @brief Close a socket
 *
 * @param sockfd The socket to close
 * @retval On success, 0 is returned. Otherwise, -1 is returned, and errno is set appropriately. (See errno.h)
 */
int sal_close(int sockfd);

/**
 * @brief Set the socket options
 *
 * @param sockfd The socket to set options
 * @param level the protocol level
 * @param optname the option name
 * @param optval the option value
 * @param optlen the length of the option value
 * @retval On success, 0 is returned. Otherwise, -1 is returned, and errno is set appropriately. (See errno.h)
 */
int sal_setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);

#endif // _SAL_SOCKET_H_
