#include <unp.h>

int main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;
	struct sockaddr_in addr; // IPv4 socket address type
	socklen_t len;
	char *pinet_str;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13); /* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	// connect 建立连接之前，内核还没给这个socket隐式绑定一个ip和port
	// len = sizeof(addr);
	// if (getsockname(sockfd, (struct sockaddr *)&addr, &len) == -1)
	// 	err_sys("getsockname");
	// if ((pinet_str = sock_ntop((struct sockaddr *)&addr, len)) == NULL)
	// 	err_sys("sock_ntop");
	// printf("%s\n", pinet_str);

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	// connect 建立连接之后，内核会给这个socket隐式绑定一个ip和port
	len = sizeof(addr);
	if (getsockname(sockfd, (struct sockaddr *)&addr, &len) == -1)
		err_sys("getsockname");
	if ((pinet_str = sock_ntop((struct sockaddr *)&addr, len)) == NULL)
		err_sys("sock_ntop");
	printf("%s\n", pinet_str);

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
