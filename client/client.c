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
	FILE *fd;
	long int offset = 0;
	int ret;
	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	address.sin_family = AF_INET;
	//address.sin_addr.s_addr = inet_addr("39.106.112.82");
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(8080);
	len = sizeof(address);

	if (argv[1] ==  NULL) {
		printf("argv == NULL, it shouldn't be\n");
		return 0;
	}
	result = connect(sockfd, (struct sockaddr *)&address, len);
	ret = getsockname(sockfd, (struct sockaddr *)&client, &client_len);

	if (result == -1) {
		printf("error\n");
		exit(1);
	}

	do {
		char chr[16];
		fd = fopen(argv[1], "ab+");
		fseek(fd, offset, SEEK_SET);
		ret = read(sockfd, chr, 16);
		printf("ret is %d\n", ret);
		ret = fwrite(chr, sizeof(char), ret, fd);
		printf("write is %d\n", ret);
		offset = ftell(fd);
		//offset += ret;
		printf("receive %ld KB\n", offset/1024);
		fclose(fd);
	} while(ret);
	//write(sockfd, &ch, 1);
	//printf("write: %s\n", &ch);
	//read(sockfd, &ch, 1);
	//printf("read: %s\n", &ch);
	printf("ip=%s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	//listen(sockfd, 5);
	close(sockfd);
	
	exit(0);
}
