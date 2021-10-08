#include <unp.h>
#include <time.h>

int udp_server2(const char *host, const char *serv, socklen_t *addrlenp)
{
	int sockfd, n;
	struct addrinfo hints, *res, *ressave;
	int val;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_quit("udp_server error for %s, %s: %s",
				 host, serv, gai_strerror(n));
	ressave = res;

	val = 1;
	do
	{
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0)
			continue; /* error - try next one */

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
			err_sys("setsockopt");

		if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break; /* success */

		Close(sockfd); /* bind error - close and try next one */
	} while ((res = res->ai_next) != NULL);

	if (res == NULL) /* errno from final socket() or bind() */
		err_sys("udp_server error for %s, %s", host, serv);

	if (addrlenp)
		*addrlenp = res->ai_addrlen; /* return size of protocol address */

	freeaddrinfo(ressave);

	return (sockfd);
}
/* end udp_server */

int Udp_server2(const char *host, const char *serv, socklen_t *addrlenp)
{
	return (udp_server2(host, serv, addrlenp));
}

int main(int argc, char **argv)
{
	int sockfd;
	char buff[MAXLINE];
	time_t ticks;
	socklen_t len;
	struct sockaddr_storage cliaddr;

	if (argc == 2)
		sockfd = Udp_server2(NULL, argv[1], NULL);
	else if (argc == 3)
		sockfd = Udp_server2(argv[1], argv[2], NULL);
	else
		err_quit("usage: daytimeudpsrv [ <host> ] <service or port>");

	for (;;)
	{
		len = sizeof(cliaddr);
		Recvfrom(sockfd, buff, MAXLINE, 0, (SA *)&cliaddr, &len);
		printf("datagram from %s\n", Sock_ntop((SA *)&cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Sendto(sockfd, buff, strlen(buff), 0, (SA *)&cliaddr, len);
	}
}
