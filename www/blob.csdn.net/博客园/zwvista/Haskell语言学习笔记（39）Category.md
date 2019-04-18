# Haskell语言学习笔记（39）Category - zwvista - 博客园

## [Haskell语言学习笔记（39）Category](https://www.cnblogs.com/zwvista/p/7837003.html)

### Category

```
class Category cat where
    id :: cat a a
    (.) :: cat b c -> cat a b -> cat a c

instance Category (->) where
    id = GHC.Base.id
    (.) = (GHC.Base..)

(<<<) :: Category cat => cat b c -> cat a b -> cat a c
(<<<) = (.)
(>>>) :: Category cat => cat a b -> cat b c -> cat a c
f >>> g = g . f
```
- Category（范畴）是个类型类，它包含两个函数。

id函数是Category类型类的幺元。

(.)函数是Category类型类的组合子。

id函数和(.)函数形成了一个幺半群（Monoid）。
- 对于(->)函数操作符这个实例来说

id函数和(.)函数的定义就是GHC.Base模块中的同名函数。
- (<<<)函数与(.)同义。

(>>>) 函数则相当于 flip (.)。

### category theory（范畴论）中的 category（范畴）

在范畴论中，一个范畴包括
- A collection of objects.

一个对象的集合。
- A collection of morphisms.

一个态射的集合。每个态射绑定两个对象：一个输入，一个输出。

若态射 f 绑定了一个输入对象 A 一个输出对象 B，则它可以被记作

f : A -> B.
- A notion of composition of these morphisms.

态射的组合。

态射 g : A -> B 可以和态射 f : B -> C 组合成新的态射 f . g : A -> C。

### 范畴的法则
- 态射的组合满足结合律。即

(f . g) . h = f . (g . h)
- 态射的组合是封闭的。即

态射组合后形成的新态射仍然必须在范畴之内。
- 存在单元态射 id。即

id . g = g . id = g

### Haskell语言中的范畴

Haskell语言中的范畴被称为Hask
- Hask范畴中的对象为类型。
- Hask范畴中的态射为函数。
- Hask范畴中态射的组合子是函数 (.)。
- Hask范畴中的单元态射是函数 id。

### 范畴论中的 Functor（函子）

Functor是范畴之间的转换器。

给定范畴 C 和 D，函子 F : C -> D 可以将
- 任何 C 中的对象 A 映射成 D 中的对象 F(A)。
- 任何 C 中的态射 f : A -> B 映射成 D 中的态射 F(f) : F(A) -> F(B)。

### 范畴论中的 Monad（单子）

Monad是一种特殊的Functor。
- Monad M 的映射关系建立在某个范畴 C 自身之上。

即Monad可以被记作 M : C -> C。
- 对于范畴 C 中的任何对象 X，都存在两个特殊映射

unit : X -> M(X)

join : M(M(X)) -> M(X)


