
# THUCTC源码解读(四) - Multiangle's Notepad - CSDN博客


2016年05月23日 21:15:39[multiangle](https://me.csdn.net/u014595019)阅读数：3498所属专栏：[自然语言处理](https://blog.csdn.net/column/details/13461.html)



---

## LiblinearTextClassifier
我认为LiblinearTextClassifier是整个分类器中最核心的模块，实现了训练，分类等功能。
LiblinearTextClassifier实现的是TextClassifier的接口，实现了TextClassifier中定义的
addTrainingText, train, saveModel, loadModel, classify, saveToString, loadFromString, setMaxFeatures, getLexicon方法。这些方法的用途在后面也会一一提到。
LiblinearTextClassifier类中的变量非常多，如下所示：
```python
public
```
```python
Lexicon lexicon;
```
```python
// 词典,初始为空，从文档中训练获得
```
```python
private
```
```python
DocumentVector trainingVectorBuilder;
```
```python
// 用来构造训练特征向量
```
```python
private
```
```python
DocumentVector testVectorBuilder;
```
```python
// 用来构造待分类文本的特征向量
```
```python
private
```
```python
WordSegment seg;
```
```python
// 分词器
```
```python
//private svm_model model; // 训练好的模型
```
```python
private
```
```python
de.bwaldvogel.liblinear.Model lmodel;
```
```python
//线性分类器
```
```python
private
```
```python
int
```
```python
maxFeatures =
```
```python
5000
```
```python
;
```
```python
// 默认的最大特征数
```
```python
private
```
```python
int
```
```python
nclasses;
```
```python
// 类别数
```
```python
private
```
```python
int
```
```python
longestDoc;
```
```python
// 最长的文档向量长度，决定读取临时文件时缓冲大小
```
```python
private
```
```python
int
```
```python
ndocs;
```
```python
// 训练集的大小 （num of training docs）
```
```python
public
```
```python
ArrayList<Integer> labelIndex =
```
```python
new
```
```python
ArrayList<Integer>();
```
```python
// 类别标签
```
```python
public
```
```python
File tsCacheFile;
```
```python
// 训练集的cache文件，存放在磁盘上
```
```python
public
```
```python
DataOutputStream tsCache =
```
```python
null
```
```python
;
```
```python
// 训练集的cache输出流
```
可以看到，这里使用的是liblinear的线性分类器，默认的最大特征数是5000。
下面来看LiblinearTextClassifier的方法，这里只写了我认为比较重要的方法。那些查询，修改类内private变量的简单函数就不贴上来了。
---

### [1]构造函数
该类的构造函数有两个，分别是
```python
public
```
```python
LiblinearTextClassifier
```
```python
(
```
```python
int
```
```python
nclasses, WordSegment seg ) {
        init( nclasses, seg);
    }
```
以及
```python
public
```
```python
LiblinearTextClassifier
```
```python
(
```
```python
int
```
```python
nclasses ) {
        init( nclasses, initWordSegment());
    }
```
可以看到两者的差异很小，区别只是如果没有指定分词器seg，就调用initWordSegment函数生成一个。该函数在此类中没有定义，需要继承类去实现。在获得了分词器之后，就调用Init方法进行初始化。
---

### [2]public void init ( int nclasses, WordSegment seg)
init方法是**实质上的构造函数**，完成了各变量的初始化工作
```python
public
```
```python
void
```
```python
init
```
```python
(
```
```python
int
```
```python
nclasses, WordSegment seg) {
        lexicon =
```
```python
new
```
```python
Lexicon();
```
```python
// 初始化词典
```
```python
// 训练文档向量构造器，注意这里的权重方法采用了TfOnly形式，即与词频成正比。
```
```python
//注意这里的词频与lexicon中的词频不同，这里指的是在该文档中出现的次数，
```
```python
//而lexicon中的词频指的是所有文档中出现该词的次数
```
```python
trainingVectorBuilder =
```
```python
new
```
```python
DocumentVector(lexicon,
```
```python
new
```
```python
TfOnlyTermWeighter()); 
        testVectorBuilder =
```
```python
null
```
```python
;
```
```python
//model = null;
```
```python
lmodel =
```
```python
null
```
```python
;
```
```python
// 模型为空，需要后续导入或利用文档训练得到
```
```python
this
```
```python
.nclasses = nclasses;
        ndocs =
```
```python
0
```
```python
;
```
```python
this
```
```python
.seg = seg;
    }
```
---

### [3]public boolean loadModel(String filename)
顾名思义，**从本地文件中读取之前训练好的模型**。这样就可以直接进行分类工作，而不需进行训练。
要载入模型，需要读入lexicon和model两个文件，分别包含了词典信息和liblinear模型中的信息。读取完毕以后，锁住词典，不需加入新词，为接下来的分类做准备
与之相似的还有saveModel，流程与loadModel相反
```python
public
```
```python
boolean
```
```python
loadModel
```
```python
(String filename) {
```
```python
// filename 是一个目录名称
```
```python
File modelPath =
```
```python
new
```
```python
File(filename);
```
```python
if
```
```python
( ! modelPath.isDirectory() )
```
```python
// 判断路径合法性
```
```python
return
```
```python
false
```
```python
;
        File lexiconFile =
```
```python
new
```
```python
File( modelPath,
```
```python
"lexicon"
```
```python
);
```
```python
// 读取词典
```
```python
File modelFile =
```
```python
new
```
```python
File( modelPath,
```
```python
"model"
```
```python
);
```
```python
// 读取model(liblinear库生成)
```
```python
System.
```
```python
out
```
```python
.println(lexiconFile.getAbsolutePath());
```
```python
try
```
```python
{
```
```python
if
```
```python
( lexiconFile.exists() ) {
                lexicon.loadFromFile(lexiconFile);
```
```python
// 调用词典中的loadFromFile方法
```
```python
System.
```
```python
out
```
```python
.println(
```
```python
"lexicon exists!"
```
```python
);
            }
```
```python
else
```
```python
{
```
```python
return
```
```python
false
```
```python
;
            }
```
```python
if
```
```python
( modelFile.exists() ) {
```
```python
//this.model = svm.svm_load_model(modelFile.getAbsolutePath());
```
```python
//this.lmodel = de.bwaldvogel.liblinear.Linear.loadModel(new File(modelFile.getAbsolutePath()));
```
```python
System.
```
```python
out
```
```python
.println(
```
```python
"model exists!"
```
```python
);
```
```python
//调用liblinear中的loadModel方法
```
```python
this
```
```python
.lmodel = de.bwaldvogel.liblinear.Linear.loadModel(modelFile); 
            }
```
```python
else
```
```python
{
```
```python
return
```
```python
false
```
```python
;
            }
        }
```
```python
catch
```
```python
( Exception e ) {
```
```python
// 只要载入2个文件中有一个出错，即返回false
```
```python
return
```
```python
false
```
```python
;
        }
        lexicon.setLock(
```
```python
true
```
```python
);
```
```python
// 将词典锁定，不允许加入新词，为接下来的分类工作做准备
```
```python
trainingVectorBuilder =
```
```python
null
```
```python
;
```
```python
// trainingVectorBuilder清空(因为不需要训练了)
```
```python
testVectorBuilder =
```
```python
//生成testVectorBuilder 准备为测试文本建立文档向量
```
```python
new
```
```python
DocumentVector(lexicon,
```
```python
new
```
```python
TfIdfTermWeighter(lexicon));
```
```python
return
```
```python
true
```
```python
;
```
```python
// 所有任务完成以后，返回true
```
```python
}
```
---

### [4]public String saveToString()
这个方法需要跟loadFromString方法配对，原本想写loadFromString的，不过发现还是写saveToString比较好，如果写loadFromString的话不太好理解。
saveToString这个方法实现的功能很简单，就是将lexicon（词典）和lmodel( liblinear模型对象 )转化成字符串。但是这个字符串对于人来说是不可读的。字符串的生成过程是，将这两个对象通过ObjectOutputStream.writeObject转化成byte数组，经过Base64加密以后生成字符串。
同样的，loadFromString获取字符串，先按Base64解码成比特数组，然后利用ObjectOutputStream.writeObject读取内容，然后转型成lexicon和lmodel
```python
public
```
```python
String
```
```python
saveToString
```
```python
() {
      ByteArrayOutputStream baos =
```
```python
new
```
```python
ByteArrayOutputStream();
```
```python
try
```
```python
{
        ObjectOutputStream oos =
```
```python
new
```
```python
ObjectOutputStream(baos);
```
```python
//注意ObjectOutputStream(ByteArrayOutputStream)的用法
```
```python
oos.writeObject(
```
```python
this
```
```python
.lexicon);
```
```python
// 写入lexicon
```
```python
//oos.writeObject(this.model);
```
```python
oos.writeObject(
```
```python
this
```
```python
.lmodel);
```
```python
// 写入lmodel
```
```python
oos.close();                
      }
```
```python
catch
```
```python
(IOException e) {
        e.printStackTrace();
```
```python
return
```
```python
""
```
```python
;
```
```python
// Failed to serialize the model.
```
```python
}
      String base64 =
```
```python
new
```
```python
String(Base64.encodeBase64(baos.toByteArray()));
```
```python
return
```
```python
base64;
    }
```
---

### [5]public boolean addTrainingText(String text, int label)
**向模型中加入一篇训练文档**。要求label是小于总类别数的整数，从0开始。text代表训练文本，label表示类别编号。若加入成功，则返回true
**流程**大概如下所示 :
文本预处理( 清除空行，去掉多余的空格 )
将文本进行分词，得到String数组
遍历一遍，将生词加入lexicon
再遍历一遍，将String数组根据lexicon转换成Word数组
再遍历一遍，将Word数组中id相同的合并，最后得到Term数组，其中包含了词的id和词频
Term数组即为文档向量的稀疏化表示。
将Term数组遍历一遍，将其中值逐个写入缓存文件
可以看到这个流程总的来讲是有非常多的冗余的。。。比如说第一项的预处理，就需要遍历两次，步骤3，4，5其实可以合起来做，只遍历一次即可。只不过这样做的代价是会破坏封装性，增大模块间耦合度。如果项目大了以后可维护性是致命的。
下面上代码
publicbooleanaddTrainingText(String text,intlabel) {if( label >= nclasses || label <0) {returnfalse;// 进行label合法性检查}if( tsCache ==null) {try{// tsCacheFile = File.createTempFile("tctscache", "data");// new File(".","adsfa") 前者表示为当前目录，后面为文件名tsCacheFile =newFile(".","tctscache"+ Long.toString(System.currentTimeMillis()) +"data");
                tsCache =newDataOutputStream(newBufferedOutputStream(newFileOutputStream(tsCacheFile)));
                longestDoc =0;
            }catch(IOException e) {returnfalse;
            }
        }
        text = LangUtils.removeEmptyLines(text);
        text = LangUtils.removeExtraSpaces(text);
        String [] bigrams = seg.segment(text);// bigrams 里面存放分词结果lexicon.addDocument(bigrams);
        Word [] words = lexicon.convertDocument(bigrams);
        bigrams =null;// 将words数组转化成文档向量，第i维的值即为词典中第i个词在该文档中出现次数Term [] terms = trainingVectorBuilder.build( words,false);try{// 这边可以学习DataOutputStream的一些用法tsCache.writeInt(label);// 首先向缓存中写入该文档的分类标签以及单词数tsCache.writeInt(terms.length);if( terms.length > longestDoc ) {
                longestDoc = terms.length;// 更新longestDoc}for(inti =0; i < terms.length ; i++ ) {// 通过循环将文档向量逐个写入缓存文件tsCache.writeInt(terms[i].id);
                tsCache.writeDouble(terms[i].weight);
            }
        }catch(IOException e) {returnfalse;
        }if( ! labelIndex.contains(label) ) {//如果之前没碰到过该标签，则加入标签列表labelIndex.add(label);
        }
        ndocs++;// ndocs+1 （以后计算TF-IDF权重的时候会用到）returntrue;
    }
