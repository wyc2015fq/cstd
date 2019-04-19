# win cmd 设置代理 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月01日 16:06:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：162

windows:
HTTP(S)代理服务器：127.0.0.1:5783
SOCKS代理服务器：127.0.0.1:5789
- set http_proxy=http://127.0.0.1:1189
- set https_proxy=[http://127.0.0.1:1189](http://127.0.0.1:1189/)
SOCKS5 代理设置：
|12|set http_proxy=socks5://127.0.0.1:1080set https_proxy=socks5://127.0.0.1:1080|
git代理
git config --global http.proxy http://127.0.0.1:8118
git config --global https.proxy [http://127.0.0.1:8118](http://127.0.0.1:8118/)
SOCKS5 代理设置
git config --global http.proxy socks5://127.0.0.1:1080
git config --global https.proxy socks5://127.0.0.1:1080
取消
git config --global --unset http.proxy
git config --global --unset https.proxy
Socks代理比其他应用层代理要快得多。它通常绑定在代理服务器的1080端口上。
SOCK4只支持TCP协议，SOCK5支持TCP和UDP协议
Http代理其端口一般是80
