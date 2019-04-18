# Haskell语言学习笔记（35）Contravariant - zwvista - 博客园

## [Haskell语言学习笔记（35）Contravariant](https://www.cnblogs.com/zwvista/p/7778766.html)

### contravariant 模块

contravariant 模块需要安装

```
$ cabal install contravariant
contravariant-1.4
Prelude> :m +Data.Functor.Contravariant
Prelude Data.Functor.Contravariant>
```

### Contravariant Functor（逆变函子）

```
class Contravariant f where
  contramap :: (a -> b) -> f b -> f a

  (>$) :: b -> f b -> f a
  (>$) = contramap . const
```

Functor（函子）类型类是协变的，因此它可以被看做 Covariant Functor （协变函子）的简写。

Functor是协变，是因为它改变的是输出端，相对于函数定义来说是正向的（positive）。

与此相对，Contravariant 类型类是逆变的，它是 Contravariant Functor （逆变函子）的简写。

Contravariant是逆变，是因为它改变的是输入端，相对于函数定义来说是反向的（negative）。

### Contravariant 的法则

```
1. contramap id = id
2. contramap f . contramap g = contramap (g . f)
```

### Predicate（谓词）是个Contravariant

```
newtype Predicate a = Predicate { getPredicate :: a -> Bool }

instance Contravariant Predicate where
  contramap f g = Predicate $ getPredicate g . f
```

Predicate（谓词 ）类型封装了一个 a -> Bool 类型的函数。

```
证明 Predicate 符合 Contravariant 的法则
1. contramap id = id
contramap id p
= Predicate $ getPredicate p . id
= Predicate $ getPredicate p
= p = id p
2. contramap f . contramap g = contramap (g . f)
(contramap f . contramap g) p
= contramap f (contramap g p)
= contramap f (Predicate $ getPredicate p . g)
= Predicate $ getPredicate (Predicate $ getPredicate p . g) . f
= Predicate $ (getPredicate p . g) . f
= Predicate $ getPredicate p . g . f
contramap (g . f) p
= Predicate $ getPredicate p . (g . f)
= Predicate $ getPredicate p . g . f
```

```
import Data.Functor.Contravariant

greaterThanThree :: Predicate Int
greaterThanThree = Predicate (> 3)

lengthGTThree :: Predicate [a]
lengthGTThree = contramap length greaterThanThree

englishGTThree :: Predicate Int
englishGTThree = contramap english lengthGTThree

english :: Int -> String
english 1 = "one"
english 2 = "two"
english 3 = "three"
english 4 = "four"
english 5 = "five"
english 6 = "six"
english 7 = "seven"
english 8 = "eight"
english 9 = "nine"
english 10 = "ten"

main :: IO ()
main = print $ filter (getPredicate englishGTThree) [1..10]

-- [3,4,5,7,8,9]
```

### Comparison a 是个Contravariant

```
newtype Comparison a = Comparison { getComparison :: a -> a -> Ordering }

instance Contravariant Comparison where
  contramap f g = Comparison $ on (getComparison g) f
```

Comparison a（同类比较）类型封装了一个 a -> a -> Ordering 类型的函数（比如 compare 函数）。

```
on :: (b -> b -> c) -> (a -> b) -> a -> a -> c
(*) `on` f = \x y -> f x * f y

证明 Comparison a 符合 Contravariant 的法则
1. contramap id = id
contramap id c
= Comparison $ on (getComparison c) id
= Comparison $ \x y -> (getComparison c) (id x) (id y) 
= Comparison $ \x y -> (getComparison c) x y
= Comparison $ getComparison c
= c = id c
2. contramap f . contramap g = contramap (g . f)
(contramap f . contramap g) c
= contramap f (contramap g c)
= contramap f (Comparison $ on (getComparison c)  g)
= contramap f (Comparison $ on (getComparison c)  g)
= Comparison $ on (getComparison (Comparison $ on (getComparison c)  g))  f
= Comparison $ on (on (getComparison c)  g)  f
= Comparison $ on (\x y -> (getComparison c)  (g x) (g y))  f
= Comparison $ \x y -> (\x y -> (getComparison c)  (g x) (g y)) (f x) (f y)
= Comparison $ \x y -> (getComparison c)  (g (f x)) (g (f y))
contramap (g . f) c
= Comparison $ on (getPredicate c) (g . f)
= Comparison $ \x y (getPredicate c) ((g . f) x) ((g . f) y)
= Comparison $ \x y -> (getComparison c)  (g (f x)) (g (f y))
```

```
Prelude Data.Functor.Contravariant Data.List> sortBy (getComparison $ contramap length $ Comparison compare) ["Groovy","Java","Scala"]
["Java","Scala","Groovy"]
Prelude Data.List Data.Function> sortBy (compare `on` length) ["Groovy","Java","Scala"]
["Java","Scala","Groovy"]
Prelude Data.List> sortOn length ["Groovy","Java","Scala"]
["Java","Scala","Groovy"]
```

### Const a 是个Contravariant

```
newtype Const a b = Const { getConst :: a }

instance Contravariant (Const a) where
  contramap _ (Const a) = Const a
```

Const a b 封装了一个值 a。

```
证明 Const a 符合 Contravariant 的法则
1. contramap id = id
contramap id (Const a) = Const a = id (Const a)
2. contramap f . contramap g = contramap (g . f)
(contramap f . contramap g) (Const a)
= contramap f (contramap g (Const a))
= contramap f (Const a)
= Const a
= contramap (g . f) (Const a)
```

### 正向与反向

```
a  -- positive position
a -> Bool -- negative position
(a -> Bool) -> Bool -- positive position
((a -> Bool) -> Bool) -> Bool -- negative position
a -> Bool -> Bool = a -> (Bool -> Bool) -- negative position
```

### 参考链接

[What is a contravariant functor?](https://stackoverflow.com/questions/38034077/what-is-a-contravariant-functor)


