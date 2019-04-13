
# OpenSSL编写SSL,TLS程序 - 高科的专栏 - CSDN博客

2018年08月18日 12:25:18[高科](https://me.csdn.net/pbymw8iwm)阅读数：315


一、简介:
SSL(Secure Socket Layer)是netscape公司提出的主要用于web的安全通信标准,分为2.0版和3.0版.TLS(Transport Layer Security)是IETF的TLS 工作组在SSL3.0基础之上提出的安全通信标准,目前版本是1.0,即RFC2246.SSL/TLS提供的安全机制可以保证应用层数据在互联网络传输不 被监听,伪造和窜改.
openssl(http://www.openssl.org/)是sslv2,sslv3,tlsv1的一份完整实现,内部包含了大量加密算法程序.其命令行提供了丰富的加密,验证,证书生成等功能,甚至可以用其建立一个完整的CA.与其同时,它也提供了一套完整的库函数,可用开发用SSL/TLS的通信程序. Apache的https两种版本 mod_ssl和apachessl均基于它实现的.openssl继承于ssleay,并做了一定的扩展,当前的版本是0.9.5a.
openssl 的缺点是文档太少,连一份完整的函数说明都没有,man page也至今没做完整:-(,如果想用它编程序,除了熟悉已有的文档(包括 ssleay,mod_ssl,apachessl的文档)外,可以到它的maillist上找相关的帖子,许多问题可以在以前的文章中找到答案.
编程:
程序分为两部分,客户端和服务器端,我们的目的是利用SSL/TLS的特性保证通信双方能够互相验证对方身份(真实性),并保证数据的完整性, 私密性.
1.客户端程序的框架为:
/*生成一个SSL结构*/
meth = SSLv23_client_method();
ctx = SSL_CTX_new (meth);
ssl = SSL_new(ctx);
/*下面是正常的socket过程*/
fd = socket();
connect();
/*把建立好的socket和SSL结构联系起来*/
SSL_set_fd(ssl,fd);
/*SSL的握手过程*/
SSL_connect(ssl);
/*接下来用SSL_write(), SSL_read()代替原有的write(),read()即可*/
SSL_write(ssl,"Hello world",strlen("Hello World!"));
2.服务端程序的框架为:
/*生成一个SSL结构*/
meth = SSLv23_server_method();
ctx = SSL_CTX_new (meth);
ssl = SSL_new(ctx);
/*下面是正常的socket过程*/
fd = socket();
bind();
listen();
accept();
/*把建立好的socket和SSL结构联系起来*/
SSL_set_fd(ssl,fd);
/*SSL的握手过程*/
SSL_connect(ssl);
/*接下来用SSL_write(), SSL_read()代替原有的write(),read()即可*/
SSL_read (ssl, buf, sizeof(buf));
根据RFC2246(TLS1.0)整个TLS(SSL)的流程如下:
Client Server
ClientHello -------->
ServerHello
Certificate*
ServerKeyExchange*
CertificateRequest*
<-------- ServerHelloDone
Certificate*
ClientKeyExchange
CertificateVerify*
[ChangeCipherSpec]
Finished -------->
[ChangeCipherSpec]
<-------- Finished
Application Data <-------> Application Data
对程序来说,openssl将整个握手过程用一对函数体现,即客户端的SSL_connect和服务端的SSL_accept.而后的应用层数据交换则用SSL_read和 SSL_write来完成.
二、证书文件生成
除将程序编译成功外,还需生成必要的证书和私钥文件使双方能够成功验证对方,步骤如下:
1.首先要生成服务器端的私钥(key文件):
openssl genrsa -des3 -out server.key 1024
运行时会提示输入密码,此密码用于加密key文件(参数des3便是指加密算法,当然也可以选用其他你认为安全的算法.),以后每当需读取此文 件(通过openssl提供的命令或API)都需输入口令.如果觉得不方便,也可以去除这个口令,但一定要采取其他的保护措施!
去除key文件口令的命令:
openssl rsa -in server.key -out server.key
2.openssl req -new -key server.key -out server.csr
生成Certificate Signing Request（CSR）,生成的csr文件交给CA签名后形成服务端自己的证书.屏幕上将有提示,依照其指示一步一步输入要 求的个人信息即可.
3.对客户端也作同样的命令生成key及csr文件:
openssl genrsa -des3 -out client.key 1024
openssl req -new -key client.key -out client.csr
4.CSR文件必须有CA的签名才可形成证书.可将此文件发送到verisign等地方由它验证,要交一大笔钱,何不自己做CA呢.
首先生成CA的key文件:
openssl -des3 -out ca.key 1024
在生成CA自签名的证书:
openssl req -new -x509 -key ca.key -out ca.crt
如果想让此证书有个期限,如一年,则加上"-days 365".
("如果非要为这个证书加上一个期限,我情愿是..一万年")
5.用生成的CA的证书为刚才生成的server.csr,client.csr文件签名:
可以用openssl中CA系列命令,但不是很好用(也不是多难,唉,一言难尽),一篇文章中推荐用mod_ssl中的sign.sh脚本,试了一下,确实方便了不 少,如果ca.csr存在的话,只需:
./sigh.sh server.csr
./sign.sh client.csr
相应的证书便生成了(后缀.crt).
现在我们所需的全部文件便生成了.
其实openssl中还附带了一个叫CA.pl的文件(在安装目录中的misc子目录下),可用其生成以上的文件,使用也比较方便,但此处就不作介绍了.
三、需要了解的一些函数:
1.int SSL_CTX_set_cipher_list(SSL_CTX *,const char *str);
根据SSL/TLS规范,在ClientHello中,客户端会提交一份自己能够支持的加密方法的列表,由服务端选择一种方法后在ServerHello中通知服务端, 从而完成加密算法的协商.
可用的算法为:
EDH-RSA-DES-CBC3-SHA
EDH-DSS-DES-CBC3-SHA
DES-CBC3-SHA
DHE-DSS-RC4-SHA
IDEA-CBC-SHA
RC4-SHA
RC4-MD5
EXP1024-DHE-DSS-RC4-SHA
EXP1024-RC4-SHA
EXP1024-DHE-DSS-DES-CBC-SHA
EXP1024-DES-CBC-SHA
EXP1024-RC2-CBC-MD5
EXP1024-RC4-MD5
EDH-RSA-DES-CBC-SHA
EDH-DSS-DES-CBC-SHA
DES-CBC-SHA
EXP-EDH-RSA-DES-CBC-SHA
EXP-EDH-DSS-DES-CBC-SHA
EXP-DES-CBC-SHA
EXP-RC2-CBC-MD5
EXP-RC4-MD5
这些算法按一定优先级排列,如果不作任何指定,将选用DES-CBC3-SHA.用SSL_CTX_set_cipher_list可以指定自己希望用的算法(实际上只是 提高其优先级,是否能使用还要看对方是否支持).
我们在程序中选用了RC4做加密,MD5做消息摘要(先进行MD5运算,后进行RC4加密).即
SSL_CTX_set_cipher_list(ctx,"RC4-MD5");
在消息传输过程中采用对称加密(比公钥加密在速度上有极大的提高),其所用秘钥(shared secret)在握手过程中中协商(每次对话过程均不同, 在一次对话中都有可能有几次改变),并通过公钥加密的手段由客户端提交服务端.
2.void SSL_CTX_set_verify(SSL_CTX *ctx,int mode,int (*callback)(int, X509_STORE_CTX *));
缺省mode是SSL_VERIFY_NONE,如果想要验证对方的话,便要将此项变成SSL_VERIFY_PEER.SSL/TLS中缺省只验证server,如果没有设置 SSL_VERIFY_PEER的话,客户端连证书都不会发过来.
3.int SSL_CTX_load_verify_locations(SSL_CTX *ctx, const char *CAfile,const char *CApath);
要验证对方的话,当然装要有CA的证书了,此函数用来便是加载CA的证书文件的.
4.int SSL_CTX_use_certificate_file(SSL_CTX *ctx, const char *file, int type);
加载自己的证书文件.
5.int SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, const char *file, int type);
加载自己的私钥,以用于签名.
6.int SSL_CTX_check_private_key(SSL_CTX *ctx);
调用了以上两个函数后,自己检验一下证书与私钥是否配对.
7.void RAND_seed(const void *buf,int num);
在win32 的环境中client程序运行时出错(SSL_connect返回-1)的一个主要机制便是与UNIX平台下的随机数生成机制不同(握手的时候用的到). 具体描述可见mod_ssl的FAQ.解决办法就是调用此函数,其中buf应该为一随机的字符串,作为"seed".
还可以采用一下两个函数:
void RAND_screen(void);
int RAND_event(UINT, WPARAM, LPARAM);
其中RAND_screen()以屏幕内容作为"seed"产生随机数,RAND_event可以捕获windows中的事件(event),以此为基础产生随机数.如果一直有 用户干预的话,用这种办法产生的随机数能够"更加随机",但如果机器一直没人理(如总停在登录画面),则每次都将产生同样的数字.
这几个函数都只在WIN32环境下编译时有用,各种UNIX下就不必调了.
大量其他的相关函数原型,见crypto\rand\rand.h.
8.OpenSSL_add_ssl_algorithms()或SSLeay_add_ssl_algorithms()
其实都是调用int SSL_library_init(void)
进行一些必要的初始化工作,用openssl编写SSL/TLS程序的话第一句便应是它.
9.void SSL_load_error_strings(void );
如果想打印出一些方便阅读的调试信息的话,便要在一开始调用此函数.
10.void ERR_print_errors_fp(FILE *fp);
如果调用了SSL_load_error_strings()后,便可以随时用ERR_print_errors_fp()来打印错误信息了.
11.X509 *SSL_get_peer_certificate(SSL *s);
握手完成后,便可以用此函数从SSL结构中提取出对方的证书(此时证书得到且已经验证过了)整理成X509结构.
12.X509_NAME *X509_get_subject_name(X509 *a);
得到证书所有者的名字,参数可用通过SSL_get_peer_certificate()得到的X509对象.
13.X509_NAME *X509_get_issuer_name(X509 *a)
得到证书签署者(往往是CA)的名字,参数可用通过SSL_get_peer_certificate()得到的X509对象.
14.char *X509_NAME_oneline(X509_NAME *a,char *buf,int size);
将以上两个函数得到的对象变成字符型,以便打印出来.
15.SSL_METHOD的构造函数,包括
SSL_METHOD *TLSv1_server_method(void); /* TLSv1.0 */
SSL_METHOD *TLSv1_client_method(void); /* TLSv1.0 */
SSL_METHOD *SSLv2_server_method(void); /* SSLv2 */
SSL_METHOD *SSLv2_client_method(void); /* SSLv2 */
SSL_METHOD *SSLv3_server_method(void); /* SSLv3 */
SSL_METHOD *SSLv3_client_method(void); /* SSLv3 */
SSL_METHOD *SSLv23_server_method(void); /* SSLv3 but can rollback to v2 */
SSL_METHOD *SSLv23_client_method(void); /* SSLv3 but can rollback to v2 */
在程序中究竟采用哪一种协议(TLSv1/SSLv2/SSLv3),就看调哪一组构造函数了.
四:程序源代码(WIN32版本):
基本上是改造的openssl自带的demos目录下的cli.cpp,serv.cpp文件,做了一些修改,并增加了一些功能.
引用文字
> /*****************************************************************

> *SSL/TLS客户端程序WIN32版(以demos/cli.cpp为基础)

> *需要用到动态连接库libeay32.dll,ssleay.dll,

> *同时在setting中加入ws2_32.lib libeay32.lib ssleay32.lib,

> *以上库文件在编译openssl后可在out32dll目录下找到,

> *所需证书文件请参照文章自行生成*/

> *****************************************************************/

> \#include <stdio.h>

> \#include <stdlib.h>

> \#include <memory.h>

> \#include <errno.h>

> \#include <sys/types.h>

> \#include <winsock2.h>

> \#include "openssl/rsa.h"

> \#include "openssl/crypto.h"

> \#include "openssl/x509.h"

> \#include "openssl/pem.h"

> \#include "openssl/ssl.h"

> \#include "openssl/err.h"

> \#include "openssl/rand.h"

> /*所有需要的参数信息都在此处以\#define的形式提供*/

> \#define CERTF "client.crt" /*客户端的证书(需经CA签名)*/

> \#define KEYF "client.key" /*客户端的私钥(建议加密存储)*/

> \#define CACERT "ca.crt" /*CA 的证书*/

> \#define PORT 1111 /*服务端的端口*/

> \#define SERVER_ADDR "127.0.0.1" /*服务段的IP地址*/

> \#define CHK_NULL(x) if ((x)==NULL) exit (-1)

> \#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(-2); }

> \#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(-3); }

> int main ()

> {

> int err;

> int sd;

> struct sockaddr_in sa;

> SSL_CTX* ctx;

> SSL* ssl;

> X509* server_cert;

> char* str;

> char buf [4096];

> SSL_METHOD *meth;

> int seed_int[100]; /*存放随机序列*/

> WSADATA wsaData;

> if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){

> printf("WSAStartup()fail:%d\n",GetLastError());

> return -1;

> }

> OpenSSL_add_ssl_algorithms(); /*初始化*/

> SSL_load_error_strings(); /*为打印调试信息作准备*/

> meth = TLSv1_client_method(); /*采用什么协议(SSLv2/SSLv3/TLSv1)在此指定*/

> ctx = SSL_CTX_new (meth);

> CHK_NULL(ctx);

> SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL); /*验证与否*/

> SSL_CTX_load_verify_locations(ctx,CACERT,NULL); /*若验证,则放置CA证书*/


> if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {

> ERR_print_errors_fp(stderr);

> exit(-2);

> }

> if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {

> ERR_print_errors_fp(stderr);

> exit(-3);

> }

> if (!SSL_CTX_check_private_key(ctx)) {

> printf("Private key does not match the certificate public key\n");

> exit(-4);

> }

> /*构建随机数生成机制,WIN32平台必需*/

> srand( (unsigned)time( NULL ) );

> for( int i = 0; i < 100;i++ )

> seed_int
> = rand();

> RAND_seed(seed_int, sizeof(seed_int));

> /*以下是正常的TCP socket建立过程 .............................. */

> printf("Begin tcp socket...\n");

> sd = socket (AF_INET, SOCK_STREAM, 0); CHK_ERR(sd, "socket");

> memset (&sa, '\0', sizeof(sa));

> sa.sin_family = AF_INET;

> sa.sin_addr.s_addr = inet_addr (SERVER_ADDR); /* Server IP */

> sa.sin_port = htons (PORT); /* Server Port number */

> err = connect(sd, (struct sockaddr*) &sa,

> sizeof(sa));

> CHK_ERR(err, "connect");

> /* TCP 链接已建立.开始 SSL 握手过程.......................... */

> printf("Begin SSL negotiation \n");

> ssl = SSL_new (ctx);

> CHK_NULL(ssl);

> SSL_set_fd (ssl, sd);

> err = SSL_connect (ssl);

> CHK_SSL(err);

> /*打印所有加密算法的信息(可选)*/

> printf ("SSL connection using %s\n", SSL_get_cipher (ssl));

> /*得到服务端的证书并打印些信息(可选) */

> server_cert = SSL_get_peer_certificate (ssl);

> CHK_NULL(server_cert);

> printf ("Server certificate:\n");

> str = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);

