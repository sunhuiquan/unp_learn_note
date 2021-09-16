#include <unp.h>
#include <netinet/in.h>
#include <string.h>

int inet_pton_loose(int family, const char *strptr, void *addrptr)
{
	struct in_addr inaddr;
	int res = inet_pton(family, strptr, addrptr);
	if (res == 1)
		return 1;
	else if (res == 0) // format wrong
	{
		if (inet_aton(strptr, &inaddr) == 1)
		{
			memcpy(addrptr, &inaddr, sizeof(struct in_addr));
			return 1;
		}
		else
			return -1;
	}
	else
		return -1;
}

int main(int argc, char **argv)
{
	struct in_addr addr;
	struct in6_addr addr6;

	if (argc != 3)
	{
		printf("%s usage: <family> <address>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	bzero(&addr, sizeof(struct in_addr));
	bzero(&addr6, sizeof(struct in6_addr));
	if (strcmp(argv[1], "AF_INET") == 0)
	{
		if (inet_pton_loose(AF_INET, argv[2], (void *)&addr.s_addr) < 0)
			printf("convert failed\n");
		printf("network bytes order: %08x\n", addr.s_addr);
	}
	else if (strcmp(argv[1], "AF_INET6") == 0)
	{
		if (inet_pton_loose(AF_INET6, argv[2], (void *)&addr6.__in6_u) < 0)
			printf("convert failed\n");
		printf("network bytes order: %08x%08x%08x%08x\n", addr6.s6_addr32[0], addr6.s6_addr32[1], addr6.s6_addr32[2], addr6.s6_addr32[3]);
	}
	else
	{
		printf("only support AF_INET and AF_INET6 family\n");
	}

	return 0;
}
