#include	<time.h>
#include	<unp.h>

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t addrlen, len;
	struct sockaddr	*cliaddr;
	char buff[MAXLINE];
	time_t ticks;

	if (argc > 2)
		err_quit("usage: daytimetcpsrv2 [ <host> ]");

	// daemon_init(argv[0], 0);

	if (argc == 1)
		listenfd = Tcp_listen(NULL, "9999", &addrlen);
	else
		listenfd = Tcp_listen(argv[1], "9999", &addrlen);

	cliaddr = Malloc(addrlen);

	for ( ; ; ) {
		len = addrlen;
		connfd = Accept(listenfd, cliaddr, &len);
		err_msg("connection from %s", Sock_ntop(cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		int sz = strlen(buff);
		for(int i = 0;i < sz;++i)
			Write(connfd, &buff[i], sizeof(char));

		Close(connfd);
	}
}
