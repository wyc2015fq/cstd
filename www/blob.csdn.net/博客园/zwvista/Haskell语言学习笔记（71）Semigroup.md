# Haskell语言学习笔记（71）Semigroup - zwvista - 博客园

## [Haskell语言学习笔记（71）Semigroup](https://www.cnblogs.com/zwvista/p/8966941.html)

### Semigroup

```
class Semigroup a where
        (<>) :: a -> a -> a
        sconcat :: NonEmpty a -> a
        stimes :: Integral b => b -> a -> a

class Semigroup a => Monoid a where
        mempty  :: a

        mappend :: a -> a -> a
        mappend = (<>)

        mconcat :: [a] -> a
        mconcat = foldr mappend mempty
```

半群（Semigroup）是个类型类，它是幺半群（Monoid）的基类。

```
Prelude Data.List.NonEmpty Data.Semigroup> Sum 3 <> Sum 4
Sum {getSum = 7}
Prelude Data.List.NonEmpty Data.Semigroup> sconcat $ Sum 3 :| [Sum 4]
Sum {getSum = 7}
Prelude Data.List.NonEmpty Data.Semigroup> [3] <> [4]
[3,4]
Prelude Data.List.NonEmpty Data.Semigroup> sconcat $ [3] :| [[4]]
[3,4]
Prelude Data.List.NonEmpty Data.Semigroup> stimes 3 [3]
[3,3,3]
Prelude Data.List.NonEmpty Data.Semigroup> (1 :| [2]) <> (1 :| [2])
1 :| [2,1,2]
Prelude Data.List.NonEmpty Data.Semigroup> stimes 3 $ 1 :| [2]
1 :| [2,1,2,1,2]
Prelude Data.List.NonEmpty Data.Semigroup> stimes 3 $ Sum 3
Sum {getSum = 9}
```

运算符 <> 的实现与 Monoid 中的 mempty 同义。

sconcat a 将 NonEmpty 列表 a 中的各个元素用运算符 <> 连接起来。

stimes b a 将 a 重复 b 次，然后用运算符 <> 连接起来。


