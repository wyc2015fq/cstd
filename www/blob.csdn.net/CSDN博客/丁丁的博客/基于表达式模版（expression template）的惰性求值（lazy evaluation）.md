
# 基于表达式模版（expression template）的惰性求值（lazy evaluation） - 丁丁的博客 - CSDN博客


2016年11月30日 11:11:00[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：1016


读[代码](https://github.com/ddemidov/vexcl)碰到惰性求值的模版编程技术，动手实践下。
目标：设计一个支持任意四则表达式运算的向量类（Vec）
定义一个类Vec
```python
template
```
```python
<
```
```python
typename
```
```python
T>
```
```python
struct
```
```python
Vec {    
    Vec() : len(
```
```python
0
```
```python
), dptr(
```
```python
0
```
```python
), owned(
```
```python
false
```
```python
) {}
    Vec(T* dptr,
```
```python
int
```
```python
len) : len(len), dptr(dptr), owned(
```
```python
false
```
```python
) {}
    Vec(
```
```python
int
```
```python
len) : len(len), owned(
```
```python
true
```
```python
) {
        dptr =
```
```python
new
```
```python
T[len];
    }
    ~Vec() {
```
```python
if
```
```python
(owned)
```
```python
delete
```
```python
[] dptr;   
        dptr =
```
```python
0
```
```python
;
    }
```
```python
inline
```
```python
int
```
```python
length() {
```
```python
return
```
```python
len;
    }
```
```python
inline
```
```python
T
```
```python
operator
```
```python
[] (
```
```python
int
```
```python
i)
```
```python
const
```
```python
{
```
```python
return
```
```python
dptr[i];
    }
```
```python
inline
```
```python
T &
```
```python
operator
```
```python
[] (
```
```python
int
```
```python
i) {
```
```python
return
```
```python
dptr[i];
    }
```
```python
inline
```
```python
Vec &
```
```python
operator
```
```python
= (
```
```python
const
```
```python
Vec<T>& src) {
```
```python
#pragma omp parallel for
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
; i < len; i++) {
            dptr[i] = src[i];
        }
```
```python
return
```
```python
*
```
```python
this
```
```python
;
    }
```
```python
private
```
```python
:
```
```python
int
```
```python
len;
    T* dptr;
```
```python
bool
```
```python
owned;
};
```
支持任意长度四则运算，如
```python
Vec<double>
```
```python
a
```
```python
, b, c, d, e, f
```
```python
;
```
```python
a
```
```python
= b + c - d / e * f
```
```python
;
```
first try
重定义运算符+、-、*、/。这是C++的一个常规解决方案
template<T>
Vec<T> & operator+ (Vec<T> &lhs, Vec<T> &rhs) {
    // assert(lhs.size() == rhs.size());
    Vec<T> rs(lhs.size());for(int i =0; i < lhs.size(); i++) {
        rs[i] = lhs[i] + rhs[i];
    }returnrs;
}这一方案的问题在于，运算过程中需要分配临时空间。此外，还存在多次函数调用。
更好的方案——表达式模版
这里， 我们使用表达式模版实现运算的惰性求值。不仅不需要额外的空间，也减少函数调用开销。
表达式模版
表达式（等号右边部分）可以用一个表达式树抽象的表示。其中，叶子节点（终结符）是我们的向量，它也是一种特殊的表达式。树的根节点是运算符，左右子树是子表达式。
实现
– 首先，我们定义表达式。它是所有可能表达式的父类。
template<typenameRealType>structExp {inlineconstRealType& self()const{return*static_cast<constRealType*>(this);
    }
};实质上，它只是一个wrapper。它的作用是，当我们需要将一个对象做为表达式传递是时，它将其他封装。在传递之后，通过self()函数再得到原来的对象。
例如，我们如下定义Vec：
template<T>
struct Vec: Exp<Vec<T>> {.....
}对比常规定义：
template<T>
struct Vec {......}Vec的完整定义如下：
template<typenameT>structVec :publicExp < Vec<T> > {typedefT value_type;intlen;
    T* dptr;boolowned;
    Vec(T* dptr,intlen) : len(len), dptr(dptr), owned(false) {}
    Vec(intlen) : len(len), owned(true) {
        dptr =newT[len];
    }
    ~Vec() {if(owned)delete[] dptr;   
        dptr =0;
    }inlineToperator[] (inti)const{returndptr[i];
    }template<typenameEType>inlineVec &operator= (constExp<EType>& src_) {constEType &src = src_.self();\#pragma omp parallel forfor(inti =0; i < len; i++) {
            dptr[i] = src[i];
        }return*this;
    }
};唯一需要解释是赋值操作
template<typenameEType>inlineVec &operator= (constExp<EType>& src_) {constEType &src = src_.self();\#pragma omp parallel forfor(inti =0; i < len; i++) {
            dptr[i] = src[i];
        }return*this;
    }Vec接受一个表达式，表达式必须提供operator[]函数，返回相应的值。正是由于[]的定义，使得惰性求值成为可能。
