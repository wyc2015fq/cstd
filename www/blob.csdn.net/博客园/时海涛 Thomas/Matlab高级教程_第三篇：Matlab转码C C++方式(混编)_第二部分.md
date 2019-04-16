# Matlab高级教程_第三篇：Matlab转码C/C++方式(混编)_第二部分 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第三篇：Matlab转码C/C++方式(混编)_第二部分](https://www.cnblogs.com/noah0532/p/9511349.html)





　　这一部分通过一些实例来进行转码和调试的讲解：

**1. 输入变量、输出变量和过程内变量的内存预分配**

函数代码：函数名test

```
function [A,B] = test( mark,num,array )%#codegen
%输入参数中，mark、num是标量，array是向量  
a=ones(1,5);  
b=zeros(1,5);  
% 为A B预分配个内存大小  
**A****=ones(1,5);  % 输出变量预分配大小B=zeros(1,5);  % 输出变量预分配大小    **if mark==0  
        for i=1:num  
             if i==1  
                 A=a;  
                 B=b;  
             else  
                 A=[A,a];  
                 B=[B,b];     
             end  
        end  
    else  
        for i=1:num  
             if i==1  
                 A=a;  
                 B=b;  
             else  
                 A=[A,array];  
                 B=[B,array];     
             end  
        end  
    end  
end
```

测试脚本：main

[A,B]=test( 3,4,[1 2 5 6 8] )



输入变量内存大小分配上面，最后一个为向量先固定为和设定为inf

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821143339068-1715085680.png)

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821143916769-231950006.png)

可以看到内存是可以预分配长短的



**2. 生成代码和相关报告内容**

生成相关文件：

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821144102627-298827298.png)

对应MATLAB当中生成的文件

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821144136751-1382213119.png)

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821144207563-1061688630.png)

比较乱还有比较多，包括在lib静态链接库的文件内容，codegen解码内容，测试样本等等。

我们还是打开MATLAB coder最终生成的报告：

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821144334988-1888910614.png)

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821144406321-483368008.png)

我们看到在报告生成文件列表当中，只包含了头文件.h和源文件.c格式的文件

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821144453121-482504140.png)

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821144718122-1432921319.png)

和标准演示在VS当中就是少了main这个演示文档文件，如果要在VS里面调试要把这个演示脚本拿回来














