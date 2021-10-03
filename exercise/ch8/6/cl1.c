/* 首先绑定127.0.0.1这个回环地址，测试
 * 然后绑定一个连接该NAT局域网的一个接口IP，测试
 */

#include <unp.h>

int main()
{
	int cfd;
	struct sockaddr_in addr;

	if ((cfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err_sys("socket");

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	if (inet_pton(AF_INET, "172.28.33.22", &addr.sin_addr) == -1)
		err_sys("inet_pton");

	if (sendto(cfd, "a", 1, 0, (struct sockaddr *)&addr, sizeof(addr)) != 1)
		err_sys("sendto");

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) == -1)
		err_sys("inet_pton");

	if (sendto(cfd, "b", 1, 0, (struct sockaddr *)&addr, sizeof(addr)) != 1)
		err_sys("sendto");

	return 0;
}