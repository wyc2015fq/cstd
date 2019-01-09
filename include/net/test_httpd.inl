
//#include "cfile.h"
//#include "getopt.inl"
#include "httpd.inl"

#define test1_html "<HTML><BODY>This is just a test</BODY>"
#define ember_code1 "printf(\"This is from ember in process %d\n\",getpid());"
#define ember_code "load \"/usr/local/nk/www/content.lib\"; dispatchWebPage(\"/index.html\");"

static void index_html(httpd* s)
{
#if 1
  httpdPrintf(s, "Welcome to the httpd server running in process number %d<P>\n", sys_getpid());
  httpdPrintf(s, "Click <A HREF=/test1.html>here</A> to view a test page<P>\n");
  httpdPrintf(s, "Click <A HREF=/login.html>here</A> to authenticate<P>\n");
  httpdPrintf(s, "Or <A HREF=/wildcard/foo>here</A> for a test wildcard page<P>\n");
  httpdPrintf(s, "<P><FORM ACTION=test2.html METHOD=POST>\n");
  httpdPrintf(s, "Enter your name <INPUT NAME=name SIZE=10>\n");
  httpdPrintf(s, "<INPUT TYPE=SUBMIT VALUE=Click!><P></FORM>\n");
#endif
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
  //httpdOutput(s, "Hello $name 大傻逼");
  httpdPrintf(s, "<P>Hello %s 大傻逼!</P>", variable->value);
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

int test_httpd(int argc, char* argv[])
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
  httpdAddCWildcardContent(svr, "/wildcard", NULL, test3_html);
  httpdAddStaticContent(svr, "/", "test1.html", HTTP_FALSE, NULL, test1_html);
  httpdAddEmberContect(svr, "/", "ember.html", HTTP_FALSE, NULL, ember_code);
  // Go into our service loop
  printf("server start..... \n");
  int count = 0;
  while (1 == 1) {
    int result;
    // Linux modifies the timouet value during the
    // select call so we must set it everyt ime. Most
    // other UNIX implementations do not modify timeout
    // but it doesn't hurt to set it each time anyway
    httpd* s=0;
    result = httpdGetConnection(svr, 5000, &s);
    if (result == 0) {
      //printf("Timeout ... \n");
      continue;
    }
    if (result < 0) {
      printf("Error ... \n");
      continue;
    }
    printf("---------------%d\n", ++count);
    printf("%s connect\n", s->clientAddr);
    if (1) {
      httpdRequestProc(s);
    }
    else {
      thread_t th[1] = { 0 };
      sys_thread_create(th, NULL, (thread_callback)httpdRequestProc, s);
    }
  }
  return 0;
}