---
[6]selectFeaturesByChiSquare
用来从所有特征项中筛选出一部分。
所谓特征项，在这里就是指的单词。在读取文档阶段，所有生词都会加入词典成为一个特征项，而现在的任务就是从中选出一部分，比如说5000个。选择的标准有很多种，在这里是使用卡方检验(chi-square)来进行选择的。在该类中还有其他特征选择方式，比如说scalable term selection。
进行特征选择的大概流程是
遍历所有文档，得到一个矩阵featureStats和一个向量featureFreq. featureStats存储了每个分类下每个单词的出现次数，而featureFreq则存储了所有文档中每个单词总的出现次数。也就是说，featureFreq就是featureStats沿着分类方向求和的结果。
对每一个特征中的每一个分类，都求出相应的chisqr值，从这些chisqr中挑最大的作为chimax，是这个特征的一个值，作为与其他特征相比较的标准
保留前N个chimax值最大的特征项
生成选择后特征与原先特征的映射表。比如说，原先的特征数是20000，其中有一个特征的id是7000 ，那么就需要做一次映射，一边是原先的id，另一边是新特征集的序号。
具体的chi-square校验的原理就不说了，可以去其他地方搜索相关资料。
这边还是想说一下不必要的冗余。。步骤3中使用了PriorityQueue，本身就能保证有序性，而步骤4中又排了序。。。
publicMap<Integer, Integer>selectFeaturesByChiSquare( 
            File cacheFile,intfeatureSize,intkept,intndocs,intnclasses,intlongestDoc,double[] chimaxValues ) {
        System.out.println("selectFeatureBySTS : "+"featureSize = "+ featureSize +"; kept = "+ kept +"; ndocs = "+ ndocs +"; nclasses = "+ nclasses +"; longestDoc = "+ longestDoc);int[][] featureStats =newint[featureSize][nclasses];//某词在某类出现次数int[] featureFreq =newint[featureSize];//某词词频PriorityQueue<Term> selectedFeatures;//优先队列，会自动保持从小到大顺序int[] classSize =newint[nclasses];//每类多少篇文章// 统计chi-square需要的计数// 每篇文档都是 label num-of-terms [id-weight] 形式， 如此往复，所有预处理后的都放在cacheFile里面//该部分统计出每个标签下各个特征（单词）的出现次数，每个单词总的出现次数intlabel =0;intnterms =0;
        Term [] terms =newTerm[longestDoc +1];for(inti =0; i < terms.length ; i++ ) {
            terms[i] =newTerm();
        }intndocsread =0;try{
            DataInputStream dis =newDataInputStream(newBufferedInputStream(newFileInputStream(cacheFile)));while(true) {try{
                    label = dis.readInt();
                    nterms = dis.readInt();for(inti =0; i < nterms ; i++ ) { 
                        terms[i].id = dis.readInt();
                        terms[i].weight = dis.readDouble();
                    }
                }catch( EOFException e ) {break;
                }
                classSize[label] ++;for(inti =0; i < nterms ; i++ ) { 
                    Term t = terms[i];
                    featureStats[t.id][label] ++;
                    featureFreq[t.id] ++;
                }if( ndocsread++ %10000==0) {// 每处理1W篇文档输出一个信号System.err.println("scanned "+ ndocsread);
                }
            }
            dis.close();
        }catch( IOException e ) {returnnull;
        }

        System.err.println("start chi-square calculation");// 计算chi^2_avg(t)，这里利用一个优先级队列来选择chi^2最高的特征selectedFeatures =newPriorityQueue<Term>( kept +1,//使用了优先级队列newComparator<Term>() {//这边有一个匿名类，用于实现term项之间的比较publicintcompare(Term t0, Term t1) {returnDouble.compare(t0.weight, t1.weight);
                    }           
        });longA, B, C, D;for(inti =0; i < featureSize ; i++ ) {
            Word w = lexicon.getWord(i);if(w !=null) {if( w.getDocumentFrequency() ==1|| w.getName().length() >50)continue;// 剔除掉了单词过长而出现次数极少的情况（一般长度大于50的都是一些命名实体）}doublechisqr = -1;doublechimax = -1;for(intj =0; j < nclasses ; j++ ) {
                A = featureStats[i][j];
                B = featureFreq[i] - A;
                C = classSize[j] - A;
                D = ndocs - A - B - C;//System.out.println("A:"+A+" B:"+B+" C:"+C+" D:"+D);doublefractorBase = (double)( (A+C) * (B+D) * (A+B) * (C+D) );if( Double.compare(fractorBase,0.0) ==0) {
                    chisqr =0;
                }else{// 我们不用ndocs，因为所有特征的ndocs都一样//chisqr = ndocs * ( A*D -B*C) * (A*D - B*C) / fractorBase  ;chisqr = ( A*D -B*C) / fractorBase * (A*D - B*C)   ;
                }if( chisqr > chimax ) {
                    chimax = chisqr;
                }//              被注释的方法是计算chi^2_avg即概率加权平均的卡方值。我们实际用的是chimax//              chisqr += (classSize[j] / (double) ndocs) *//                      ndocs * ( A*D -B*C) * (A*D - B*C)//                      / (double)( (A+C) * (B+D) * (A+B) * (C+D) ) ;}if( chimaxValues !=null) {
                chimaxValues[i] = chimax;
            }// 更新第i个特征值的权重值Term t =newTerm();
            t.id = i;
            t.weight = chimax;
            selectedFeatures.add(t);if( selectedFeatures.size() > kept ) {// poll用于检索并移除此队列的头。在此处则就是去掉队列中weight最小的值selectedFeatures.poll();
            }
        }
        outputSecletedFeatures(selectedFeatures);
        System.err.println("generating feature map");// 生成旧id和新选择的id的对应表// 将优先队列中的元素 放入 featuresToSort 队列中// 这边明显能看出来前后程序不是一人所写// PriorityQueue 本身就是从小到大排序的 没必要再排序一遍Map<Integer, Integer> fidmap =newHashtable<Integer, Integer>(kept);
        Term [] featuresToSort =newTerm[selectedFeatures.size()];intn =0;while( selectedFeatures.size() >0) {
            Term t = selectedFeatures.poll();
            featuresToSort[n] = t;
            n++;
        }
        Arrays.sort(featuresToSort,newTerm.TermIdComparator());for(inti =0; i < featuresToSort.length ; i++ ) {
            fidmap.put(featuresToSort[i].id, i);
        }returnfidmap;
    }
