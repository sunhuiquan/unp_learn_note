#include <unp.h>

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(19);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

	// 第二次测试使用SHUTDOWN(SHUT_RD)
	shutdown(sockfd, SHUT_RD);
	printf("shutdown\n");

	char ch;
	printf("%ld\n", read(sockfd, &ch, 1));

	pause();

	exit(0);
}
