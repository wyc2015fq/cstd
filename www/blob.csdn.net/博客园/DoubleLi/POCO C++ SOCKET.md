# POCO C++ SOCKET - DoubleLi - 博客园





- // client program  
- #include "Poco/Net/DatagramSocket.h"  
- #include "Poco/Net/SocketAddress.h"  
- #include "Poco/Timestamp.h"  
- #include "Poco/DateTimeFormatter.h"  
- 
- int testUdpClient(){  
- const char* ipaddr = "192.168.81.140"; //"192.168.81.140"  
-     Poco::Net::SocketAddress sa(ipaddr, 5004);  
-     Poco::Net::DatagramSocket dgs(Poco::Net::IPAddress::IPv4);  
-     dgs.connect(sa);  
- //dgs.bind(sa);  
-     std::string syslogMsg;  
-     Poco::Timestamp now;  
-     syslogMsg = Poco::DateTimeFormatter::format(now, "<14>%w %f %H:%M:%S Hello,world!");  
-     dgs.sendBytes(syslogMsg.data(), syslogMsg.size());  
- 
- return 0;  
- }  
- 
- /// server program  
- #include "Servers.h"  
- #include "Poco/Net/DatagramSocket.h"  
- #include "Poco/Net/SocketAddress.h"  
- #include "Poco/Timestamp.h"  
- #include "Poco/DateTimeFormatter.h"  
- int testDatagramSocket(){  
-     Poco::Net::SocketAddress sa(Poco::Net::IPAddress(), 5004);  
-     Poco::Net::DatagramSocket dgs(sa);  
- char buffer[1024]; // 1K byte  
- 
- while(1){  
-         Poco::Net::SocketAddress sender;  
- int n = dgs.receiveFrom(buffer, sizeof(buffer)-1, sender);  
-         buffer[n] = '\0';  
-         std::cout << sender.toString() << ":" << buffer << std::endl;  
-     }  
- 
- return 0;  
- }  


tcp

- // client program  
- #include "Poco/Net/SocketAddress.h"  
- #include "Poco/Net/StreamSocket.h"  
- #include "Poco/Net/SocketStream.h"  
- #include "Poco/StreamCopier.h"  
- #include <iostream>  
- 
- int main(int argc, char** argv){  
- const char* ipaddr = "192.168.81.140"; // the server address.  
-     Poco::Net::SocketAddress sa(ipaddr, 5004); // the server port.  
-     Poco::Net::StreamSocket socket(sa);  
-     Poco::Net::SocketStream str(socket);  
- 
- // Writes all bytes readable from str into std::cout, using an internal buffer.  
-     Poco::StreamCopier::copyStream(str, std::cout);  
- 
- return 0;  
- }  
- 
- // server program  
- #include "Poco/Net/ServerSocket.h"  
- #include "Poco/Net/StreamSocket.h"  
- #include "Poco/Net/SocketStream.h"  
- #include "Poco/Net/SocketAddress.h"  
- 
- int main(int argc, char** argv){  
-     Poco::Net::ServerSocket srv(5004); // does bind + listen  
- for(;;){  
-         Poco::Net::StreamSocket ss = srv.acceptConnection();  
-         Poco::Net::SocketStream str(ss);  
- 
- // flush() make sure the file stream is updated with the data.  
- // endl() puts a newline and then uses flush().  
-         str << "HTTP/1.0 200 OK\r\n"  
- "Content-Type: text/html\r\n"  
- "\r\n"  
- "<html><head><title>My 1st Web Server</title></head>"  
- "<body><h1>Hello,Wordl!</h1></body></html>"  
- "\r\n"  
-             << std::flush;  
-     }  
- return 0;  
- }  













