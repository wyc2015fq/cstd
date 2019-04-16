# matlab实现线性回归成绩预测 - 彩虹糖的博客 - CSDN博客





2018年09月29日 23:33:34[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：1121








# 目的

1. 熟悉matlab基本语法。

2. 使用matlab进行绘图。

3. 复习线性回归于梯度下降。

# 数据集

与之前的文章，[朴素贝叶斯实现成绩等级分类](https://blog.csdn.net/caozixuan98724/article/details/77990081)相同，也是某市一模考试成绩（只保留了语文英语数学和总分）

# 假设函数

![](https://img-blog.csdn.net/20180929230539579?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

等号的左侧代表预测的成绩，左侧(θ0，θ1，θ2，θ3)代表参数，考虑到成绩的实际情况，可以将θ0定义为0，因此要求的参数列表为(θ1，θ2，θ3)。x1,x2,x3分别代表语文成绩，英语成绩和数学成绩。

# 损失函数

![](https://img-blog.csdn.net/20180929231102804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 使用梯度下降算法进行迭代

![](https://img-blog.csdn.net/20180929231230136?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# matlab代码

gradient_descent.m

```cpp
[num,grades_matrix,row] = xlsread('d:/mlGrades/1.xls',1,'A2:D4000');
grades_matrix = str2double(grades_matrix);
parameters = [0,0,0];
step = 0.0000003;
points = [];
for i = 1:1500
    grades_total = grades_matrix(i,:);
    chinese = grades_total(1);
    english = grades_total(2);
    math = grades_total(3);
    total = grades_total(4);
    grades = [chinese english math];
    parameters = parameters - step*(parameters*grades'-total).*grades;
    total_predict = parameters*grades';
    points(end+1)=total - total_predict;
end
plot(points)
```

这里面的代码简单解释一下，xlsread的返回有三个值，num代表excel文件中的数字矩阵，grades_matrix代表excel中的文字矩阵，row代表当前行。

这里面出现的问题是无论我怎么设置，xlsread这个函数都会把表中的数字识别为字符串，因此需要str2double把字符串矩阵转化为向量矩阵。

在matlab中，*代表矩阵正常的相乘，.*代表矩阵元素与元素直接相乘，grades'代表grades矩阵的转置。

plot函数用于绘图。

# 结果

![](https://img-blog.csdn.net/20180929232819438?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到经过大约五百次左右的迭代后，实际值与预测值的差值在0左右波动，达到了最终的目的。

结论

最后得到的三个参数的值分别为1.5460（语文），1.5583（英语），1.8500（数学）。

这说明了什么呢？对于理科生来说，数学当然是最最最最重要的啦٩(๑>◡<๑)۶ 

代码和数据集稍后整理发布到github上去。



