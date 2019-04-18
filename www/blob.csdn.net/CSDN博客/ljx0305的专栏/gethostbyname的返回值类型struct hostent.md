# gethostbyname的返回值类型struct hostent - ljx0305的专栏 - CSDN博客
2008年09月09日 14:14:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3603
                 这个数据结构是这样的： 
　　 struct hostent { 
　　 char *h_name; 
　　 char **h_aliases; 
　　 int h_addrtype; 
　　 int h_length; 
　　 char **h_addr_list; 
　　 }; 
　　 #define h_addr h_addr_list[0]  
这里是这个数据结构的详细资料：  
struct hostent:  
　　h_name – 地址的正式名称。 
　　h_aliases – 空字节-地址的预备名称的指针。 
　　h_addrtype –地址类型; 通常是AF_INET。  
　　h_length – 地址的比特长度。 
　　h_addr_list – 零字节-主机网络地址指针。网络字节顺序。 
　　h_addr - h_addr_list中的第一地址。 
gethostbyname () 成功时返回一个指向结构体 hostent 的指针，或者 是个空 (NULL) 指针。(但是和以前不同，不设置errno，h_errno 设置错 误信息。请看下面的 herror()。) 但是如何使用呢? 这个函数可不象它看上去那么难用。 
这里是个例子： 
#include  
　　#include  
　　#include  
　　#include  
　　#include  
　　#include  
int main(int argc, char *argv[]) 
　　 { 
　　 struct hostent *h; 
if (argc != 2) { /* 检查命令行 */ 
　　 fprintf(stderr,"usage: getip address "); 
　　 exit(1); 
　　 } 
if ((h=gethostbyname(argv[1])) == NULL) { /* 取得地址信息 */ 
　　 herror("gethostbyname"); 
　　 exit(1); 
　　 } 
printf("Host name : %s ", h->h_name); 
　　printf("IP Address : %s ",inet_ntoa(*((struct in_addr *)h->h_addr))); 
return 0; 
　　 } 
在使用 gethostbyname() 的时候，你不能用perror() 打印错误信息 (因为 errno 没有使用)，你应该调用 herror()。
相当简单，你只是传递一个保存机器名的字符串(例如 "whitehouse.gov") 给gethostbyname()，然后从返回的数据结构  struct hostent 中获取信息。唯一也许让人不解的是输出 IP 地址信息。h->h_addr 是一个 char *， 但是 inet_ntoa() 需要的是  struct in_addr。因此，我转换 h->h_addr 成 struct in_addr *，然后得到数据。            
