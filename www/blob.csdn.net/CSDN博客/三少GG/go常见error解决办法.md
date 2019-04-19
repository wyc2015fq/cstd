# go常见error解决办法 - 三少GG - CSDN博客
2015年12月02日 19:22:26[三少GG](https://me.csdn.net/scut1135)阅读数：1874

1.
**unrecognized import path** "golang.org/x/crypto/bcrypt"
原因是网络被GFW墙啦！！！！用pandafan的代理进行设置。
要为 shell 配置 HTTP 代理只需两步：
- 
设定环境 http_proxy ，bash中的命令为
`export http_proxy=http://SERVER:PORT`
`export https_proxy=http://SERVER:PORT`
SERVER 和 PORT 即服务器地址、端口
