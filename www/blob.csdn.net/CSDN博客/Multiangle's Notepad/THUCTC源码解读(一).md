
# THUCTC源码解读(一) - Multiangle's Notepad - CSDN博客


2016年05月16日 23:08:52[multiangle](https://me.csdn.net/u014595019)阅读数：5102所属专栏：[自然语言处理](https://blog.csdn.net/column/details/13461.html)



THUCTC（THU Chinese Text Classification）,是由清华大学自然语言处理实验室推出的中文文本分类工具包，能够自动实现用户自定义的文本分类语料的训练、评测、分类功能。
THUCTC中选取二字串bigram作为特征单元，特征降维方法为Chi-square，权重计算方法为tfidf，分类模型使用的是LibSVM或LibLinear。THUCTC对于开放领域的长文本具有良好的普适性，不依赖于任何中文分词工具的性能，具有准确率高、测试速度快的优点。
THUCTC项目的地址:[http://thuctc.thunlp.org/](http://thuctc.thunlp.org/)
我测试了下，虽然对长文本有良好的普适性，但是对于短文本，如新浪微博之类，准确度就不太高了，也许是因为长度过短的缘故。不过，短文本分类一直是一个比较困难的问题，而THUCTC作为比较新的文本分类工具，还是非常值得阅读下源码来了解实现机理的。
---
了解THUCTC可以先从他提供的Demo.java开始
其中的main方法如下所示
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String args[]) {
        runTrainAndTest();
```
```python
//      runLoadModelAndUse();
```
```python
//      AddFilesManuallyAndTrain();
```
```python
//      testLoadModelAndUse();
```
```python
}
```
其中有4个方法, 分别为
runTrainAndTest() 训练模型
runLoadModelAndUse() 加载模型
AddFilesManuallyAndTrain() 添加更多的训练语料并训练
testLoadModelAndUse() 测试能否加载模型
具体用法会在下面分别讲解
---
**runTrainAndTest()**
通过训练语料来训练模型。源码为
```python
public
```
```python
static
```
```python
void
```
```python
runTrainAndTest
```
```python
() {
```
```python
// 新建分类器对象
```
```python
BasicTextClassifier classifier =
```
```python
new
```
```python
BasicTextClassifier();
```
```python
// 设置参数
```
```python
String defaultArguments =
```
```python
""
```
```python
+
```
```python
"-train F:\\multiangle\\Coding!\\NLP相关资料\\THUCNews\\THUCNewsPart "
```
```python
// 训练语料的路径
```
```python
+
```
```python
"-test F:\\multiangle\\Coding!\\NLP相关资料\\THUCNews\\THUCNewsPart "
```
```python
//  + "-l C:\\Users\\do\\workspace\\TestJar\\my_novel_model "
```
```python
//  + "-cdir E:\\Corpus\\书库_cleared "
```
```python
//  + "-n 1 "
```
```python
// + "-classify E:\\Corpus\\书库_cleared\\言情小说 "  // 设置您的测试路径。一般可以设置为与训练路径相同，即把所有文档放在一起。
```
```python
+
```
```python
"-d1 0.7 "
```
```python
// 前70%用于训练
```
```python
+
```
```python
"-d2 0.3 "
```
```python
// 后30%用于测试
```
```python
+
```
```python
"-f 35000 "
```
```python
// 设置保留特征数，可以自行调节以优化性能
```
```python
+
```
```python
"-s .\\my_novel_model"
```
```python
// 将训练好的模型保存在硬盘上，便于以后测试或部署时直接读取模型，无需训练
```
```python
;
```
```python
// 初始化
```
```python
classifier.Init(defaultArguments.split(
```
```python
" "
```
```python
));
```
```python
// 运行
```
```python
classifier.runAsBigramChineseTextClassifier();
}
```
可以看到这里使用了BasicTextClassifier， 里面实现了模型的训练， 文本的分类等功能
在训练之前，需要先设置参数，包括用于训练语料的路径，训练和测试文本的比例等。
之后，调用classifier.runAsBigramChineseTextClassifier() 进行模型训练。
训练好之后，在my_novel_model文件夹下会出现几个文件,分别为
category 里面记录了类别信息
lexicon  里面记录词典的信息，包括出现次数，包含该词的文档数
maxFeatures 里面记录了该模型的特征数目
model  是liblinera类中saveModel方法生成的模型
---
**runLoadModelAndUse()**
如果有已经训练好的模型，可以加载现有的模型，然后进行分类。源码为
```python
public
```
```python
static
```
```python
void
```
```python
runLoadModelAndUse
```
```python
() {
```
```python
// 新建分类器对象
```
```python
BasicTextClassifier classifier =
```
```python
new
```
```python
BasicTextClassifier();
```
```python
// 设置分类种类，并读取模型
```
```python
//从my_novel_model\category读取分类情况
```
```python
classifier.loadCategoryListFromFile(
```
```python
"my_novel_model/category"
```
```python
);
```
```python
//为classifier设置一个linear分类器
```
```python
classifier.setTextClassifier(
```
```python
new
```
```python
LinearBigramChineseTextClassifier(
            classifier.getCategorySize()
        )
    );
```
```python
//从my_novel_model载入训练好的模型
```
```python
classifier.getTextClassifier().loadModel(
```
```python
"my_novel_model"
```
```python
);
```
```python
// 之后就可以使用分类器进行分类
```
```python
String text =
```
```python
"万达在商业地产的杀手锏核心就是订单式地产，万达凭借订单式迅速在全国滚动开疆扩土。一时之间，订单式地产风起云涌，邯郸学步者众。但最后几乎全都铩羽而归，也包括万达。我解释下，什么叫订单地产。就是地产商和品牌商家签订联合拓展协议，是个一荣俱荣一损俱损的模式。万达走到哪里，这些品牌（最初全是跨国品牌）跟到哪里开店。像后来华润系、凯德系、中粮系、深国投系都有订单地产的影子。也就是所谓的成熟商业地产模式的滚动复制。\n"
```
```python
;
```
```python
int
```
```python
topN =
```
```python
3
```
```python
;
```
```python
// 保留最有可能的3个结果
```
```python
ClassifyResult[] result = classifier.classifyText(text, topN);
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < topN; ++i) {
```
```python
// 输出分类编号，分类名称，以及概率值。
```
```python
System.
```
```python
out
```
```python
.println(result[i].label +
```
```python
"\t"
```
```python
+ 
            classifier.getCategoryName(result[i].label) +
```
```python
"\t"
```
```python
+ 
            result[i].prob); 
    }
}
```
得到的分类结果是:
4   房产  0.3241975704148429
11  科技  0.26579823573387895
10  社会  0.18590871847598023
可以看到分类还是非常准确的。
---
剩下的两个函数，AddFilesManuallyAndTrain() 和 testLoadModelAndUse() ，用法与上面两个比较类似，就不加详述了

