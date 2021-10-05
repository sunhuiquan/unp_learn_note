#include <unp.h>

static int is_timeout;
static void sigalarm_handler(int sig)
{
	int saved = errno;
	is_timeout = 1;
	errno = saved;
}

int getnameinfo_timeout(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags, int timeout)
{
	struct sigaction act, old; // old 用于离开时恢复对SIGALARM的原先处理方式

	if (timeout < 0)
	{
		errno = EINVAL;
		return -1;
	}

	if (timeout > 0)
	{
		is_timeout = 0;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		act.sa_handler = sigalarm_handler;
		if (sigaction(SIGALRM, &act, &old) == -1)
			return -1;
		alarm(timeout);
	}

	if (getnameinfo(sa, salen, host, hostlen, serv, servlen, flags) != 0)
	{
		if (timeout > 0)
			if (sigaction(SIGALRM, &old, NULL) == -1)
				return -1;

		if (errno == EINTR && is_timeout && !(flags & NI_NAMEREQD) &&
			(sa->sa_family == AF_INET || sa->sa_family == AF_INET6))
		{
			if (sa->sa_family == AF_INET)
			{
				struct sockaddr_in *si = (struct sockaddr_in *)sa;
				if (inet_ntop(si->sin_family, &si->sin_addr, host, hostlen) == NULL)
					return -1;
				snprintf(serv, servlen, "%hu", ntohs(si->sin_port));
			}
			else if (sa->sa_family == AF_INET6)
			{
				struct sockaddr_in6 *si6 = (struct sockaddr_in6 *)sa;
				if (inet_ntop(si6->sin6_family, &si6->sin6_addr, host, hostlen) == NULL)
					return -1;
				snprintf(serv, servlen, "%hu", ntohs(si6->sin6_port));
			}
			return 0;
		}
		return -1;
	}
	alarm(0);
	if (timeout > 0)
		if (sigaction(SIGALRM, &old, NULL) == -1)
			return -1;
	return 0;
}

int main()
{
	// tests
	int err;
	struct sockaddr_in sa;
	char host[NI_MAXHOST], serv[NI_MAXSERV];

	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(13);
	if (inet_pton(AF_INET, "39.156.66.18", &sa.sin_addr) == -1)
		err_sys("inet_pton");

	if ((err = getnameinfo_timeout((struct sockaddr *)&sa, sizeof(sa), host, NI_MAXHOST, serv, NI_MAXSERV, NI_NAMEREQD, 0)) != 0)
		err_sys("getnameinfo_timeout");

	printf("host: %s\nserveice: %s\n", host, serv);

	return 0;
}