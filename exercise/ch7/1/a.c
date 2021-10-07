#include <unp.h>

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(40000);
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
		err_sys("bind");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, "136.126.63.221", &servaddr.sin_addr);

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) == -1)
		err_sys("connect");

	char ch;
	if (read(sockfd, &ch, 1) < 0)
		err_sys("read");

	exit(0);
}
