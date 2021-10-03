#include <unp.h>

int main()
{
	int cfd;
	struct sockaddr_in bind_addr, addr;

	if ((cfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err_sys("socket");

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) == -1)
		err_sys("inet_pton");

	bzero(&bind_addr, sizeof(bind_addr));
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(40000);
	if (inet_pton(AF_INET, "172.28.33.22", &bind_addr.sin_addr) == -1)
		err_sys("inet_pton");
	if (bind(cfd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) == -1)
		err_sys("bind");

	if (sendto(cfd, "a", 1, 0, (struct sockaddr *)&addr, sizeof(addr)) != 1)
		err_sys("sendto");

	return 0;
}