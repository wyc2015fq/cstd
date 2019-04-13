
# LoadRunner学习笔记 - anlcy - 博客园






# [LoadRunner学习笔记](https://www.cnblogs.com/camilla/p/8576922.html)
**什么是性能测试：**
简单说，功能测试是软件是否能用，性能测试是看软件好不好用；
性能测试的含义，大体来讲就是通过自动化的手段，模拟生产运行的业务压力或者相应的场景，来测试协同系统是否满足生产需要。
**性能测试（Performance Testing）核心概念**：1.负载测试(Load Testing):关注用户数量，即性能指标，尽可能的真实场景；
2.压力测试（Stress Testing）：关注的是高压力，即系统是怎么死的，预案场景，不需要思考时间；
3.容量测试：模拟系统长时间运行后的性能状态，最大的支撑数据数量。
4、稳定性测试：长时间，标准用户数（最佳用户-系统处于最佳状态，最大用户数-某一个指标出现极限）；
**性能测试的基本指标概念**：1.响应时间（Response Time），从用户角度评价系统的处理速度（2秒快，5秒一般，10秒慢）；
2.吞吐量（IO）：包括硬盘IO，网络IO，CPU内存，请求处理能力，打开页面数量；
3.事物处理能力（TPS Transaction Per Second）：事物即例如打开一个页面或者实现登录或者实现一笔网购等。
**性能测试关注点：**
1.响应时间，即服务器端的处理速度； 2.服务器端的资源使用情况；3.数据库端的资源使用情况；4.最大访问用户数量；
5.最大业务处理数量（关心核心业务就好）；6.考察系统能否支持7*24小时的持续运转；7.内存资源、线程资源能否正常回收。
8.从代码：算法和sql语句的优化；9.系统的稳定性、可恢复性
**性能测试的核心原理：**
1.基于协议，基于页面，基于代码；2.多线程（目的为了模拟用户负载）；3.模拟真实场景（a：多用户登录。b：用户访问随时间的不同而不同）。**性能测试常见指标：**
![](https://images2018.cnblogs.com/blog/74815/201804/74815-20180403100245535-1166626301.png)
**LR三大组件：**
Vuser：用户行为模拟器--单脚本测试；Controller：性能测试控制台--运行测试场景；Analysis：结果统计--找瓶颈。
![](https://images2018.cnblogs.com/blog/74815/201803/74815-20180318224704122-1110092240.png)
LR启动web页面出现错误解决方式：
1.端口被占用，cmd输入netstat -ano 查找各端口被什么程序占用，找到占用程序的PID，在资源管理器关闭改运用程序；
2.Internal error：your request  was unsuccessful Cannot create CGL process  program not found,就按装strawberry-perl-5.12.3.0，和lr安装在一个盘符。
LR日志：
1.回放日志（replay log）即脚本回放运行时输出的日志，使用不同的颜色文本记录日志：
黑色：标准输出消息；红色：标准错误信息；绿色：用引号括起来的文字字符串9例如URL；
蓝色：事物信息（开始、结束、状态和时间）；橙色：迭代次数信息。
2.录制日志（recording log），录制脚本时，Vugen会捕捉client端（浏览器）与服务器之间的对话，并且记录下来，生产脚本，在vugen的recording log中，我们可以找到浏览器与服务器之间的所有的对话，包括通信内容、日期、时间、浏览器的请求、服务器的响应内容等。
Vugen错误处理函数：
LR在脚本的Run-time Settings中，可以设置在脚本运行过程中发生错误的处理方式。进入到Run-time Settings中，切换到Miscellaneous标签页，可以看到Error Handing的第一个设置项：Continue on error，勾选Continue on error选项后，如果运行过程中发生错误，也会继续执行脚本。
显然，无论是否勾选Continue on error项，对于一些在Controller中运行的脚本都可能存在问题：勾选后，将会忽略所有函数的执行错误，即使一些关键业务函数执行发生错误也会忽略掉，而遇到这种情况时继续运行场景显然已经没有意义；如果不勾选，一些无关紧要的函数执行时如果发生错误将导致当前Vuser停止运行，而当较多Vuser因为小错误而停止执行后同样会导致整个场景的运行结果失去意义，显然这种情况也不应该发生。
VuGen提供了错误处理函数lr_continue_on_error，用来在脚本中实时修改Vuser的出错设置。lr_continue_on_error函数语法结构如下：
void**lr_continue_on_error**( int*value*);
*value*是是脚本运行出错设置的取值，具体取值及相应值的含义如表1-7所示。在使用表1-7中的参数时，既可以使用常量名称作为参数，也可直接使用对应常量值作为参数。
表1-7
|常量名称
|常量值
|含义
|
|LR_ON_ERROR_NO_OPTIONS
|0
|取消出错继续执行设置
|
|LR_ON_ERROR_CONTINUE
|1
|发生错误继续运行脚本
|
|LR_ON_ERROR_SKIP_TO_NEXT_ACTION
|2
|发生错误跳至下个Action运行
|
|LR_ON_ERROR_SKIP_TO_NEXT_ITERATION
|3
|发生错误跳至下一迭代来运行
|
|LR_ON_ERROR_END_VUSER
|4
|发生错误结束当前用户
|
lr_continue_on_error调用后将对其后面所有的语句产生影响。因此，脚本中较常用的是lr_continue_on_error (0)与lr_continue_on_error (1)。这两个语句成对出现，期间包含着需要设置为发生错误停止执行的关键业务脚本，如例1-71所示。
可以看出，例1-71比较适合压力较大且在夜间自动运行的测试场景，而这类场景通常会在Run-time Settings中开启Continue on error选项，因此需要通过lr_continue_on_error (0) 关闭Continue on error选项，使一些关键业务发生错误停止执行。对于一些非关键业务，则需要通过lr_continue_on_error (1)开启Continue on error选项，这样即使遇到错误也不会影响脚本的继续执行。
例1-71
|Action()
|{
|。。。。。。。。。。。。。。。。。
|。。。。。。。。。
|lr_continue_on_error (0); //下面是一些重要事务代码，因此关闭出错继续执行设置
|……
|……
|lr_continue_on_error(1); //后面是一些非关键事务代码，例如下载一些图片文件，因此开启出错继续执行参数设置。
|。。。。。。。。。。
|return 0；
|}
|
反之，如果在Run-time Settings中没有勾选Continue on error项，则应该通过组合使用lr_continue_on_error (1)与lr_continue_on_error (0)来使一些非关键业务发生错误后继续执行。
**LR移动APP录制：**
首先需要安装mobile app(http/html)协议（LR_03105_patch4.exe）或者使用wireshark-win抓包录制,然后创建一个wiff热点。
**LR创建java脚本：**
1.环境配置：安装jdk，注意LR11最高配置jdk1.6；
2.配置环境变量；
3.在LR选择java Vuser协议；
4.脚本结构：一般在init中编写初始化脚本(例如登录等)，在action中编写业务流程（需要重复使用的业务流程），在end编写释放资源的内容（例如退出）。
5.适用范围：1.不太适合录制业务功能点性能测试，如视频播放、http文件下载。
2.利用java语言开发的应用系统。
LR验证一个脚本的过程：
1.Genrate：录制或者开发脚本；
2.SUSI（Single User Single Iterations,单用户单循环）运行录制生成的脚本，解决困难存在的关联问题；
3.SUMI(Single User Multi Iterations,单用户多循环)：参数化脚本，早run-time中设置iteration，再次运行，验证参数化问题；
4.MUSI(Multi User Single Iteration,多用户单循环)：Controller里多用户运行脚本，验证脚本中困难的多线程同步问题；
5.MUMI（Multi User Multi Iteratiosn，多用户多循环）：即性能测试开始。
其中1和2、3在VU中执行，4和5在controller运行.
**LR脚本参数化：**
1.select next row（选择下一行方式）：
顺序sequential：安照参数化的数据顺序，一个一个来取；随机（Random）：随机从设置的参数中取数据；唯一unique: 为每个虚拟用户分配唯一的参数；
2.update value on(更新时的值)：
每次迭代（each iteration）：每次迭代时取新的数据，假如50个用户都取第一条数据，称为一次迭代,完了50个用户都取第二条数据，后面以此类推；
每次出现（each occurrence）：每次参数化时取新的值，强调前后2次取值不能相同；
只取一次(once):设置的参数，一条数据只能被抽取一次。（如果数据轮次完，脚本还在运行将会报错）
上面两个选项都有三种情况，如果将他们进行组合，将产生九种取值方式。
|Select Next Row
|【选择下一行】
|Update Value On
|【更新时的值】
|Replay Result
|【结果】
|
|顺序（Sequential）
|每次迭代(Each iteration)
|结果：分别将15条数据写入数据表中
|功能说明：每迭代一次取一行值，从第一行开始取。当所有的值取完后，再从第一行开始取
|如：如果参数化文件中有15条数据，而迭代设置为16次，那执行结果中，参数化文件第一行的数据有两条
|
|顺序（Sequential）
|每次出现(Each occurrence)
|结果：分别将15条数据写入数据表中
|功能说明：每迭代一次取一行值，从第一行开始取。当所有的值取完后，再从第一行开始取
|如：如果参数化文件中有15条数据，而迭代设置为16次，那执行结果中，参数化文件第一行的数据有两条
|
|顺序（Sequential）
|只取一次(once)
|结果：表中写入15条一模一样的数据。
|功能说明：每次迭代都取参数化文件中第一行的数据。
|
|随机（Random）
|每次迭代(Each iteration)
|结果：表中写入15条数据，但可能有重复数据出现
|功能说明：每次从参数化文件中随机选择一行数据进行赋值
|
|随机（Random）
|每次出现(Each occurrence)
|结果：表中写入15条数据，但可能有重复数据出现
|功能说明：每次从参数化文件中随机选择一行数据进行赋值
|
|随机（Random）
|只取一次(once)
|结果：表中写入15条相同数据
|功能说明：第一次迭代时随机从参数化文件中取一行数据，后面每次迭代都用第一次迭代的数据。
|
|唯一（Unique）
|每次迭代(Each iteration)
|自动分配块大小
|结果：分别将15条数据写入数据表中
|功能说明：第一次迭代取参数化文件中的第一条数据，第二次迭代取第二条数据，以此类推。
|注：如果设置迭代次数为16次。结果：在执行第16次迭代时会抛异常，异常日志可在LoadRunner的回放日志（replayLog）中看到。
|
|唯一（Unique）
|每次出现(Each occurrence)
|步长为1
|结果：分别将15条数据写入数据表中
|功能说明：第一次迭代取参数化文件中的第一条数据，第二次迭代取第二条数据，以此类推。
|注：如果设置迭代次数为16次，而参数化文件中只有15条数据，明显数据不够。此时可以设置“when out of values”属性来判断当数据不够时的处理方式
|Abort Vuser：中断虚拟用户
|Countinue in a cylic manage:循环取参数化文件中的值，即：当参数化文件中的值取完后又从参数化文件的第一行开始取值。
|Countinue with last value：继续用最后一条数据
|
|唯一（Unique）
|只取一次(once)
|结果：表中写入15条相同数据
|功能说明：每次都取参数文件中的第一条数据进行赋值
|
**LR编码乱码：**
1.录制过程中产生的乱码；2.运行过程编码不一致产生乱码。
录制解决方式：在录制时的tool下面的Recording Options中设置Advanced选项中勾选Support charset选项；
运行过程中编码解决方式：Vuser下面选择run-time settings中选择preferences中选择general下面的convert front/to UTF-8,如果录制的系统是UTF-8，则选择YES；
**LR关联核心原理：**
通过设置左右边界值从响应html当中查询我们想要的内容；
LR通过方法web_reg_save_param查找，例如：
普通查找：　web_reg_save_param("testV",
"LB=name=\"test\" value=\"",
"RB=\">",
LAST);--即查找web页面中name=test的input的value值，并将值保存到testV的参数中。
我们可以在被查找函数后面打印验证下关联参数是否正确：
例如：lr_output_message(lr_eval_string("{testV}"));
关联查找：web_reg_save_param("test",
"LB/DIG/ALNUMIC=name=\"\#\#\" value=\"",
"RB=\">",
"Ord=2",--从找出的列表指定第2个内容
LAST);
数组 关联：
int i;
char mystring[9]="" ;
web_reg_save_param("test",
"LB/DIG/ALNUMIC=name=\"\#\#\" value=\"",
"RB=\">",
"Ord=all",--将查找到的所有内容保存到数组test里面
LAST);
打印数组test：
for（i=1;i<10;i++）{//循环10个值
sprintf(mystring,"{test{_%d}",i);
lr_output_message(lr_eval_string(mystring));
}
或者：
for（i=1;i<lr_paramarr_len("test");i++）{
lr_output_message(lr_paramarr_idx("test",i));//循环输出所有值；
}
或者：lr_output_message(lr_paramarr_random("test"));//随机输出一个值；
将获取的值保存到一个LR参数里面,方便在脚本里面引用：
例如：lr_save_string(lr_paramarr_random("test"),"testid");脚本引用{testid}；
其中\#\#为通配符号，用通配符号我们在设置关联参数（Save Data to a Parameter）时必须
勾选Macth case;
Save Data to a Parameter设置关联参数选项里面的各个设置解释：
1.Macth case：允许用通配符；
2、Binary data：二进制参数；
3、Use \# for ary digt：用\#号做通配符；
4、Use ^ a% a wildcard for'方式' alphanumerical characters：匹配大小写机制
5、instance：如果匹配到多个值，设置取第几个值，用int整数或者all设置参数值；
6、relative Frame ID：查找响应包含框架页面的响应；
7、Not Found ：当未查到值时的处理方式，直接报错或者警告；
8:、Search in：查找响应内容的方式；
9:、Save Length：保存查找出来的内容的长度，也就是字符串截取；
10、Save Offset：保存截取时从哪个长度开始；
11、 Lgnore redirections:过滤重定向；
关联函数一定要放在被查找的函数的前面，不能放在被关联的函数前面。
**LR事务（Transaction）：**
1.定义：事务是这样一个点，我们为了衡量某个操作的性能，需要在操作的开始和结束位置插入这样一个范围，这就定义了事务；
2.原因：从性能角度出发，我们需要知道不同的操作所花费的时间，这样我们就可以衡量不同的操作对被测系统所造成的影响，那
么我们如何知道不同的操作所花费的时间，这就用到了事务，我们在操作之前插入一个事务开始标识，在操作完成之后插入一个事
务结束标识，这样我们就知道了这个操作所花费的时间。
3.作用：LR运行到该事务的开始点，就开始计时，直到运行到该事务的结束点，计时就结束，这个事务运行的时间在LR的运行结果
中会有反映，通俗的讲事务就是一个计时标识，通常事务时间所反映的是一个操作过程的响应时间。
**LR事务响应时间：**
LR事务一统计每一个请求或者每一批请求的响应时间，也就是处理速度， 二是统计事务的成功率：即系统的稳定性；
检测每个请求事务的时间，有2种方法，第一种，在Vuser 下的run-time setting中选择miscellaneous选项中设置aoutomatic transaction，
选择define each action as a transaction，则统计action为单位，选择define each step as a transaction，则以单个请求为单位统计事物；
第二种方法，是在脚本中将要统计的请求包括在lr_start_transaction()和lr_end_transaction()之间，方法插入方式：在Vuser的insert菜单下
选择start transaction和end transaction新建，这样只详细统计包括在方法类的请求事务的时间。
**LR检查点：**
由于事物响应时间统计并不排除事物是否是正确的响应，这样统计是有偏差的，我们就要设计一个检查点来控制只统计正确的事物响应；
在录制时需要打开菜单Vuser ---> run-time setting ，选择Preferences 选项，勾选“Enable Image and text check”选项。
检查函数只能用于以HTML-based方式录制的脚本，录制脚本的方式是在Recording Options中选择的；
如果不勾选此项，脚本运行将不会执行查找函数。要判断的执行前插入检查点，使用右键方式，
弹出的下拉的菜单中进行选择“insert”的选项，选择“new step”的选项。
LR中使用web_reg_find()方法检查，检查的实质：从响应当中查找特定的内容，特定的标识，来决定请求是否成功提交；例如：
图片检查,放在要检查的请求后面：web_image_check("web_image_check"，"Alt="，"Src="，LAST);
参数解释：“Alt”和“Src”的值直接取该图片在网页源代码中相应参数的值；
文本检查，放在要检查的请求前面：web_reg_find("Search=Body", --定义查找范围
"SaveCount=logincount",--定义计数变量
"Text=退出",--定义查找内容
LAST);--查找页面是否有退出，将找到的次数保存到logincount，找到则表示登录的事物是成功的；
*检查点与事物结合实例：*
*if(atoi(lr_eval_string("{logincount}"))==1){--atoi是c语言中字符串转整型函数*
*lr_end_transaction("Login",LR_PASS);*
*//lr_output_message("Log in successfully!");--打印消息*
*}else{**lr_end_transaction("Login",LR_FAIL);*
*lr_error_message("Log in Failly!");*
*}*
**LR思考时间：**
思考时间即用户用于思考的时间，即用户暂停发请求的时间；在LR中使用函数lr_think_time(12);12即暂停12秒；
如果脚本设置了思考时间，则我们在运行时要设置回放的思考时间，在Vuser下面选择run-time settings中选择
think time 选项中选择replay think time，因为每个用户的思考时间是不一样的，则我们选择user random percentage of
recorded think time，设置一个随机的用户暂停时间范围，暂停时间最好不要设置太长。
**LR集合点：**
集合点就是指所有用户同时提交同一个请求，也就是严格上的并发测试；比如多个用户同一时间登录。
集合点的用处对于LR来说意义非常大，它可以设置多个虚拟用户等待到一个点，同时出发一个事务，
以达到模拟真实环境下同时多个用户操作，同时模拟负载，实现性能测试的最终目的，由此可见，插入
集合点只要是为了衡量在加重负载的情况下服务器的性能情况，从而找到性能瓶颈。
在LR中用lr_rendezvous("login")函数设置，哪个请求需要并发测试，就将函数放在请求脚本前面就可以，
*且集合点一定要放在事物的外面。*
*通过场景设计controller设置集合点策略：在scenario下选择rendezvous设置集合，enable vuser设置选中的用户不参与集合点。*
*打开集合点设置页面的Policy，设置集合点运行策略：*
1．当百分之多少的用户到达集合点时脚本继续。
2．当百分之多少的运行用户到达集合点时脚本继续
3．多少个用户到达集合点时脚本继续。
这3个策略的区别在于：假设脚本由100个用户来运行，但100个用户并不是一开始就共同运行的。假设每隔1分钟添加10个用户，
也就是说10分钟后系统才有100个在线用户。这里100就是指系统访问的所有用户数，而不同时间的在线用户数是不同的。
设置的集合点策略百分比均为100%。
在场景运行时，当Vuser脚本运行到集合点函数时，该虚拟用户会进入集合点状态直到集合点策略满足后才释放。
策略1是指当全部用户都运行到了集合点函数才释放集合，让这100个用户并发运行后面的脚本。
策略2是指当前时间如果只有10个用户在线，那么只需要这10个用户都运行到了集合点函数就释放集合，让这10个用户并发运行后面的脚本。
策略3就比较好理解了，当到达集合点的用户数达到自己设置的数量后就释放等待，并发运行后面的脚本。
可以在多个脚本上设置相同的集合点名称来实现多个脚本同时并发的效果。
备注：
LoadRunner官网下载地址:
[https://software.microfocus.com/en-us/products/loadrunner-load-testing/download](https://software.microfocus.com/en-us/products/loadrunner-load-testing/download)
语言包:
[LoadRunner 12.60 Community Edition Language Packs](https://software.microfocus.com/pnx/download/submit/request?fileName=LoadRunner_12.60_Community_Edition_Language_Packs_Micro_Focus_LoadRunner_1260_Community_Edition_Language_Packs.exe&productNumber=T7177I&lang=en-us)
LoadRunner12.60安装指南:
[https://admhelp.microfocus.com/lr/en/12.60/help/PDFs/LoadRunner_Installation_Guide.pdf](https://admhelp.microfocus.com/lr/en/12.60/help/PDFs/LoadRunner_Installation_Guide.pdf)






