
//#include "std/fileio_c.h"
#include "utime.h"
#include "std/thread_c.h"
#include "std/dir_c.h"
#include "std/net_c.h"
#include "cfgparse.inl"

#define __USE_GNU
#define _GNU_SOURCE

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>

#ifdef _WIN32
//#include <windows.h>
typedef u_long in_port_t;
typedef u_long in_addr_t;
#define PATH_MAX 512
//struct gnutls_session_t{};
typedef void* gnutls_session_t;
struct gnutls_certificate_credentials_t {};
struct gnutls_priority_t {};
#define MSG_NOSIGNAL 0
#define gnutls_record_send(a, b, c)  (0)
#define gnutls_record_recv(a, b, c)  (0)
#define GNUTLS_SHUT_RDWR 0
#define gnutls_bye(a, b)
#define gnutls_deinit(a)
#define gnutls_record_get_max_size(a)  (0)
#define sys_cleanup_pop(a)
#define sleep(x)  Sleep(x)
typedef u_long mode_t;
typedef int ssize_t;
#else
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <gnutls/gnutls.h>
#define closesocket close
#endif


typedef struct _FTP_CONFIG {
  char			*ConfigFile;
  unsigned int	MaxUsers;
  in_port_t		Port;
  in_port_t		PasvPortBase;
  in_port_t		PasvPortMax;
  in_addr_t		BindToInterface;
  in_addr_t		ExternalInterface;
  in_addr_t		LocalIPMask;
} FTP_CONFIG, *PFTP_CONFIG;

#define	CONFIG_FILE_NAME		"fftp.conf"
#define	CONFIG_SECTION_NAME		"ftpconfig"
#define	DEFAULT_FTP_PORT		21

//#define INVALID_SOCKET -1
//#define SOCKET	int

#define MODE_NORMAL			0
#define MODE_PASSIVE		1

/*
* NOT_LOGGED_IN = "banned" in config. Not logged in or banned.
* READONLY = "readonly" in config. Browse and download.
* CREATENEW = "upload" in config. Creating new directories, store new files. Append, rename and delete disabled.
* FULL = "admin" in config. All access features enabled.
*/

#define FTP_ACCESS_NOT_LOGGED_IN	0
#define FTP_ACCESS_READONLY			1
#define FTP_ACCESS_CREATENEW		2
#define FTP_ACCESS_FULL				3

#define TRANSMIT_BUFFER_SIZE	65536

static const unsigned long int	FTP_PATH_MAX = PATH_MAX;

#define	SIZE_OF_GPBUFFER		4*FTP_PATH_MAX

typedef struct	_FTPCONTEXT {
  mutex_t		MTLock;
  SOCKET				ControlSocket;
  SOCKET				DataSocket;
  thread_t			WorkerThreadId[1];
  /*
  * WorkerThreadValid is output of pthread_create
  * therefore zero is VALID indicator and -1 is invalid.
  */
  int					WorkerThreadValid;
  int					WorkerThreadAbort;
  in_addr_t			ServerIPv4;
  in_addr_t			ClientIPv4;
  in_addr_t			DataIPv4;
  in_port_t			DataPort;
  int					File;
  int					Mode;
  int					Access;
  int					CreateMode;
  int					SessionID;
  int					DataProtectionLevel;
  off_t				RestPoint;
  unsigned long int	BlockSize;
  char				CurrentDir[PATH_MAX];
  char				RootDir[PATH_MAX];
  char				*GPBuffer;
  gnutls_session_t	TLS_session;
  int utf8;
} FTPCONTEXT, *PFTPCONTEXT;


typedef int(*FTPROUTINE) (PFTPCONTEXT context, const char *params);
typedef void *(__thread_start_routine)(void *), *__ptr_thread_start_routine;

FTP_CONFIG	g_cfg;
FILE*			g_log = NULL;
void *ftpmain(void *p);

gnutls_certificate_credentials_t		x509_cred;
gnutls_priority_t					priority_cache;

#define	MAX_CMDS 32

static const char success200[] = "200 Command okay.\r\n";
static const char success200_1[] = "200 Type set to A.\r\n";
static const char success200_2[] = "200 Type set to I.\r\n";
static const char success211[] =
"211-Extensions supported:\r\n PASV\r\n UTF8\r\n TVFS\r\n REST STREAM\r\n "
"SIZE\r\n MLSD\r\n AUTH TLS\r\n PBSZ\r\n PROT\r\n EPSV\r\n"
"211 End.\r\n";

static const char success214[] =
"214-The following commands are recognized.\r\n"
" ABOR APPE AUTH CDUP CWD  DELE EPSV FEAT HELP LIST MKD MLSD NOOP OPTS\r\n"
" PASS PASV PBSZ PORT PROT PWD  QUIT REST RETR RMD RNFR RNTO SITE SIZE\r\n"
" STOR SYST TYPE USER\r\n"
"214 Help OK.\r\n";

static const char success215[] = "215 UNIX Type: L8\r\n";
static const char success220[] = "220 FTP server v1.0a ready\r\n";
static const char success221[] = "221 Goodbye!\r\n";
static const char success226[] = "226 Transfer complete. Closing data connection.\r\n";
static const char success227[] = "227 Entering Passive Mode (%u,%u,%u,%u,%u,%u).\r\n";
static const char success229[] = "229 Entering Extended Passive Mode (|||%u|)\r\n";
static const char success230[] = "230 User logged in, proceed.\r\n";
static const char success234[] = "234 AUTH command OK. Initializing TLS connection.\r\n";
static const char success250[] = "250 Requested file action okay, completed.\r\n";
static const char success257[] = "257 Directory created.\r\n";
static const char error425[] = "425 Can not open data connection.\r\n";
static const char error426[] = "426 Connection closed; transfer aborted.\r\n";
static const char error451[] = "451 Requested action aborted. Local error in processing.\r\n";
static const char error500[] = "500 Syntax error, command unrecognized.\r\n";
static const char error500_auth[] = "500 AUTH unsuccessful.\r\n";
static const char error501[] = "501 Syntax error in parameters or arguments.\r\n";
static const char error503[] = "503 Invalid sequence of commands (AUTH TLS required prior to authentication).\r\n";
static const char error504[] = "504 Command not implemented for that parameter.\r\n";
static const char error530[] = "530 Please login with USER and PASS.\r\n";
static const char error530_b[] = "530 This account is disabled.\r\n";
static const char error530_r[] = "530 Invalid user name or password.\r\n";
static const char error550[] = "550 File or directory unavailable.\r\n";
static const char error550_r[] = "550 Permission denied.\r\n";
static const char error550_a[] = "550 Data channel was closed by ABOR command from client.\r\n";
static const char error550_t[] = "550 Another action is in progress, use ABOR command first.\r\n";
static const char error550_m[] = "550 Insufficient resources.\r\n";
static const char interm125[] = "125 Data connection already open; Transfer starting.\r\n";
static const char interm150[] = "150 File status okay; about to open data connection.\r\n";
static const char interm350[] = "350 REST supported. Ready to resume at byte offset ";
static const char interm350_ren[] = "350 File exists. Ready to rename.\r\n";
static const char interm331[] = "331 User ";
static const char interm331_tail[] = " OK. Password required\r\n";
static const char NOSLOTS[] = "MAXIMUM ALLOWED USERS CONNECTED\r\n";

static const char* shortmonths[12] = {
  "Jan\0", "Feb\0", "Mar\0", "Apr\0", "May\0", "Jun\0",
  "Jul\0", "Aug\0", "Sep\0", "Oct\0", "Nov\0", "Dec\0" };

// FTP_PASSCMD_INDEX
// must be in sync with ftpprocs & ftpcmds "PASS" index
#define FTP_PASSCMD_INDEX 13

unsigned int g_newid = 0;

void delete_last_slash(char *s)
{
  if (*s != 0) {
    // don't remove root directory sign as special case
    if ((s[0] == '/') && (s[1] == 0))
      return;

    while (s[1] != 0)
      ++s;

    if (*s == '/')
      *s = 0;
  }
}

void add_last_slash(char *s)
{
  if (*s != 0) {
    while (s[1] != 0)
      ++s;

    if (*s != '/') {
      s[1] = '/';
      s[2] = 0;
    }
  }
}

