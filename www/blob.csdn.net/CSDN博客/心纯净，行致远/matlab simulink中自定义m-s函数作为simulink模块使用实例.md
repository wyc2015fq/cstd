# matlab/simulink中自定义m-s函数作为simulink模块使用实例 - 心纯净，行致远 - CSDN博客





2018年03月09日 22:00:47[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：8488








    一般而言matlab/simulink能满足多数使用要求，但是往往在研究中会使用User-difinition s-function或者调用c,c++的库函数，这时候就需要matlab能进行c，c++以及fortran语言的编译。

    本文是我第一次参考资料，利用m语言编写s函数的实例。User-Definition的s-fun可以作为一个simulink模块使用，这样在工程和科研中就可以自己开发需要的模块，并入simulink仿真之中，不必受制于simulink自带的有限模块。**使用m语言写s-fun不需要mingw编译器。**

题目：编写User-Definition的s-fun，使得正弦函数的波形加倍20倍，然后输出到scope。

实现步骤：

1. 新建simulink模型，拖入User-Definition的s-fun，然后双击s-fun，点击edit，进入编辑模块；

![](https://img-blog.csdn.net/2018030921572926)


2. 编写m-sfun代码，并另存为time20.m，可以参照m-sfun的格式，具体代码如下：

```
function [sys,x0,str,ts]=timestwo(t,x,u,flag)
%   t是采样时间
%   x是状态变量
%   u是该simulink模块的输入
%   flag是仿真过程的状态标志，用来判断当前状态是初始化、执行、输出等
%   sys是flag的函数，不同的标志执行不同的sys
%   x0是状态变量的初始值
%   str是保留参数，一般在初始化置空
%   ts是一个1*2的向量，其中ts(1)是采样周期，ts(2)是偏移量
%   本s-fun主要就是调用initialize和output函数

switch flag,
    
%Initialization，flag=0时，具体初始化函数参照35行
  case 0         
    [sys,x0,str,ts] = mdlInitializeSizes;

%Calculate  outputs
  case 3
    sys = mdlOutputs(t,x,u);

%Unused flags
  case {1,4,2,9}
    sys = []; % do nothing

  otherwise
         error('Simulink:blocks:unhandledFlag', num2str(flag));
end
%
%=============================================================================
% mdlInitializeSizes
% Return the sizes, initial conditions, and sample times for the S-function.
%=============================================================================
%初始化函数是必不可少的，其中的结构体形式是固定不变的
function [sys,x0,str,ts] = mdlInitializeSizes

sizes = simsizes;
%用于设置模块参数的结构体用simsizes生成
sizes.NumContStates  = 0;
%模块连续状态变量的个数
sizes.NumDiscStates  = 0;
%模块离散状态变量的个数
sizes.NumOutputs     = 1;
%模块输出变量的个数
sizes.NumInputs      = 1;
%模块输入变量的个数
sizes.DirFeedthrough = 1;
%模块是否存在直通反馈，存在为1。直通的意思是输入能直接控制输出
sizes.NumSampleTimes = 1;
%模块的采样时间个数，至少是一个

sys = simsizes(sizes);  %设置完后赋给sys输出
str = [];   %   str是保留参数，一般在初始化置空
x0  = [];   %   x0是状态变量的初始值，显然初始值置空
ts  = [-1 0];   % sample time: [period, offset]

%start mdlOutputs
%当运行到mdlOutputs，就会输出20*u。
function sys = mdlOutputs(t,x,u)
sys=20*u;
% end mdlOutputs
```

3. 代码完成之后，将matlab工作目录更改为time20.m所在的文件夹，然后双击simulink模块，将time20填入s-function name，由于本例不需要输入参数，故其他不需要填写。

![](https://img-blog.csdn.net/20180309215506469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


4.  运行。

![](https://img-blog.csdn.net/20180309215945751)







