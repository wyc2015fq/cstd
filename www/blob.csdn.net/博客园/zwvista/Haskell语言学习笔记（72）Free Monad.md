# Haskell语言学习笔记（72）Free Monad - zwvista - 博客园

## [Haskell语言学习笔记（72）Free Monad](https://www.cnblogs.com/zwvista/p/9213331.html)

### 安装 free 包

```
$ cabal install free
Installed free-5.0.2
```

### Free Monad

```
data Free f a = Pure a | Free (f (Free f a))

instance Functor f => Functor (Free f) where
  fmap f = go where
    go (Pure a)  = Pure (f a)
    go (Free fa) = Free (go <$> fa)

instance Functor f => Monad (Free f) where
  return = Pure
  Free x >>= f = Free (fmap (>>= f) x)
  Pure x >>= f = f x
```
- data Free f a = Pure a | Free (f (Free f a))

Free f a 是一种递归数据结构。它带有两个类型参数：Functor类型参数 f 以及作为递归终点的数据类型 a。

Free 数据类型内可包含一个或多个 f，但只能包含一个 a。
- instance Functor f => Functor (Free f) where

如果 f 是 Functor，那么 Free f 就是 Functor
- instance Functor f => Monad (Free f) where

如果 f 是 Functor，那么 Free f 就是 Monad

```
证明 Free f’ 符合Funtor法则：  
1. fmap id ≡ id
即 fmap id m ≡ id m
1.1 m = Pure a 时
fmap id (Pure a) ≡ Pure (id a) ≡ Pure a ≡ id (Pure a)
1.2 m = Free (f' x) 时
fmap id (Free (f' x)) ≡ Free (fmap id (f' x)) ≡ Free (f' (id x)) ≡ Free (f' x) ≡ id (Free (f' x))
2. fmap (f . g) ≡ fmap f . fmap g
即 fmap (f . g) m ≡ (fmap f . fmap g) m
2.1 m = Pure a 时
fmap (f . g) (Pure a) ≡ Pure ((f . g) a) ≡ Pure (f (g a))
(fmap f . fmap g) (Pure a) ≡ fmap f (fmap g (Pure a)) ≡ fmap f (Pure (g a)) ≡ Pure (f (g a))
2.2 m = Free (f' x) 时
fmap (f . g) (Free (f' x)) ≡ Free (fmap (f . g) (f' x)) ≡ Free (f' ((f . g) x) ≡ Free (f' (f (g x)))
(fmap f . fmap g) (Free (f' x)) ≡ fmap f (fmap g (Free (f' x)))
≡ fmap f (Free (fmap g (f' x))) ≡ fmap f (Free (f' (g x)))
≡ Free (fmap f (f' (g x))) ≡ Free (f' (f (g x)))
```

```
证明 Free f’ 符合Monad法则：  
1. return a >>= f ≡ f a
return a >>= f ≡ Pure a >>= f ≡ f a   
2. m >>= return ≡ m
2.1 m = Pure a 时
Pure a >>= return ≡ Pure a >>= Pure ≡ Pure a
2.2 m = Free (f' x) 时
Free (f' x) >>= return
≡ Free (f' x) >>= Pure
≡ Free (fmap (>>= Pure) (f' x))
≡ Free (f' (x >>= Pure))
≡ Free (f' (Free (f' ... (Free (f' (Pure a >>= Pure))) ... )))
≡ Free (f' (Free (f' ... (Free (f' (Pure a))) ... )))
≡ Free (f' x)
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(m >>= f) >>= g
≡ (Free (f' (Free (f' ... (Free (f' (Pure a))) ... ))) >>= f) >>= g
≡ Free (f' (Free (f' ... (Free (f' (Pure a >>= f))) ... ))) >>= g
≡ Free (f' (Free (f' ... (Free (f' (f a))) ... ))) >>= g
≡ Free (f' (Free (f' ... (Free (f' (f a >>= g))) ... )))
m >>= (\x -> f x >>= g)  
≡ Free (f' (Free (f' ... (Free (f' (Pure a))) ... ))) >>= (\x -> f x >>= g)
≡ Free (f' (Free (f' ... (Free (f' (Pure a >>= (\x -> f x >>= g)))) ... )))
≡ Free (f' (Free (f' ... (Free (f' ((\x -> f x >>= g) a))) ... )))
≡ Free (f' (Free (f' ... (Free (f' (f a >>= g))) ... )))
```

