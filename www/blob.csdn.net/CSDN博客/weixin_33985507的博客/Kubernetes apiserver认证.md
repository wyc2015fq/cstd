# Kubernetes apiserver认证 - weixin_33985507的博客 - CSDN博客
2017年04月02日 17:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# kubernetes认证
> 
Kubernetes集群的操作可以通过apiserver来进行操作，kubectl命令最终也是调用的apiserver，如果想要获取对apiserver进行操作，需要先通过其认证
api-server的认证方式：
- 
#### 基本认证：basic-auth
--basic-auth-file=/path/to/basic-auth.csv
在basic-auth.csv中拥有以列为单位的认证信息，格式为password，username，uid
***示例：***
```
passwd，kinderao，1
password2，test，2
```
然后在 kube-apiserver启动的时候加上--basic-auth-file=/path/to/basic-auth.csv这个参数，启动起来过后再在使用k8s的api就需要加上认证信息，否则就会unauthorized，加认证信息的方法是在http请求的header中添加一个Authorization，value是Basic base64编码后的用户名密码信息
- 
#### Token认证：token-auth
--token-auth-file=/path/to/token-auth.csv
在token-auth.csv中拥有以列为单位的认证信息，格式为token，username，uid
***示例***
```
token,kinderao,1
token2,kinderao2,2
```
同样也是在apiserver的启动参数里面加入--token-auth-file=/path/to/token-auth.csv这个参数，然后在请求的时候同样在header中添加Authorization，value是Bearer token
- 
#### CA证书认证：
在使用证书认证之前首先需要申请证书，证书可以通过权威CA来申请，也可以通过自签证书，不过部署kubernetes的大多数环境都是内网环境，所以更多的还是使用的是自签证书。
生成证书的步骤如下：
- 首先需要你的linux系统上安装有openssl，大多数的linux发行版都带有这个工具，使用openssl生成根证书cacert：
```bash
# 生成密钥
openssl genrsa -out ca.key 2048
# 生成根证书
openssl req -x509 -nodes -key ca.key -subj "/CN=yourcomany.com" -days 5000 -out ca.crt
```
- 为server生成证书：
```bash
# 生成server的密钥
openssl genrsa -out server.key 2048
# 生成证书申请，其中的hostname需要填入你的服务器的域名或者ip地址，这个地方有个坑
# 就是在这填入的是什么地址，在client请求的时候就要使用这个地址，之前配的是ip，但是请求的时候使用主机名，导致一直没有出现bad certificate的问题
openssl req -new -key server.key -subj "/CN=`hostname`" -out server.csr
# 使用刚才生成的根证书以及密钥来生成server的证书
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 5000
```
- 再为client生成证书:
```bash
# 生成client的密钥
openssl genrsa -out client.key 2048
# 生成证书申请，这里需要填的地址和上面的server一致即可
openssl req -new -key client.key -subj "/CN=`hostname`" -out client.csr
# 使用根证书和密钥来生成client的证书
openssl x509 -req -in client.csr -CA ca.crt -CAcreateserial -out client.crt -days 5000
```
- 配置api-server的启动参数,在apiserver的启动参数中加上下面的启动参数
```
--secure-port=443 
--client_ca_file=/root/genkey/ca.crt 
--tls-private-key-file=/root/genkey/server.key 
--tls-cert-file=/root/genkey/server.crt
```
也可以放到/etc/kubernets/apiserver配置文件的args参数里面
启动kube-apiserver会看见一下日志：
`I0330 05:17:46.582385 4776 config.go:531] Will report 10.0.0.103 as public IP address. [restful] 2017/03/30 05:17:46 log.go:30: [restful/swagger] listing is available at https://10.0.0.103:443/swaggerapi/ [restful] 2017/03/30 05:17:46 log.go:30: [restful/swagger] https://10.0.0.103:443/swaggerui/ is mapped to folder /swagger-ui/ I0330 05:17:46.950556 4776 serve.go:104] Serving securely on 0.0.0.0:443 I0330 05:17:46.950618 4776 serve.go:118] Serving insecurely on 127.0.0.1:8080`
我们在本机上使用curl来验证一下:
```
curl https://hostname:443 --cacert ca.crt --key client.key --cert client.crt
```
以后的请求都需要带有根证书和client key和client的证书
