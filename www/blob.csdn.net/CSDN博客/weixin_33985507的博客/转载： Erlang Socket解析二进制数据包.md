# 转载： Erlang Socket解析二进制数据包 - weixin_33985507的博客 - CSDN博客
2013年08月06日 16:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
转自：http://www.itkee.com/developer/detail-318.html
今天在家里闲来无事，实践了一下[Erlang](http://www.itkee.com/tag/erlang)的Socket的功能。记录一下在过程中遇到的一些问题，以及编码的步骤。
**1. 对于测试用例的介绍：**
Erlang编写TCP服务器。只做一次Accept，接收到Socket
之后开始收数据。用python编写Client，连接到服务器上；发送LEN(int)+CMD(short)+BODY(binary)格式的数据
包。用于熟悉Erlang如何做拆解包，数据读取。
**2. 编写简单的Erlang TCP服务器：**
Erlang里面的TCP socket应该都是这个方式来编写代码。指的修改和优化的是在于可以启动更多的进程来驱动起这个应用。
%% 文件名:server.erl
%% 模块定义
-module(server).
%% 导出函数
-export([start/0]).
%% 宏定义
-define( PORT, 2345 ).
-define( HEAD_SIZE, 4 ).
%% 解数字类型用到的宏
-define( UINT, 32/unsigned-little-integer).
-define( INT, 32/signed-little-integer).
-define( USHORT, 16/unsigned-little-integer).
-define( SHORT, 16/signed-little-integer).
-define( UBYTE, 8/unsigned-little-integer).
-define( BYTE, 8/signed-little-integer).
%% 对外接口
start() ->
 %% 这个地方有些有意思的东西:
 %% 1.{packet,0}这个设定，可以让Erlang不再接管socket的封包了；
 %%   如果被Erlang接管了，在物理网络包前面4Bytes里面写的东西不
 %%  是简单的网络包的Size.
 %% 2.{active,false}这个设定，可以让接受到的Socket Recv指定Size
 %%   网络包，这样也就方便了拆解包的工作了。
    {ok, Listen}=gen_tcp:listen( ?PORT,[ binary,
                    { packet, 0 }, { reuseaddr, true }, { active, false }]),
    io:format("start listen port: ~p~n", [?PORT] ),
    {ok, Socket} = gen_tcp:accept(Listen),
 %% 接收到客户端之后将马上关闭Listen Socket
    gen_tcp:close( Listen ),
 %% 开始读取数据包头
    looph(Socket).
%% 读出包头
looph(Socket) ->
    case gen_tcp:recv( Socket, ?HEAD_SIZE ) of
        { ok, H } ->
            io:format("recv head binary=~p~n", [H] ) ,
            %% 匹配出包头
            << TotalSize:?UINT >> = H ,
   %% 除去包头的SIZE
            BodySize = TotalSize - ?HEAD_SIZE,
            %% 开始收包体
            loopb(Socket,BodySize);
  %% 出异常了
        { error, closed } ->
            io:format("recv head fail." )
    end.
%% 读出包体
loopb(Socket,BodySize) ->
    case gen_tcp:recv( Socket, BodySize ) of
        { ok, B } ->
            %% 模式匹配
   %% 1.得出数据包中的CMD编号
   %% 2.将后面部分的Buffer放到Contain里面
            << CMD:?USHORT, Contain/binary>> = B,
            io:format("recv body binary = ~p~n", [B] ),
            io:format("recv protocol CMD = ~p~n", [CMD] ),
            io:format("recv body = ~p~n", [Contain] ),
   %% 继续读取包头
            looph(Socket);
  %% 异常处理
        {error,close} ->
            io:format("recv body fail.")
    end.
在编写这个代码过程中遇到的麻烦：
2.1. 不知道如何匹配出数据包头来：
<< TotalSize:?UINT >> = H 
2.2. 不知道如何将一个binary匹配出来部分，将剩余部分binary放到别的里面:
<< CMD:?USHORT, Contain/binary>> = B
2.3. 在多次调试之后出来这样的错误：
{error,eaddrinuse}
端口被占用了，这个时候去关闭全部后台的.beam也是没有解决这个问题。最后重启了机器才能让这个问题解决。
2.4. Erlang中对于binary操作的熟悉：
term_to_binary和binary_to_term函数的功效：
用于将一个任意的Erlang值转化成为二进制（反向操作），这个特性可能也只有在Erlang之间打交道的时候可以用上。
list_to_binary：
这个函数非常有用，原因是它不挑食。打个比方：
1> A = "A".
"A"
2> B = list_to_binary(A).
<<"A">>
结果这个"A"字符串被好好的放在了binary里面去了。
还有一个用处就是用来连接已经生成好的一些binary的对象
10> A = << 1,2,3,4 >>.
<<1,2,3,4>>
11> B = << "A" >>.
<<"A">>
12> C = list_to_binary( [A, B] ).
<<1,2,3,4,65>>
**3. 开始编写python客户端代码：**
这个Socket客户端是使用的asyncore的dispatcher来做的。用起来有些像ACE里面的reactor模型。这个代码写起来非常容易了。
# -*- coding: utf-8 -*-
import socket
import asyncore
# 宏定义
MAX_RECV_CACHE = 1024
CHAT_MSG = 0x101A
# 聊天客户端
class ChatClient( asyncore.dispatcher ):
    def __init__( self, host = Host, port = Port ):
        asyncore.dispatcher.__init__( self )
        self.create_socket( socket.AF_INET, socket.SOCK_STREAM)
        self.connect( ( host, port) )
        self.buffer_ = ''
        self.recv_buf_ = ''
        pass
    # 链接成功
    def handle_connect( self ):
        print( "[SOCKET] handle_connect event." )
        self.send_message( CHAT_MSG, "hello then world." )
        self.send_message( CHAT_MSG, "this data is come from python." )
        pass
    # 读取内容
    def handle_read( self ):
        ret = self.recv( MAX_RECV_CACHE )
        pass
    def send_message( self, _prop_cmd, _msg ):
        print( "presend size = %d"%len( _msg ) )
        total_size = len( _msg ) + 4 + 2
        self.buffer_ = self.buffer_ + struct.pack( "I", total_size )
        self.buffer_ = self.buffer_ + struct.pack( "H", _prop_cmd )
        self.buffer_ = self.buffer_ + _msg
    pass
if __name__ == "__main__":
    try:
        client = ChatClient()
        asyncore.loop()
    except KeyboardInterrupt:
        print( "退出." )
    pass
完结。下次开始学习Erlang的OTP ETS了。
