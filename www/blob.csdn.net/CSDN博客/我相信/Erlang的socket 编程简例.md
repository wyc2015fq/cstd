# Erlang的socket 编程简例 - 我相信...... - CSDN博客





2015年03月07日 07:17:09[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2107








Erlang 中gen_tcp 用于编写TCP程序，gen_udp用于编写UDP程序。一个简单的TCP服务器echo示例：

Start_echo_server()->

         {ok,Listen}= gen_tcp:listen(1234,[binary,{packet,4},{reuseaddr,true},{active,true}]),

         {ok,socket}=get_tcp:accept(Listen),

         gen_tcp:close(Listen),

         loop(Socket).



loop(Socket) ->

         receive

                  {tcp,Socket,Bin} ->

                            io:format(“serverreceived binary = ~p~n”,[Bin])

                            Str= binary_to_term(Bin),

                            io:format(“server  (unpacked) ~p~n”,[Str]),

                            Reply= lib_misc:string2value(Str),

                            io:format(“serverreplying = ~p~n”,[Reply]),

                            gen_tcp:send(Socket,term_to_binary(Reply)),

                            loop(Socket);

                   {tcp_closed,Socket} ->

                            Io:format(“ServerSocket closed ~n”)

         end.



Tcp 的echo客户端示例：

echo_client_eval(Str) ->

         {Ok,Socket} = gen_tcp:connect(“localhost”,2345,[binary,{packet,4}]),

         ok= gen_tcp:send(Socket, term_to_binary(Str)),

         receive

                   {tcp,Socket,Bin}->

                            Io:format(“Clientreceived binary  = ~p~n”,[Bin]),

                            Val=binary_to_term(Bin),

                            io:format(“Clientresult = ~p~n”,[Val]),

                            gen_tcp:close(Socket)

         end.



UDP server示例

udp_demo_server(Port) ->

         {ok,Socket}= gen_udp:open(Open,[Binary]),

         loop(Socket).

Loop(Socket)->

         receive

                   {udp,Socket,Host,Port,Bin}->

                            BinReply= …,

                            gen_udp:send(Socket,Host,Port,BinReply),

                            loop(Socket)

         End.



UDP client 示例：

udp_demo_client(Request) ->

         {ok,Socket}= gen_udp:open(0,[Binary]),

         ok= gen_udp:send(Socket,”localhost”,1234,Request),

         Value=    receive

                                     {udp,Socket,_,_,Bin}-> {ok,Bin}

                            after2000 -> error

                            end,

         gen_udp:close(Socket),

         Value

注意，因为UDP是不可靠的，一定要设一个超时时间，而且Reqeust最好小于500字节。

WebSocket， JS 和Erlang相结合，能够实现Web的绝大多数功能。



