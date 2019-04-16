# Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_并行运算-关于parfor的问题 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_并行运算-关于parfor的问题](https://www.cnblogs.com/noah0532/p/8648317.html)





1　　我们知道MATLAB支持并行运行一般有四种模式：

**第一种模式**：本机调用多核CPU性能并行运算

**第二种模式**：本机调用GPU性能并行运算

**第三种模式**：多计算机组成集群，调用集群CPU性能并行运算

**第四种模式**：运用MATLAB提供的云计算服务。

2　　**在第一种并行运算模式中**：

　　标准模式为：

**　　　　　　第一步：打开并行池parpool**

**　　　　　　第二步：parfor等相关PCT，并行计算工具箱中函数**

**　　　　　　第三步：关闭并行池**

3　　第一步：打开并行池的方式：

　　1）p = parpool('local',2);   % 在知道有几个本地核的情况下，选择调用几个核进行并行运算。

　　2）parpool; % 这是直接打开并行池的方式。

　　备注：建议采用第一种，方便关闭和更改参数。



**第二步：parfor函数的问题**：

　　1）很多简单的低纬度运算，建议只需要用普通函数和关键字即可，用parfor函数进行运算并不能提升性能。

　　2）MATLAB利用多核心的计算能力来加速计算时非常有必要的，因为目前所有的电脑最少都双核心了。

　　3）循环并行使用parfor循环代替for循环，但是很多时候parfor很不好用，当parfor循环体内的代码比较多时，非常不容易满足循环条件而失败。因此我们首先要分析代码，另一种方式用profiler打开代码分析器来分析代码那一部分耗时较大，进行代码分析。

　　3）**通用格式**：

　　　  parfor ss = 1:100

　　　  XXX

　　　  ......

　　     ......

　　　  end

　　4）**通用并行计算模板**：其基本思路就是将循环体内代码打包为函数！

**　　　　parfor ss=1:100**
**func(ss,...,...);**
**　　　　end**

　　　　函数如下所示：
**　　　　function func(ss,...,...)**
**　　　　xxx**
**　　　　.....**
**　　　　.....**
**　　　　end**

　　备注：该函数不应该有返回值，当parfor循环体内计算结果需要返回主函数时(绝大多数情况是这样的)，那么将结果在循环体内保存为文件，文件名与循环变量ss有关。当parfor的并行运行完之后，将保存的临时文件读取进来即可，保存和读取计算结果相比于大型的计算耗时来讲应该小很多，从而体现出并行的优势。所提供的该方法可以用于任何一维、二维或者多维循环的加速计算，采用这种方法，规避了学习parofr规则的所有难点，非常具有通用性。

　　第三步：删除并行池参数关闭

　　delete(p)



**举个例子**：比如有这么一段并行计算代码,其中绿色标注的位置为循环体。

p = parpool('local',2);
tic
n = 200;
A = 500;
a = zeros(n);
parfor i = 1:n
    **a(i) = max(abs(eig(rand(A))));**
end
toc
delete(p)

　　当然这个循环体内可以有不同的计算队列，我们把这个队列封装成为一个函数，如下：

function [a] = sample(A)
a = max(abs(eig(rand(A))));
end


　　然后再用parpool调用，如下：

p = parpool('local',2);
clear;
tic
n = 200;
A = 500;
a = zeros(n);
parfor i = 1:n
**    b = sample(A);    a(i) = b;**
end
clear b;
toc
delete(p);
时间已过 28.838599 秒。

　　对比原先的代码：

tic
n = 200;
A = 500;
a = zeros(n);
for i = 1:n
    a(i) = max(abs(eig(rand(A))));
end
toc
时间已过 42.570194 秒。



4　　MATLAB并行计算的提升有限和MATLAB语言的效率有关，并且和处理的数据量和计算复杂度有关。