// Cuts off filename from string leaving only path.
// Return value: pointer to a terminating null character at the end of path
char *filepath(char *s)
{
  char *p = s;

  if (*s == 0)
    return s;
  /*
   * leave root directory sign untouched
   */
  if (*s == '/') {
    ++s;
    ++p;
  }

  while (*s != 0) {
    if (*s == '/')
      p = s;
    ++s;
  }

  *p = 0;

  return p;
}

 // This function filters the path out of ".." members
 // not allowing user to escape the home directory
void format_path(char *input_path, char *filtered_path)
{
  char *p0, *pnext, *fp0;
  size_t sl;

  if (*input_path == '/')
  {
    ++input_path;
    *filtered_path = '/';
    ++filtered_path;
  }

  p0 = input_path;
  pnext = input_path;
  fp0 = filtered_path;
  *fp0 = 0;

  while (1)
  {
    while ((*pnext != '/') && (*pnext != 0))
      ++pnext;

    sl = pnext - p0;

    while (sl > 0)
    {
      if (sl == 1)
        if (*p0 == '.')
          break;

      if (sl == 2)
        if ((p0[0] == '.') && (p0[1] == '.'))
        {
          delete_last_slash(filtered_path);
          fp0 = filepath(filtered_path);
          if (fp0 != filtered_path)
          {
            *fp0 = '/';
            ++fp0;
            *fp0 = 0;
          }
          break;
        }

      strncpy(fp0, p0, sl);
      fp0 += sl;
      if (*pnext != 0)
      {
        *fp0 = '/';
        ++fp0;
      }
      *fp0 = 0;

      break;
    }

    if (*pnext == 0)
      break;

    ++pnext;
    p0 = pnext;
  }
}

char *finalpath(char *root_dir, char *current_dir, char *params, char *result_path)
{
  char *tmp, *user_root;
  size_t total_len;

  total_len = strlen(root_dir) + strlen(current_dir);
  if (params != NULL)
    total_len += strlen(params);

  if (total_len >= SIZE_OF_GPBUFFER)
    return NULL;

  tmp = (char*)malloc(SIZE_OF_GPBUFFER);
  *tmp = 0;

  strcpy(result_path, root_dir);
  add_last_slash(result_path);
  user_root = result_path + strlen(result_path);

  do {
    if (params == NULL)
    {
      strcpy(tmp, current_dir);
      add_last_slash(tmp);
      break;
    }

    if (params[0] != '/')
    {
      strcpy(tmp, current_dir);
      add_last_slash(tmp);
    }

    strcat(tmp, params);
  } while (0);

  format_path(tmp, user_root);
  free(tmp);
  return result_path;
}

static void cleanup_handler(void *arg)
{
  PFTPCONTEXT context = (PFTPCONTEXT)arg;

  sys_mutex_unlock(&context->MTLock);
}

int sendstring_plaintext(SOCKET s, const char *Buffer)
{
  return (send(s, Buffer, strlen(Buffer), MSG_NOSIGNAL) >= 0);
}

int InitTLSSession(gnutls_session_t *session, SOCKET s, int send_success_string)
{
  int ret;

#ifdef _WIN32
#else
  while (session != NULL)
  {
    if (gnutls_init(session, GNUTLS_SERVER | GNUTLS_NO_SIGNAL) < 0)
      break;

    if (gnutls_priority_set(*session, priority_cache) < 0)
      break;

    if (gnutls_credentials_set(*session, GNUTLS_CRD_CERTIFICATE, x509_cred) < 0)
      break;

    gnutls_certificate_server_set_request(*session, GNUTLS_CERT_IGNORE);
    gnutls_handshake_set_timeout(*session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);
    gnutls_transport_set_int2(*session, s, s);

    if (send_success_string != 0)
      sendstring_plaintext(s, success234);

    do {
      ret = gnutls_handshake(*session);
    } while ((ret < 0) && (gnutls_error_is_fatal(ret) == 0));

    if (ret < 0)
    {
      gnutls_deinit(*session);
      *session = NULL;
    }

    return 1;
  }
#endif

  return sendstring_plaintext(s, error500_auth);
}

SOCKET create_datasocket(PFTPCONTEXT context)
{
  SOCKET  clientsocket = INVALID_SOCKET;
  struct sockaddr_in laddr;
  socklen_t  asz;

  memset(&laddr, 0, sizeof(laddr));

  switch (context->Mode) {
  case MODE_NORMAL:
    clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    context->DataSocket = clientsocket;
    if (clientsocket == INVALID_SOCKET)
      return INVALID_SOCKET;

    laddr.sin_family = AF_INET;
    laddr.sin_port = context->DataPort;
    laddr.sin_addr.s_addr = context->DataIPv4;
    if (connect(clientsocket, (const struct sockaddr *)&laddr, sizeof(laddr)) == -1) {
      closesocket(clientsocket);
      return INVALID_SOCKET;
    }
    break;

  case MODE_PASSIVE:
    asz = sizeof(laddr);
    clientsocket = accept(context->DataSocket, (struct sockaddr *)&laddr, &asz);
    closesocket(context->DataSocket);
    context->DataSocket = clientsocket;

    if (clientsocket == INVALID_SOCKET)
      return INVALID_SOCKET;

    context->DataIPv4 = 0;
    context->DataPort = 0;
    context->Mode = MODE_NORMAL;
    break;

  default:
    return INVALID_SOCKET;
  }
  return clientsocket;
}

int sendstring(PFTPCONTEXT context, const char *Buffer)
{
  size_t l = strlen(Buffer);

  if (context->TLS_session == NULL)
    return (send(context->ControlSocket, Buffer, l, MSG_NOSIGNAL) >= 0);
  else
    return (gnutls_record_send(context->TLS_session, Buffer, l) >= 0);
}

int sendstring_auto(SOCKET s, gnutls_session_t session, const char *Buffer)
{
  size_t l = strlen(Buffer);

  if (session == NULL)
    return (send(s, Buffer, l, MSG_NOSIGNAL) >= 0);
  else
    return (gnutls_record_send(session, Buffer, l) >= 0);
}

ssize_t send_auto(int __fd, gnutls_session_t session, const void *__buf, size_t __n)
{
  if (session == NULL)
    return (send(__fd, (const char*)__buf, __n, MSG_NOSIGNAL));
  else
    return (gnutls_record_send(session, __buf, __n));
}

ssize_t recv_auto(int __fd, gnutls_session_t session, void *__buf, size_t __n)
{
  if (session == NULL)
    return (recv(__fd, (char*)__buf, __n, 0));
  else
    return (gnutls_record_recv(session, __buf, __n));
}

ssize_t writeconsolestr(const char *Buffer)
{
  size_t l = strlen(Buffer);

  if (g_log != NULL) {
    fwrite(Buffer, l, 1, g_log);
  }

  return fwrite(Buffer, l, 1, stdout);
}
#ifdef _WIN32
void localtime_r(const time_t* timep, struct tm* t1) {
  //struct tm* localtime(const time_t* timep);
  struct tm* t = localtime(timep);
  *t1 = *t;
  return;
}
#endif
int writelogentry(PFTPCONTEXT context, const char *logtext1, const char *logtext2)
{
  char text[SIZE_OF_GPBUFFER];
  time_t itm = time(NULL);
  struct tm ltm;

  localtime_r(&itm, &ltm);

  if (context == NULL)
  {
    snprintf(text, sizeof(text), "%02u-%02u-%u %02u:%02u:%02u : %s%s\r\n",
      ltm.tm_mday, ltm.tm_mon + 1, ltm.tm_year + 1900,
      ltm.tm_hour, ltm.tm_min, ltm.tm_sec, logtext1, logtext2);
  }
  else
  {
    snprintf(text, sizeof(text), "%02u-%02u-%u %02u:%02u:%02u S-id=%u : %s%s\r\n",
      ltm.tm_mday, ltm.tm_mon + 1, ltm.tm_year + 1900,
      ltm.tm_hour, ltm.tm_min, ltm.tm_sec,
      context->SessionID, logtext1, logtext2);
  }

  return writeconsolestr(text);
}

void WorkerThreadCleanup(PFTPCONTEXT context)
{
  int   err;

  if (context->WorkerThreadValid == 0) {

    /*
     * trying to stop gracefully
     */
    context->WorkerThreadAbort = 1;
    sleep(2);

    err = sys_thread_join(context->WorkerThreadId);
    if (err != 0)
    {
      writelogentry(context, "Enter cancel", "");
      sys_thread_cancel(context->WorkerThreadId);
    }

    context->WorkerThreadValid = -1;
  }

  if (context->DataSocket != INVALID_SOCKET) {
    closesocket(context->DataSocket);
    context->DataSocket = INVALID_SOCKET;
  }

  if (context->File != -1) {
    closesocket(context->File);
    context->File = -1;
  }

  context->DataIPv4 = 0;
  context->DataPort = 0;
  writelogentry(context, "WorkerThreadCleanup complete", "");
}

