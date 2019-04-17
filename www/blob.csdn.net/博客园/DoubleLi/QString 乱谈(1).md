# QString 乱谈(1) - DoubleLi - 博客园





- 
一个月前尝试写了一篇关于QStringLiteral，存盘时MoinMoin罢工了。吸取一点经验，还是写成短篇吧


可是，可是，QString不就是简简单单一个字符串么？能有什么可谈的。真的么...

（本文范围：Qt4）

## 字符串比较

如果翻看Qt的源码，或者Qt的(正规一点)第三方库，我们很少看到这种代码：
//QString myString
if (myString == "dbzhang800") {
}
取而代之的是
if (myString == QLatin1String("dbzhang800")) {
}
为什么不使用前者？

### 为什么？
- QT_NO_CAST_FROM_ASCII

熟悉这个宏的，应该清楚，一旦定义之后，前者将无法通过编译。
- 同样，下列代码也无法通过编译。


QString s = "China";
QString s2("China");
### 为什么？续
- 其实，即使不考虑 QT_NO_CAST_FROM_ASCII，前面的两个比较操作仍然可能会有性能区别。（特别在国内！）

在国内，很多新手喜欢使用
QTextCodec::setCodecForCStrings()
但是大家一般很少考虑到：该语句会直接影响到字符串比较操作的性能
if (myString == "dbzhang800") {
}
？？看似完全没什么联系嘛？

### 答案

看看Qt4是怎么做的(为了清晰起见，进行了删减)。
#ifndef QT_NO_CAST_FROM_ASCII
inline bool QString::operator==(const char *s) const
{
    if (QString::codecForCStrings)
        return (s1 == QString::fromAscii(s2));
    return (*this == QLatin1String(s2));
}
#endif
可是，此处用QLatin1String和QString又有什么区别？

## QLatin1String

Manual 中如是说
The QLatin1String class provides a thin wrapper around an US-ASCII/Latin-1 encoded string literal.
Latin1? 看来这个东西，对国内用户也没有多大用，简单说说好了。

我们知道，QString内部存储的是 utf16 字符串。于是
QString s = "dbzhang800";
这些一个简单的10个字符的字符串，至少需要在堆上申请20个字节的存储空间。对于程序中大量使用的单字节Latin1字符来说，这还真的有点浪费。

那么 QLatin1String 是怎么做的，是少申请了一些存储空间么？

确实少了不少：这种"..."字符串本身不就在常用区么？
QLatin1String("dbzhang800");
所以，只需要直接存储它的指针的就够了
class QLatin1String
{
public:
    QLatin1String(const char *s) : chars(s) {}
private:
    const char *chars;
只是，这还不是最优的...

## QLatin1Literal

如果仔细看QString的Manual的话，你应该注意到这个东西的存在。可是这个东西又是干嘛的？和 QLatin1String 有什么瓜葛？

额，...
- 这是一个私有类，【从Qt4.6开始出现，从Qt5.0开始消失(变成了QLatin1String的别名)】
- 与QLatin1String的最大区别是：它在构造时直接获取字符串的长度，而QLatin1String只保存一个指针。


class QLatin1Literal
{   
public: 
    template <int N>
    QLatin1Literal(const char (&str)[N])
        : m_size(N - 1), m_data(str) {}
private:
    const int m_size;
    const char * const m_data;
对比下面的两种用法：
QString type = "long";
QLatin1String("vector<") + type + QLatin1String(">::iterator")
与
QString type = "long";
QLatin1Literal("vector<") + type + QLatin1Literal(">::iterator")
后者的好处就是，不用调用strlen()来获取latin1字符串的长度。

### 为何在Qt5中消失了呢？

因为Qt5中引入了全新的 QStringLiteral，而且估计大家会比较喜欢这个东西。毕竟可以用于中文嘛。

一个问题是，在Qt5自身的源码中，何时使用QStringLiteral，何时使用QLatin1String，有时特别让人纠结。因为
QStringLiteral("dbzhang800");
QLatin1String("dbzhang800");
前者占用的常量区比较多，程序体积会稍微大一点。(但对于中文来说，完全没有这个问题)

QString内部不是存储的utf16字符串么？而QStringLiteral就是编译期直接生成utf16字符串。(当然，需要编译器的支持)

## 参考
- 
[QString与中文问题(Qt4)](http://hi.baidu.com/cyclone/blog/item/9d7293130e5a498d6538dbf1.html)

- 
[高效使用QString(Qt4)](http://blog.csdn.net/dbzhang800/article/details/6567197)

- from:http://blog.csdn.net/dbzhang800/article/details/7517422









