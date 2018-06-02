
#ifndef _WEBSOCKET_INL_
#define _WEBSOCKET_INL_
#include "net.h"
#include "net/net.inl"

#include <string>

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


static inline bool is_base64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);

    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4) ; i++) {
        ret += base64_chars[char_array_4[i]];
      }

      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++) {
      char_array_3[j] = '\0';
    }

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++) {
      ret += base64_chars[char_array_4[j]];
    }

    while ((i++ < 3)) {
      ret += '=';
    }

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string)
{
  size_t in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_];
    in_++;

    if (i == 4) {
      for (i = 0; i < 4; i++) {
        char_array_4[i] = base64_chars.find(char_array_4[i]);
      }

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++) {
        ret += char_array_3[i];
      }

      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      char_array_4[j] = 0;
    }

    for (j = 0; j < 4; j++) {
      char_array_4[j] = base64_chars.find(char_array_4[j]);
    }

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) {
      ret += char_array_3[j];
    }
  }

  return ret;
}

//#define __WRITE_FILE__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

static void write_log(const char* msg)
{
  time_t tim;
  time(&tim);
  FILE* fp = NULL;
  char message[1024];
  struct tm* t = localtime(&tim);
  snprintf(message, 1024, "[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
#ifdef __WRITE_FILE__

  if (t->tm_mday != last_log_time.tm_mday || t->tm_mon != last_log_time.tm_mon
      || t->tm_year != last_log_time.tm_year) {
    fp = fopen("./log.txt", "a+");
  }

  fwrite(message, strlen(message), 1, fp);
  fflush(fp);

  if (fp) {
    fclose(fp);
  }

#else
  printf("\n%s", message);
  fflush(stdout);
#endif
}

static void DEBUG_LOG(const char* msg, ...)
{
  char message[1024] = {0};
  va_list args;
  va_start(args, msg);
  vsnprintf(message, 1024, msg, args);
  va_end(args);
  write_log(message);
}

#include "sha1.inl"

//#define read(s, data, len) sock_recv_safe(s, data, len, 0)
//#define write(s, data, len) sock_send_safe(s, data, len, 0)
//#define close closesocket


class Websocket_Request
{
public:
  Websocket_Request();
  ~Websocket_Request();
  int fetch_websocket_info(char* msg);
  void print();
  void reset();

private:
  int fetch_fin(char* msg, int& pos);
  int fetch_opcode(char* msg, int& pos);
  int fetch_mask(char* msg, int& pos);
  int fetch_masking_key(char* msg, int& pos);
  int fetch_payload_length(char* msg, int& pos);
  int fetch_payload(char* msg, int& pos);
private:
  uint8_t fin_;
  uint8_t opcode_;
  uint8_t mask_;
  uint8_t masking_key_[4];
  int payload_length_;
  char payload_[2048];
};


Websocket_Request::Websocket_Request():
  fin_(),
  opcode_(),
  mask_(),
  masking_key_(),
  payload_length_(),
  payload_()
{
}

Websocket_Request::~Websocket_Request()
{

}

int Websocket_Request::fetch_websocket_info(char* msg)
{
  int pos = 0;
  fetch_fin(msg, pos);
  fetch_opcode(msg, pos);
  fetch_mask(msg, pos);
  fetch_payload_length(msg, pos);
  fetch_masking_key(msg, pos);
  return fetch_payload(msg, pos);
}

void Websocket_Request::print()
{
  DEBUG_LOG("WEBSOCKET PROTOCOL\n"
      "FIN: %d\n"
      "OPCODE: %d\n"
      "MASK: %d\n"
      "PAYLOADLEN: %d\n"
      "PAYLOAD: %s\n",
      fin_, opcode_, mask_, payload_length_, payload_);
  //DEBUG_LOG("PAYLOAD: %s\n", payload_);

  reset();
}

void Websocket_Request::reset()
{
  fin_ = 0;
  opcode_ = 0;
  mask_ = 0;
  memset(masking_key_, 0, sizeof(masking_key_));
  payload_length_ = 0;
  memset(payload_, 0, sizeof(payload_));
}

int Websocket_Request::fetch_fin(char* msg, int& pos)
{
  fin_ = (unsigned char)msg[pos] >> 7;
  return 0;
}

int Websocket_Request::fetch_opcode(char* msg, int& pos)
{
  opcode_ = msg[pos] & 0x0f;
  pos++;
  return 0;
}

int Websocket_Request::fetch_mask(char* msg, int& pos)
{
  mask_ = (unsigned char)msg[pos] >> 7;
  return 0;
}

int Websocket_Request::fetch_masking_key(char* msg, int& pos)
{
  if (mask_ != 1) {
    return 0;
  }

  for (int i = 0; i < 4; i++) {
    masking_key_[i] = msg[pos + i];
  }

  pos += 4;
  return 0;
}

int Websocket_Request::fetch_payload_length(char* msg, int& pos)
{
  payload_length_ = msg[pos] & 0x7f;
  pos++;

  if (payload_length_ == 126) {
    uint16_t length = 0;
    memcpy(&length, msg + pos, 2);
    pos += 2;
    payload_length_ = ntohs(length);
  }
  else if (payload_length_ == 127) {
    uint32_t length = 0;
    memcpy(&length, msg + pos, 4);
    pos += 4;
    payload_length_ = ntohl(length);
  }

  return 0;
}

int Websocket_Request::fetch_payload(char* msg, int& pos)
{
  memset(payload_, 0, sizeof(payload_));

  if (mask_ != 1) {
    memcpy(payload_, msg + pos, payload_length_);
  }
  else {
    for (uint i = 0; i < payload_length_; i++) {
      int j = i % 4;
      payload_[i] = msg[pos + i] ^ masking_key_[j];
    }
  }

  pos += payload_length_;
  return 0;
}

#include <map>
#include <sstream>

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

enum WEBSOCKET_STATUS {
  WEBSOCKET_UNCONNECT = 0,
  WEBSOCKET_HANDSHARKED = 1,
};

typedef std::map<std::string, std::string> HEADER_MAP;

class Websocket_Handler
{
public:
  Websocket_Handler(int fd);
  ~Websocket_Handler();
  int process();
  inline char* getbuff();
private:
  int handshark();
  void parse_str(char* request);
  int fetch_http_info();
  int send_data(char* buff);
private:
  char buff_[2048];
  WEBSOCKET_STATUS status_;
  HEADER_MAP header_map_;
  int fd_;
  Websocket_Request* request_;
};

inline char* Websocket_Handler::getbuff()
{
  return buff_;
}

Websocket_Handler::Websocket_Handler(int fd):
  buff_(),
  status_(WEBSOCKET_UNCONNECT),
  header_map_(),
  fd_(fd),
  request_(new Websocket_Request)
{
}

Websocket_Handler::~Websocket_Handler()
{
}

int Websocket_Handler::process()
{
  if (status_ == WEBSOCKET_UNCONNECT) {
    return handshark();
  }

  request_->fetch_websocket_info(buff_);
  request_->print();
  memset(buff_, 0, sizeof(buff_));
  return 0;
}

int Websocket_Handler::handshark()
{
  char request[1024] = {0};
  status_ = WEBSOCKET_HANDSHARKED;
  fetch_http_info();
  parse_str(request);
  memset(buff_, 0, sizeof(buff_));
  return send_data(request);
}

void Websocket_Handler::parse_str(char* request)
{
  strcat(request, "HTTP/1.1 101 Switching Protocols\r\n");
  strcat(request, "Connection: upgrade\r\n");
  strcat(request, "Sec-WebSocket-Accept: ");
  std::string server_key = header_map_["Sec-WebSocket-Key"];
  server_key += MAGIC_KEY;

  SHA1 sha;
  unsigned int message_digest[5];
  sha.Reset();
  sha << server_key.c_str();

  sha.Result(message_digest);

  for (int i = 0; i < 5; i++) {
    message_digest[i] = htonl(message_digest[i]);
  }

  server_key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
  server_key += "\r\n";
  strcat(request, server_key.c_str());
  strcat(request, "Upgrade: websocket\r\n\r\n");
}

int Websocket_Handler::fetch_http_info()
{
  std::istringstream s(buff_);
  std::string request;

  std::getline(s, request);

  if (request[request.size() - 1] == '\r') {
    request.erase(request.end() - 1);
  }
  else {
    return -1;
  }

  std::string header;
  std::string::size_type end;

  while (std::getline(s, header) && header != "\r") {
    if (header[header.size() - 1] != '\r') {
      continue; //end
    }
    else {
      header.erase(header.end() - 1); //remove last char
    }

    end = header.find(": ", 0);

    if (end != std::string::npos) {
      std::string key = header.substr(0, end);
      std::string value = header.substr(end + 2);
      header_map_[key] = value;
    }
  }

  return 0;
}

int Websocket_Handler::send_data(char* buff)
{
  return sock_send_safe(fd_, buff, strlen(buff), 0);
}

class Websocket_Respond
{
public:
  Websocket_Respond() { };
  ~Websocket_Respond() { };
};


#include "net/sepoll.inl"

#define PORT 9000
#define TIMEWAIT 100
#define BUFFLEN 2048
#define MAXEVENTSSIZE 20

typedef std::map<int, Websocket_Handler*> WEB_SOCKET_HANDLER_MAP;

class Network_Interface
{
private:
  Network_Interface();
  ~Network_Interface();
  int init();
  int epoll_loop();
  int set_noblock(int fd);
  void ctl_event(int fd, bool flag);
public:
  void run();
  static Network_Interface* get_share_network_interface();
private:
  epoll_handle epollfd_;
  int listenfd_;
  WEB_SOCKET_HANDLER_MAP websocket_handler_map_;
  static Network_Interface* m_network_interface;
};

#define NETWORK_INTERFACE Network_Interface::get_share_network_interface()


Network_Interface* Network_Interface::m_network_interface = NULL;

Network_Interface::Network_Interface():
  epollfd_(0),
  listenfd_(0),
  websocket_handler_map_()
{
  if (0 != init()) {
    exit(1);
  }
}

Network_Interface::~Network_Interface()
{

}

int Network_Interface::init()
{
  sock_init();
  listenfd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (listenfd_ == -1) {
    DEBUG_LOG("创建套接字失败!");
    return -1;
  }

  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(sockaddr_in));

  server_addr.sin_family = AF_INET;

  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  server_addr.sin_port = htons(PORT);

  if (-1 == bind(listenfd_, (struct sockaddr*)(&server_addr), sizeof(server_addr))) {
    DEBUG_LOG("绑定套接字失败!");
    return -1;
  }

  if (-1 == listen(listenfd_, 5)) {
    DEBUG_LOG("监听失败!");
    return -1;
  }

  epollfd_ = epoll_create(MAXEVENTSSIZE);

  ctl_event(listenfd_, true);
  DEBUG_LOG("服务器启动成功!");
  return 0;
}

