#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_func(void *arg);

int main(int argc, char **argv)
{
	int server_sockfd, client_sockfd[5];
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address[5];
	//FILE *fd;
	long int offset;
	int ret;
	int i = 0;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(8080);
	server_len = sizeof(server_address);

	ret = bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	if (ret) {
		printf("error bind\n");
	}
	ret = listen(server_sockfd, 5);
	if (ret) {
		printf("error listen\n");
	}
	while (1) {
		//char ch[16];
		int res;
		pthread_t a_thread;
		void *thread_result;
		offset = 0;
		printf("server waiting...\n");
		client_len = sizeof(client_address[i]);
		client_sockfd[i] = accept(server_sockfd, (struct sockaddr *)&client_address[i], (socklen_t *)&client_len);
		printf("ip=%s, port=%d\n", inet_ntoa(client_address[i].sin_addr), ntohs(client_address[i].sin_port));
		
		res = pthread_create(&a_thread, NULL, thread_func, (void *)&client_sockfd[i]);
		/*do {
			//transfer file***************************************************
			fd = fopen("/home/allenliu/base/socket/server/woman.mkv", "rb");
			fseek(fd, offset, SEEK_SET);
			ret = fread(ch, sizeof(char), 16, fd);
			printf("read from file is %d\n", ret);	
			//read(client_sockfd[i], &ch, 1);
			//ch++;
			ret = write(client_sockfd[i], ch, ret);
			printf("write to socket is %d\n", ret);
			//offset = ftell(fd);
			offset += ret;
			printf("transferred %ld KB\n", offset/1024);
			fclose(fd);
			//transfer file end***********************************************
		} while(ret);*/
		i++;
		if (i >= 5) {
			i = 0;
		}
		//close(client_sockfd[i]);
	}
}

void *thread_func(void *arg)
{
	int client_fd;
	int ret;
	char ch[16];
	client_fd = *(int *)arg;
	FILE *fd;
	long int offsetp = 0;
	do {
		//transfer file***************************************************
		fd = fopen("/home/allenliu/base/socket/server/woman.mkv", "rb");
		fseek(fd, offsetp, SEEK_SET);
		ret = fread(ch, sizeof(char), 16, fd);
		//printf("read from file is %d\n", ret);	
		ret = write(client_fd, ch, ret);
		//printf("write to socket is %d\n", ret);
		//offsetp = ftell(fd);
		offsetp += ret;
		//printf("transferred %ld KB\n", offsetp/1024);
		fclose(fd);
		//transfer file end***********************************************
	} while(ret);
	close(client_fd);

}












