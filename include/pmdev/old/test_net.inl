#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 获取本机IP地址
int test_gethostip()
{
  {
    char hname[128];
    struct hostent* hent;
    int i;

    gethostname(hname, sizeof(hname));

    //hent = gethostent();
    hent = gethostbyname(hname);

    printf("hostname: %s/naddress list: \n", hent->h_name);

    for (i = 0; hent->h_addr_list[i]; i++) {
      printf("%s\n", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
    }
  }

  //通过枚举网卡，API接口可查看man 7 netdevice

  /*代码来自StackOverflow: http://stackoverflow.com/questions/212528/linux-c-get-the-ip-address-of-local-computer */
#if 0

  if (1) {
    struct ifaddrs* ifAddrStruct = NULL;
    void* tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct != NULL) {
      if (ifAddrStruct->ifa_addr->sa_family == AF_INET) { // check it is IP4
        // is a valid IP4 Address
        tmpAddrPtr = &((struct sockaddr_in*)ifAddrStruct->ifa_addr)->sin_addr;
        char addressBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
        printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
      }
      else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) { // check it is IP6
        // is a valid IP6 Address
        tmpAddrPtr = &((struct sockaddr_in*)ifAddrStruct->ifa_addr)->sin_addr;
        char addressBuffer[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
        printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
      }

      ifAddrStruct = ifAddrStruct->ifa_next;
    }
  }

#endif



  return 0;
}





int test_net()
{
  test_gethostip();
  return 0;
}