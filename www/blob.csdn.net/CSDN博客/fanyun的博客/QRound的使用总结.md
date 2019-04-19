# QRound的使用总结 - fanyun的博客 - CSDN博客
2017年07月30日 21:36:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1574
    Qt中qRound的实现，在负数的情况下不同于一般的四舍五入。如果负数尾数<=.5的情况下，则舍弃尾数。（注意，=的情况下也舍弃o） 如果负数尾数 >0.5的情况下，则负小数整数部分-1。    
```cpp
Q_DECL_CONSTEXPR inline int qRound(double d)
{ return d >= 0.0 ? int(d + 0.5) : int(d - double(int(d-1)) + 0.5) + int(d-1); }
/**
 * Qt中通过模板获取qptrdiff的类型
 * TODO: 知识点：我能力有限，暂时不清楚模板应用底层原理。
 * 通过模板参数直接获取相关类型
 * 似乎与工厂设计模式有些相近（只是个人理解，如有误请留言指正）
 */
template <int> struct QIntegerForSize;
template <>    struct QIntegerForSize<1> { typedef quint8  Unsigned; typedef qint8  Signed; };
template <>    struct QIntegerForSize<2> { typedef quint16 Unsigned; typedef qint16 Signed; };
template <>    struct QIntegerForSize<4> { typedef quint32 Unsigned; typedef qint32 Signed; };
template <>    struct QIntegerForSize<8> { typedef quint64 Unsigned; typedef qint64 Signed; };
template <class T> struct QIntegerForSizeof: QIntegerForSize<sizeof(T)> { };
typedef QIntegerForSizeof<void*>::Unsigned quintptr;
typedef QIntegerForSizeof<void*>::Signed qptrdiff;
typedef qptrdiff qintptr;
/* unused参数的非warnning */
#  define Q_UNUSED(x) (void)x;
/*
 * QGlobalStatic在非线程情况下为一般static实现
 * 在线程情况下使用了QAtomicPointer自动指针，然后用QGlobalStaticDeleter注册其智能指针，在QGlobalStaticDeleter析构后，自动delete掉QGlobalStatic对象。
 * TODO: 具体原因暂时不清
 */
/*
 * 因浮点数不适宜用 == 好直接比较，所以采用了浮点数精度模糊的方法
 */
Q_DECL_CONSTEXPR static inline bool qFuzzyCompare(double p1, double p2)
{  return (qAbs(p1 - p2) <= 0.000000000001 * qMin(qAbs(p1), qAbs(p2)));}
/**
 * Q_FOREACH宏末尾
 * 主要是兼顾Q_FOREACH之后的内容（用{}括起来）
 */
for (variable = *_container_.i;; __extension__ ({--_container_.brk; break;}))
{ ;}
/* 使用模板返回相关的指针，用于private class当中，避免void然后强制类型转换
 */
  template <typename T> static inline T *qGetPtrHelper(T *ptr) { return ptr; }
template <typename Wrapper> static inline typename Wrapper::pointer qGetPtrHelper(const Wrapper &p) { return p.data(); }
/*
 * Q_DECLARE_PRIVATE(Class)等宏通过类中的d_ptr和q_ptr实现private class，以实现二进制兼容
 */
/*
 * TODO: QPrivate 中的enable_if还需要了解，可借助 std::enable_if的说明
 */
```
