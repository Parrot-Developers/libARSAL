/**
 * @file libSAL/socket.c
 * @brief This file contains sources about socket abstraction layer
 * @date 06/06/2012
 * @author frederic.dhaeyer@parrot.com
*/
#include <config.h>
#include <stdlib.h>
#include <libSAL/socket.h>

int sal_socket(int domain, int type, int protocol)
{
	return socket(domain, type, protocol);
}

int sal_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	return connect(sockfd, addr, addrlen);
}

ssize_t sal_sendto(int sockfd, const void *buf, size_t buflen, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
	return sendto(sockfd, buf, buflen, flags, dest_addr, addrlen);
}

ssize_t sal_send(int sockfd, const void *buf, size_t buflen, int flags)
{
	return sal_sendto(sockfd, buf, buflen, flags, NULL, 0);
}

ssize_t sal_recvfrom(int sockfd, void *buf, size_t buflen, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
	return recvfrom(sockfd, buf, buflen, flags, src_addr, addrlen);
}

ssize_t sal_recv(int sockfd, void *buf, size_t buflen, int flags)
{
	return sal_recvfrom(sockfd, buf, buflen, flags, (struct sockaddr *)NULL, (socklen_t *)NULL);
}

int sal_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	return bind(sockfd, addr, addrlen);
}

int listen(int sockfd, int backlog)
{
	return listen(sockfd, backlog);
}

int sal_close(int sockfd)
{
	return close(sockfd);
}
