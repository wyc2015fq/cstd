# cloudflare的新waf，用Lua实现的 - 囧囧有神的专栏 - CSDN博客





2014年07月03日 10:41:10[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：7771








> 



我们使用nginx贯穿了我们的网络，做前线web服务，代理，流量过滤。在某些情况下，我们已经扩充了nginx上我们自己的模块的核心C代码，但最近我们做了一个重大举措，与nginx结合使用lua


> 
几乎全部用lua写的我们的一个项目是新的cloudflare WAF。这个我们另有博客。[​http://blog.cloudflare.com/heuristics-and-rules-why-we-built-a-new-old-waf](http://blog.cloudflare.com/heuristics-and-rules-why-we-built-a-new-old-waf)


> 
Lua WAF使用nginx Lua模块来嵌入Lua代码，执行Lua code就像nginx 正常handling阶段的代码一样。WAF的整个执行实际上由下列nginx配置控制：
location / {   
        set $backend_waf    "WAF_CORE";   
        default_type      'text/plain';   
        access_by_lua '      
            local waf = require "waf"      
            waf.execute()   
        ';
}


> 
这个access_by_lua指令告诉nginx，执行这些Lua code作为一个access 阶段的handler。然后，WAF中的Lua代码决定是否阻断请求，或将请求传递给原始服务器去处理。Waf模块通过调用ngx.exit()来终结，响应码或者为200（nginx继续处理），或者为403（阻断请求）




> 
所有真实的WAF工作被用lua写的waf模块做了。实现这个WAF，我们希望能够读取为流行mod_security开源WAF开发的现有WAF配置和支持我们自己的简单的WAF语言。该mod_security的语言已经集成到了Apache配置文件，这样导致它是有点难以阅读，但也有一个庞大的规则使用它（如流行的OWASP规则集）撰写，我们希望能够在本机运行。


> 
在写新的WAF之前，我们实际上还运行了Apache，只是为了mod_security，但这个组合缓慢，繁琐，且不能随着CloudFlare的不断增长的业务扩展。我们已经做了妥协，只是为了保持它运行。


> 
一个用mod_security本地语言写的规则像下列所示，（这是一个我们落实过的真实客户的有点模糊的版本）。第一个版本用mod_security版本书写，第二个等价于cloudflare自己的规则语言书写。
SecRule REQUEST_HEADERS:User-Agent "@beginsWith DataStore/" 
     "id:100000,phase:0,t:none,deny,chain,msg:'DataStore Attack'"   
SecRule REQUEST_METHOD "@streq GET" "chain"      
SecRule REQUEST_URI "\/\?-?\d+=-?\d+" ""

rule 100000 DataStore Attack   
    REQUEST_HEADERS:User-Agent has-prefix DataStore/ and   
    REQUEST_METHOD is GET and   
    REQUEST_URI matches \/\?-?\d+=-?\d+     
    deny


> 
我们将开放自定义规则来拓展更广泛的能力，不管是用mod_security还是cloudflare风格来写waf规则。最重要的是cloudflare的WAF语言不是Lua，尽管事实上这个WAF是用Lua实现的。


> 
实际上，这个WAF首先通过将不管是用mod_security还是cloudflare风格书写的规则转化成通用的JSON格式，这个JSON格式编码了规则，并且加入了一些WAF UI的额外的信息（例如这个规则是否使能）


> 
这个JSON格式，然后编译成WAF执行的Lua程序。这个编译步骤允许我们支持不同的输入语言（像以上说的两种），并且性能得到优化，这样WAF执行的更快。


> 
举例说明，编译器执行以下任务：

- 子句排序，这样当一个子条款不符合时，规则可以快速跳过 
- 正则表达式的优化和简化
- 运算符更换，这样更快的运算符（比如更简单的字符串匹配）用在可能的地方
- 关于WAF运行的线索，关于是否宏扩展是必要的。
- 全局优化，如识别重复使用相同的字符串或变量，并确保他们只计算一次
- Lua的优化，例如使用全局函数的本地引用

> 
> 
上面的规则，结果转化为如下Lua代码：
if waf_begins(waf, v3_6, '3_6', t3_1, '3_1', 'DataStore/', false) then  
    waf.vars['RULE']['ID'] = '100000'  
    if waf_eq(waf, v3_7, '3_7', t3_1, '3_1', 'GET', false) then    
        if waf_regex(waf, v3_4, '3_4', t3_1, '3_1', [=[\/\?-?\d+=-?\d+]=],          false, nil, false) then      
            waf_activate(waf, rulefile)      
            waf_msg(waf, 'DataStore Attack')      
            waf_deny(waf, rulefile)    
        end  
    end
end



> 
> 
生成的代码难以阅读，因为它本质上是WAF的汇编语言，而且是自动生成的。Waf运行实现了像waf_begins, waf_eq, and waf_regex这些用于规则匹配的函数。这些函数本身是被高度优化的。



> 
> 
总的运行目标是，在真实环境运行时，WAF做阻止/通过决定的时间的中位数小于1ms。



> 
> 
WAF运行的优化，来自于用一个有行级时序信息的测试工具测试WAF的性能，来自于在带有非常详细的基于systemtap的工具的cloudflare的网络中运行WAF。



> 
> 
为了在测试环境得到行级时序信息，我们写了一个小的行级的代码分析器，在我们运行一个请求测试集时使用。这个分析器，叫做lulip，是一个开源项目。它输出哪些行被调用的最频繁，哪些行消耗了最多的执行时间这些信息。



> 
> 
例如，这儿是一个简化的输出的版本：
file:line     count   elapsed (ms)   line
wr.lua:1129      2         822.455   hash = ngx_sha1_bin(value)
wr.lua:1172    428         470.849   captures, err = ngx_re_match(v, p)
wr.lua:1197   3762         207.487   x = string_find(v, f)
wr.lua:212     157         154.386   string_gsub(v, "//([^/]+)//", "%1")
wr.lua:1196   3788          87.475   for i=1,g() do
wr.lua:1158   1563          52.906   if not f() then



> 
> 
它显示了一个ngx_sha1_bin（实际上是一个对ngx.shal_bin函数的本地的的引用）被调用了2次，但是花费了823毫秒。第二个最耗时的行是第1172行，总共花了471号码，被调用了428次。使用这些细节信息，我们能够优化指定的热点代码。



> 
> 
来自systemtap或者堆栈跟踪的信息反馈到我们自己的收录引擎，分析它并自动生成火焰图，这展示了代码在哪里运行。 将鼠标停留在任何部分，将给出任何部分的代码花费时间的百分比。



![](https://img-blog.csdn.net/20140703103751421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


> 
在早期的WAF的优化，火焰图快速显示闭包的广泛使用导致缓慢，由于它们的花费在LuaJIT里。编译器的修改移除了它们的使用。


> 
从相同的信息生成的另一种视图显示一个函数中花费（忽略它本身调用的任何函数）的总时间。这使得能快速识别热点函数。在这里，可以很容易地看到，正则表达式处理和串匹配是最昂贵的操作（这并不奇怪，因为这就是WAF做什么，主要是）。


![](https://img-blog.csdn.net/20140703103626781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


> 
检查这些跟踪信息，我们决定改善LuaJIT开源项目是值得赞助的。


> 
优化WAF最关键的是两件事：可重复的测试数据和工具检查运行的代码。火焰图和lulip的结合意味着通过精确的检查时间花在哪里可以让WAF性能有一个巨大的飞跃。规则编译器的使用意味着优化可以迅速的应用到所有需要的规则上。不用去猜测哪里是慢的：测量它！！


> 
生成的代码大量使用局部变量（其中有许多是由编译器自动生成）和内存化。我们还使用Lua 字节码的nginx Lua的缓存，以加快自定义规则的加载，一个两阶段的内存缓存，它使用lua_shared_dict和memcached作额外的加载加速。而我们的全球分布式数据存储意味着新的规则可以在几秒钟内铺开。


> 
最后，衡量WAF在生产环境的运作，我们有一个全球的指标体系，收集cloudflare 网络所有部分的指标，这儿是一个图标显示了WAF几个小时的运行。它显示了处理一个请求的平均时间在毫秒内。这个waf运行每个请求花费380us到480us之间，大大优于1ms的目标。


![](https://img-blog.csdn.net/20140703103653343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


> 
随着语言上各种优化，编译器和WAF核心表明我们拥有了一个非常快的纯Lua waf，这给了我们很大的灵活性，并且运行在nginx核心里。这是又一个项目指出了Lua成为了一门极其优秀的嵌入式语言，在Lua里，我们可以写Cloudflare需要的各种可扩展的逻辑。

原文链接： http://blog.cloudflare.com/cloudflares-new-waf-compiling-to-lua   膜拜前淘宝大神 章亦春



> 
> 
                                                                                                                           -------- translate by 囧囧有神(814329735@qq.com)







