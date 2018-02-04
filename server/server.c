#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	FILE *fd;
	long int offset;
	int ret;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = 8080;
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	while (1) {
		char ch[16];
		offset = 0;
		printf("server waiting...\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
		
		do {
			/** transfer file **/
			fd = fopen("/home/allenliu/base/socket/server/woman.mkv", "rb");
			fseek(fd, offset, SEEK_SET);
			ret = fread(ch, sizeof(char), 16, fd);
			printf("read from file is %d\n", ret);	
			//read(client_sockfd, &ch, 1);
			//ch++;
			ret = write(client_sockfd, ch, ret);
			printf("write to socket is %d\n", ret);
			//offset = ftell(fd);
			offset += ret;
			printf("transferred %ld KB\n", offset/1024);
			fclose(fd);
			/*******************/
		} while(ret);
		
		close(client_sockfd);
	}
}
