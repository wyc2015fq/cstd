# loadrunner中log的使用初步总结 - weixin_33985507的博客 - CSDN博客
2015年02月04日 13:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
1、log的设置方式 。
在 runtime setting中可以设置log的生成方式：
默认的log方式：
Enable logging选中，log option是Send messages only when an error occurs.
可以修改日志的方式：
Always send messages（这种方式会一直打印输出日志，不仅在错误时）
standard log——记录所有的请求反馈的日志，包括successful和fail的日志。
Extended log——可提供扩展的日志信息，包括
Parameter subsititution——日志中打印所有中使用的参数值。
Data returned by server——日志中打印每个客户端请求服务器返回的数据值
Advanced trace——日志中打印所有的消息信息和函数执行信息
2、log的存储方式
log的存储路径在res文件夹下。
如果是从脚本中直接关联create scenario则在脚本的目录下的res下。如果是直接create 
scenario然后再选择脚本的话，则存储在c盘，如“C:\Documents and Settings\USERNAME\Local 
Settings\Temp\res\log”
具体该场景的日志结果存哪，可以查看controll的result界面的result setting定义的目录。
根据不同的log记录级别，在log文件中记录日志。
3、log信息的分析
1）从log中得到虚拟用户失败的原因。
通常如果场景在运行时出现用户失败，则先要查看错误原因，可直接查看日志，从日志中查看ERROR的信息；
从outputdb中可以查看到错误代码ERROR CODE
2）从日志中确认每次分配给虚拟用户的参数值
想判断是否在场景中每个用户使用不同的或预定义规则的参数，可以通过在log生成规则处设定为parameter 
subsititution，然后查看每个日志文件中的对应行参数值是否为预计的参数值。可从此判断出是否同用户的实际使用类似，是否达到对服务器的压力
策略。
3）从日志中确认服务器端返回的值是什么。
在脚本中通常设定了检查点，检查点策略是否生效，如果对此产生怀疑则可以考虑从日志中查看一下。查看服务器返回的值来验证是不是所期待得到的值。
思考问题
log的输出 会不会影响到客户端，会不会使客户端成为瓶颈？（认为是会的，任何程序都是要消耗资源的，loadrunner也一样，所以选取日志输出的模式是要谨慎考虑尽量以适用为前提）
