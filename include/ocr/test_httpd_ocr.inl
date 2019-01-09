
//#include "cfile.h"
//#include "getopt.inl"

#ifdef _DEBUG
#define BIN_PIC "E:/OCR_Line/bin/pic/"
#else
#define BIN_PIC "./pic/"
#endif

#include "std/time_c.h"
#include "net/httpd.inl"
#include "parser/cjson.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "codec/base64.inl"

#define test1_html "<HTML><BODY>This is just a test</BODY>"
#define ember_code1 "printf(\"This is from ember in process %d\n\",getpid());"
#define ember_code "load \"/usr/local/nk/www/content.lib\"; dispatchWebPage(\"/index.html\");"

static void index_html(httpd* s)
{
#if 0
  httpdPrintf(s, "Welcome to the httpd server running in process number %d<P>\n", sys_getpid());
  httpdPrintf(s, "Click <A HREF=/test1.html>here</A> to view a test page<P>\n");
  httpdPrintf(s, "Click <A HREF=/login.html>here</A> to authenticate<P>\n");
  httpdPrintf(s, "Or <A HREF=/wildcard/foo>here</A> for a test wildcard page<P>\n");
  httpdPrintf(s, "<P><FORM ACTION=test2.html METHOD=POST>\n");
  httpdPrintf(s, "Enter your name <INPUT NAME=name SIZE=10>\n");
  httpdPrintf(s, "<INPUT TYPE=SUBMIT VALUE=Click!><P></FORM>\n");
#endif
  httpdPrintf(s, "hello");
  return;
}
static void test2_html(httpd* s)
{
  httpVar* variable;
  // Grab the symbol table entry to see if the variable exists
  variable = httpdGetVariableByName(s, "name");
  if (variable == NULL) {
    httpdPrintf(s, "Missing form data!");
    return;
  }
  // Use httpdOutput() rather than httpdPrintf() so that the variable embedded in the text is expanded automatically
  //httpdOutput(s, "Hello $name ¥Û…µ±∆");
  httpdPrintf(s, "<P>Hello %s !</P>", variable->value);
}
std::string gb2utf8(std::string gb) {
  std::string utf;
  int sz = gb.size() * 2;
  char* buf = (char*)malloc(sz);
  sz = iconv_c(ICONV_GB2312, ICONV_UTF8, gb.c_str(), gb.size(), buf, sz);
  buf[sz] = 0;
  utf = buf;
  free(buf);
  return utf;
}
static void ocridcard(httpd* s) {
  char* path;
  path = httpdRequestPath(s);
  httpVar* api_key;
  api_key = httpdGetVariableByName(s, "api_key");
  if (api_key) {
    printf("api_key = %s\n", api_key->value);
  }
  httpVar* api_secret;
  api_secret = httpdGetVariableByName(s, "api_secret");
  if (api_secret) {
    printf("api_secret = %s\n", api_secret->value);
  }
  cv::Mat cv_img;
  httpVar* image_file;
  char* imgbuf = NULL;
  char* tmpbuf = NULL;
  int imgbuflen = 0;
  image_file = httpdGetVariableByName(s, "image_file");
  if (image_file) {
    //printf("image_file");
    imgbuf = image_file->value;
    imgbuflen = image_file->valuelen;
  }
  httpVar* image_base64;
  image_base64 = httpdGetVariableByName(s, "image_base64");
  if (image_base64) {
    imgbuf = tmpbuf = (char*)malloc(image_file->valuelen);
    imgbuflen = base64_decode(image_file->value, imgbuflen, imgbuf);
  }
  if (imgbuf && imgbuflen>10) {
    std::vector<char> vec_data(imgbuf, imgbuf + imgbuflen);
    cv_img = cv::imdecode(vec_data, cv::IMREAD_COLOR);
  }
  if (cv_img.rows>10 && cv_img.cols>10) {
    char buf[256];
    if (0) {
      cv::imshow("image_file", cv_img);
      cv::waitKey(-1);
    }
    char* nowstr = timenow();
    _snprintf(buf, 256, BIN_PIC"%s.jpg", nowstr);
    cv::imwrite(buf, cv_img);
#ifdef __DETECT_IDCARD_HPP__
    if (1) {
      static ocr_detect od;
      string str = od.getjson(cv_img);
      string str_utf8 = gb2utf8(str);
      httpdPrintf(s, str_utf8.c_str());
    }
#endif
  }
  if (tmpbuf) {
    free(tmpbuf);
  }
  return;
}
static void test3_html(httpd* s)
{
  char* path;
  path = httpdRequestPath(s);
  httpdOutput(s, "Wilcard dynamic request received<P>");
  httpdPrintf(s, "The requested path was %s<P>", path);
}
static void test4_html(httpd* s)
{
  char* path;
  path = httpdRequestPath(s);
  if (STRNCASECMP2(httpdRequestContentType(s), "text/json")==0) {
    int asdf=0;
  }

  httpdOutput(s, "Wilcard dynamic request received<P>");
  httpdPrintf(s, "The requested path was %s<P>", path);
}
static void login_html(httpd* s)
{
  if (httpdAuthenticate(s, "LibHTTPD Test") == 0) {
    return;
  }
  httpdPrintf(s, "Your username is '%s'<P>\n", s->request.authUser);
  httpdPrintf(s, "Your password is '%s'<P>\n", s->request.authPassword);
  httpdOutput(s, "Click <A HREF=login2.html>here</A> to force reauthentication");
  httpdOutput(s, ". Use a username = test password = 123");
}
static void login2_html(httpd* s)
{
  if (httpdAuthenticate(s, "LibHTTPD Test") == 0) {
    httpdOutput(s, "Authentication failure(1).");
    return;
  }
  if (strcmp(s->request.authUser, "test") != 0 ||
      strcmp(s->request.authPassword, "123") != 0) {
    httpdForceAuthenticate(s, "LibHTTPD Test");
    httpdOutput(s, "Authentication failure (2).");
    return;
  }
  httpdOutput(s, "Your login was accepted.");
}
static void myError404Handler(httpd* s, int error)
{
  httpdOutput(s, "Whoa there. You hit a page that doesn't exist! <P><BR><BR>\n");
  httpdOutput(s, "Or in other words : <B>Error 404</B>\n\n");
}

