#include <unp.h>

int main()
{
	int sfd, readn;
	struct sockaddr_in addr;
	struct sockaddr_in from_addr;
	socklen_t len;
	char data[MAXLINE];
	char buf[MAXLINE];

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err_sys("socket");

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	if (inet_pton(AF_INET, "172.28.33.22", &addr.sin_addr) == -1)
		err_sys("inet_pton");
	if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		err_sys("bind");

	len = sizeof(from_addr);
	if ((readn = recvfrom(sfd, data, MAXLINE, 0, (struct sockaddr *)&from_addr, &len)) < 0)
		err_sys("recvfrom");

	if (write(STDOUT_FILENO, data, readn) != readn)
		err_sys("write");
	if (inet_ntop(AF_INET, &from_addr.sin_addr, buf, MAXLINE) == NULL)
		err_sys("inet_ntop");
	printf("\nfrom ip:%s\n", buf);

	len = sizeof(from_addr);
	if ((readn = recvfrom(sfd, data, MAXLINE, 0, (struct sockaddr *)&from_addr, &len)) < 0)
		err_sys("recvfrom");

	if (write(STDOUT_FILENO, data, readn) != readn)
		err_sys("write");
	if (inet_ntop(AF_INET, &from_addr.sin_addr, buf, MAXLINE) == NULL)
		err_sys("inet_ntop");
	printf("\nfrom ip:%s\n", buf);

	return 0;
}