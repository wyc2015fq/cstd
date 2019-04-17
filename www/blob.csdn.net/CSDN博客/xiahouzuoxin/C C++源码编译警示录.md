# C/C++源码编译警示录 - xiahouzuoxin - CSDN博客





2013年08月09日 15:19:47[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4086








        近几年，国内开源越闹越火，不仅软件开源，硬件也开源。去[开源中国](http://www.oschina.net/)看看，已经有专门的硬件开源专区，[61ic代码工厂](http://www.61ic.com/code/index.php)提供了许多的硬件（ARM、DSP等）相关的代码。在这种趋势下，从头开始写一个软件或算法已经是上个世纪的事了，我们多多少少会从开源代码中借鉴点经验，甚至好的算法或中间件都会直接拿来用，当然，工程需要付费的就需要掏money了，不过提供开源免费代码的网站却还真不少，最有名的有[sourceforge](http://www.sf.net/)，[github](http://www.github.com/)。我常常从github上下载别人的代码，尽管github中的代码良莠不齐，但长尾理论的告诉我们：除了Linux这种牛逼排名靠前的优秀源代码外，那些star较低的Repositories中也有许多好的可供我们学习甚至可以直接在工程中使用的代码，但需要我们去发现。

        哎哟，扯远了，回到正题……因为我们现在有太多可用的代码，我们不用从头编写，而要做的大部分工作都是移植（更专业点的叫重构），这里将持续地将自己移植工程（主要是C/C++工程）中遇到的问题及解决过程记录下来，“掘地三尺非一日之功”，这是一个积累的过程。




1. 出现大量的 link error #10056: symbol "_fann_set_scaling_params" redefined

【情况】

在链接一个神经网络的C语言库fann时，出现大量的类似上面redefined错误，错误原因很明显，“_fann_set_scaling_params重复定义”，而我在工程里找遍了，发先确实都只在一处有定义。

【解释】

_fann_set_scaling_params是编译后.obj文件中的标识符号（symbol，一般这些符号开头带下划线），以上错误出现在链接的时候，这说明多个.obj文件中出现了_fann_set_scaling_params的定义。我们知道，C编译器是根据.c文件编译的（即一个.c文件生成一个.obj，c编译器不会主动去编译头文件）。而fann库有所不同，其中一个floatfann.c文件中有这样几行代码：



```cpp
#include "fann.c"
#include "fann_io.c"
#include "fann_train.c"
#include "fann_train_data.c"
#include "fann_error.c"
#include "fann_cascade.c"
```

你没看错，c语言中也是可以包含.c文件的，包含的.c文件会当做共成文件也编译成.obj文件。这就明白了，我将上面包含的几个.c文件添加到了工程里面，而这里又包含了这几个.c文件，同一个.c文件编译了两次，当然会出现重复定义了。



【解决办法】

第一种方法，将上面使用include包含的.c文件从工程中移除，第二种方法，将上面的#include "**.c"注释掉，相应的源文件添加到工程中。

**注：**这是第一个问题，记得之前遇到过很多次类似的问题，每次都迷迷糊糊的弄了好久才解决，甚至程序编译通过后还迷迷糊糊不知道怎么就跑通了，真后悔没早作记录。这次在往DSP上移植fann时又遇到这个问题，谨记。

2. fatal error #5: could not open source file "fann.h"

【解释】


相对于上面的编译错误，这是链接错误，意思是说fann.h头文件打不开，要么压根就没有这个头文件（检查下头文件名有没有错误），要么就是头文件路径设置不对（编译器找到不头文件）。

【解决方法】

在确定有这个头文件的情况下，eclipse C/C++中解决问题的方法是，工程右键->Properities->Build->Compiler->Include Options，在-I选项中添加头文件所在目录。关于这一点，用过Linux下gcc编译器的就会对头文件搜索路径，库搜索路径这些内容特别敏感，因此，这里强烈推荐做软件（尤其是C/C++软件）的要学会gcc的使用，这对学会构建工程非常有用。




3. warning:  #231-D: declaration is not visible outside of function




【解释】


没有包含相关的头文件，将相关头文件包含就OK了







待增加中……