---
createLiblinearProblem
这是一个与liblinear库对接的接口。通过这个方法，能够从缓存文件和选定的特征生成liblinear能够使用的数据格式。
使用liblinear需要建立一个de.bwaldvogel.liblinear.Problem()类的对象，其中需要设置如下变量：
n:  节点的向量长度
l:节点数目
x: 所有节点的向量，是一个矩阵，长为节点数，宽为特征数
y:是一个数组，存储每个节点的标签，标签值从1开始
这个方法做的工作其实挺简单，就是读取每个文档中各个单词的数目，然后使用tf-idf计算权重，然后将特征向量和标签值存入Problem对象中。
令我有点疑惑的就是代码中先从缓存将数据读入一个中间量中，再从中间量读入Problem对象。而这个中间量的唯一用途就是根据标签值从大到小对各特征向量进行排序，感觉没什么必要，反而占用大量内存，也需要消耗计算量。
代码如下:
privatede.bwaldvogel.liblinear.ProblemcreateLiblinearProblem( File cacheFile, 
            Map<Integer, Integer> selectedFeatures){
        Vector<Double> vy =newVector<Double>();
        Vector<svm_node[]> vx =newVector<svm_node[]>();//DataNode [] datanodes = new DataNode[this.ndocs];// 长度等同ndocs， 一个文档一个LdataNode节点LdataNode [] ldatanodes =newLdataNode[this.ndocs];//FeatureNode[][] lfeatureNodes;// LdataNode是自定义节点，包含标签和FeatureNode数组intlabel, nterms;
        Term [] terms =newTerm[longestDoc +1];for(inti =0; i < terms.length ; i++ ) {
            terms[i] =newTerm();
        }intndocsread =0;//add------------------------intmaxIndex=0;try{
            DataInputStream dis =newDataInputStream(newBufferedInputStream(newFileInputStream(cacheFile)));while(true) {intn =0;try{
                    label = dis.readInt();// 读取各个文档，与之前selectFeaturesByChiSquare类似nterms = dis.readInt();for(inti =0; i < nterms ; i++ ) {inttid = dis.readInt();doubletweight = dis.readDouble();
                        Integer id = selectedFeatures.get(tid);if( id !=null) {
                            terms[n].id = id;//addmaxIndex=Math.max(maxIndex, id+1);
                            Word w = lexicon.getWord(tid);intdf = w.getDocumentFrequency();
                            terms[n].weight = Math.log( tweight +1)//使用了tf-idf权重计算方法* ( Math.log( (double) ( ndocs +1) / df ) );
                            n++;//System.err.println("doc " + id + " " + w);}
                    }
                }catch( EOFException e ) {break;
                }// 归一化向量doublenormalizer =0;for(inti =0; i < n ; i++ ) { 
                    normalizer += terms[i].weight * terms[i].weight;
                }
                normalizer = Math.sqrt(normalizer);for(inti =0; i < n ; i++ ) { 
                    terms[i].weight /= normalizer;
                }//datanodes[ndocsread] = new DataNode();// 放入svm problem中ldatanodes[ndocsread] =newLdataNode();
                ldatanodes[ndocsread].llabel= label;// 往LdataNode中写入labelFeatureNode[] lx =newFeatureNode[n];//向量是一个FeatureNode数组for(inti =0; i < n ; i++ ) {// FeatureNode与Term类似，id和权重//id是从1开始的lx[i] =newFeatureNode(terms[i].id +1,terms[i].weight); 
                }                                    
                ldatanodes[ndocsread].lnodes = lx;// 将FeatureNode[] 存入LdataNode之中if( ndocsread++ %10000==0) {
                    System.err.println("scanned "+ ndocsread);
                }
            }
            dis.close();
        }catch( IOException e ) {returnnull;
        }
        assert(this.ndocs == ndocsread );
        Arrays.sort( ldatanodes );// 按照类别排序(有必要吗？)//建立liblinear problemde.bwaldvogel.liblinear.Problem lprob =newde.bwaldvogel.liblinear.Problem(); 
        System.out.println("max index: -------------------------------------: "+ maxIndex);
        lprob.n = maxIndex;// n 记录的是存储的最大id,即最大特征数lprob.l = ldatanodes.length;// l 记录的是节点数// x记录所有节点的向量。node_num * feature_sizelprob.x =newde.bwaldvogel.liblinear.FeatureNode[lprob.l][];for(inti =0; i < lprob.l ; i++ )//lprob.x[i] = datanodes[i].nodes;lprob.x[i]=ldatanodes[i].lnodes;// 存入节点向量lprob.y =newint[lprob.l];// y记录的是节点的标签for(inti =0; i < lprob.l ; i++ )
            lprob.y[i] = ldatanodes[i].llabel;returnlprob;
    }