> CHK_NULL(str);

> printf ("\t subject: %s\n", str);

> Free (str);

> str = X509_NAME_oneline (X509_get_issuer_name (server_cert),0,0);

> CHK_NULL(str);

> printf ("\t issuer: %s\n", str);

> Free (str);

> X509_free (server_cert); /*如不再需要,需将证书释放 */

> /* 数据交换开始,用SSL_write,SSL_read代替write,read */

> printf("Begin SSL data exchange\n");

> err = SSL_write (ssl, "Hello World!", strlen("Hello World!"));

> CHK_SSL(err);

> err = SSL_read (ssl, buf, sizeof(buf) - 1);

> CHK_SSL(err);

> buf[err] = '\0';

> printf ("Got %d chars:'%s'\n", err, buf);

> SSL_shutdown (ssl); /* send SSL/TLS close_notify */

> /* 收尾工作 */

> shutdown (sd,2);

> SSL_free (ssl);

> SSL_CTX_free (ctx);

> return 0;

> }

> /*****************************************************************

> * EOF - cli.cpp

> *****************************************************************/
引用文字
> /*****************************************************************

> *SSL/TLS服务端程序WIN32版(以demos/server.cpp为基础)

> *需要用到动态连接库libeay32.dll,ssleay.dll,

> *同时在setting中加入ws2_32.lib libeay32.lib ssleay32.lib,

> *以上库文件在编译openssl后可在out32dll目录下找到,

> *所需证书文件请参照文章自行生成.

> *****************************************************************/

