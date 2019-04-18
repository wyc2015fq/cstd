# Haskell语言学习笔记（22）MaybeT - zwvista - 博客园

## [Haskell语言学习笔记（22）MaybeT](https://www.cnblogs.com/zwvista/p/7606355.html)

### Monad Transformers

Monad 转换器用于将两个不同的Monad合成为一个Monad。Monad 转换器本身也是一个 Monad。

### MaybeT

MaybeT 这个 Monad 转换器通过将 Maybe Monad 封装进其它 Monad来使两者合二为一。

```
newtype MaybeT m a = MaybeT { runMaybeT :: m (Maybe a) }
instance (Monad m) => Monad (MaybeT m) where
    return = lift . return
    x >>= f = MaybeT $ do
        v <- runMaybeT x
        case v of
            Nothing -> return Nothing
            Just y  -> runMaybeT (f y)
```
- newtype MaybeT m a = MaybeT { runMaybeT :: m (Maybe a) }

MaybeT 类型是个 newtype，也就是对现有类型的封装。该类型有两个类型参数：内部 Monad 类型 m 以及基础 Monad Maybe 的参数类型 a。

MaybeT m a 封装了一个 m (Maybe a) 类型的值，通过 runMaybeT 字段可以取出这个值。
- instance (Monad m) => Monad (MaybeT m) where

如果 m 是个 Monad，那么 MaybeT m 也是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: a -> MaybeT m a

大致相当于 a -> m (Maybe a)

而 bind 操作符的类型签名为：

(>>=) :: MaybeT m a -> (a -> MaybeT m b) -> MaybeT m b

大致相当于 m (Maybe a) -> (a -> m (Maybe b)) -> m (Maybe b)
- return = lift . return

return 函数首先将类型为 a 的值封装进内部 Monad m 中，然后通过 lift 函数将它封装进 MaybeT 这个 Monad 转换器之中。

这里左侧的 return 是 MaybeT 这个 Monad 的 return，而右侧的 return 是内部 Monad m 的 return。
- x >>= f = MaybeT $ do

对比函数签名，可知 x 的类型是 MaybeT m a，大致相当于 m (Maybe a)

而 f 的类型是 a -> MaybeT m b，大致相当于 a -> m (Maybe b)
- v <- runMaybeT x

对比 x 的类型，可知 v 的类型是 Maybe a

这是因为 runMaybeT 函数让 x 脱离了 MaybeT Monad， 而 <- 运算符又让 runMaybeT x 脱离了内部 Monad m。
- case v of
- Nothing -> return Nothing

这里 return 是内部 Monad m 的 return，所以 return Nothing 的类型是 m (Maybe a)。
- Just y -> runMaybeT (f y)

f 的类型是 a -> MaybeT m b

所以 f y 的类型是 MaybeT m b

而 runMaybeT (f y) 的类型是 m (Maybe b)

```
Prelude> :m +Control.Monad.Trans.Maybe
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (return 1 :: MaybeT IO Int)
Just 1
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (return 1 :: MaybeT [] Int)
[Just 1]
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (return 1 :: MaybeT IO Int) >>= \x -> return (x * 2)
Just 2
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (return 1 :: MaybeT [] Int) >>= \x -> return (x * 2)
[Just 2]
```

### lift

```
class MonadTrans t where
    lift :: (Monad m) => m a -> t m a

instance MonadTrans MaybeT where
    lift = MaybeT . liftM Just
```

lift 函数的作用为提升已经被封装进内部 Monad 类型 m 的值，让后者进一步被封装进 Monad 转换器 t 类型中。

对于 MaybeT 这个 Monad 转换器，lift 函数的具体实现为：

首先调用 liftM Just 把内嵌在内部 Monad m 中的值装入Maybe Monad，

然后调用 MaybeT 构造器将整个值装入MaybeT Monad。

```
Prelude Control.Monad.Trans.Maybe Control.Monad.Trans> runMaybeT $ return 1 >>= lift . print
1
Just ()
Prelude Control.Monad.Trans.Maybe Control.Monad.Trans> runMaybeT $ return 1 >>= \x -> do {lift(print x); return (x * 2)}
1
Just 2
Prelude Control.Monad.Trans.Maybe Control.Monad.Trans> runMaybeT $ return 1 >>= lift . (\x -> [x, x + 1])
[Just 1,Just 2]
```

```
证明 MaybeT m 符合 Monad 法则。
1. return a >>= f ≡ f a
return a >>= f
≡ (lift . return) a >>= f
≡ lift (m a) >>= f
≡ (MaybeT . liftM Just) (m a) >>= f
≡ MaybeT (m (Just a)) >>= f 
≡ MaybeT $ runMaybeT (f a)
≡ f a
2. m >>= return ≡ m
MaybeT (m (Just x)) >>= return
≡ MaybeT $ runMaybeT (return x)
≡ MaybeT (m (Just x))
MaybeT (m Nothing) >>= return
≡ MaybeT $ (return Nothing)
≡ MaybeT (m Nothing)
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(MaybeT (m (Just x)) >>= f) >>= g ≡ f x >>= g
(MaybeT (m Nothing) >>= f) >>= g ≡ MaybeT (m Nothing) >>= g ≡ MaybeT (m Nothing)
MaybeT (m (Just x) >>= (\x -> f x >>= g) ≡ (\x -> f x >>= g) x ≡ f x >>= g
MaybeT (m Nothing) >>= (\x -> f x >>= g) ≡ MaybeT (m Nothing)
```

