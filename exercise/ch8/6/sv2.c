#include <unp.h>

int main()
{
	int sfd, readn;
	struct sockaddr_in addr;
	struct sockaddr_in conn_addr;
	char data[MAXLINE];

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err_sys("socket");

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	if (inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr) == -1)
		err_sys("inet_pton");
	if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		err_sys("bind");

	// for (;;)
	// {
	// 	if ((readn = recvfrom(sfd, data, MAXLINE, 0, NULL, NULL)) < 0)
	// 		err_sys("read");
	// 	if (write(STDOUT_FILENO, data, readn) != readn)
	// 		err_sys("write");
	// }

	bzero(&conn_addr, sizeof(conn_addr));
	conn_addr.sin_family = AF_INET;
	conn_addr.sin_port = htons(40000);
	if (inet_pton(AF_INET, "172.28.33.22", &conn_addr.sin_addr) == -1)
		err_sys("inet_pton");
	if (connect(sfd, (struct sockaddr *)&conn_addr, sizeof(conn_addr)) == -1)
		err_sys("connect");

	for (;;)
	{
		if ((readn = read(sfd, data, MAXLINE)) < 0)
			err_sys("read");
		if (write(STDOUT_FILENO, data, readn) != readn)
			err_sys("write");
	}

	return 0;
}