> \#include <stdio.h>

> \#include <stdlib.h>

> \#include <memory.h>

> \#include <errno.h>

> \#include <sys/types.h>

> \#include <winsock2.h>

> \#include "openssl/rsa.h"

> \#include "openssl/crypto.h"

> \#include "openssl/x509.h"

> \#include "openssl/pem.h"

> \#include "openssl/ssl.h"

> \#include "openssl/err.h"

> /*所有需要的参数信息都在此处以\#define的形式提供*/

> \#define CERTF "server.crt" /*服务端的证书(需经CA签名)*/

> \#define KEYF "server.key" /*服务端的私钥(建议加密存储)*/

> \#define CACERT "ca.crt" /*CA 的证书*/

> \#define PORT 1111 /*准备绑定的端口*/

> \#define CHK_NULL(x) if ((x)==NULL) exit (1)

> \#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }

> \#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

> int main ()

> {

> int err;

> int listen_sd;

> int sd;

> struct sockaddr_in sa_serv;

> struct sockaddr_in sa_cli;

> int client_len;

> SSL_CTX* ctx;

> SSL* ssl;

> X509* client_cert;

> char* str;

> char buf [4096];

> SSL_METHOD *meth;

> WSADATA wsaData;

> if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){

> printf("WSAStartup()fail:%d\n",GetLastError());

> return -1;

> }