以上，我们已经有了叶子节点（Vec）。要构造表达式树，我们要定义每个中间节点和根节点。它们本质上是二元操作。
template<typenameOp,typenameTLhs,typenameTRhs>structBinaryOpExp : Exp < BinaryOpExp<Op, TLhs, TRhs> > {constTLhs &lhs;constTRhs &rhs;typedeftypenameReturnType<TLhs, TRhs>::value_type value_type;
    BinaryOpExp(constTLhs &lhs,constTRhs &rhs) : lhs(lhs), rhs(rhs) {}inlinevalue_typeoperator[] (inti)const{returnOp::eval(lhs[i], rhs[i]);
    }
};其中，ReturnType 只是一个简单的功能模版。
template<typenameTLhs,typenameTRhs>structReturnType {typedeftypenameTLhs::value_type value_type;
};作为表达式，BinaryOpExp 重载了我们需要的 operator[]。
最后要做的是，重载+号等运算符
template<typenameT>structadd {inlinestaticT eval(constT& lhs,constT& rhs) {returnlhs + rhs;
    }
};template<typenameTLhs,typenameTRhs>inlineBinaryOpExp<add<typenameReturnType<TLhs, TRhs>::value_type>, TLhs, TRhs>operator+ (constExp<TLhs> &lhs,constExp<TRhs> &rhs) {returnBinaryOpExp<detail::add<typenameReturnType<TLhs, TRhs>::value_type>, TLhs, TRhs>(lhs.self(), rhs.self());
}一个简单的测试：
intmain() {constintn =3;doublesa[n] = {1,2,3};doublesb[n] = {2,3,4};doublesc[n] = {3,4,5};doublesd[n] = {4,5,6};doublese[n] = {5,6,7};doublesf[n] = {6,7,8};
    Vec<double> A(sa, n), B(sb, n), C(sc, n), D(sd, n), E(se, n), F(sf, n);// run expression, this expression is longer:)A = B + C - D * E / F;for(inti =0; i < n; ++i) {printf("%d:%f == %f + %f - %f * %f / %f == %f\n", i,
            A[i], B[i], C[i], D[i], E[i], F[i], B[i] + C[i] - D[i] * E[i] / F[i]);
    }return0;
}输出结果：
0:1.666667==2.000000+3.000000-4.000000*5.000000/6.000000==1.6666671:2.714286==3.000000+4.000000-5.000000*6.000000/7.000000==2.7142862:3.750000==4.000000+5.000000-6.000000*7.000000/8.000000==3.750000除基本的+、-、*、\之外，我们还可以自定义二元运算符。
template<typenameOp,typenameTLhs,typenameTRhs>inlineBinaryOpExp<Op, TLhs, TRhs> F(constExp<TLhs> &lhs,constExp<TRhs> &rhs) {returnBinaryOpExp<Op, TLhs, TRhs>(lhs.self(), rhs.self());
}类似的，我们可以定义一元操作运算
template<typenameOp,typenameT>structUnaryOpExp : Exp < UnaryOpExp<Op, T> > {constT &arg;typedeftypenameT::value_type value_type;
    UnaryOpExp(constT &arg) : arg(arg) {}inlinevalue_typeoperator[] (inti)const{returnOp::eval(arg[i]);
    }
};template<typenameOp,typenameT>inlineUnaryOpExp<Op, T> F(constExp<T> &arg) {returnUnaryOpExp<Op, T>(arg.self());
}我们重载sin函数
template<typenameT>structsinOp {inlinestaticT eval(constT& arg) {returnstd::sin(arg);
    }
};template<typenameT>
UnaryOpExp<detail::sinOp<typenameT::value_type>, T>sin(constExp<T> &arg) {returnUnaryOpExp<detail::sinOp<typenameT::value_type>, T>(arg.self());
}一个简单的测试：
intmain() {
    constintn =3;
    double sa[n] = {1,2,3};
    double sb[n] = {2,3,4};
    double sc[n] = {3,4,5};
    Vec<double> A(sa, n), B(sb, n), C(sc, n);
    A =sin(B) +sin(C);for(inti =0; i < n; ++i) {printf("%d:%f== sin(%f) + sin(%f) ==%f\n", i, A[i], B[i], C[i],sin(B[i]) +sin(C[i]));
    }return0;
}输出结果如下：
0:1.050417==sin(2.000000) +sin(3.000000) ==1.0504171:-0.615682==sin(3.000000) +sin(4.000000) == -0.6156822:-1.715727==sin(4.000000) +sin(5.000000) == -1.715727

