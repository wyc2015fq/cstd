
# THUCTC源码解读(三) - Multiangle's Notepad - CSDN博客


2016年05月22日 11:53:35[multiangle](https://me.csdn.net/u014595019)阅读数：2460所属专栏：[自然语言处理](https://blog.csdn.net/column/details/13461.html)



## Term类
Term也是一个非常简单的类，**是文档向量(DocumentVector)的基本组成部分，一个Term表示词典中的一个词。**
存储的变量只有id和weight, id表示该Term代表的词在词典中的id，而weight表示该词在文档向量中的权重。此外，Term的内部类TermIdComparator还实现了Comparator接口，使得java能够对Term数组进行排序。
```python
public
```
```python
class
```
```python
Term
```
```python
{
```
```python
public
```
```python
int
```
```python
id;
```
```python
public
```
```python
double
```
```python
weight;
```
```python
public
```
```python
static
```
```python
class
```
```python
TermIdComparator
```
```python
implements
```
```python
Comparator
```
```python
<
```
```python
Term
```
```python
>,
```
```python
Serializable
```
```python
{
```
```python
public
```
```python
int
```
```python
compare(Term o1, Term o2) {
```
```python
return
```
```python
o1.id - o2.id;
        }
    }
```
```python
public
```
```python
static
```
```python
class
```
```python
TermWeightComparator
```
```python
implements
```
```python
Comparator
```
```python
<
```
```python
Term
```
```python
> {
```
```python
public
```
```python
int
```
```python
compare(Term o1, Term o2) {
```
```python
return
```
```python
Double.compare(o2.weight, o1.weight);
        }
    }
}
```
---

## TermWeighter类
TermWeighter是一个抽象类，需要继承类实现对Term计算权重的功能。由于在THUCTC中使用了TF-IDF的权重计算方式，所以该类需要词典来提供一个词的TF和IDF值。
```python
public
```
```python
abstract
```
```python
class
```
```python
TermWeighter
```
```python
implements
```
```python
Serializable
```
```python
{
```
```python
protected
```
```python
Lexicon lexicon;
```
```python
public
```
```python
TermWeighter
```
```python
( Lexicon l ) {
        lexicon = l;
    }
```
```python
abstract
```
```python
public
```
```python
double
```
```python
weight
```
```python
(
```
```python
int
```
```python
id,
```
```python
double
```
```python
tf,
```
```python
int
```
```python
doclen);
}
```
---

## DocumentVector类
DocumentVector，就是将一篇文档向量化表示。文档向量的长度即为词典中单词数目，向量中第i个term表示词典中的第i个词在该文档向量中的权重。
DocumentVector本身并不存储向量内容，而是向外界提供了生成文档向量的方法。DocumentVector类中只有2个变量，分别是TermWeighter和Term.TermIdComparator对象，分别实现计算权重和term数组的排序功能。
除了构造方法外，该类只有两个方法，分别是build和dotProduct，前者负责将Word数组形式的文档转化成文档向量（Term数组），后者负责两个向量的内乘，这里主要看build方法
```python
public Term [] build( Word [] doc, boolean normalized) {
```
```python
// Term 为 id-weight 表示形式
```
```python
Hashtable<Integer, Term> terms = new Hashtable<Integer, Term>();
```
```python
// 这部分是统计各terms出现次数
```
```python
for
```
```python
( Word w : doc ) {
            Term t = terms.get(w.id);
```
```python
if
```
```python
( t == null ) {
                t = new Term();
                t.id = w.id;
                t.weight =
```
```python
0
```
```python
;
                terms.put(t.id, t);
            }
            t.weight +=
```
```python
1
```
```python
;
```
```python
// 出现次数+1
```
```python
}
```
```python
// 这部分是将hashtable转化成数组并使用TermWeighter计算权重
```
```python
Term []
```
```python
vec
```
```python
= new Term[terms.size()];
        Enumeration<Term> en = terms.elements();
```
```python
//使用迭代的方法遍历，类似于Python中的迭代器
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
;
        double normalizer =
```
```python
0
```
```python
;
```
```python
while
```
```python
( en.hasMoreElements() ) {
```
```python
vec
```
```python
[i] = en.nextElement();
```
```python
vec
```
```python
[i].weight = 
                weighter.weight(
```
```python
vec
```
```python
[i].id,
```
```python
vec
```
```python
[i].weight, doc.length);
            normalizer +=
```
```python
vec
```
```python
[i].weight *
```
```python
vec
```
```python
[i].weight;
            i++;
        }
```
```python
if
```
```python
( normalized ) {
```
```python
//如果normalized=true，则进行正则化计算
```
```python
normalizer = Math.sqrt(normalizer);
```
```python
for
```
```python
( Term t :
```
```python
vec
```
```python
) {
                t.weight /= normalizer;
            }
        }
        Arrays.sort(
```
```python
vec
```
```python
, idcomp);
```
```python
//这里通过TermIdComparator来实现对term数组的排序
```
```python
return
```
```python
vec
```
```python
;
    }
```
---

## LinearBigramChineseClassifier类
这个类在Demo中出现过。虽然在demo中分类器是由BasicTextClassifier创建的，但是毫无疑问LinearBigramChineseClassifier是分类器的核心，BasicTextClassifier只是在此基础上的一层封装，实现了模型的导入导出，参数初始化等一些交互功能。
这个类继承自LiblinearTextClassifier，大部分内容由LiblinearTextClassifier实现，LiblinearTextClassifier是一个抽象类，因为其中有一个抽象的方法initWordSegment，而LinearBigramChineseClassifier实现了这个方法。
```python
public
```
```python
class
```
```python
LinearBigramChineseTextClassifier
```
```python
extends
```
```python
LiblinearTextClassifier
```
```python
{
```
```python
public
```
```python
LinearBigramChineseTextClassifier
```
```python
(
```
```python
int
```
```python
nclasses) {
```
```python
super
```
```python
(nclasses);
  }
```
```python
public
```
```python
LinearBigramChineseTextClassifier
```
```python
(
```
```python
int
```
```python
nclasses, WordSegment seg ) {
```
```python
super
```
```python
(nclasses, seg);
  }
```
```python
@Override
```
```python
protected
```
```python
WordSegment
```
```python
initWordSegment
```
```python
() {
```
```python
return
```
```python
new
```
```python
BilingualBigramWordSegment();
```
```python
// 实现了父类中的抽象方法
```
```python
}
}
```

