# Boost算法库——搜索算法（Knuth-Morris-Pratt Search） - 一世豁然的专栏 - CSDN博客





2017年10月26日 14:41:45[一世豁然](https://me.csdn.net/Explorer_day)阅读数：302








一、概论

头文件'knuth_morris_pratt.hpp'包含用于搜索值序列的Knuth-Morris-Pratt算法的实现。





Knuth-Morris-Pratt算法的基本前提是当发生不匹配时，在搜索的模式中存在可用于确定下一个匹配可以开始的位置的信息，从而能够跳过那些已经被检查语料库的某些元素。





它通过从正在搜索的模式构建一个表，通过模式中的每个元素的一个条目来实现。





这个算法是在1974年由唐纳德·克努特（Donald Knuth）和沃恩·普拉特（Vaughan Pratt 这三个在1977年联合出版的“SIAM计算机学报”http://citeseer.ist.psu.edu/context/23820/0。





然而，Knuth-Morris-Pratt算法不能与比较谓词如std :: search一起使用。








二、接口

命名：我将所搜索的序列称为“模式”，将序列作为“语料库”进行搜索。





为了灵活性，Boyer-Moore-Horspool算法具有两个接口; 一个基于对象的接口和一个程序接口。 基于对象的接口在构造函数中构建表，并使用operator（）来执行搜索。 程序接口构建表，并在一个步骤中进行搜索。 如果您要在多个语料库中搜索相同的模式，那么您应该使用对象接口，并且只能构建一次表。





这里是对象接口：




```cpp
template <typename patIter>
class boyer_moore_horspool {
public:
    boyer_moore_horspool ( patIter first, patIter last );
    ~boyer_moore_horspool ();

    template <typename corpusIter>
    pair<corpusIter, corpusIter> operator () ( corpusIter corpus_first, corpusIter corpus_last );
    };
```


这里是相应的程序接口：





```cpp
template <typename patIter, typename corpusIter>
pair<corpusIter, corpusIter> boyer_moore_horspool_search (
        corpusIter corpus_first, corpusIter corpus_last,
        patIter pat_first, patIter pat_last );
```


每个函数都能传递两对迭代器。 前一个定义语料库，第二个定义语料库。 请注意，两对不需要是相同的类型，但是它们确实需要以相同的类型“点”。 换句话说，patIter :: value_type和curpusIter :: value_type需要是相同的类型。






函数的返回值是指向语料库中模式位置的一对迭代器。 如果模式为空，则在语料库开头的空白范围返回（corpus_first，corpus_first）。 如果没有找到模式，它将返回到语料库末尾的空白范围（corpus_last，corpus_last）。








三、兼容性说明

此搜索器的早期版本只返回一个迭代器。 如[https://cplusplusmusings.wordpress.com/2016/02/01/sometimes-you-get-things-wrong/](https://cplusplusmusings.wordpress.com/2016/02/01/sometimes-you-get-things-wrong/)中所述，这是一个次优的界面选择，并从1.62.0版本开始更改。
 期望单个迭代器返回值的旧代码可以通过用对的.first字段替换搜索者的operator（）的返回值来更新。





代替：




```cpp
iterator foo = searcher(a, b);
```


你现在应该写成：





```cpp
iterator foo = searcher(a, b).first;
```





四、性能

Knuth-Morris-Pratt算法的执行时间在所搜索的字符串的大小上是线性的。 通常，随着正在搜索的模式变得更长，算法变得更快。 它的效率源于这样一个事实：每次尝试找到搜索字符串和正在搜索的文本之间的匹配尝试时，它使用从该尝试中获得的信息排除尽可能多的文本位置，其中字符串不匹配。








五、内存使用

算法一个包含每个元素一个条目的模式，加上一个额外的。 因此，当搜索1026字节的字符串时，该表将具有1027个条目。








六、复杂度

最坏的情况是O（2n），其中n是语料库的长度。 平均时间为O（n）。 最好的情况是子线性。








七、安全异常

Knuth-Morris-Pratt算法的面向对象和程序版本都以值为参数，不使用传入的信息，因此两个接口提供了强大的异常保证。








八、注意

1、当使用基于对象的接口时，模式在搜索期间必须保持不变; 即从对象构建到对operator（）的最后调用返回的时间。

2、Knuth-Morris-Pratt算法需要模式和语料库的随机访问迭代器。 应该可以写这个来使用双向迭代器（或者甚至可以转发），但是这个实现不会这样做。



