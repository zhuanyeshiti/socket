#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

int main(int argc, char **argv)
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch[10];
	FILE *fd;
	long int offset = 0;
	int ret;
	unsigned short tmp_port;
	struct sockaddr_in client, peeraddr;
	socklen_t client_len = sizeof(client);
	socklen_t peer_len = sizeof(peeraddr);
	memset(ch, '\0', 10);
	
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
	ret = getpeername(sockfd, (struct sockaddr *)&peeraddr, &peer_len);

	if (result == -1) {
		printf("error\n");
		exit(1);
	}

	/*do {
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
	} while(ret);*/

	//write(sockfd, &ch, 1);
	//printf("write: %s\n", &ch);
	read(sockfd, ch, 10);
	tmp_port = c2u(ntohs(client.sin_port), ch, 10);
	printf("read: %s, %u\n", ch, tmp_port);

	printf("client: ip=%s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	printf("address: ip=%s, port=%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	printf("peer: ip=%s, port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
	//listen(sockfd, 5);
	close(sockfd);
	
	exit(0);
}
