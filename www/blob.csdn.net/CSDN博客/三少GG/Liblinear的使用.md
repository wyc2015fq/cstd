# Liblinear的使用 - 三少GG - CSDN博客
2012年09月27日 20:51:15[三少GG](https://me.csdn.net/scut1135)阅读数：7130
 Ubuntu下安装:[http://pkgs.org/ubuntu-11.04/ubuntu-universe-i386/liblinear-tools_1.6+dfsg-1_i386.deb.html](http://pkgs.org/ubuntu-11.04/ubuntu-universe-i386/liblinear-tools_1.6+dfsg-1_i386.deb.html)
**然后软件中心查找liblinear的附加组件，添加Development libraries and header files for liblinear**
### [Libsvm和Liblinear的使用经验谈](http://blog.csdn.net/interxuxing/article/details/7607112)
[http://blog.csdn.net/interxuxing/article/details/7607112](http://blog.csdn.net/interxuxing/article/details/7607112)
[Libsvm](http://www.csie.ntu.edu.tw/~cjlin/libsvm/)和[Liblinear](http://www.csie.ntu.edu.tw/~cjlin/liblinear/)都是国立台湾大学的[Chih-Jen
 Lin](http://www.csie.ntu.edu.tw/~cjlin/index.html)博士开发的，Libsvm主要是用来进行非线性svm 分类器的生成，提出有一段时间了，而Liblinear则是去年才创建的，主要是应对large-scale的data classification，因为linear分类器的训练比非线性分类器的训练计算复杂度要低很多，时间也少很多，而且在large scale data上的性能和非线性的分类器性能相当，所以Liblinear是针对大数据而生的。
   两者都是一个跨平台的通用工具库，支持windows/linux/mac os,代码本身是c++写的，同时也有matlab，python，java，c/c++扩展接口，方便不同语言环境使用，可以说是科研和企业人员的首选！像我这样在学校的一般用matlab/c++，而我同学在百度则主要用的是python/c++，所以只是各自侧重不一样，但所使用的核心还是其svm库。
   以上Libsvm和Liblinear的主页上都有windows下的binary文件下载，zip,tar格式都有，解压后，找到matlab子文件目录，参看里面的readme文件，需要在matlab中进入此目录，运行make.m文件，matlab会根据本机默认的c/c++编译器将.c文件生成为.mexw32文件(由于我是32位操作系统，此处为mexw32,对于64位os，则对应为mexw64)，提供matlab下能使用的接口。之后生成了这些.mexw32文件复制到你自己的matlab工程根目录中，就可以在matlab文件中调用libsvm/liblinear库中的函数了~
[http://blog.sina.com.cn/s/blog_5bd2cb260100ev25.html](http://blog.sina.com.cn/s/blog_5bd2cb260100ev25.html) 这位网友对libsvm在matlab中的使用说明的很详细，可以参考下。
有关Liblinear和Libsvm各自的优势可以归纳如下：
1.libsvm用来就解决通用典型的分类问题
2.liblinear主要为大规模数据的线性模型设计
- it can be able to handle large-scaled dataset 可以用来处理大规模的数据
- it runs really faster than libsvm because it doesn't have to compute thekernel for any two points 由于采用线性核,所以不需要计算kernel value,速度更快
- trust region method for optimization looks new for machine learning people 
以下为一位网友采用liblinear进行数据分类的实验性能说明“
”今天试用了以下liblinear，速度很快（快到我没有想到）， 
我的实验数据： 
训练集：21504 * 1500（1500是样本的数量，21504是维度） 
测试集：21504 * 2985 
速度用秒来衡量，20次实验总共不到2分钟。 
同样的问题我用了libsvm实验速度上相差太大，libsvm实验5次，每次将近10分钟，时间是其次，发现一个问题就是，libsvm比liblinear的结果相差1个百分点，没有读liblinear的文章，不知道问题出在那个地方，libsvm我直接用的默认参数，线性模型。这样必然引起一个问题，如果我想评价线性模型和非线性模型的性能，我不可能一个用liblinear一个用libsvm,如果两个都用libsvm，报告的性能肯定有一些问题。 
所以如果你的问题维度很大（线性模型就有非常好的性能），不妨考虑liblinear. “
大致看了一下libsvm和liblinear的说明文档，发现一个问题就是在线性问题上两者的目标函数就不一样，所以性能上的差异是正常的，应该说如果优化同一样的目标函数两者性能应该会差不多，但是速度很明显，liblinear快很多。 
对于什么时候用线性模型的问题，我想上面的我举的例子用linear classifier就比较好，非线性分类不一定比线性分类器好，尤其是在样本及其有限，同时特征维度很高的情况下，因为样本有限的情况下，kernel map通常不准确，很有可能错误地划分类别空间，可能造成比线性模型更差的结果。 
说到scale，我建议不要用libsvm里自带的scale，因为一旦使用这个工具，它就会把原来稀疏的数据，变成非稀疏的格式，这样不但会生成非常大的数据文件，而且liblinear对稀疏数据快速处理的优势就不能体现出来了。因此，要scale，就自己写一个，以保持原来稀疏的格式
liblinear的好处就是速度快，尤其是对稀疏的特征。缺点就是太吃内存了。10G的数据量需要接近50G的内存，数据量再大就没法做了 。
另外，还有一个经常提到的svm库SVM-per：[http://www.cs.cornell.edu/people/tj/svm_light/svm_perf.html](http://www.cs.cornell.edu/people/tj/svm_light/svm_perf.html) 是康奈尔大学的人设计的。好像对计算机硬件的性能要求比liblinear要低...有做图像处理的人使用这个svm-per代替liblinear。
另外，对于多分类问题以及核函数的选取，以下经验规则可以借鉴：
- 如果如果特征数远远大于样本数的情况下,使用线性核就可以了.
- 如果特征数和样本数都很大,例如文档分类,一般使用线性核, LIBLINEAR比LIBSVM速度要快很多.
- 如果特征数远小于样本数,这种情况一般使用RBF.但是如果一定要用线性核,则选择LIBLINEAR较好,而且使用-s 2选项。
对于多分类问题：
对于15类场景来说，每类100幅训练图像，如果直接训练一个15类的multi-class classifier，则训练文件的Label值取1～15，wi标记不用指定（default 1）。如果对于每个类单独训练一个分类器，这样就把这个类的100幅图像作为正样本（假设Label=1），而其余所有的训练图像作为负样本（共1400幅，假设Label=-1），由此可以看出正负样本不平衡，因此应该制定wi选项，具体地可以指定-w1
 14，-w-1 1( 1是缺省值），在cross validation的时候首先指定-wi，然后通过grid.py来确定(C,g)的最优值。实际做实验的时候，可以具体比较一下两种情况到底有多大差别。
### [Libsvm与Liblinear源码剖析与封装（一）](http://blog.csdn.net/zhzhl202/article/details/7438160)
# 简介
因为项目的需要，我们需要同时使用Libsvm与Liblinear，并将其封装起来做成统一调用形式，目前软件已经完成，名称为[Tmsvm](https://code.google.com/p/tmsvm/)-基于SVM的文本挖掘系统
本文就来分析一下Libsvm与Liblinear的异同点以及封装这两个软件需要注意的事宜。
关于Libsvm的源码分析，上海交通大学模式识别实验室曾经有过这方面的工作，可以从网上下载。本文也结合他之前的工作，对libsvm的剖析做一些完善。目前关于Liblinear这方面的源码分析较少，因此我们就着手分析liblinear在具体实现上一些细节。
本文所分析所对应的libsvm的版本为3.0，liblinear的版本为1.8，开发语言为Java。对于Libsvm的Java版本，原作者用Java重新实现，而不是调用C++的动态链接库。而liblinear的Java版本因为原作者只提供了一个Jar包，并没有提供源代码，所以我们使用的是一个叫做Benedikt的人实现的，其源代码可在[http://www.bwaldvogel.de/liblinear-java/](http://www.bwaldvogel.de/liblinear-java/) 进行下载。虽然是别人实现，但具体算法上和原作者基本相同。这也不影响本文对这两者进行一个综合的比较。
## 异同
关于Libsvm，台湾大学林智仁这样描述它："LIBSVM is an integrated software for support vector classification, (C-SVC, nu-SVC), regression (epsilon-SVR, nu-SVR) and distribution estimation (one-class SVM). It supports multi-class classification."即Libsvm是一个整合了支持向量机(C-SVC, nu-SVC)、回归、分布估计(one-class
 SVM)的软件。并且支持多类别的分类。而对于LIblinear，官网上是这样介绍的：”LIBLINEAR is a linear classifier for data with millions of instances and features“，即主要专门为百万级别的数据和特征实现的线性分类器。
他们两个都是用来做分类的，相对来说Libsvm应用的范围较广， 而Liblinear主要用于处理大数据量的训练过程。在什么样的情况下，该选择Liblinear而不是Libsvm呢？作者给出几点建议：
-  当你面对海量的数据时，这里的海量通常是百万级别以上。海量数据分为两个层次：样本数量和特征的数量。
- 使用线性和非线性映射训练模型得到相近的效果。
- 对模型训练的时间效率要求较高。
在这类情况下，建议你使用Liblinear，而不是libsvm。文本分类是最典型的例子，文本分类的样本量非常多，而且特征的维度也是很高，从几千-几百万的数量级，因此在做文本方面的分类时最好选择liblinear。作者给出一个例子，对比liblinear与libsvm训练效果与时间效率。数据总共包含20,242样本，每个样本都包含47,236 个特征。
**[plain]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438160#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438160#)
- % time libsvm-2.85/svm-train -c 4 -t 0 -e 0.1 -m 800 -v 5 rcv1_train.binary  
- Cross Validation Accuracy = 96.8136%  
- 345.569s  
- % time liblinear-1.21/train -c 4 -e 0.1 -v 5 rcv1_train.binary  
- Cross Validation Accuracy = 97.0161%  
- 2.944s  
## 代码风格
因为不是同一个人缩写，所以在代码风格上可以看出，原作者充满了c的风味，因为连类的名字都是用 svm_model 这样的风格，liblinear的作者显然在这这些细节上上更像Java的风格。这里我们首先来剖析一下两者的代码风格：
下面要比较的两个类为特征向量中的节点类，Libsvm定义为svm_node，而Liblinear中定义为FeatureNode。即如果一个特征向量x={0.02,0.05,0,0.03}，存储时就使用一个包含5 个svm_node 的数组来存储此4 维向量
|1|2|3|4|-1|
|----|----|----|----|----|
|0.02|0.05|0|0.03|空|
如果有一个节点的value为0，程序中是不会存储他的，上面的这个表只是为了能清晰的表示。
### Libsvm
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438160#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438160#)
- publicclass svm_node implements java.io.Serializable  
- {  
- publicint index;  
- publicdouble value;  
- }  
### Liblinear
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438160#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438160#)
- publicclass FeatureNode{  
- 
- publicfinalint index;  
- publicdouble    value;  
- 
- public FeatureNode( finalint index, finaldouble value ) {  
- if (index < 0) thrownew IllegalArgumentException("index must be >= 0");  
- this.index = index;  
- this.value = value;  
-     }  
- 
- /**
-      * @since 1.9
-      */
- publicint getIndex() {  
- return index;  
-     }  
- 
- /**
-      * @since 1.9
-      */
- publicdouble getValue() {  
- return value;  
-     }  
- 
- /**
-      * @since 1.9
-      */
- publicvoid setValue(double value) {  
- this.value = value;  
-     }  
- 
- @Override
- publicint hashCode() {  
- finalint prime = 31;  
- int result = 1;  
-         result = prime * result + index;  
- long temp;  
-         temp = Double.doubleToLongBits(value);  
-         result = prime * result + (int)(temp ^ (temp >>> 32));  
- return result;  
-     }  
- 
- @Override
- publicboolean equals(Object obj) {  
- if (this == obj) returntrue;  
- if (obj == null) returnfalse;  
- if (getClass() != obj.getClass()) returnfalse;  
-         FeatureNode other = (FeatureNode)obj;  
- if (index != other.index) returnfalse;  
- if (Double.doubleToLongBits(value) != Double.doubleToLongBits(other.value)) returnfalse;  
- returntrue;  
-     }  
- 
- @Override
- public String toString() {  
- return"FeatureNode(idx=" + index + ", value=" + value + ")";  
-     }  
- }  
从上述代码，我们理解可以看出若干端倪：
- 在类命名上，Libsvm的作者沿袭了C的风格，开头小写，两个单词之间用”_“连接。而Liblinear在继承了Java命名的一贯风格，开头大写，第二个单词也大写。
- 在类的定义上，Libsvm更像是C中的Struct，没有构造函数，只给出了成员变量，而且都是public型，也没有相应的Getter和Setter。而liblinear则遵循了Java类封装的原则。
- Liblinear除了定义构造函数、Getter、Setter外，还定义了hashCode()和equals()。如果将FeatureNode作为HashMap的Key，定义这两个函数至关重要。
好了，上面这些只是开胃小菜，接下来我们将会深入的剖析Libsvm与Liblinear的异同，以及封装他们的细节。
### [Libsvm与Liblinear源码浅析与封装（二）](http://blog.csdn.net/zhzhl202/article/details/7438313)
# 封装Libsvm与Liblinear
开篇我们基于以下场景：
       假设我们已经用libsvm或者是Liblinear训练了一个模型，现在需要读入该模型并基于该模型对一个样本进行预测，返回预测的类标签以及相应的隶属度分数。
 从封装的角度我们需要考虑一下几个问题：
-  如何规范输入，使得读入的数据在Libsvm与Liblinear上都可以直接运行
- 系统如何识别读入的模型的类别，并根据模型类别，选择Libsvm或者是Liblinear中合适的函数进行预测
- 如何预测该样本对应的标签以及得分
## 系统输入
因为我们对单个样本进行预测，所以这里直接定义单个节点的格式即可。对于多个样本，则可以循环调用。
在上一篇文章中，我们看到虽然Libsvm与Liblinear在向量节点的定义上有所差别，但是其中的成员变量还是一致的，因此为了统一，我们只需要重新定义一个新的类。
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438313#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438313#)
- publicclass SvmNode {  
- /** 封装的Libsvm与Liblinear的节点格式**/
- privatefinalint index;  
- privatedouble value;  
- 
- public SvmNode( finalint index, finaldouble value ) {  
- if (index < 0) thrownew IllegalArgumentException("index must be >= 0");  
- this.index = index;  
- this.value = value;  
-     }  
- 
- /**以下Getter和Setter、hashCode、equals函数省略**/
- 
- }  
## 模型读入
我们首先来剖析一下Libsvm与Liblinear生成的模型的文件格式
![](https://img-my.csdn.net/uploads/201204/08/1333882054_6176.jpg)
从上述文件中，我们看到两者的模型文件存在很大的差异，因此我们只需要写一个解析函数直接读取模型的格式来判断到底属于那种模型。
最简单的形式就是直接通过第一行来判断，因为其模型的文件都是固定的，所以这种最简单的方式也是很有效的。
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438313#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438313#)
- /**
-  * 读入模型文件。通过文件中的格式来判断到底属于Libsvm还是liblinear
-  * @param model_filename 模型文件保存的路径及名称
-  */
- publicvoid load_model(String model_filename){  
-     BufferedReader fp;  
- try {  
-         InputStream in = new FileInputStream(model_filename);                 
-         fp = new BufferedReader(new InputStreamReader(in,"UTF-8"));  
- 
-         String line = null;  
- try {  
- while((line = fp.readLine())!=null){              
-                 String[] arg = whitespace.split(line);  
- //通过判断文件中每行第一个字符是否包含"svm_type"或者是"solver_type"
- //来判断是Libsvm还是Liblinear
- if(arg[0].equals("svm_type")){  
-                     svm_type = "libsvm";  
-                     svmModel = svm.svm_load_model(model_filename);  
-                     nr_class = svmModel.nr_class;  
-                     linearModel = null;  
- break;  
-                 }  
- if(arg[0].equals("solver_type")){  
-                     svm_type="liblinear";  
-                     svmModel = null;  
-                     linearModel = Linear.loadModel(new File(model_filename));  
-                     nr_class = linearModel.getNrClass();  
- break;  
-                 }  
-             }  
-         } catch (IOException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-     } catch (UnsupportedEncodingException e1) {  
- // TODO Auto-generated catch block
-         e1.printStackTrace();  
-     } catch (FileNotFoundException e) {  
- // TODO Auto-generated catch block
-         e.printStackTrace();  
-     }  
- }  
## 多分类策略
这里我们研究一下Libsvm与Liblinear的多分类策略，虽然为多分类，但其实也适用于二分类。
对于Libsvm来说，采用为One-against-one的策略，而Liblinear则采用为One-against-rest的策略，关于这两类分类策略的解释，可以看下图：
![](https://img-my.csdn.net/uploads/201204/08/1333884038_9287.jpg)
我们接下来通过源代码具体分析一下：
首先来看一下Libsvm的模型训练部分：
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438313#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438313#)
- /**
-  * Libsvm的模型训练函数
-  * @param prob 输入的问题
-  * @param param 输入的参数
-  * @return 训练好的模型
-  */
- publicstatic svm_model svm_train(svm_problem prob, svm_parameter param)  
- {  
- /**为了代码的简洁性，此处删除了部分**/
- 
- /**
-      * 此处省略了部分代码：这里的代码主要做：
-      * 1、统计类别总数,同时记录类别的标号，统计每个类的样本数目
-      * 2、计算权重C
-      * 3、初始化nozero数组，便于统计SV
-      * 4、初始化概率数组
-     */
-     svm_model model = new svm_model();  
-     model.param = param;  
- 
- // 将属于同一个类别的数据组织起来
-     svm_group_classes(prob,tmp_nr_class,tmp_label,tmp_start,tmp_count,perm);  
- 
- //采用one-against-one进行分类
- int p = 0;  
- //定义决策函数数组，将每一个而分类存储起来
-     decision_function[] f = new decision_function[nr_class*(nr_class-1)/2];  
- for(i=0;i<nr_class;i++)  
- for(int j=i+1;j<nr_class;j++)  
-             svm_problem sub_prob = new svm_problem();  
- 
- //计算第i个类与第j个类的起始位置与数目。
- int si = start[i], sj = start[j];  
- int ci = count[i], cj = count[j];  
-             sub_prob.l = ci+cj;  
- //将第i个类的标签定义为+1，第j个类的标签定义为-1
- for(k=0;k<ci;k++)  
-             {  
-                 sub_prob.x[k] = x[si+k];  
-                 sub_prob.y[k] = +1;  
-             }  
- for(k=0;k<cj;k++)  
-             {  
-                 sub_prob.x[ci+k] = x[sj+k];  
-                 sub_prob.y[ci+k] = -1;  
-             }  
- //对第i个类与第j个类采用二分类策略，训练模型
-             f[p] = svm_train_one(sub_prob,param,weighted_C[i],weighted_C[j]);  
-             ++p;  
-         }  
- /**
-      * 此处省略了部分代码：这里的代码主要做：
-      * 1、统计一下nozero,如果nozero已经是真，就不变，如果为假，则改为真
-      * 2、输出模型,主要是填充svm_model
-     */
- return model;  
- }  
然后再来看一下其模型预测部分：
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438313#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438313#)
- /**
-  * Libsvm模型预测部分，预测该样本的类标签以及属于该类的分值。
-  * 这里采用的方法为投票策略
-  * @param model 已训练好的Libsvm模型
-  * @param x 一个待预测的样本向量
-  * @param dec_values 保存预测结果的向量，其维度为k*(k-1)/2
-  * @return
-  */
- publicstaticdouble svm_predict_values(svm_model model, svm_node[] x, double[] dec_values)  
- {  
- 
- //对一些变量进行初始化
- int i;  
- int nr_class = model.nr_class;  
- int l = model.l;  
- 
- double[] kvalue = newdouble[l];  
- //根据模型中的支持向量以及内核函数计算数值
- for(i=0;i<l;i++)  
-         kvalue[i] = Kernel.k_function(x,model.SV[i],model.param);  
- 
- int[] start = newint[nr_class];  
-     start[0] = 0;  
- for(i=1;i<nr_class;i++)  
-         start[i] = start[i-1]+model.nSV[i-1];  
- 
- //初始化vote向量
- int[] vote = newint[nr_class];  
- for(i=0;i<nr_class;i++)  
-         vote[i] = 0;  
- 
- //依次计算k*(k-1)/2个二分类模型的预测值
- int p=0;  
- for(i=0;i<nr_class;i++)  
- for(int j=i+1;j<nr_class;j++)  
-         {  
- double sum = 0;  
- int si = start[i];  
- int sj = start[j];  
- int ci = model.nSV[i];  
- int cj = model.nSV[j];  
- 
- //根据决策函数计算每一个二分类对样本预测的分值
- int k;  
- double[] coef1 = model.sv_coef[j-1];  
- double[] coef2 = model.sv_coef[i];  
- for(k=0;k<ci;k++)  
-                 sum += coef1[si+k] * kvalue[si+k];  
- for(k=0;k<cj;k++)  
-                 sum += coef2[sj+k] * kvalue[sj+k];  
-             sum -= model.rho[p];  
- //dec_values存储的就是第p个二分类模型对样本的预测值
-             dec_values[p] = sum;                      
- 
- //根据dec_values  值的正负决定给那个类别投票
- if(dec_values[p] > 0)  
-                 ++vote[i];  
- else
-                 ++vote[j];  
-             p++;  
-         }  
- 
- //遍历vote向量，找出具有最大投票数的所对应类别即为所预测的类标签。
- //而dec_values则存储了k*(k-1)/2个二分类模型的预测值
- int vote_max_idx = 0;  
- for(i=1;i<nr_class;i++)  
- if(vote[i] > vote[vote_max_idx])  
-             vote_max_idx = i;  
- 
- return model.label[vote_max_idx];  
- 
- }  
接下来我们分析一下Liblinear的模型训练与模型预测部分
模型训练部分：
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438313#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438313#)
- /**
-  * Liblinear模型训练部分
-  * @param prob 输入的问题
-  * @param param 输入的参数
-  * @return 返回训练好的模型
-  */publicstatic Model train(Problem prob, Parameter param) {  
- 
- /**
-      * 此处省略了部分代码：这里的代码主要做：
-      * 1、统计类别总数,同时记录类别的标号，统计每个类的样本数目
-      * 2、将属于相同类的样本分组，连续存放
-      * 3、计算权重C
-      * 4、初始化nozero数组，便于统计SV
-      * 5、初始化概率数组
-     */
- 
- //此处可以看出需要寻k个模型
- for (int i = 0; i < nr_class; i++) {  
- int si = start[i];  
- int ei = si + count[i];  
- 
- //重建子数据集，样本的特征不变，但样本的类别要改为+1/-1
- int k = 0;  
- for (; k < si; k++)  
-             sub_prob.y[k] = -1;  
- for (; k < ei; k++)  
-             sub_prob.y[k] = +1;  
- for (; k < sub_prob.l; k++)  
-             sub_prob.y[k] = -1;  
- //训练子数据集svm_train_one
-         train_one(sub_prob, param, w, weighted_C[i], param.C);  
- 
- for (int j = 0; j < n; j++)  
-             model.w[j * nr_class + i] = w[j];  
-     }  
- 
- 
- /**
-      * 此处省略了部分代码：这里的代码主要做：
-      * 1、统计一下nozero,如果nozero已经是真，就不变，如果为假，则改为真
-      * 2、输出模型,主要是填充svm_model
-     */
- 
- return model;  
- }  
模型预测部分：
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438313#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438313#)
- /**
-  * 模型预测部分，根据训练好的模型对样本进行预测。
-  * @param model 已训练好的模型
-  * @param x 样本的向量
-  * @param dec_values 每个模型预测分值
-  * @return 预测的类标签
-  */
- publicstaticint predictValues(Model model, Feature[] x, double[] dec_values) {  
- 
- //变量的初始化
- int n;  
- if (model.bias >= 0)  
-         n = model.nr_feature + 1;  
- else
-         n = model.nr_feature;  
- 
- double[] w = model.w;  
- 
- int nr_w;  
- if (model.nr_class == 2 && model.solverType != SolverType.MCSVM_CS)  
-         nr_w = 1;  
- else
-         nr_w = model.nr_class;  
- 
- for (int i = 0; i < nr_w; i++)  
-         dec_values[i] = 0;  
- 
- //计算每个模型预测分值
- for (Feature lx : x) {  
- int idx = lx.getIndex();  
- // the dimension of testing data may exceed that of training
- if (idx <= n) {  
- for (int i = 0; i < nr_w; i++) {  
-                 dec_values[i] += w[(idx - 1) * nr_w + i] * lx.getValue();  
-             }  
-         }  
-     }  
- //根据预测的分值来计算所属的标签，和Libsvm所不同的是：
- //这里采用的方式是比较那个模型预测分值大。
- if (model.nr_class == 2)  
- return (dec_values[0] > 0) ? model.label[0] : model.label[1];  
- else {  
- int dec_max_idx = 0;  
- for (int i = 1; i < model.nr_class; i++) {  
- if (dec_values[i] > dec_values[dec_max_idx]) dec_max_idx = i;  
-         }  
- return model.label[dec_max_idx];  
-     }  
- }  
综上所述，Libsvm采用的为One-against-one策略，Liblinear采用的One-against-rest策略。最后都能返回预测的类标签。但是对于由于他们采用的策略不同，导致其在预测的分值上不同。所以接下来我们将会对预测的分值进行归一化处理。
## 模型结果
模型会返回两个结果：label和score，其中label即其预测的标签。而score是该样本属于该类的隶属度，分值越大，代表属于该类的置信度越大。具体的计算方式则是根据：
![](https://img-my.csdn.net/uploads/201204/08/1333889613_5141.JPG)
其中k为所有支持判别类得个数，n为所有类别个数，si 为所有支持判别类的分数。这样就相当于把两个软件计算出来的分数进行了统一，而且经过公式的计算，会将分数的映射到[0,1]区间内，这样方便进行阈值控制。
代码为 
**[java]**[view
 plain](http://blog.csdn.net/zhzhl202/article/details/7438313#)[copy](http://blog.csdn.net/zhzhl202/article/details/7438313#)
- /**
-  * 返回具有最大投票数的标签所获得分数的总和
-  * @param des_values
-  */
- publicdouble sumPreValue(double[] des_values){  
- int size=1;  
- double init_score=0.0;  
- int k=1;  
- //对于Libsvm的类型，对其k*(k-1)/2个值进行计算
- if (this.svm_type.equals("libsvm")){  
- int n = 1+(int)Math.sqrt(2.0*des_values.length+1);  
-         size = n-1;  
- int [] vote = newint[n];  
- double[] score = newdouble[n];  
- int p =0;  
- for(int i =0;i<n-1;i++){  
- for(int j =i+1;j<n;j++){  
- if(des_values[p]>0){  
-                     vote[i]+=1;  
-                     score[i]+=Math.abs(des_values[p]);  
-                 }else{  
-                     vote[j]+=1;  
-                     score[j]+=Math.abs(des_values[p]);  
-                 }  
-                 p+=1;  
-             }  
-         }  
- int max = 0;  
- for(int i =1;i<n;i++)  
- if(vote[i]>vote[max])  
-                 max = i;  
-         k = vote[max];  
-         init_score = score[max];  
-     }  
- //对于Liblinear的类型，对其k个值进行计算
- if (this.svm_type.equals("liblinear")){  
- int n =des_values.length;  
- int max = 0;  
- for(int i =1;i<n-1;i++){  
- if(des_values[i]>des_values[max]){  
-                 max = i;  
-             }  
-         }  
-         size =1;  
-         k=1;  
-         init_score = des_values[max];  
- 
-     }  
- return init_score/(2.0*k)+k/(2.0*size) ;  
- 
- }  
以Libsvm为例，说明一下分数的计算
![](https://img-my.csdn.net/uploads/201204/08/1333891974_9022.jpg)
## 总结
这样我们通过输入一个样本，根据读入的模型，就可以真正的对样本进行预测了。而且通过归一化两者的分值，可以很好的对结果进行阈值控制。更多详细的代码请见 https://code.google.com/p/tmsvm/

