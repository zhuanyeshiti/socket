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
	int sockfd, client_fd, client2_fd;
	int len, c_len, c2_len;
	int opt_val = 1;
	struct sockaddr_in address;
	struct sockaddr_in client_address;
	struct sockaddr_in client2_address;
	int result;
	char ch[10];
	char *tmp_ch;
	FILE *fd;
	long int offset = 0;
	int ret;
	unsigned short tmp_port;
	struct sockaddr_in client, peeraddr;
	socklen_t client_len = sizeof(client);
	socklen_t peer_len = sizeof(peeraddr);
	socklen_t tmp_len = sizeof(client_address);
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
	read(sockfd, ch, 10);
	printf("read2: %s\n", ch);
	printf("client: ip=%s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	printf("address: ip=%s, port=%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	printf("peer: ip=%s, port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
	//***reuse port
	ret = getsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt_val, &tmp_len);
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt_val, sizeof(opt_val));
	ret = getsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt_val, &tmp_len);
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt_val, sizeof(opt_val));
	if (ret) {
		printf("error\n");
	}
	close(sockfd);
	//memset(ch, '\0', 10);
	if (strcmp(ch, "hello") == 0) {
		c_len = sizeof(client_address);
		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		client_address.sin_family = AF_INET;
		client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
		//client_address.sin_port = htons(tmp_port);
		client_address.sin_port = client.sin_port;
		//client_address.sin_port = htons(9090);
		client_len = sizeof(client_address);
		//***reuse port
		opt_val = 1;
		tmp_len = sizeof(client_address);
		ret = getsockopt(client_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt_val, &tmp_len);
		ret = setsockopt(client_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt_val, sizeof(opt_val));
		ret = getsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt_val, &tmp_len);
		ret = setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt_val, sizeof(opt_val));
		ret = bind(client_fd, (struct sockaddr *)&client_address, c_len);
		if (ret) {
			printf("bind error, ret=%d\n", ret);
		}
		ret = listen(client_fd, 5);
		//ret = listen(sockfd, 5);
		if (ret) {
			printf("listen error, ret=%d\n", ret);
		}
		//address.sin_port = ntohs(tmp_port + 2);
		c2_len = sizeof(client2_address);
		client2_fd = accept(client_fd, (struct sockaddr *)&client2_address, (socklen_t *)&c2_len);
		printf("client2: ip=%s, port=%d\n", inet_ntoa(client2_address.sin_addr), ntohs(client2_address.sin_port));
		//ret = getpeername(client2_fd, (struct sockaddr *)&client, &client_len);
		memset(ch, '\0', 10);
		ret = read(client2_fd, ch, 10);
		printf("1: %s, %d bytes\n", ch, ret);	
		ret = read(client2_fd, ch, 10);
		printf("2: %s, %d bytes\n", ch, ret);
	} else if (strcmp(ch, "world") == 0) {
		c_len = sizeof(client_address);
		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		client_address.sin_family = AF_INET;
		client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
		client_address.sin_port = htons(tmp_port - 2);
		//client_address.sin_port = htons(9090);
		ret = connect(client_fd, (struct sockaddr *)&client_address, c_len);
		if (ret) {
			printf("connect2 error, ret=%d\n", ret);
		}
		ret = getsockname(client_fd, (struct sockaddr *)&client, &client_len);
		printf("client2: ip=%s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		tmp_ch = "client2";
		ret = write(client_fd, tmp_ch, strlen(tmp_ch));
		printf("is world, write %d bytes\n", ret);
		ret = write(client_fd, tmp_ch, strlen(tmp_ch));
		printf("is world, write %d bytes\n", ret);
	} else {
		printf("read a error value\n");
	}

	//close(sockfd);
	
	exit(0);
}