---
[8]public boolean train()
在介绍完了各个模块以后，终于可以开始介绍最重要的train和classify方法了。train方法用于训练模型，而classify方法用于对新的文本进行分类。
虽然train和classify是两个最重要的方法，但是长度却并不长，主要有两个原因，一是之前对一些功能已经进行了比较完善的封装，二是直接调用了现成的模块，使用liblinear进行训练和分类。
训练的大概流程是:
根据卡方校验筛选特征项(selectFeaturesByChiSquare)
创建liblinear项目(createLiblinearProblem)
将词典中非特征项去掉，只保留特征项并重新排序
使用Liblinear进行训练
publicbooleantrain() {// 训练之前把训练集的cache输出流关闭try{
            tsCache.close();
        }catch(IOException e) {returnfalse;
        }//根据卡方校验筛选特征项Map<Integer, Integer> selectedFeatures = selectFeaturesByChiSquare(
                tsCacheFile, lexicon.getSize(), maxFeatures);if( selectedFeatures ==null) {returnfalse;
        }
        System.err.println("feature selection complete");///////////////////// 创建liblinear项目de.bwaldvogel.liblinear.Problem lproblem = createLiblinearProblem(tsCacheFile, selectedFeatures);
        System.err.println("liblinear problem created");//将词典中非特征项去掉，只保留特征项并重新排序lexicon = lexicon.map( selectedFeatures );
        lexicon.setLock(true);//锁定词典，此时无法加入新词tsCacheFile.delete();
        trainingVectorBuilder =null;
        testVectorBuilder =newDocumentVector(lexicon,newTfIdfTermWeighter(lexicon));
        de.bwaldvogel.liblinear.Parameter lparam =newParameter(SolverType.L1R_LR,500,0.01);
        de.bwaldvogel.liblinear.Model tempModel = de.bwaldvogel.liblinear.Linear.train(lproblem, lparam);
        System.err.println("TRAINING COMPLETE=========================================================================================");this.lmodel = tempModel;returntrue;
    }
