#include <stdio.h>

int main()
{
	void* vsize;
	int sock;
	int fsize;
	struct sockaddr_in addr;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0)
	{
		perror("socket does not created");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6001);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{	
		perror("connection error");
		exit(2);	
	}

	//TODO: file resieving

	close(sock);

	return 0;
	
}
