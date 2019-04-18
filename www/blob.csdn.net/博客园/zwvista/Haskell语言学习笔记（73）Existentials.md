# Haskell语言学习笔记（73）Existentials - zwvista - 博客园

## [Haskell语言学习笔记（73）Existentials](https://www.cnblogs.com/zwvista/p/9222465.html)

### Existentials（存在类型）

Existentially quantified types（Existentially types，Existentials）是一种将一组类型归为一个类型的方式。

通常在使用 type, newtype, data 定义新类型的时候，出现在右边的类型参数必须出现在左边。

存在类型可以突破此限制。

### 实例

```
{-# LANGUAGE ExistentialQuantification #-}

data ShowBox = forall s. Show s => SB s
 
heteroList :: [ShowBox]
heteroList = [SB (), SB 5, SB True]

instance Show ShowBox where
  show (SB s) = show s
 
f :: [ShowBox] -> IO ()
f xs = mapM_ print xs

main = f heteroList

{-
()
5
True
-}
```
- data ShowBox = forall s. Show s => SB s

数据构造器中（等式右边）的类型参数 s 必须是 Show 的实例类型。

等式右边（数据构造器）的类型参数 s 没有出现在等式左边（类型构造器） ，故而 ShowBox 类型是一种存在类型。
- heteroList = [SB (), SB 5, SB True]

可以看出存在类型将三种类型归为了一种类型，这种用法接近于Java语言中的接口。

```
Prelude> :set -XExistentialQuantification
Prelude> :set -XRankNTypes
Prelude> newtype Pair a b = Pair {runPair :: forall c. (a -> b -> c) -> c}
Prelude> makePair a b = Pair $ \f -> f a b
Prelude> pair = makePair "a" 'b' 
Prelude> :t pair
pair :: Pair [Char] Char
Prelude> runPair pair (\x y -> x)
"a"
Prelude> runPair pair (\x y -> y)
'b'
```


