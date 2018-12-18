
//#include "cstd.h"
//#define bzero(arr, n) memset(arr, 0, n)

// Standard header preamble.  Ensure singular inclusion, setup for
// function prototypes and c++ inclusion

// ** Macro Definitions

#define HTTP_PORT             80
#define HTTP_MAX_LEN          10240
#define HTTP_MAX_URL          1024
#define HTTP_MAX_HEADERS      1024
#define HTTP_MAX_AUTH         128
#define HTTP_IP_ADDR_LEN      17
#define HTTP_TIME_STRING_LEN  40
#define HTTP_ANY_ADDR         NULL
#define HTTP_MAX_VAR_NAME_LEN 48

#define HTTP_GET    1
#define HTTP_POST   2

#define HTTP_TRUE     1
#define HTTP_FALSE    0

#define HTTP_FILE           1
#define HTTP_C_FUNCT        2
#define HTTP_EMBER_FUNCT    3
#define HTTP_STATIC         4
#define HTTP_WILDCARD       5
#define HTTP_C_WILDCARD     6
#define HTTP_EMBER_WILDCARD 7

#define HTTP_METHOD_ERROR "\n<B>ERROR : Method Not Implemented</B>\n\n"

#define httpdRequestMethod(s)         s->request.method
#define httpdRequestPath(s)           s->request.path
#define httpdRequestContentType(s)    s->request.contentType
#define httpdRequestContentLength(s)  s->request.contentLength

#define HTTP_ACL_PERMIT   1
#define HTTP_ACL_DENY     2

// Type Definitions

struct httpd;

typedef struct _httpd_var {
  char* name, *value;
  int valuelen;
  struct  _httpd_var*  nextValue, *nextVariable;
} httpVar;

typedef struct _httpd_content {
  char*  name;
  int type, indexFlag;
  void (*function)(httpd* server);
  char* data, *path;
  int (*preload)(httpd* server);
  struct  _httpd_content*  next;
} httpContent;

typedef struct {
  int method,
    contentLength,
    authLength;
  char* content;
  char  path[HTTP_MAX_URL],
    host[HTTP_MAX_URL],
    userAgent[HTTP_MAX_URL],
    referer[HTTP_MAX_URL],
    ifModified[HTTP_MAX_URL],
    contentType[HTTP_MAX_URL],
    authUser[HTTP_MAX_AUTH],
    authPassword[HTTP_MAX_AUTH];
} httpReq;

typedef struct {
  int   responseLength;
  httpContent* content;
  char headersSent,
    headers[HTTP_MAX_HEADERS],
    response[HTTP_MAX_URL],
    contentType[HTTP_MAX_URL];
} httpRes;

typedef struct _httpd_dir {
  char*  name;
  struct  _httpd_dir* children, *next;
  struct  _httpd_content* entries;
} httpDir;


typedef struct ip_acl_s {
  int     addr;
  char    len, action;
  struct  ip_acl_s* next;
} httpAcl;
