# matlab中的@函数 - wishchinYang的专栏 - CSDN博客
2014年08月20日 18:03:56[wishchin](https://me.csdn.net/wishchin)阅读数：1053
原文链接：[http://blog.sina.com.cn/s/blog_5e73a8fc0100t9yg.html](http://blog.sina.com.cn/s/blog_5e73a8fc0100t9yg.html)
这是个函数句柄 
@(x,y) 表示未知数是x和y
punct     - Function handlecreation          @
@ 在匿名函数中表示函数句柄
例如ln(x)，在matlab中是没有定义的，正确表示是log(x);
但如果要直观表示自然对数，意义用以下语句表示：
ln=@(x)  log(x);
执行后，ln(4)=log(4) , 即用ln 替换 log。
以上表示可能无法看出‘@’的好处，再看下例：
ploy6 = @(x)[8*x.^6+6*x.^5+3*x.^3+x.^2+x+520];
fplot(ploy6,[0,100]);%画ploy6的图，其中x从0到100
fzero(ploy6,13);%判断x=13是否为ploy6的零点
在这种长且多次调用的情况下，用函数句柄就可以方便很多。
后记：关于各种语言的语法;
        为何不能统一各种语言的语法呢？
        就像各种自然语言一样，也许创造出来，就是为了防碍交流！
