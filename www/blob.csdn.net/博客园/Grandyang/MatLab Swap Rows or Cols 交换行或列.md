# MatLab Swap Rows or Cols 交换行或列 - Grandyang - 博客园







# [MatLab Swap Rows or Cols 交换行或列](https://www.cnblogs.com/grandyang/p/4714834.html)







Matlab是矩阵运算的神器，所以可以很轻易的交换任意行或列，而且写法非常简洁，如下所示：



```
a = [1 2 3; 4 5 6; 7 8 9];

b = a;
b(:,[1;2]) = b(:,[2;1]); % Swap col 1 and 2

c = a;
c([1;2],:) = c([2;1],:); % Swap row 1 and 2
```














