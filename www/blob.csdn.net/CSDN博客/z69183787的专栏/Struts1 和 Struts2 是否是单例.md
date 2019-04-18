# Struts1 和 Struts2 是否是单例 - z69183787的专栏 - CSDN博客
2014年04月08日 15:54:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4463
### struts 2的Action是多实例的并非单例，也就是每次请求产生一个Action的对象。原因是：struts 2的Action中包含数据，例如你在页面填写的数据就会包含在Action的成员变量里面。如果Action是单实例的话，这些数据在[多线程](http://wenwen.soso.com/z/Search.e?sp=S%E5%A4%9A%E7%BA%BF%E7%A8%8B&ch=w.search.yjjlink&cid=w.search.yjjlink)的环境下就会相互影响，例如造成别人填写的数据被你看到了；
### struts 1的Action是单实例的，因为它的数据保存在Form类中，多线程环境下，Action只负责处理一些逻辑，并没有数据，也就是大家把它当做一个工具使用。
按照单例的编码规则，Servlet本身只是一个Java，结构并不是单例结构。只是Web容器在维护这些Servlet的时候只给创建一个实例存在JVM中，用户请求服务时，服务器只调用它已经实例化好的Servlet对象来处理请求。所以，告诉你的是Servlet并不是单例，只是容器让它只实例化一次，变现出来的是单例的效果而已。但需要注意的是，Servlet是[web容器](http://zhidao.baidu.com/search?word=web%E5%AE%B9%E5%99%A8&fr=qb_search_exp&ie=utf8)来控制实例化的，并不是你自己用你编写的代码来实例化，即使你自己编写代码实例化你的servlet，Web服务器也不会直接调用你的实例化的Servlet对象的。
struts1 中actioin 是单例，即所有session对应一个action
由spring来创建action时，默认处理，为单例（既然是单例，即使你设置成了多例，也不影响程序功能）
struts2 中action 是多例，即一个session产生一个action。
由spring来创建action时，须设置为prototype的，不能是singleton。
个人感觉，之所以struts2中action必须是多例的，主要原因在与请求数据和返回数据，大多都封装到了action中，用于实例变量，这样以来，如果将action设置成单例模式，则多个县城访问的时候，就会共享这些数据，从而引起数据混乱或者线程安全问题！
线程模式: 
" Struts1 Action是单例模式并且必须是线程安全的，因为仅有Action的一个实例来处理所有的请求。单例策略限制了Struts1 Action能作的事，并且要在开发时特别小心。Action资源必须是线程安全的或同步的。
" Struts2 Action对象为每一个请求产生一个实例，因此没有线程安全问题。（实际上，servlet容器给每个请求产生许多可丢弃的对象，并且不会导致性能和垃圾回收问题）
Action执行的控制： 
" Struts1支持每一个模块有单独的Request Processors（生命周期），但是模块中的所有Action必须共享相同的生命周期。 
" Struts2支持通过拦截器堆栈（Interceptor Stacks）为每一个Action创建不同的生命周期。堆栈能够根据需要和不同的Action一起使用。
