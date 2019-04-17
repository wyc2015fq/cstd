# Matlab Delete Row or Col 删除矩阵的行或列 - Grandyang - 博客园







# [Matlab Delete Row or Col 删除矩阵的行或列](https://www.cnblogs.com/grandyang/p/4876322.html)







Matlab中，我们有时候要删除矩阵中的某行某列，可以采用下列方法进行删除：



```
a = [
1 2 3
4 5 6
7 8 9];

a(2,:) = []; % Delete row 2
a(:,2) = []; % Delete col 2
```














