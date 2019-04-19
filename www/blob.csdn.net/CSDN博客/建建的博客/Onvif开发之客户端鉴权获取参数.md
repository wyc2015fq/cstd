# Onvif开发之客户端鉴权获取参数 - 建建的博客 - CSDN博客
2017年02月14日 14:50:07[纪建](https://me.csdn.net/u013898698)阅读数：522
ONVIF中不管是客户端还是设备端，最先实现的接口都是关于能力的那个接口，在客户端实现的函数名也就是[soap_call___tds__GetServiceCapabilities]通过获取的接口才知道设备具有那些能力，能够[转载地址](http://blog.csdn.net/max_min_go/article/details/17617057)进行那些操作，服务端最基本的也需要实现这接口，让客户端知道设备支持那些基本操作。但是当设备端作了加密处理的话，即使你实现了这些接口，也不能正常获取到参数的，所以需要在获取设备参数之前，每次都需要作鉴权处理，这也是为什么我在前篇搜索的例程中把ONVIF_Initsoap这个接口函数里面的操作单独用一个函数接口来完成的原因。下面，我们就需要在这个接口函数里处理鉴权的问题了首先ONVIF鉴权是有两种方式的，一种是通过实现soap_wsse_add_UsernameTokenDigest函数，然后又实现soap_wsse_add_UsernameTokenText等一些复杂的函数接口，我第一次也是用这个鉴权方式做，但是太麻烦，而且最重要的是鉴权还是失败的，最后无奈网上各种问，终于发现了一个很简单直观的方法，虽然还是不明白为什么，但是确认达到了鉴权的效果了,在前篇中已经介绍过Onvif_Initsoap这个接口了鉴权的主要操作也是在这个接口里作的认证，具体鉴权实现如下：
**[cpp]**[view
 plain](http://blog.csdn.net/max_min_go/article/details/17617057#)[copy](http://blog.csdn.net/max_min_go/article/details/17617057#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134241/fork)
- typedefstruct
- {  
- char username[64];  
- char password[32];  
- }UserInfo_S;  
- 
- staticvoid ONVIF_GenrateDigest(unsigned char *pwddigest_out, unsigned char *pwd, char *nonc, char *time)  
- {  
- const unsigned char *tdist;  
-     unsigned char dist[1024] = {0};  
- char tmp[1024] = {0};  
-     unsigned char bout[1024] = {0};  
-     strcpy(tmp,nonc);  
-     base64_64_to_bits((char*)bout, tmp);  
-     sprintf(tmp,"%s%s%s",bout,time,pwd);  
-     SHA1((const unsigned char*)tmp,strlen((constchar*)tmp),dist);  
-     tdist = dist;  
-     memset(bout,0x0,1024);  
-     base64_bits_to_64(bout,tdist,(int)strlen((constchar*)tdist));  
-     strcpy((char *)pwddigest_out,(constchar*)bout);  
- }  
- //鉴权操作函数，以及上面的用到了openssl接口
- staticstruct soap* ONVIF_Initsoap(struct SOAP_ENV__Header *header, constchar *was_To, constchar *was_Action, int timeout, UserInfo_S *pUserInfo)  
- {  
- struct soap *soap = NULL;  
-     unsigned char macaddr[6];  
- char _HwId[1024];  
-     unsigned int Flagrand;  
-     soap = soap_new();  
- if(soap == NULL)  
-     {  
-         printf("[%d]soap = NULL\n", __LINE__);  
- return NULL;  
-     }  
-      soap_set_namespaces( soap, namespaces);  
- //超过5秒钟没有数据就退出
- if (timeout > 0)  
-     {  
-         soap->recv_timeout = timeout;  
-         soap->send_timeout = timeout;  
-         soap->connect_timeout = timeout;  
-     }  
- else
-     {  
- //如果外部接口没有设备默认超时时间的话，我这里给了一个默认值10s
-         soap->recv_timeout    = 10;  
-         soap->send_timeout    = 10;  
-         soap->connect_timeout = 10;  
-     }  
-     soap_default_SOAP_ENV__Header(soap, header);  
- 
- // 为了保证每次搜索的时候MessageID都是不相同的！因为简单，直接取了随机值
-     srand((int)time(0));  
-     Flagrand = rand()%9000 + 1000; //保证四位整数
-     macaddr[0] = 0x1; macaddr[1] = 0x2; macaddr[2] = 0x3; macaddr[3] = 0x4; macaddr[4] = 0x5; macaddr[5] = 0x6;  
-     sprintf(_HwId,"urn:uuid:%ud68a-1dd2-11b2-a105-%02X%02X%02X%02X%02X%02X",  
-             Flagrand, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);  
-     header->wsa__MessageID =(char *)malloc( 100);  
-     memset(header->wsa__MessageID, 0, 100);  
-     strncpy(header->wsa__MessageID, _HwId, strlen(_HwId));  
- 
- // 这里开始作鉴权处理了，如果有用户信息的话，就会处理鉴权问题
- //如果设备端不需要鉴权的话，在外层调用此接口的时候把User信息填空就可以了 
- if( pUserInfo != NULL )  
-     {  
-         header->wsse__Security = (struct _wsse__Security *)malloc(sizeof(struct _wsse__Security));  
-         memset(header->wsse__Security, 0 , sizeof(struct _wsse__Security));  
- 
-         header->wsse__Security->UsernameToken = (struct _wsse__UsernameToken *)calloc(1,sizeof(struct _wsse__UsernameToken));  
-         header->wsse__Security->UsernameToken->Username = (char *)malloc(64);  
-         memset(header->wsse__Security->UsernameToken->Username, '\0', 64);  
- 
-         header->wsse__Security->UsernameToken->Nonce = (char*)malloc(64);  
-         memset(header->wsse__Security->UsernameToken->Nonce, '\0', 64);  
-         strcpy(header->wsse__Security->UsernameToken->Nonce,"LKqI6G/AikKCQrN0zqZFlg=="); //注意这里
- 
-         header->wsse__Security->UsernameToken->wsu__Created = (char*)malloc(64);  
-         memset(header->wsse__Security->UsernameToken->wsu__Created, '\0', 64);  
-         strcpy(header->wsse__Security->UsernameToken->wsu__Created,"2010-09-16T07:50:45Z");  
- 
-         strcpy(header->wsse__Security->UsernameToken->Username, pUserInfo->username);  
-         header->wsse__Security->UsernameToken->Password = (struct _wsse__Password *)malloc(sizeof(struct _wsse__Password));  
-         header->wsse__Security->UsernameToken->Password->Type = (char*)malloc(128);  
-         memset(header->wsse__Security->UsernameToken->Password->Type, '\0', 128);  
-         strcpy(header->wsse__Security->UsernameToken->Password->Type,\  
- "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest");  
-         header->wsse__Security->UsernameToken->Password->__item = (char*)malloc(128);  
-         ONVIF_GenrateDigest((unsigned char*)header->wsse__Security->UsernameToken->Password->__item,\  
-                 (unsigned char*)pUserInfo->password,header->wsse__Security->UsernameToken->Nonce,header->wsse__Security->UsernameToken->wsu__Created);  
- 
-     }  
- if (was_Action != NULL)  
-     {  
-         header->wsa__Action =(char *)malloc(1024);  
-         memset(header->wsa__Action, '\0', 1024);  
-         strncpy(header->wsa__Action, was_Action, 1024);//"http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
-     }  
- if (was_To != NULL)  
-     {  
-         header->wsa__To =(char *)malloc(1024);  
-         memset(header->wsa__To, '\0', 1024);  
-         strncpy(header->wsa__To,  was_To, 1024);//"urn:schemas-xmlsoap-org:ws:2005:04:discovery";   
-     }  
-     soap->header = header;  
- return soap;  
- }  
之所以把这些操作独立一个接口是因为每次在客户端获取参数之前都需要调用这个接口，需要是获取soap和就是鉴权操作了。实际上，整个鉴权的过程，通过这个来作是非常简单的，而且不需要那么多接口来越搞越乱，这是一个前辈指导我这么做的，但是具体为什么我也不清楚，**如果有大神知道的话，可以交流下，不甚感激**！
下面我们就来看看鉴权的效果吧，看看获取这重要的一个接口，能力值获取的接口
实现如下：
**[cpp]**[view
 plain](http://blog.csdn.net/max_min_go/article/details/17617057#)[copy](http://blog.csdn.net/max_min_go/article/details/17617057#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134241/fork)
- int ONVIF_Capabilities()  //获取设备能力接口
- {  
- 
- int retval = 0;  
- struct soap *soap = NULL;  
- struct _tds__GetCapabilities capa_req;  
- struct _tds__GetCapabilitiesResponse capa_resp;  
- 
- struct SOAP_ENV__Header header;  
- 
-     UserInfo_S stUserInfo;  
-     memset(&stUserInfo, 0, sizeof(UserInfo_S));  
- 
- //正确的用户名和错误的密码
-     strcpy(stUserInfo.username, "admin");  
-     strcpy(stUserInfo.password, "admin");  
- 
- //此接口中作验证处理， 如果不需要验证的话，stUserInfo填空即可
-     soap = ONVIF_Initsoap(&header, NULL, NULL, 5, &stUserInfo);  
- char *soap_endpoint = (char *)malloc(256);  
-     memset(soap_endpoint, '\0', 256);  
- //海康的设备，固定ip连接设备获取能力值 ,实际开发的时候，"172.18.14.22"地址以及80端口号需要填写在动态搜索到的具体信息
-     sprintf(soap_endpoint, "http://%s:%d/onvif/device_service", "172.18.14.22", 80);  
- 
-     capa_req.Category = (enum tt__CapabilityCategory *)soap_malloc(soap, sizeof(int));  
-     capa_req.__sizeCategory = 1;  
-     *(capa_req.Category) = (enum tt__CapabilityCategory)0;  
- //此句也可以不要，因为在接口soap_call___tds__GetCapabilities中判断了，如果此值为NULL,则会给它赋值
- constchar *soap_action = "http://www.onvif.org/ver10/device/wsdl/GetCapabilities";  
- 
- do
-     {  
-         soap_call___tds__GetCapabilities(soap, soap_endpoint, soap_action, &capa_req, &capa_resp);  
- if (soap->error)  
-         {  
-                 printf("[%s][%d]--->>> soap error: %d, %s, %s\n", __func__, __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
-                 retval = soap->error;  
- break;  
-         }  
- else//获取参数成功
-         {  
- // 走到这里的时候，已经就是验证成功了，可以获取到参数了，
- // 在实际开发的时候，可以把capa_resp结构体的那些需要的值匹配到自己的私有协议中去，简单的赋值操作就好            
-               printf("[%s][%d] Get capabilities success !\n", __func__, __LINE__);  
-         }  
-     }while(0);  
- 
-     free(soap_endpoint);  
-     soap_endpoint = NULL;  
-     soap_destroy(soap);  
- return retval;  
- }  
我们知道，海康的设备用户名是"admin",
 密码是"12345",所以上面第一次测试的话，我估计填写了错误的密码！
**[cpp]**[view
 plain](http://blog.csdn.net/max_min_go/article/details/17617057#)[copy](http://blog.csdn.net/max_min_go/article/details/17617057#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134241/fork)
- 
- //正确的用户名和错误的密码
-     strcpy(stUserInfo.username, "admin");  
-     strcpy(stUserInfo.password, "admin");  
![](https://img-blog.csdn.net/20131227191950296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**[cpp]**[view
 plain](http://blog.csdn.net/max_min_go/article/details/17617057#)[copy](http://blog.csdn.net/max_min_go/article/details/17617057#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134241/fork)
- 
- //正确的用户名和密码
-     strcpy(stUserInfo.username, "admin");  
-     strcpy(stUserInfo.password, "12345");  
![](https://img-blog.csdn.net/20131227192130468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上下两次结果对比很明显了，认证通过了，而且客户端也能正常获取能力值了！
如果顺利进行到这里了，客户端的参数部分开发基本也就差不多了，剩下的工作就是填充一些结构体以及复制一些代码工作了！
这里我说下自己在开发客户端的过程中的几个经验，后面也将继续完善客户端的代码：
1. 搜索固定端口3702，而且不可以跨网段的，虚拟机下也无法搜索，也能点对点的，也就是probe按钮操作。
2. 每次用soap_malloc分配新的空间的时候，一定记得清空操作，建议：宁愿多分配100个字节，也不要冒险纠结1个字节的！
3. 有些获取参数的或者设置参数的时候，需要填写对应的token，token也要不同的操作匹配不同的token值！profiles函数会告诉你所有操作的token值！
 4 onvif开发是极其单调的工作，多是赋值粘贴的操作，请耐心点对待工作。
下面我添加下base64加密解密的两个接口函数！
**[cpp]**[view
 plain](http://blog.csdn.net/max_min_go/article/details/17617057#)[copy](http://blog.csdn.net/max_min_go/article/details/17617057#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134241/fork)
- #include <stdio.h>
- #include <ctype.h>
- 
- staticconstchar base64digits[] =  
- "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  
- 
- #define BAD     -1
- staticconstsignedchar base64val[] = {   
-     BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD,  
-     BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD,  
-     BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD, 62, BAD,BAD,BAD, 63,   
-     52, 53, 54, 55,  56, 57, 58, 59,  60, 61,BAD,BAD, BAD,BAD,BAD,BAD,  
-     BAD,  0,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,   
-     15, 16, 17, 18,  19, 20, 21, 22,  23, 24, 25,BAD, BAD,BAD,BAD,BAD,  
-     BAD, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, 40,   
-     41, 42, 43, 44,  45, 46, 47, 48,  49, 50, 51,BAD, BAD,BAD,BAD,BAD  
- };  
- #define DECODE64(c)  (isascii(c) ? base64val[c] : BAD)
- void base64_bits_to_64(unsigned char *out, const unsigned char *in, int inlen)  
- {  
- for (; inlen >= 3; inlen -= 3)  
-     {     
-         *out++ = base64digits[in[0] >> 2];   
-         *out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];  
-         *out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];  
-         *out++ = base64digits[in[2] & 0x3f];  
-         in += 3;  
-     }     
- 
- if (inlen > 0)  
-     {     
-         unsigned char fragment;  
- 
-         *out++ = base64digits[in[0] >> 2];   
-         fragment = (in[0] << 4) & 0x30;  
- 
- if (inlen > 1)  
-             fragment |= in[1] >> 4;  
- 
-         *out++ = base64digits[fragment];  
-         *out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];  
-         *out++ = '=';  
-     }  
- 
-     *out = '\0';  
- }  
- int base64_64_to_bits(char *out, constchar *in)  
- {  
- int len = 0;  
- register unsigned char digit1, digit2, digit3, digit4;  
- 
- if (in[0] == '+' && in[1] == ' ')  
-         in += 2;  
- if (*in == '\r')  
- return(0);  
- 
- do {  
-         digit1 = in[0];  
- if (DECODE64(digit1) == BAD)  
- return(-1);  
-         digit2 = in[1];  
- if (DECODE64(digit2) == BAD)  
- return(-1);  
-         digit3 = in[2];  
- if (digit3 != '=' && DECODE64(digit3) == BAD)  
- return(-1);  
-         digit4 = in[3];  
- if (digit4 != '=' && DECODE64(digit4) == BAD)  
- return(-1);  
-         in += 4;  
-         *out++ = (DECODE64(digit1) << 2) | (DECODE64(digit2) >> 4);  
-         ++len;  
- if (digit3 != '=')  
-         {  
-             *out++ = ((DECODE64(digit2) << 4) & 0xf0) | (DECODE64(digit3) >> 2);  
-             ++len;  
- if (digit4 != '=')  
-             {  
-                 *out++ = ((DECODE64(digit3) << 6) & 0xc0) | DECODE64(digit4);  
-                 ++len;  
-             }  
-         }  
-     } while (*in && *in != '\r' && digit4 != '=');  
- 
- return (len);  
- }  
- //简单的demo测试程序，实际可以不需要下面这个，只要上面两个函数接口就好！
- int main(void)  
- {  
- char p[] = "I Love You, Forever!";  
- char test[48] = {0};  
-     base64_bits_to_64(test, p, sizeof("I Love You, Forever!"));  
- 
-     printf("p = %s , test = %s \n", p, test);  
- char a[48]= {0};  
-     base64_64_to_bits( a, test);  
-     printf("a = %s , test = %s \n", a, test);  
- return 0;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)

