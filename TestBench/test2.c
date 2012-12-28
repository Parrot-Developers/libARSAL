#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <libSAL/socket.h>
#include <libSAL/thread.h>
#include <libSAL/mutex.h>
#include <libSAL/print.h>

#define PORT        0x1234
#define HOST        "localhost"
#define MSG			"HELLO WORLD !!!"
#define DIRSIZE     8192

static sal_mutex_t mutex;
static sal_cond_t cond;

void *thread_client(void *arg)
{
    char hostname[100];
    char    dir[DIRSIZE];
	int	sd;
	struct sockaddr_in pin;
	struct hostent *hp;

	SAL_PRINT(PRINT_WARNING, "%s started\n", __FUNCTION__);
    strcpy(hostname,HOST);

    /* go find out about the desired host machine */
    if ((hp = gethostbyname(hostname)) == 0)
    {
		SAL_PRINT(PRINT_ERROR, "gethostbyname : %s\n", strerror(errno));
		return NULL;
    }

	/* fill in the socket structure with host information */
	memset(&pin, 0, sizeof(pin));
	pin.sin_family = AF_INET;
	pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	pin.sin_port = htons(PORT);

	/* grab an Internet domain socket */
	if ((sd = sal_socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_socket : %s\n", strerror(errno));
		return NULL;
	}

	/* connect to PORT on HOST */
	if (sal_connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_connect : %s\n", strerror(errno));
		return NULL;
	}

	/* send a message to the server PORT on machine HOST */
	if (sal_send(sd, MSG, strlen(MSG), 0) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_send : %s\n", strerror(errno));
		return NULL;
	}

	/* wait for a message to come back from the server */
	if (sal_recv(sd, dir, DIRSIZE, 0) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_recv : %s\n", strerror(errno));
		return NULL;
	}

	/* spew-out the results and bail out of here! */
	SAL_PRINT(PRINT_WARNING, "Message received : %s\n", dir);

	sal_close(sd);

	return NULL;
}

void *thread_server(void *arg)
{
    char     dir[DIRSIZE];  /* used for incomming dir name, and
				outgoing data */
    int 	 sd, sd_current;
    unsigned int 	 addrlen;
    struct   sockaddr_in sin;
    struct   sockaddr_in pin;

	SAL_PRINT(PRINT_WARNING, "%s started\n", __FUNCTION__);
	/* grab an Internet domain socket */
	if ((sd = sal_socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_socket : %s\n", strerror(errno));
		return NULL;
	}

	/* fill in the socket structure with host information */
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);

	/* bind the socket to the port number */
	if (sal_bind(sd, (struct sockaddr *) &sin, sizeof(sin)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_bind : %s\n", strerror(errno));
		return NULL;
	}

	/* show that we are willing to listen */
	if (sal_listen(sd, 5) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_lsiten : %s\n", strerror(errno));
		return NULL;
	}

	/* wait for a client to talk to us */
    addrlen = sizeof(pin);
	if ((sd_current = sal_accept(sd, (struct sockaddr *)&pin, &addrlen)) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_accept : %s\n", strerror(errno));
		return NULL;
	}

	/* get a message from the client */
	if (sal_recv(sd_current, dir, sizeof(dir), 0) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_recv : %s\n", strerror(errno));
		return NULL;
	}

	/* spew-out the results and bail out of here! */
	SAL_PRINT(PRINT_WARNING, "Message received : %s\n", dir);

	/* acknowledge the message, reply w/ the file names */
	if (sal_send(sd_current, dir, strlen(dir), 0) == -1)
	{
		SAL_PRINT(PRINT_ERROR, "sal_send : %s\n", strerror(errno));
		return NULL;
	}

	/* close up both sockets */
	sal_close(sd_current);
	sal_close(sd);

    /* give client a chance to properly shutdown */
    sleep(1);

	return NULL;
}

int main(int argc, char **argv)
{
	sal_thread_t server, client;

	SAL_PRINT(PRINT_ERROR, "mutex init\n");
	sal_mutex_init(&mutex);

	SAL_PRINT(PRINT_WARNING, "condition init\n");
	sal_cond_init(&cond);

	SAL_PRINT(PRINT_WARNING, "create threads\n");
	sal_thread_create(&server, thread_server, NULL);
	sal_thread_create(&client, thread_client, NULL);

	sal_thread_join(server, NULL);
	sal_thread_join(client, NULL);

	SAL_PRINT(PRINT_DEBUG, "condition destroy\n");
	sal_cond_destroy(&cond);

	SAL_PRINT(PRINT_DEBUG, "mutex destroy\n");
	sal_mutex_destroy(&mutex);

	SAL_PRINT(PRINT_DEBUG, "destroy threads\n");
	sal_thread_destroy(&server);
	sal_thread_destroy(&client);
	return 0;
}
