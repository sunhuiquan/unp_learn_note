#include <unp.h>

void sigpipe_handler(int sig)
{
	int saved = errno;
	write(STDOUT_FILENO, "get SIGPIPE\n", 12);
	errno = saved;
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	struct sigaction act;
	time_t tm;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13); // daytime service
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	act.sa_handler = sigpipe_handler;
	if (sigaction(SIGPIPE, &act, NULL) == -1)
		err_quit("sigaction");

	Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

	sleep(5);
	tm = time(NULL);
	printf("%s\n", ctime(&tm));
	if (write(sockfd, "a", 1) == -1) // get RST response
		err_quit("write");

	sleep(5);
	tm = time(NULL);
	printf("%s\n", ctime(&tm));
	if (write(sockfd, "a", 1) == -1) // cause SIGPIPE signal, and return -1 if not be killed by SIGPIPE
		err_quit("write");

	exit(0);
}
