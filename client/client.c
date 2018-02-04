#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch='A';
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = 8080;
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len);

	if (result == -1) {
		printf("error\n");
		exit(1);
	}

	write(sockfd, &ch, 1);
	printf("write: %s\n", &ch);
	read(sockfd, &ch, 1);
	printf("read: %s\n", &ch);
	close(sockfd);
	
	exit(0);
}
