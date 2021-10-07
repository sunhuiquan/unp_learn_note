#include <unp.h>
#include <netinet/tcp.h> // for TCP_MAXSEG

union sockopt_val
{
	int i_val;
	long l_val;
	//另外两种返回类型懒得写了
};

int main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;
	union sockopt_val val;
	socklen_t len;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	len = sizeof(val);
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &val, &len) == -1)
		err_sys("getsockopt");
	printf("TCP default MSS: %d\n", val.i_val);
	len = sizeof(val);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &val, &len) == -1)
		err_sys("getsockopt");
	printf("TCP receive buffer default size: %d\n", val.i_val);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13); /* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	len = sizeof(val);
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &val, &len) == -1)
		err_sys("getsockopt");
	printf("TCP after connected MSS: %d\n", val.i_val);
	len = sizeof(val);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &val, &len) == -1)
		err_sys("getsockopt");
	printf("TCP receive buffer after connected size: %d\n", val.i_val);

	while ((n = read(sockfd, recvline, MAXLINE)) > 0)
	{
		recvline[n] = 0; /* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}
