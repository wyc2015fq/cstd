# Haskell语言学习笔记（70）NonEmpty - zwvista - 博客园

## [Haskell语言学习笔记（70）NonEmpty](https://www.cnblogs.com/zwvista/p/8955460.html)

### NonEmpty（非空列表）

```
infixr 5 :|

data NonEmpty a = a :| [a]
  deriving (Eq, Ord)

instance Functor NonEmpty where
  fmap f ~(a :| as) = f a :| fmap f as
  b <$ ~(_ :| as)   = b   :| (b <$ as)

instance Applicative NonEmpty where
  pure a = a :| []
  (<*>) = ap
  liftA2 = liftM2

instance Monad NonEmpty where
  ~(a :| as) >>= f = b :| (bs ++ bs')
    where b :| bs = f a
          bs' = as >>= toList . f
          toList ~(c :| cs) = c : cs
```

NonEmpty是一个非空的list类型，支持大多数 list 类型的操作。

```
Prelude> import Data.List.NonEmpty as NE
Prelude NE> a = 5 :| []
Prelude NE> :t a
a :: Num a => NonEmpty a
Prelude NE> NE.head a
5
Prelude NE> NE.tail a
[]
Prelude NE> b = 6 <| a
Prelude NE> b
6 :| [5]
Prelude NE> NE.map (+2) b
8 :| [7]
Prelude NE> 3 <$ b
3 :| [3]
Prelude NE> fromList [1,2,3]
1 :| [2,3]
Prelude NE> toList it
[1,2,3]
```
- :| 是构造器，a : [a] 得到列表，而 a :| [a] 得到 NonEmpty列表。
- <| 在 NonEmpty列表前端添加元素，相当于 [] 的 : 。
- fromList toList 用于实现 NonEmpty 与 [] 之间的转换


