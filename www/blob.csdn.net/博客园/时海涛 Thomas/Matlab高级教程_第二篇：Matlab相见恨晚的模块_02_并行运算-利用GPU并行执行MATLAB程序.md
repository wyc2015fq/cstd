# Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_并行运算-利用GPU并行执行MATLAB程序 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：Matlab相见恨晚的模块_02_并行运算-利用GPU并行执行MATLAB程序](https://www.cnblogs.com/noah0532/p/8646376.html)





1　　MATLAB原文：

　　如果所有你想使用的函数支持GPU，你能够使用gpuArray把输入的数据传输到GPU，也能够唤起gather命令把传输值GPU的数据回收。

2　　通过**gpuDevice**命令观察当前电脑的GPU设备

```
>> gpuDevice
ans = 
  CUDADevice (具有属性):

                      Name: 'GeForce GT 430'  % GPU设备的型号
                     Index: 1　　% 当前GPU设备的编号
         ComputeCapability: '2.1' % 计算能力
            SupportsDouble: 1　　%知否支持双精度运算
             DriverVersion: 8　　% Cude驱动版本
            ToolkitVersion: 7.5000　　% 工具版本
        MaxThreadsPerBlock: 1024　　% 每个Block的最大线程数目
          MaxShmemPerBlock: 49152　　% 每个Block可用的最大shared内容
        MaxThreadBlockSize: [1024 1024 64]　　%单个Block支持x，y，z三个方向的最大值
               MaxGridSize: [65535 65535 65535]　　%最大的grid大小
                 SIMDWidth: 32　　%Warp大小
               TotalMemory: 1.0737e+09　　%GPU设备全部内存大小
           AvailableMemory: 799592448　　%GPU设备可分配内存大小
       MultiprocessorCount: 2　　%GPU设备处理器个数（同CPU处理器个数相同，这是双核GPU）
              ClockRateKHz: 1500000　　% 时钟频率多少赫兹
               ComputeMode: 'Default'　　%计算模式
      GPUOverlapsTransfers: 1　　
    KernelExecutionTimeout: 1
          CanMapHostMemory: 1
           DeviceSupported: 1　　%本机MATLAB支持的GPU设备个数
            DeviceSelected: 1　　%当前选择GPU设备编号。
```

3　　通过**gpuDevice(index)编号选择第几个GPU处理器进行数值运算。**

4　　**创建GPU数值阵列（最简单的一种复制和提取应用）**

　　通过**gpuArray**函数完成，通过**gather**函数回收运算或复制的GPU数据：

x = rand(3,3);
B = gpuArray(x);
whos B
% whos B  % 返回GPU赋值后的对象
%   Name      Size            Bytes  Class       Attributes
% 
%   B         3x3                 4  gpuArray   

X1 = gather(B);  % 把GPU中的值在收回来，赋值一个变量
x - X1; 
% x - X1 % 结果为0，表示过程正确
% ans =
%      0     0     0
%      0     0     0
%      0     0     0


5　　对应的创建GPU数值阵列有一些其他的GPU函数，与常用的MATLAB函数一样，只不过加上gpuArray字符说明，就可以转换成GPU数值阵列。常用的这些函数有两种方式，而且可以函数名来作为定放的位置，分别如下：

**eye(___,'gpuArray')	rand(___,'gpuArray')false(___,'gpuArray')	randi(___,'gpuArray')Inf(___,'gpuArray')	randn(___,'gpuArray')NaN(___,'gpuArray')	gpuArray.colon(值)ones(___,'gpuArray')	gpuArray.freqspace（值）true(___,'gpuArray')	gpuArray.linspace（值）zeros(___,'gpuArray')	gpuArray.logspace（值） 	                gpuArray.speye（值）**

6　　其他的创建GPU数值阵列的函数可以用帮助的方式取查看

**help gpuArray.methodname(methodname就是想要查看的函数)**

7　　还有操作GPU数据的函数常用的有：

**classUnderlying(___,'gpuArray')     gupArray.classUnderlying(值) % gpu数值阵列数据元素类型isreal(___,'gpuArray')      gupArray.isreal(值) % 判断gpu数值阵列数据元素是否为实数length(___,'gpuArray')     gupArray.length(值) %gpu数值阵列最后一维的数据长度ndims(___,'gpuArray')     gupArray.ndims(值) %gpu数值阵列的尾数size(___,'gpuArray')     gupArray.size(值) %gpu数值阵列各维大小**

8　　常用的还有这些

![](https://images2018.cnblogs.com/blog/1328368/201803/1328368-20180325200356150-89090097.png)

9　　其实GPU变得很简单，就是赋值计算，取回。














