# matlab 矩阵按列按行归一化 - 家家的专栏 - CSDN博客





2013年12月17日 15:53:33[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：24549









# 矩阵按列按行归一化到L2范数的原理和最精简Matlab代码

    在模式识别和机器学习的数据预处理过程中，对数据集按行或者按列进行L2范数归一化是一种常见的归一化方式，因此本文将介绍对向量进行L2范数归一化的原理和方法，并给出相关的Matlab源代码，供后学者作为基础知识参考使用。





![](http://image.sciencenet.cn/album/201301/20/2113219tbr4cnzb8n7pynt.png)


    由此，我们可以很块的写出最简单的matlab源代码如下：

    首先按行归一化：

% Examples

A=[3 4;5 12];

[m n] = size(A);

% normalize each row to unit

for i = 1:m

    A(i,:)=A(i,:)/norm(A(i,:));

end




    按列归一化：

% normalize each column to unit

A=[3 4;5 12];

for i = 1:n

    A(:,i)=A(:,i)/norm(A(:,i));

end



    然而，上述代码最能实现功能，但并不是最优的，它只是一种对该过程的最佳理解代码。在Matlab中，for循环是一件非常费时间的结构，因此我们在代码中应该尽量少用for循环。由此，我们可以用repmat命令得到另一种更加简洁更加快速的代码，只是这种代码对于初学者理解起来比较费劲。可以看错是自己水平的一种进阶吧。

%  normalize each row to unit

A = A./repmat(sqrt(sum(A.^2,2)),1,size(A,2));

%  normalize each column to unit

A = A./repmat(sqrt(sum(A.^2,1)),size(A,1),1);




转自：[http://blog.sciencenet.cn/blog-810210-655011.html](http://blog.sciencenet.cn/blog-810210-655011.html)








