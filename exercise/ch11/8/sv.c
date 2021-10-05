#include <unp.h>
#include <time.h>

int main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t len, addrlen;
	char buff[MAXLINE];
	time_t ticks;
	struct sockaddr_storage cliaddr;
	int err;
	char host[NI_MAXHOST], serv[NI_MAXSERV];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: daytimetcpsrv2 [ <host> ] <service or port>");

	for (;;)
	{
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &len);
		// printf("connection from %s\n", Sock_ntop((SA *)&cliaddr, len));
		if ((err = getnameinfo((SA *)&cliaddr, len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV)) != 0)
		{
			printf("getnameinfo failed: %s\n", gai_strerror(err));
			exit(EXIT_FAILURE);
		}
		printf("Connected from %s:%s.\n", host, serv);

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