```
Prelude Control.Monad.Free> :t Pure 3
Pure 3 :: Num a => Free f a
Prelude Control.Monad.Free> :t Free (Just (Pure 3))
Free (Just (Pure 3)) :: Num a => Free Maybe a
Prelude Control.Monad.Free> :t Free (Just (Free (Just (Pure 3))))
Free (Just (Free (Just (Pure 3)))) :: Num a => Free Maybe a
Prelude Control.Monad.Free> Free (Just (Free (Just (Pure 3)))) >> Free (Just (Free (Just (Pure 3))))
Free (Just (Free (Just (Free (Just (Free (Just (Pure 3))))))))
Prelude Control.Monad.Free> :t Free [Pure 3]
Free [Pure 3] :: Num a => Free [] a
Prelude Control.Monad.Free> :t Free [Free [Pure 3]]
Free [Free [Pure 3]] :: Num a => Free [] a
Prelude Control.Monad.Free> Free [Free [Pure 3]] >> Free [Free [Pure 3]]
Free [Free [Free [Free [Pure 3]]]]
```

### 应用实践

[Why free monads matter](http://www.haskellforall.com/2012/06/you-could-have-invented-free-monads.html)

Free Monad 可以用来实现语言解释器。

假设有一种Toy语言，它包含以下三种命令。

```
output b -- prints a "b" to the console
bell     -- rings the computer's bell
done     -- end of execution
```
- output 命令输出数据 b 到控制台，带参数 b。
- bell 命令响铃，不带参数。
- done 命令用于结束程序。

下面通过使用 Free Monad 来实现该语言的解释器。
`import Control.Monad.Free`
首先定义 Toy 数据类型，它是由三条命令组成的和类型：

```
data Toy b next =
    Output b next
  | Bell next
  | Done
```
- 类型参数 b 是通过 output 命令输出到控制台的数据的类型
- 类型参数 next 是下一条命令的类型

要使用 Free Monad，Toy 数据类型必须是 Functor 类型类的实例：

```
instance Functor (Toy b) where
    fmap f (Output x next) = Output x (f next)
    fmap f (Bell     next) = Bell     (f next)
    fmap f  Done           = Done
```

要避免手动实现 Functor 类型类可以使用语言扩展 DeriveFunctor

```
{-# LANGUAGE DeriveFunctor #-}

import Control.Monad.Free

data Toy b next =
    Output b next
  | Bell next
  | Done
  deriving (Functor)
```

Toy b 是 Functor，根据定义可得 Free (Toy b) 是 Free Monad。

要使用 Free Monad，所有命令都必须是 Free (Toy b) r 类 型。此时需要使用 liftF 函数。

```
output :: a -> Free (Toy a) ()
output x = liftF (Output x ())

bell :: Free (Toy a) ()
bell     = liftF (Bell     ())

done :: Free (Toy a) r
done     = liftF  Done
```

要避免这些重复定义可以使用 makeFree

```
{-# LANGUAGE DeriveFunctor #-}
{-# LANGUAGE TemplateHaskell #-}
{-# LANGUAGE FlexibleContexts #-}

import Control.Monad.Free
import Control.Monad.Free.TH

data Toy b next =
    Output b next
  | Bell next
  | Done
  deriving (Functor)

makeFree ''Toy
```

以下定义子例程 subroutine 和程序 program ：

```
subroutine :: Free (Toy Char) ()
subroutine = output 'A'

program :: Free (Toy Char) r
program = do
    subroutine
    bell
    done
```

以下定义第一个解释器：打印程序的函数 showProgram

```
showProgram :: (Show a, Show r) => Free (Toy a) r -> String
showProgram (Free (Output a x)) =
    "output " ++ show a ++ "\n" ++ showProgram x
showProgram (Free (Bell x)) =
    "bell\n" ++ showProgram x
showProgram (Free Done) =
    "done\n"
showProgram (Pure r) =
    "return " ++ show r ++ "\n"

pretty :: (Show a, Show r) => Free (Toy a) r -> IO ()
pretty = putStr . showProgram
```

以下定义第二个解释器：解释运行程序的函数 interpret

```
interpret :: (Show b) => Free (Toy b) r -> IO ()
interpret (Free (Output b x)) = print b  >> interpret x
interpret (Free (Bell     x)) = print "bell" >> interpret x
interpret (Free  Done       ) = return ()
interpret (Pure r) = return ()
```

载入程序，确认运行结果：

```
*Main> putStr (showProgram program)
output 'A'
bell
done

*Main> pretty (output 'A')
output 'A'
return ()

*Main> pretty (return 'A' >>= output)
output 'A'
return ()

*Main> pretty (output 'A' >>= return)
output 'A'
return ()

*Main> pretty ((output 'A' >> done) >> output 'C')
output 'A'
done

*Main> pretty (output 'A' >> (done >> output 'C'))
output 'A'
done

*Main> interpret program
'A'
"bell"
```

### 参考链接

[https://github.com/lotz84/haskell/blob/master/docs/free-monad.md](https://github.com/lotz84/haskell/blob/master/docs/free-monad.md)
[Free monads in 7 easy steps](https://joashc.github.io/posts/2015-09-13-free-monad-steps.html)


