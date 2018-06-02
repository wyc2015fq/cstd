
typedef struct ttt_t {
  int id;
  int ssock, csock;
  int netinited;
  uchar* pGray;
} ttt_t;

static int port[2] = {5000, 5001};
