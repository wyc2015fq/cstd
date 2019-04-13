
# TCP_UDP网络调试工具 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月05日 17:45:58[seven-soft](https://me.csdn.net/softn)阅读数：6856


TCP_UDP网络调试工具使用说明
芝麻软件工作室 （http://softroom.org）     QQ:1005974096

TCP_UDP网络调试工具下载:
https://sourceforge.net/projects/softroom/files/release/TCP_UDP_debug_tool/1.0.0.1/TCP_UDP_DEBUG_TOOL.exe/download

TCP_UDP网络调试工具是网络socket测试的好助手，可以帮助开发人快速发现网络socket通信问题，还可以抓取网络socket的通信数据，方便分析。
1，可以作为TCP SERVER（服务器端）接受TCP Client（客户端）的连接请求，并与客户端相互通信。
2，可以作为TCP Client（客户端）与TCP SERVER（服务器端）进行连接，并与服务器端相互通信。

3，可以作为UDP SERVER（服务器端）接受UDP Client（客户端）的访问，并与客户端相互通信。
4，可以作为UDP Client（客户端）接受UDP SERVER（服务器端）的访问，并与服务器端相互通信。
5，C/S和协议是选择软件的工作模式（TCP SERVER、TCP Client、UDP SERVER、UDP Client）
6，远端IP是要访问目标的IP地址
7，远程端口是要访问目标的端口号（PORT）
8，指定本机端口是使用的本机端口号（PORT）
9，网络接收设置-HEX显示是选中之后，接收到的数据以HEX（十六进制）显示，否则以ASCII显示
10，状态-接收是目前总共接收到了多少个字节的数据
11，状态-发送时目前总共发送了多少个字节的数据
12，计算清零是把状态-接收和状态-发送的数据清零
13，清除是清空网络接收到的数据
14，HEX发送是选中之后会以HEX（十六进制）把数据发送出去，否则会以ASCII方式把数据发送出去
15，发送新行是选中之后会在发送末尾追加回车换行，否则不会追加任何内容。
16，发送是点击发送会把左边编辑框内容发送出去
17，定时发送是选中之后会定时发送数据出去，可以定时的时间间隔（单位是毫秒）


