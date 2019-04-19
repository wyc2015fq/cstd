# LibSVM 工具箱...不可分肿么办！ - 三少GG - CSDN博客
2011年05月28日 17:33:00[三少GG](https://me.csdn.net/scut1135)阅读数：2418
1.
## 请教Matlab中svm libsvm工具箱分类问题
为什么在训练集上的准确率达到了100％
但是在测试集上的准确率只有46％
我也用交叉验证 寻找了最优的c和g，依然在训练集的准确率恨高
但是在测试集不到50％ 显然是失败的分类
能不能说明我的数据根本就不能分类呢？
是哪些因素影响了测试集上的准确率？
如果规范化数据才是最好的呢？
+++++++=
一则看看数据有没有问题,若数据没有问题..
那就说明 你需要进行数据的预处理[去噪,归一化,特征提取]
尤其是需要特征提取,或者你进行了特征提取,但特征提取的不好,无法表征测试数据的结构!!.
faruto老师说的不错
我怀疑是我的数据本身就没甚么特征。。也就是不可能找到个模型来分类
如何才能进行特征提取那？
 +++++++++++++++++++++++++++++
特征提取涉及到的面就很广了.因为不同的数据有不同的特征提取的办法..
这需要看的东西就多了.无法.详细说.
这要的 就是 什么pca  ica 谱特征什么乱起八糟的....还有一大堆呢....
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
svm-light、libsvm 和 svmtorch（torch）
http://hi.baidu.com/superdxf/blog/item/b169cc1686d38a4f21a4e9ee.html
最近用了这两种software去测试打手机，发觉svmlight速度比较快，准确率还可以，而且便于操作，而libsvm在train的时候速度不够快，而且操作起来挺麻烦，更重要的是在c++中测试单个图片的时候非常慢，一张576*720的图片要费3秒左右的时间，几乎是svmlight的三倍！这个无论如何优化都不可能提高太大，虽然准确率还可以，但是毕竟速度太慢，用在trecvid上面根本不work，还是将注意力转移到svmlight上面来，用mask的方法去除一些区域，然后选择更好的正样本，加上PCA，再对每个块进行大小scale的调整，测试一下，看看结果如何再说吧！
http://blog.sina.com.cn/s/blog_4c98b96001000ald.html
今天做了七组实验，数据集采取的是UCI的数据集，a1a-a7a,SVMlight采用的是默认的参数设置，其实本来开始使用RBF的呢，效果很烂，后来改为poly的也不行，最后实在不行了，偶然的用线性的做了一次，结果效果最好，说明其实很多时候数据线性可分可能性还是很大的。当然因为数据都是向量的形式提供的，我们没有办法知道这些向量背后的意义是什么。LIBSVM用的也是线性的，为了对比的需要我们同样采用的是最简单的形式，是c-SVM的形式。没有采用启发式原则，这点让笨笨有点疑惑，因为从理论上来说如果采用启发式原则正确率会高一些，但是实际却相反。针对这两种算法比较了它们呢的traing
 time和predicte time和accuracy，可以看到libsvm和svmlight的accuracy是相当的，但是时间就差很多了，svmlight要比libsvm快了很多倍，而且在predict时除了IO操作之外，svmlight几乎没有使用cpu的时间，再一次证明了svmlight的强大优势。其实本来对HeroSVM抱有很大的希望，因为它比SVMlight还要快9-10左右，这个相当好了，但是不开源需要money，近期又不对外了，所以很是郁闷，精力再一次转换到了SVMlight身上了。
http://www.newsmth.net/bbsanc.php?path=/groups/sci.faq/AI/SourceCodes/M.1202657568.T0
我做过一些测试，二类问题SVMlight要比libsvm快很多。
svmlight是用c写的，结构化程度不高，读起来可能有点费力；libsvm要清晰一点；但是和svmtorch比起来感觉还是torch要好一些。
http://whalebox.blogspot.com/2008/03/libsvmsvm-light-zz.html
其中，libsvm似乎是从svm-light基础上发展起来的。但是在解二次规划的方法上，没有完全沿用svm-light的方法。我做了些简单的实验，发现普遍来说，svm-light比libsvm快，而且效果好一些。不知道有没有大侠对他们有研究，比较一些如何？**不过，libsvm很容易使用，很容易被集成在自己的cpp工程中，svm-light似乎很麻烦吧，呵呵。**各种SVM程序的比较：
[http://hi.baidu.com/oliveai/blog/item/50e6c5ceba62bb3fb600c8a1.html](http://hi.baidu.com/oliveai/blog/item/50e6c5ceba62bb3fb600c8a1.html)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## 补充:在WindowsXP+OpenCVRC1平台下整合OpenCV与libSVM 
虽然从RC1版开始opencv开始增设ML类，提供对常见的分类器和回归算法的支持。但是尚存在一些问题，比如说例子少（官方许诺说很快会提供一批新例子,见CVS版）。单说SVM这种算法，它自己提供了一套比较完备的函数，但是并不见得优于老牌的libsvm（它也应该参考过libsvm，至于是否效率优于libsvm，我并没有作过测试，官方也没有什么说法，但是libsvm持续开源更新，是公认的现存的开源SVM库中最易上手，性能最好的库）。所以在你的程序里整合opencv和libSVM还是一种比较好的解决方案。在VC中整合有些小地方需要注意，这篇文档主要是提供把图象作为SVM输入时程序遇到的这些小问题的解决方案。希望大家遇到问题时，多去读libSVM的源码，它本身也是开源的，C代码写得也很优秀，数据结构定义得也比较好。
首先是SVM的训练，这部分我并没有整合到VC里，直接使用它提供的python程序，虽然网格搜索这种简易搜索也可以自己写，但是识别时只需要训练生成的SVMmodel文件即可，所以可以和主程序分离开。至于python在windows下的使用，还是要设置一下的，首先要在系统环境变量path里把python的路径设进去，libsvm画训练等高线图还需要gnuplot的支持，打开python源程序(grid.py)，把gnuplot_exe设置成你机器里的安装路径，这样才能正确的运行程序。然後就是改步长和搜索范围，官方建议是先用大步长搜索，搜到最优值後再用小步长在小范围内搜索（我觉得这有可能会陷入局部最优，不过近似出的结果还可以接受）。我用的python版本是2.4，gnuplot4.0。
## 常用libSVM资料链接 
[官方站点，有一些tutorial和测试数据](http://www.csie.ntu.edu.tw/~cjlin/libsvm/)
[哈工大的机器学习论坛，非常好](http://bbs.ir-lab.org/cgi-bin/leoboard.cgi)
上交的一个研究生还写过libsvm2.6版的代码中文注释，源链接找不着了，大家自己搜搜吧，写得很好，上海交通大学模式分析与机器智能实验室。 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++================================================================
### libsvm FAQ!!!!!!
英文原版地址： [http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
中文翻译地址： [http://blog.csdn.net/ssrs626/archive/2008/11/09/3261520.aspx](http://blog.csdn.net/ssrs626/archive/2008/11/09/3261520.aspx)
    (或者 [http://happybaby99.blog.sohu.com/76788562.html](http://happybaby99.blog.sohu.com/76788562.html))
这几天写论文用到libsvm，下载了程序和源码，却不知道怎么用，在网上搜了一大箩筐，还是没什么进展。后来发现了几个不错的介绍。
转载 1.http://blog.csdn.net/lanseliuying/category/331083.aspx
转载2.FAQ http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#f203
**问: 我在那里能够找到libsvm的文件 ?**
软件包中有一个 README 文件，里面详细说明了所有参数选项、数据格式以及库函数的调用。在python目录下，模型选择工具和python界面的libsvm各有一个README文件。 初学者可以通过[A practical guide to support vector classification](http://www.csie.ntu.edu.tw/~cjlin/papers/guide/guide.pdf)了解如何训练和检验数据.论文[LIBSVM : a library for support vector machines](http://www.csie.ntu.edu.tw/~cjlin/papers/libsvm.pdf) 详细讨论了libsvm的使用.                                       
[Go Top]
**问: 以前版本的libsvm都有什么变化?**
详见[**变化日志**](http://www.csie.ntu.edu.tw/~cjlin/libsvm/log). 你可以到[**这里**](http://www.csie.ntu.edu.tw/~cjlin/libsvm/oldfiles)下载以前版本的libsvm .                                 [[Go
 Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 如果我想引用libsvm， 我应该引用哪篇论文?**
请引用以下论文: Chih-Chung Chang and Chih-Jen Lin, LIBSVM : a library for support vector machines, 2001. Software available at http://www.csie.ntu.edu.tw/~cjlin/libsvm      bibtex格式如下：
@Manual{CC01a,
author =	 {Chih-Chung Chang and Chih-Jen Lin},
title =	 {{LIBSVM}: a library for support vector machines},
year =	 {2001},
note =	 {Software available at /url{http://www.csie.ntu.edu.tw/~cjlin/libsvm}}
}                                                                   [[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 如果我想在我的软件中应用svm，是否存在软件许可问题？**
Libsvm许可文件 ("修改了的BSD许可文件") 和其他许多软件的许可文件都是兼容的，如 GPL. 因此你很容易就可以把libsvm应用到你的软件中，你还可以将它应用到商业产品中.                    
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 是否有一系列基于libsvm的附加工具?**
是的，详见[libsvm tools](http://www.csie.ntu.edu.tw/~cjlin/libsvmtools)[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 在Unix操作系统中， 出现了"error in loading shared libraries"（“在装载共享库时发生错误”）或者"cannot open shared object file."（无法打开共享目标文件“） 这是为什么?**
如果你在一台机器上编译代码，然后到另外一台有着不兼容库的机器上运行，这种情况酒会经常发生。你在那台机器上可以重新编译一下，或者用静态连接。                                     
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:修改源代码后，如果想在MS窗口中建立一个“svm-toy”的图形界面，我该怎么做？**
你可以通过选择 "Win32 Project"来建立一个工程。另外对于"svm-train"和 "svm-predict" ，你需要选择"Win32 Console Project"。对于libsvm2.5以后的版本，你还可以通过Makefile.win来实现。详见README。
如果你没有用Makefile.win并出现了以下连接错误：LIBCMTD.lib(wwincrt0.obj) : error LNK2001: unresolved external symbol
_wWinMain@16
那么，你很有可能是选择了一个错误的工程类型。                                     [[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 我是MS Windows用户，在这些预编译的exe文件中，为什么只有一个(SVM_toy)运行?**你应当打开命令窗口，然后输入svmtrain.exe，这样就可以在DOS窗口中看到所有的选项了。你可以到README文件中看到一些例子                                                                       
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:为什么有时我在training/model 文件中看不到所有的数据属性呢？W**
libsvm应用了所谓的”稀疏“格式，这样零值就不用存储了。例如，有下面属性的数据 
1 0 2 0
将被替换为： 1:1 3:2                                                  [[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:如果我的数据是非数值型的，可以用libsvm吗？**目前libsvm只支持数值型的数据。因此，你必须将非数值型的转为数值型的数据。比如，你可以用二进制属性来替代原来的类别属性。
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:为什么要采用稀疏格式呢? 密集数据在训练时候会不会很慢？**这是个具有争议的话题。将系数向量赋值给核函数是比较慢的，因此总的训练时间至少是采用密集格式的2倍或3倍。 但是，我们不支持密集格式的数据，因为我们不能够处理极度稀疏的数据。代码的简洁也是我们考虑的一个因素。目前我们决定只支持稀疏格式的数据。                                                                    
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 训练C-SVM时，其输出结果如下， 它们代表什么意思?**
optimization finished, #iter = 219 
nu = 0.431030 
obj = -100.877286, rho = 0.424632 
nSV = 132, nBSV = 107 
Total nSV = 132 obj 是对偶SVM问题的最优目标值. rho 是判决函数sgn(w^Tx - rho)的偏项. nSV 和 nBSV 分别代表支持向量和边界支持向量 (即alpha_i = C)的个数. nu-svm 在某中程度上可以看作 C-SVM的等价形式，不过其中的 C被替换为 nu. nu 仅表明了相应的参数. 更详细的解释请看[libsvm
 document](http://www.csie.ntu.edu.tw/~cjlin/papers/libsvm.pdf). 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:关于模型文件，您能否解释的更详细一点？**
在参数后面，每一条线代表一个支持向量。 支持向量按照先列出的标签列出（即类标签列表中，第一类的支持向量首先分组，然后依次类推） 
如果k是类别的总数，那么每个支持向量的前面就有k-1个y*alpha系数，其中alpha代表如下二分类问题的对偶解：
1 vs j, 2 vs j, ..., j-1 vs j, j vs j+1, j vs j+2, ..., j vs k 
并且在前j-1个系数中，y=+1, 其余的k-j个系数中y=-1.例如，对于4分类问题，文件看起来结构如下： 
+-+-+-+--------------------+
|1|1|1|                    |
|v|v|v|  类别1的支持向量  |
|2|3|4|                    |
+-+-+-+--------------------+
|1|2|2|                    |
|v|v|v|  类别2的支持向量 |
|2|3|4|                    |
+-+-+-+--------------------+
|1|2|3|                    |
|v|v|v|  类别3的支持向量  |
|3|3|4|                    |
+-+-+-+--------------------+
|1|2|3|                    |
|v|v|v|  类别4的支持向量  |
|4|4|4|                    |
+-+-+-+--------------------+**问: 在缓缓存中，是否可以用浮点型或者双精度型来存储数据?** 
当你在缓存中存入更多的数据时，我们默认的是浮点型.一般情况下，这样就很好了，但是对于少数困难的情况（如C非常非常大）解将是巨大的数字。这时，仅用浮点型，数值的精度可能就不够了。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问：怎样选择核函数?**
通常我们建议你首先采用RBF核函数。Keerthi 和 Lin 的最近的研究([** 下载论文**](http://www.csie.ntu.edu.tw/~cjlin/papers/limit.ps.gz)) 表明如果模型选择RBF的话，就没有必要再考虑线性核函数了。采用sigmoid核函数的矩阵不一定会正有界，而且通常它的准确性也不如RBF(可参见Lin和Lin的论文[** 此处下载**](http://www.csie.ntu.edu.tw/~cjlin/papers/tanh.pdf)). 多项式核函数还不错，但是如果度数很高的话，数值困难就会发生(考虑到(<1)的d次幂会趋向0，(>1)的d次幂会趋向无穷） 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: libsvm是否可以用来专门处理线性SVM？D**
不是的，目前libsvm用同样的方法处理线性/非线性SVMs. 注意：如果采用线性核函数，一些技巧可能会节省训练/检验的时间。 因此libsvm对线性SVM并不时特别有效,尤其是采用很大的C的问题，这些问题数据的数值比其属性值要大得多。你可以： 
- 仅用很大的C.下面的论文表明了：当C大于一个确定的阀值以后，判决函数是相同的。 
[**S. S. Keerthi**](http://guppy.mpe.nus.edu.sg/~mpessk/) and **C.-J. Lin**. [**Asymptotic behaviors of support vector machines with Gaussian kernel **](http://www.csie.ntu.edu.tw/~cjlin/libsvm/papers/limit.ps.gz). [**Neural Computation**](http://mitpress.mit.edu/journal-home.tcl?issn=08997667), 15(2003), 1667-1689. 
- 尝试用[**bsvm**](http://www.csie.ntu.edu.tw/~cjlin/bsvm), 它有个对解决线性SVMs很有效的方法.你可以在下面的研究中找到更详细的内容: 
K.-M. Chung, W.-C. Kao, T. Sun, and C.-J. Lin. **Decomposition Methods for Linear Support Vector Machines. Neural Computation**, 16(2004), 1689-1704. 
另外，你并没必要一定要解决线性SVMs.你可以参考前面有关如何选取核函数的问题。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 自由支持向量特别大，我该怎么办?**
当数据过拟和时，这种情况会经常发生. 如果数据的属性值范围特别大，你可以尝试调整它们的范围。这样合适参数的域可能会很大。注意：libsvm里包含有数据调整程序的，即svm_scale. 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 我是否可以用同样的方法来调整训练数据和检验数据的范围？**
是的, 可以按照以下方法: 
svm-scale -s scaling_parameters train_data > scaled_train_data 
svm-scale -r scaling_parameters test_data > scaled_test_data 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:将属性值限制到[0,1]，是否比限制到 [-1,1]有很大的不同?**
对于线性规划方法，如果采用可RBF核函数并进行了参数的选择，两者是没什么不同的。假设Mi和mi分别代表第i个属性的最大值和最小值.规划到[0,1]即： 
                x'=(x-mi)/(Mi-mi)
对于[-1 1]： 
                x''=2(x-mi)/(Mi-mi)-1.
在RBF核函数中： 
                x'-y'=(x-y)/(Mi-mi), x''-y''=2(x-y)/(Mi-mi).
因此在[0,1]数据规划中用(C,g)，和[-1 1]数据规划中用(C,g/2)是一样的。 
尽管性能相同，但是计算机时间可能不同。对于有许多零入口的数据, [0,1]规划保持了输入数据的稀疏特性，因此可能会节省计算时间。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 预测准确率很低，应该如何改进？**
可以用python目下的grid.py来寻找合适的参数，grid.py是一个用来选择模型参数的工具。关于模型选择的重要性，你可以参考我的谈话： [**A practical guide to support vector classification **](http://www.csie.ntu.edu.tw/~cjlin/talks/freiburg.pdf)
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 我的数据是不平衡的，libsvm能解决这样的问题吗？**
可以。libsvm有一个-wi选项。例如，你用： 
svm-train -s 0 -c 10 -w1 1 -w-1 5 data_file 
则对类别“-1”的惩罚就较大。注意-w选项仅用在C-SVC中。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: nu-SVC和C-SVC有什么不同之处?**
除了参数不同外，两者基本是一样的。C-SVC中，C的范围是0到无穷，nu-SVC中C的范围是[0 1]。 nu一个很好的特性是：它与支持向量的比率和训练误差的比率相关。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 程序一直运行，但却没有输出，应该怎么办？**
你最好检查一下数据. 每一个训练/学习的数据都必须在一行，不可以分开。此外，你必须删除空行。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 程序一直运行（有输出，比如许多点）。应该怎么办？**
从理论上讲，如果核矩阵式半正定的，libsvm可以保证收敛。2.4以后版本的libsvm还可以处理非半正定矩阵核，比如sigmoid(tanh)核。这样看来，你处理的情况是病态情况（比如参数过大或过小），这样机会发生数值困难。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问: 训练时间太长，应该怎么办？**
对于大型问题，请确保有足够的缓存空间（即-m）。对于一些困难的情况（比如-c很大），会出现收敛很慢的情况. 你可以常识用一个较大点的终止可容忍偏差. 如果这样仍然不行，你可以和我们联系。我们会告诉你一些关于改善训练时间短小技巧。 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:我如何才能得到判决值?**
对于回归，程序可以打印出判决值。对于分类，可以通过解几个二进制SVMs来实现多分类，因此你可以通过调用子程序svm_predict_values来实现输出判决值。然后可以通过子程序svm_get_labels得到相应的标签值。详见软件包中的README文件。 
我们并不推荐以下操作，但是如果你要得到二分类的标签值+1和-1（注：这里的+1、-1和5、10的含义是不一样的），你只要在svm.cpp文件中的 svm_predict_values(model, x, dec_values) 后面添加：printf("%f/n",dec_values[0]*model->label[0])就可以了。 
正（负）判决值对应的值为+1（-1） 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:怎样得到一个点到超平面的距离？**
距离的表达式为： |decision_value| / |w|.我们有： |w|^2 = w^Tw = alpha^T Q alpha = 2*(dual_obj + sum alpha_i). 因此在svm.pp文件中，知道输出对偶目标值的语句，在后面加上打印输出 w^Tw的语句就可以了. 
[[Go Top]](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#_TOP)
**问:在linux操作系统上，对于一些问题，如果用很大的缓存（即很大的-m），有时候会出现“段错误”（"segmentation fault“），为什么?**
在32位机器上，最大可设地址空间为4GB。Linux核将其分成3：1，即：用户的空间为3GB，核空间为1G。尽管用户有3GB的存储空间，但是最大动态分配内存只有2GB。因此如果你将-m设为2G，内存将被耗尽。因此svm-train运行时候如果需要更多内存，程序就无法运行。详见[**此文**](http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&selm=3BA164F6.BAFA4FB%40daimi.au.dk). 
有两中方法可以解决这个问题。如果你的机器支持Intel的物理地址扩展，你可以在Linux核中起用HIGHMEM64G选项，这样用户和核都将分配到4G的空间. 此外, 你还可以使用`tub'软件，它将为动态分配内存 elimate the 2G boundary . `tub' 可以到[**http://www.bitwagon.com/tub.html**](http://www.bitwagon.com/tub.html)下载. 