---
[9]classify
classify，顾名思义，就是对新输入的文本进行分类的过程
分类部分的程序也比较简短，且前期的预处理与train有一定的相同之处。大概流程是：
对文本进行分词
将String数组转化成Word数组
将Word数组转化从文档向量，权重计算采用tf-idf方式
再进一步转化成FeatureNode数组
调用Liblinear进行分类处理，得到该文本属于各类的概率。
代码如下所示：
publicClassifyResult classify(Stringtext) {// 对输入文本进行分词String [] bigrams = seg.segment(text);
        Word [] words = lexicon.convertDocument(bigrams);
        bigrams =null;
        Term [] terms = testVectorBuilder.build( words,true);intm = terms.length;//svm_node[] x = new svm_node[m];FeatureNode[] lx =newFeatureNode[m];for(intj =0; j < m; j++)
        {
            lx[j] =newFeatureNode(terms[j].id +1, terms[j].weight);
        }// 此时 lx 是一个向量，是各个特征的权重组合,用于liblinear类// 这边的FeatureNode, ClassifyResult 应该都是liblinear库中元素ClassifyResult cr =newClassifyResult(-1, -Double.MAX_VALUE);//double [] probs = new double[svm.svm_get_nr_class(model)];double[] probs =newdouble[this.lmodel.getNrClass()];//svm.svm_predict_probability(model, x, probs);//de.bwaldvogel.liblinear.Linear.predictValues(lmodel, lx, probs);de.bwaldvogel.liblinear.Linear.predictProbability(lmodel, lx, probs);for(inti =0; i < probs.length; i++) {if(probs[i] > cr.prob) {
            cr.prob = probs[i];
            cr.label = i;
          }
        }// 挑出概率最高的分类returncr;
    }

