# hypertable bug - 三少GG - CSDN博客
2013年11月10日 17:15:31[三少GG](https://me.csdn.net/scut1135)阅读数：786
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

hypertable bug
/**
 * A client for the ThriftBroker
 */
class Client : private ClientHelper, public ThriftGen::HqlServiceClient {
public:
  Client(const std::string &host, int port, int timeout_ms = 300000,
         bool open = true)
    : ClientHelper(host, port, timeout_ms), HqlServiceClient(protocol),
      m_do_close(false) {
    if (open) {
      transport->open();
      m_do_close = true;
    }
  }
  virtual ~Client() {
    if (m_do_close) {
      transport->close();
      m_do_close = false;
    }
  }
private:
  bool m_do_close;
};
在操作Hbase前，**需要先打开到Hbase Thrift service的连接，即 transport->open()，在操作完 Hbase之后，需要关闭连接，即 transport->close()**
4. 编写c++客户端
同样仿照tutorial，将tutorial/cpp中的Makefile和CppClient.cpp拷到hello/cpp下。
1) 将CppClient.cpp重命名为Client.cpp，并修改
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include "../gen-cpp/Hello.h"
#include <string>
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace demo;
using namespace boost;
int main(int argc, char** argv) {
  shared_ptr<TTransport> socket(new TSocket("localhost", 7911));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
HelloClient client(protocol);
**try {**
**    transport->open();**
    string ret;
client.helloString(ret, "world");
    printf("%s\n", ret.c_str());
    transport->close();
**  } catch (TException &tx) {**
    printf("ERROR: %s\n", tx.what());
  }
}