> SSL_load_error_strings(); /*为打印调试信息作准备*/

> OpenSSL_add_ssl_algorithms(); /*初始化*/

> meth = TLSv1_server_method(); /*采用什么协议(SSLv2/SSLv3/TLSv1)在此指定*/

> ctx = SSL_CTX_new (meth);

> CHK_NULL(ctx);

> SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL); /*验证与否*/

> SSL_CTX_load_verify_locations(ctx,CACERT,NULL); /*若验证,则放置CA证书*/

> if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {

> ERR_print_errors_fp(stderr);

> exit(3);

> }

> if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {

> ERR_print_errors_fp(stderr);

> exit(4);

> }

> if (!SSL_CTX_check_private_key(ctx)) {

> printf("Private key does not match the certificate public key\n");

> exit(5);

> }

> SSL_CTX_set_cipher_list(ctx,"RC4-MD5");

> /*开始正常的TCP socket过程.................................*/

> printf("Begin TCP socket...\n");

> listen_sd = socket (AF_INET, SOCK_STREAM, 0);

> CHK_ERR(listen_sd, "socket");

> memset (&sa_serv, '\0', sizeof(sa_serv));

> sa_serv.sin_family = AF_INET;

> sa_serv.sin_addr.s_addr = INADDR_ANY;

