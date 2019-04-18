# Haskell语言学习笔记（30）MonadCont, Cont, ContT - zwvista - 博客园

## [Haskell语言学习笔记（30）MonadCont, Cont, ContT](https://www.cnblogs.com/zwvista/p/7689957.html)

### MonadCont 类型类

```
class Monad m => MonadCont m where
    callCC :: ((a -> m b) -> m a) -> m a
instance MonadCont (ContT r m) where
    callCC = ContT.callCC
```
- class Monad m => MonadCont m where

MonadState 是个类型类，它为 ContT 等封装了CPS函数的 Monad 定义了通用接口。

MonadState 只包含 callCC 一个函数。该函数为CPS函数提供了显式的流程控制功能。
- instance MonadCont (ContT r m) where

callCC = ContT.callCC

对于 ContT 这个Monad转换器来说，callCC 函数的定义由 ContT 模块来提供。注意这里点运算符的含义不是函数的合成而是受限名字。
[Hackage - Where is the MonadReader implementation for ReaderT defined?](https://stackoverflow.com/questions/31437398/hackage-where-is-the-monadreader-implementation-for-readert-defined)

### ContT Monad转换器

```
newtype ContT r m a = ContT { runContT :: (a -> m r) -> m r }
instance Monad (ContT r m) where
    return x = ContT ($ x)
    m >>= k  = ContT $ \c -> runContT m (\x -> runContT (k x) c)
```
- newtype ContT r m a = ContT { runContT :: (a -> m r) -> m r }

ContT 类型是个 newtype，也就是对现有类型的封装。该类型有三个类型参数：内部 Monad 类型参数 m，中间结果类型参数 a 以及最终结果类型参数 r。

ContT r m a 类型封装了一个CPS函数：\k -> m r（通常形式为 \k -> k a），通过 runContT 字段可以从 ContT 类型中取出这个函数。
- instance Monad (ContT r m) where

ContT r m 是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: a -> ContT r m a

大致相当于 a -> (a -> m r) -> m r

而 bind 操作符的类型签名为：

(>>=) :: ContT r m a -> (a -> ContT r m b) -> ContT r m b

大致相当于 (a -> m r) -> m r -> (a -> (b -> m r) -> m r) -> (b -> m r) -> m r
- return x = ContT ($ x)

return 函数把 a 类型的值 x 封装进了 ContT Monad 中。

return x = ContT $ \k -> k x

这里 x 的类型为 a, k 的类型为 a -> m r，k x 的类型为 m r，

\k -> k x 的类型为 (a -> m r) -> m r，

ContT $ \k -> k x 也就是 ContT ($ x) 的类型为 ContT r m a。
- m >>= k = ContT $ \c -> runContT m (\x -> runContT (k x) c)

对比函数签名，可知 m 的类型为 ContT r m a，大致相当于 (a -> m r) -> m r。

而 k 的类型为 a -> ContT r m b，大致相当于 a -> (b -> m r) -> m r。

x 的类型为 a，c 的类型为 b -> m r。

k x 的类型 ContT r m b，

runContT (k x) 的类型为 (b -> m r) -> m r，

runContT (k x) c 的类型为 m r。

\x -> runContT (k x) c 的类型为 a -> m r，

runContT m 的类型为 (a -> m r) -> m r，

runContT m (\x -> runContT (k x) c) 的类型为 m r。

\c -> runContT m (\x -> runContT (k x) c) 的类型为 (b -> m r) -> m r，

ContT $ \c -> runContT m (\x -> runContT (k x) c) 的类型为 ContT r m b。
- m >>= k = ContT $ \c -> runContT m (\x -> runContT (k x) c)

bind 操作符组合两个封装了CPS函数的 Cont Monad，计算结果仍然是一个封装了CPS函数的 Cont Monad。

这里假设 m = ContT c1 where c1 f1 = f1 a，k = \a -> ContT c2 where c2 f2 = f2 b。

bind 操作符具体计算流程如下：

首先通过 runContT m 把封装在 bind 操作符的左操作数 m 中的CPS函数 c1 取出来，

c1 函数将把类型为 a 的运算结果 x 传递给 f1，也就是 (\x -> runContT (k x) c) 这个函数，得到 runContT (k x) c 这个运算结果。

然后 runContT (k x) 再把 bind 操作符的右操作数 k 中所封装的CPS函数 c2 取出来。

c2 函数将把类型为 b 的运算结果 y 传递给 f2, 也就是最外层的函数 c，得到最终运算结果 c y。

最后通过 ContT $ \c -> c y 这一形式，把计算结果重新封装进了 ContT Monad。

```
证明ContT符合Monad法则：
1. return a >>= f ≡ f a
return a >>= f
≡ ContT (\k -> k a) >>= f
≡ ContT $ \c -> runContT (ContT (\k -> k a)) (\x -> runContT (f x) c)
≡ ContT $ \c -> (\k -> k a) (\x -> runContT (f x) c)
≡ ContT $ \c -> (\x -> runContT (f x) c) a
≡ ContT $ \c -> runContT (f a) c
≡ ContT $ runContT (f a)
≡ f a
2. m >>= return ≡ m
m >>= return
≡ ContT $ \c -> runContT m (\x -> runContT (return x) c)
≡ ContT $ \c -> runContT m (\x -> runContT (ContT $ \k -> k x) c)
≡ ContT $ \c -> runContT m (\x -> (\k -> k x) c)
≡ ContT $ \c -> runContT m (\x -> c x)
≡ ContT $ \c -> runContT m c
≡ ContT $ runContT m
≡ m
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(m >>= f) >>= g
≡ (ContT $ \c -> runContT m (\x -> runContT (f x) c)) >>= g
≡ ContT $ \c -> runContT (ContT $ \c -> runContT m (\x -> runContT (f x) c)) (\x -> runContT (g x) c)
≡ ContT $ \c -> (\c -> runContT m (\x -> runContT (f x) c)) (\x -> runContT (g x) c)
≡ ContT $ \c -> runContT m (\x -> runContT (f x) (\x -> runContT (g x) c))
m >>= (\x -> f x >>= g)
≡ ContT $ \c -> runContT m (\x -> runContT ((\x -> f x >>= g) x) c)
≡ ContT $ \c -> runContT m (\x -> runContT (f x >>= g) c)
≡ ContT $ \c -> runContT m (\x -> runContT (ContT $ \c -> runContT (f x) (\x -> runContT (g x) c)) c)
≡ ContT $ \c -> runContT m (\x -> (\c -> runContT (f x) (\x -> runContT (g x) c)) c)
≡ ContT $ \c -> runContT m (\x -> runContT (f x) (\x -> runContT (g x) c))
```

### lift liftIO 函数

```
instance MonadTrans (ContT r) where
    lift m = ContT (m >>=)

instance (MonadIO m) => MonadIO (ContT r m) where
    liftIO = lift . liftIO
```

lift m 将封装在内部 Monad m 的值封装进了 ContT Monad 之中。

lift m = ContT $ \k -> m >>= k

这里 m 的类型为 m a, k 的类型为 a -> m r，m >> k 的类型为 m r，

\k -> m >>= k 的类型为 (a -> m r) -> m r，

ContT $ \k -> m >>= k 也就是 ContT (m >>=) 的类型为 ContT r m a。

```
证明 ContT 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
lift . return $ a
≡ lift (m a)
≡ ContT (m a >>=)
≡ ContT $ \k -> m a >>= k
≡ ContT $ \k -> k a
≡ return a
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ lift (n b)
≡ ContT (n b >>=)
≡ ContT $ \k -> n b >>= k
≡ ContT $ \k -> k b
≡ return b
lift m >>= (lift . f)
≡ ContT (n a >>=) >>= \x -> ContT (f x >>=)
≡ ContT (\k -> k a) >>= \x -> ContT (\k -> f x >>= k)
≡ ContT $ \c -> runContT $ ContT (\k -> k a) (\x -> runContT $ ContT (\k -> f x >>= k) c)
≡ ContT $ \c -> (\k -> k a) (\x -> (\k -> f x >>= k) c)
≡ ContT $ \c -> (\x -> (\k -> f x >>= k) c) a
≡ ContT $ \c -> (\k -> f a >>= k) c
≡ ContT $ \c -> (\k -> n b >>= k) c
≡ ContT $ \c -> (\k -> k b) c
≡ ContT $ \c -> c b
≡ return b
```

```
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> f a = if even a then Just (a `div` 2) else Nothing
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> c = lift . f :: Int -> ContT Int Maybe Int
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> runContT (c 3) return
Nothing
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> runContT (c 4) f
Just 1
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> c = lift getLine :: ContT r IO String
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> runContT c print
abc
"abc"
```

### callCC 函数

```
callCC :: ((a -> ContT r m b) -> ContT r m a) -> ContT r m a
callCC f = ContT $ \c -> runContT (f (\x -> ContT $ \_ -> c x)) c
```

CallCC 是 Call With Current Continuation（对当前延续函数进行调用）的缩写，它为 ContT Monad 提供了退出的手段。

CallCC 有一个参数 f，CallCC f 返回一个 ContT Monad。

f 的类型是 (a -> ContT r m b) -> ContT r m a，也就是说 f 本身是个函数，它的返回值类型和 CallCC f 相同，都是一个ContT Monad。

f 通常采用 \exit -> do ... 这种形式。

```
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> runContT (callCC (\exit -> exit 1) :: ContT Int Maybe Int) return
Just 1
Prelude Control.Monad.Trans.Cont Control.Monad.Trans> runContT (callCC (\exit -> do{exit 1; return 2}) :: ContT Int Maybe Int) return
Just 1
```

也就是说只要 CallCC 的参数采用 \exit -> do {...; exit a; ...} 这种形式，

那么该函数所返回的 ContT Monad 将无视 exit a 后面的处理流程，无条件地将 a 传递给外围函数。

下面看看 CallCC 函数是如何做到这一点的。
- callCC :: ((a -> ContT r m b) -> ContT r m a) -> ContT r m a

callCC f = ContT $ \c -> runContT (f (\x -> ContT $ \_ -> c x)) c

ContT $ \c -> runContT (f (\x -> ContT $ \_ -> c x)) c 类型为 ContT r m a

\c -> runContT (f (\x -> ContT $ \_ -> c x)) c 类型为 (a -> m r) -> m r

c 的类型为 a -> m r，

runContT (f (\x -> ContT $ \_ -> c x)) 的类型为 (a -> m r) -> m r，

f (\x -> ContT $ \_ -> c x) 的类型为 ContT r m a，

f 的类型为 (a -> ContT r m b) -> ContT r m a，

\x -> ContT $ \_ -> c x 的类型为 a -> ContT r m b，x 的类型为 a，

ContT $ \_ -> c x 的类型为 ContT r m b，

\_ -> c x 的类型为 (b -> m r) -> m r，

_ 的类型为 b -> m r，

c x 的类型为 m r。
- callCC f = ContT $ \c -> runContT (f (\x -> ContT $ \_ -> c x)) c

这里假设 f = \exit -> exit a >>= k。

callCC f where f = \exit -> exit a >>= k

= ContT $ \c -> runContT (f (\x -> ContT $ \_ -> c x)) c

= ContT $ \c -> runContT ((\exit -> exit a >>= k) (\x -> ContT $ \_ -> c x)) c

= ContT $ \c -> runContT ((\x -> ContT $ \_ -> c x) a >>= k) c

= ContT $ \c -> runContT ((ContT $ \_ -> c a) >>= k) c
- m >>= k = ContT $ \c -> runContT m (\x -> runContT (k x) c)

(ContT $ \_ -> c a) >>= k

= ContT $ \c -> runContT (ContT $ \_ -> c a) (\x -> runContT (k x) c)

= ContT $ \c -> (\_ -> c a) (\x -> runContT (k x) c)

= ContT $ \c -> c a
- callCC f where f = \exit -> exit a >>= k

= ContT $ \c -> runContT ((ContT $ \_ -> c a) >>= k) c

= ContT $ \c -> runContT (ContT $ \c -> c a) c

= ContT $ \c -> (\c -> c a) c

= ContT $ \c -> c a
- callCC f = ContT $ \c -> runContT (f (\x -> ContT $ \_ -> c x)) c

这里假设 f = \exit -> exit a >>= k >>= h。

f = \exit -> exit a >>= k >>= h

= \exit -> exit a >>= (\x -> k x >> h)

= \exit -> exit a >>= k' where k' = \x -> k x >> h

callCC f where f = \exit -> exit a >>= k >>= h

= callCC f where f = \exit -> exit a >>= k' where k' = \x -> k x >> h

= ContT $ \c -> c a

从以上推导过程可以看出 CallCC 的参数如果采用 \exit -> do {...; exit a; ...} 这种形式，

该函数所返回的 ContT Monad 确实会无视 exit a 后面的处理流程，无条件地将 a 传递给外围函数。

### ContT Monad转换器的其他函数

```
evalContT :: (Monad m) => ContT r m r -> m r
evalContT m = runContT m return

mapContT :: (m r -> m r) -> ContT r m a -> ContT r m a
mapContT f m = ContT $ f . runContT m

withContT :: ((b -> m r) -> (a -> m r)) -> ContT r m a -> ContT r m b
withContT f m = ContT $ runContT m . f

resetT :: (Monad m) => ContT r m r -> ContT r' m r
resetT = lift . evalContT

shiftT :: (Monad m) => ((a -> m r) -> ContT r m r) -> ContT r m a
shiftT f = ContT (evalContT . f)

liftLocal :: (Monad m) => m r' -> ((r' -> r') -> m r -> m r) ->
    (r' -> r') -> ContT r m a -> ContT r m a
liftLocal ask local f m = ContT $ \c -> do
    r <- ask
    local f (runContT m (local (const r) . c))
```

### Cont Monad

```
cont :: ((a -> r) -> r) -> Cont r a
cont f = ContT (\c -> Identity (f (runIdentity . c)))

runCont :: Cont r a  -> (a -> r)  -> r
runCont m k = runIdentity (runContT m (Identity . k))

evalCont :: Cont r r -> r
evalCont m = runIdentity (evalContT m)

mapCont :: (r -> r) -> Cont r a -> Cont r a
mapCont f = mapContT (Identity . f . runIdentity)

withCont :: ((b -> r) -> (a -> r)) -> Cont r a -> Cont r b
withCont f = withContT ((Identity .) . f . (runIdentity .))

reset :: Cont r r -> Cont r' r
reset = resetT

shift :: ((a -> r) -> Cont r r) -> Cont r a
shift f = shiftT (f . (runIdentity .))
```

Cont Monad 是 ContT Monad（转换器） 的一个特例。

### 应用实例

```
import Control.Monad.Trans.Cont
import Control.Monad (when)

add :: Int -> Int -> Int
add x y = x + y

square :: Int -> Int
square x = x * x

add_cont :: Int -> Int -> Cont r Int
add_cont x y = return (add x y)

square_cont :: Int -> Cont r Int
square_cont x = return (square x)

pythagoras_cont :: Int -> Int -> Cont r Int
pythagoras_cont x y = do
    x_squared <- square_cont x
    y_squared <- square_cont y
    add_cont x_squared y_squared

pythagoras_cont' :: Int -> Int -> Cont r Int
pythagoras_cont' x y = callCC $ \exit -> do
    when (x < 0 || y < 0) $ exit (-1)
    x_squared <- square_cont x
    y_squared <- square_cont y
    add_cont x_squared y_squared

main = do
    runCont (pythagoras_cont 3 4) print -- 25
    runCont (pythagoras_cont' 3 4) print -- 25
    runCont (pythagoras_cont' (-3) 4) print -- -1
```

[How and why does the Haskell Cont monad work?](https://stackoverflow.com/questions/3322540/how-and-why-does-the-haskell-cont-monad-work)


