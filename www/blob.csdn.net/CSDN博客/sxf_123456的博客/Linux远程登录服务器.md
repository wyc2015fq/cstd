# Linux远程登录服务器 - sxf_123456的博客 - CSDN博客
2018年01月25日 10:27:13[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：84
                
linux 下登录远程服务器
linux 下登录远程服务器的方式有两种，一是ssh，二是telnet
telnet使用的是明文传输，传输过程账号和密码容易截获。ssh远程登录比较可靠
1、telnet测试端口
        IP      端口
telnet 0.0.0.0  22
2、ssh 远程登录服务器
2.1使用默认端口
ssh 0.0.0.0
2.2使用端口号登录
ssh root@0.0.0.0 -p 端口号