int Network_Interface::epoll_loop()
{
  struct sockaddr_in client_addr;
  int nfds = 0;
  int fd = 0;
  int bufflen = 0;
  struct epoll_event events[MAXEVENTSSIZE];

  while (true) {
    nfds = epoll_wait(epollfd_, events, MAXEVENTSSIZE, TIMEWAIT);

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == listenfd_) {
        socklen_t clilen = sizeof(struct sockaddr);
        fd = accept(listenfd_, (struct sockaddr*)&client_addr, &clilen);
        ctl_event(fd, true);
      }
      else if (events[i].events & EPOLLIN) {
        if ((fd = events[i].data.fd) < 0) {
          continue;
        }

        Websocket_Handler* handler = websocket_handler_map_[fd];

        if (handler == NULL) {
          continue;
        }

        if ((bufflen = sock_recv(fd, handler->getbuff(), BUFFLEN, 0)) <= 0) {
          ctl_event(fd, false);
        }
        else {
          handler->process();
        }
      }
    }
  }

  return 0;
}

int Network_Interface::set_noblock(int fd)
{
#if 0
  int flags;

  if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
    flags = 0;
  }

  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
  u_long ul = 1;
  return ioctlsocket(fd, FIONBIO, &ul); // no block
#endif
}

Network_Interface* Network_Interface::get_share_network_interface()
{
  if (m_network_interface == NULL) {
    m_network_interface = new Network_Interface();
  }

  return m_network_interface;
}

void Network_Interface::ctl_event(int fd, bool flag)
{
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = flag ? EPOLLIN : 0;
  epoll_ctl(epollfd_, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);

  if (flag) {
    set_noblock(fd);
    websocket_handler_map_[fd] = new Websocket_Handler(fd);

    if (fd != listenfd_) {
      DEBUG_LOG("fd: %d 加入epoll循环", fd);
    }
  }
  else {
    sock_close(fd);
    delete websocket_handler_map_[fd];
    websocket_handler_map_.erase(fd);
    DEBUG_LOG("fd: %d 退出epoll循环", fd);
  }
}

void Network_Interface::run()
{
  epoll_loop();
}

#endif // _WEBSOCKET_INL_
