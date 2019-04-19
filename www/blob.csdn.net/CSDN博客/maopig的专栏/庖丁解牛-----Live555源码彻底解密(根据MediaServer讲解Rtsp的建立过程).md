# 庖丁解牛-----Live555源码彻底解密(根据MediaServer讲解Rtsp的建立过程) - maopig的专栏 - CSDN博客
2015年10月14日 18:03:32[maopig](https://me.csdn.net/maopig)阅读数：8787
live555MediaServer.cpp服务端源码讲解(testRelay.cpp,[http://blog.csdn.net/smilestone_322/article/details/18923139](http://blog.csdn.net/smilestone_322/article/details/18923139))
int main(int argc, char** argv) {
     // Begin by setting up our usage environment:
     TaskScheduler* scheduler = BasicTaskScheduler::createNew();
     UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
     UserAuthenticationDatabase* authDB = NULL;
// Create the RTSP server.  Try first with the default port number (554),
// and then with the alternative port number (8554):
     RTSPServer* rtspServer;
     portNumBits rtspServerPortNum = 554;
//先使用554默认端口建立Rtsp Server
     rtspServer = DynamicRTSPServer::createNew(*env, rtspServerPortNum, authDB);
//如果建立不成功，使用8554建立rtsp server
     if (rtspServer == NULL) {
         rtspServerPortNum = 8554;
         rtspServer = DynamicRTSPServer::createNew(*env, rtspServerPortNum, authDB);
     }
     if (rtspServer == NULL) {
         *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
         // exit(1);
         return -1;
     }
     env->taskScheduler().doEventLoop(); // does not return
     return 0; // only to prevent compiler warning
}
跟踪进入CreateNew函数;
DynamicRTSPServer*
DynamicRTSPServer::createNew(UsageEnvironment&env,PortourPort,
UserAuthenticationDatabase*authDatabase,
unsigned
reclamationTestSeconds) {
int
ourSocket = setUpOurSocket(env,ourPort);//建立tcp socket
if (ourSocket == -1)returnNULL;
return
newDynamicRTSPServer(env,ourSocket,ourPort,authDatabase,reclamationTestSeconds);
}
DynamicRTSPServer::DynamicRTSPServer(UsageEnvironment&env,intourSocket,
Port
ourPort,
UserAuthenticationDatabase*authDatabase,unsignedreclamationTestSeconds)
  : RTSPServerSupportingHTTPStreaming(env,ourSocket,ourPort,authDatabase,reclamationTestSeconds)
 {
}
首先建立socket，然后在调用DynamicRtspServer的构造函数，DynamicRtspServer继承RTSPServerSupportingHTTPStreaming类; RTSPServerSupportingHTTPStreaming类又继承RTSPServer类；
RTSPServerSupportingHTTPStreaming类的主要作用是支持Http；
接着看setUpOurSocket函数在前面已经讲过；就是建立socket；最后我们跟踪进入RTSPServer类的构造函数：
RTSPServer::RTSPServer(UsageEnvironment& env,
int ourSocket, Port ourPort,
                UserAuthenticationDatabase* authDatabase,
unsigned reclamationTestSeconds)
  : Medium(env),
    fRTSPServerPort(ourPort), fRTSPServerSocket(ourSocket), fHTTPServerSocket(-1), fHTTPServerPort(0),
    fServerMediaSessions(HashTable::create(STRING_HASH_KEYS)),
    fClientConnections(HashTable::create(ONE_WORD_HASH_KEYS)),
    fClientConnectionsForHTTPTunneling(NULL), 
// will get created if needed
    fClientSessions(HashTable::create(STRING_HASH_KEYS)),
    fPendingRegisterRequests(HashTable::create(ONE_WORD_HASH_KEYS)),
    fAuthDB(authDatabase), fReclamationTestSeconds(reclamationTestSeconds) {
  ignoreSigPipeOnSocket(ourSocket); // so that clients on the same host that are killed don't also kill us
// Arrange to handle connections from others:
  env.taskScheduler().turnOnBackgroundReadHandling(fRTSPServerSocket,
                               (TaskScheduler::BackgroundHandlerProc*)&incomingConnectionHandlerRTSP,this);
}
当fRTSPServerSocket收到数据时，调用incomingConnectionHandlerRTSP回调函数，继续跟进到incomingConnectionHandlerRTSP函数，源码如下：
void RTSPServer::incomingConnectionHandlerRTSP(void* instance,int/*mask*/) {
  RTSPServer* server = (RTSPServer*)instance;
  server->incomingConnectionHandlerRTSP1();
}
void RTSPServer::incomingConnectionHandler(int serverSocket) {
struct sockaddr_in clientAddr;
  SOCKLEN_T clientAddrLen = sizeof clientAddr;
int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
if (clientSocket < 0) {
int err = envir().getErrno();
if (err != EWOULDBLOCK) {
        envir().setResultErrMsg("accept() failed: ");
    }
return;
  }
  makeSocketNonBlocking(clientSocket);
  increaseSendBufferTo(envir(), clientSocket, 50*1024);
#ifdef DEBUG
envir() << "accept()ed connection from " << AddressString(clientAddr).val() << "\n";
#endif
// Create a new object for handling this RTSP connection:
  (void)createNewClientConnection(clientSocket, clientAddr);
}
当收到客户的连接时需保存下代表客户端的新socket，以后用这个socket与这个客户通讯。每个客户将来会对应一个rtp会话，而且各客户的RTSP请求只控制自己的rtp会话；
incomingConnectionHandler函数的作用是accept接受客户端的socket连接,然后设置clientSocket的属性,这里需要注意,我们在建立服务端socket时已经对服务端socket设置了非阻塞属性,这个地方又要设置accept后的clientSecket的属性;
incomingConnectionHandler函数最后调用createNewClientConnection函数，源码如下：
RTSPServer::RTSPClientConnection*
RTSPServer::createNewClientConnection(int clientSocket,struct sockaddr_in clientAddr) {
return
new RTSPClientConnection(*this, clientSocket, clientAddr);
}
对于每个新建立的客户端连接请求,new RTSPClientConnection的对象进行管理;
RTSPServer::RTSPClientConnection
::RTSPClientConnection(RTSPServer& ourServer, 
int clientSocket, struct sockaddr_in clientAddr)
  : fOurServer(ourServer), fIsActive(True),
    fClientInputSocket(clientSocket), fClientOutputSocket(clientSocket), fClientAddr(clientAddr),
    fRecursionCount(0), fOurSessionCookie(NULL) {
// Add ourself to our 'client connections' table:
  fOurServer.fClientConnections->Add((charconst*)this,this);
// Arrange to handle incoming requests:
  resetRequestBuffer();
  envir().taskScheduler().setBackgroundHandling(fClientInputSocket, SOCKET_READABLE|SOCKET_EXCEPTION,
                            (TaskScheduler::BackgroundHandlerProc*)&incomingRequestHandler,this);
}
在该函数中首先对RTSPServer的成员变量进行赋值：
fOurServer= ourServer;
fClientInputSocket= clientSocket;
fClientOutputSocket= clientSocket;
fClientAddr= clientAddr;
setBackgroundHandling函数用来处理fClientInputSocket socket上收到数据，或异常时，调用incomingRequestHandler回调函数;
下面在跟进到incomingRequestHandler函数:
void RTSPServer::RTSPClientConnection::incomingRequestHandler(void* instance,int/*mask*/) {
  RTSPClientConnection* session = (RTSPClientConnection*)instance;
  session->incomingRequestHandler1();
}
Session 为刚才new的RTSPClientConnection 对象，这个地方需要调试验证下;调用成员函数incomingRequestHandler1;跟进到该成员函数的代码:
void RTSPServer::RTSPClientConnection::incomingRequestHandler1() {
struct sockaddr_in dummy; 
// 'from' address, meaningless in this case
int bytesRead = readSocket(envir(), fClientInputSocket, &fRequestBuffer[fRequestBytesAlreadySeen], fRequestBufferBytesLeft, dummy);
  handleRequestBytes(bytesRead);
}
该函数调用ReadSocket从fClientInputSocket上读取数据；读到的数据保存在fRequestBuffer中，readSocket的返回值为实际读到的数据的长度；源码如下：
int readSocket(UsageEnvironment& env,
int socket, 
unsignedchar* buffer, 
unsigned bufferSize,
struct sockaddr_in& fromAddress) {
  SOCKLEN_T addressSize = sizeof fromAddress;
int bytesRead = recvfrom(socket, (char*)buffer, bufferSize, 0,
                 (struct sockaddr*)&fromAddress,
                 &addressSize);
if (bytesRead < 0) {
//##### HACK to work around bugs in Linux and Windows:
int err = env.getErrno();
if (err == 111 
/*ECONNREFUSED (Linux)*/
#ifdefined(__WIN32__) ||defined(_WIN32)
// What a piece of crap Windows is. Sometimes
// recvfrom() returns -1, but with an 'errno' of 0.
// This appears not to be a real error; just treat
// it as if it were a read of zero bytes, and hope
// we don't have to do anything else to 'reset'
// this alleged error:
     || err == 0 || err == EWOULDBLOCK
#else
|| err == EAGAIN
#endif
     || err == 113 /*EHOSTUNREACH (Linux)*/) {// Why does Linux return this for datagram sock?
      fromAddress.sin_addr.s_addr = 0;
return 0;
    }
//##### END HACK
    socketErr(env, "recvfrom() error: ");
  } else
if (bytesRead == 0) {
// "recvfrom()" on a stream socket can return 0 if the remote end has closed the connection. Treat this as an error:
return -1;
  }
return bytesRead;
}
从socket中读到数据后必须对数据进行解析，解析的源码如下：
void RTSPServer::RTSPClientConnection::handleRequestBytes(int newBytesRead) {
int numBytesRemaining = 0;
  ++fRecursionCount;
do {
    RTSPServer::RTSPClientSession* clientSession = NULL;
if (newBytesRead < 0 || (unsigned)newBytesRead >= fRequestBufferBytesLeft) {
// Either the client socket has died, or the request was too big for us.
// Terminate this connection:
#ifdef DEBUG
fprintf(stderr, "RTSPClientConnection[%p]::handleRequestBytes() read %d new bytes (of %d); terminating connection!\n", this, newBytesRead, fRequestBufferBytesLeft);
#endif
      fIsActive = False;
break;
    }
    Boolean endOfMsg = False;
unsigned
char* ptr = &fRequestBuffer[fRequestBytesAlreadySeen];
#ifdef DEBUG
ptr[newBytesRead] = '\0';
fprintf(stderr, "RTSPClientConnection[%p]::handleRequestBytes() %s %d new bytes:%s\n",
    this, numBytesRemaining > 0 ? "processing" : "read", newBytesRead, ptr);
#endif
if (fClientOutputSocket != fClientInputSocket) {
// We're doing RTSP-over-HTTP tunneling, and input commands are assumed to have been Base64-encoded.
// We therefore Base64-decode as much of this new data as we can (i.e., up to a multiple of 4 bytes).
// But first, we remove any whitespace that may be in the input data:
unsigned toIndex = 0;
for (int fromIndex = 0; fromIndex < newBytesRead; ++fromIndex) {
char c = ptr[fromIndex];
if (!(c == 
' ' || c == '\t' || c == 
'\r' || c == '\n')) { 
// not 'whitespace': space,tab,CR,NL
       ptr[toIndex++] = c;
     }
      }
      newBytesRead = toIndex;
unsigned numBytesToDecode = fBase64RemainderCount + newBytesRead;
unsigned newBase64RemainderCount = numBytesToDecode%4;
      numBytesToDecode -= newBase64RemainderCount;
if (numBytesToDecode > 0) {
     ptr[newBytesRead] = '\0';
unsigned decodedSize;
unsigned
char* decodedBytes = base64Decode((char
const*)(ptr-fBase64RemainderCount), numBytesToDecode, decodedSize);
#ifdef DEBUG
fprintf(stderr, "Base64-decoded %d input bytes into %d new bytes:", numBytesToDecode, decodedSize);
for (unsigned k = 0; k < decodedSize; ++k) fprintf(stderr, "%c", decodedBytes[k]);
fprintf(stderr, "\n");
#endif
// Copy the new decoded bytes in place of the old ones (we can do this because there are fewer decoded bytes than original):
unsigned
char* to = ptr-fBase64RemainderCount;
for (unsigned i = 0; i < decodedSize; ++i) *to++ = decodedBytes[i];
// Then copy any remaining (undecoded) bytes to the end:
for (unsigned j = 0; j < newBase64RemainderCount; ++j) *to++ = (ptr-fBase64RemainderCount+numBytesToDecode)[j];
     newBytesRead = decodedSize + newBase64RemainderCount; 
// adjust to allow for the size of the new decoded data (+ remainder)
delete[] decodedBytes;
      }
      fBase64RemainderCount = newBase64RemainderCount;
if (fBase64RemainderCount > 0)break;// because we know that we have more input bytes still to receive
    }
// Look for the end of the message: <CR><LF><CR><LF>
unsigned
char *tmpPtr = fLastCRLF + 2;
if (tmpPtr < fRequestBuffer) tmpPtr = fRequestBuffer;
while (tmpPtr < &ptr[newBytesRead-1]) {
if (*tmpPtr == 
'\r' && *(tmpPtr+1) == '\n') {
if (tmpPtr - fLastCRLF == 2) {// This is it:
       endOfMsg = True;
break;
     }
     fLastCRLF = tmpPtr;
      }
      ++tmpPtr;
    }
    fRequestBufferBytesLeft -= newBytesRead;
    fRequestBytesAlreadySeen += newBytesRead;
if (!endOfMsg) 
break; // subsequent reads will be needed to complete the request
// Parse the request string into command name and 'CSeq', then handle the command:
    fRequestBuffer[fRequestBytesAlreadySeen] = 
'\0';
char cmdName[RTSP_PARAM_STRING_MAX];
char urlPreSuffix[RTSP_PARAM_STRING_MAX];
char urlSuffix[RTSP_PARAM_STRING_MAX];
char cseq[RTSP_PARAM_STRING_MAX];
char sessionIdStr[RTSP_PARAM_STRING_MAX];
unsigned contentLength = 0;
fLastCRLF[2] = '\0'; 
// temporarily, for parsing
//解析Rtsp请求字符串
    Boolean parseSucceeded = parseRTSPRequestString((char*)fRequestBuffer, fLastCRLF+2 - fRequestBuffer,
                                cmdName, 
sizeof cmdName,
                                urlPreSuffix, 
sizeof urlPreSuffix,
                                urlSuffix, 
sizeof urlSuffix,
                                cseq, sizeof cseq,
                                sessionIdStr, 
sizeof sessionIdStr,
                                contentLength);
    fLastCRLF[2] = '\r'; 
// restore its value
if (parseSucceeded) {
#ifdef DEBUG
fprintf(stderr, "parseRTSPRequestString() succeeded, returning cmdName \"%s\", urlPreSuffix \"%s\", urlSuffix \"%s\", CSeq \"%s\", Content-Length %u, with %d bytes following the message.\n", cmdName, urlPreSuffix,
 urlSuffix, cseq, contentLength, ptr + newBytesRead - (tmpPtr + 2));
#endif
// If there was a "Content-Length:" header, then make sure we've received all of the data that it specified:
if (ptr + newBytesRead < tmpPtr + 2 + contentLength)break;// we still need more data; subsequent reads will give it to us
// We now have a complete RTSP request.
// Handle the specified command (beginning by checking those that don't require session ids):
      fCurrentCSeq = cseq;
//收到客户端的OPTIONS请求
if (strcmp(cmdName, 
"OPTIONS") == 0) {
// If the request included a "Session:" id, and it refers to a client session that's current ongoing, then use this
// command to indicate 'liveness' on that client session:
if (sessionIdStr[0] != 
'\0') {
       clientSession = (RTSPServer::RTSPClientSession*)(fOurServer.fClientSessions->Lookup(sessionIdStr));
//根据sessionIdStr查表，看该客户端的会话是否存在,存在会话，调用noteLiveness函数
if (clientSession != NULL) clientSession->noteLiveness();
     }
//处理Opinion请求,构建应答包
     handleCmd_OPTIONS();
      } else
if (urlPreSuffix[0] == '\0' && urlSuffix[0] =='*' && urlSuffix[1] =='\0') {
// The special "*" URL means: an operation on the entire server. This works only for GET_PARAMETER and SET_PARAMETER:
if (strcmp(cmdName, 
"GET_PARAMETER") == 0) {
       handleCmd_GET_PARAMETER((charconst*)fRequestBuffer);
     } else
if (strcmp(cmdName, "SET_PARAMETER") == 0) {
       handleCmd_SET_PARAMETER((charconst*)fRequestBuffer);
     } else {
       handleCmd_notSupported();
     }
      } else
if (strcmp(cmdName, "DESCRIBE") == 0) {
//收到客户端的Describe请求,处理该请求，构建应答包
     handleCmd_DESCRIBE(urlPreSuffix, urlSuffix, (charconst*)fRequestBuffer);
      } else
if (strcmp(cmdName, "SETUP") == 0) {
//收到客户端的Setup请求,如果是第一次Setup，那么就需要调用createNewClientSession函数进行会话，然后将sessionIdStr和clientSession关联起来
if (sessionIdStr[0] == 
'\0') {
// No session id was present in the request. So create a new "RTSPClientSession" object for this request.
// Choose a random (unused) 32-bit integer for the session id (it will be encoded as a 8-digit hex number).
// (We avoid choosing session id 0, because that has a special use (by "OnDemandServerMediaSubsession").)
       u_int32_t sessionId;
do {
         sessionId = (u_int32_t)our_random32();
         sprintf(sessionIdStr, "%08X", sessionId);
       } while (sessionId == 0 || fOurServer.fClientSessions->Lookup(sessionIdStr) != NULL);
       clientSession = fOurServer.createNewClientSession(sessionId);
       fOurServer.fClientSessions->Add(sessionIdStr, clientSession);
     } else {
// The request included a session id. Make sure it's one that we have already set up:
  //如果存在会话，直接查找原来的会话；
       clientSession = (RTSPServer::RTSPClientSession*)(fOurServer.fClientSessions->Lookup(sessionIdStr));
if (clientSession == NULL) {
         handleCmd_sessionNotFound();
       }
     }
//构建Setup应答包
if (clientSession != NULL) clientSession->handleCmd_SETUP(this, urlPreSuffix, urlSuffix, (charconst*)fRequestBuffer);
      } else
if (strcmp(cmdName, "TEARDOWN") == 0
          || strcmp(cmdName, "PLAY") == 0
          || strcmp(cmdName, "PAUSE") == 0
          || strcmp(cmdName, "GET_PARAMETER") == 0
          || strcmp(cmdName, "SET_PARAMETER") == 0) {
     RTSPServer::RTSPClientSession* clientSession
       = sessionIdStr[0] == '\0' ? NULL : (RTSPServer::RTSPClientSession*)(fOurServer.fClientSessions->Lookup(sessionIdStr));
if (clientSession == NULL) {
       handleCmd_sessionNotFound();
     } else {
       clientSession->handleCmd_withinSession(this, cmdName, urlPreSuffix, urlSuffix, (charconst*)fRequestBuffer);
     }
      } else
if (strcmp(cmdName, "REGISTER") == 0 || strcmp(cmdName,"REGISTER_REMOTE") == 0) {
// Because - unlike other commands - an implementation of these commands needs the entire URL, we re-parse the
// command to get it:
char* url = strDupSize((char*)fRequestBuffer);
if (sscanf((char*)fRequestBuffer,"%*s %s", url) == 1) {
       handleCmd_REGISTER(url, urlSuffix, strcmp(cmdName, 
"REGISTER_REMOTE") == 0);
     } else {
       handleCmd_bad();
     }
delete[] url;
      } else {
// The command is one that we don't handle:
     handleCmd_notSupported();
      }
    } else {
#ifdef DEBUG
fprintf(stderr, "parseRTSPRequestString() failed; checking now for HTTP commands (for RTSP-over-HTTP tunneling)...\n");
#endif
// The request was not (valid) RTSP, but check for a special case: HTTP commands (for setting up RTSP-over-HTTP tunneling):
char sessionCookie[RTSP_PARAM_STRING_MAX];
char acceptStr[RTSP_PARAM_STRING_MAX];
      *fLastCRLF = '\0'; 
// temporarily, for parsing
      parseSucceeded = parseHTTPRequestString(cmdName, 
sizeof cmdName,
                             urlSuffix, 
sizeof urlPreSuffix,
                             sessionCookie, 
sizeof sessionCookie,
                             acceptStr, 
sizeof acceptStr);
      *fLastCRLF = '\r';
if (parseSucceeded) {
#ifdef DEBUG
fprintf(stderr, "parseHTTPRequestString() succeeded, returning cmdName \"%s\", urlSuffix \"%s\", sessionCookie \"%s\", acceptStr \"%s\"\n", cmdName, urlSuffix, sessionCookie, acceptStr);
#endif
// Check that the HTTP command is valid for RTSP-over-HTTP tunneling: There must be a 'session cookie'.
     Boolean isValidHTTPCmd = True;
if (sessionCookie[0] == 
'\0') {
// There was no "x-sessioncookie:" header. If there was an "Accept: application/x-rtsp-tunnelled" header,
// then this is a bad tunneling request. Otherwise, assume that it's an attempt to access the stream via HTTP.
if (strcmp(acceptStr, 
"application/x-rtsp-tunnelled") == 0) {
         isValidHTTPCmd = False;
       } else {
         handleHTTPCmd_StreamingGET(urlSuffix, (charconst*)fRequestBuffer);
       }
     } else
if (strcmp(cmdName, "GET") == 0) {
       handleHTTPCmd_TunnelingGET(sessionCookie);
     } else
if (strcmp(cmdName, "POST") == 0) {
// We might have received additional data following the HTTP "POST" command - i.e., the first Base64-encoded RTSP command.
// Check for this, and handle it if it exists:
unsigned
charconst* extraData = fLastCRLF+4;
unsigned extraDataSize = &fRequestBuffer[fRequestBytesAlreadySeen] - extraData;
if (handleHTTPCmd_TunnelingPOST(sessionCookie, extraData, extraDataSize)) {
// We don't respond to the "POST" command, and we go away:
         fIsActive = False;
break;
       }
     } else {
       isValidHTTPCmd = False;
     }
if (!isValidHTTPCmd) {
       handleHTTPCmd_notSupported();
     }
      } else {
#ifdef DEBUG
fprintf(stderr, "parseHTTPRequestString() failed!\n");
#endif
     handleCmd_bad();
      }
    }
#ifdef DEBUG
fprintf(stderr, "sending response: %s", fResponseBuffer);
#endif
//发送应答包
    send(fClientOutputSocket, (charconst*)fResponseBuffer, strlen((char*)fResponseBuffer), 0);
if (clientSession != NULL && clientSession->fStreamAfterSETUP && strcmp(cmdName,"SETUP") == 0) {
// The client has asked for streaming to commence now, rather than after a
// subsequent "PLAY" command.  
So, simulate the effect of a "PLAY" command:
      clientSession->handleCmd_withinSession(this,"PLAY", urlPreSuffix, urlSuffix, (charconst*)fRequestBuffer);
    }
// Check whether there are extra bytes remaining in the buffer, after the end of the request (a rare case).
// If so, move them to the front of our buffer, and keep processing it, because it might be a following, pipelined request.
unsigned requestSize = (fLastCRLF+4-fRequestBuffer) + contentLength;
    numBytesRemaining = fRequestBytesAlreadySeen - requestSize;
    resetRequestBuffer(); // to prepare for any subsequent request
if (numBytesRemaining > 0) {
      memmove(fRequestBuffer, &fRequestBuffer[requestSize], numBytesRemaining);
      newBytesRead = numBytesRemaining;
    }
  } while (numBytesRemaining > 0);
  --fRecursionCount;
if (!fIsActive) {
if (fRecursionCount > 0) closeSockets();elsedeletethis;
// Note: The "fRecursionCount" test is for a pathological situation where we reenter the event loop and get called recursively
// while handling a command (e.g., while handling a "DESCRIBE", to get a SDP description).
// In such a case we don't want to actually delete ourself until we leave the outermost call.
  }
}
void RTSPServer::RTSPClientSession::noteLiveness() {
if (fOurServer.fReclamationTestSeconds > 0) {
    envir().taskScheduler()
      .rescheduleDelayedTask(fLivenessCheckTask,
                   fOurServer.fReclamationTestSeconds*1000000,
                   (TaskFunc*)livenessTimeoutTask, 
this);
  }
}
noteLiveness该函数可以用来判断流是不是断开；这个相当重要，我们可以使用它判断网络是否断开，尤其在客户端可以使用这样的方法来判断网络是否断开，然后实现断网重连的功能。
RTSPClientSession要提供什么功能呢,可以想象：需要监听客户端的rtsp请求并回应它，需要在DESCRIBE请求中返回所请求的流的信息，需要在SETUP请求中建立起RTP会话，需要在TEARDOWN请求中关闭RTP会话，等等;
下面在接着跟进到createNewClientSession会话的函数:
RTSPServer::RTSPClientSession*
RTSPServer::createNewClientSession(u_int32_t sessionId) {
return
new RTSPClientSession(*this, sessionId);
}
RTSPServer::RTSPClientSession
::RTSPClientSession(RTSPServer& ourServer, u_int32_t sessionId)
  : fOurServer(ourServer), fOurSessionId(sessionId), fOurServerMediaSession(NULL), fIsMulticast(False), fStreamAfterSETUP(False),
    fTCPStreamIdCount(0), fLivenessCheckTask(NULL), fNumStreamStates(0), fStreamStates(NULL) {
  noteLiveness();
}
这个构造函数旧版本的live555和v0.78版本是不同的，旧版本的live555，在accept后就建立了rtsp会话，而新版本的是在收到setup请求后才建立的会话，所以这些地方都不同，在旧版本中RTSPClientSession会有一个回调函数，新版本中没有，该回调函数在收到客户端的Connect命令时设置；
下面在分析下服务端对Opinion各种命令的请求的处理的代码；首先还是分析Opinion，该命令请求的作用是客户端请求服务端支持哪些命令；Describe请求是得到会话描述信息，包括h264的sps，pps信息也可以在Describe的应答中发送；Setup命令是用来建立会话，服务端收到Setup请求后，建立会话，new 一个RTSPClientSession对象，该对象用来处理客户端的各种Rtsp命令请求；同时服务端保存会话Id和会话对象，每次可以从表中取出RTSPClientSession对象；响应客户端的请求；在收到Setup命令后;没有等到客户端的Play命令，就开始视频流；
if (clientSession != NULL && clientSession->fStreamAfterSETUP && strcmp(cmdName,"SETUP") == 0) {
// The client has asked for streaming to commence now, rather than after a
// subsequent "PLAY" command.  
So, simulate the effect of a "PLAY" command:
      clientSession->handleCmd_withinSession(this,"PLAY", urlPreSuffix, urlSuffix, (charconst*)fRequestBuffer);
    }
1)服务端对Opinion命令的处理;跟踪源码:
void RTSPServer::RTSPClientConnection::handleCmd_OPTIONS() {
  snprintf((char*)fResponseBuffer,sizeof fResponseBuffer,
"RTSP/1.0 200 OK\r\nCSeq: %s\r\n%sPublic: %s\r\n\r\n",
        fCurrentCSeq, dateHeader(), fOurServer.allowedCommandNames());
}
1)      服务端对Describe命令的处理
void RTSPServer::RTSPClientConnection
::handleCmd_DESCRIBE(char
const* urlPreSuffix, char
const* urlSuffix, char
const* fullRequestStr) {
char* sdpDescription = NULL;
char* rtspURL = NULL;
do {
//整理一下下RTSP地址
char urlTotalSuffix[RTSP_PARAM_STRING_MAX];
if (strlen(urlPreSuffix) + strlen(urlSuffix) + 2 >sizeof urlTotalSuffix) {
      handleCmd_bad();
break;
    }
    urlTotalSuffix[0] = '\0';
if (urlPreSuffix[0] != 
'\0') {
      strcat(urlTotalSuffix, urlPreSuffix);
      strcat(urlTotalSuffix, "/");
    }
    strcat(urlTotalSuffix, urlSuffix);
//鉴权
if (!authenticationOK("DESCRIBE", urlTotalSuffix, fullRequestStr))break;
// We should really check that the request contains an "Accept:" #####
// for "application/sdp", because that's what we're sending back #####
// Begin by looking up the "ServerMediaSession" object for the specified "urlTotalSuffix":
//跟据流的名字查找ServerMediaSession
    ServerMediaSession* session = fOurServer.lookupServerMediaSession(urlTotalSuffix);
if (session == NULL) {
      handleCmd_notFound();
break;
    }
// Then, assemble a SDP description for this session:
    sdpDescription = session->generateSDPDescription();
if (sdpDescription == NULL) {
// This usually means that a file name that was specified for a
// "ServerMediaSubsession" does not exist.
      setRTSPResponse("404 File Not Found, Or In Incorrect Format");
break;
    }
unsigned sdpDescriptionSize = strlen(sdpDescription);
// Also, generate our RTSP URL, for the "Content-Base:" header
// (which is necessary to ensure that the correct URL gets used in subsequent "SETUP" requests).
    rtspURL = fOurServer.rtspURL(session, fClientInputSocket);
    snprintf((char*)fResponseBuffer,sizeof fResponseBuffer,
"RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
"%s"
"Content-Base: %s/\r\n"
"Content-Type: application/sdp\r\n"
"Content-Length: %d\r\n\r\n"
"%s",
          fCurrentCSeq,
          dateHeader(),
          rtspURL,
          sdpDescriptionSize,
          sdpDescription);
  } while (0);
delete[] sdpDescription;
delete[] rtspURL;
}
ServerMediaSession*
DynamicRTSPServer::lookupServerMediaSession(charconst* streamName) {
// First, check whether the specified "streamName" exists as a local file:
  FILE* fid = fopen(streamName, "rb");
  Boolean fileExists = fid != NULL;
// Next, check whether we already have a "ServerMediaSession" for this file:
//查找是否已经存在一个ServerMediaSession
  ServerMediaSession* sms = RTSPServer::lookupServerMediaSession(streamName);
  Boolean smsExists = sms != NULL;
// Handle the four possibilities for "fileExists" and "smsExists":
if (!fileExists) {
//文件不存在
if (smsExists) {
// "sms" was created for a file that no longer exists. Remove it:
//删除ServerMediaSession
      removeServerMediaSession(sms);
    }
return NULL;
  } else {
if (!smsExists) {
// Create a new "ServerMediaSession" object for streaming from the named file.
//如果ServerMediaSession不存在,新建一个ServerMediaSession
      sms = createNewSMS(envir(), streamName, fid);
//将ServerMediaSession和会话关联起来
      addServerMediaSession(sms);
    }
    fclose(fid);
return sms;
  }
}
void RTSPServer::addServerMediaSession(ServerMediaSession* serverMediaSession) {
if (serverMediaSession == NULL)return;
charconst* sessionName = serverMediaSession->streamName();
if (sessionName == NULL) sessionName ="";
  removeServerMediaSession(sessionName); 
// in case an existing "ServerMediaSession" with this name already exists
  fServerMediaSessions->Add(sessionName, (void*)serverMediaSession);
}
2)      服务端对Setup命令的处理
void RTSPServer::RTSPClientSession
::handleCmd_SETUP(RTSPServer::RTSPClientConnection* ourClientConnection,
char
const* urlPreSuffix, char
const* urlSuffix, char
const* fullRequestStr) {
// Normally, "urlPreSuffix" should be the session (stream) name, and "urlSuffix" should be the subsession (track) name.
// However (being "liberal in what we accept"), we also handle 'aggregate' SETUP requests (i.e., without a track name),
// in the special case where we have only a single track. I.e., in this case, we also handle:
//    "urlPreSuffix" is empty and "urlSuffix" is the session (stream) name, or
//    "urlPreSuffix" concatenated with "urlSuffix" (with "/" inbetween) is the session (stream) name.
charconst* streamName = urlPreSuffix;// in the normal case
charconst* trackId = urlSuffix;// in the normal case
char* concatenatedStreamName = NULL;// in the normal case
  noteLiveness();
do {
// First, make sure the specified stream name exists:
//下面的注释参数参考：
http://blog.csdn.net/niu_gao/article/details/6911130
每个ServerMediaSession中至少要包含一个//ServerMediaSubsession。一个ServerMediaSession对应一个媒体，可以认为是Server上的一个文件，或一个实时获取设备。其包含的每个ServerMediaSubSession代表媒体中的一个Track。所以一个ServerMediaSession对应一个媒体，如果客户请求的媒体名相同，就使用已存在的ServerMediaSession，如果不同，就创建一个新的。一个流对应一个StreamState，StreamState与ServerMediaSubsession相关，但代表的是动态的，而ServerMediaSubsession代表静态的。
fOurServer.lookupServerMediaSession(streamName)中会在找不到同名ServerMediaSession时新建一个，代表一个RTP流的ServerMediaSession们是被RTSPServer管理的，而不是被RTSPClientSession拥有。为什么呢？因为ServerMediaSession代表的是一个静态的流，也就是可以从它里面获取一个流的各种信息，但不能获取传输状态。不同客户可能连接到同一个流，所以ServerMediaSession应被RTSPServer所拥有。
    ServerMediaSession* sms = fOurServer.lookupServerMediaSession(streamName);
if (sms == NULL) {
// Check for the special case (noted above), before we give up:
if (urlPreSuffix[0] == 
'\0') {
     streamName = urlSuffix;
      } else {
     concatenatedStreamName = newchar[strlen(urlPreSuffix) + strlen(urlSuffix) + 2];// allow for the "/" and the trailing '\0'
     sprintf(concatenatedStreamName, 
"%s/%s", urlPreSuffix, urlSuffix);
     streamName = concatenatedStreamName;
      }
      trackId = NULL;
// Check again:
      sms = fOurServer.lookupServerMediaSession(streamName);
    }
if (sms == NULL) {
if (fOurServerMediaSession == NULL) {
// The client asked for a stream that doesn't exist (and this session descriptor has not been used before):
     ourClientConnection->handleCmd_notFound();
      } else {
// The client asked for a stream that doesn't exist, but using a stream id for a stream that does exist. Bad request:
     ourClientConnection->handleCmd_bad();
      }
break;
    } else {
if (fOurServerMediaSession == NULL) {
// We're accessing the "ServerMediaSession" for the first time.
     fOurServerMediaSession = sms;
     fOurServerMediaSession->incrementReferenceCount();
      } else
if (sms != fOurServerMediaSession) {
// The client asked for a stream that's different from the one originally requested for this stream id. Bad request:
     ourClientConnection->handleCmd_bad();
break;
      }
    }
if (fStreamStates == NULL) {
// This is the first "SETUP" for this session. Set up our array of states for all of this session's subsessions (tracks):
      ServerMediaSubsessionIterator iter(*fOurServerMediaSession);
for (fNumStreamStates = 0; iter.next() != NULL; ++fNumStreamStates) {}// begin by counting the number of subsessions (tracks)
      fStreamStates = new
struct streamState[fNumStreamStates];
      iter.reset();
      ServerMediaSubsession* subsession;
for (unsigned i = 0; i < fNumStreamStates; ++i) {
     subsession = iter.next();
     fStreamStates[i].subsession = subsession;
     fStreamStates[i].streamToken = NULL; 
// for now; it may be changed by the "getStreamParameters()" call that comes later
      }
    }
// Look up information for the specified subsession (track):
    ServerMediaSubsession* subsession = NULL;
unsigned streamNum;
if (trackId != NULL && trackId[0] !='\0') {// normal case
for (streamNum = 0; streamNum < fNumStreamStates; ++streamNum) {
     subsession = fStreamStates[streamNum].subsession;
if (subsession != NULL && strcmp(trackId, subsession->trackId()) == 0)break;
      }
if (streamNum >= fNumStreamStates) {
// The specified track id doesn't exist, so this request fails:
     ourClientConnection->handleCmd_notFound();
break;
      }
    } else {
// Weird case: there was no track id in the URL.
// This works only if we have only one subsession:
if (fNumStreamStates != 1 || fStreamStates[0].subsession == NULL) {
     ourClientConnection->handleCmd_bad();
break;
      }
      streamNum = 0;
      subsession = fStreamStates[streamNum].subsession;
    }
// ASSERT: subsession != NULL
// Look for a "Transport:" header in the request string, to extract client parameters:
    StreamingMode streamingMode;
char* streamingModeString = NULL;// set when RAW_UDP streaming is specified
char* clientsDestinationAddressStr;
    u_int8_t clientsDestinationTTL;
    portNumBits clientRTPPortNum, clientRTCPPortNum;
unsigned
char rtpChannelId, rtcpChannelId;
    parseTransportHeader(fullRequestStr, streamingMode, streamingModeString,
               clientsDestinationAddressStr, clientsDestinationTTL,
               clientRTPPortNum, clientRTCPPortNum,
               rtpChannelId, rtcpChannelId);
if ((streamingMode == RTP_TCP && rtpChannelId == 0xFF) ||
     (streamingMode != RTP_TCP && ourClientConnection->fClientOutputSocket != ourClientConnection->fClientInputSocket)) {
// An anomolous situation, caused by a buggy client. Either:
//     1/ TCP streaming was requested, but with no "interleaving=" fields.  (QuickTime Player sometimes does this.), or
//     2/ TCP streaming was not requested, but we're doing RTSP-over-HTTP tunneling (which implies TCP streaming).
// In either case, we assume TCP streaming, and set the RTP and RTCP channel ids to proper values:
      streamingMode = RTP_TCP;
      rtpChannelId = fTCPStreamIdCount; rtcpChannelId = fTCPStreamIdCount+1;
    }
if (streamingMode == RTP_TCP) fTCPStreamIdCount += 2;
    Port clientRTPPort(clientRTPPortNum);
    Port clientRTCPPort(clientRTCPPortNum);
// Next, check whether a "Range:" or "x-playNow:" header is present in the request.
// This isn't legal, but some clients do this to combine "SETUP" and "PLAY":
double rangeStart = 0.0, rangeEnd = 0.0;
char* absStart = NULL; 
char* absEnd = NULL;
if (parseRangeHeader(fullRequestStr, rangeStart, rangeEnd, absStart, absEnd)) {
delete[] absStart; 
delete[] absEnd;
      fStreamAfterSETUP = True;
    } else
if (parsePlayNowHeader(fullRequestStr)) {
      fStreamAfterSETUP = True;
    } else {
      fStreamAfterSETUP = False;
    }
// Then, get server parameters from the 'subsession':
int tcpSocketNum = streamingMode == RTP_TCP ? ourClientConnection->fClientOutputSocket : -1;
    netAddressBits destinationAddress = 0;
    u_int8_t destinationTTL = 255;
#ifdef RTSP_ALLOW_CLIENT_DESTINATION_SETTING
if (clientsDestinationAddressStr != NULL) {
// Use the client-provided "destination" address.
// Note: This potentially allows the server to be used in denial-of-service
// attacks, so don't enable this code unless you're sure that clients are
// trusted.
destinationAddress = our_inet_addr(clientsDestinationAddressStr);
}
// Also use the client-provided TTL.
destinationTTL = clientsDestinationTTL;
#endif
delete[] clientsDestinationAddressStr;
    Port serverRTPPort(0);
    Port serverRTCPPort(0);
// Make sure that we transmit on the same interface that's used by the client (in case we're a multi-homed server):
struct sockaddr_in sourceAddr; SOCKLEN_T namelen =sizeof sourceAddr;
    getsockname(ourClientConnection->fClientInputSocket, (struct sockaddr*)&sourceAddr, &namelen);
    netAddressBits origSendingInterfaceAddr = SendingInterfaceAddr;
    netAddressBits origReceivingInterfaceAddr = ReceivingInterfaceAddr;
// NOTE: The following might not work properly, so we ifdef it out for now:
#ifdef HACK_FOR_MULTIHOMED_SERVERS
ReceivingInterfaceAddr = SendingInterfaceAddr = sourceAddr.sin_addr.s_addr;
#endif
    subsession->getStreamParameters(fOurSessionId, ourClientConnection->fClientAddr.sin_addr.s_addr,
                       clientRTPPort, clientRTCPPort,
                       tcpSocketNum, rtpChannelId, rtcpChannelId,
                       destinationAddress, destinationTTL, fIsMulticast,
                       serverRTPPort, serverRTCPPort,
                       fStreamStates[streamNum].streamToken);
    SendingInterfaceAddr = origSendingInterfaceAddr;
    ReceivingInterfaceAddr = origReceivingInterfaceAddr;
    AddressString destAddrStr(destinationAddress);
    AddressString sourceAddrStr(sourceAddr);
if (fIsMulticast) {
switch (streamingMode) {
case RTP_UDP:
       snprintf((char*)ourClientConnection->fResponseBuffer,sizeof ourClientConnection->fResponseBuffer,
"RTSP/1.0 200 OK\r\n"
"CSeq: %s\r\n"
"%s"
"Transport: RTP/AVP;multicast;destination=%s;source=%s;port=%d-%d;ttl=%d\r\n"
"Session: %08X\r\n\r\n",
            ourClientConnection->fCurrentCSeq,
            dateHeader(),
            destAddrStr.val(), sourceAddrStr.val(), ntohs(serverRTPPort.num()), ntohs(serverRTCPPort.num()), destinationTTL,
            fOurSessionId);
break;
case RTP_TCP:
// multicast streams can't be sent via TCP
       ourClientConnection->handleCmd_unsupportedTransport();
break;
case RAW_UDP:
       snprintf((char*)ourClientConnection->fResponseBuffer,sizeof ourClientConnection->fResponseBuffer,
"RTSP/1.0 200 OK\r\n"
"CSeq: %s\r\n"
"%s"
"Transport: %s;multicast;destination=%s;source=%s;port=%d;ttl=%d\r\n"
"Session: %08X\r\n\r\n",
            ourClientConnection->fCurrentCSeq,
            dateHeader(),
            streamingModeString, destAddrStr.val(), sourceAddrStr.val(), ntohs(serverRTPPort.num()), destinationTTL,
            fOurSessionId);
break;
      }
    } else {
switch (streamingMode) {
case RTP_UDP: {
       snprintf((char*)ourClientConnection->fResponseBuffer,sizeof ourClientConnection->fResponseBuffer,
"RTSP/1.0 200 OK\r\n"
"CSeq: %s\r\n"
"%s"
"Transport: RTP/AVP;unicast;destination=%s;source=%s;client_port=%d-%d;server_port=%d-%d\r\n"
"Session: %08X\r\n\r\n",
            ourClientConnection->fCurrentCSeq,
            dateHeader(),
            destAddrStr.val(), sourceAddrStr.val(), ntohs(clientRTPPort.num()), ntohs(clientRTCPPort.num()), ntohs(serverRTPPort.num()), ntohs(serverRTCPPort.num()),
            fOurSessionId);
break;
     }
case RTP_TCP: {
       snprintf((char*)ourClientConnection->fResponseBuffer,sizeof ourClientConnection->fResponseBuffer,
"RTSP/1.0 200 OK\r\n"
"CSeq: %s\r\n"
"%s"
"Transport: RTP/AVP/TCP;unicast;destination=%s;source=%s;interleaved=%d-%d\r\n"
"Session: %08X\r\n\r\n",
            ourClientConnection->fCurrentCSeq,
            dateHeader(),
            destAddrStr.val(), sourceAddrStr.val(), rtpChannelId, rtcpChannelId,
            fOurSessionId);
break;
     }
case RAW_UDP: {
       snprintf((char*)ourClientConnection->fResponseBuffer,sizeof ourClientConnection->fResponseBuffer,
"RTSP/1.0 200 OK\r\n"
"CSeq: %s\r\n"
"%s"
"Transport: %s;unicast;destination=%s;source=%s;client_port=%d;server_port=%d\r\n"
"Session: %08X\r\n\r\n",
            ourClientConnection->fCurrentCSeq,
            dateHeader(),
            streamingModeString, destAddrStr.val(), sourceAddrStr.val(), ntohs(clientRTPPort.num()), ntohs(serverRTPPort.num()),
            fOurSessionId);
break;
     }
      }
    }
delete[] streamingModeString;
  } while (0);
delete[] concatenatedStreamName;
}
//新建ServerMediaSession的源代码如下：
static ServerMediaSession* createNewSMS(UsageEnvironment& env,
char
const* fileName, FILE* /*fid*/) {
// Use the file name extension to determine the type of "ServerMediaSession":
charconst* extension = strrchr(fileName,'.');
if (extension == NULL) 
return NULL;
  ServerMediaSession* sms = NULL;
  Boolean const reuseSource = False;
if (strcmp(extension, 
".aac") == 0) {
// Assumed to be an AAC Audio (ADTS format) file:
    NEW_SMS("AAC Audio");
    sms->addSubsession(ADTSAudioFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else
if (strcmp(extension, ".amr") == 0) {
// Assumed to be an AMR Audio file:
    NEW_SMS("AMR Audio");
    sms->addSubsession(AMRAudioFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else
if (strcmp(extension, ".ac3") == 0) {
// Assumed to be an AC-3 Audio file:
    NEW_SMS("AC-3 Audio");
    sms->addSubsession(AC3AudioFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else
if (strcmp(extension, ".m4e") == 0) {
// Assumed to be a MPEG-4 Video Elementary Stream file:
    NEW_SMS("MPEG-4 Video");
    sms->addSubsession(MPEG4VideoFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else
if (strcmp(extension, ".264") == 0) {
// Assumed to be a H.264 Video Elementary Stream file:
    NEW_SMS("H.264 Video");
    OutPacketBuffer::maxSize = 100000; 
// allow for some possibly large H.264 frames
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else
if (strcmp(extension, ".mp3") == 0) {
// Assumed to be a MPEG-1 or 2 Audio file:
    NEW_SMS("MPEG-1 or 2 Audio");
// To stream using 'ADUs' rather than raw MP3 frames, uncomment the following:
//#define STREAM_USING_ADUS 1
// To also reorder ADUs before streaming, uncomment the following:
//#define INTERLEAVE_ADUS 1
// (For more information about ADUs and interleaving,
//  see <http://www.live555.com/rtp-mp3/>)
    Boolean useADUs = False;
    Interleaving* interleaving = NULL;
#ifdef STREAM_USING_ADUS
useADUs = True;
#ifdef INTERLEAVE_ADUS
unsigned char interleaveCycle[] = {0,2,1,3}; // or choose your own...
unsigned const interleaveCycleSize
= (sizeof interleaveCycle)/(sizeof (unsigned char));
interleaving = new Interleaving(interleaveCycleSize, interleaveCycle);
#endif
#endif
    sms->addSubsession(MP3AudioFileServerMediaSubsession::createNew(env, fileName, reuseSource, useADUs, interleaving));
  } else
if (strcmp(extension, ".mpg") == 0) {
// Assumed to be a MPEG-1 or 2 Program Stream (audio+video) file:
    NEW_SMS("MPEG-1 or 2 Program Stream");
    MPEG1or2FileServerDemux* demux
      = MPEG1or2FileServerDemux::createNew(env, fileName, reuseSource);
    sms->addSubsession(demux->newVideoServerMediaSubsession());
    sms->addSubsession(demux->newAudioServerMediaSubsession());
  } else
if (strcmp(extension, ".vob") == 0) {
// Assumed to be a VOB (MPEG-2 Program Stream, with AC-3 audio) file:
    NEW_SMS("VOB (MPEG-2 video with AC-3 audio)");
    MPEG1or2FileServerDemux* demux
      = MPEG1or2FileServerDemux::createNew(env, fileName, reuseSource);
    sms->addSubsession(demux->newVideoServerMediaSubsession());
    sms->addSubsession(demux->newAC3AudioServerMediaSubsession());
  } else
if (strcmp(extension, ".ts") == 0) {
// Assumed to be a MPEG Transport Stream file:
// Use an index file name that's the same as the TS file name, except with ".tsx":
unsigned indexFileNameLen = strlen(fileName) + 2;// allow for trailing "x\0"
char* indexFileName = 
newchar[indexFileNameLen];
    sprintf(indexFileName, "%sx", fileName);
    NEW_SMS("MPEG Transport Stream");
    sms->addSubsession(MPEG2TransportFileServerMediaSubsession::createNew(env, fileName, indexFileName, reuseSource));
delete[] indexFileName;
  } else
if (strcmp(extension, ".wav") == 0) {
// Assumed to be a WAV Audio file:
    NEW_SMS("WAV Audio Stream");
// To convert 16-bit PCM data to 8-bit u-law, prior to streaming,
// change the following to True:
    Boolean convertToULaw = False;
    sms->addSubsession(WAVAudioFileServerMediaSubsession::createNew(env, fileName, reuseSource, convertToULaw));
  } else
if (strcmp(extension, ".dv") == 0) {
// Assumed to be a DV Video file
// First, make sure that the RTPSinks' buffers will be large enough to handle the huge size of DV frames (as big as 288000).
    OutPacketBuffer::maxSize = 300000;
    NEW_SMS("DV Video");
    sms->addSubsession(DVVideoFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else
if (strcmp(extension, ".mkv") == 0 || strcmp(extension,".webm") == 0) {
// Assumed to be a Matroska file (note that WebM ('.webm') files are also Matroska files)
    NEW_SMS("Matroska video+audio+(optional)subtitles");
// Create a Matroska file server demultiplexor for the specified file. (We enter the event loop to wait for this to complete.)
    newMatroskaDemuxWatchVariable = 0;
    MatroskaFileServerDemux::createNew(env, fileName, onMatroskaDemuxCreation, NULL);
    env.taskScheduler().doEventLoop(&newMatroskaDemuxWatchVariable);
    ServerMediaSubsession* smss;
while ((smss = demux->newServerMediaSubsession()) != NULL) {
      sms->addSubsession(smss);
    }
  }
return sms;
}
3)      服务端对Play命令的处理
void RTSPServer::RTSPClientSession
::handleCmd_withinSession(RTSPServer::RTSPClientConnection* ourClientConnection,
char
const* cmdName,
char
const* urlPreSuffix, char
const* urlSuffix,
char
const* fullRequestStr) {
// This will either be:
// - a non-aggregated operation, if "urlPreSuffix" is the session (stream)
//   name and "urlSuffix" is the subsession (track) name, or
// - an aggregated operation, if "urlSuffix" is the session (stream) name,
//   or "urlPreSuffix" is the session (stream) name, and "urlSuffix" is empty,
//   or "urlPreSuffix" and "urlSuffix" are both nonempty, but when concatenated, (with "/") form the session (stream) name.
// Begin by figuring out which of these it is:
  ServerMediaSubsession* subsession;
  noteLiveness();
if (fOurServerMediaSession == NULL) {// There wasn't a previous SETUP!
    ourClientConnection->handleCmd_notSupported();
return;
  } else
if (urlSuffix[0] != '\0' && strcmp(fOurServerMediaSession->streamName(), urlPreSuffix) == 0) {
// Non-aggregated operation.
// Look up the media subsession whose track id is "urlSuffix":
    ServerMediaSubsessionIterator iter(*fOurServerMediaSession);
while ((subsession = iter.next()) != NULL) {
if (strcmp(subsession->trackId(), urlSuffix) == 0)break;// success
    }
if (subsession == NULL) { 
// no such track!
      ourClientConnection->handleCmd_notFound();
return;
    }
  } else
if (strcmp(fOurServerMediaSession->streamName(), urlSuffix) == 0 ||
          (urlSuffix[0] == '\0' && strcmp(fOurServerMediaSession->streamName(), urlPreSuffix) == 0)) {
// Aggregated operation
    subsession = NULL;
  } else
if (urlPreSuffix[0] != '\0' && urlSuffix[0] !='\0') {
// Aggregated operation, if <urlPreSuffix>/<urlSuffix> is the session (stream) name:
unsigned
const urlPreSuffixLen = strlen(urlPreSuffix);
if (strncmp(fOurServerMediaSession->streamName(), urlPreSuffix, urlPreSuffixLen) == 0 &&
     fOurServerMediaSession->streamName()[urlPreSuffixLen] == 
'/' &&
     strcmp(&(fOurServerMediaSession->streamName())[urlPreSuffixLen+1], urlSuffix) == 0) {
      subsession = NULL;
    } else {
      ourClientConnection->handleCmd_notFound();
return;
    }
  } else { 
// the request doesn't match a known stream and/or track at all!
    ourClientConnection->handleCmd_notFound();
return;
  }
if (strcmp(cmdName, 
"TEARDOWN") == 0) {
    handleCmd_TEARDOWN(ourClientConnection, subsession);
  } else
if (strcmp(cmdName, "PLAY") == 0) {
    handleCmd_PLAY(ourClientConnection, subsession, fullRequestStr);
  } else
if (strcmp(cmdName, "PAUSE") == 0) {
    handleCmd_PAUSE(ourClientConnection, subsession);
  } else
if (strcmp(cmdName, "GET_PARAMETER") == 0) {
    handleCmd_GET_PARAMETER(ourClientConnection, subsession, fullRequestStr);
  } else
if (strcmp(cmdName, "SET_PARAMETER") == 0) {
    handleCmd_SET_PARAMETER(ourClientConnection, subsession, fullRequestStr);
  }
}
void RTSPServer::RTSPClientSession
::handleCmd_PLAY(RTSPServer::RTSPClientConnection* ourClientConnection,
          ServerMediaSubsession* subsession, 
charconst* fullRequestStr) {
char* rtspURL = fOurServer.rtspURL(fOurServerMediaSession, ourClientConnection->fClientInputSocket);
unsigned rtspURLSize = strlen(rtspURL);
// Parse the client's "Scale:" header, if any:
float scale;
  Boolean sawScaleHeader = parseScaleHeader(fullRequestStr, scale);
// Try to set the stream's scale factor to this value:
if (subsession == NULL 
/*aggregate op*/) {
    fOurServerMediaSession->testScaleFactor(scale);
  } else {
    subsession->testScaleFactor(scale);
  }
char buf[100];
char* scaleHeader;
if (!sawScaleHeader) {
    buf[0] = '\0'; 
// Because we didn't see a Scale: header, don't send one back
  } else {
    sprintf(buf, "Scale: %f\r\n", scale);
  }
  scaleHeader = strDup(buf);
// Parse the client's "Range:" header, if any:
float duration = 0.0;
double rangeStart = 0.0, rangeEnd = 0.0;
char* absStart = NULL; 
char* absEnd = NULL;
  Boolean sawRangeHeader = parseRangeHeader(fullRequestStr, rangeStart, rangeEnd, absStart, absEnd);
if (sawRangeHeader && absStart == NULL/*not seeking by 'absolute' time*/) {
// Use this information, plus the stream's duration (if known), to create our own "Range:" header, for the response:
    duration = subsession == NULL /*aggregate op*/
      ? fOurServerMediaSession->duration() : subsession->duration();
if (duration < 0.0) {
// We're an aggregate PLAY, but the subsessions have different durations.
// Use the largest of these durations in our header
      duration = -duration;
    }
// Make sure that "rangeStart" and "rangeEnd" (from the client's "Range:" header) have sane values
// before we send back our own "Range:" header in our response:
if (rangeStart < 0.0) rangeStart = 0.0;
else
if (rangeStart > duration) rangeStart = duration;
if (rangeEnd < 0.0) rangeEnd = 0.0;
else
if (rangeEnd > duration) rangeEnd = duration;
if ((scale > 0.0 && rangeStart > rangeEnd && rangeEnd > 0.0) ||
     (scale < 0.0 && rangeStart < rangeEnd)) {
// "rangeStart" and "rangeEnd" were the wrong way around; swap them:
double tmp = rangeStart;
      rangeStart = rangeEnd;
      rangeEnd = tmp;
    }
  }
// Create a "RTP-Info:" line.  
It will get filled in from each subsession's state:
charconst* rtpInfoFmt =
"%s"
// "RTP-Info:", plus any preceding rtpInfo items
"%s"
// comma separator, if needed
"url=%s/%s"
";seq=%d"
";rtptime=%u"
    ;
unsigned rtpInfoFmtSize = strlen(rtpInfoFmt);
char* rtpInfo = strDup("RTP-Info: ");
unsigned i, numRTPInfoItems = 0;
// Do any required seeking/scaling on each subsession, before starting streaming.
// (However, we don't do this if the "PLAY" request was for just a single subsession of a multiple-subsession stream;
//  for such streams, seeking/scaling can be done only with an aggregate "PLAY".)
for (i = 0; i < fNumStreamStates; ++i) {
if (subsession == NULL 
/* means: aggregated operation */ || fNumStreamStates == 1) {
if (sawScaleHeader) {
if (fStreamStates[i].subsession != NULL) {
       fStreamStates[i].subsession->setStreamScale(fOurSessionId, fStreamStates[i].streamToken, scale);
     }
      }
if (sawRangeHeader) {
if (absStart != NULL) {
// Special case handling for seeking by 'absolute' time:
if (fStreamStates[i].subsession != NULL) {
         fStreamStates[i].subsession->seekStream(fOurSessionId, fStreamStates[i].streamToken, absStart, absEnd);
       }
     } else {
// Seeking by relative (NPT) time:
double streamDuration = 0.0;// by default; means: stream until the end of the media
if (rangeEnd > 0.0 && (rangeEnd+0.001) < duration) {// the 0.001 is because we limited the values to 3 decimal places
// We want the stream to end early. Set the duration we want:
         streamDuration = rangeEnd - rangeStart;
if (streamDuration < 0.0) streamDuration = -streamDuration;// should happen only if scale < 0.0
       }
if (fStreamStates[i].subsession != NULL) {
         u_int64_t numBytes;
//查找流
         fStreamStates[i].subsession->seekStream(fOurSessionId, fStreamStates[i].streamToken,
                                rangeStart, streamDuration, numBytes);
       }
     }
      } else {
// No "Range:" header was specified in the "PLAY", so we do a 'null' seek (i.e., we don't seek at all):
if (fStreamStates[i].subsession != NULL) {
       fStreamStates[i].subsession->nullSeekStream(fOurSessionId, fStreamStates[i].streamToken);
     }
      }
    }
  }
// Create the "Range:" header that we'll send back in our response.
// (Note that we do this after seeking, in case the seeking operation changed the range start time.)
char* rangeHeader;
if (!sawRangeHeader) {
// There wasn't a "Range:" header in the request, so, in our response, begin the range with the current NPT (normal play time):
float curNPT = 0.0;
for (i = 0; i < fNumStreamStates; ++i) {
if (subsession == NULL 
/* means: aggregated operation */
       || subsession == fStreamStates[i].subsession) {
if (fStreamStates[i].subsession == NULL)continue;
float npt = fStreamStates[i].subsession->getCurrentNPT(fStreamStates[i].streamToken);
if (npt > curNPT) curNPT = npt;
// Note: If this is an aggregate "PLAY" on a multi-subsession stream, then it's conceivable that the NPTs of each subsession
// may differ (if there has been a previous seek on just one subsession). In this (unusual) case, we just return the
// largest NPT; I hope that turns out OK...
      }
    }
    sprintf(buf, "Range: npt=%.3f-\r\n", curNPT);
  } else
if (absStart != NULL) {
// We're seeking by 'absolute' time:
if (absEnd == NULL) {
      sprintf(buf, "Range: clock=%s-\r\n", absStart);
    } else {
      sprintf(buf, "Range: clock=%s-%s\r\n", absStart, absEnd);
    }
delete[] absStart; 
delete[] absEnd;
  } else {
// We're seeking by relative (NPT) time:
if (rangeEnd == 0.0 && scale >= 0.0) {
      sprintf(buf, "Range: npt=%.3f-\r\n", rangeStart);
    } else {
      sprintf(buf, "Range: npt=%.3f-%.3f\r\n", rangeStart, rangeEnd);
    }
  }
  rangeHeader = strDup(buf);
// Now, start streaming:
for (i = 0; i < fNumStreamStates; ++i) {
if (subsession == NULL 
/* means: aggregated operation */
     || subsession == fStreamStates[i].subsession) {
unsigned
short rtpSeqNum = 0;
unsigned rtpTimestamp = 0;
if (fStreamStates[i].subsession == NULL)continue;
      fStreamStates[i].subsession->startStream(fOurSessionId,
                              fStreamStates[i].streamToken,
                              (TaskFunc*)noteClientLiveness, 
this,
                              rtpSeqNum, rtpTimestamp,
                               RTSPServer::RTSPClientConnection::handleAlternativeRequestByte, ourClientConnection);
const
char *urlSuffix = fStreamStates[i].subsession->trackId();
char* prevRTPInfo = rtpInfo;
unsigned rtpInfoSize = rtpInfoFmtSize
     + strlen(prevRTPInfo)
     + 1
     + rtspURLSize + strlen(urlSuffix)
     + 5 /*max unsigned short len*/
     + 10 /*max unsigned (32-bit) len*/
     + 2 /*allows for trailing \r\n at final end of string*/;
      rtpInfo = new
char[rtpInfoSize];
      sprintf(rtpInfo, rtpInfoFmt,
           prevRTPInfo,
           numRTPInfoItems++ == 0 ? "" :",",
           rtspURL, urlSuffix,
           rtpSeqNum,
           rtpTimestamp
           );
delete[] prevRTPInfo;
    }
  }
if (numRTPInfoItems == 0) {
    rtpInfo[0] = '\0';
  } else {
unsigned rtpInfoLen = strlen(rtpInfo);
    rtpInfo[rtpInfoLen] = '\r';
    rtpInfo[rtpInfoLen+1] = '\n';
    rtpInfo[rtpInfoLen+2] = '\0';
  }
// Fill in the response:
  snprintf((char*)ourClientConnection->fResponseBuffer,sizeof ourClientConnection->fResponseBuffer,
"RTSP/1.0 200 OK\r\n"
"CSeq: %s\r\n"
"%s"
"%s"
"%s"
"Session: %08X\r\n"
"%s\r\n",
        ourClientConnection->fCurrentCSeq,
        dateHeader(),
        scaleHeader,
        rangeHeader,
        fOurSessionId,
        rtpInfo);
delete[] rtpInfo; 
delete[] rangeHeader;
delete[] scaleHeader; 
delete[] rtspURL;
}
Live555 RTP建立流程
RTP的建立流程在客户端发送Setup请求开始建立，客户端发送Setup请求时，会将RTP/RTCP的端口号告诉服务端，也会将Rtp over tcp还是udp的方式告诉到服务端，服务端收到Setup请求时，根据端口号建立socket，在收到客户端的Play命令时，启动流传输；启动流传输的代码如下：
void OnDemandServerMediaSubsession::startStream(unsigned clientSessionId,
void* streamToken,
                            TaskFunc* rtcpRRHandler,
void* rtcpRRHandlerClientData,
unsignedshort& rtpSeqNum,
unsigned& rtpTimestamp,
                            ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
void* serverRequestAlternativeByteHandlerClientData) {
  StreamState* streamState = (StreamState*)streamToken;
  Destinations* destinations
    = (Destinations*)(fDestinationsHashTable->Lookup((charconst*)clientSessionId));
if (streamState != NULL) {
    streamState->startPlaying(destinations,
                    rtcpRRHandler, rtcpRRHandlerClientData,
                    serverRequestAlternativeByteHandler, serverRequestAlternativeByteHandlerClientData);
    RTPSink* rtpSink = streamState->rtpSink(); 
// alias
if (rtpSink != NULL) {
      rtpSeqNum = rtpSink->currentSeqNo();
      rtpTimestamp = rtpSink->presetNextTimestamp();
    }
  }
}
//
Live555 rtsp/rtp是同一个socket，但端口号不同吗？
看源码：
void OnDemandServerMediaSubsession
::getStreamParameters(unsigned clientSessionId,
               netAddressBits clientAddress,
               Port const& clientRTPPort,
               Port const& clientRTCPPort,
int tcpSocketNum,
unsigned
char rtpChannelId,
unsigned
char rtcpChannelId,
               netAddressBits& destinationAddress,
               u_int8_t& /*destinationTTL*/,
               Boolean& isMulticast,
               Port& serverRTPPort,
               Port& serverRTCPPort,
void*& streamToken) {
if (destinationAddress == 0) destinationAddress = clientAddress;
struct in_addr destinationAddr; destinationAddr.s_addr = destinationAddress;
  isMulticast = False;
if (fLastStreamToken != NULL && fReuseFirstSource) {
// Special case: Rather than creating a new 'StreamState',
// we reuse the one that we've already created:
    serverRTPPort = ((StreamState*)fLastStreamToken)->serverRTPPort();
    serverRTCPPort = ((StreamState*)fLastStreamToken)->serverRTCPPort();
    ++((StreamState*)fLastStreamToken)->referenceCount();
    streamToken = fLastStreamToken;
  } else {
// Normal case: Create a new media source:
unsigned streamBitrate;
    FramedSource* mediaSource
      = createNewStreamSource(clientSessionId, streamBitrate);
// Create 'groupsock' and 'sink' objects for the destination,
// using previously unused server port numbers:
    RTPSink* rtpSink;
    BasicUDPSink* udpSink;
    Groupsock* rtpGroupsock;
    Groupsock* rtcpGroupsock;
    portNumBits serverPortNum;
if (clientRTCPPort.num() == 0) {
// We're streaming raw UDP (not RTP). Create a single groupsock:
      NoReuse dummy(envir()); // ensures that we skip over ports that are already in use
for (serverPortNum = fInitialPortNum; ; ++serverPortNum) {
struct in_addr dummyAddr; dummyAddr.s_addr = 0;
     serverRTPPort = serverPortNum;
     rtpGroupsock = new Groupsock(envir(), dummyAddr, serverRTPPort, 255);
if (rtpGroupsock->socketNum() >= 0)break;// success
      }
      rtcpGroupsock = NULL;
      rtpSink = NULL;
      udpSink = BasicUDPSink::createNew(envir(), rtpGroupsock);
    } else {
// Normal case: We're streaming RTP (over UDP or TCP). Create a pair of
// groupsocks (RTP and RTCP), with adjacent port numbers (RTP port number even):
      NoReuse dummy(envir()); // ensures that we skip over ports that are already in use
for (portNumBits serverPortNum = fInitialPortNum; ; serverPortNum += 2) {
struct in_addr dummyAddr; dummyAddr.s_addr = 0;
     serverRTPPort = serverPortNum;
//建立RTPsocket
     rtpGroupsock = new Groupsock(envir(), dummyAddr, serverRTPPort, 255);
if (rtpGroupsock->socketNum() < 0) {
delete rtpGroupsock;
continue; 
// try again
     }
//建立Rtcp socket
     serverRTCPPort = serverPortNum+1;
     rtcpGroupsock = new Groupsock(envir(), dummyAddr, serverRTCPPort, 255);
if (rtcpGroupsock->socketNum() < 0) {
delete rtpGroupsock;
delete rtcpGroupsock;
continue; 
// try again
     }
break; 
// success
      }
unsigned
char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
      rtpSink = createNewRTPSink(rtpGroupsock, rtpPayloadType, mediaSource);
      udpSink = NULL;
    }
// Turn off the destinations for each groupsock. They'll get set later
// (unless TCP is used instead):
if (rtpGroupsock != NULL) rtpGroupsock->removeAllDestinations();
if (rtcpGroupsock != NULL) rtcpGroupsock->removeAllDestinations();
if (rtpGroupsock != NULL) {
// Try to use a big send buffer for RTP - at least 0.1 second of
// specified bandwidth and at least 50 KB
unsigned rtpBufSize = streamBitrate * 25 / 2;// 1 kbps * 0.1 s = 12.5 bytes
if (rtpBufSize < 50 * 1024) rtpBufSize = 50 * 1024;
      increaseSendBufferTo(envir(), rtpGroupsock->socketNum(), rtpBufSize);
    }
// Set up the state of the stream. The stream will get started later:
    streamToken = fLastStreamToken
      = new StreamState(*this, serverRTPPort, serverRTCPPort, rtpSink, udpSink,
              streamBitrate, mediaSource,
              rtpGroupsock, rtcpGroupsock);
  }
// Record these destinations as being for this client session id:
  Destinations* destinations;
if (tcpSocketNum < 0) { 
// UDP
    destinations = new Destinations(destinationAddr, clientRTPPort, clientRTCPPort);
  } else { 
// TCP
    destinations = new Destinations(tcpSocketNum, rtpChannelId, rtcpChannelId);
  }
  fDestinationsHashTable->Add((charconst*)clientSessionId, destinations);
}
//从这段代码中可以看到rtsp，rtp，rtcp的socket是不同的；同时分析了客户端的源码，socket也是不一样的，初始化subsession时，在其中会建立RTP/RTCPsocket以及RTPSource。对于每个subsession都会建立不同的socket。
3)MediaSession和socket的关系？一个MediaSession包括多个连接，关联到多个socket吗？
MediaSession 包括多个MediaSubSession,每个MediaSubSession对应相应的socket，source和sink，形成一个数据流！
