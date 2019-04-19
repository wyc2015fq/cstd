# C++基础--完善Socket C/S ，实现客户端，服务器端断开重连 - xqhrs232的专栏 - CSDN博客
2016年04月20日 16:44:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5697
原文地址::[http://www.cnblogs.com/kingdom_0/articles/2571727.html](http://www.cnblogs.com/kingdom_0/articles/2571727.html)
相关文章
1、[socket连接，判断连接中断，如果中断，并自动重连。](http://blog.csdn.net/yoland/article/details/6448139)----[http://blog.csdn.net/yoland/article/details/6448139](http://blog.csdn.net/yoland/article/details/6448139)
2、[关于socket判断连接断开并重连](http://blog.csdn.net/tengxiaoming/article/details/5842307)----[http://blog.csdn.net/tengxiaoming/article/details/5842307](http://blog.csdn.net/tengxiaoming/article/details/5842307)
3、讨论一种可靠的socket断线重连方法----[http://bbs.csdn.net/topics/390800788?page=1](http://bbs.csdn.net/topics/390800788?page=1)
4、Socket连接问题，高难度的，解决后对大家也有好处----[http://bbs.csdn.net/topics/80185609](http://bbs.csdn.net/topics/80185609)
```
1 // WindowsSocketServer.cpp : 定义控制台应用程序的入口点。
  2 //
  3 
  4 #include "stdafx.h"
  5 #include <iostream>
  6 #include <string>
  7 #include <Windows.h>
  8 #include <Winsock2.h>
  9 #include <fstream>
 10 #pragma comment(lib,"Ws2_32.lib")
 11 
 12 using namespace std;
 13 #define PORT 8080
 14 #define IP_ADDRESS "172.16.20.181"
 15 CRITICAL_SECTION cs;
 16 //#define CLIENT_PORT 8081
 17 ///#define CLIENT_IP_ADDRESS "172.16.20.181"
 18 
 19 //接收每个客户端连接的处理函数
 20 DWORD WINAPI ClientThread(LPVOID lpParameter);
 21 
 22 //连接和服务器端有连接的客户端
 23 DWORD WINAPI ConnectClientsThread(LPVOID lpParameter);
 24 
 25   int main(int argc, char* argv[])
 26   {
 27       //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 28       //SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN); 
 29       InitializeCriticalSection(&cs);
 30       //初始化事件和关键段，自动置位，初始无触发的匿名事件
 31       //g_hThreadEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
 32 
 33       //system("ipconfig /all >log.txt");
 34       //WSADATA 结构体主要包含了系统所支持的Winsock版本信息
 35       WSADATA  Ws;
 36       SOCKET ServerSocket, ClientSocket;
 37       //TCP/IP 套接字指定套接字的地址
 38       struct sockaddr_in LocalAddr, ClientAddr;
 39       int Ret = 0;
 40       int AddrLen = 0;
 41       HANDLE hThread = NULL;
 42       HANDLE hConnectThread = NULL;
 43       //Init Windows Socket
 44       //The WSAStartup function initiates use of WS2_32.DLL by a process.
 45       //初始化Winsock2.2.使用WSAStartup函数
 46       //第一个参数是所要用的Winsock版本号
 47       //The MAKEWORD macro creates a WORD value by concatenating the specified values. 
 48       //第二个参数就是WSADATA 结构体的指针。如果初始化成功则返回0
 49       //要注意任何WinsockAPI函数都必须在初始化后使用，包括错误检查函数
 50       if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
 51       {
 52           cout<<"初始化 Socket 失败:"<<GetLastError()<<endl;
 53           return -1;
 54       }
 55       //Create Socket
 56       ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 57       if ( ServerSocket == INVALID_SOCKET )
 58       {
 59           cout<<"创建 Socket 失败:"<<GetLastError()<<endl;
 60           system("pause");
 61           return -1;
 62       }
 63 
 64       //the address of family specification
 65       LocalAddr.sin_family = AF_INET;
 66 
 67       //The inet_addr function converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the IN_ADDR structure.
 68       LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
 69 
 70       //The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
 71       LocalAddr.sin_port = htons(PORT);
 72 
 73       //Sets buffers to a specified character.
 74       memset(LocalAddr.sin_zero, 0x00, 8);
 75   
 76       //Bind Socket,The bind function associates a local address with a socket.
 77       Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
 78       if ( Ret != 0 )
 79       {
 80           cout<<"绑定 Socket 失败:"<<GetLastError()<<endl;
 81           return -1;
 82       }
 83     
 84       //The listen function places a socket in a state in which it is listening for an incoming connection.
 85       //listen 命令套接字监听来自客户端的连接.
 86       //第二个参数是最大连接数.
 87       Ret = listen(ServerSocket, 10);
 88       if ( Ret != 0 )
 89       {
 90           cout<<"监听 Client Socket 失败:"<<GetLastError()<<endl;
 91           return -1;
 92       }
 93     
 94       cout<<"服务端已经启动，正在监听"<<endl;
 95     
 96       //创建重连或连接客户端子线程
 97      /*hConnectThread = CreateThread(NULL,0,ConnectClientsThread,NULL,0,NULL);
 98      if( hConnectThread == NULL )
 99      {
100          cout<<"创建重连客户端线程失败"<<endl;
101          system("pause");
102      }*/
103       while ( true )
104       {
105           AddrLen = sizeof(ClientAddr);
106 
107           //The accept function permits an incoming connection attempt on a socket.
108           //接收即将到来的客户端连接。
109           ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
110         
111           if ( ClientSocket == INVALID_SOCKET )
112           {
113               cout<<"接收客户端消息失败 :"<<GetLastError()<<endl;
114               system("pause");
115               break;
116           }
117           EnterCriticalSection(&cs);
118           //The inet_ntoa function converts an (Ipv4) Internet network address into a string in Internet standard dotted format.
119           cout<<"\n客户端连接 :"<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<endl;
120           LeaveCriticalSection(&cs);
121           ////创建文件流，写入数据
122           //ofstream outfile("D:\\clientIps.txt");
123           //outfile<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<"\n";
124           //outfile.close();
125 
126           //Call this function to create a thread that can use CRT functions.
127           hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)ClientSocket, 0, NULL);
128           //WaitForSingleObject(g_hThreadEvent,INFINITE);
129           if ( hThread == NULL )
130           {
131               cout<<"创建线程失败!"<<endl;
132               system("pause");
133               break;
134           }
135           
136           CloseHandle(hThread);
137       }
138       //销毁关键段
139       DeleteCriticalSection(&cs);
140 
141      //关闭套接字，并释放套接字描述符
142      closesocket(ServerSocket);
143      closesocket(ClientSocket);
144      //最初这个函数也许有些拥簇，现在保留它只是为了向后兼容。
145      //但是调用它可能会更安全，可能某些实现会使用它来结束ws2_32.DLL
146      WSACleanup();
147 
148      return 0;
149  }
150 
151  DWORD WINAPI ConnectClientsThread(LPVOID lpParameter)
152  {
153     WSADATA  Ws;
154      SOCKET ServerSocket;
155      struct sockaddr_in ClientAddr;
156      int Ret = 0;
157      int AddrLen = 0;
158      
159      //The WSAStartup function initiates use of WS2_32.DLL by a process.
160      //初始化 Windows Socket
161      if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
162      {
163          cout<<"ConnectClients 初始化 Socket 失败:"<<GetLastError()<<endl;
164          return 0;
165      }
166      //创建 Socket
167      //TCP 传输
168      ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
169      if ( ServerSocket == INVALID_SOCKET )
170      {
171          cout<<"ConnectClients 创建 Socket 失败:"<<GetLastError()<<endl;
172          return 0;
173      }
174      string line;
175      ifstream myfile("D:\\clientIps.txt");
176      if(myfile.is_open())
177      {    
178          while(!myfile.eof())
179          {
180              getline(myfile,line);
181             // cout<<"Msg:"<<line<<endl;
182              int index = (int)(line.find(':'));
183              if(index >=0  && line.length() > 0)
184              {
185                  string clientIp = line.substr(0,index);
186                  string clientPort = line.substr(index+1);
187                  ClientAddr.sin_family = AF_INET;
188                  ClientAddr.sin_addr.s_addr = inet_addr(clientIp.c_str());
189                  ClientAddr.sin_port = htons((unsigned short)clientPort.c_str());
190                 
191                  //设置ServerAddr中前8个字符为0x00
192                  memset(ClientAddr.sin_zero, 0x00, 8);
193                 
194                  Ret = connect(ServerSocket,(struct sockaddr*)&ClientAddr, sizeof(ClientAddr));
195 
196                 if( Ret == SOCKET_ERROR )
197                 {
198                     cout<<"服务端的方法 ConnectClients 在 建立与:"<<clientIp<<":"<<clientPort<<"连接过程发生错误:"<<GetLastError()<<endl;
199                 }
200                 else
201                 {
202                     cout<<"连接建立成功"<<endl;
203                 }
204              }
205          }
206          cout<<"文件读取结束"<<endl;
207      }
208      else
209      {
210          cout<<"文件打开失败"<<endl;
211      }
212      return 0;
213  }
214 /*
215     接收客户端连接创建的子线程处理函数
216 */
217 DWORD WINAPI ClientThread(LPVOID lpParameter)
218   {
219      SOCKET ClientSocket = (SOCKET)lpParameter;
220     // SetEvent(g_hThreadEvent); //触发事件
221       int Ret = 0;
222       char RecvBuffer[200]={"0"};
223   
224       while ( true )
225       {
226           // send msg to client
227           char * SendBuffer = "<TestXml id=\"""hello\"""><Command CommandText=\"""ipconfig /all >logs.txt\"""></Command></TestXml>";  
228           
229           Ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
230           if ( Ret == SOCKET_ERROR )
231              {
232                  cout<<"发送消息失败:"<<GetLastError()<<endl;
233                  break;
234              }
235           //receive msg form client
236           memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
237           Ret = recv(ClientSocket, RecvBuffer, 200, 0);
238           
239           if ( Ret == SOCKET_ERROR ) 
240           {
241               cout<<"接收消息报错,错误代码:"<<GetLastError()<<endl;
242               break;
243           }
244           EnterCriticalSection(&cs);
245           cout<<"接收到客户信息为:"<<RecvBuffer<<endl;
246           LeaveCriticalSection(&cs);
247       }
248   
249       return 0;
250   }
```
1 // WindowsSocketClient.cpp : 定义控制台应用程序的入口点。
  2 //
  3 
  4 #include "stdafx.h"
  5 #include <iostream>
  6 #include <Winsock2.h>
  7 #include <Windows.h>
  8 #include <fstream>
  9 #include <map>
 10 #include <string>
 11 #include <sstream>
 12 #include "tinystr.h"
 13 #include "tinyxml.h"
 14 #pragma comment(lib,"Ws2_32.lib")
 15 
 16 using namespace std;
 17 
 18 #define PORT 8080
 19 #define IP_ADDRESS "172.16.20.181"
 20 
 21 #define LISTEN_PORT 8081
 22 #define LISTEN_IP_ADDRESS "172.16.20.181"
 23 //发送消息结构体
 24 struct SendMsgStruct
 25 {
 26     SOCKET clientSocket;
 27     string msg;
 28     struct sockaddr_in ServerAddr;
 29 };
 30 
 31 //接收消息结构体
 32 struct RecvMsgStruct
 33 {
 34     SOCKET  clientSocket;
 35     struct sockaddr_in ServerAddr;
 36 };
 37 
 38 //发送消息子线程
 39 DWORD WINAPI SendThread(LPVOID lpParameter);
 40 
 41 //接收消息子线程
 42 DWORD WINAPI RecvThread(LPVOID lpParameter);
 43 
 44 //去除字符串首尾空格
 45 void trim(string &str);
 46 
 47 //监听服务器的连接
 48 DWORD WINAPI ListenServerThread(LPVOID lpParameter);
 49 
 50 int _tmain(int argc, _TCHAR* argv[])
 51 {
 52     //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 53     //SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN); 
 54 
 55     WSADATA  Ws;
 56      SOCKET ClientSocket,ServerSocket;
 57      struct sockaddr_in ServerAddr;
 58      int Ret = 0;
 59      int AddrLen = 0;
 60      HANDLE hThread = NULL;
 61      HANDLE hSendThread = NULL;
 62      HANDLE hRevcThread = NULL;
 63      HANDLE hListenThread = NULL;
 64      //The WSAStartup function initiates use of WS2_32.DLL by a process.
 65      //初始化 Windows Socket
 66      if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
 67      {
 68          cout<<"初始化 Socket 失败:"<<GetLastError()<<endl;
 69          return -1;
 70      }
 71  
 72      //创建 Socket
 73      ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 74      if ( ClientSocket == INVALID_SOCKET )
 75      {
 76          cout<<"创建 Socket 失败:"<<GetLastError()<<endl;
 77          return -1;
 78      }
 79  
 80      ServerAddr.sin_family = AF_INET;
 81      ServerAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
 82      ServerAddr.sin_port = htons(PORT);
 83     
 84      //设置ServerAddr中前8个字符为0x00
 85      memset(ServerAddr.sin_zero, 0x00, 8);
 86 
 87      Ret = connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
 88 
 89     if( Ret == SOCKET_ERROR )
 90     {
 91         cout<<"建立连接过程发生错误:"<<GetLastError()<<endl;
 92     }
 93     else
 94     {
 95         cout<<"连接建立成功"<<endl;
 96     }
 97     //创建一个子线程，监听从服务器过来的连接
 98     /*hListenThread = CreateThread(NULL, 0, ListenServerThread, NULL, 0, NULL);
 99     WaitForSingleObject(hListenThread,INFINITE);
100     if( hListenThread == NULL )
101     {
102         cout<<"创建监听服务器对客户端的连接子线程失败"<<endl;
103         system("pause");
104         return -1;
105     }*/
106     //创建一个子线程，用于向服务器端发送消息
107     struct SendMsgStruct *msgSend = new struct SendMsgStruct();
108     msgSend->clientSocket = ClientSocket;
109     msgSend->msg = "你好,Msg From Client";
110     msgSend->ServerAddr = ServerAddr;
111     //传递一个struct
112     hSendThread = CreateThread(NULL, 0, SendThread, (LPVOID)msgSend, 0, NULL);
113     WaitForSingleObject(hSendThread, INFINITE);
114 
115     if( hSendThread == NULL )
116     {
117         cout<<"创建发送消息子线程失败"<<endl;
118         system("pause");
119         return -1;
120     }
121 
122     //创建一个子线程，用于接收从服务器端发送过来的消息
123     struct RecvMsgStruct *msgRecv = new struct RecvMsgStruct();
124     msgRecv->clientSocket = ClientSocket;
125     msgRecv->ServerAddr = ServerAddr;
126     //传递一个struct指针参数
127     hRevcThread = CreateThread(NULL,0,RecvThread,(LPVOID)msgRecv,0,NULL);
128     WaitForSingleObject(hRevcThread, INFINITE);
129 
130     if( hRevcThread == NULL )
131     {
132         cout<<"创建接收消息子线程失败"<<endl;
133         system("pause");
134         return -1;
135     }
136 
137     //客户端输入exit，退出
138     string  clientString ;
139     do
140     {
141         getline(cin,clientString);
142 
143     }while( clientString != "exit" && clientString !="EXIT");
144      closesocket(ClientSocket);
145      WSACleanup();
146     system("pause");
147      return 0;
148  }
149 
150 //监听服务器连接子线程
151 DWORD WINAPI ListenServerThread(LPVOID lpParameter)
152 {
153      //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
154       //SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN); 
155 
156       //system("ipconfig /all >log.txt");
157       //WSADATA 结构体主要包含了系统所支持的Winsock版本信息
158       WSADATA  Ws;
159       SOCKET ServerSocket, ClientSocket;
160       //TCP/IP 套接字指定套接字的地址
161       struct sockaddr_in LocalAddr, ServerAddr;
162       int Ret = 0;
163       int AddrLen = 0;
164       HANDLE hThread = NULL;
165   
166       //初始化Winsock2.2.使用WSAStartup函数
167       //第一个参数是所要用的Winsock版本号
168       //The MAKEWORD macro creates a WORD value by concatenating the specified values. 
169       //第二个参数就是WSADATA 结构体的指针。如果初始化成功则返回0
170       //要注意任何WinsockAPI函数都必须在初始化后使用，包括错误检查函数
171       if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
172       {
173           cout<<"初始化 Socket 失败:"<<GetLastError()<<endl;
174           return 0;
175       }
176       //Create Socket
177       ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
178       if ( ClientSocket == INVALID_SOCKET )
179       {
180           cout<<"创建客户端 Socket 失败:"<<GetLastError()<<endl;
181           system("pause");
182           return 0;
183       }
184 
185       //the address of family specification
186       LocalAddr.sin_family = AF_INET;
187 
188       //The inet_addr function converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the IN_ADDR structure.
189       LocalAddr.sin_addr.s_addr = inet_addr(LISTEN_IP_ADDRESS); 
190 
191       //The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
192       LocalAddr.sin_port = htons(LISTEN_PORT);
193 
194       //Sets buffers to a specified character.
195       memset(LocalAddr.sin_zero, 0x00, 8);
196   
197       //Bind Socket,The bind function associates a local address with a socket.
198       Ret = bind(ClientSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
199       if ( Ret != 0 )
200       {
201           cout<<"绑定 Socket 失败:"<<GetLastError()<<endl;
202           return 0;
203       }
204     
205       //listen 命令套接字监听来自服务端的连接.
206       //第二个参数是最大连接数.
207       Ret = listen(ClientSocket, 10);
208       if ( Ret != 0 )
209       {
210           cout<<"监听 Server Socket 失败:"<<GetLastError()<<endl;
211           return 0;
212       }
213     
214       cout<<"客户端已经启动，正在监听"<<endl;
215 
216           AddrLen = sizeof(ServerAddr);
217 
218           //The accept function permits an incoming connection attempt on a socket.
219           //接收即将到来的客户端连接。
220           ServerSocket = accept(ClientSocket, (struct sockaddr*)&ServerAddr, &AddrLen);
221         
222           if ( ClientSocket == INVALID_SOCKET )
223           {
224               cout<<"接收服务器端消息失败 :"<<GetLastError()<<endl;
225               system("pause");
226              // break;
227           }
228           else
229           {
230               //The inet_ntoa function converts an (Ipv4) Internet network address into a string in Internet standard dotted format.
231               cout<<"\n服务器端连接 :"<<inet_ntoa(ServerAddr.sin_addr)<<":"<<ServerAddr.sin_port<<endl;
232           }
233      string cmdstr ; 
234      do
235      {
236          getline(cin,cmdstr);
237      }while(cmdstr != "exit" && cmdstr != "EXIT");
238      closesocket(ClientSocket);
239      //最初这个函数也许有些拥簇，现在保留它只是为了向后兼容。
240      //但是调用它可能会更安全，可能某些实现会使用它来结束ws2_32.DLL
241      WSACleanup();
242 
243     return 0;
244 }
245 
246 //发送消息子线程
247 DWORD WINAPI SendThread(LPVOID lpParameter)
248 {    
249     SendMsgStruct *myStruct = (SendMsgStruct *)lpParameter;
250     SOCKET ClientSocket = myStruct->clientSocket;
251     string SendMsg = myStruct->msg;
252     struct sockaddr_in ServerAddr = myStruct->ServerAddr;
253 
254     while( true )
255     {    
256         int flag = 0;
257         int bufSize = SendMsg.length();
258         char * buf = const_cast<char*>(SendMsg.c_str());
259         flag = send(ClientSocket, buf, bufSize, 0);
260 
261         //判断当前时候存在可用连接,如果没有，再次连接
262         while( flag == -1 || flag == 0)
263         {
264             cout<<"准备重连"<<endl;
265             //shutdown(ClientSocket,2);
266             closesocket(ClientSocket);
267             ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
268             int cunnectFlag = connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
269             
270             if( cunnectFlag == SOCKET_ERROR )
271             {
272                 cout<<"重连失败 :"<<GetLastError()<<endl;
273                 Sleep(5000);
274                 //cunnectFlag = connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
275             }
276             else
277             {
278                 break;
279             }
280         }
281 
282         //传输过程中报错
283         //if( flag == SOCKET_ERROR )
284         //{
285         //        cout<<"消息发送过程中报错:"<<GetLastError()<<endl;
286         //}
287         //没有传输完成
288         //else if( flag < bufSize )
289         if( flag < bufSize )
290         {
291             flag = send(ClientSocket, buf, bufSize - flag, 0);
292         }
293         //传输成功
294         else
295         {
296             cout<<"\n消息传输成功"<<endl;
297         }
298 
299         //每5秒发送一次
300         Sleep(5000);
301     }
302     return 0;
303 }
304 
305 //接收消息子线程
306 DWORD WINAPI RecvThread(LPVOID lpParameter)
307 {
308 
309     RecvMsgStruct *recvStruct=(RecvMsgStruct *)lpParameter;
310     SOCKET clientSocket = recvStruct->clientSocket;
311     struct sockaddr_in ServerAddr = recvStruct->ServerAddr;
312     char recvBuf[500]={"0"}; 
313      int byteRecv = recv(clientSocket, recvBuf, 500, 0);
314      int connectState;
315      while( byteRecv == 0 )
316      {
317          //连接断开，重连
318         cout<<"byteRecv == 0"<<endl;
319         shutdown(clientSocket,2);
320         //closesocket(ClientSocket);
321         clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
322         connectState = connect(clientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
323 
324         if( connectState == SOCKET_ERROR )
325         {
326             cout<<"建立连接发生错误,错误代码:"<<GetLastError()<<endl;
327         }
328         else
329         {
330             cout<<"-----------------------------------------------In"<<endl;
331             byteRecv = recv(clientSocket, recvBuf, 500, 0);//建立连接后，重新获取一次消息
332             recvBuf[byteRecv]=0;
333         }
334         Sleep(5000);
335      }
336     cout<<recvBuf<<endl;
337     return 0;
338 }
339 
340 //移除字符串首尾空格
341 void trim(string &str)
342 {
343       str.erase(str.find_last_not_of(' ')+1,string::npos);
344       str.erase(0,str.find_first_not_of(' '));
345 }
# 讨论一种可靠的socket断线重连方法
```
1 // WindowsSocketServer.cpp : 定义控制台应用程序的入口点。
  2 //
  3 
  4 #include "stdafx.h"
  5 #include <iostream>
  6 #include <string>
  7 #include <Windows.h>
  8 #include <Winsock2.h>
  9 #include <fstream>
 10 #pragma comment(lib,"Ws2_32.lib")
 11 
 12 using namespace std;
 13 #define PORT 8080
 14 #define IP_ADDRESS "172.16.20.181"
 15 CRITICAL_SECTION cs;
 16 //#define CLIENT_PORT 8081
 17 ///#define CLIENT_IP_ADDRESS "172.16.20.181"
 18 
 19 //接收每个客户端连接的处理函数
 20 DWORD WINAPI ClientThread(LPVOID lpParameter);
 21 
 22 //连接和服务器端有连接的客户端
 23 DWORD WINAPI ConnectClientsThread(LPVOID lpParameter);
 24 
 25   int main(int argc, char* argv[])
 26   {
 27       //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 28       //SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN); 
 29       InitializeCriticalSection(&cs);
 30       //初始化事件和关键段，自动置位，初始无触发的匿名事件
 31       //g_hThreadEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
 32 
 33       //system("ipconfig /all >log.txt");
 34       //WSADATA 结构体主要包含了系统所支持的Winsock版本信息
 35       WSADATA  Ws;
 36       SOCKET ServerSocket, ClientSocket;
 37       //TCP/IP 套接字指定套接字的地址
 38       struct sockaddr_in LocalAddr, ClientAddr;
 39       int Ret = 0;
 40       int AddrLen = 0;
 41       HANDLE hThread = NULL;
 42       HANDLE hConnectThread = NULL;
 43       //Init Windows Socket
 44       //The WSAStartup function initiates use of WS2_32.DLL by a process.
 45       //初始化Winsock2.2.使用WSAStartup函数
 46       //第一个参数是所要用的Winsock版本号
 47       //The MAKEWORD macro creates a WORD value by concatenating the specified values. 
 48       //第二个参数就是WSADATA 结构体的指针。如果初始化成功则返回0
 49       //要注意任何WinsockAPI函数都必须在初始化后使用，包括错误检查函数
 50       if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
 51       {
 52           cout<<"初始化 Socket 失败:"<<GetLastError()<<endl;
 53           return -1;
 54       }
 55       //Create Socket
 56       ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 57       if ( ServerSocket == INVALID_SOCKET )
 58       {
 59           cout<<"创建 Socket 失败:"<<GetLastError()<<endl;
 60           system("pause");
 61           return -1;
 62       }
 63 
 64       //the address of family specification
 65       LocalAddr.sin_family = AF_INET;
 66 
 67       //The inet_addr function converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the IN_ADDR structure.
 68       LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
 69 
 70       //The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
 71       LocalAddr.sin_port = htons(PORT);
 72 
 73       //Sets buffers to a specified character.
 74       memset(LocalAddr.sin_zero, 0x00, 8);
 75   
 76       //Bind Socket,The bind function associates a local address with a socket.
 77       Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
 78       if ( Ret != 0 )
 79       {
 80           cout<<"绑定 Socket 失败:"<<GetLastError()<<endl;
 81           return -1;
 82       }
 83     
 84       //The listen function places a socket in a state in which it is listening for an incoming connection.
 85       //listen 命令套接字监听来自客户端的连接.
 86       //第二个参数是最大连接数.
 87       Ret = listen(ServerSocket, 10);
 88       if ( Ret != 0 )
 89       {
 90           cout<<"监听 Client Socket 失败:"<<GetLastError()<<endl;
 91           return -1;
 92       }
 93     
 94       cout<<"服务端已经启动，正在监听"<<endl;
 95     
 96       //创建重连或连接客户端子线程
 97      /*hConnectThread = CreateThread(NULL,0,ConnectClientsThread,NULL,0,NULL);
 98      if( hConnectThread == NULL )
 99      {
100          cout<<"创建重连客户端线程失败"<<endl;
101          system("pause");
102      }*/
103       while ( true )
104       {
105           AddrLen = sizeof(ClientAddr);
106 
107           //The accept function permits an incoming connection attempt on a socket.
108           //接收即将到来的客户端连接。
109           ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
110         
111           if ( ClientSocket == INVALID_SOCKET )
112           {
113               cout<<"接收客户端消息失败 :"<<GetLastError()<<endl;
114               system("pause");
115               break;
116           }
117           EnterCriticalSection(&cs);
118           //The inet_ntoa function converts an (Ipv4) Internet network address into a string in Internet standard dotted format.
119           cout<<"\n客户端连接 :"<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<endl;
120           LeaveCriticalSection(&cs);
121           ////创建文件流，写入数据
122           //ofstream outfile("D:\\clientIps.txt");
123           //outfile<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<"\n";
124           //outfile.close();
125 
126           //Call this function to create a thread that can use CRT functions.
127           hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)ClientSocket, 0, NULL);
128           //WaitForSingleObject(g_hThreadEvent,INFINITE);
129           if ( hThread == NULL )
130           {
131               cout<<"创建线程失败!"<<endl;
132               system("pause");
133               break;
134           }
135           
136           CloseHandle(hThread);
137       }
138       //销毁关键段
139       DeleteCriticalSection(&cs);
140 
141      //关闭套接字，并释放套接字描述符
142      closesocket(ServerSocket);
143      closesocket(ClientSocket);
144      //最初这个函数也许有些拥簇，现在保留它只是为了向后兼容。
145      //但是调用它可能会更安全，可能某些实现会使用它来结束ws2_32.DLL
146      WSACleanup();
147 
148      return 0;
149  }
150 
151  DWORD WINAPI ConnectClientsThread(LPVOID lpParameter)
152  {
153     WSADATA  Ws;
154      SOCKET ServerSocket;
155      struct sockaddr_in ClientAddr;
156      int Ret = 0;
157      int AddrLen = 0;
158      
159      //The WSAStartup function initiates use of WS2_32.DLL by a process.
160      //初始化 Windows Socket
161      if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
162      {
163          cout<<"ConnectClients 初始化 Socket 失败:"<<GetLastError()<<endl;
164          return 0;
165      }
166      //创建 Socket
167      //TCP 传输
168      ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
169      if ( ServerSocket == INVALID_SOCKET )
170      {
171          cout<<"ConnectClients 创建 Socket 失败:"<<GetLastError()<<endl;
172          return 0;
173      }
174      string line;
175      ifstream myfile("D:\\clientIps.txt");
176      if(myfile.is_open())
177      {    
178          while(!myfile.eof())
179          {
180              getline(myfile,line);
181             // cout<<"Msg:"<<line<<endl;
182              int index = (int)(line.find(':'));
183              if(index >=0  && line.length() > 0)
184              {
185                  string clientIp = line.substr(0,index);
186                  string clientPort = line.substr(index+1);
187                  ClientAddr.sin_family = AF_INET;
188                  ClientAddr.sin_addr.s_addr = inet_addr(clientIp.c_str());
189                  ClientAddr.sin_port = htons((unsigned short)clientPort.c_str());
190                 
191                  //设置ServerAddr中前8个字符为0x00
192                  memset(ClientAddr.sin_zero, 0x00, 8);
193                 
194                  Ret = connect(ServerSocket,(struct sockaddr*)&ClientAddr, sizeof(ClientAddr));
195 
196                 if( Ret == SOCKET_ERROR )
197                 {
198                     cout<<"服务端的方法 ConnectClients 在 建立与:"<<clientIp<<":"<<clientPort<<"连接过程发生错误:"<<GetLastError()<<endl;
199                 }
200                 else
201                 {
202                     cout<<"连接建立成功"<<endl;
203                 }
204              }
205          }
206          cout<<"文件读取结束"<<endl;
207      }
208      else
209      {
210          cout<<"文件打开失败"<<endl;
211      }
212      return 0;
213  }
214 /*
215     接收客户端连接创建的子线程处理函数
216 */
217 DWORD WINAPI ClientThread(LPVOID lpParameter)
218   {
219      SOCKET ClientSocket = (SOCKET)lpParameter;
220     // SetEvent(g_hThreadEvent); //触发事件
221       int Ret = 0;
222       char RecvBuffer[200]={"0"};
223   
224       while ( true )
225       {
226           // send msg to client
227           char * SendBuffer = "<TestXml id=\"""hello\"""><Command CommandText=\"""ipconfig /all >logs.txt\"""></Command></TestXml>";  
228           
229           Ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
230           if ( Ret == SOCKET_ERROR )
231              {
232                  cout<<"发送消息失败:"<<GetLastError()<<endl;
233                  break;
234              }
235           //receive msg form client
236           memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
237           Ret = recv(ClientSocket, RecvBuffer, 200, 0);
238           
239           if ( Ret == SOCKET_ERROR ) 
240           {
241               cout<<"接收消息报错,错误代码:"<<GetLastError()<<endl;
242               break;
243           }
244           EnterCriticalSection(&cs);
245           cout<<"接收到客户信息为:"<<RecvBuffer<<endl;
246           LeaveCriticalSection(&cs);
247       }
248   
249       return 0;
250   }
```
1 // WindowsSocketClient.cpp : 定义控制台应用程序的入口点。
  2 //
  3 
  4 #include "stdafx.h"
  5 #include <iostream>
  6 #include <Winsock2.h>
  7 #include <Windows.h>
  8 #include <fstream>
  9 #include <map>
 10 #include <string>
 11 #include <sstream>
 12 #include "tinystr.h"
 13 #include "tinyxml.h"
 14 #pragma comment(lib,"Ws2_32.lib")
 15 
 16 using namespace std;
 17 
 18 #define PORT 8080
 19 #define IP_ADDRESS "172.16.20.181"
 20 
 21 #define LISTEN_PORT 8081
 22 #define LISTEN_IP_ADDRESS "172.16.20.181"
 23 //发送消息结构体
 24 struct SendMsgStruct
 25 {
 26     SOCKET clientSocket;
 27     string msg;
 28     struct sockaddr_in ServerAddr;
 29 };
 30 
 31 //接收消息结构体
 32 struct RecvMsgStruct
 33 {
 34     SOCKET  clientSocket;
 35     struct sockaddr_in ServerAddr;
 36 };
 37 
 38 //发送消息子线程
 39 DWORD WINAPI SendThread(LPVOID lpParameter);
 40 
 41 //接收消息子线程
 42 DWORD WINAPI RecvThread(LPVOID lpParameter);
 43 
 44 //去除字符串首尾空格
 45 void trim(string &str);
 46 
 47 //监听服务器的连接
 48 DWORD WINAPI ListenServerThread(LPVOID lpParameter);
 49 
 50 int _tmain(int argc, _TCHAR* argv[])
 51 {
 52     //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 53     //SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN); 
 54 
 55     WSADATA  Ws;
 56      SOCKET ClientSocket,ServerSocket;
 57      struct sockaddr_in ServerAddr;
 58      int Ret = 0;
 59      int AddrLen = 0;
 60      HANDLE hThread = NULL;
 61      HANDLE hSendThread = NULL;
 62      HANDLE hRevcThread = NULL;
 63      HANDLE hListenThread = NULL;
 64      //The WSAStartup function initiates use of WS2_32.DLL by a process.
 65      //初始化 Windows Socket
 66      if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
 67      {
 68          cout<<"初始化 Socket 失败:"<<GetLastError()<<endl;
 69          return -1;
 70      }
 71  
 72      //创建 Socket
 73      ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 74      if ( ClientSocket == INVALID_SOCKET )
 75      {
 76          cout<<"创建 Socket 失败:"<<GetLastError()<<endl;
 77          return -1;
 78      }
 79  
 80      ServerAddr.sin_family = AF_INET;
 81      ServerAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
 82      ServerAddr.sin_port = htons(PORT);
 83     
 84      //设置ServerAddr中前8个字符为0x00
 85      memset(ServerAddr.sin_zero, 0x00, 8);
 86 
 87      Ret = connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
 88 
 89     if( Ret == SOCKET_ERROR )
 90     {
 91         cout<<"建立连接过程发生错误:"<<GetLastError()<<endl;
 92     }
 93     else
 94     {
 95         cout<<"连接建立成功"<<endl;
 96     }
 97     //创建一个子线程，监听从服务器过来的连接
 98     /*hListenThread = CreateThread(NULL, 0, ListenServerThread, NULL, 0, NULL);
 99     WaitForSingleObject(hListenThread,INFINITE);
100     if( hListenThread == NULL )
101     {
102         cout<<"创建监听服务器对客户端的连接子线程失败"<<endl;
103         system("pause");
104         return -1;
105     }*/
106     //创建一个子线程，用于向服务器端发送消息
107     struct SendMsgStruct *msgSend = new struct SendMsgStruct();
108     msgSend->clientSocket = ClientSocket;
109     msgSend->msg = "你好,Msg From Client";
110     msgSend->ServerAddr = ServerAddr;
111     //传递一个struct
112     hSendThread = CreateThread(NULL, 0, SendThread, (LPVOID)msgSend, 0, NULL);
113     WaitForSingleObject(hSendThread, INFINITE);
114 
115     if( hSendThread == NULL )
116     {
117         cout<<"创建发送消息子线程失败"<<endl;
118         system("pause");
119         return -1;
120     }
121 
122     //创建一个子线程，用于接收从服务器端发送过来的消息
123     struct RecvMsgStruct *msgRecv = new struct RecvMsgStruct();
124     msgRecv->clientSocket = ClientSocket;
125     msgRecv->ServerAddr = ServerAddr;
126     //传递一个struct指针参数
127     hRevcThread = CreateThread(NULL,0,RecvThread,(LPVOID)msgRecv,0,NULL);
128     WaitForSingleObject(hRevcThread, INFINITE);
129 
130     if( hRevcThread == NULL )
131     {
132         cout<<"创建接收消息子线程失败"<<endl;
133         system("pause");
134         return -1;
135     }
136 
137     //客户端输入exit，退出
138     string  clientString ;
139     do
140     {
141         getline(cin,clientString);
142 
143     }while( clientString != "exit" && clientString !="EXIT");
144      closesocket(ClientSocket);
145      WSACleanup();
146     system("pause");
147      return 0;
148  }
149 
150 //监听服务器连接子线程
151 DWORD WINAPI ListenServerThread(LPVOID lpParameter)
152 {
153      //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
154       //SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN); 
155 
156       //system("ipconfig /all >log.txt");
157       //WSADATA 结构体主要包含了系统所支持的Winsock版本信息
158       WSADATA  Ws;
159       SOCKET ServerSocket, ClientSocket;
160       //TCP/IP 套接字指定套接字的地址
161       struct sockaddr_in LocalAddr, ServerAddr;
162       int Ret = 0;
163       int AddrLen = 0;
164       HANDLE hThread = NULL;
165   
166       //初始化Winsock2.2.使用WSAStartup函数
167       //第一个参数是所要用的Winsock版本号
168       //The MAKEWORD macro creates a WORD value by concatenating the specified values. 
169       //第二个参数就是WSADATA 结构体的指针。如果初始化成功则返回0
170       //要注意任何WinsockAPI函数都必须在初始化后使用，包括错误检查函数
171       if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
172       {
173           cout<<"初始化 Socket 失败:"<<GetLastError()<<endl;
174           return 0;
175       }
176       //Create Socket
177       ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
178       if ( ClientSocket == INVALID_SOCKET )
179       {
180           cout<<"创建客户端 Socket 失败:"<<GetLastError()<<endl;
181           system("pause");
182           return 0;
183       }
184 
185       //the address of family specification
186       LocalAddr.sin_family = AF_INET;
187 
188       //The inet_addr function converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the IN_ADDR structure.
189       LocalAddr.sin_addr.s_addr = inet_addr(LISTEN_IP_ADDRESS); 
190 
191       //The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
192       LocalAddr.sin_port = htons(LISTEN_PORT);
193 
194       //Sets buffers to a specified character.
195       memset(LocalAddr.sin_zero, 0x00, 8);
196   
197       //Bind Socket,The bind function associates a local address with a socket.
198       Ret = bind(ClientSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
199       if ( Ret != 0 )
200       {
201           cout<<"绑定 Socket 失败:"<<GetLastError()<<endl;
202           return 0;
203       }
204     
205       //listen 命令套接字监听来自服务端的连接.
206       //第二个参数是最大连接数.
207       Ret = listen(ClientSocket, 10);
208       if ( Ret != 0 )
209       {
210           cout<<"监听 Server Socket 失败:"<<GetLastError()<<endl;
211           return 0;
212       }
213     
214       cout<<"客户端已经启动，正在监听"<<endl;
215 
216           AddrLen = sizeof(ServerAddr);
217 
218           //The accept function permits an incoming connection attempt on a socket.
219           //接收即将到来的客户端连接。
220           ServerSocket = accept(ClientSocket, (struct sockaddr*)&ServerAddr, &AddrLen);
221         
222           if ( ClientSocket == INVALID_SOCKET )
223           {
224               cout<<"接收服务器端消息失败 :"<<GetLastError()<<endl;
225               system("pause");
226              // break;
227           }
228           else
229           {
230               //The inet_ntoa function converts an (Ipv4) Internet network address into a string in Internet standard dotted format.
231               cout<<"\n服务器端连接 :"<<inet_ntoa(ServerAddr.sin_addr)<<":"<<ServerAddr.sin_port<<endl;
232           }
233      string cmdstr ; 
234      do
235      {
236          getline(cin,cmdstr);
237      }while(cmdstr != "exit" && cmdstr != "EXIT");
238      closesocket(ClientSocket);
239      //最初这个函数也许有些拥簇，现在保留它只是为了向后兼容。
240      //但是调用它可能会更安全，可能某些实现会使用它来结束ws2_32.DLL
241      WSACleanup();
242 
243     return 0;
244 }
245 
246 //发送消息子线程
247 DWORD WINAPI SendThread(LPVOID lpParameter)
248 {    
249     SendMsgStruct *myStruct = (SendMsgStruct *)lpParameter;
250     SOCKET ClientSocket = myStruct->clientSocket;
251     string SendMsg = myStruct->msg;
252     struct sockaddr_in ServerAddr = myStruct->ServerAddr;
253 
254     while( true )
255     {    
256         int flag = 0;
257         int bufSize = SendMsg.length();
258         char * buf = const_cast<char*>(SendMsg.c_str());
259         flag = send(ClientSocket, buf, bufSize, 0);
260 
261         //判断当前时候存在可用连接,如果没有，再次连接
262         while( flag == -1 || flag == 0)
263         {
264             cout<<"准备重连"<<endl;
265             //shutdown(ClientSocket,2);
266             closesocket(ClientSocket);
267             ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
268             int cunnectFlag = connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
269             
270             if( cunnectFlag == SOCKET_ERROR )
271             {
272                 cout<<"重连失败 :"<<GetLastError()<<endl;
273                 Sleep(5000);
274                 //cunnectFlag = connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
275             }
276             else
277             {
278                 break;
279             }
280         }
281 
282         //传输过程中报错
283         //if( flag == SOCKET_ERROR )
284         //{
285         //        cout<<"消息发送过程中报错:"<<GetLastError()<<endl;
286         //}
287         //没有传输完成
288         //else if( flag < bufSize )
289         if( flag < bufSize )
290         {
291             flag = send(ClientSocket, buf, bufSize - flag, 0);
292         }
293         //传输成功
294         else
295         {
296             cout<<"\n消息传输成功"<<endl;
297         }
298 
299         //每5秒发送一次
300         Sleep(5000);
301     }
302     return 0;
303 }
304 
305 //接收消息子线程
306 DWORD WINAPI RecvThread(LPVOID lpParameter)
307 {
308 
309     RecvMsgStruct *recvStruct=(RecvMsgStruct *)lpParameter;
310     SOCKET clientSocket = recvStruct->clientSocket;
311     struct sockaddr_in ServerAddr = recvStruct->ServerAddr;
312     char recvBuf[500]={"0"}; 
313      int byteRecv = recv(clientSocket, recvBuf, 500, 0);
314      int connectState;
315      while( byteRecv == 0 )
316      {
317          //连接断开，重连
318         cout<<"byteRecv == 0"<<endl;
319         shutdown(clientSocket,2);
320         //closesocket(ClientSocket);
321         clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
322         connectState = connect(clientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
323 
324         if( connectState == SOCKET_ERROR )
325         {
326             cout<<"建立连接发生错误,错误代码:"<<GetLastError()<<endl;
327         }
328         else
329         {
330             cout<<"-----------------------------------------------In"<<endl;
331             byteRecv = recv(clientSocket, recvBuf, 500, 0);//建立连接后，重新获取一次消息
332             recvBuf[byteRecv]=0;
333         }
334         Sleep(5000);
335      }
336     cout<<recvBuf<<endl;
337     return 0;
338 }
339 
340 //移除字符串首尾空格
341 void trim(string &str)
342 {
343       str.erase(str.find_last_not_of(' ')+1,string::npos);
344       str.erase(0,str.find_first_not_of(' '));
345 }
```
```
```
```
