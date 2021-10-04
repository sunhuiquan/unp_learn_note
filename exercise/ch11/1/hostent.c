#include <unp.h>

#define MAXLIST 40

int main(int argc, char **argv)
{
	char *ptr, **pptr;
	char str[INET_ADDRSTRLEN];
	struct hostent *hptr;
	char addrs[MAXLINE][MAXLIST];
	int i, af;

	while (--argc > 0)
	{
		ptr = *++argv;
		if ((hptr = gethostbyname(ptr)) == NULL)
		{
			err_msg("gethostbyname error for host: %s: %s",
					ptr, hstrerror(h_errno));
			continue;
		}
		printf("official hostname: %s\n", hptr->h_name);

		af = hptr->h_addrtype;
		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("\talias: %s\n", *pptr);

		pptr = hptr->h_addr_list;
		for (i = 0; i < MAXLIST && *pptr; ++i, ++pptr)
			strncpy(addrs[i], *pptr, MAXLINE);

		switch (af)
		{
		case AF_INET:
			for (int j = 0; j < i; ++j)
			{
				if (inet_ntop(af, addrs[j], str, sizeof(str)) == NULL)
					err_sys("inet_ntop");
				printf("\taddress: %s\n", str);

				if ((hptr = gethostbyaddr(addrs[j], sizeof(addrs[j]), AF_INET)) == NULL)
				{
					err_msg("gethostbyaddr error for addr: %s: %s",
							str, hstrerror(h_errno));
					continue;
				}
				printf("h_name: %s\n", (hptr->h_name) ? hptr->h_name : "NULL");
			}
			break;

		default:
			err_ret("unknown address type");
			break;
		}
	}
	exit(EXIT_SUCCESS);
}
