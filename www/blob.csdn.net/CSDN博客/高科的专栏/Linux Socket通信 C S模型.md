
# Linux Socket通信 C-S模型 - 高科的专栏 - CSDN博客

2011年09月05日 16:34:26[高科](https://me.csdn.net/pbymw8iwm)阅读数：907


## 代码片段
## (8)
### [代码]
### MySocket.h
|01|\#ifndef _MYSOCKET_0623_H|
|02|\#define _MYSOCKET_0623_H|
|03|
|04|\#include <sys/socket.h>|
|05|\#include <sys/types.h>|
|06|\#include <arpa/inet.h>|
|07|\#include <netinet/in.h>|
|08|\#include <sys/wait.h>|
|09|\#include <unistd.h>|
|10|\#include <iostream>|
|11|\#include <string.h>|
|12|\#include <stdlib.h>|
|13|
|14|//Base class Socket|
|15|class|Socket|
|16|{|
|17|public|:|
|18|Socket();|
|19|virtual|~Socket();|
|20|bool|Create();|
|21|bool|Bind(|const|unsigned|short|port);|
|22|bool|Listen()|const|;|
|23|bool|Accept(Socket &new_sock)|const|;|
|24|bool|Connect(std::string host,|const|unsigned|short|port);|//client|
|25|bool|Send(|const|std::string s)|const|;|
|26|int|Recv(std::string &s)|const|;|
|27|void|Close();|
|28|bool|IsValid()|const|;|
|29|
|30|private|:|
|31|int|m_sock;|
|32|sockaddr_in m_addr;|
|33|};|
|34|
|35|//ServerSocket, Use Socket as a service|
|36|class|ServerSocket :|private|Socket|
|37|{|
|38|public|:|
|39|ServerSocket(){Create();}|//for normal data-exchange socket|
|40|ServerSocket(unsigned|short|port);|//for listen socket|
|41|virtual|~ServerSocket();|
|42|void|Accept(ServerSocket &new_sock)|const|;|
|43|void|Close();|
|44|const|ServerSocket& operator << (|const|std::string&)|const|;|
|45|const|ServerSocket& operator >> (std::string&)|const|;|
|46|};|
|47|
|48|//ClientSocket derived form Socket|
|49|class|ClientSocket :|private|Socket|
|50|{|
|51|public|:|
|52|ClientSocket(){Create();}|
|53|ClientSocket(std::string host,|const|unsigned|short|port);|
|54|virtual|~ClientSocket();|
|55|void|Close();|
|56|const|ClientSocket& operator << (|const|std::string&)|const|;|
|57|const|ClientSocket& operator >> (std::string&)|const|;|
|58|};|
|59|
|60|//SocketException|
|61|class|SocketException|
|62|{|
|63|public|:|
|64|SocketException(std::string msg = std::string(|"Socket Exception"|)): m_msg(msg){}|
|65|void|Print(){std::cout << m_msg << std::endl;}|
|66|private|:|
|67|const|std::string m_msg;|
|68|
|69|};|
|70|\#endif|


### [代码]
### MySocket.cpp
|001|\#include "MySocket.h"|
|002|\#include <assert.h>|
|003|using|namespace|std;|
|004|//Implement of Socket class|
|005|
|006|static|void|Trace(|const|string& msg =|"Socket Class Error"|)|
|007|{|
|008|\#ifdef DEBUG|
|009|cout << msg << endl;|
|010|\#endif|
|011|}|
|012|
|013|Socket::Socket()|
|014|{|
|015|m_sock = -1;|
|016|m_addr.sin_family = AF_INET;|
|017|m_addr.sin_addr.s_addr = inet_addr(|"127.0.0.1"|);|
|018|m_addr.sin_port = htons(8089);|
|019|}|
|020|
|021|Socket::~Socket()|
|022|{|
|023|if|(m_sock != -1)|//valid socket|
|024|{|
|025|Close();|
|026|}|
|027|}|
|028|
|029|bool|Socket::Create()|
|030|{|
|031|m_sock = socket(AF_INET, SOCK_STREAM, 0);|
|032|if|(m_sock == -1)|
|033|{|
|034|Trace(|"Create Socket Failure"|);|
|035|return|false|;|
|036|}|
|037|else|
|038|{|
|039|Trace(|"Create Socket Succeed"|);|
|040|return|true|;|
|041|}|
|042|}|
|043|
|044|bool|Socket::Bind(|const|unsigned|short|port)|
|045|{|
|046|assert|(m_sock != -1);|
|047|m_addr.sin_family = AF_INET;|
|048|m_addr.sin_addr.s_addr = inet_addr(|"127.0.0.1"|);|
|049|m_addr.sin_port = htons(port);|
|050|int|server_len =|sizeof|( m_addr );|
|051|if|( bind(m_sock, (|struct|sockaddr *)& m_addr, server_len) == -1 )|
|052|{|
|053|Trace(|"Server Bind Failure"|);|
|054|return|false|;|
|055|}|
|056|else|
|057|{|
|058|return|true|;|
|059|}|
|060|}|
|061|
|062|bool|Socket::Listen()|const|
|063|{|
|064|if|( listen(m_sock, 5) == -1 )|
|065|{|
|066|Trace(|"Server Listen Failure"|);|
|067|return|false|;|
|068|}|
|069|else|
|070|{|
|071|return|true|;|
|072|}|
|073|}|
|074|
|075|bool|Socket::Accept(Socket & new_socket)|const|//new_socket as a return|
|076|{|
|077|int|len =|sizeof|( new_socket.m_addr );|
|078|new_socket.m_sock = accept( m_sock,|
|079|(|struct|sockaddr *)& new_socket.m_addr, (socklen_t*)&len );|
|080|if|( new_socket.m_sock == -1 )|
|081|{|
|082|Trace(|"Server Accept Failure"|);|
|083|return|false|;|
|084|}|
|085|else|
|086|{|
|087|return|true|;|
|088|}|
|089|}|
|090|
|091|bool|Socket::Connect(|const|std::string host,|const|unsigned|short|port)|
|092|{|
|093|assert|(m_sock != -1);|
|094|m_addr.sin_family = AF_INET;|
|095|m_addr.sin_addr.s_addr = inet_addr(host.c_str());|
|096|m_addr.sin_port = htons(port);|
|097|int|len =|sizeof|(m_addr);|
|098|int|res = connect( m_sock, (|struct|sockaddr*)& m_addr, len);|
|099|if|(res == -1)|
|100|{|
|101|Trace(|"Client Connect Failure"|);|
|102|return|false|;|
|103|}|
|104|return|true|;|
|105|}|
|106|
|107|bool|Socket::Send(|const|std::string send_str)|const|
|108|{|
|109|//copy the send_str to a buffer so that we can send it.|
|110|size_t|len = send_str.size() + 1;|
|111|char|*send_buf = NULL;|
|112|send_buf =|new|char|[len];|
|113|if|(NULL == send_buf)|
|114|{|
|115|Trace(|"Socket: memory allocation failed in Send()"|);|
|116|return|false|;|
|117|}|
|118|char|t = send_str[0];|
|119|int|i = 0;|
|120|while|( t != 0 )|
|121|{|
|122|send_buf[i] = t;|
|123|i++;|
|124|t = send_str[i];|
|125|}|
|126|send_buf[i] = 0;|
|127|//end of copying string|
|128|assert|(i == len-1);|
|129|
|130|int|xoff = 0;|
|131|int|xs = 0;|
|132|do|
|133|{|
|134|xs = write(m_sock, send_buf+xoff, len-xoff);|
|135|if|(-1 == xs)|
|136|{|
|137|Trace(|"Socket: send data failed"|);|
|138|delete|[] send_buf;|
|139|return|false|;|
|140|}|
|141|else|
|142|{|
|143|xoff += xs;|
|144|}|
|145|}|while|(xoff < len);|
|146|Trace(|"Socket: send data succeed"|);|
|147|delete|[] send_buf;|
|148|return|true|;|
|149|}|
|150|
|151|int|Socket::Recv( std::string & recv_str)|const|
|152|{|
|153|cout <<|"enter recv"|<< endl;|
|154|char|*recv_buf;|
|155|recv_buf =|new|char|[256];|
|156|memset|(recv_buf, 0 ,256);|
|157|int|len = read(m_sock, recv_buf, 256);|
|158|if|(-1 == len)|
|159|{|
|160|Trace(|"Socket: recv data failed"|);|
|161|delete|[] recv_buf;|
|162|return|-1;|
|163|}|
|164|else|
|165|{|
|166|recv_str = recv_buf;|
|167|delete|[] recv_buf;|
|168|return|len;|
|169|}|
|170|}|
|171|
|172|
|173|bool|Socket::IsValid()|const|
|174|{|
|175|return|( m_sock != -1 );|
|176|}|
|177|
|178|void|Socket::Close()|
|179|{|
|180|if|(m_sock != -1)|
|181|{|
|182|close(m_sock);|
|183|m_sock = -1;|
|184|}|
|185|
|186|}|
|187|//////////////////////////////////////////////////////////////////|


### [代码]
### MyClientSocket.cpp
|01|\#include "MySocket.h"|
|02|using|namespace|std;|
|03|
|04|static|void|Trace(|const|string& msg =|"ServerSocket Class Error"|)|
|05|{|
|06|\#ifdef DEBUG|
|07|cout << msg << endl;|
|08|\#endif|
|09|}|
|10|
|11|ClientSocket::ClientSocket(std::string host,|const|unsigned|short|port)|
|12|{|
|13|if|( !Create() )|
|14|{|
|15|Trace(|"Client Socket Constructor Failed"|);|
|16|throw|SocketException(|"Client Socket Constructor Failed"|);|
|17|}|
|18|if|( !Connect(host, port) )|
|19|{|
|20|throw|SocketException(|"Client Socket Constructor Failed"|);|
|21|}|
|22|}|
|23|
|24|ClientSocket::~ClientSocket()|
|25|{|
|26|Close();|
|27|}|
|28|
|29|void|ClientSocket::Close()|
|30|{|
|31|Socket::Close();|
|32|}|
|33|
|34|const|ClientSocket& ClientSocket::operator << (|const|std::string &s)|const|
|35|{|
|36|if|( !Send(s) )|
|37|{|
|38|Trace(|"Could not write to server socket"|);|
|39|throw|SocketException(|"Could not write to client socket"|);|
|40|}|
|41|return|*|this|;|
|42|}|
|43|
|44|const|ClientSocket& ClientSocket::operator >> (std::string& s)|const|
|45|{|
|46|if|( Recv(s) == -1 )|
|47|{|
|48|Trace(|"Could not read from socket"|);|
|49|throw|SocketException(|"Could not read from client socket"|);|
|50|}|
|51|return|*|this|;|
|52|}|


### [代码]
### MyServerSocket.cpp
|01|\#include "MySocket.h"|
|02|\#include <assert.h>|
|03|using|namespace|std;|
|04|
|05|static|void|Trace(|const|string& msg =|"ServerSocket Class Error"|)|
|06|{|
|07|\#ifdef DEBUG|
|08|cout << msg << endl;|
|09|\#endif|
|10|}|
|11|
|12|ServerSocket::ServerSocket(unsigned|short|port)|
|13|{|
|14|if|( !Create() )|
|15|{|
|16|Trace(|"Could not create server socket!"|);|
|17|throw|SocketException(|"Could not create : server socket!"|);|
|18|}|
|19|if|( !Bind(port) )|
|20|{|
|21|Trace(|"Could not bind to port!"|);|
|22|throw|SocketException(|"Could not Bind : server socket!"|);|
|23|}|
|24|if|( !Listen() )|
|25|{|
|26|Trace(|"Could not listen to socket"|);|
|27|throw|SocketException(|"Could not Litsen : server socket!"|);|
|28|}|
|29|}|
|30|
|31|ServerSocket::~ServerSocket()|
|32|{|
|33|Close();|
|34|}|
|35|
|36|
|37|void|ServerSocket::Accept(ServerSocket& new_socket)|const|
|38|{|
|39|if|( !Socket::Accept(new_socket) )|
|40|{|
|41|throw|SocketException(|"Server Accept Failed"|);|
|42|}|
|43|}|
|44|
|45|void|ServerSocket::Close()|
|46|{|
|47|Socket::Close();|
|48|}|
|49|
|50|const|ServerSocket& ServerSocket::operator << (|const|std::string& s)|const|
|51|{|
|52|if|( !Send(s) )|
|53|{|
|54|Trace(|"Could not write to server socket"|);|
|55|throw|SocketException(|"Could not write to server socket"|);|
|56|}|
|57|return|*|this|;|
|58|}|
|59|
|60|const|ServerSocket& ServerSocket::operator >> (std::string& s)|const|
|61|{|
|62|int|ret = Recv(s);|
|63|if|( ret == -1 )|
|64|{|
|65|Trace(|"Could not read from server socket"|);|
|66|throw|SocketException(|"Could not read form server socket"|);|
|67|}|
|68|return|*|this|;|
|69|}|


### [代码]
### ServerTest.cpp
|01|//Create a server for testing|
|02|\#include "MySocket.h"|
|03|\#include "Calculator.h"|
|04|using|namespace|std;|
|05|int|main()|
|06|{|
|07|try|
|08|{|
|09|int|result;|
|10|ServerSocket server(30000);|
|11|
|12|string s;|
|13|while|(|true|)|
|14|{|
|15|ServerSocket new_sock;|
|16|server.Accept( new_sock );|
|17|while|(|true|)|
|18|{|
|19|new_sock >> s;|
|20|cout <<|"receive : \t"|<< s << endl;|
|21|//Deal with client requests|
|22|new_sock << s;|
|23|cout <<|"send: \t "|<< s << endl;|
|24|}|
|25|new_sock.Close();|
|26|}|
|27|
|28|}|
|29|catch|(SocketException &e)|
|30|{|
|31|cout <<|"SocketException:\t"|;|
|32|e.Print();|
|33|}|
|34|catch|(...)|
|35|{|
|36|cout <<|"Common Exception"|<< endl;|
|37|}|
|38|return|0;|
|39|}|


### [代码]
### ClientTest.cpp
|01|\#include "MySocket.h"|
|02|\#include "Calculator.h"|
|03|using|namespace|std;|
|04|
|05|int|main()|
|06|{|
|07|try|
|08|{|
|09|ClientSocket client(|"127.0.0.1"|,30000);|
|10|string s =|"HELLO"|;|
|11|
|12|int|count = 1;|
|13|while|(count <= 5)|
|14|{|
|15|cout <<|"input a string : "|;|
|16|cin >> s;|
|17|cout <<|"You have input "|<< s << endl;|
|18|client << s;|
|19|cout<<|"Send to Socket : "|<<s<<|'\t'|<<count<<endl;|
|20|client >> s;|
|21|cout<<|"Read from Socket : "|<<s<<|'\t'|<<count<<endl;|
|22|//sleep(2);|
|23|s =|"HELLO"|;|
|24|count++;|
|25|}|
|26|client.Close();|
|27|}|
|28|catch|(SocketException &e)|
|29|{|
|30|cout <<|"SocketException:\t"|;|
|31|e.Print();|
|32|}|
|33|///////////////////////////////////////////|
|34|
|35|return|0;|
|36|}|


### [代码]
### MakeClient
|01|objects = ClientTest.o MyClientSocket.o MySocket.o Calculator.o mathop.o|
|02|ClientTest.out: $(objects)|
|03|g++ $(objects) -o ClientTest.out|
|04|ClientTest.o: ClientTest.cpp MySocket.h  Calculator.h|
|05|g++ -c -g -Wall -pedantic -ansi -DDEBUG -o ClientTest.o ClientTest.cpp|
|06|MyClientSocket.o: MyClientSocket.cpp MySocket.h|
|07|g++ -c -g -Wall -pedantic -ansi -DDEBUG -o MyClientSocket.o MyClientSocket.cpp|
|08|MySocket.o: MySocket.cpp MySocket.h|
|09|g++ -c -g -Wall -pedantic -ansi -DDEBUG MySocket.cpp -o MySocket.o|
|10|Calculator.o: Calculator.cpp Calculator.h|
|11|g++ -g -c -Wall -pedantic -ansi -DDEBUG -o Calculator.o Calculator.cpp|
|12|mathop.o: mathop.cpp mathop.h|
|13|g++ -g -c -Wall -pedantic -ansi -DDEBUG -o mathop.o mathop.cpp|
|14|.PHONY: clean|
|15|clean:|
|16|-rm $(objects) ClientTest.out|
|17|


### [代码]
### MakeServer
|01|objects = ServerTest.o MyServerSocket.o MySocket.o Calculator.o mathop.o|
|02|ServerTest.out: $(objects)|
|03|g++ $(objects) -o ServerTest.out|
|04|ServerTest.o: ServerTest.cpp MySocket.h  Calculator.h|
|05|g++ -c -g -Wall -pedantic -ansi -DDEBUG -o ServerTest.o ServerTest.cpp|
|06|MyServerSocket.o: MyServerSocket.cpp MySocket.h|
|07|g++ -c -g -Wall -pedantic -ansi -DDEBUG -o MyServerSocket.o MyServerSocket.cpp|
|08|MySocket.o: MySocket.cpp MySocket.h|
|09|g++ -c -g -Wall -pedantic -ansi -DDEBUG MySocket.cpp -o MySocket.o|
|10|Calculator.o: Calculator.cpp Calculator.h|
|11|g++ -g -c -Wall -pedantic -ansi -DDEBUG -o Calculator.o Calculator.cpp|
|12|mathop.o: mathop.cpp mathop.h|
|13|g++ -g -c -Wall -pedantic -ansi -DDEBUG -o mathop.o mathop.cpp|
|14|.PHONY: clean|
|15|clean:|
|16|-rm $(objects) ServerTest.out|
|17|


