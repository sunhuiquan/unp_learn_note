#include <unp.h>

#define MAX_IPV4_DATAGRAM 65507

int main()
{
	int cfd;
	struct sockaddr_in addr;
	char data[MAX_IPV4_DATAGRAM];

	if ((cfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err_sys("socket");

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) == -1)
		err_sys("inet_pton");

	// 直接把里面的未定义值发过去行了，反正二进制字节流又不会被解释
	if (sendto(cfd, data, MAX_IPV4_DATAGRAM, 0, (struct sockaddr *)&addr, sizeof(addr)) != MAX_IPV4_DATAGRAM)
		err_sys("sendto");
	printf("send 65507 successfully\n");

	if (sendto(cfd, data, MAX_IPV4_DATAGRAM + 1, 0, (struct sockaddr *)&addr, sizeof(addr)) != MAX_IPV4_DATAGRAM)
	{
		printf("send 65508 in fail\n");
		printf("errno message: %s\n", strerror(errno));
	}

	return 0;
}