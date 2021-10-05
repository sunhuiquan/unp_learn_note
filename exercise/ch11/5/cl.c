#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAXLINE 1024

int main(int argc, char *argv[])
{
	int sfd, err, readn;
	struct addrinfo hints, *res, *reserve;
	char buf[MAXLINE];

	if (argc != 2)
	{
		printf("%s usage: <host>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	bzero(&hints, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((err = getaddrinfo(argv[1], "daytime", &hints, &res)) != 0)
	{
		printf("getaddrinfo failed: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}
	reserve = res;

	for (; res; res = res->ai_next)
	{
		if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
		{
			printf("socket() failed, and continue\n");
			continue;
		}

		if (connect(sfd, res->ai_addr, res->ai_addrlen) == 0)
			break;
		printf("connect() failed, close() it and continue\n");
		close(sfd);
	}
	if (res == NULL)
	{
		printf("can't connect even one host\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		readn = read(sfd, buf, MAXLINE);
		if (readn < 0)
		{
			printf("read failed\n");
			exit(EXIT_FAILURE);
		}
		else if (readn == 0)
			break;

		if (write(STDOUT_FILENO, buf, readn) != readn)
		{
			printf("write failed\n");
			exit(EXIT_FAILURE);
		}
	}

	freeaddrinfo(reserve);
	return 0;
}