int ftpUSER(PFTPCONTEXT context, const char *params)
{
  if (params == NULL)
    return sendstring(context, error501);

  context->Access = FTP_ACCESS_NOT_LOGGED_IN;

  /*
   * Save username in GPBuffer for next PASS command
   */
  strcpy(context->GPBuffer, params);

  writelogentry(context, " USER: ", (char *)params);
  sendstring(context, interm331);
  sendstring(context, params);
  return sendstring(context, interm331_tail);
}

int ftpQUIT(PFTPCONTEXT context, const char *params)
{
  writelogentry(context, " QUIT", "");
  sendstring(context, success221);
  /*
   * retrun 0 to break command processing loop
   */
  return 0;
}

int ftpNOOP(PFTPCONTEXT context, const char *params)
{
  return sendstring(context, success200);
}

int ftpPWD(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  strcpy(context->GPBuffer, "257 \"");
  strcat(context->GPBuffer, context->CurrentDir);
  strcat(context->GPBuffer, "\" is a current directory.\r\n");
  return sendstring(context, context->GPBuffer);
}

int ftpTYPE(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  if (params == NULL)
    return sendstring(context, error501);

  switch (*params)
  {
  case 'A':
  case 'a':
    return sendstring(context, success200_1);
  case 'I':
  case 'i':
    return sendstring(context, success200_2);
  }

  return sendstring(context, error501);
}

int ftpPORT(PFTPCONTEXT context, const char *params)
{
  int  c;
  in_addr_t DataIP = 0, DataPort = 0;
  char *p = (char *)params;

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  if (params == NULL)
    return sendstring(context, error501);

  for (c = 0; c < 4; c++) {
    DataIP += ((unsigned char)strtoul(p, NULL, 10)) << c * 8;
    while ((*p >= '0') && (*p <= '9'))
      p++;
    if (*p == 0)
      break;
    p++;
  }

  for (c = 0; c < 2; c++) {
    DataPort += ((unsigned char)strtoul(p, NULL, 10)) << c * 8;
    while ((*p >= '0') && (*p <= '9'))
      p++;
    if (*p == 0)
      break;
    p++;
  }

  if (DataIP != context->ClientIPv4)
    return sendstring(context, error501);

  context->DataIPv4 = DataIP;
  context->DataPort = DataPort;
  context->Mode = MODE_NORMAL;

  return sendstring(context, success200);
}

/*
filemode.c -- make a string describing file modes

 Copyright (C) 1985, 1990, 1993, 1998-2000, 2004, 2006, 2009-2018 Free
 Software Foundation, Inc.
*/

/* Return a character indicating the type of file described by
  file mode BITS:
  '-' regular file
  'b' block special file
  'c' character special file
  'C' high performance ("contiguous data") file
  'd' directory
  'D' door
  'l' symbolic link
  'm' multiplexed file (7th edition Unix; obsolete)
  'n' network special file (HP-UX)
  'p' fifo (named pipe)
  'P' port
  's' socket
  'w' whiteout (4.4BSD)
  '?' some other file type */

static char ftypelet(mode_t bits)
{
  /* These are the most common, so test for them first. */
  if (S_ISREG(bits))
    return '-';
  if (S_ISDIR(bits))
    return 'd';

  /* Other letters standardized by POSIX 1003.1-2004. */
  if (S_ISBLK(bits))
    return 'b';
  if (S_ISCHR(bits))
    return 'c';
  if (S_ISLNK(bits))
    return 'l';
  if (S_ISFIFO(bits))
    return 'p';

  /* Other file types (though not letters) standardized by POSIX. */
  if (S_ISSOCK(bits))
    return 's';

  return '?';
}

/* Like filemodestring, but rely only on MODE. */
void strmode(mode_t mode, char *str)
{
  str[0] = ftypelet(mode);
  str[1] = mode & S_IRUSR ? 'r' : '-';
  str[2] = mode & S_IWUSR ? 'w' : '-';
  str[3] = (mode & S_ISUID
    ? (mode & S_IXUSR ? 's' : 'S')
    : (mode & S_IXUSR ? 'x' : '-'));
  str[4] = mode & S_IRGRP ? 'r' : '-';
  str[5] = mode & S_IWGRP ? 'w' : '-';
  str[6] = (mode & S_ISGID
    ? (mode & S_IXGRP ? 's' : 'S')
    : (mode & S_IXGRP ? 'x' : '-'));
  str[7] = mode & S_IROTH ? 'r' : '-';
  str[8] = mode & S_IWOTH ? 'w' : '-';
  str[9] = (mode & S_ISVTX
    ? (mode & S_IXOTH ? 't' : 'T')
    : (mode & S_IXOTH ? 'x' : '-'));
  str[10] = ' ';
  str[11] = '\0';
}

/*
 END filemode.c
*/

int list_sub(char *dirname, SOCKET s, gnutls_session_t session, fileinfo_t *entry)
{
  char  text[SIZE_OF_GPBUFFER],
    sacl[12] = "";

  struct tm ftm_fields;
  time_t  deltatime;

  if (strcmp(entry->name, ".") == 0)
    return 1;
  if (strcmp(entry->name, "..") == 0)
    return 1;

  strcpy(text, dirname);
  add_last_slash(text);
  strcat(text, entry->name);

  if (1)
  {
    strmode(entry->attrib, sacl);

    localtime_r(&entry->wtime, &ftm_fields);
    deltatime = time(NULL) - entry->wtime;
    if (deltatime <= 180 * 24 * 60 * 60) {
      snprintf(text, sizeof(text), "%s %lu %lu %lu %lu %s %02u %02u:%02u %s\r\n",
        sacl, entry->st_nlink,
        (unsigned long int)entry->st_uid,
        (unsigned long int)entry->st_gid,
        (unsigned long int)entry->size,
        shortmonths[(ftm_fields.tm_mon)], ftm_fields.tm_mday,
        ftm_fields.tm_hour, ftm_fields.tm_min, entry->name);
    }
    else
    {
      snprintf(text, sizeof(text), "%s %lu %lu %lu %lu %s %02u %02u %s\r\n",
        sacl, entry->st_nlink,
        (unsigned int)entry->st_uid,
        (unsigned int)entry->st_gid,
        (unsigned int)entry->size,
        shortmonths[(ftm_fields.tm_mon)], ftm_fields.tm_mday,
        ftm_fields.tm_year + 1900, entry->name);
    }
  }

  return sendstring_auto(s, session, text);
}

#define sys_cleanup_push(a, b)

void *list_thread(PFTPCONTEXT context)
{
  SOCKET  clientsocket;
  gnutls_session_t TLS_datasession;
  int   ret;
  dir_t pdir[1] = { 0 };
  fileinfo_t entry[1];
  int flag = context->utf8 ? FF_UTF8 : 0;
  sys_mutex_lock(&context->MTLock);
  sys_cleanup_push(cleanup_handler, context);
  ret = 0;
  TLS_datasession = NULL;

  clientsocket = create_datasocket(context);
  if (clientsocket != INVALID_SOCKET)
  {
    if (context->TLS_session != NULL)
      InitTLSSession(&TLS_datasession, clientsocket, 0);

    while (sys_find_next_file(pdir, context->GPBuffer, NULL, entry, flag)) {
      ret = list_sub(context->GPBuffer, clientsocket, TLS_datasession, entry);
      if ((ret == 0) || (context->WorkerThreadAbort != 0))
        break;
    }

    sys_find_close(pdir);
  }
  if (TLS_datasession != NULL)
  {
    gnutls_bye(TLS_datasession, GNUTLS_SHUT_RDWR);
    gnutls_deinit(TLS_datasession);
  }

  writelogentry(context, " LIST complete", "");

  if (clientsocket == INVALID_SOCKET) {
    sendstring(context, error451);
  }
  else {
    if ((context->WorkerThreadAbort == 0) && (ret != 0))
      sendstring(context, success226);
    else
      sendstring(context, error426);

    closesocket(clientsocket);
  }

  context->WorkerThreadValid = -1;
  sys_cleanup_pop(0);
  sys_mutex_unlock(&context->MTLock);
  return NULL;
}

