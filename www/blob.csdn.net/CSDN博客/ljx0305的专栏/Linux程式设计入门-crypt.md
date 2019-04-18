# Linux程式设计入门 - crypt - ljx0305的专栏 - CSDN博客
2008年07月30日 16:05:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：475标签：[linux																[encryption																[buffer																[加密																[debian																[fp](https://so.csdn.net/so/search/s.do?q=fp&t=blog)](https://so.csdn.net/so/search/s.do?q=debian&t=blog)](https://so.csdn.net/so/search/s.do?q=加密&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=encryption&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
crypt是个密码加密函数，它是基於Data Encryption Standard(DES)演算法。 
crypt基本上是One way encryption，因此它只适用於密码的使用，不适合於资料加密。 
char *crypt(const char *key, const char *salt); 
key是使用者的密码。salt是两个字，每个字可从[a-zA-Z0-9./]中选出来，因 此同一密码增加了4096种可能性。透过使用key中每个字的低七位元，取得 56-bit关键字，这56-bit关键字被用来加密成一组字，这组字有13个可显示的 ASCII字，包含开头两个salt。 
crypt在您有自行管理使用者的场合时使用，例如会员网站、BBS等等。 
范例一 : crypt_word.c 
#include 
#include 
#include 
void main(int argc,char **argv) 
{ 
if (argc!=3) exit(0); 
printf("%s/n",crypt(argv[1],argv[2])); 
} 
编译 
gcc -o crypt_word crypt.c -lcrypt 
检验 
请先看您的/etc/passwd，找一个您自己的帐号，看前面两个字，那是您自己的 
salt。接下来输入: 
./crypt_word your_password salt 
看看它们是否相同(应该要相同，除非您加了crypt plugin或使用不同的crypt function，例如shadow、pam，这种状况下，加密字是不同的)，另外检验看看 他们是否为13个字。 
您也可以利用Apache上所附的htpasswd来产生加密字做为验证。 
范例二: verify_passwd.c 
注意，这个范例读取/etc/passwd的资料，不适用於使用shadow或已经使用pam 的系统(例如slackware，RedHat及Debian在不外加crypt plugin的状况下，应 当相同)。此范例仅供叁考，做为了解crypt函数运作的情形，真正撰写程式 时，应该避免类似的写法。 
#include 
#include 
#include 
typedef struct { 
char username[64]; 
char passwd[16]; 
int uid; 
int gid; 
char name[256]; 
char root[256]; 
char shell[256]; 
} account; 
/* 注意! 以下的写法，真实世界的软体开发状况下，千万不要用! */ 
int acc_info(char *info,account *user) 
{ 
char * start = info; 
char * now = info; 
/* username */ 
while (*now&&*now!=':') now++; /* 这是超级大安全漏洞 */ 
if (!*now) return 0; 
*now = 0; now++; 
strcpy(user->username,start); /* 这会导致buffer overflow */ 
start = now; 
/* passwd */ 
while (*now&&*now!=':') now++; /* 这是超级大安全漏洞 */ 
if (!*now) return 0; 
*now = 0; now++; 
strcpy(user->passwd,start); /* 这会导致buffer overflow */ 
start = now; 
/* uid */ 
while (*now&&*now!=':') now++; 
if (!*now) return 0; 
*now = 0; now++; 
user->uid = atoi(start); 
start = now; 
/* gid */ 
while (*now&&*now!=':') now++; 
if (!*now) return 0; 
*now = 0; now++; 
user->gid = atoi(start); 
start = now; 
/* name */ 
while (*now&&*now!=':') now++; /* 这是超级大安全漏洞 */ 
if (!*now) return 0; 
*now = 0; now++; 
strcpy(user->name,start); /* 这会导致buffer overflow */ 
start = now; 
/* root */ 
while (*now&&*now!=':') now++; /* 这是超级大安全漏洞 */ 
if (!*now) return 0; 
*now = 0; now++; 
strcpy(user->root,start); /* 这会导致buffer overflow */ 
start = now; 
/* shell */ 
while (*now&&*now!=':') now++; /* 这是超级大安全漏洞 */ 
*now = 0; now++; 
strcpy(user->shell,start); /* 这会导致buffer overflow */ 
start = now; 
return 1; 
} 
int read_password(char *filename,account *users) 
{ 
FILE *fp; 
char buf[1024]; 
int n; 
n = 0; 
fp = fopen(filename,"rt"); 
while (fgets(buf,1024,fp)!=NULL) { 
if (acc_info(buf,&users[n])) n++; 
} 
fclose(fp); 
return n; 
} 
void main(int argc,char **argv) 
{ 
int n,i,done; 
account ACC[128]; 
char username[256]; 
char password[256]; 
char * passwd; 
char salt[4]; 
if (argc<2) { 
printf("username:"); 
scanf("%s",username); /* 这是超级大安全漏洞 */ 
} else strcpy(username,argv[1]); /* 这是超级大安全漏洞 */ 
if (argc<3) { 
printf("password:"); 
scanf("%s",password); /* 这是超级大安全漏洞 */ 
} else strcpy(password,argv[2]); /* 这是超级大安全漏洞 */ 
n = read_password("/etc/passwd",ACC); 
for (i=0,done=0;i if (strcmp(username,ACC*.username)==0) { salt[0] = ACC*.passwd[0]; salt[1] = ACC*.passwd[1]; salt[2] = 0; passwd = crypt(password,salt); printf("%s %s %s/n",ACC*.username,ACC*.passwd,passwd); if (strcmp(passwd,ACC*.passwd)==0) { printf("login successfully!/n"); } else { printf("incorrect password!/n"); } done = 1; } if (!done) printf("invalid username!/n"); } 编译 gcc -o verify_passwd verify_passwd.c -lcrypt 检验 ./verify_passwd your_username your_password 避免安全漏洞 buffer overflow是个很严重的安全漏洞，通常您不可使用像char buf[xxxx]的 宣告。在这一类与安全有相关的任何程式写作中(不是只有密码，例如像 www/ftp/telnet的这一类对外窗口都要算在内)，您应该要先检查字串长度。例 如以下例子: len = strlen(incoming_username); if (len>xxx) invalid; new_string = (char*)malloc(len+1); strcpy(new_string,incoming_username); your_own_operations... 如此才能避免buffer overflow，万万不可滥做假设，切记切记，连许多数十年经验丰富的老手都会犯这个错误。 与crypt函数相关者尚有以下三个: void setkey (const char *key); void encrypt (char *block, int edflag); void swab (const char *from, char *to, ssize_t n); 一般来说，除非您有特殊需求，你不会用到这三个。 ******
*引用:http://www.chinalinuxpub.com/read.php?wid=103*
