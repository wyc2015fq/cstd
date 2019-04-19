# wince下SE955激光头,SE655红光头的配置 - xqhrs232的专栏 - CSDN博客
2013年09月17日 21:35:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1487
原文地址::[http://hi.baidu.com/liguohei/item/1762367a3baea22b5c1789f1](http://hi.baidu.com/liguohei/item/1762367a3baea22b5c1789f1)
 网上搜索一下，发现关于SE955和SE655扫描头的，除了大量的供应商的广告和一本英文datasheet之外，相关资料少之又少。对于做扫描头的工作的，没有一丝的帮助。没法，这就是国内的现状，很多时候都是信息封闭，不愿共享出来。
        通过扫描头的datasheet可以知道，扫描头的参数设置有两种方式，第一种是扫描datasheet给出的设置条码，比如扫描datasheet给出的code-93的enable条码，这样扫描头就可以支持code-93类型的条码了（出厂时默认是不支持的），如果扫一次code-93的disable条码，扫描头就不再支持code-93类型的条码。
    第二种方式是通过串口来配置条码。wince下的配置如下：其他平台类似，只要读写相应的串口即可；
首先扫描头要上电，通过上层应用给出的接口，可以设置扫描头上电。上电以后就可以对其进行操作了。出厂默认为：low power模式，所以直接给扫描头发送命令很多是不能执行的，返回的只有一条错误码：0x05 0xD1 0x00 0x00 0x01 0xFF 0x29
//必须使用字符数组，否则不能正确操作
static char custom_default[]={0x05,0x12,0x04,0x00,0x00,0xFF,0xE5};
static char param_default[]={0x04,0xC8,0x04,0x00,0xFF,0x30};//factory default value
static char disable_all_code_type[]={0xC9,0x04,0x08,0x00};
static char sleep_mode[]={0xEB,0x04,0x00};
static char wakeup[]={0x00};
static char cmd_ack[]={0x04,0xD0,0x00,0x00,0xFF,0x2C};
static char cmd_nak[]={0x05,0xD1,0x00,0x00,0x01,0xFF,0x29};
static struct _SCANNER_MANAGER_{
int b_init;
HANDLE h_com;
OVERLAPPED o;
}SCANNER_MANAGER_ITEM;
/*
*2's complement sum of message contents excluding checksum
*param:  sum of message content
*/
static int __CheckSum(char data[],int d_size)
{
int count=0;
long sum=0;
if(data == NULL || d_size <= 0)
{
return 0;
}
while(d_size--)
{
sum+=(data[d_size] & 0xFF);
}
return ((~sum)+1);
}
static int ReadDataFromCom(void* data)
{
HANDLE h_com;
DWORD dwError;
COMSTAT stat;
int readed,data_len;
char RBuffer[4096]={0};
if(data == NULL)
{
return -1;
}
h_com=SCANNER_MANAGER_ITEM.h_com;
ClearCommError(h_com,&dwError,&stat);
data_len=(stat.cbInQue<4096)?stat.cbInQue:4096;
readed=0;
if(data_len>0) {
ReadFile(h_com,RBuffer,4096,&readed,NULL);
if(readed) { // success
memcpy(data,RBuffer,readed);
return readed;
}
}
return 0;
}
static int WriteDataToCom(void* data,int w_size)
{
HANDLE h_com;
int status=0;
int written=0;
if(data == NULL || w_size <= 0)
{
return 0;
}
h_com=SCANNER_MANAGER_ITEM.h_com;
PurgeComm(h_com,PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_RXABORT|PURGE_TXABORT); 
status=WriteFile(h_com,data,w_size,&written,NULL);
if(!status)//write failed
{
return 0;
}else
{
return written;
}
}
/*
*Init serail 
*Scanner Power On
*/
void Scanner1D_Moto_init()
{
if(!SCANNER_MANAGER_ITEM.b_init) {
Scanner_1D_power_on();            //根据用户自己平台对扫描头进行上电
memset(&SCANNER_MANAGER_ITEM,0,sizeof(SCANNER_MANAGER_ITEM));
{
HKEY h_rcom;
HANDLE h_com;
DWORD dwType;
DWORD dwValue;
DWORD dwsize;
TCHAR buf[20];
LPDCB lpDCB=malloc(sizeof(DCB));
dwsize=4;
RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Drivers\\BuiltIn\\IrCOMM"), 0, 0, &h_rcom);
RegQueryValueEx(h_rcom, _T("Index"), 0, &dwType, (BYTE*)&dwValue, &dwsize);
wsprintf(buf,L"COM%d:",dwValue);
SCANNER_MANAGER_ITEM.h_com=h_com=CreateFile(buf,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
if(h_com==INVALID_HANDLE_VALUE) {
return;
}
lpDCB=malloc(sizeof(DCB));
if(GetCommState(h_com,lpDCB)) {
SetupComm(h_com,1024,1024);
lpDCB->BaudRate=CBR_9600; 
lpDCB->Parity=NOPARITY; 
lpDCB->StopBits=ONESTOPBIT; 
lpDCB->ByteSize=8;
SetCommState(h_com,lpDCB);
SetCommMask(h_com,EV_RXCHAR);
Sleep(500);
PurgeComm(h_com,PURGE_RXCLEAR);
SCANNER_MANAGER_ITEM.b_init=1;
}
free(lpDCB);
SCANNER_MANAGER_ITEM.o.Internal=0;
SCANNER_MANAGER_ITEM.o.Offset=0;
SCANNER_MANAGER_ITEM.o.OffsetHigh=0;
SCANNER_MANAGER_ITEM.o.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
}
Sleep(100);
}
}
/*
*exit
*power off
*/
void scanner1D_Moto_exit()
{
if(SCANNER_MANAGER_ITEM.b_init) {
Scanner_1D_power_off();
CloseHandle(SCANNER_MANAGER_ITEM.h_com);
SCANNER_MANAGER_ITEM.h_com=NULL;
SCANNER_MANAGER_ITEM.b_init=0;
}
}
/*
*set custom default value
*/
int SetCustomDefaults()
{
WriteDataToCom(wakeup,sizeof(wakeup));//need wakeup at least remain on 1 second.
Sleep(10);
return WriteDataToCom(custom_default,sizeof(custom_default));
}
/*
*set factory default value
*/
int SetFactoryDefaults()
{
WriteDataToCom(wakeup,sizeof(wakeup));//need wakeup at least remain on 1 second.
Sleep(10);
return WriteDataToCom(param_default,sizeof(param_default));
}
/*
*get all/some paramter values
*/
static int GetParamValues(void* data,char ctrl[],int ctrl_len)
{
HWND h_com;
char buffer[4096]={0};
int len=1;
DWORD dwEvtMask = 0 ;
h_com=SCANNER_MANAGER_ITEM.h_com;
WriteDataToCom(wakeup,sizeof(wakeup));
Sleep(10);//Once the WAKEUP command is sent, the host must wait at least 10 msec
if(WriteDataToCom(ctrl,ctrl_len) > 0)
{
WaitCommEvent(h_com,&dwEvtMask,&(SCANNER_MANAGER_ITEM.o));
if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR) {//A character was received and placed in the input buffer
Sleep(50);            //needed
len=ReadDataFromCom(buffer);
}
if(len>0 && memcmp(buffer,cmd_nak,sizeof(cmd_nak)/sizeof(char)) != 0)//success
{
memcpy(data,buffer,len);
dwEvtMask=0;
return len;
}
}
return 0;
}
int ParseParamValues(char param_list[],int count,char *output)
{
char buffer[4096]={0};
char get_param[4096]={0};
char *p=NULL;
int len=0,i=0,values=0;
int check_sum=0;
FILE *fp=NULL;
char temp[]={0xC7,0x04,0x00};
if(param_list == NULL || count <= 0 || output == NULL)
{
return 0;
}
len=sizeof(temp)/sizeof(char);
get_param[0]=((len+count+1)&0xFF);
memcpy(get_param+1,temp,len);
memcpy(get_param+len+1,param_list,count);
check_sum=__CheckSum(get_param,len+count+1);
get_param[len+count+1]=((check_sum>>8) & 0xFF);
get_param[len+count+2]=((check_sum) & 0xFF);
len=GetParamValues(buffer,get_param,len+count+3);
p=buffer+5;//skip (length,opcode,src,status,beep-code)
memcpy(output,p,len-7);
return len-7;
}
/*
 return:  fail return -1
  success return 0
*/
int SetParamValues(char param_list[],int count)
{
int check_sum=0;
char temp[]={0xC6,0x04,0x08,0xFF};
int len=0;
char buffer[4096]={0};
if(param_list == NULL || count <= 0)
{
return -1;
}
len=sizeof(temp)/sizeof(char);
buffer[0]=((count+len+1)&0xFF);//length
memcpy(buffer+1,temp,len);
memcpy(buffer+len+1,param_list,count);
check_sum=__CheckSum(buffer,len+count+1);
buffer[len+count+1]=((check_sum>>8)&0xFF);//height byte
buffer[len+count+2]=(check_sum&0xFF);//low byte
WriteDataToCom(wakeup,sizeof(wakeup));
Sleep(10);
WriteDataToCom(buffer,len+count+3);//Length: 1 byte,CheckSum: 2 byte
return 0;
}