int ftpLIST(PFTPCONTEXT context, const char *params)
{
  thread_t tid = {0};

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->WorkerThreadValid == 0)
    return sendstring(context, error550_t);

  if (params != NULL)
  {
    if ((strcmp(params, "-a") == 0) || (strcmp(params, "-l") == 0))
      params = NULL;
  }

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (is_directory(context->GPBuffer)) {
    sendstring(context, interm150);
    writelogentry(context, " LIST", (char *)params);
    context->WorkerThreadAbort = 0;
    sys_mutex_lock(&context->MTLock);
    context->WorkerThreadValid = sys_thread_create(&tid, NULL, (thread_callback)list_thread, context);
    if (context->WorkerThreadValid == 0)
      sendstring(context, error451);

    sys_mutex_unlock(&context->MTLock);

    return 1;
  }

  return sendstring(context, error550);
}

int ftpCDUP(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  if (strcmp(context->CurrentDir, "/") == 0)
    return sendstring(context, success250);

  delete_last_slash(context->CurrentDir);
  filepath(context->CurrentDir);

  writelogentry(context, " CDUP", "");
  return sendstring(context, success250);
}

int ftpCWD(PFTPCONTEXT context, const char *params)
{
  size_t  rl;

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  if (params == NULL)
    return sendstring(context, error501);

  if (finalpath(context->RootDir, context->CurrentDir, (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (is_directory(context->GPBuffer)) {
    rl = strlen(context->RootDir);
    strcpy(context->CurrentDir, context->GPBuffer + rl);
    writelogentry(context, " CWD: ", context->CurrentDir);
    return sendstring(context, success250);
  }

  return sendstring(context, error550);
}

void *retr_thread(PFTPCONTEXT context)
{
  volatile SOCKET clientsocket;
  int   sent_ok, f;
  off_t  offset;
  ssize_t  sz, sz_total;
  size_t  buffer_size;
  char  *buffer;
  int64 lt0, lt1, dtx;
  gnutls_session_t TLS_datasession;

  sys_mutex_lock(&context->MTLock);
  sys_cleanup_push(cleanup_handler, context);

  sent_ok = 0;
  sz_total = 0;
  buffer = NULL;
  TLS_datasession = NULL;
  f = -1;
  lt0 = utime_counter();

  buffer = (char*)malloc(TRANSMIT_BUFFER_SIZE);
  while (buffer != NULL)
  {
    clientsocket = create_datasocket(context);
    if (clientsocket == INVALID_SOCKET)
      break;

    if (context->TLS_session != NULL)
    {
      InitTLSSession(&TLS_datasession, clientsocket, 0);
      buffer_size = gnutls_record_get_max_size(TLS_datasession);
      if (buffer_size > TRANSMIT_BUFFER_SIZE)
        buffer_size = TRANSMIT_BUFFER_SIZE;
    }
    else
      buffer_size = TRANSMIT_BUFFER_SIZE;

    f = open(context->GPBuffer, O_RDONLY|_O_BINARY);
    context->File = f;
    if (f == -1)
      break;

    offset = lseek(f, context->RestPoint, SEEK_SET);
    if (offset != context->RestPoint)
      break;

    while (context->WorkerThreadAbort == 0) {
      sz = read(f, buffer, buffer_size);
      if (sz <= 0)
        break;

      sz_total += sz;

      if (send_auto(clientsocket, TLS_datasession, buffer, sz) == sz)
        sent_ok = 1;
      else
      {
        sent_ok = 0;
        break;
      }
    }

    break;
  }

  lt1 = utime_counter();

  if (f != -1)
    closesocket(f);

  context->File = -1;

  if (TLS_datasession != NULL)
  {
    gnutls_bye(TLS_datasession, GNUTLS_SHUT_RDWR);
    gnutls_deinit(TLS_datasession);
  }

  /* calculating performance */
  dtx = lt1 - lt0;

  if (buffer != NULL) {
    sprintf(buffer, " RETR complete. %zd bytes (%f MBytes) total sent in %f seconds (%f MBytes/s)",
      sz_total, sz_total / 1048576.0, dtx / 1000000000.0, (1000000000.0*sz_total) / dtx / 1048576);
    writelogentry(context, buffer, "");
    free(buffer);
  }

  if (clientsocket == INVALID_SOCKET) {
    sendstring(context, error451);
  }
  else {
    if ((context->WorkerThreadAbort == 0) && (sent_ok != 0))
      sendstring(context, success226);
    else
      sendstring(context, error426);

    closesocket(clientsocket);
  }

  context->WorkerThreadValid = -1;
  sys_cleanup_pop(0);
  sys_mutex_unlock(&context->MTLock);
  return NULL;
}

int ftpRETR(PFTPCONTEXT context, const char *params)
{
  thread_t tid = {0};

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->WorkerThreadValid == 0)
    return sendstring(context, error550_t);
  if (params == NULL)
    return sendstring(context, error501);

  if (context->File != -1) {
    closesocket(context->File);
    context->File = -1;
  }

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (!is_directory(context->GPBuffer)) {
    sendstring(context, interm150);
    writelogentry(context, " RETR: ", (char *)params);
    context->WorkerThreadAbort = 0;

    sys_mutex_lock(&context->MTLock);

    context->WorkerThreadValid = sys_thread_create(&tid, NULL, (thread_callback)&retr_thread, context);
    if (context->WorkerThreadValid == 0)
      sendstring(context, error451);

    sys_mutex_unlock(&context->MTLock);

    return 1;
  }

  return sendstring(context, error550);
}

int ftpABOR(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  writelogentry(context, " ABORT command", NULL);
  WorkerThreadCleanup(context);
  return sendstring(context, success226);
}

int ftpDELE(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->Access < FTP_ACCESS_FULL)
    return sendstring(context, error550_r);
  if (params == NULL)
    return sendstring(context, error501);

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (unlink(context->GPBuffer) == 0) {
    sendstring(context, success250);
    writelogentry(context, " DELE: ", (char *)params);
  }
  else
    sendstring(context, error550_r);

  return 1;
}

int pasv(PFTPCONTEXT context)
{
  SOCKET  datasocket;
  struct sockaddr_in laddr;
  int   socketret = -1, result = 0;
  unsigned long c;

  while (1)
  {
    if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    {
      sendstring(context, error530);
      break;
    }

    if (context->WorkerThreadValid == 0)
    {
      sendstring(context, error550_t);
      break;
    }

    if (context->DataSocket != INVALID_SOCKET)
      closesocket(context->DataSocket);

    context->DataSocket = INVALID_SOCKET;

    datasocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (datasocket == INVALID_SOCKET)
    {
      sendstring(context, error451);
      break;
    }

    for (c = g_cfg.PasvPortBase; c <= g_cfg.PasvPortMax; c++) {
      int r = rand();
      memset(&laddr, 0, sizeof(laddr));
      laddr.sin_family = AF_INET;
      laddr.sin_port = htons((in_port_t)(g_cfg.PasvPortBase + (r % (g_cfg.PasvPortMax - g_cfg.PasvPortBase))));
      laddr.sin_addr.s_addr = context->ServerIPv4;
      socketret = bind(datasocket, (struct sockaddr *)&laddr, sizeof(laddr));
      if (socketret == 0)
        break;
    }

    if (socketret != 0) {
      closesocket(datasocket);
      sendstring(context, error451);
      break;
    }

    socketret = listen(datasocket, SOMAXCONN);
    if (socketret != 0) {
      closesocket(datasocket);
      sendstring(context, error451);
      break;
    }

    if ((context->ClientIPv4 & g_cfg.LocalIPMask) == (context->ServerIPv4 & g_cfg.LocalIPMask))
    {
      context->DataIPv4 = context->ServerIPv4;
      writelogentry(context, " local client.", "");
    }
    else {
      context->DataIPv4 = g_cfg.ExternalInterface;
      writelogentry(context, " nonlocal client.", "");
    }

    context->DataPort = laddr.sin_port;
    context->DataSocket = datasocket;
    context->Mode = MODE_PASSIVE;

    result = 1;
    break;
  }

  return result;
}

int ftpEPSV(PFTPCONTEXT context, const char *params)
{
  if (pasv(context) == 0)
    return 1;

  snprintf(context->GPBuffer, SIZE_OF_GPBUFFER, success229,
    ntohs(context->DataPort));

  writelogentry(context, " entering extended passive mode", "");

  return sendstring(context, context->GPBuffer);
}

int ftpPASV(PFTPCONTEXT context, const char *params)
{
  if (pasv(context) == 0)
    return 1;

  snprintf(context->GPBuffer, SIZE_OF_GPBUFFER, success227,
    context->DataIPv4 & 0xff,
    (context->DataIPv4 >> 8) & 0xff,
    (context->DataIPv4 >> 16) & 0xff,
    (context->DataIPv4 >> 24) & 0xff,
    context->DataPort & 0xff,
    (context->DataPort >> 8) & 0xff);

  writelogentry(context, " entering passive mode", "");

  return sendstring(context, context->GPBuffer);
}

int ftpPASS(PFTPCONTEXT context, const char *params)
{
  char temptext[256];

  if (params == NULL)
    return sendstring(context, error501);

  memset(temptext, 0, sizeof(temptext));

  /*
   * we have username saved in context->GPBuffer from USER command
   */
  if (!ParseConfig(g_cfg.ConfigFile, context->GPBuffer, "pswd", temptext, sizeof(temptext)))
    return sendstring(context, error530_r);

  if ((strcmp(temptext, params) == 0) || (temptext[0] == '*'))
  {
    memset(context->RootDir, 0, sizeof(context->RootDir));
    memset(temptext, 0, sizeof(temptext));

    ParseConfig(g_cfg.ConfigFile, context->GPBuffer, "root", context->RootDir, sizeof(context->RootDir));
    ParseConfig(g_cfg.ConfigFile, context->GPBuffer, "accs", temptext, sizeof(temptext));

    context->Access = FTP_ACCESS_NOT_LOGGED_IN;
    do {

      if (strcasecmp(temptext, "admin") == 0) {
        context->Access = FTP_ACCESS_FULL;
        break;
      }

      if (strcasecmp(temptext, "upload") == 0) {
        context->Access = FTP_ACCESS_CREATENEW;
        break;
      }

      if (strcasecmp(temptext, "readonly") == 0) {
        context->Access = FTP_ACCESS_READONLY;
        break;
      }

      return sendstring(context, error530_b);
    } while (0);

    writelogentry(context, " PASS->successful logon", "");
  }
  else
    return sendstring(context, error530_r);

  return sendstring(context, success230);
}

int ftpREST(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  if (params == NULL)
    return sendstring(context, error501);

  context->RestPoint = strtol(params, NULL, 10);
  snprintf(context->GPBuffer, SIZE_OF_GPBUFFER, "%s %llu\r\n",
    interm350, (uint64)context->RestPoint);

  return sendstring(context, context->GPBuffer);
}

int ftpSIZE(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (params == NULL)
    return sendstring(context, error501);

  if (finalpath(context->RootDir, context->CurrentDir, (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (!is_directory(context->GPBuffer))
  {
    uint64 size = sys_filesize(context->GPBuffer);
    snprintf(context->GPBuffer, SIZE_OF_GPBUFFER, "213 %llu\r\n",
      (uint64)size);
    sendstring(context, context->GPBuffer);
  }
  else
    sendstring(context, error550);

  return 1;
}

int ftpMKD(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->Access < FTP_ACCESS_CREATENEW)
    return sendstring(context, error550_r);
  if (params == NULL)
    return sendstring(context, error501);

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (mkdirs(context->GPBuffer) == 0) {
    sendstring(context, success257);
    writelogentry(context, " MKD: ", (char *)params);
  }
  else
    sendstring(context, error550_r);

  return 1;
}

int ftpRMD(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->Access < FTP_ACCESS_FULL)
    return sendstring(context, error550_r);
  if (params == NULL)
    return sendstring(context, error501);

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (rmdir(context->GPBuffer) == 0) {
    sendstring(context, success250);
    writelogentry(context, " DELE: ", (char *)params);
  }
  else
    sendstring(context, error550_r);

  return 1;
}

void *stor_thread(PFTPCONTEXT context)
{
  SOCKET  clientsocket;
  int   f;
  ssize_t  sz, sz_total;
  char  *buffer;
  int64 lt0, lt1, dtx;
  gnutls_session_t TLS_datasession;

  sys_mutex_lock(&context->MTLock);
  sys_cleanup_push(cleanup_handler, context);

  f = -1;
  sz_total = 0;
  buffer = NULL;
  TLS_datasession = NULL;
  lt0 = utime_counter();

  clientsocket = create_datasocket(context);
  while (clientsocket != INVALID_SOCKET)
  {
    buffer = (char*)malloc(TRANSMIT_BUFFER_SIZE);
    if (buffer == NULL)
      break;

    if (context->TLS_session != NULL)
      InitTLSSession(&TLS_datasession, clientsocket, 0);

    f = open(context->GPBuffer, context->CreateMode);
    context->File = f;
    if (f == -1)
      break;

    while (context->WorkerThreadAbort == 0) {
      sz = recv_auto(clientsocket, TLS_datasession, buffer, TRANSMIT_BUFFER_SIZE);
      if (sz > 0)
      {
        sz_total += sz;
        write(f, buffer, sz);
      }
      else
        break;
    }

    break;
  }

  lt1 = utime_counter();

  if (f != -1)
    closesocket(f);

  context->File = -1;

  if (TLS_datasession != NULL)
  {
    gnutls_bye(TLS_datasession, GNUTLS_SHUT_RDWR);
    gnutls_deinit(TLS_datasession);
  }

  /* calculating performance */
  if (buffer != NULL)
  {
    dtx = lt1 - lt0;
    sprintf(buffer, " STOR complete. %zd bytes (%f MBytes) total sent in %f seconds (%f MBytes/s)",
      sz_total, sz_total / 1048576.0, dtx / 1000000000.0, (1000000000.0*sz_total) / dtx / 1048576);
    writelogentry(context, buffer, "");
    free(buffer);
  }

  if (clientsocket == INVALID_SOCKET) {
    sendstring(context, error451);
  }
  else {
    if (context->WorkerThreadAbort == 0)
      sendstring(context, success226);
    else
      sendstring(context, error426);

    closesocket(clientsocket);
  }

  context->WorkerThreadValid = -1;
  sys_cleanup_pop(0);
  sys_mutex_unlock(&context->MTLock);
  return NULL;
}

int ftpSTOR(PFTPCONTEXT context, const char *params)
{
  thread_t tid = {0};

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->Access < FTP_ACCESS_CREATENEW)
    return sendstring(context, error550_r);
  if (params == NULL)
    return sendstring(context, error501);
  if (context->WorkerThreadValid == 0)
    return sendstring(context, error550_t);

  if (context->File != -1) {
    closesocket(context->File);
    context->File = -1;
  }

  if (finalpath(context->RootDir, context->CurrentDir, (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (context->Access == FTP_ACCESS_FULL)
    context->CreateMode = O_CREAT | O_WRONLY | O_TRUNC;
  else
  {
    context->CreateMode = O_CREAT | O_WRONLY | O_EXCL;
    if (is_directory(context->GPBuffer))
      return sendstring(context, error550_r);
  }

  sendstring(context, interm150);
  writelogentry(context, " STOR: ", (char *)params);
  context->WorkerThreadAbort = 0;

  sys_mutex_lock(&context->MTLock);

  context->WorkerThreadValid = sys_thread_create(&tid, NULL, (thread_callback)&stor_thread, context);
  if (context->WorkerThreadValid == 0)
    sendstring(context, error451);

  sys_mutex_unlock(&context->MTLock);

  return 1;
}

int ftpSYST(PFTPCONTEXT context, const char *params)
{
  return sendstring(context, success215);
}

int ftpHELP(PFTPCONTEXT context, const char *params)
{
  return sendstring(context, success214);
}

int ftpSITE(PFTPCONTEXT context, const char *params)
{
  if (params != NULL)
    if (strcasecmp(params, "help") == 0)
      return sendstring(context, "200 chmod\r\n");

  return sendstring(context, error500);
}

int ftpFEAT(PFTPCONTEXT context, const char *params)
{
  return sendstring(context, success211);
}

void *append_thread(PFTPCONTEXT context)
{
  SOCKET  clientsocket;
  int   f = -1;
  ssize_t  sz;
  char  *buffer = NULL;
  gnutls_session_t TLS_datasession;

  sys_mutex_lock(&context->MTLock);
  sys_cleanup_push(cleanup_handler, context);
  TLS_datasession = NULL;

  clientsocket = create_datasocket(context);
  while (clientsocket != INVALID_SOCKET)
  {
    if (context->TLS_session != NULL)
      InitTLSSession(&TLS_datasession, clientsocket, 0);

    f = open(context->GPBuffer, O_RDWR);
    context->File = f;
    if (f == -1)
      break;

    lseek(f, 0, SEEK_END);
    buffer = (char*)malloc(TRANSMIT_BUFFER_SIZE);
    if (buffer == NULL)
      break;

    while (context->WorkerThreadAbort == 0) {
      sz = recv_auto(clientsocket, TLS_datasession, buffer, TRANSMIT_BUFFER_SIZE);
      if (sz > 0)
        write(f, buffer, sz);
      else
        break;
    }

    break;
  }

  if (buffer != NULL)
    free(buffer);

  if (f != -1)
    closesocket(f);

  context->File = -1;

  if (TLS_datasession != NULL)
  {
    gnutls_bye(TLS_datasession, GNUTLS_SHUT_RDWR);
    gnutls_deinit(TLS_datasession);
  }

  writelogentry(context, " STOR complete", "");

  if (clientsocket == INVALID_SOCKET) {
    sendstring(context, error451);
  }
  else {
    if (context->WorkerThreadAbort == 0)
      sendstring(context, success226);
    else
      sendstring(context, error426);

    closesocket(clientsocket);
  }

  context->WorkerThreadValid = -1;
  sys_cleanup_pop(0);
  sys_mutex_unlock(&context->MTLock);
  return NULL;
}

int ftpAPPE(PFTPCONTEXT context, const char *params)
{
  thread_t tid = {0};

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->Access < FTP_ACCESS_FULL)
    return sendstring(context, error550_r);
  if (params == NULL)
    return sendstring(context, error501);
  if (context->WorkerThreadValid == 0)
    return sendstring(context, error550_t);

  if (context->File != -1) {
    closesocket(context->File);
    context->File = -1;
  }

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  /*
   * stat must NOT fail
   */
  if (is_directory(context->GPBuffer))
  {
    // do not try to "append" for directories
    sendstring(context, interm150);
    writelogentry(context, " APPE: ", (char *)params);
    context->WorkerThreadAbort = 0;

    sys_mutex_lock(&context->MTLock);

    context->WorkerThreadValid = sys_thread_create(&tid, NULL, (thread_callback)&append_thread, context);
    if (context->WorkerThreadValid == 0)
      sendstring(context, error451);

    sys_mutex_unlock(&context->MTLock);

    return 1;
  }

  return sendstring(context, error550);
}

int ftpRNFR(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->Access < FTP_ACCESS_FULL)
    return sendstring(context, error550_r);
  if (params == NULL)
    return sendstring(context, error501);

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (is_directory(context->GPBuffer))
  {
    writelogentry(context, " RNFR: ", context->GPBuffer);
    sendstring(context, interm350_ren);
  }
  else
    sendstring(context, error550);

  return 1;
}

int ftpRNTO(PFTPCONTEXT context, const char *params)
{
  char *_text = 0;

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->Access < FTP_ACCESS_FULL)
    return sendstring(context, error550_r);
  if (params == NULL)
    return sendstring(context, error501);

  _text = (char*)malloc(SIZE_OF_GPBUFFER);

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, _text) == NULL)
  {
    free(_text);
    return 0;
  }

  if (rename(context->GPBuffer, _text) == 0)
  {
    writelogentry(context, " RNTO: ", _text);
    sendstring(context, success250);
  }
  else
    sendstring(context, error550);

  free(_text);
  return 1;
}

