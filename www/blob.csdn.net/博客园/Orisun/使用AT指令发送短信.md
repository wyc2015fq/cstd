# 使用AT指令发送短信 - Orisun - 博客园







# [使用AT指令发送短信](https://www.cnblogs.com/zhangchaoyang/articles/1914207.html)







#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<iconv.h>									//编码转换

#define STTY_DEV "/dev/ttyS0"
#define BUFF_SIZE 512
#define MSG_SIZE 140								//一条短信的容量

char phone[20]="+8613476090094";					//定义短信发送的目标手机号码
char sms_number[20]="+8613800200500";				//定义短消息服务中心号码
char sms_gb2312[MSG_SIZE]="Best Regards!";			//定义短消息内容
char sms_utf8[MSG_SIZE];
char *sms_in=sms_gb2312;							//要把输入的gb2312编码转换成unicode编码
char *sms_out=sms_utf8;
int gb2312_len, utf8_len;

int SetOption(int fd);								//设置串口通信的参数
void TransPhone();									//转换手机号码格式
void TransSmsc();									//转换SMSC号码
int TransMsg();										//转换短信消息内容

int main()
{
	int stty_fd,n;
	char buffer[BUFF_SIZE];
	
	//打开串口
	stty_fd=open(STTY_DEV,O_RDWR);
	if(-1==stty_fd){
		perror("open device");
		return 0;
	}
	printf("Open device success!\n");

	//设置串口参数
	if(0!=SetOption(stty_fd)){
		close(stty_fd);
		return 0;
	}
	printf("Set Serial option success!\n");
	
	TransPhone();
	TransSmsc();
	if(1!=TransMsg()){
		perror("Convert ShortMsg");
		close(stty_fd);
		return 0;
	}
	printf("Message convert success!\n");
	
	//设置使用PDU模式
	strcpy(buffer,"AT+CMGF=0\n");					//0表示PDU模式，1表示文本模式
	write(stty_fd,buffer,strlen(buffer));			//写入配置命令
	n=read(stty_fd,buffer,BUFF_SIZE);				//向串口写入AT+CMGF=0后，如果系统支持PDU模式，返回向串口返回“OK"
	if(n<=0){
		perror("set pdu mode");
		close(stty_fd);
		return 0;
	}
	if(0!=strncmp(buffer,"OK",2)){
		perror("set pdu mode");
		close(stty_fd);
		return 0;
	}
	
	//发送消息
	sprintf(buffer,"AT+CMGS=%d\n",utf8_len);		//写入发送消息命令
	write(stty_fd,buffer,strlen(buffer));
	write(stty_fd,sms_utf8,utf8_len);				//写入消息内容
	printf("Send message OK!\n");
	
	close(stty_fd);
}
	
int SetOption(int fd)
{
	struct termios opt;

	//获取当前串口配置
	tcgetattr(fd,&opt);
	tcflush(fd,TCIOFLUSH);

	//设置波特率
	cfsetispeed(&opt,B19200);
	cfsetospeed(&opt,B19200);

	//设置数据位--8位数据位
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;

	//设置奇偶位--无奇偶校验
	opt.c_cflag &= ~PARENB;
	opt.c_iflag &= ~INPCK;

	//设置停止位--1位停止位
	opt.c_cflag &= ~CSTOPB;

	//设置超时时间--15秒
	opt.c_cc[VTIME]=150;
	opt.c_cc[VMIN]=0;

	//设置写入设备
	if(0!=tcsetattr(fd,TCSANOW,&opt)){
		perror("set baudrate");
		return -1;
	}
	tcflush(fd,TCIOFLUSH);
	return 0;
}
void TransPhone()
{
	int i,str_len,tmp;
	if(phone[0]='+'){								//去掉号码开头的‘+’
		for(i=0;i<strlen(phone)-1;i++)				//strlen函数得到的是字符数组的真实长度，不包括后面的'\0'
			phone[i]=phone[i+1];
		phone[i]='\0';
	}
	str_len=strlen(phone);							//如果号码（不包括‘+’）是奇数，在最后加'F'
	if((strlen(phone)%2)!=0){
		phone[str_len]='F';
		phone[str_len+1]='\0';
	}
	for(i=0;i<strlen(phone);i+=2){					//把号码的奇偶位掉换
		tmp=phone[i];
		phone[i]=phone[i+1];
		phone[i+1]=tmp;
	}
}
void TransSmsc()
{
	int i,str_len,tmp;
	TransPhone(sms_number);
	str_len=strlen(sms_number);						//所有字符向后移动两个字节
	for(i=strlen(sms_number)+2;i!=0;i--)
		sms_number[i]=sms_number[i-2];
	sms_number[str_len+3]='\0';
	strncpy(sms_number,"91",2);						//开头写入字符“91”
	tmp=strlen(sms_number)/2;
	str_len=strlen(sms_number);						//所有字符向后移动两个字节
	for(i=strlen(sms_number)+2;i!=0;i--)
		sms_number[i]=sms_number[i-2];
	sms_number[str_len+3]='\0';
	sms_number[0]=(char)(tmp/10)+0x30;				//将字符串长度值由整形转换为字符型并写入短信字符串的开头部分
	sms_number[1]=(char)(tmp%10)+0x30;
}
int TransMsg()
{
	iconv_t cd=iconv_open("utf-8","gb2312");		//设置类型转换gb2312==>utf-8
	if(0==cd){
		perror("create iconv handle!");
		return 0;
	}
	gb2312_len=strlen(sms_gb2312);
	utf8_len=MSG_SIZE;
	if(-1==iconv(cd,&sms_in,&gb2312_len,&sms_out,&utf8_len)){
		perror("convert code");
		return 0;
	}
	iconv_close(cd);
	return 1;
}