> sa_serv.sin_port = htons (PORT);

> err = bind(listen_sd, (struct sockaddr*) &sa_serv,

> sizeof (sa_serv));

> CHK_ERR(err, "bind");

> /*接受TCP链接*/

> err = listen (listen_sd, 5);

> CHK_ERR(err, "listen");

> client_len = sizeof(sa_cli);

> sd = accept (listen_sd, (struct sockaddr*) &sa_cli, &client_len);

> CHK_ERR(sd, "accept");

> closesocket (listen_sd);

> printf ("Connection from %lx, port %x\n",

> sa_cli.sin_addr.s_addr, sa_cli.sin_port);

> /*TCP连接已建立,进行服务端的SSL过程. */

> printf("Begin server side SSL\n");

> ssl = SSL_new (ctx);

> CHK_NULL(ssl);

> SSL_set_fd (ssl, sd);

> err = SSL_accept (ssl);

> printf("SSL_accept finished\n");

> CHK_SSL(err);

> /*打印所有加密算法的信息(可选)*/

> printf ("SSL connection using %s\n", SSL_get_cipher (ssl));

> /*得到服务端的证书并打印些信息(可选) */

> client_cert = SSL_get_peer_certificate (ssl);

> if (client_cert != NULL) {

> printf ("Client certificate:\n");

> str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);

