#include	<unp.h>

int
main(int argc, char **argv)
{
	int count = 0;
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr_storage	ss;

	if (argc != 2)
		err_quit("usage: daytimetcpcli <hostname/IPaddress>");

	sockfd = Tcp_connect(argv[1],"9999");

	len = sizeof(ss);
	Getpeername(sockfd, (SA *)&ss, &len);
	printf("connected to %s\n", Sock_ntop_host((SA *)&ss, len));

	while ( (n = Read(sockfd, recvline, MAXLINE)) > 0) {
		++count;
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
	printf("Read %d times\n",count);
	exit(0);
}
