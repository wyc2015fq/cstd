
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SOCK_LENTH socklen_t
#define closesocket(x)  close(x)
int sock_init() {
  return 1;
}
#include "net_impl.inl"