> CHK_NULL(str);

> printf ("\t subject: %s\n", str);

> Free (str);

> str = X509_NAME_oneline (X509_get_issuer_name (client_cert), 0, 0);

> CHK_NULL(str);

> printf ("\t issuer: %s\n", str);

> Free (str);

> X509_free (client_cert);/*如不再需要,需将证书释放 */

> }

> else

> printf ("Client does not have certificate.\n");

> /* 数据交换开始,用SSL_write,SSL_read代替write,read */

> err = SSL_read (ssl, buf, sizeof(buf) - 1);

> CHK_SSL(err);

> buf[err] = '\0';

> printf ("Got %d chars:'%s'\n", err, buf);

> err = SSL_write (ssl, "I hear you.", strlen("I hear you."));

> CHK_SSL(err);

> /* 收尾工作*/

> shutdown (sd,2);

> SSL_free (ssl);

> SSL_CTX_free (ctx);

> return 0;

> }

> /*****************************************************************

> * EOF - serv.cpp

> *****************************************************************/

五、https / http 兼容客户端的c语言实现
引用文字
> /*

> * OpenSSL SSL/TLS Https Client example

> * Only for Unix/Linux:

> * cc -c https.c

> * cc -o https https.c -lssl

> * OpenSSL library needed.

> *

> * 同时支持普通的socket连接以及基于普通socket基础之上的ssl

> * 连接。这对于已有的socket程序修改来说会比较方便，不至于

> * 和原来的结构发生太大的冲突.

> * 要注意的一点，似乎当使用socket套接字来创建ssl连接的时候,

> * 如果套接字是采用非阻塞方式建立的话，会导致ssl会话失败，不

> * 知道为什么。所以这里对于提供给https的套接字采用了普通的

> * connect方法创建。

> *

> */

> \#include <stdio.h>

> \#include <stdlib.h>

> \#include <string.h>

> \#include <stdarg.h>

> \#include <errno.h>

> \#include <fcntl.h>

> \#include <unistd.h>

> \#include <sys/types.h>

> \#include <sys/socket.h>

> \#include <netinet/in.h>

