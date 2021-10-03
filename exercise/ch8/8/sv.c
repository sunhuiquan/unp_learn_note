#include <unp.h>

#define MAX_IPV4_DATAGRAM 65507

int main()
{
	int sfd, readn;
	struct sockaddr_in addr;
	struct sockaddr_in from_addr;
	socklen_t len;
	char data[MAX_IPV4_DATAGRAM];

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err_sys("socket");

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	if (inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr) == -1)
		err_sys("inet_pton");

	if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		err_sys("bind");

	len = sizeof(from_addr);
	if ((readn = recvfrom(sfd, data, MAX_IPV4_DATAGRAM, 0, (struct sockaddr *)&from_addr, &len)) < 0)
		err_sys("recvfrom");
	printf("actually read: %d\n", readn);

	len = sizeof(from_addr);
	if ((readn = recvfrom(sfd, data, MAX_IPV4_DATAGRAM, 0, (struct sockaddr *)&from_addr, &len)) < 0)
		err_sys("recvfrom");
	printf("actually read: %d\n", readn);

	return 0;
}