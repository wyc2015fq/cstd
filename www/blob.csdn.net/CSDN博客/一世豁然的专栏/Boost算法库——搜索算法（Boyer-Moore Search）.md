# Boost算法库——搜索算法（Boyer-Moore Search） - 一世豁然的专栏 - CSDN博客





2017年10月26日 14:10:00[一世豁然](https://me.csdn.net/Explorer_day)阅读数：404








一、概览

头文件“boyer_moore.hpp”包含用于搜索值序列的Boyer-Moore算法的实现。





Boyer-Moore字符串搜索算法是简短字符串搜索算法，它已成为实际字符串搜索文献的标准基准，Boyer-Moore算法由Bob Boyer和J. Strother Moore发表，发表于10月 1977年ACM通信的通知，该文章的副本可在[http://www.cs.utexas.edu/~moore/publications/fstrpos.pdf.](http://www.cs.utexas.edu/~moore/publications/fstrpos.pdf.)查看。





Boyer-Moore算法使用两个预计算表来提供比简单搜索更好的性能。这些表取决于正在搜索的模式，并且给予Boyer-Moore算法比一个更简单的算法更大的内存占用和启动成本，但是这些成本在搜索过程中被快速恢复，特别是如果模式长于几个元素。





然而，Boyer-Moore算法不能与比较谓词如std :: search一起使用。





命名：我将所搜索的序列称为“模式”，将序列作为“语料库”进行搜索。








二、接口

为了灵活性，Boyer-Moore算法具有两个接口; 一个基于对象的界面和一个程序界面。 基于对象的接口在构造函数中构建表，并使用operator（）来执行搜索。 程序接口构建表，并在一个步骤中进行搜索。 如果您要在多个语料库中搜索相同的模式，那么您应该使用对象接口，并且只能构建一次表。





这里是对象接口：




```cpp
template <typename patIter>
class boyer_moore {
public:
    boyer_moore ( patIter first, patIter last );
    ~boyer_moore ();

    template <typename corpusIter>
    pair<corpusIter, corpusIter> operator () ( corpusIter corpus_first, corpusIter corpus_last );
    };
```


这里是相应的程序接口：



```cpp
template <typename patIter, typename corpusIter>
pair<corpusIter, corpusIter> boyer_moore_search (
        corpusIter corpus_first, corpusIter corpus_last,
        patIter pat_first, patIter pat_last );
```


每个函数都通过两对迭代器。 前两个定义语料库，第二个定义模式。 请注意，两对不需要是相同的类型，但是它们确实需要以相同的类型“点”。 换句话说，patIter :: value_type和curpusIter :: value_type需要是相同的类型。




函数的返回值是指向语料库中模式位置的一对迭代器。 如果模式为空，则在语料库开头的空白范围返回（corpus_first，corpus_first）。 如果没有找到模式，它将返回到语料库末尾的空白范围（corpus_last，corpus_last）。








三、兼容性说明

此搜索器的早期版本只返回一个迭代器。 如[https://cplusplusmusings.wordpress.com/2016/02/01/sometimes-you-get-things-wrong/](https://cplusplusmusings.wordpress.com/2016/02/01/sometimes-you-get-things-wrong/)中所述，这是一个次优的界面选择，并从1.62.0版本开始更改。 期望单个迭代器返回值的旧代码可以通过用对的.first字段替换搜索者的operator（）的返回值来更新。





代替：




```cpp
iterator foo = searcher(a, b);
```


你现在写成：



```cpp
iterator foo = searcher(a, b).first;
```



四、性能

Boyer-Moore算法的执行时间在搜索的字符串的大小上仍然是线性的，可以比许多其他搜索算法具有显着更低的常数因子：它不需要检查要搜索的字符串的每个字符 ，而是跳过其中一些。 通常，随着正在搜索的模式变得更长，算法变得更快。 它的效率源于这样一个事实：每次尝试找到搜索字符串和正在搜索的文本之间的匹配尝试时，它使用从该尝试中获得的信息排除尽可能多的文本位置在字符串不匹配的地方。








五、内存使用

该算法分配两个内部表。 第一个与模式的长度成比例; 第二个对于模式中的“字母表”的每个成员都有一个条目。 对于（8位）字符类型，此表包含256个条目。








六、复杂性

在语料库中找到模式的最坏情况是O（N）（线性）时间; 也就是说，与搜索的语料库的长度成正比。 一般来说，搜索是亚线性的; 不需要检查语料库中的每个条目。








七、安全异常

Boyer-Moore算法的面向对象和过程版本都以值为参数，不使用传递的信息，因此两个接口都提供强大的异常保证。








八、注意

1、当使用基于对象的接口时，模式在搜索期间必须保持不变; 即从对象构建到对operator（）的最后调用返回的时间。

2、Boyer-Moore算法需要模式和语料库的随机访问迭代器。







九、定制点

Boyer-Moore对象采用traits模板参数，该参数允许调用者自定义如何存储预计算表之一。 称为跳过表的此表包含（逻辑地）一个条目，用于模式可以包含的每个可能的值。 当搜索8位字符数据时，该表包含256个元素。 traits类定义要使用的表。





默认的traits类对于小的“alphabets”使用了boost :: array，对于较大的字符串使用了tr1 :: unordered_map。 基于阵列的跳过表提供了出色的性能，但是当要搜索的元素的“字母表”增长时，可能会非常大。 基于unordered_map的版本只增加了模式中唯一元素的数量，但是增加了更多的堆分配，并且缓慢的查找性能。





要使用不同的跳过表，您应该定义自己的跳过表对象和您自己的traits类，并使用它们来实例化Boyer-Moore对象。 这些对象的界面被描述为TBD。