> \#include <arpa/inet.h>

> \#include <netdb.h>

> \#include <openssl/crypto.h>

> \#include <openssl/ssl.h>

> \#include <openssl/err.h>

> \#include <openssl/rand.h>


> \#define BUF_LEN 1024

> \#define MAX_STRING_LEN 2048

> //xnet_select x defines

> \#define READ_STATUS 0

> \#define WRITE_STATUS 1

> \#define EXCPT_STATUS 2

> /* flag to set request with ssl or not. */

> static int bIsHttps = 1;

> static int timeout_sec = 10;

> static int timeout_microsec = 0;

> void err_doit(int errnoflag, const char *fmt, va_list ap);

> void err_quit(const char *fmt, ...);

> int create_tcpsocket(const char *host, const unsigned short port);

> int xnet_select(int s, int sec, int usec, short x);

> int main(int argc, char* argv[]){

> char* host = "127.0.0.1";

> unsigned short port = 80;

> int fd;

> SSL *ssl;

> SSL_CTX *ctx;

> int n,ret;

> char buf[BUF_LEN];

> char* requestpath = "/";

> if( argc == 5 ){

> host = argv[1];

> port = atoi(argv[2]);

> requestpath = argv[3];

> bIsHttps = atoi(argv[4]);

> }

> /* make connection to the cache server */

> fd = create_tcpsocket(host, port);

> /* http request. */

> sprintf(buf, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n",

> requestpath, host);

> if(bIsHttps != 1){

> if(xnet_select(fd, timeout_sec, timeout_microsec,

> WRITE_STATUS)>0){

> /* send off the message */

> write(fd, buf, strlen(buf));

> }

> else{

> err_quit("Socket I/O Write Timeout %s:%d\n", host, port);

> }

> printf("Server response:\n");

> while (xnet_select(fd, timeout_sec, timeout_microsec,

> READ_STATUS)>0){

> if ((n = read(fd, buf, BUF_LEN-1)) > 0) {

> buf[n] = '\0';

> printf("%s", buf);

> }

> else{

> break;

> }

> }

> // close the plain socket handler.

> close(fd);

> }

> else{

> SSL_load_error_strings();

> SSL_library_init();

> ctx = SSL_CTX_new(SSLv23_client_method());

> if ( ctx == NULL ){

> err_quit("init SSL CTX failed:%s\n",

> ERR_reason_error_string(ERR_get_error()));

> }

> ssl = SSL_new(ctx);

> if ( ssl == NULL ){

> err_quit("new SSL with created CTX failed:%s\n",

> ERR_reason_error_string(ERR_get_error()));

> }

> ret = SSL_set_fd(ssl, fd);

> if ( ret == 0 ){

> err_quit("add SSL to tcp socket failed:%s\n",

> ERR_reason_error_string(ERR_get_error()));

> }

> /* PRNG */

> RAND_poll();

> while ( RAND_status() == 0 ){

> unsigned short rand_ret = rand() % 65536;

> RAND_seed(&rand_ret, sizeof(rand_ret));

> }

> /* SSL Connect */

> ret = SSL_connect(ssl);

> if( ret != 1 ){

> err_quit("SSL connection failed:%s\n",

> ERR_reason_error_string(ERR_get_error()));

> }

> // https socket write.

> SSL_write(ssl, buf, strlen(buf));

> while((n = SSL_read(ssl, buf, BUF_LEN-1)) > 0){

> buf[n] = '\0';

> write(1, buf, n);

> }

> if(n != 0){

> err_quit("SSL read failed:%s\n",

> ERR_reason_error_string(ERR_get_error()));

> }

> // close ssl tunnel.

> ret = SSL_shutdown(ssl);

> if( ret != 1 ){

> close(fd);

> err_quit("SSL shutdown failed:%s\n",

> ERR_reason_error_string(ERR_get_error()));

> }

> // close the plain socket handler.

> close(fd);

> // clear ssl resource.

> SSL_free(ssl);

> SSL_CTX_free(ctx);

> ERR_free_strings();

> }

> }

