
#ifndef __CLIENT_COMMON_H
#define __CLIENT_COMMON_H

#include <map>
#include <string>
#include "CPMServer.h"
#include "CriticalSection.h"
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#ifdef WIN32
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}
#define MILLISEC       1
#else
#include <sys/socket.h>
#include <unistd.h>
#define SLEEP(Milliseconds)  {struct timeval tval;tval.tv_sec= 0;tval.tv_usec=(Milliseconds); select( 0 , NULL , NULL , NULL , &tval ); }
#define MILLISEC       1000
#endif

using namespace std;

#define SERVER_PORT 13333

#define BINDE_TIMES 20
#define SEND_TIMEOUT 1


#define SEND_BUF (64*1024)
#define RECV_BUF (64*1024)
#endif
