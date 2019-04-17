# MatLab Mark Points 给点标序号 - Grandyang - 博客园







# [MatLab Mark Points 给点标序号](https://www.cnblogs.com/grandyang/p/4714495.html)







在MatLab中，我们有时要给画出的点按一定顺序标上序号，下面这个简单的例子实现这个功能：



```
x=rand(1,10);
y=rand(1,10);
plot(x,y,'r*');
text(x+0.02,y,num2cell(1:10));
```














