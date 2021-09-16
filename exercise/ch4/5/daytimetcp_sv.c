#include <unp.h>
#include <time.h>

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	struct sockaddr_in addr;
	socklen_t len;
	char buff[MAXLINE];
	time_t ticks;
	char *pstr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999); /* to avoid permiting assessment */

	// Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

	len = sizeof(addr);
	if (getsockname(listenfd, (struct sockaddr *)&addr, &len) == -1)
		err_sys("getsockname");
	if ((pstr = sock_ntop((struct sockaddr *)&addr, len)) == NULL)
		err_sys("sock_ntop");
	printf("%s\n", pstr);

	Listen(listenfd, LISTENQ);

	len = sizeof(addr);
	if (getsockname(listenfd, (struct sockaddr *)&addr, &len) == -1)
		err_sys("getsockname");
	if ((pstr = sock_ntop((struct sockaddr *)&addr, len)) == NULL)
		err_sys("sock_ntop");
	printf("%s\n", pstr);

	for (;;)
	{
		connfd = Accept(listenfd, (SA *)NULL, NULL);

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
