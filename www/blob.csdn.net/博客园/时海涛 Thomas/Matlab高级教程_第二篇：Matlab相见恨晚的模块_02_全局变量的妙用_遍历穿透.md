# Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_全局变量的妙用_遍历穿透 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_全局变量的妙用_遍历穿透](https://www.cnblogs.com/noah0532/p/8651133.html)





1　　比如我这边写了一个函数，这个函数中有一个变量作为参数，给定这个参数一个值，然后这个函数返回给我一个值。但是，我写这函数的时候，这个传参我不写到函数里面。可以通过全局变量的方式进行在外部穿透遍历。

2　　举例如下：

比如有这么一个函数

function [a] = sample
    global b;
%     b = 0;
    a = b;
end

%% 里面的b的变量我不进行命名，通过全局话，同行方式。

在函数外部进行遍历：

global b;
result = [];
for i = 1:100
    b = i;
    a = sample;
    result(i) = a;
end

%% 这样这个b在外面通过全局的方式就会传到进这个函数体内














