# linux 心跳包的实现 - u013366022的专栏 - CSDN博客
2016年12月09日 15:58:55[slitaz](https://me.csdn.net/u013366022)阅读数：3448
在做游戏开发时，经常需要在应用层实现自己的心跳机制，即定时发送一个自定义的结构体（心跳包），让对方知道自己还活着，以确保连接的有效性。
在TCP socket心跳机制中，心跳包可以由服务器发送给客户端，也可以由客户端发送给服务器，不过比较起来，前者开销可能更大。—— 这里实现的是由客户端给服务器发送心跳包，基本思路是：
1） 服务器为每个客户端保存了IP和计数器count，即`map<fd, pair<ip, count>>`。服务端主线程采用 select 实现多路IO复用，监听新连接以及接受数据包（心跳包），子线程用于检测心跳：
- 如果主线程接收到的是心跳包，将该客户端对应的计数器 count 清零；
- 在子线程中，每隔3秒遍历一次所有客户端的计数器 count： 
- 若 count 小于 5，将 count 计数器加 1；
- 若 count 等于 5，说明已经15秒未收到该用户心跳包，判定该用户已经掉线；
2） 客户端则只是开辟子线程，定时给服务器发送心跳包（本示例中定时时间为3秒）。
下面是Linux下一个socket心跳包的简单实现：
```cpp
/*************************************************************************
    > File Name: Server.cpp
    > Author: SongLee
    > E-mail: lisong.shine@qq.com
    > Created Time: 2016年05月05日 星期四 22时50分23秒
    > Personal Blog: http://songlee24.github.io/
 ************************************************************************/
#include<netinet/in.h>   // sockaddr_in
#include<sys/types.h>    // socket
#include<sys/socket.h>   // socket
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h>   // select
#include<sys/ioctl.h>
#include<sys/time.h>
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<cstring>
using namespace std;
#define BUFFER_SIZE 1024
enum Type {HEART, OTHER};
struct PACKET_HEAD
{
    Type type;
    int length;
};
void* heart_handler(void* arg);
class Server
{
private:
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    int listen_fd;    // 监听的fd
    int max_fd;       // 最大的fd
    fd_set master_set;   // 所有fd集合，包括监听fd和客户端fd   
    fd_set working_set;  // 工作集合
    struct timeval timeout;
    map<int, pair<string, int> > mmap;   // 记录连接的客户端fd--><ip, count>
public:
    Server(int port);
    ~Server();
    void Bind();
    void Listen(int queue_len = 20);
    void Accept();
    void Run();
    void Recv(int nums);
    friend void* heart_handler(void* arg);
};
Server::Server(int port)
{
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);
    // create socket to listen
    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        cout << "Create Socket Failed!";
        exit(1);
    }
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}
Server::~Server()
{
    for(int fd=0; fd<=max_fd; ++fd)
    {
        if(FD_ISSET(fd, &master_set))
        {
            close(fd);
        }
    }
}
void Server::Bind()
{
    if(-1 == (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))))
    {
        cout << "Server Bind Failed!";
        exit(1);
    }
    cout << "Bind Successfully.\n"; 
}
void Server::Listen(int queue_len)
{
    if(-1 == listen(listen_fd, queue_len))
    {
        cout << "Server Listen Failed!";
        exit(1);
    }
    cout << "Listen Successfully.\n";
}
void Server::Accept()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int new_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(new_fd < 0)
    {
        cout << "Server Accept Failed!";
        exit(1);
    }
    string ip(inet_ntoa(client_addr.sin_addr));    // 获取客户端IP
    cout << ip << " new connection was accepted.\n";
    mmap.insert(make_pair(new_fd, make_pair(ip, 0)));
    // 将新建立的连接的fd加入master_set
    FD_SET(new_fd, &master_set);
    if(new_fd > max_fd)
    {
        max_fd = new_fd;
    }
}   
void Server::Recv(int nums)
{
    for(int fd=0; fd<=max_fd; ++fd)
    {
        if(FD_ISSET(fd, &working_set))
        {
            bool close_conn = false;  // 标记当前连接是否断开了
            PACKET_HEAD head;
            recv(fd, &head, sizeof(head), 0);   // 先接受包头
            if(head.type == HEART)
            {
                mmap[fd].second = 0;        // 每次收到心跳包，count置0
                cout << "Received heart-beat from client.\n";
            }
            else
            {
                // 数据包，通过head.length确认数据包长度 
            }   
            if(close_conn)  // 当前这个连接有问题，关闭它
            {
                close(fd);
                FD_CLR(fd, &master_set);
                if(fd == max_fd)  // 需要更新max_fd;
                {
                    while(FD_ISSET(max_fd, &master_set) == false)
                        --max_fd;
                }
            }
        }
    }   
}
void Server::Run()
{
    pthread_t id;     // 创建心跳检测线程
    int ret = pthread_create(&id, NULL, heart_handler, (void*)this);
    if(ret != 0)
    {
        cout << "Can not create heart-beat checking thread.\n";
    }
    max_fd = listen_fd;   // 初始化max_fd
    FD_ZERO(&master_set);
    FD_SET(listen_fd, &master_set);  // 添加监听fd
    while(1)
    {
        FD_ZERO(&working_set);
        memcpy(&working_set, &master_set, sizeof(master_set));
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;
        int nums = select(max_fd+1, &working_set, NULL, NULL, &timeout);
        if(nums < 0)
        {
            cout << "select() error!";
            exit(1);
        }
        if(nums == 0)
        {
            //cout << "select() is timeout!";
            continue;
        }
        if(FD_ISSET(listen_fd, &working_set))
            Accept();   // 有新的客户端请求
        else
            Recv(nums); // 接收客户端的消息
    }
}
// thread function
void* heart_handler(void* arg)
{
    cout << "The heartbeat checking thread started.\n";
    Server* s = (Server*)arg;
    while(1)
    {
        map<int, pair<string, int> >::iterator it = s->mmap.begin();
        for( ; it!=s->mmap.end(); )
        {   
            if(it->second.second == 5)   // 3s*5没有收到心跳包，判定客户端掉线
            {
                cout << "The client " << it->second.first << " has be offline.\n";
                int fd = it->first;
                close(fd);            // 关闭该连接
                FD_CLR(fd, &s->master_set);
                if(fd == s->max_fd)      // 需要更新max_fd;
                {
                    while(FD_ISSET(s->max_fd, &s->master_set) == false)
                        s->max_fd--;
                }
                s->mmap.erase(it++);  // 从map中移除该记录
            }
            else if(it->second.second < 5 && it->second.second >= 0)
            {
                it->second.second += 1;
                ++it;
            }
            else
            {
                ++it;
            }
        }
        sleep(3);   // 定时三秒
    }
}
int main()
{
    Server server(15000);
    server.Bind();
    server.Listen();
    server.Run();
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
```cpp
/*************************************************************************
    > File Name: Client.cpp
    > Author: SongLee
    > E-mail: lisong.shine@qq.com
    > Created Time: 2016年05月05日 星期四 23时41分56秒
    > Personal Blog: http://songlee24.github.io/
 ************************************************************************/
#include<netinet/in.h>   // sockaddr_in
#include<sys/types.h>    // socket
#include<sys/socket.h>   // socket
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<iostream>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<cstring>
using namespace std;
#define BUFFER_SIZE 1024
enum Type {HEART, OTHER};
struct PACKET_HEAD
{
    Type type;
    int length;
};
void* send_heart(void* arg); 
class Client 
{
private:
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    int fd;
public:
    Client(string ip, int port);
    ~Client();
    void Connect();
    void Run();
    friend void* send_heart(void* arg); 
};
Client::Client(string ip, int port)
{
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) == 0)
    {
        cout << "Server IP Address Error!";
        exit(1);
    }
    server_addr.sin_port = htons(port);
    server_addr_len = sizeof(server_addr);
    // create socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        cout << "Create Socket Failed!";
        exit(1);
    }
}
Client::~Client()
{
    close(fd);
}
void Client::Connect()
{
    cout << "Connecting......" << endl;
    if(connect(fd, (struct sockaddr*)&server_addr, server_addr_len) < 0)
    {
        cout << "Can not Connect to Server IP!";
        exit(1);
    }
    cout << "Connect to Server successfully." << endl;
}
void Client::Run()
{
    pthread_t id;
    int ret = pthread_create(&id, NULL, send_heart, (void*)this);
    if(ret != 0)
    {
        cout << "Can not create thread!";
        exit(1);
    }
}
// thread function
void* send_heart(void* arg)
{
    cout << "The heartbeat sending thread started.\n";
    Client* c = (Client*)arg;
    int count = 0;  // 测试
    while(1) 
    {
        PACKET_HEAD head;
        head.type = HEART;
        head.length = 0;    
        send(c->fd, &head, sizeof(head), 0);
        sleep(3);     // 定时3秒
        ++count;      // 测试：发送15次心跳包就停止发送
        if(count > 15)
            break;
    }
}
int main()
{
    Client client("127.0.0.1", 15000);
    client.Connect();
    client.Run();
    while(1)
    {
        string msg;
        getline(cin, msg);
        if(msg == "exit")
            break;
        cout << "msg\n";
    }
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
![](https://img-blog.csdn.net/20160506030636290)
可以看出，客户端启动以后发送了15次心跳包，然后停止发送心跳包。在经过一段时间后（3s*5），服务器就判断该客户端掉线，并断开了连接。
