# OpenCL总结 - 工作笔记 - CSDN博客





2016年03月19日 16:23:41[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8154








﻿﻿

**总体介绍**

**原文：[http://blog.csdn.net/leonwei/article/details/8880012](http://blog.csdn.net/leonwei/article/details/8880012)**

**1 异构计算、GPGPU与OpenCL**

  OpenCL是当前一个通用的由很多公司和组织共同发起的多CPU\GPU\其他芯片 异构计算（heterogeneous）的标准，它是跨平台的。旨在充分利用GPU强大的并行计算能力以及与CPU的协同工作，更高效的利用硬件高效的完成大规模的（尤其是并行度高的）计算。在过去利用GPU对图像渲染进行加速的技术非常成熟，但是我们知道GPU的芯片结构擅长大规模的并行计算（PC级的GPU可能就是CPU的上万倍），CPU则擅长逻辑控制，因此不只局限与图像渲染，人们希望将这种计算能力扩展到更多领域，所以这也被称为GPGPU（即通用处计算处理的GPU）。

    简单的说，我们的CPU并不适合计算，它是多指令单数据流（MISD）的体系结构，更加擅长的是做逻辑控制，而数据处理基本是单流水线的，所以我们的代码for(i=0;...;i++)这种在CPU上要重复迭代的跑很多遍，但是你的显卡GPU则不是这样，GPU是典型的单指令多数据（SIMD）的体系结构，它不擅长逻辑控制，但是确实天生的向量计算机器，对于for(i=0;...;i++)这样的代码有时只需要跑一遍，所以图形世界中那么多的顶点、片段才能快速的并行在显卡中渲染处理

![](https://img-blog.csdn.net/20130503144649880)



GPU的晶体管可以到几十亿个，而CPU通常只有几个亿，

![](https://img-blog.csdn.net/20130503144449782)

如上图是NVidia Femi100的结构，它有着大量的并行计算单元。

所以人们就想如何将更多的计算代码搬到GPU上，让他不知做rendering，而CPU只负责逻辑控制，这种一个CPU（控制单元）+几个GPU（有时可能再加几个CPU）（计算单元）的架构就是所谓的异构编程（heterogeneous），在这里面的GPU就是GPGPU。异构编程的前景和效率是非常振奋人心的，在很多领域，尤其是高并行度的计算中，效率提升的数量级不是几倍，而是百倍千倍。

   其实NVIDIA在很早就退出了利用其显卡的GPGPU计算 CUDA架构，当时的影响是很大的，将很多计算工作（科学计算、图像渲染、游戏）的问题提高了几个数量级的效率，记得那时NVIDIA来浙大介绍CUDA，演示了实时的ray tracing、大量刚体的互相碰撞等例子，还是激动了一下的，CUDA现在好像已经发展到了5.0，而且是NVDIA主力推的通用计算架构，但是CUDA最大的局限就是它只能使用N家自己的显卡，对于广大的A卡用户鞭长莫及。OpenCL则在之后应运而生，它由极大主流芯片商、操作系统、软件开发者、学术机构、中间件提供者等公司联合发起，它最初由Apple提出发起标准，随后Khronos
 Group成立工作组，协调这些公司共同维护这套通用的计算语言。Khronos Group听起来比较熟悉吧，图像绘制领域著名的软硬件接口API规范著名的OpenGL也是这个组织维护的，其实他们还维护了很多多媒体领域的规范，可能也是类似于Open***起名的（所以刚听到OpenCL的时候就在想它与OpenGl有啥关系），OpenCl没有一个特定的SDK，Khronos Group只是指定标准（你可以理解为他们定义头文件），而具体的implementation则是由不同参与公司来做，这样你会发现NVDIA将OpenCL做了实现后即成到它的CUDA
 SDK中，而AMD则将其实现后放在所谓是AMD APP （Accelerated Paral Processing）SDK中,而Intel也做了实现，所以目前的主流CPU和GPU都支持OpenCL架构，虽然不同公司做了不同的SDK，但是他们都遵照同样的OpenCL规范，也就是说原则上如果你用标准OpenCl头中定义的那些接口的话,使用NVIDIA的SDK编的程序可以跑在A家的显卡上的。但是不同的SDK会有针对他们芯片的特定扩展，这点类似于标砖OpenGL库和GL库扩展的关系。

  OpenGL的出现使得AMD在GPGPU领域终于迎头赶上的NVIDIA，但是NVIDIA虽为OpenCL的一员，但是他们似乎更加看重自己的独门武器CUDA，所以N家对OpenCL实现的扩展也要比AMD少，AMD由于同时做CPU和GPU，还有他们的APU，似乎对OpenCL更来劲一些。



**2.关于在GPU上写代码的那些事儿**

  OpenCL也是通过在GPU上写代码来加速，只不过他把CPU、GPU、其他什么芯片给统一封装了起来，更高了一层，对开发者也更友好。说到这里突然很想赘述一些在GPU上写代码的那些历史。。

  其实最开始显卡是不存在的，最早的图形处理是放在CPU上，后来发现可以再主板上放一个单独的芯片来加速图形绘制，那时还叫图像处理单元，直到NVIDIA把这东西做强做大，并且第一给它改了个NB的称呼，叫做GPU，也叫图像处理器，后来GPU就以比CPU高几倍的速度增长性能。

   开始的时候GPU不能编程，也叫固定管线的，就是把数据按照固定的通路走完

   和CPU同样作为计算处理器，顺理成章就出来了可编程的GPU，但是那时候想在GPU上编程可不是容易的事，你只能使用GPU汇编来写GPU程序，GPU汇编？听起来就是很高级的玩意儿，所以那时使用GPU绘制很多特殊效果的技能只掌握在少数图形工程师身上，这种方式叫可编程管线。

    很快这种桎桍被打破，GPU上的高级编程语言诞生，在当时更先进的一些显卡上（记忆中应该是3代显卡开始吧），像C一样的高级语言可以使程序员更加容易的往GPU写代码，这些语言代表有nvidia和微软一起创作的CG，微软的HLSL，openGl的GLSL等等，现在它们也通常被称为高级着色语言（Shading Language），这些shader目前已经被广泛应用于我们的各种游戏中。

   在使用shading language的过程中，一些科研人员发现很多非图形计算的问题（如数学、物理领域的并行计算）可以伪装成图形问题利用Shading Language实现在GPU上计算，而这结果是在CPU上跑速度的N倍，人们又有了新的想法，想着利用GPU这种性能去解决所有大量并行计算的问题（不只图形领域），这也叫做通用处理的GPU（GPGPU），很多人尝试这样做了，一段时间很多论文在写怎样怎样利用GPU算了哪个东东。。。但是这种工作都是伪装成图形处理的形式做的，还没有一种天然的语言来让我们在GPU上做通用计算。这时又是NVIDIA带来了革新，09年前后推出的GUDA架构，可以让开发者在他们的显卡上用高级语言编写通用计算程序，一时CUDA热了起来，直到现在N卡都印着大大的CUDA
 logo，不过它的局限就是硬件的限制。

  OpenCL则突破了硬件的壁垒，试图在所有支持的硬件上搭建起通用计算的协同平台，不管你是cpu还是gpu通通一视同仁，都能进行计算，可以说OpenCL的意义在于模糊了主板上那两种重要处理器的界限，并使在GPU上跑代码变得更容易。



  3 **OpenCL架构**

**  3.1 硬件层：**

上面说的都是关于通用计算以及OpenCL是什么，下面就提纲挈领的把OpenCL的架构总结一下：

  以下是OpenCL硬件层的抽象

![](https://img-blog.csdn.net/20130503150713608)



 它是一个Host（控制处理单元，通常由一个CPU担任）和一堆Computer Device（计算处理单元，通常由一些GPU、CPU其他支持的芯片担任），其中Compute Device切分成很多Processing Element（这是独立参与单数据计算的最小单元，这个不同硬件实现都不一样，如GPU可能就是其中一个Processor，而CPU可能是一个Core，我猜的。。因为这个实现对开发者是隐藏的），其中很多个Processing Element可以组成组为一个Computer Unit，一个Unit内的element之间可以方便的共享memory，也只有一个Unit内的element可以实现同步等操作。

3.2 内存架构

![](https://img-blog.csdn.net/20130503152241609)

其中Host有自己的内存，而在compute Device上则比较复杂，首先有个常量内存，是所有人能用的，通常也是访问最快的但是最稀少的，然后每个element有自己的memory，这是private的，一个组内的element有他们共用的一个local memery。仔细分析，这是一个高效优雅的内存组织方式。数据可以沿着Host-》gloabal-》local-》private的通道流动（这其中可能跨越了很多个硬件）。

**3.3软件层面的组成**

 这些在SDK中都有对应的数据类型

 setup相关：

  Device：对应一个硬件（标准中特别说明多core的CPU是一个整个Device）



  Context：环境上下文，一个Context包含几个device（单个Cpu或GPU），一个Context就是这些device的一个联系纽带，只有在一个Context上的那些Device才能彼此交流工作，你的机器上可以同时存在很多Context。你可以用一个CPu创建context，也可以用一个CPU和一个GPU创建一个。



Command queue：这是个给每个Device提交的指令序列



内存相关：

Buffers：这个好理解，一块内存

Images：毕竟并行计算大多数的应用前景在图形图像上，所以原生带有几个类型，表示各种维度的图像。



gpu代码执行相关：

 Program：这是所有代码的集合，可能包含Kernel是和其他库，OpenCl是一个动态编译的语言，代码编译后生成一个中间文件（可实现为虚拟机代码或者汇编代码，看不同实现），在使用时连接进入程序读入处理器。

 Kernel：这是在element跑的核函数及其参数组和，如果把计算设备看做好多人同时为你做一个事情，那么Kernel就是他们每个人做的那个事情，这个事情每个人都是同样的做，但是参数可能是不同的，这就是所谓的单指令多数据体系。

 WorkI tem：这就是代表硬件上的一个Processing Element，最基本的计算单元。



同步相关：

Events：在这样一个分布式计算的环境中，不同单元之间的同步是一个大问题，event是用来同步的



他们的关系如下图

![](https://img-blog.csdn.net/20130503152615657)



   上面就是OpenCL的入门介绍，其实说实话在10年左右就跟踪过GPGPU相关的东西，那时很多相关技术还存在于实验室，后来的CUDA出现后，也激动过，学习过一阵，不过CUDA过度依赖于特定硬件，产业应用前景并不好，只能做做工程试验，你总不能让用户装个游戏的同时，让他顺便换个高配的N卡吧。所以一度也对这个领域不太感兴趣，最近看到OpenCL的出现，发现可能这个架构还是有很好的应用前景的，也是众多厂商目前合力力推的一个东西。想想一下一个迭代10000次的for循环一遍过，还是很激动的一件事。

  在游戏领域，OpenCL已经有了很多成功的实践，好像EA的F1就已经应用了OpenCL，还有一些做海洋的lib应用OpenCL（海面水波的FFT运算在过去是非常慢的），另外还有的库干脆利用OpenCL去直接修改现有的C代码，加速for循环等，甚至还有OpenCl版本的C++ STL，叫thrust，所以我觉得OpenCL可能会真正的给我们带来些什么~



以下是一些关于OpenCL比较重要的资源：

[http://www.khronos.org/opencl/](http://www.khronos.org/opencl/) 组织的主页

[https://developer.nvidia.com/opencl](https://developer.nvidia.com/opencl) N家的主页

[http://developer.amd.com/resources/heterogeneous-computing/opencl-zone/](http://developer.amd.com/resources/heterogeneous-computing/opencl-zone/) A家的主页

[http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/](http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/) 标准的reference

[http://developer.amd.com/wordpress/media/2012/10/opencl-1.2.pdf](http://developer.amd.com/wordpress/media/2012/10/opencl-1.2.pdf) 必看 最新的1.2版本标准

[http://www.khronos.org/assets/uploads/developers/library/overview/opencl-overview.pdf](http://www.khronos.org/assets/uploads/developers/library/overview/opencl-overview.pdf) 必看，入门的review

[http://www.kimicat.com/opencl-1/opencl-jiao-xue-yi](http://www.kimicat.com/opencl-1/opencl-jiao-xue-yi)  一个教学网站

**使用例子**

**原文： **[**http://blog.csdn.net/leonwei/article/details/8893796**](http://blog.csdn.net/leonwei/article/details/8893796)



**1 Hello OpenCL **

    这里编写一个最简单的示例程序，演示OpenCl的基本使用方法：

    1.首先可以从Nvdia或者Amd或者Intel或者所有OpenCl成员的开发者网站上下载一份他们实现的OpenCL的SDK。虽然不同公司支持了不同版本的OpenCL和扩展ext，但是在相同版本上对于标准的OpenCL接口，每个SDK实现的结果都是一样的，如果你只是用标准的OpenCL规范，那么采用哪个SDK无所谓，当然有些公司把OpenCL SDK捆绑在更大的SDK里，如NVDIA放在他们的CUDA开发包里，这时我们要做的只是把其中cl文件夹下的h 以及 OpenCL.lib OpenCL.dll文件拿出来就行。



   下面进入代码的部分，本例中实现两个一维数组的相加（这是最容易理解的可并行计算问题），代码主要这几个部分：



   2.获取机器中所有已实现的OpenCL平台：

   //get platform numbers

   err = clGetPlatformIDs(0, 0, &num);


 //get all platforms

  vector<cl_platform_id> platforms(num);

  err = clGetPlatformIDs(num, &platforms[0], &num);



  首先要知道OpenCL平台platform是什么意思。我们知道不同OpenCL组织里不同厂商的不同硬件都纷纷支持OpenCL标准，而每个支持者都会独自去实现OpenCl的具体实现，这样如果你的机器中有很多个不同“OpenCl厂商”的硬件（通常实现在驱动中），那么你的机器中就会出现几套对OpenCL的不同实现，如你装了intel cpu，可能就一套intel的实现，装了NVDIA的显卡，可能还有一套Nvidia的实现，还有值得注意的是，就算你可能没有装AMD的显卡，但是你装了AMD的opencl开发包，你机器中也可能存在一套AMD的实现。这里的每套实现都是一个platform，可以说不同厂商拿到的SDK可能是一样的，但是查询到的机器里的platform则可能是不一样的，sdk是代码层，platform是在驱动里的实现层，opencl在不同厂商的代码层一样，但是在一个机器里会存在不同的实现层（原凉我这么啰嗦，但是这个问题我开始纠结了很久）。

 不同厂商给了相同的代码SDK，但是在驱动层，不同厂商的实现是完全不一样的，也就是paltform是不一样的，例如NVIDIA的的platform只支持N自己的显卡作为计算设备（可能他们认为cpu作为计算设备是在是鸡肋），但是AMD的platform则不仅支持AMD自己的设备，还支持Intel的CPU。

所以你要在程序开始查询机器所有支持的platform，再根据情况选择一个合适的paltform。（通常你要选择包含compute device的能力最强的那个platform，例如你发现客户机装的是N卡，而机器上有N的platform那么就选它了）

通过clGetPlatformInfo 这个函数还可以进一步的得到该平台的更多信息（名字、cl版本、实现者等等）



  3.查询device信息（在程序中这一步是可以不做的，但是可以用来判断platform的计算能力）

  //get device num

 err=clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_ALL,0,0,&num);

  vector<cl_device_id> did(num);

 //get all device

  err=clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_ALL,num,&did[0],&num);

  //get device info

 clGetDeviceInfo（...）

 以上代码可以获取某个platform下的所有支持的device（这里和下面都特指compute device，因为在pc下host device一定是你的CPU了）

 这些有助于你判断用哪个platform的计算能力更强



 4.选定一个platform，创建context（设备上下文）

 //set property with certain platform 

 cl_context_properties prop[] = { CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[0]), 0 };

 cl_context context = clCreateContextFromType(prop, CL_DEVICE_TYPE_ALL, NULL, NULL, &err);

 上面代码首先使用你选定的那个paltform设置context属性，然后利用这个属性创建context。context被成功创建好之后，你的CL工作环境就等于被搭建出来了，CL_DEVICE_TYPE_ALL意味着你把这个platform下所有支持的设备都连接进入这个context作为compute device。



 5.为每个device创建commandQueue。command queue是像每个device发送指令的信使。

   cqueue[i] = clCreateCommandQueue(context, did[0], 0, 0);



6.下面进入真正在device run code的阶段：kernal函数的准备

    首先准备你的kernal code，如果有过shader编程经验的人可能会比较熟悉，这里面你需要把在每个compute item上run的那个函数写成一段二进制字符串，通常我们实现方法是写成单独的一个文件（扩展名随意），然后在程序中使用的时候二进制读入这个文件。

   例如本例的数组相加的kernal code：

 __kernel void adder(__global const float* a, __global const float* b, __global float* result)

{

 int idx = get_global_id(0);

 result[idx] = a[idx]) +b[idx];

}

具体的限定符和函数我们后面会分析，但是这段代码的大意是获取当前compute item的索引idx，然后两个数组idx上的成员相加后存储在一个buf上。这段代码会尽可能并行的在device上跑。



把上面那个文件命名为kernal1.cl



然后在程序中读入它到字符串中（通常你可以为这个步骤写一个工具函数）

ifstream in(_T("kernal11.cl"), std::ios_base::binary);

 if(!in.good()) {

  return 0;

 }

 // get file length

 in.seekg(0, std::ios_base::end);

 size_t length = in.tellg();

 in.seekg(0, std::ios_base::beg);

 // read program source

 std::vector<char> data(length + 1);

 in.read(&data[0], length);

 data[length] = 0;

 // create and build program 

 const char* source = &data[0];



这样我们的kernal code就装进char* source里面了。



7.从kernal code 到program

 program在cl中代表了程序中所用到的所有kernal函数及其使用的函数，是device上代码的抽象表示，我们需要把上面的char* source转化成program：



cl_program program = clCreateProgramWithSource(context, 1, &source, 0, 0);


clBuildProgram(program, 0, 0, 0, 0, 0) 



如上两句代码分别先从字符串的source创建一个program，在build它（我们说过OpenCl是一个动态编译的架构）



8 .  拿到kernal 函数

kernal是CL中对执行在一个最小粒度的compute item上的代码及参数的抽象（你可以理解成为cpu上的main函数）。

我们需要首先从前面build好的program里抽取我们要run的那个kernal函数。

cl_kernel adder = clCreateKernel(program, "adder", 0);


9. 准备kernal函数的参数

kernal函数需要三个参数，分别是输入的两个数组mem，和一个输出的数组mem，这些mem都要一一创建准备好。

首先是输入的两个mem

std::vector<float> a(DATA_SIZE), b(DATA_SIZE)

 for(int i = 0; i < DATA_SIZE; i++) {

  a[i] = i;

  b[i] = i;

 }

a个b是我们要运算的两个输入数组（注意他们是在CPU上的，或者说分配与你的主板内存）

cl计算的变量要位于device的存储上（例如显卡的显存），这样才能快起来，所以首先要把内存搬家，把这部分输入数据从host mem拷贝到device的mem上，代码如下：

cl_mem cl_a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * DATA_SIZE, &a[0], NULL);

 cl_mem cl_b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * DATA_SIZE, &b[0], NULL);

上面代码的含义是使用host mem的指针来创建device的只读mem。


最后还要在device上分配保存结果的mem

cl_mem cl_res = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * DATA_SIZE, NULL, NULL);

这是直接在device上分配的。



最后设置好kernal的参数

 clSetKernelArg(adder, 0, sizeof(cl_mem), &cl_a);

 clSetKernelArg(adder, 1, sizeof(cl_mem), &cl_b);

 clSetKernelArg(adder, 2, sizeof(cl_mem), &cl_res);

10.执行kernal函数

err = clEnqueueNDRangeKernel(cqueue[0], adder, 1, 0, &work_size, 0, 0, 0, 0);



注意cl的kernal函数的执行是异步的，这也是为了能让cpu可以与gpu同时做事（但是异步就涉及到设备间的同步、状态查询等，这是非常复杂的一部分，后面再说）

所以上面这个函数会立即返回，clEnqueueNDRangeKernel的意思是往某个device的commoand queue里面推入一个kernal函数让其执行，device会按某个顺序执行它的command queue里面的指令，所以这个语句调用后，kernal是否真的立即执行还要取决于它的queue里面是否还有其他的指令。



11.将结果拷回CPU

  上面执行后的结果是直接写在device的存储上，通常要在代码中继续使用，我们就需要把这个结果再拷回到CPU的内存上，使用下面的代码：

std::vector<float> res(DATA_SIZE)

 err = clEnqueueReadBuffer(cqueue[0], cl_res, CL_TRUE, 0, sizeof(float) * DATA_SIZE, &res[0], 0, 0, 0);

clEnqueueReadBuffer的含义是往command queue里面推出一个条指令，是回拷mem，这里面的CL_TRUE是标志着这个指令的执行的同步的，就会阻塞cpu，所以这行代码返回就标志着该device上直到这个指令之前的所有指令都已经执行完了。

 上面为止就可以到带在res里我们使用cl在device上执行kernla函数的结果了，可以与纯CPU的执行结果对比一遍，结果应该是一致的。



12.打扫战场

//release

 clReleaseKernel(adder);

 clReleaseProgram(program);

 clReleaseMemObject(cl_a);

 clReleaseMemObject(cl_b);

 clReleaseMemObject(cl_res);



 for(size_t i=0;i<num;i++){

  clReleaseCommandQueue(cqueue[i]);

 }

 clReleaseContext(context);



**2.性能分析**

 上面的是一个非常简单的CL入门程序。借助这个程序，我后来又做了很多性能分析，想知道究竟使用CL执行运算和平常的CPu上运算有什么区别，性能会有怎样的不同。

我修改了不同版本的kernal函数，使kernal的运算复杂度不断提升，并在不同platform下和单纯在CPU上执行这些运算，得到的统计数据如下：

注意：

0.1、2、3的复杂度分别使用的简单扩大数组长度、求幂操作、增加求幂操作的指数

1.以下的数据皆为毫秒

2.第一列为传统的CPU运算，后两列为使用Amd 和Nvidia两个平台的运算

3.由于测试机未安装AMD显卡，所以AMD平台使用的device其实是一个CPU，所以1、2、3列代表的情况可以看做纯CPU，使用openCL架构用CPU做计算设备、使用OpenCL架构用GPU做设备

4.由于OpenCL架构多涉及到一个host和device间内存拷贝的操作，2、3列中的+号两端分别代表拷贝内存所用的时间和实际运算时间。


|运算复杂度|CPU计算（intel E6600 Duo core）|AMD platform +CPU device（intel E6600 Duo core）|Nvidia platform+Nvidia（Geforce GT440）|
|----|----|----|----|
|1|78|63+60|63+120|
|2|1600|63+500|63+130|
|3|9600|63+1300|63+130|





从上表我们“以偏盖全”的得到一些结论：

1.纯CPU的计算会随着计算复杂度的增加而显著上涨，纯GPU的CL架构的计算在与此同时计算耗时基本平稳，虽然在第一个运算，GPU的时间还会高于CPU，但是到第三个运算时GPU的时间依然没有明显增长，而CPU已经长到GPU时间的70多倍。

2.不同平台的CL实现在内存拷贝上所化时间基本一致，这部分时间跟计算复杂度无官，只跟内存大小有关。在我们的例子中他们都是63ms

3.从1.2列的对比看出，就算是同样使用CPU做为计算，在CL架构下性能也会得到较大提升，虽然实质上1和2列都是最终在CPU上计算，但是CL的架构可能封转了更高一层，利用了CPU内的一些高级指令或者利用了CPU的更多的并行计算能力。

4.OpenCL是真正兼容各种硬件的，不同于CUDA，这对于产业化产品的开发意义重大，在主流的机器上，你总能找到一个可用的opencl platform，而它都会比CPU计算提示性能。



   从这个简单的性能分析可以看出，使用OpenCL架构的异构计算可以大幅度提高传统在CPU上的计算性能，而且这种提高可能会随着计算量的复杂度升高而增长，所以那些所谓“百倍”、“千倍”的增长在某些计算领域是有可能的，同时尽量使用GPU做device是可以最大提升性能的；

  同时我们要注意到异构计算通常涉及到大量的内存拷贝时间，这取决于你内存与显存间的带宽，这部分时间是不可忽视的，如果一个计算工作，它在CPU上运行的时间都比内存在异构设备间拷贝的时间短，那么将他做OpenCL的加速是没有任何意义的，也就是说我们要注意计算的复杂度，复杂度过小的计算使用异构计算反而会增加计算时间，GPU运算都存在一个跟计算复杂度无关的“起步时间”（例如本例在180ms左右，当计算在CPU上执行小于180ms时放在GPU上是无意义的。）




关于OpenCL并行单元索引问题

[http://www.csdn.net/article/a/2012-08-20/2808832](http://www.csdn.net/article/a/2012-08-20/2808832)

Kernel对象：

Kernel就是在程序代码中的一个函数，这个函数能在OpenCL设备上执行。一个Kernel对象就是kernel函数以及其相关的输入参数。

![image](http://articles.csdn.net/uploads/allimg/120820/11322H5Y-0.png)

Kernel对象通过程序对象以及指定的函数名字创建。注意：函数必须是程序源代码中存在的函数。

![image](http://articles.csdn.net/uploads/allimg/120820/11322H220-1.png)

运行时编译：

在运行时，编译程序和创建kernel对象是有时间开销的，但这样比较灵活，能够适应不同的OpenCL硬件平台。程序动态编译一般只需一次，而Kernel对象在创建后，可以反复调用。

![image](http://articles.csdn.net/uploads/allimg/120820/11322Ja7-2.png)

创建Kernel后，运行Kernel之前，我们还要为Kernel对象设置参数。我们可以在Kernel运行后，重新设置参数再次运行。

![image](http://articles.csdn.net/uploads/allimg/120820/11322I217-3.png)

arg_index指定该参数为Kernel函数中的第几个参数(比如第一个参数为0，第二个为1,…)。内存对象和单个的值都可以作为Kernel参数。下面是2个设置Kernel参数的例子：

clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&d_iImage);

clSetKernelArg(kernel, 1, sizeof(int), (void*)&a);

![image](http://articles.csdn.net/uploads/allimg/120820/11322J0D-4.png)

在Kernel运行之前，我们先看看OpenCL中的线程结构：

大规模并行程序中，通常每个线程处理一个问题的一部分，比如向量加法，我们会把两个向量中对应的元素加起来，这样，每个线程可以处理一个加法。

下面我看一个16个元素的向量加法：两个输入缓冲A、B，一个输出缓冲C

![image](http://articles.csdn.net/uploads/allimg/120820/11322I4U-5.png)

在这种情况下，我们可以创建一维的线程结构去匹配这个问题。

![image](http://articles.csdn.net/uploads/allimg/120820/11322H017-6.png)

每个线程把自己的线程id作为索引，把相应元素加起来。

![image](http://articles.csdn.net/uploads/allimg/120820/11322K306-7.png)

OpenCL中的线程结构是可缩放的，Kernel的每个运行实例称作WorkItem(也就是线程），WorkItem组织在一起称作WorkGroup，OpenCL中，每个Workgroup之间都是相互独立的。

通过一个global id(在索引空间，它是唯一的）或者一个workgroup id和一个work group内的local id，我就能标定一个workitem。

![image](http://articles.csdn.net/uploads/allimg/120820/11322J919-8.png)

在kernel函数中，我们能够通过API调用得到global id以及其他信息：

get_global_id(dim)

get_global_size(dim)

这两个函数能得到每个维度上的global id。

get_group_id(dim)

get_num_groups(dim)

get_local_id(dim)

get_local_size(dim）

这几个函数用来计算group id以及在group内的local id。

get_global_id(0) = column, get_global_id(1) = row

get_num_groups(0) * get_local_size(0) == get_global_size(0)

﻿﻿



[更有用的demo(高通GPU)](http://blog.csdn.net/leonwei/article/details/8880012)

[http://blog.csdn.net/wcj0626/article/details/26272019](http://blog.csdn.net/leonwei/article/details/8880012)




[PowerVR demo](http://blog.csdn.net/leonwei/article/details/8880012)




[http://imgtec.eetrend.com/blog/3133](http://blog.csdn.net/leonwei/article/details/8880012)

代表性demo


[http://blog.csdn.net/dj0379/article/details/39484061](http://blog.csdn.net/leonwei/article/details/8880012)﻿﻿


﻿﻿



