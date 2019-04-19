# gSOAP 源码分析(四) - 建建的博客 - CSDN博客
2017年02月17日 15:50:06[纪建](https://me.csdn.net/u013898698)阅读数：445
http://blog.csdn.net/flyfish1986/article/details/7626230
gSOAP 源码分析(四) 
2012-6-2 邵盛松
  前言
本文主要说明gSOAP中对Client的认证分析
gSOAP中包含了HTTP基本认证，NTLM认证等，还可以自定义SOAP Heard实现认证等
一 HTTP基本认证
Client在向Server发送请求时提供以用户名和口令形式的凭证
gSOAP中对应 用户名是userid，密码是passwd
const char *userid; /* HTTP Basic authorization userid */
const char *passwd; /* HTTP Basic authorization passwd */
用户名和密码采用Base64编码
处理base64编码
SOAP_FMAC1 int SOAP_FMAC2 soap_putbase64(struct soap*, const unsigned char*, int);
SOAP_FMAC1 unsigned char* SOAP_FMAC2 soap_getbase64(struct soap*, int*, int);
(查标签的含义AuthorizationBasic *
WWW-AuthenticateProxy-Authenticate
Expect100-continue
gSOAP源码中HTTP基本认证部分
  else if (!soap_tag_cmp(key, "Authorization"))
  { if (!soap_tag_cmp(val, "Basic *"))
    { int n;
      char *s;
      soap_base642s(soap, val + 6, soap->tmpbuf, sizeof(soap->tmpbuf) - 1, &n);
      soap->tmpbuf[n] = '\0';
      if ((s = strchr(soap->tmpbuf, ':')))
      { *s = '\0';
        soap->userid = soap_strdup(soap, soap->tmpbuf);
        soap->passwd = soap_strdup(soap, s + 1);
      }
    }
  }
Client用冒号将用户名和密码连接在一起，并以base64[算法](http://lib.csdn.net/base/datastructure)进行编码，传输到Server
从HTTP头中找到Authorization: Basic 标签，使用soap_base642s将Base64编码转换为字符串，strchr找到冒号（：）的位置然后截取，分解出了soap->userid 用户名和soap->passwd 密码，然后由Server验证
二 NTLM认证
gSOAP提供了NTLM认证方式
NTLM是NT LAN Manager的缩写，如果想要代码支持NTLM认证需要增加#define WITH_NTLM
认证原理图如下
![](https://img-my.csdn.net/uploads/201206/02/1338627391_7420.png)
Domain中存在一个[数据库](http://lib.csdn.net/base/mysql)，该数据库存储属于这个Domain计算机的用户名 密码Hash值等信息
1 Client开机的时候，需要输入用户名和密码，假设输入的用户名是admin，密码是123456，那么[操作系统](http://lib.csdn.net/base/operatingsystem)会将123456加密，存储密码的哈希值假设为H1，为了安全原密码123456会被丢弃
2 Client向Server发送请求，请求中包含admin这个用户名
3 Server接收到请求后，生成一个16位的随机数。这个随机数被称为Challenge或者Nonce，
Server先将Server保存起来，然后以明文的方式发送给Client，gSOAP对应的Challenge是ntlm_challenge
4 Client在接收到server发回的Challenge后，用H1对其加密，然后再将加密后的Challenge（H2） 发送给Server
5 Server接收到Client发送回来的加密后的Challenge后，会向Domain发送针对Client的验证请求。该请求主要包含以下三方面的内容：Client的用户名；Client的密码哈希值加密的Challenge（H2）和原始的Challenge。
6 Domain根据用户名获取该帐号的密码哈希值，对原始的Challenge进行加密生成H3。如果加密后的Challenge（H3）和Server发送的（H2）一致，则意味着用户拥有正确的密码，验证通过，否则验证失败。Domain将验证结果发给Server，并最终由Server反馈给Client
代码中对应变量声明 
 const char *authrealm; /* HTTP authentication realm (NTLM domain) */
 const char *ntlm_challenge; /* HTTP NTLM challenge key string */
 short ntlm_auth; /* HTTP NTLM authentication type */
 short ntlm_stage; /* HTTP NTLM stage 0..3 */
三 gSOAP可以扩展SOAPHeader，对Client进行验证
gSOAP绑定了HTTP，消息标识和消息关联都在HTTP中，所以gSOAP中关于SOAP Header的结构
只有一个虚设的成员变量，这个结构体属于虚设代码(dummy code)
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
private:
char dummy; /* dummy member to enable compilation */
#endif
};
SOAP中可以绑定去其他协议，如果协议中不提供SOAP Header可以对SOAP进行扩充，利用SOAP Header实现消息标识和消息关联
四 其他
在每一个Client连接Server时，都提供了自身的IP，Server可以指定允许连接到Server的IP
只要在calcService这个封装类accept()的添加
SOAP_SOCKET calcService::accept()
{ 
CString strIP=_T("");
strIP.Format(_T("客户端的IP是%d.%d.%d.%d"),((this->ip)>>24)&0xFF,((this->ip)>>16)&0xFF,((this->ip)>>8)&0xFF,((this->ip))&0xFF);
    AfxMessageBox(strIP);//这里增加IP的对比
return soap_accept(this);
}
