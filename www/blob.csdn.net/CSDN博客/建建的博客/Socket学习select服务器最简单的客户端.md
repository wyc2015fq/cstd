# Socket学习select服务器最简单的客户端 - 建建的博客 - CSDN博客
2017年01月28日 04:48:09[纪建](https://me.csdn.net/u013898698)阅读数：604
int _tmain(int argc, _TCHAR* argv[])
{
//初始化Winsock
WSADATA wsaDATA;
WORD version = MAKEWORD(2, 0);
int ret = WSAStartup(version, &wsaDATA);
if(ret != 0)
cout << "初始化失败!" << endl;
    SOCKET m_hSocket;
SOCKADDR_IN m_addr;
//创建新的套接字，这里是创建的流类型的套接字
m_hSocket = socket(AF_INET, SOCK_STREAM,0);
m_addr.sin_family = AF_INET;
m_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
m_addr.sin_port = 1234;
//这里主动连接服务器，该过程将等待一定时间
ret = 0;
int error = 0;
ret = connect(m_hSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));
if(ret == SOCKET_ERROR){
cout << "连接失败：" << (error = WSAGetLastError()) << endl;
if(error == 10061)
cout << "请确认服务器确实已经打开并工作在同样的端口！" << endl;
system("pause");
return 0;
}
while(1)
{
//发送数据到服务器
string buf1;
cin >> buf1;
ret = send(m_hSocket, buf1.c_str(), buf1.size(), 0);
if(ret != buf1.size()){
cout << "发送数据失败：" <<  WSAGetLastError() << endl;
system("pause");
return 0;
}
//等待服务器的响应
char buff[256];
ret = recv(m_hSocket, buff, 256, 0);
if(ret == 0){
cout << "接收数据失败：" << WSAGetLastError() << endl;
system("pause");
return 0;
}
buff[ret] = '\0';
cout << buff << endl;
}
system("pause");
return 0;
}
