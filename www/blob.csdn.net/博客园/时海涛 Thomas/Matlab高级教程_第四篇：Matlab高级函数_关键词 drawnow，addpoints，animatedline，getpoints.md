# Matlab高级教程_第四篇：Matlab高级函数_关键词:drawnow，addpoints，animatedline，getpoints - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第四篇：Matlab高级函数_关键词:drawnow，addpoints，animatedline，getpoints](https://www.cnblogs.com/noah0532/p/9917746.html)





0. MATLAB真实航母基本的工具，其中的函数/工具不计其数，而且有些函数/工具非常的炫酷。在MATLAB第四篇章把平时工作中用到的些许函数进行使用的讲解



主题1.：drawnow

解释：更新图窗并处理回调

说明：drawnow更新图窗并处理任何挂起的回调。如果您修改图形对象并需要在屏幕上立即查看此次更新，请使用该命令。这就是把图形更新的过程进行动态的流数据的动画。

后面有几个方法：

方法1.limitrate，更新数量限制为每秒20帧。如果上次更新后不到50毫秒，则会丢弃新的更新

方法2：nocallbacks，会延迟回调，知道下一个完整的命令。

方法3：limitrate nocallbacks降序局更新限制为每秒20帧

方法4：update会跳过更新并延迟回调（不建议）

方法5：expose，更新窗口延迟（不建议）



主题2：addpoints

解释：向动画线条中添加点

说明：

addpoints（an,x,y） % 二维的

addpoints(an,x,y,z) % 三维的，

an：指定动画线条中添加x和y的点



主题3：animatedline

解释：创建动画线条

说明：


`an = animatedline`

`an = animatedline(x,y)`

`an = animatedline(x,y,z)`



`an = animatedline(___,Name,Value)`

`an = animatedline(ax,___)`




因此：主题1，主题2，主题3可以结合起来组成一个使用

% 首先创建动画线条
h = animatedline('LineStyle','-.','LineWidth',0.01,'Marker','>','Color','r');
% 确定x，y
x = linspace(0,4*pi,10000);
y = cos(x);
% 进行循环画图上的点
for k = 1:10000
    addpoints(h,x(k),y(k));
    %动画
     drawnow update % 快速更新前面的点
end
drawnow


% 可以标注时间段的方式
% 首先创建动画线条
h = animatedline('LineStyle','-.','LineWidth',0.01,'Marker','>','Color','r');
% 确定x，y
x = linspace(0,4*pi,10000);
y = cos(x);
a = tic; % start timer
% 进行循环画图上的点
for k = 1:10000
    addpoints(h,x(k),y(k))
    b = toc(a); % check timer
    if b > (1/10)
        drawnow % update screen every 1/30 seconds
        a = tic; % reset timer after updating
    end
end
drawnow


![](https://img2018.cnblogs.com/blog/1328368/201811/1328368-20181106202019901-89592212.png)



好有意思的动态图示。其实还有一些常用的。后面有机会再说



主题4：gepoints（h）

获取返回线条中的点

h = animatedline(1:10,1:10);
[x,y] = getpoints(h)
x = 1×10

     1     2     3     4     5     6     7     8     9    10

y = 1×10

     1     2     3     4     5     6     7     8     9    10












