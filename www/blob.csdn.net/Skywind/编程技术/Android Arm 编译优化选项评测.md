# Skywind Inside » Android Arm 编译优化选项评测
## Android Arm 编译优化选项评测
August 25th, 2015[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
用不同测试用例具体测试 softfp, armv7-a, cortax 等优化选项，看选项不同性能差别多大。首先设计下面几个测试用例，包含字符串处理、复杂逻辑、整数运算、浮点运算几个方面：
- compress：进行 LZO/LZW 大规模压缩，测试搜索，字符串匹配，复杂分支等性能 
- resample：进行一系列整数 DSP 运算，包括 resample 和 fir low pass 
- int add：一亿次整数加法 
- int mul：一亿次整数乘法 
- int div：一亿次整数除法 
- float add：一亿次浮点加法 
- float mul：一亿次浮点乘法 
- float div：一亿次浮点除法 
- const div：一亿次整数除以常数255 
- matrix：若干次矩阵乘法运算，同时考察浮点数乘法加法 
- normalize：若干次矢量归一化运算，同时考察浮点数乘法，除法，加法，sqrt 
其次对安卓的几个 gcc 的编译选项进行分别测试：
- -mfloat-abi=softfp，如果有硬件浮点处理器将会使用硬件，如果没有会转移到软件模拟 
- -march=armv7-a，生成适合 armv7a 架构的代码 
- -mtune=cortex-9，代码生成按照 cortex-9 进行调优 
- –mfpu=neon，使用 neon 进行硬件浮点运算，决定 softfp 的硬件方式到底用这个 
- -mfpu=vfp，使用 vfp 进行硬件浮点运算，决定 softfp 的硬件方式用这个 
 测试硬件：   
- 桌面电脑：Intel® Core™ i5-2520M CPU @ 2.50GHz 
- 安卓手机：三星，双核 CPU 1.73GHz armv7-a cortex-9 
结果如下：
> 
Desktop PC -O2:      
    compress : 120 ms       
    resample : 290 ms       
    int add  : 78 ms       
    int mul  : 86 ms       
    int div  : 459 ms       
    float add: 76 ms       
    float mul: 69 ms       
    float div: 797 ms       
    const div: 190 ms       
    matrix   : 166 ms       
    normalize: 177 ms
samsung -O2:      
    compress : 577 ms       
    resample : 576 ms       
    int add  : 291 ms       
    int mul  : 332 ms       
    int div  : 2267 ms       
    float add: 3080 ms       
    float mul: 1792 ms       
    float div: 7103 ms       
    const div: 779 ms       
    matrix   : 9280 ms       
    normalize: 5721 ms
samsung -O2, -mfloat-abi=softfp:      
    compress : 540 ms       
    resample : 558 ms       
    int add  : 291 ms       
    int mul  : 333 ms       
    int div  : 2268 ms       
    float add: 266 ms       
    float mul: 270 ms       
    float div: 986 ms       
    const div: 780 ms       
    matrix   : 301 ms       
    normalize: 702 ms
samsung -O2, -mfloat-abi=softfp, -march=armv7-a:      
    compress : 497 ms       
    resample : 489 ms       
    int add  : 537 ms       
    int mul  : 331 ms       
    int div  : 2342 ms       
    float add: 266 ms       
    float mul: 270 ms       
    float div: 986 ms       
    const div: 703 ms       
    matrix   : 371 ms       
    normalize: 701 ms
samsung -O2, -mfloat-abi=softfp, -march=armv7-a, -mtune=cortex-a9:      
    compress : 493 ms       
    resample : 517 ms       
    int add  : 626 ms       
    int mul  : 408 ms       
    int div  : 2381 ms       
    float add: 266 ms       
    float mul: 271 ms       
    float div: 987 ms       
    const div: 703 ms       
    matrix   : 302 ms       
    normalize: 701 ms
samsung -O2, -mfloat-abi=softfp, -march=armv7-a, -mtune=cortex-a9, -mfpu=neon:      
    compress : 498 ms       
    resample : 474 ms       
    int add  : 514 ms       
    int mul  : 330 ms       
    int div  : 2511 ms       
    float add: 267 ms       
    float mul: 269 ms       
    float div: 987 ms       
    const div: 711 ms       
    matrix   : 302 ms       
    normalize: 672 ms
samsung -O2, -mfloat-abi=softfp, -march=armv7-a, -mtune=cortex-a9, -mfpu=vfpv4:      
    compress : 738 ms       
    resample : 616 ms       
    int add  : 590 ms       
    int mul  : 360 ms       
    int div  : 2458 ms       
    float add: 270 ms       
    float mul: 274 ms       
    float div: 996 ms       
    const div: 718 ms       
    matrix   : 306 ms       
    normalize: 687 ms
同PC的各项性能比较，手机上的执行时间除以PC的时间
![image](http://www.skywind.me/blog/wp-content/uploads/2015/08/image_thumb10.png)
结果分析：
换了几款手机，测试结果差别不大（各个项目比例区别不大），
主流手机中，浮点数加减乘除皆快过整数加减乘除，很多库的定点数编译选项应该关闭了
最重要的编译选项是 –mfloat-abi=softfp，加上后浮点性能从默认软件模拟方式提升十倍。
增加：-march=armv7-a 计算压缩有 8%-10%左右提高，但是整数加法性能明显下降。
-mtune=cortex-a9 提高基本看不出来
