/* Get default send and receive buffer size from TCP and UDP sockets. */
#include <unp.h>

union sockopt_val
{
	int i_val;
	long l_val;
	//另外两种返回类型懒得写了
};

int main()
{
	int tfd, ufd; // tcp and udp
	union sockopt_val val;
	socklen_t len;

	if ((tfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err_sys("socket");
	if ((ufd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err_sys("socket");

	len = sizeof(val);
	if (getsockopt(tfd, SOL_SOCKET, SO_SNDBUF, &val, &len) == -1)
		err_sys("getsockopt");
	printf("TCP send buffer default size:%d\n", val.i_val);
	len = sizeof(val);
	if (getsockopt(tfd, SOL_SOCKET, SO_RCVBUF, &val, &len) == -1)
		err_sys("getsockopt");
	printf("TCP receive buffer default size:%d\n", val.i_val);

	len = sizeof(val);
	if (getsockopt(ufd, SOL_SOCKET, SO_SNDBUF, &val, &len) == -1)
		err_sys("getsockopt");
	printf("UDP send buffer default size:%d\n", val.i_val);
	len = sizeof(val);
	if (getsockopt(ufd, SOL_SOCKET, SO_RCVBUF, &val, &len) == -1)
		err_sys("getsockopt");
	printf("UDP receive buffer default size:%d\n", val.i_val);

	return 0;
}