int ftpOPTS(PFTPCONTEXT context, const char *params)
{
  if (params != NULL) {
    if (strcasecmp(params, "utf8 on") == 0) {
      context->utf8 = 1;
      return sendstring(context, "200 Always in UTF8 mode.\r\n");
    }
  }

  writelogentry(context, " unsupported OPTS: ", params);
  return sendstring(context, error500);
}

int ftpAUTH(PFTPCONTEXT context, const char *params)
{
  if (params == NULL)
    return sendstring(context, error501);

  if (strcasecmp(params, "TLS") == 0)
    /* InitTLSSession will send reply */
    return InitTLSSession(&context->TLS_session, context->ControlSocket, 1);
  else
    return sendstring(context, error504);
}

int ftpPBSZ(PFTPCONTEXT context, const char *params)
{
  if (params == NULL)
    return sendstring(context, error501);

  if (context->TLS_session == NULL)
    return sendstring(context, error503);

  context->BlockSize = strtoul(params, NULL, 10);
  return sendstring(context, success200);
}

int ftpPROT(PFTPCONTEXT context, const char *params)
{
  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);

  if (params == NULL)
    return sendstring(context, error501);

  if (context->TLS_session == NULL)
    return sendstring(context, error503);

  switch (*params)
  {
  case 'C':
    context->DataProtectionLevel = 0;
    return sendstring(context, success200);
    break;

  case 'P':
    context->DataProtectionLevel = 100;
    return sendstring(context, success200);
    break;

  default:
    return sendstring(context, error504);
  }
}