> /* create common tcp socket connection */

> int create_tcpsocket(const char *host, const unsigned short port){

> int ret;


> char * transport = "tcp";

> struct hostent *phe; /* pointer to host information entry */

> struct protoent *ppe; /* pointer to protocol information entry */

> struct sockaddr_in sin; /* an Internet endpoint address */

> int s; /* socket descriptor and socket type */

> memset(&sin, 0, sizeof(sin));

> sin.sin_family = AF_INET;

> if ((sin.sin_port = htons(port)) == 0)

> err_quit("invalid port \"%d\"\n", port);

> /* Map host name to IP address, allowing for dotted decimal */

> if( phe = gethostbyname(host) )

> memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);

> else if( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )

> err_quit("can't get \"%s\" host entry\n", host);

> /* Map transport protocol name to protocol number */

> if ( (ppe = getprotobyname(transport)) == 0)

> err_quit("can't get \"%s\" protocol entry\n", transport);

> /* Allocate a common TCP socket */

> s = socket(PF_INET, SOCK_STREAM, ppe->p_proto);

> if (s < 0)

> err_quit("can't create socket: %s\n", strerror(errno));

> if(bIsHttps != 1){

> /* Connect the socket with timeout */

> fcntl(s,F_SETFL, O_NONBLOCK);

> if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1){

> if (errno == EINPROGRESS){// it is in the connect process

> struct timeval tv;

> fd_set writefds;

> tv.tv_sec = timeout_sec;

> tv.tv_usec = timeout_microsec;

> FD_ZERO(&writefds);

> FD_SET(s, &writefds);

> if(select(s+1,NULL,&writefds,NULL,&tv)>0){

> int len=sizeof(int);

> //下面的一句一定要，主要针对防火墙

> getsockopt(s, SOL_SOCKET, SO_ERROR, &errno, &len);

> if(errno != 0)

> ret = 1;

> else

> ret = 0;

> }

> else

> ret = 2;//timeout or error happen

> }

> else ret = 1;

> }

> else{

> ret = 1;

> }

> }

> else{

> /* create common tcp socket.seems

> non-block type is not supported by ssl. */

> ret = connect(s, (struct sockaddr *)&sin, sizeof(sin));

> }

> if(ret != 0){

> close(s);

> err_quit("can't connect to %s:%d\n", host, port);

> }

> return s;

> }

> /*

> s - SOCKET

> sec - timeout seconds

> usec - timeout microseconds

> x - select status

> */

> int xnet_select(int s, int sec, int usec, short x){

> int st = errno;

> struct timeval to;

> fd_set fs;

> to.tv_sec = sec;

> to.tv_usec = usec;

> FD_ZERO(&fs);

> FD_SET(s, &fs);

> switch(x){

> case READ_STATUS:

> st = select(s+1, &fs, 0, 0, &to);

> break;

> case WRITE_STATUS:

> st = select(s+1, 0, &fs, 0, &to);

> break;

> case EXCPT_STATUS:

> st = select(s+1, 0, 0, &fs, &to);

> break;

> }

> return(st);

> }

> void err_doit(int errnoflag, const char *fmt, va_list ap){

> int errno_save;

> char buf[MAX_STRING_LEN];

> errno_save = errno;

> vsprintf(buf, fmt, ap);

> if (errnoflag)

> sprintf(buf + strlen(buf), ": %s", strerror(errno_save));

> strcat(buf, "\n");

> fflush(stdout);

> fputs(buf, stderr);

> fflush(NULL);

> return;

> }

> /* Print a message and terminate. */

> void err_quit(const char *fmt, ...){

> va_list ap;

> va_start(ap, fmt);

> err_doit(0, fmt, ap);

> va_end(ap);

> exit(1);

> }

