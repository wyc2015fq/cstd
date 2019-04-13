
# vcg数据结构1 - paohui0134的博客 - CSDN博客


2016年11月08日 23:25:27[paohui0134](https://me.csdn.net/paohui0134)阅读数：211


# 基础结构1
derivation_chain.h头文件中定义了一些基础模板类，顾名思义，就是一连串的推导链表
## 最底层的默认类
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
T=
```
```python
int
```
```python
>
```
```python
class
```
```python
DefaultDeriver :
```
```python
public
```
```python
T {};
```
继承于模板类型的空类
# 单参数
## 推导链表的第一层
```python
template <
```
```python
class
```
```python
Base
```
```python
,
```
```python
template <typename>
```
```python
class
```
```python
A
```
```python
>
```
```python
class
```
```python
Arity1
```
```python
:
```
```python
public
```
```python
A
```
```python
<
```
```python
Base
```
```python
>
```
```python
{};
```
1.类Arity1是一个继承于模板参数A的类
2.A也是一个模板类
3.A的模板参数为类Base
那么，此处的A可以使用DefaultDeriver 类，此时A是一个继承于Base的类，Arity1是一个继承于A的类
## 推导链表第二层
```python
template <
```
```python
class
```
```python
Base
```
```python
,
```
```python
template <typename>
```
```python
class
```
```python
A
```
```python
,
```
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
>
```
```python
class
```
```python
B
```
```python
>
```
```python
class
```
```python
Arity2
```
```python
:
```
```python
public
```
```python
B
```
```python
<
```
```python
Arity1
```
```python
<
```
```python
Base
```
```python
,
```
```python
A
```
```python
> >
```
```python
{};
```
1.模板参数class B是一个模板类
2.Arity2继承于B，此时B的模板参数为Arity1 <`Base, A>`
`3.Arity1 <``Base, A>根据第一层的分析，是一个继承于A的类，其中A的模板参数为Base`
`事实上vcg使用的链表推导中的模板参数类都是类似于DefaultDeriver 的类，即继承自己的模板参数类，所以：`
`4.Arity2继承于B，B继承于Arity1，Arity1继承于A，A继承于Base，这就形成了推导链表`
`推导链表最后一层`
`vcg最多定义了一个12层的类：`
`template``<``class``Base,``template``<``typename``>``class``A,``template``<``typename``>``class``B,``template``<``typename``>``class``C,``template``<``typename``>``class``D,``template``<``typename``>``class``E,``template``<``typename``>``class``F,``template``<``typename``>``class``G,``template``<``typename``>``class``H,``template``<``typename``>``class``I,``template``<``typename``>``class``J,``template``<``typename``>``class``K,``template``<``typename``>``class``L>``class``Arity12:``public``L<Arity11<Base, A, B, C, D, E, F, G, H, I, J, K> > {};``那么一般情况下，Arity12继承于L，L继承于Arity11，Arity11继承于K，K继承于Arity10………B继承于Arity1，Arity1继承于A，A继承于Base。`
`通过这种方式，可以将不同的功能独立到不同的类，并通过在模板参数中使用不同的类，来达到开启不同功能的作用。`
`双参数`
`与单参数相比，模板参数类的模板参数个数为2`
`第一层`
`template <``class``Base``,``class``TA``,``template <typename,typename>``class``A``>``class``MArity1``:``public``A``<``Base``,``TA``> {``};``MArity1继承于A，A的模板参数为Base与TA`
`第二层`
`template <``class``Base``,``class``TA``,``template <typename,typename>``class``A``,``class``TB``,``template <typename,typename>``class``B``>``class``MArity2``:``public``B``<``MArity1``<``Base``,``TA``,``A``>,``TB``> {};``1.MArity2继承于B，B的模板参数为MArity1`
`最后一层`
`template <``class``Base``,``class``TA``,``template <typename,typename>``class``A``,``class``TB``,``template <typename,typename>``class``B``,``class``TC``,``template <typename,typename>``class``C``,``class``TD``,``template <typename,typename>``class``D``>``class``MArity4``:``public``D``<``MArity3``<``Base``,``TA``,``A``,``TB``,``B``,``TC``,``C``>,``TD``> {};``1.MArity4继承于D，D的模板参数为MArity3<·Base, TA,A,TB, B, TC,C>与TD`
`2.MArity3继承于C，C的模板参数为MArity2<·Base, TA,A,TB,B>与TC`
`3.MArity2继承于B，B的模板参数为MArity1<·Base, TA, A>与TB`
`4.MArity1继承于A，A的模板参数为Base与TA`
`vcg中TriMesh类继承于MArity4，其模板参数类A B C D都为Der`
`template``<``typename``T,``typename``CONT>``struct``Der:``public``MeshTypeHolder<T,CONT,``typename``CONT::value_type::IAm>{};``template``<``typename``T,``class``CONT>``struct``MeshTypeHolder< T, CONT, AllTypes::AHEdgeType>:``public``T``Der继承于MeshTypeHolder，而MeshTypeHolder继承于第一个模板参数类`
`那么在此处：`
`MArity4继承于Der,Der继承于MeshTypeHolder，MeshTypeHolder继承于MArity3，以此类推，形成推导链。`