int mlsd_sub(char *dirname, SOCKET s, gnutls_session_t session, fileinfo_t *entry)
{
  char  text[SIZE_OF_GPBUFFER], *entrytype, *sizetype;
  struct tm ftm_fields;
  //sys_stat entry[1] = {0};

  if (strcmp(entry->name, ".") == 0)
    return 1;
  if (strcmp(entry->name, "..") == 0)
    return 1;

  strcpy(text, dirname);
  add_last_slash(text);
  strcat(text, entry->name);

  if (1) {
    if (entry->attrib & S_IFDIR)
    {
      entrytype = "dir";
      sizetype = "sizd";
    }
    else
    {
      entrytype = "file";
      sizetype = "size";
    }
#ifndef _WIN32
    if (S_ISLNK(entry->attrib))
    {
      entrytype = "OS.unix=slink";
    }
#endif

    localtime_r(&entry->wtime, &ftm_fields);
    ++ftm_fields.tm_mon;

    snprintf(text, sizeof(text),
      "type=%s;%s=%llu;UNIX.mode=%lo;UNIX.owner=%lu;UNIX.group=%lu;modify=%u%02u%02u%02u%02u%02u; %s\r\n",
      entrytype, sizetype,
      (uint64)entry->size,
      (unsigned long int)entry->attrib,
      (unsigned long int)entry->st_uid,
      (unsigned long int)entry->st_gid,
      ftm_fields.tm_year + 1900, ftm_fields.tm_mon, ftm_fields.tm_mday,
      ftm_fields.tm_hour, ftm_fields.tm_min, ftm_fields.tm_sec, entry->name
    );
  }

  return sendstring_auto(s, session, text);
}

void *msld_thread(PFTPCONTEXT context)
{
  SOCKET  clientsocket;
  gnutls_session_t TLS_datasession;
  int   ret;
  dir_t pdir[1] = { 0 };
  fileinfo_t entry[1] = { 0 };

  sys_mutex_lock(&context->MTLock);
  sys_cleanup_push(cleanup_handler, context);
  ret = 0;
  TLS_datasession = NULL;
  int flag = context->utf8 ? FF_UTF8 : 0;

  clientsocket = create_datasocket(context);
  if (clientsocket != INVALID_SOCKET)
  {
    if (context->TLS_session != NULL)
      InitTLSSession(&TLS_datasession, clientsocket, 0);

    while (sys_find_next_file(pdir, context->GPBuffer, NULL, entry, flag)) {
      ret = mlsd_sub(context->GPBuffer, clientsocket, TLS_datasession, entry);
      if ((ret == 0) || (context->WorkerThreadAbort != 0))
        break;
    }

    sys_find_close(pdir);
  }

  if (TLS_datasession != NULL)
  {
    gnutls_bye(TLS_datasession, GNUTLS_SHUT_RDWR);
    gnutls_deinit(TLS_datasession);
  }

  writelogentry(context, " LIST complete", "");

  if (clientsocket == INVALID_SOCKET) {
    sendstring(context, error451);
  }
  else {
    if ((context->WorkerThreadAbort == 0) && (ret != 0))
      sendstring(context, success226);
    else
      sendstring(context, error426);

    closesocket(clientsocket);
  }

  context->WorkerThreadValid = -1;
  sys_cleanup_pop(0);
  sys_mutex_unlock(&context->MTLock);
  return NULL;
}