int httpdRequestProc(httpd* s) {
  if (httpdReadRequest(s) < 0) {
    httpdEndRequest(s);
    return 0;
  }
  httpdProcessRequest(s);
  httpdEndRequest(s);
  return 0;
}

int test_httpd_ocr(int argc, char* argv[])
{
  //CPCODE; ChangeDisplaySettings DEVMODE
  //sys_mkdir("C:/code/testc/root");
  //sys_chdir("C:/code/testc/root");
  httpSvr* svr = NULL;
  char* host = NULL;
  int port;
#if 0
  int errFlag = 0;
  extern char* optarg;
  extern int optind, opterr, optopt;
  int c;
  while ((c = getopt(argc, argv, "h:p:")) != -1) {
    switch (c) {
    case 'h':
      host = optarg;
      break;
    case 'p':
      port = atoi(optarg);
      break;
    default:
      errFlag++;
    }
  }
  if (errFlag) {
    fprintf(stderr, "usage: [-h <host IP>] [ -p <port >]\n");
    fprintf(stderr, "\nLibHTTPD version %s\n\n", LIBHTTPD_VERSION);
    exit(1);
  }
#else
  host = NULL;
  //host = 10.1.60.222
  port = 80;
  //host = "192.168.1.3";
  if (argc>1) {
    //port = atoi(argv[1]);
  }
#endif
  svr = httpdCreate(host, port);
  if (svr == NULL) {
    perror("Can't create server");
    exit(1);
  }
  httpdSetAccessLog(svr, stdout);
  httpdSetErrorLog(svr, stdout);
  // We are fussy and don't want the default Error 404 page
  httpdSetErrorFunction(svr, 404, myError404Handler);
  // Setup some content for the server
  httpdAddCContent(svr, "/", "index.html", HTTP_TRUE, NULL, index_html);
  httpdAddCContent(svr, "/", "test2.html", HTTP_FALSE, NULL, test2_html);
  httpdAddCContent(svr, "/", "test", HTTP_FALSE, NULL, test4_html);
  httpdAddCContent(svr, "/", "login.html", HTTP_FALSE, NULL, login_html);
  httpdAddCContent(svr, "/", "login2.html", HTTP_FALSE, NULL, login2_html);
  httpdAddCContent(svr, "/cardpp/v1/", "ocridcard", HTTP_FALSE, NULL, ocridcard);
  httpdAddCWildcardContent(svr, "/wildcard", NULL, test3_html);
  httpdAddStaticContent(svr, "/", "test1.html", HTTP_FALSE, NULL, test1_html);
  httpdAddEmberContect(svr, "/", "ember.html", HTTP_FALSE, NULL, ember_code);
  // Go into our service loop
  printf("server start..... \n");
  while (1 == 1) {
    int result;
    // Linux modifies the timouet value during the
    // select call so we must set it everyt ime. Most
    // other UNIX implementations do not modify timeout
    // but it doesn't hurt to set it each time anyway
    httpd* s = 0;
    result = httpdGetConnection(svr, 5000, &s);
    if (result == 0) {
      //printf("Timeout ... \n");
      continue;
    }
    if (result < 0) {
      printf("Error ... \n");
      continue;
    }
    printf("%s connect\n", s->clientAddr);
    httpdRequestProc(s);
  }
  return 0;
}
