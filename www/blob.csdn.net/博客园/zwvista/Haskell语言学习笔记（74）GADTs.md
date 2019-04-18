# Haskell语言学习笔记（74）GADTs - zwvista - 博客园

## [Haskell语言学习笔记（74）GADTs](https://www.cnblogs.com/zwvista/p/9224037.html)

### GADTs

GADTs（Generalised Algebraic Data Types，广义代数数据类型）是对代数数据类型的一种扩展。

它允许在定义数据类型时明确指定类型参数的类型并使用模式匹配。

```
--ADT
data Maybe a =  
    Nothing |   
    Just a
--GADT
data Maybe a where
   Nothing  :: Maybe a
   Just :: a -> Maybe a
```

### 实例

```
{-#LANGUAGE GADTs #-}

data Expr a where
    I   :: Int  -> Expr Int
    B   :: Bool -> Expr Bool
    Add :: Expr Int -> Expr Int -> Expr Int
    Mul :: Expr Int -> Expr Int -> Expr Int
    Eq  :: Eq a => Expr a -> Expr a -> Expr Bool

eval :: Expr a -> a
eval (I n) = n
eval (B b) = b
eval (Add e1 e2) = eval e1 + eval e2
eval (Mul e1 e2) = eval e1 * eval e2
eval (Eq  e1 e2) = eval e1 == eval e2
```

数据类型 Expr a 中的类型参数被明确限定为五种类型：
- I Int
- B Bool
- Add Int Int
- Mul Int Int
- Eq (Expr a) (Expr a)

这是以往通常的代数数据类型的定义所做不到的。

```
*Main> eval (Add (I 3) (I 4))
7
*Main> eval (Mul (I 3) (I 4))
12
*Main> eval (Eq (I 3) (I 4))
False
*Main> eval (Eq (B True) (B True))
True
```