int ftpMLSD(PFTPCONTEXT context, const char *params)
{
  thread_t tid = {0};

  if (context->Access == FTP_ACCESS_NOT_LOGGED_IN)
    return sendstring(context, error530);
  if (context->WorkerThreadValid == 0)
    return sendstring(context, error550_t);

  if (finalpath(
    context->RootDir,
    context->CurrentDir,
    (char *)params, context->GPBuffer) == NULL)
    return 0;

  if (is_directory(context->GPBuffer))
  {
    sendstring(context, interm150);
    writelogentry(context, " MLSD-LIST ", (char *)params);
    context->WorkerThreadAbort = 0;

    sys_mutex_lock(&context->MTLock);

    context->WorkerThreadValid = sys_thread_create(&tid, NULL, (thread_callback)&msld_thread, context);
    if (context->WorkerThreadValid == 0)
      sendstring(context, error451);

    sys_mutex_unlock(&context->MTLock);

    return 1;
  }

  return sendstring(context, error550);
}

int recvcmd(PFTPCONTEXT context, char *buffer, size_t buffer_size)
{
  ssize_t l, p = 0;

  if (buffer_size < 5)
    return 0;

  memset(buffer, 0, buffer_size);
  --buffer_size;

  while (buffer_size > 0)
  {
    if (context->TLS_session == NULL)
      l = recv(context->ControlSocket, buffer + p, buffer_size, 0);
    else
      l = gnutls_record_recv(context->TLS_session, buffer + p, buffer_size);

    if (l <= 0)
      return 0;

    buffer_size -= l;
    p += l;

    if (p >= 2)
      if ((buffer[p - 2] == '\r') && (buffer[p - 1] == '\n'))
      {
        buffer[p - 2] = 0;
        return 1;
      }
  }

  return 0;
}

static const char *ftpcmds[MAX_CMDS] = {
  "USER", "QUIT", "NOOP", "PWD", "TYPE", "PORT", "LIST", "CDUP",
  "CWD", "RETR", "ABOR", "DELE", "PASV", "PASS", "REST", "SIZE",
  "MKD", "RMD", "STOR", "SYST", "FEAT", "APPE", "RNFR", "RNTO",
  "OPTS", "MLSD", "AUTH", "PBSZ", "PROT", "EPSV", "HELP", "SITE"
};

static const FTPROUTINE ftpprocs[MAX_CMDS] = {
  ftpUSER, ftpQUIT, ftpNOOP, ftpPWD, ftpTYPE, ftpPORT, ftpLIST, ftpCDUP,
  ftpCWD, ftpRETR, ftpABOR, ftpDELE, ftpPASV, ftpPASS, ftpREST, ftpSIZE,
  ftpMKD, ftpRMD, ftpSTOR, ftpSYST, ftpFEAT, ftpAPPE, ftpRNFR, ftpRNTO,
  ftpOPTS, ftpMLSD, ftpAUTH, ftpPBSZ, ftpPROT, ftpEPSV, ftpHELP, ftpSITE
};

void *ftp_client_thread(SOCKET *s)
{
  FTPCONTEXT  ctx;
  char   *cmd, *params, rcvbuf[FTP_PATH_MAX * 2];
  int   c, cmdno, rv;
  size_t   i, cmdlen;
  socklen_t  asz;
  struct sockaddr_in laddr;

  memset(&ctx, 0, sizeof(ctx));
  ctx.Access = FTP_ACCESS_NOT_LOGGED_IN;
  ctx.ControlSocket = *s;
  ctx.GPBuffer = (char*)malloc(SIZE_OF_GPBUFFER);
  ctx.utf8 = 0;
  *ctx.GPBuffer = 0;

  memset(&laddr, 0, sizeof(laddr));
  asz = sizeof(laddr);
  while (getsockname(ctx.ControlSocket, (struct sockaddr *)&laddr, &asz) == 0)
  {
    ctx.ServerIPv4 = laddr.sin_addr.s_addr;

    memset(&laddr, 0, sizeof(laddr));
    asz = sizeof(laddr);
    if (getpeername(ctx.ControlSocket, (struct sockaddr *)&laddr, &asz) != 0)
      break;

    ctx.ClientIPv4 = laddr.sin_addr.s_addr;
    ctx.Mode = MODE_NORMAL;
    ctx.WorkerThreadAbort = 0;
    ctx.WorkerThreadValid = -1;
    ctx.SessionID = 0;
    ctx.File = -1;
    ctx.DataSocket = INVALID_SOCKET;

    sys_mutex_init(&ctx.MTLock, NULL);

    ctx.CurrentDir[0] = '/';
    sendstring(&ctx, success220);

    memset(&rcvbuf, 0, sizeof(rcvbuf));

    snprintf(rcvbuf, sizeof(rcvbuf), "<- New user IP=%u.%u.%u.%u:%u",
      laddr.sin_addr.s_addr & 0xff,
      (laddr.sin_addr.s_addr >> 8) & 0xff,
      (laddr.sin_addr.s_addr >> 16) & 0xff,
      (laddr.sin_addr.s_addr >> 24) & 0xff,
      ntohs(laddr.sin_port)
    );

    writelogentry(&ctx, rcvbuf, "");

    while (ctx.ControlSocket != INVALID_SOCKET) {
      if (!recvcmd(&ctx, rcvbuf, sizeof(rcvbuf)))
        break;

      i = 0;
      while ((rcvbuf[i] != 0) && (isalpha(rcvbuf[i]) == 0))
        ++i;

      cmd = &rcvbuf[i];
      while ((rcvbuf[i] != 0) && (rcvbuf[i] != ' '))
        ++i;

      cmdlen = &rcvbuf[i] - cmd;
      while (rcvbuf[i] == ' ')
        ++i;

      if (rcvbuf[i] == 0)
        params = NULL;
      else
        params = &rcvbuf[i];

      cmdno = -1;
      for (c = 0; c < MAX_CMDS; c++)
        if (strncasecmp(cmd, ftpcmds[c], cmdlen) == 0)
        {
          cmdno = c;
          rv = ftpprocs[c](&ctx, params);
          break;
        }

      if (cmdno != FTP_PASSCMD_INDEX)
        writelogentry(&ctx, " @@ CMD: ", rcvbuf);
      else
        writelogentry(&ctx, " @@ CMD: ", "PASS ***");

      if (cmdno == -1)
        sendstring(&ctx, error500);

      if (rv <= 0)
        break;
    };

    WorkerThreadCleanup(&ctx);

    sys_mutex_destroy(&ctx.MTLock);
    writelogentry(&ctx, "User disconnected", "");
    break;
  }

  if (ctx.TLS_session != NULL)
    gnutls_deinit(ctx.TLS_session);

  free(ctx.GPBuffer);
  closesocket(ctx.ControlSocket);
  *s = INVALID_SOCKET;
  return NULL;
}

void *ftpmain(void *p)
{
  struct sockaddr_in laddr;

  int ftpsocket = INVALID_SOCKET,
    clientsocket,
    socketret,
    rv;
  SOCKET *scb = NULL;

  socklen_t asz;
  uint32 i;
  thread_t th = {0};

  ftpsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ftpsocket == INVALID_SOCKET)
  {
    printf("\r\n socket create error\r\n");
    return 0;
  }

  rv = 1;
  setsockopt(ftpsocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&rv, sizeof(rv));

  scb = (SOCKET *)malloc(sizeof(SOCKET)*g_cfg.MaxUsers);
  for (i = 0; i < g_cfg.MaxUsers; i++)
    scb[i] = INVALID_SOCKET;

  memset(&laddr, 0, sizeof(laddr));
  laddr.sin_family = AF_INET;
  laddr.sin_port = htons(g_cfg.Port);
  laddr.sin_addr.s_addr = g_cfg.BindToInterface;
  socketret = bind(ftpsocket, (struct sockaddr *)&laddr, sizeof(laddr));
  if (socketret != 0) {
    printf("\r\n Failed to start server. Can not bind to address\r\n\r\n");
    free(scb);
    closesocket(ftpsocket);
    return 0;
  }

  writelogentry(NULL, success220, "");

  socketret = listen(ftpsocket, SOMAXCONN);
  while (socketret == 0) {
    memset(&laddr, 0, sizeof(laddr));
    asz = sizeof(laddr);
    clientsocket = accept(ftpsocket, (struct sockaddr *)&laddr, &asz);
    if (clientsocket != INVALID_SOCKET) {
      rv = -1;
      for (i = 0; i < g_cfg.MaxUsers; i++) {
        if (scb[i] == INVALID_SOCKET) {

          scb[i] = clientsocket;
          //rv = sys_thread_create(&th, NULL, (thread_callback)&ftp_client_thread, &scb[i]);
          ftp_client_thread(&scb[i]);
          if (rv != 0)
            scb[i] = INVALID_SOCKET;

          break;
        }
      }

      if (rv != 0) {
        sendstring_plaintext(clientsocket, NOSLOTS);
        closesocket(clientsocket);
      }
    }
  }

  free(scb);
  closesocket(ftpsocket);

  return NULL;
}