### liftIO

```
class (Monad m) => MonadIO m where
    liftIO :: IO a -> m a

instance MonadIO IO where
    liftIO = id

instance (MonadIO m) => MonadIO (MaybeT m) where
    liftIO = lift . liftIO
```

liftIO 函数的作用为提升已经被封装进 IO Monad 的值，让后者进一步被封装进 Monad 转换器 m 类型中。

（前提是Monad 转换器 m 类型必须内部封装了 IO Monad。）

对于 MaybeT IO 这个 Monad 转换器，liftIO 作用等同于 lift。

```
Prelude Control.Monad.Trans.Maybe Control.Monad.Trans> runMaybeT $ return 1 >>= liftIO . print
1
Just ()
Prelude Control.Monad.Trans.Maybe Control.Monad.Trans> runMaybeT $ return 1 >>= \x -> do {liftIO(print x); return (x * 2)}
1
Just 2
```

### lift 与 liftIO 的法则

```
1. lift . return ≡ return
   liftIO . return ≡ return
2. lift (m >>= f) ≡ lift m >>= (lift . f)
   liftIO (m >>= f) ≡ liftIO m >>= (liftIO . f)
```

```
证明 MaybeT 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
根据 MaybeT 中 return 函数的定义
return ≡ lift . return
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ MaybeT . liftM Just $ m >>= f
≡ MaybeT . liftM Just $ n b
≡ MaybeT (n (Just b))
lift m >>= (lift . f)
≡ (MaybeT . liftM Just $ m) >>= (MaybeT . liftM Just . f)
≡ (MaybeT (n (Just a))) >>= (\x -> MaybeT . liftM Just . f $ x)
≡ MaybeT $ runMaybeT $ MaybeT . liftM Just . f $ a
≡ MaybeT $ runMaybeT $ MaybeT . liftM Just $ n b
≡ MaybeT $ runMaybeT $ MaybeT (n (Just b))
≡ MaybeT (n (Just b))
```

### MaybeT 是 Functor 也是 Applicative

```
instance (Functor m) => Functor (MaybeT m) where
    fmap f = mapMaybeT (fmap (fmap f))

mapMaybeT :: (m (Maybe a) -> n (Maybe b)) -> MaybeT m a -> MaybeT n b
mapMaybeT f = MaybeT . f . runMaybeT
    
instance (Functor m, Monad m) => Applicative (MaybeT m) where
    pure = lift . return
    mf <*> mx = MaybeT $ do
        mb_f <- runMaybeT mf
        case mb_f of
            Nothing -> return Nothing
            Just f  -> do
                mb_x <- runMaybeT mx
                case mb_x of
                    Nothing -> return Nothing
                    Just x  -> return (Just (f x))
```

```
fmap f x
= mapMaybeT (fmap (fmap f)) x
= MaybeT . (fmap (fmap f)) . runMaybeT $ x
= MaybeT . (fmap (fmap f)) $ m (Maybe a)
= MaybeT (n (Maybe b))
```

```
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (*2) <$> (return 1 :: MaybeT IO Int)
Just 2
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (*2) <$> (return 1 :: MaybeT [] Int)
[Just 2]
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (*) <$> (return 1 :: MaybeT IO Int) <*> return 2
Just 2
Prelude Control.Monad.Trans.Maybe> runMaybeT $ (*) <$> (return 1 :: MaybeT [] Int) <*> return 2
[Just 2]
```

### MaybeT 是 Alternative 也是 MonadPlus

```
instance (Functor m, Monad m) => Alternative (MaybeT m) where
    empty = MaybeT (return Nothing)
    x <|> y = MaybeT $ do
        v <- runMaybeT x
        case v of
            Nothing -> runMaybeT y
            Just _  -> return v

instance (Monad m) => MonadPlus (MaybeT m) where
    mzero = MaybeT (return Nothing)
    mplus x y = MaybeT $ do
        v <- runMaybeT x
        case v of
            Nothing -> runMaybeT y
            Just _  -> return v
```

```
Prelude Control.Monad.Trans.Maybe Control.Applicative> runMaybeT $ (empty :: MaybeT IO Int) <|> return 2
Just 2
Prelude Control.Monad.Trans.Maybe Control.Monad> runMaybeT $ (mzero :: MaybeT IO Int) `mplus` return 2
Just 2
```

### 应用实例

```
import Control.Monad             (guard)
import Control.Monad.Trans       (lift)
import Control.Monad.Trans.Maybe (MaybeT(..), runMaybeT)

import Data.Char

funA :: MaybeT IO String
funA = do
    lift $ putStrLn "What is your name?"
    name <- lift getLine
    guard $ not (null name)
    return name

funB :: String -> MaybeT IO String
funB name = do
    lift $ putStrLn ("hello, " ++ name)
    lift $ putStrLn "how old are you?"
    age <- lift getLine
    guard (all isDigit age)
    return age

main :: IO (Maybe String)
main = runMaybeT $ do
    a <- funA
    funB a
```

```
*Main> main
What is your name?
Steven
hello, Steven
how old are you?
20
Just "20"
*Main> main
What is your name?
Steven
hello, Steven
how old are you?

Just ""
*Main> main
What is your name?

Nothing
```


