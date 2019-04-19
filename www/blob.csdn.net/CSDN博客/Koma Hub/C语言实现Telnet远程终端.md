# C语言实现Telnet远程终端 - Koma Hub - CSDN博客
2019年02月23日 20:20:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：593
**Table of Contents**
[服务端server.c](#%E6%9C%8D%E5%8A%A1%E7%AB%AFserver.c)
[客户端client.c](#%E5%AE%A2%E6%88%B7%E7%AB%AFclient.c)
[头文件mt.h](#%E5%A4%B4%E6%96%87%E4%BB%B6mt.h)
[README](#README)
### 服务端server.c
```cpp
#include "mt.h"
#define D(...) fprintf(new_stream, __VA_ARGS__)
int main() {
	int sock;
	struct sockaddr_in name;
	char buf[MAX_MSG_LENGTH] = {0};
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) perro("opening socket");
	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(PORT);
	if(bind(sock, (void*) &name, sizeof(name))) perro("binding tcp socket");
	if(listen(sock, 1) == -1) perro("listen");
	
	struct sockaddr cli_addr;
	int cli_len = sizeof(cli_addr);
	int new_socket, new_fd, pid;
	FILE* new_stream;
	
	if(new_fd = dup(STDERR_FILENO) == -1) perro("dup");
	new_stream = fdopen(new_fd, "w");
	setbuf(new_stream, NULL); // sin buffering
	
	D("Initializing server...\n");
	while(new_socket = accept(sock, &cli_addr, &cli_len)) {
		D("Client connected.\nForking... ");
		if(pid = fork()) D("child pid = %d.\n", pid);
		else {
			pid = getpid();
			if(new_socket < 0) perro("accept");
			if(dup2(new_socket, STDOUT_FILENO) == -1) perro("dup2");
			if(dup2(new_socket, STDERR_FILENO) == -1) perro("dup2");
			while(1) {
				int readc = 0, filled = 0;
				while(1) {
					readc = recv(new_socket, buf+filled, MAX_MSG_LENGTH-filled-1, 0);
					if(!readc) break;
					filled += readc;
					if(buf[filled-1] == '\0') break;
				}
				if(!readc) {
					D("\t[%d] Client disconnected.\n", pid);
					break;
				}
				D("\t[%d] Command received: %s", pid, buf);
				system(buf);
				D("\t[%d] Finished executing command.\n", pid);
				send(new_socket, "> ", 3, MSG_NOSIGNAL);
			}
			close(new_socket);
			D("\t[%d] Dying.", pid);
			exit(0);
		}
	}
	fclose(new_stream);
	close(sock);
	return 0;
}
```
### 客户端client.c
```cpp
#include "mt.h"
void send_cmd(int sock, int pid) {
	char str[MAX_MSG_LENGTH] = {0};
	printf("> ");
	while (fgets(str, MAX_MSG_LENGTH, stdin) == str) {
		if(strncmp(str, END_STRING, strlen(END_STRING)) == 0) break;
		if(send(sock, str, strlen(str)+1, 0) < 0) perro("send");
	}
	kill(pid, SIGKILL);
	printf("Goodbye.\n");
}
void receive(int sock) {
	char buf[MAX_MSG_LENGTH] = {0};
	int filled = 0;	
	while(filled = recv(sock, buf, MAX_MSG_LENGTH-1, 0)) {
		buf[filled] = '\0';
		printf("%s", buf);
		fflush(stdout);		
	}	
	printf("Server disconnected.\n");
}
int main(int argc, char **argv) {
	if(argc != 2) perro("args");
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) perro("socket");
	struct in_addr server_addr;
	if(!inet_aton(argv[1], &server_addr)) perro("inet_aton");
	struct sockaddr_in connection;
	connection.sin_family = AF_INET;
	memcpy(&connection.sin_addr, &server_addr, sizeof(server_addr));
	connection.sin_port = htons(PORT);
	if (connect(sock, (const struct sockaddr*) &connection, sizeof(connection)) != 0) perro("connect");
	
	int pid;	
	if(pid = fork()) send_cmd(sock, pid);
	else receive(sock);
	
	return 0;
}
```
### 头文件mt.h
```cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define PORT 5555
#define MAX_MSG_LENGTH 1024
#define END_STRING "chau\n"
#define COMPLETE_STRING "fin-respuesta"
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif
#define perro(x) {fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, x, strerror(errno));exit(1);}
```
### README
```cpp
Telnet client/server
===========================
Run the server as:
    ./server
It requires port 5555 to be open (this can be changed in mt.h, 
or make it a command line parameter if you wish).
To run clients, run
    ./client 127.0.0.1
To connect locally, as the server listens locally.
```