//FTP_CONFIG	g_cfg;
//int			g_log = -1;

typedef void* gnutls_dh_params_t;

gnutls_dh_params_t					dh_params = NULL;
//gnutls_certificate_credentials_t	x509_cred = NULL;
//gnutls_priority_t					priority_cache = NULL;

static	char	CAFILE[PATH_MAX],
CERTFILE[PATH_MAX],
KEYFILE[PATH_MAX],
KEYFILE_PASS[256];

void TLSInit()
{
#ifdef _WIN32
#else
  while (gnutls_global_init() >= 0)
  {
    if (gnutls_certificate_allocate_credentials(&x509_cred) < 0)
      break;

    if (gnutls_certificate_set_x509_trust_file(x509_cred, CAFILE,
      GNUTLS_X509_FMT_PEM) < 0)
      break;

    if (gnutls_certificate_set_x509_key_file2(x509_cred, CERTFILE,
      KEYFILE, GNUTLS_X509_FMT_PEM, KEYFILE_PASS, 0) < 0)
      break;

    if (gnutls_priority_init(&priority_cache, NULL, NULL) < 0)
      break;

#if GNUTLS_VERSION_NUMBER >= 0x030506
    gnutls_certificate_set_known_dh_params(x509_cred, GNUTLS_SEC_PARAM_HIGH);
#else
    gnutls_dh_params_init(&dh_params);
    gnutls_dh_params_generate2(dh_params,
      gnutls_sec_param_to_pk_bits(GNUTLS_PK_DH, GNUTLS_SEC_PARAM_HIGH));
    gnutls_certificate_set_dh_params(x509_cred, dh_params);
#endif
    break;
  }
#endif
}

void TLSCleanup()
{
#ifdef _WIN32
#else
#if GNUTLS_VERSION_NUMBER < 0x030506
  if (dh_params != NULL)
    gnutls_dh_params_deinit(dh_params);
#endif

  if (x509_cred != NULL)
    gnutls_certificate_free_credentials(x509_cred);

  if (priority_cache != NULL)
    gnutls_priority_deinit(priority_cache);

  gnutls_global_deinit();
#endif
}

int ftpserv(int argc, char *argv[])
{
  char		*cfg = NULL, *textbuf = NULL;
  int			c;
  uint32	bufsize = 65536;
  thread_t	thid[1] = { 0 };
#ifndef _DEBUG
  if (argc > 1)
    cfg = InitConfig(argv[1]);
  else
    cfg = InitConfig(CONFIG_FILE_NAME);
#else
  cfg = InitConfig("D:/code/git/cstd/include/net/ftp/" CONFIG_FILE_NAME);
#endif

  if (cfg != NULL)
  {
    textbuf = (char*)malloc(bufsize);

    g_cfg.ConfigFile = cfg;

    g_cfg.BindToInterface = inet_addr("127.0.0.1");
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "interface", textbuf, bufsize))
      g_cfg.BindToInterface = inet_addr(textbuf);

    g_cfg.ExternalInterface = inet_addr("0.0.0.0");
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "external_ip", textbuf, bufsize))
      g_cfg.ExternalInterface = inet_addr(textbuf);

    g_cfg.LocalIPMask = inet_addr("255.255.255.0");
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "local_mask", textbuf, bufsize))
      g_cfg.LocalIPMask = inet_addr(textbuf);

    g_cfg.Port = DEFAULT_FTP_PORT;
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "port", textbuf, bufsize))
      g_cfg.Port = strtoul(textbuf, NULL, 10);

    g_cfg.MaxUsers = 1;
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "maxusers", textbuf, bufsize))
      g_cfg.MaxUsers = strtoul(textbuf, NULL, 10);

    g_cfg.PasvPortBase = 1024;
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "minport", textbuf, bufsize))
      g_cfg.PasvPortBase = strtoul(textbuf, NULL, 10);

    g_cfg.PasvPortMax = 65535;
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "maxport", textbuf, bufsize))
      g_cfg.PasvPortMax = strtoul(textbuf, NULL, 10);

    ParseConfig(cfg, CONFIG_SECTION_NAME, "CATrustFile", CAFILE, sizeof(CAFILE));
    ParseConfig(cfg, CONFIG_SECTION_NAME, "ServerCertificate", CERTFILE, sizeof(CERTFILE));
    ParseConfig(cfg, CONFIG_SECTION_NAME, "Keyfile", KEYFILE, sizeof(KEYFILE));
    ParseConfig(cfg, CONFIG_SECTION_NAME, "KeyfilePassword", KEYFILE_PASS, sizeof(KEYFILE_PASS));

    memset(textbuf, 0, bufsize);
    if (ParseConfig(cfg, CONFIG_SECTION_NAME, "logfilepath", textbuf, bufsize))
    {
      g_log = fopen(textbuf, "wb");
      if (g_log == NULL)
      {
        printf("Error: Failed to open/create log file. Please check logfilepath: %s\r\n", textbuf);
        printf("Possible errors: 1) path is invalid; 2) file is read only; 3) file is directory; 4) insufficient permissions\r\n");
      }

    }
    else {
      printf("WARNING: logfilepath section is not found in configuration. Logging to file disabled.\r\n");
    }
    if (g_log != NULL)
      fseek(g_log, 0L, SEEK_END);

    printf("Log file        : %s\r\n", textbuf);
    getcwd(textbuf, bufsize);
    printf("Working dir     : %s\r\n", textbuf);

    if (argc > 1)
      printf("Config file     : %s\r\n", argv[1]);
    else
      printf("Config file     : %s/%s\r\n", textbuf, CONFIG_FILE_NAME);

    printf("Interface ipv4  : %u.%u.%u.%u\r\n",
      g_cfg.BindToInterface & 0xff,
      (g_cfg.BindToInterface >> 8) & 0xff,
      (g_cfg.BindToInterface >> 16) & 0xff,
      (g_cfg.BindToInterface >> 24) & 0xff);

    printf("Interface mask  : %u.%u.%u.%u\r\n",
      g_cfg.LocalIPMask & 0xff,
      (g_cfg.LocalIPMask >> 8) & 0xff,
      (g_cfg.LocalIPMask >> 16) & 0xff,
      (g_cfg.LocalIPMask >> 24) & 0xff);

    printf("External ipv4   : %u.%u.%u.%u\r\n",
      g_cfg.ExternalInterface & 0xff,
      (g_cfg.ExternalInterface >> 8) & 0xff,
      (g_cfg.ExternalInterface >> 16) & 0xff,
      (g_cfg.ExternalInterface >> 24) & 0xff);

    printf("Port            : %u\r\n", g_cfg.Port);
    printf("Max users       : %u\r\n", g_cfg.MaxUsers);
    printf("PASV port range : %u..%u\r\n", g_cfg.PasvPortBase, g_cfg.PasvPortMax);

    printf("\r\n TYPE q or Ctrl+C to terminate >\r\n");

    TLSInit();

#if 0
    if (!sys_thread_create(thid, NULL, (thread_callback)ftpmain, NULL))
    {
      printf("Error: Failed to create main server thread\r\n");
      break;
    }

    do {
      c = getc(stdin);
      sleep(1);
    } while ((c != 'q') && (c != 'Q'));
#else
    ftpmain(NULL);
#endif
  }

  if (cfg == NULL)
    printf("Could not find configuration file\r\n\r\n Usage: fftp [CONFIGFILE]\r\n\r\n");

  if (g_log != NULL)
    fclose(g_log);

  if (cfg != NULL)
    free(cfg);

  if (textbuf != NULL)
    free(textbuf);

  TLSCleanup();

  exit(2);
  return 0;
}


