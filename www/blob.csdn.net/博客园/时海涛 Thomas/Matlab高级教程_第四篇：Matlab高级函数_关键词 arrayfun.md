# Matlab高级教程_第四篇：Matlab高级函数_关键词:arrayfun - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第四篇：Matlab高级函数_关键词:arrayfun](https://www.cnblogs.com/noah0532/p/9923661.html)





% 定义一个句柄并用这个句柄求值
h = @sin; 
h(3)
% % 命令窗口返回内容
% h(3)
% ans =
%     0.1411


%定义一个完整句柄，@参数+表达式的形式，并给句柄传参
h1 = @(x)mean(x);
x = [1 2 3 4 5];
h1(x)
% % 命令窗口返回内容
% h1(x)
% ans =
%      3
     

%arrayfun的简单应用
h1 = @(x)mean(x);
x = [1 2 3 4 5];
% 可以用句柄的形式传入第一个形参位置，也可以直接@句柄内容，这两个是等价的
arrayfun(@(x)mean(x),x)
arrayfun(h1,x)
% % 命令窗口返回内容
% ans =
%      1     2     3     4     5
% ans =
%      1     2     3     4     5


%arrayfun对Name，value的要求(只举例格式化形式'UniformOutput')
x = [1 2 3 4 5];
% 如果选择为false以元胞的形式返回值
arrayfun(@(x)mean(x),x,'UniformOutput',false)  
% 如果选择为true以数组的形式返回值
arrayfun(@(x)mean(x),x,'UniformOutput',true)  
% % 命令窗口返回内容
% ans =
%   1×5 cell 数组
%     {[1]}    {[2]}    {[3]}    {[4]}    {[5]}
% ans =
%      1     2     3     4     5

% arrayfun对于提升运算效率的帮助
% 不采用arrayfun形式的运算
tic;
x = magic(3000)*magic(3000);
toc;
% 时间已过 3.125284 秒。

% 采用arrayfun形式的运算
tic;
arrayfun(@(n)magic(n)*magic(n),3000,'UniformOutput',false);
toc;
% 时间已过 3.125284 秒。




我们分采用运算的时候，第二遍比第一遍快，主要是MATLAB的内存预分配机制。

注意采用arrayfun值是标量形式必须选择false

arrayfun主要是为了打开并行运算库而设计

我们打开并行运算

% arrayfun对于提升运算效率的帮助
% 不采用arrayfun形式的运算
tic;
x = magic(5000)*magic(5000);
toc;
% 时间已过 14.077466 秒。

% 采用arrayfun形式的运算
tic;
arrayfun(@(n)magic(n)*magic(n),5000,'UniformOutput',false);
toc;
% 时间已过 13.423996 秒。


我们发现运算效率提升不太明显，因为arrayfun针对标量的运算方式，如果对于像魔方矩阵，可以用meshgrid分解的方式提升更明显　












