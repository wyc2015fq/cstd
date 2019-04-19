# Live555源码分析RTSPServer中的用户认证 - 建建的博客 - CSDN博客
2017年02月22日 21:33:51[纪建](https://me.csdn.net/u013898698)阅读数：915
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/njzhujinhua @20140601
说到鉴权，这是我多年来工作中的一部分，但这里rtsp中的认证简单多了，只是最基本的digest鉴权的策略。
在Live555的实现中， 用户信息由如下类维护，其提供增删查的接口。realm默认值为"LIVE555 Streaming Media"
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/27859347#)[copy](http://blog.csdn.net/njzhujinhua/article/details/27859347#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/372035/fork)
- class UserAuthenticationDatabase {  
- public:  
-   UserAuthenticationDatabase(charconst* realm = NULL,  
-                  Boolean passwordsAreMD5 = False);  
- // If "passwordsAreMD5" is True, then each password stored into, or removed from,
- // the database is actually the value computed
- // by md5(<username>:<realm>:<actual-password>)
- virtual ~UserAuthenticationDatabase();  
- virtualvoid addUserRecord(charconst* username, charconst* password);  
- virtualvoid removeUserRecord(charconst* username);  
- 
- virtualcharconst* lookupPassword(charconst* username);  
- // returns NULL if the user name was not present
- 
- charconst* realm() { return fRealm; }  
-   Boolean passwordsAreMD5() { return fPasswordsAreMD5; }  
- 
- protected:  
-   HashTable* fTable;  
- char* fRealm;  
-   Boolean fPasswordsAreMD5;  
- };  
一个鉴权过程的例子如下：
**[1]C-->S**
OPTIONS rtsp://10.0.0.10:8554/h264ESVideoTest RTSP/1.0
CSeq: 2
User-Agent: LibVLC/2.1.3 (LIVE555 Streaming Media v2014.01.21)
**[2]S-->C**
RTSP/1.0 200 OK
CSeq: 2
Date: Sat, May 31 2014 14:16:42 GMT
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, GET_PARAMETER, SET_PARAMETER
**[3]C-->S**
DESCRIBE rtsp://10.0.0.10:8554/h264ESVideoTest RTSP/1.0
CSeq: 3
User-Agent: LibVLC/2.1.3 (LIVE555 Streaming Media v2014.01.21)
Accept: application/sdp
**[4]S-->C**
RTSP/1.0 401 Unauthorized
CSeq: 3
Date: Sat, May 31 2014 14:16:43 GMT
WWW-Authenticate: Digest realm="LIVE555 Streaming Media", nonce="73724068291777415fec38a1593568e5"
**[5]C-->S**
DESCRIBE rtsp://10.0.0.10:8554/h264ESVideoTest RTSP/1.0
CSeq: 4
Authorization: Digest username="zjh", realm="LIVE555 Streaming Media", nonce="73724068291777415fec38a1593568e5", uri="rtsp://10.0.0.10:8554/h264ESVideoTest", response="b8c755d897abddd0206954bab0e0b763"
User-Agent: LibVLC/2.1.3 (LIVE555 Streaming Media v2014.01.21)
Accept: application/sdp
**[6]S**
lookupPassword(zjh) returned password 123
鉴权通过，生成SDP信息
RTSP的鉴权发生在DESCRIBE命令之时，在收到DESCRIBE命令时，如果不需要处理鉴权，则直接就走到第6步，生成SDP信息，发送RTSP/1.0 200 OK及SDP信息并等待下一步的setup命令了。
如果需要鉴权则检查是否对本连接生成过nonce随机数，即是否已挑战过。如果没有则发送RTSP/1.0 401 Unauthorized，同时发送的内容中包含server指定的realm以及产生的随机数nonce。WWW-Authenticate: Digest realm="LIVE555 Streaming Media", nonce="73724068291777415fec38a1593568e5"
代码参见
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/27859347#)[copy](http://blog.csdn.net/njzhujinhua/article/details/27859347#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/372035/fork)
- Boolean RTSPServer::RTSPClientConnection::authenticationOK(charconst* cmdName, charconst* urlSuffix, charconst* fullRequestStr)  
- {  
- if (!fOurServer.specialClientAccessCheck(fClientInputSocket, fClientAddr, urlSuffix))  
-     {  
-         setRTSPResponse("401 Unauthorized");  
- return False;  
-     }  
- 
- // If we weren't set up with an authentication database, we're OK:
-     <span style="color:#3333ff;">UserAuthenticationDatabase* authDB = fOurServer.getAuthenticationDatabaseForCommand(cmdName);</span>  
- if (authDB == NULL) return True;  
- 
- charconst* username = NULL; charconst* realm = NULL; charconst* nonce = NULL;  
- charconst* uri = NULL; charconst* response = NULL;  
-     Boolean success = False;  
- 
- do {  
-         <span style="color:#3333ff;">// To authenticate, we first need to have a nonce set up
- // from a previous attempt:
- if (fCurrentAuthenticator.nonce() == NULL) </span><span style="color:#ff0000;"><strong>break;</strong></span><span style="color:#3333ff;">  
- 
- // Next, the request needs to contain an "Authorization:" header,
- // containing a username, (our) realm, (our) nonce, uri,
- // and response string:</span>
-         <span style="color:#3333ff;">if (!parseAuthorizationHeader(fullRequestStr,  
-             username, realm, nonce, uri, response)  
-             || username == NULL  
-             || realm == NULL || strcmp(realm, fCurrentAuthenticator.realm()) != 0  
-             || nonce == NULL || strcmp(nonce, fCurrentAuthenticator.nonce()) != 0  
-             || uri == NULL || response == NULL)  
-         {  
- break;  
-         }  
- 
- // Next, the username has to be known to us:
- charconst* password = authDB->lookupPassword(username);  
- #ifdef DEBUG
-         fprintf(stderr, "lookupPassword(%s) returned password %s\n", username, password);  
- #endif
- if (password == NULL) break;  
-         fCurrentAuthenticator.setUsernameAndPassword(username, password, authDB->passwordsAreMD5());  
- 
- // Finally, compute a digest response from the information that we have,
- // and compare it to the one that we were given:
- charconst* ourResponse  
-             = fCurrentAuthenticator.computeDigestResponse(cmdName, uri);  
-         success = (strcmp(ourResponse, response) == 0);  
-         fCurrentAuthenticator.reclaimDigestResponse(ourResponse);  
-     } while (0);  
- 
- delete[] (char*)realm; delete[] (char*)nonce;  
- delete[] (char*)uri; delete[] (char*)response;  
- 
- if (success)  
-     {  
- // The user has been authenticated.
- // Now allow subclasses a chance to validate the user against the IP address and/or URL suffix.
- if (!fOurServer.specialClientUserAccessCheck(fClientInputSocket, fClientAddr, urlSuffix, username))  
-         {  
- // Note: We don't return a "WWW-Authenticate" header here, because the user is valid,
- // even though the server has decided that they should not have access.
-             setRTSPResponse("401 Unauthorized");  
- delete[] (char*)username;  
- return False;  
-         }  
-     }  
- delete[] (char*)username;  
- if (success) return True;</span>  
- 
-     <span style="color:#cc0000;">// If we get here, we failed to authenticate the user.
- // Send back a "401 Unauthorized" response, with a new random nonce:</span>
-     <span style="color:#ff0000;">fCurrentAuthenticator.setRealmAndRandomNonce(authDB->realm());  
-     snprintf((char*)fResponseBuffer, sizeof fResponseBuffer,  
- "RTSP/1.0 401 Unauthorized\r\n"
- "CSeq: %s\r\n"
- "%s"
- "WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\"\r\n\r\n",  
-         fCurrentCSeq,  
-         dateHeader(),  
-         fCurrentAuthenticator.realm(), fCurrentAuthenticator.nonce());  
- return False;</span>  
- }  
其中第一次收到describe命令调用的如上函数走的有效代码如上述红色所述，其它无关紧要。在第二次收到describe的时候因以生成过挑战nonce，则走的流程如蓝色代码所示。
上述函数中用到的fCurrentAuthenticator定义如下
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/27859347#)[copy](http://blog.csdn.net/njzhujinhua/article/details/27859347#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/372035/fork)
- class RTSPServer  
- {  
- class RTSPClientConnection  
-     {  
-         ...  
- protected:  
-             ...  
-             Authenticator fCurrentAuthenticator; // used if access control is needed
-             ...  
-     };  
- };  
Authenticator则是一个用于digest鉴权的类，定义如下，
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/27859347#)[copy](http://blog.csdn.net/njzhujinhua/article/details/27859347#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/372035/fork)
- // A class used for digest authentication.
- // The "realm", and "nonce" fields are supplied by the server
- // (in a "401 Unauthorized" response).
- // The "username" and "password" fields are supplied by the client.
- class Authenticator {  
- public:  
-   Authenticator();  
-   Authenticator(charconst* username, charconst* password, Boolean passwordIsMD5 = False);  
- // If "passwordIsMD5" is True, then "password" is actually the value computed
- // by md5(<username>:<realm>:<actual-password>)
-   Authenticator(const Authenticator& orig);  
-   Authenticator& operator=(const Authenticator& rightSide);  
- virtual ~Authenticator();  
- 
- void reset();  
- void setRealmAndNonce(charconst* realm, charconst* nonce);  
- void setRealmAndRandomNonce(charconst* realm);  
- // as above, except that the nonce is created randomly.
- // (This is used by servers.)
- void setUsernameAndPassword(charconst* username, charconst* password, Boolean passwordIsMD5 = False);  
- // If "passwordIsMD5" is True, then "password" is actually the value computed
- // by md5(<username>:<realm>:<actual-password>)
- 
- charconst* realm() const { return fRealm; }  
- charconst* nonce() const { return fNonce; }  
- charconst* username() const { return fUsername; }  
- charconst* password() const { return fPassword; }  
- 
- charconst* computeDigestResponse(charconst* cmd, charconst* url) const;  
- // The returned string from this function must later be freed by calling:
- void reclaimDigestResponse(charconst* responseStr) const;  
- 
- private:  
- void resetRealmAndNonce();  
- void resetUsernameAndPassword();  
- void assignRealmAndNonce(charconst* realm, charconst* nonce);  
- void assignUsernameAndPassword(charconst* username, charconst* password, Boolean passwordIsMD5);  
- void assign(charconst* realm, charconst* nonce,  
- charconst* username, charconst* password, Boolean passwordIsMD5);  
- 
- private:  
- char* fRealm; char* fNonce;  
- char* fUsername; char* fPassword;  
-   Boolean fPasswordIsMD5;  
- };  
在fCurrentAuthenticator.nonce()不为空即已发送过挑战后收到describe后的鉴权过程如下
1：首先parseAuthorizationHeader得到username, realm, nonce, uri以及response。这五项均不能为空，否则鉴权失败。
其中username为用户名，realm为域名，必须与server上一步发送的完全一致，nonce为server上一步发送的随机challenge，必须完全一致。uri为要访问的资源标识，response则是client通过digest计算的响应。
2：获取用户名及密码，用户名不存在则鉴权失败
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/27859347#)[copy](http://blog.csdn.net/njzhujinhua/article/details/27859347#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/372035/fork)
- charconst* password = authDB->lookupPassword(username);  
- if (password == NULL) break;  
- fCurrentAuthenticator.setUsernameAndPassword(username, password, authDB->passwordsAreMD5());  
3：根据server知道的信息同样计算response并与收到的response比较
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/27859347#)[copy](http://blog.csdn.net/njzhujinhua/article/details/27859347#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/372035/fork)
- // Finally, compute a digest response from the information that we have,
- // and compare it to the one that we were given:
- charconst* ourResponse  
-     = fCurrentAuthenticator.computeDigestResponse(cmdName, uri);  
- success = (strcmp(ourResponse, response) == 0);  
- fCurrentAuthenticator.reclaimDigestResponse(ourResponse);  
关于计算ourResponse的函数computeDigestResponse实现如下
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/27859347#)[copy](http://blog.csdn.net/njzhujinhua/article/details/27859347#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/372035/fork)
- charconst* Authenticator::computeDigestResponse(charconst* cmd, charconst* url) const
- {  
- // The "response" field is computed as:
- //    md5(md5(<username>:<realm>:<password>):<nonce>:md5(<cmd>:<url>))
- // or, if "fPasswordIsMD5" is True:
- //    md5(<password>:<nonce>:md5(<cmd>:<url>))     
- char ha1Buf[33];  
- if (fPasswordIsMD5) {  
-     strncpy(ha1Buf, password(), 32);  
-     ha1Buf[32] = '\0'; // just in case
-   } else {  
-     unsigned const ha1DataLen = strlen(username()) + 1  
-       + strlen(realm()) + 1 + strlen(password());  
-     unsigned char* ha1Data = new unsigned char[ha1DataLen+1];  
-     sprintf((char*)ha1Data, "%s:%s:%s", username(), realm(), password());  
-     our_MD5Data(ha1Data, ha1DataLen, ha1Buf);  
- delete[] ha1Data;  
-   }  
- 
-   unsigned const ha2DataLen = strlen(cmd) + 1 + strlen(url);  
-   unsigned char* ha2Data = new unsigned char[ha2DataLen+1];  
-   sprintf((char*)ha2Data, "%s:%s", cmd, url);  
- char ha2Buf[33];  
-   our_MD5Data(ha2Data, ha2DataLen, ha2Buf);  
- delete[] ha2Data;  
- 
-   unsigned const digestDataLen  
-     = 32 + 1 + strlen(nonce()) + 1 + 32;  
-   unsigned char* digestData = new unsigned char[digestDataLen+1];  
-   sprintf((char*)digestData, "%s:%s:%s",  
-           ha1Buf, nonce(), ha2Buf);  
- charconst* result = our_MD5Data(digestData, digestDataLen, NULL);  
- delete[] digestData;  
- return result;  
- }  
本函数实现及注释十分清晰，几乎看文字描述一样了。
注释说的很明白，如果password是MD5格式，则其已经是username:realm:plainPwd的md5摘要。我们将response表示为md5(A:B:C)
则A=md5格式password 或 md5(<username>:<realm>:<明文password>)
B=nonce
C=md5(<cmd>:<uri>)
如上代码中即分别为ha1Buf,nonce(),和ha2Buf()的拼接过程了。
这里的数据全是字符串。如ha1Data="zjh:LIVE555 Streaming Media:123"  ha1Buf=md5(ha1Data)="ad68dbfd3e130bcabd2e61d19e5695fd"
ha2Data="DESCRIBE:rtsp://10.0.0.10:8554/h264ESVideoTest"   ha2Buf="1d47c98b00946762aad35c10a7e61736"
digestData则为"ad68dbfd3e130bcabd2e61d19e5695fd:**73724068291777415fec38a1593568e5**:1d47c98b00946762aad35c10a7e61736"
在访问同一资源uri的多次情况下，实际变动的只有中间的nonce部分。
计算之后只需将此response与client在describe命令中的response比较即可。
