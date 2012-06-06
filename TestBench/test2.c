#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <libSAL/socket.h>
#include <libSAL/thread.h>
#include <libSAL/mutex.h>
#include <libSAL/print.h>

#define PORT        0x1234
#define HOST        "www.google.fr"
#define MSG			"HELLO WORLD !!!"
#define DIRSIZE     8192

static sal_mutex_t mutex;
static sal_cond_t cond;

void *thread_recv_routine(void *arg)
{
    char hostname[100];
    char    dir[DIRSIZE];
    int	sd;
    struct sockaddr_in sin;
    struct sockaddr_in pin;
    struct hostent *hp;

	SAL_PRINT(PRINT_WARNING, "%s started\n", __FUNCTION__);
	strcpy(hostname, HOST);
	/* go find out about the desired host machine */
	if ((hp = gethostbyname(hostname)) == 0)
	{
		/* spew-out the results and bail out of here! */
		SAL_PRINT(PRINT_ERROR, "%s\n", strerror(errno));
		return;
	}

	/* fill in the socket structure with host information */
	memset(&pin, 0, sizeof(pin));
	pin.sin_family = AF_INET;
	pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	pin.sin_port = htons(PORT);

	/* grab an Internet domain socket */
	if ((sd = sal_socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "%s\n", strerror(errno));
		return;
	}

	/* connect to PORT on HOST */
	if (sal_connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "%s\n", strerror(errno));
		return;
	}

	/* send a message to the server PORT on machine HOST */
	if (sal_send(sd, MSG, strlen(MSG), 0) == -1) {
		SAL_PRINT(PRINT_ERROR, "%s\n", strerror(errno));
		return;
	}

	/* wait for a message to come back from the server */
	if (sal_recv(sd, dir, DIRSIZE, 0) == -1) {
		SAL_PRINT(PRINT_ERROR, "%s\n", strerror(errno));
		return;
	}

	/* spew-out the results and bail out of here! */
	SAL_PRINT(PRINT_WARNING, "%s\n", dir);

	sal_close(sd);
}

void *thread_send_routine(void *arg)
{
	int sd;
	SAL_PRINT(PRINT_WARNING, "%s started\n", __FUNCTION__);

	/* grab an Internet domain socket */
	if ((sd = sal_socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "%s\n", strerror(errno));
		return;
	}


}

int main(int argc, char **argv)
{
	sal_thread_t thread_recv, thread_send;

	SAL_PRINT(PRINT_ERROR, "mutex init\n");
	sal_mutex_init(&mutex);

	SAL_PRINT(PRINT_WARNING, "condition init\n");
	sal_cond_init(&cond);

	SAL_PRINT(PRINT_WARNING, "create threads\n");
	sal_thread_create(&thread_send, thread_send_routine, NULL);
	sal_thread_create(&thread_recv, thread_recv_routine, NULL);

	sal_thread_join(thread_recv, NULL);
	sal_thread_join(thread_send, NULL);

	SAL_PRINT(PRINT_DEBUG, "condition destroy\n");
	sal_cond_destroy(&cond);

	SAL_PRINT(PRINT_DEBUG, "mutex destroy\n");
	sal_mutex_destroy(&mutex);

	SAL_PRINT(PRINT_DEBUG, "destroy threads\n");
	sal_thread_destroy(&thread_send);
	sal_thread_destroy(&thread_recv);
	return 0;
}
