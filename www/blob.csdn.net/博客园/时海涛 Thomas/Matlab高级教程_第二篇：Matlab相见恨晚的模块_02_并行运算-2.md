# Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_并行运算-2 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_并行运算-2](https://www.cnblogs.com/noah0532/p/8642715.html)





1　　MATLAB并行计算-从个人桌面到远程集群和云(陈伟/魏奋)视频摘录笔记

https://cn.mathworks.com/videos/parallel-computing-with-matlab-92865.html

2　　数据、硬件和算法，MATLAB发展的方向

3　　MATLAB并行计算工具的介绍

内嵌多线程(隐式)

--MATLAB 内核函数和图像处理工具箱

--矩阵操作(linear algebra,fft,filter,etc)

--无须代码修改

并行计算产品（显式）

--Parallel Computing Toolbox 并行计算工具箱

--MATLAB Distributed Computing Server 分布式计算机服务

--MATLAB用户多种控制方式

4　　MATLAB并行计算实现：串行到并行

5　　如何用串行代码换成并行代码

6　　PCT，支持本地最大12个核，12个worker，支持GPU

7　　MBCS 计算机集群

8　　CPU并行编程实现：无需代码修改

　　-- 工具箱内部函数支持

　　--上层编程解构

　　parfor ， batch，distributed

　　-- 底层编程解构

　　createJob，labSend

　　parfor当中不能parfor中再parfor

　　可以用meshgrid函数实现向量化

9　　分布式数据和SPMD

--在计算节点上处理各个数据片段

--通过消息传到实现节点间通讯

--提供MATLAB串行和并行代码的单一编程环境

x = 1

spmd

　　y = x+1;

end

y

10　　GPU，图形加速卡

--内建函数，利用GPU数组，调用，推送到gpu中

　　gpuarray

--运行客户自动以的使用GPU数组的函数

　　arrayfun

-- 调用一致CUDA代码和PTX文件创建运行内核函数

--parallel.gpu.cudakernal














