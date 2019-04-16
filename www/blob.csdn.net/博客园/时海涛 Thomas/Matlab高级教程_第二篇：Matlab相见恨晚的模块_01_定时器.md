# Matlab高级教程_第二篇：Matlab相见恨晚的模块_01_定时器 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：Matlab相见恨晚的模块_01_定时器](https://www.cnblogs.com/noah0532/p/8625155.html)





MATLAB的定时器功能(timer函数)：

1　　从MATLAB6.5版本开始，MATLAB开始支持定时器。相对于传统的pause函数来说，定时器要强大的多，不仅可以等效实现pause的功能，还可以显示更多pause无法实现的功能，一些需要在一定周期内独立运行的程序特别适合利用定时器来完成。

2　　一个完整的定时器程序并让它发挥作用，一直到最后删除需要以下几个步骤：

　　第一，利用timer函数建立定时器

　　第二，利用start函数启动定时器

　　第三，利用stop函数停止定时器

　　第四，利用delete函数删除定时器

　　第五，利用clear函数删除代表定时器句柄的变量。

3　　定时器具体是个什么东西？

　　我们知道，程序运行是自上而下，从左向右的运行方式，除了像while或者for循环控制流之外，程序运行到最末行整个程序就会停止运行。从本质上来说while或for循环控制流也是自上而下，从左向右的运行方式，条件为假停止运行退出。但是我们平时操作一些程序的时候，举个例子：比如我们玩儿一款游戏叫“王者荣耀”，如果我们能看到他的代码，其运行方式也是自上而下，从左向右的运行方式，但是全部运行完毕了，这个游戏还是正常在运行并没有停止。这也就是说，这个游戏运行程序的时候，整个程序到了结尾处再从头开始运行，如此循环往复，指导我们给定一个条件会停止整个循环往复，这也就实现了程序的整个动态运行。其在MATLAB当中要实现整个功能我们称之为定时器。

4　　第一，利用timer函数建立定时器

　　timer函数的调用结构非常的简单，只是一个简单的句柄函数加属性的方式，但是其属性稍微多一些。

**t = timer(name,value);**

　　在有的材料上也可以写成，都是一会儿事儿。

**t = time('PropertyName1',PropertyValue1,...,**'PropertyNameN',PropertyValueN**);**

　　其中t为定时器的句柄变量

　　name或者PropertyName为属性名

　　value或者PropertyValue为属性值

　　这样一个定时器就建立好了。剩下的工作就是用t这个句柄来进行开关删的操作。



　　4.1 举例：

**TimerA=timer('timerfcn',@TimerFunA,'StartDelay',0.5,'executionmode','fixedrate');**

　　有这么一个定时器。

　　TimerA为这个定时器的句柄变量

　　timer中的属性和值分别解释如下：

　　1) 'timerfcn'        ===>>> @TimerFunA

　　　属性：定时器回调函数  值：@TimerFunA

　　2)'StartDelay'     ===>>> 0.5

　　   属性：定时器从开始运行到第一次执行TimerFcn直接的时间延迟，单位是秒   值：0.5

　　3)'executionmode'  ===>>> 'fixedrate'

　　   属性：定时器执行模式    值：在MATLAB中添加计时器回调函数后立即启动



　　4.2 其中我们看到其定时器的建立非常简单，但是其中的属性值略多，现在就把常用的属性和可选值进行列举一下：

　　属性名：　　　　　　　　　　　　属性描述 ：                                           可选值：                                    备注：

　　1.TimerFcn　　　　　　　　　　　定时器回调函数                                     字符串，函数句柄　　　　　    设置循环函数，timer会循环调用该公式

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 或者cell数组　　　　　　　　  通常用@句柄的形式调用一个写好的函数

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 上面的例子就是这样

　　2.StartDelay                                      指定定时器从开始运行到第一次                                                              任何大于等于0的double型数值

　　　　　　　　　　　　　　　　　　执行TimerFcn之间的时间延迟，                                                              默认是0秒

　　　　　　　　　　　　　　　　　　单位是秒                                                                                                  当属性'Running'设为'on'时只读

　　3.ExecutionMode                              定时器的执行模式　　　　　　　　　　singleshort　　　　　　　　  这三种模式图示见MATLAB的doc

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　fixedDelay　　　　　　　　　singleShot

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　fixedRate                                  fixedDelay

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　fixedspacing                             fixedRate

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　  fixedspacing

　　4.Period　　　　　　　　　　　　设置循环时间间隔，即隔多长时间                                                            任何大于0.001的double型数值

　　　　　　　　　　　　　　　　　   循环一次。　　　　　　　　　　　　　　　　　　　　　　　　　　  默认值是1.0　　

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 当属性'Running'设为'on'时只读

　　5.TasksToExecute　　　　　　　　指定定时器需要执行TimerFcn函数的次数　　　　　　　　　　　　    设置循环次数。设置为inf，表示无线循环，默认为1

　　6.TasksToExecuted　　　　　　　  从定时器开始运行时其，记录TimerFcn调　　　　　　　　　　　　　任何大于等于0的double型数值

　　　　　　　　　　　　　　　　　　用的次数，TimerFcn每执行一次，　　　　　　　　　　　　　　　　默认值为1

　　　　　　　　　　　　　　　　　　TasksToExecuted自动加1　　　　　　　　　　　　　　　　　　　　始终是只读型

5　　第二，利用start函数启动定时器

　　  第三，利用stop函数停止定时器

　　  start(TimerA);start(TimerB);

　　  stop(TimerA);

　　  很简单吧。



6. 关于TimerFcn中的@回调函数的编写

function f_trade_timer3
% 主程序定时器
TimerA=timer('timerfcn',@TimerFunA,'StartDelay',0.5,'executionmode','fixedrate');
start(TimerA);
    function TimerFunA(obj,event)
        global main market order risk
        if (main.timenum>150000 & main.timenum<153000) | (main.timenum>023000 & main.timenum<030000)
        path_trade_data=[pwd,'\data\d_future\d_future_trade\'];
        eval(['save ',path_trade_data,'d_future_trade3.mat market order ']);
       stop(TimerA);
        end
    end
end

比如这么一段定时器的编写，控制大于某一个时间段，保存数据到路径，然后停止计时器。

其中@TimerFunA句柄了这么一个函数，所以定时器编写的时候后面一般要加上这么一个函数

这个函数的编写略有不同，如果是单一的TimerFunc函数的话要按照obj对象和event事件的方式进行起函数名。因为这里是用的对象句柄的方式

类似于 @ TimerFunA , (obj,event)

再比如这么一段代码

t=timer('TimerFcn',{@timercallback,handles},'ExecutionMode', 'fixedRate', 'Period', 0.01);
start(t)
function timercallback(obj, event,handles) 
str=datestr(now, 'HH:MM:SS:FFF');
set(handles.edit1, 'String',str);














