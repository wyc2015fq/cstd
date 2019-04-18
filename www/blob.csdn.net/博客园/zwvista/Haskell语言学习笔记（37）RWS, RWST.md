# Haskell语言学习笔记（37）RWS, RWST - zwvista - 博客园

## [Haskell语言学习笔记（37）RWS, RWST](https://www.cnblogs.com/zwvista/p/7806831.html)

### RWST Monad转换器

RWST Monad转换器是将 ReaderT, WriterT 以及 StateT 这三个Monad转换器的功能集于一体的产物。

```
newtype RWST r w s m a = RWST { runRWST :: r -> s -> m (a, s, w) }

instance (Monoid w, Monad m) => Monad (RWST r w s m) where
    return a = RWST $ \ _ s -> return (a, s, mempty)
    m >>= k  = RWST $ \ r s -> do
        ~(a, s', w)  <- runRWST m r s
        ~(b, s'',w') <- runRWST (k a) r s'
        return (b, s'', w `mappend` w')
```
- newtype RWST r w s m a = RWST { runRWST :: r -> s -> m (a, s, w) }

RWST 类型是个 newtype，也就是对现有类型的封装。该类型有五个类型参数：内部 Monad 类型参数 m，环境类型参数 r，输出信息类型参数 w，状态类型参数 s 以及结果类型参数 a。

RWST r w s m a 类型封装了一个带环境和状态的转换函数 \r s -> m (a, s', w)，通过 runRWST 字段可以从 RWST 类型中取出这个函数。

该函数接收一个环境参数 r 和一个状态参数 s，经过计算（转换）之后返回一个封装在内部 Monad m 中的三元组：输出信息 w，计算结果 a 以及新的状态 s'。
- instance (Monoid w, Monad m) => Monad (RWST r w s m) where

如果 w 是个 Monoid 并且 m 是个 Monad，那么 RWST r w s m 也是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: a -> RWST r w s m a

大致相当于 a -> r -> s -> m (a,s,w)

而 bind 函数的类型签名为：

(>>=) :: RWST r w s m a -> (a -> RWST r w s m b) -> RWST r w s m b

大致相当于 (r -> s -> m (a,s,w)) -> (a -> r -> s -> m (b,s,w)) -> (r -> s -> m (b,s,w))
- return a = RWST $ \ _ s -> return (a, s, mempty)

return 函数将 a 封装进了带环境和状态的转换函数，该函数首先把结果值设为 a，状态值 s 保持不变，输出信息为 mempty，然后把这个三元组封装进了内部 Monad m。

这里左侧的 return 是 StateT 这个 Monad 的 return，而右侧的 return 是内部 Monad m 的 return。
- m >>= k = RWST $ \ r s -> do

对比函数签名，可知 m 的类型为 RWST r w s m a，大致相当于 r -> s -> m (a,s,w)。

而 k 的类型为 a -> RWST r w s m b，大致相当于 a -> r -> s -> m (b,s,w)。

bind 操作符组合两个带环境和状态的转换函数，最终结果仍然是个带环境和状态的转换函数。
- ~(a, s', w) <- runRWST m r s

这里首先利用 runRWST 字段取出 RWST Monad m 中封装的转换函数，然后将它应用于环境参数 r 和状态值 s 之上，得到输出信息 w，结果值 a 以及新的状态值 s'。

runRWST m 让 m 脱离了 RWST 这个 Monad，而 <- 运算符让 runRWST m r s 脱离了内部 Monad m。
- ~(b, s'',w') <- runRWST (k a) r s'

根据 k 的类型 a -> RWST r w s m b，可知 k a 的类型为 RWST r w s m b，即 k a 也是一个 RWST Monad。

这里再次利用 runRWST 字段取出 RWST Monad (k a) 中封装的转换函数，然后将它应用于环境参数 r 和新的状态值 s' 之上，得到新的输出信息 w’，新的结果值 b 以及最终状态值 s''。

runRWST (k a) 让 k a 脱离了 RWST 这个 Monad，而 <- 运算符让 runRWST (k a) r s' 脱离了内部 Monad m。
- return (b, s'', w `mappend` w')

最后将状态值设为最终状态值 s''，结果值设为新的结果值 b，输出信息则设为原来的输出信息 w 与新的输出信息 w' 联结后的值。

return 函数让这个三元组重新进入内部 Monad m 之中。

```
证明 RWST r w s m 符合Monad法则：
1. return a >>= f ≡ f a
return a >>= f
≡ (RWST $ \ _ s -> return (a, s, mempty)) >>= f
≡ RWST $ \ r s -> do {~(a, s', w)  <- runRWST (RWST $ \ _ s -> return (a, s, mempty)) r s; ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- (\ _ s -> return (a, s, mempty)) r s; ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- return (a, s, mempty); ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- runRWST (f a) r s; return (b, s'', empty <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- runRWST (f a) r s; return (b, s'', w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- runRWST (f a) r s; return (b, s'', w')}
≡ RWST $ \ r s -> runRWST (f a) r s
≡ RWST $ runRWST (f a)
≡ f a
2. m >>= return ≡ m
m = RWST (\r s -> n (a, s', w))
m >>= return
≡ RWST (\r s -> n (a, s', w)) >>= return
≡ RWST $ \ r s -> do {~(a, s', w)  <- runRWST (RWST (\r s -> n (a, s', w))) r s; ~(b, s'',w') <- runRWST (RWST $ \ _ s -> return (a, s, mempty)) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- (\r s -> n (a, s', w)) r s; ~(b, s'',w') <- (\ _ s -> n (a, s, mempty)) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- n (a, s', w)); ~(b, s'',w') <- (\ _ s -> n (a, s, mempty)) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- (\ _ s -> n (a, s, mempty)) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- n (a, s', mempty); return (b, s'', w <> w')}
≡ RWST $ \ r s -> n (a, s', w <> empty)
≡ RWST $ \ r s -> n (a, s', w)
≡ m
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
假设 m = RWST (\r s -> n (a1, s1, w1)), f a1 = RWST (\r s1 -> n (a2, s2, w2)), g a2 = RWST (\r s2 -> n (a3, s3, w3))
(m >>= f) >>= g
≡ (RWST $ \ r s -> do {~(a, s', w)  <- runRWST m r s; ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}) >> g
≡ RWST $ \ r s -> do {~(a, s', w)  <- runRWST (RWST $ \ r s -> do {~(a, s', w)  <- runRWST m r s; ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}) r s; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- do {~(a, s', w)  <- runRWST (RWST (\r s -> n (a1, s1, w1))) r s; ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- do {~(a, s', w)  <- (\r s -> n (a1, s1, w1)) r s; ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- do {~(a, s', w)  <- n (a1, s1, w1); ~(b, s'',w') <- runRWST (f a) r s'; return (b, s'', w <> w')}; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- do {~(b, s'',w') <- runRWST (f a1) r s1; return (b, s'', w1 <> w')}; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- do {~(b, s'',w') <- runRWST (RWST (\r s1 -> n (a2, s2, w2))) r s1; return (b, s'', w1 <> w')}; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- do {~(b, s'',w') <- n (a2, s2, w2); return (b, s'', w1 <> w')}; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- n (a2, s2, w1 <> w2); ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- runRWST (g a2) r s2; return (b, s'', (w1 <> w2) <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- runRWST (RWST (\r s2 -> n (a3, s3, w3))) r s2; return (b, s'', (w1 <> w2) <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- (\r s2 -> n (a3, s3, w3)) r s2; return (b, s'', (w1 <> w2) <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- n (a3, s3, w3); return (b, s'', (w1 <> w2) <> w')}
≡ RWST $ \ r s -> n (a3, s3, (w1 <> w2) <> w3)
m >>= (\x -> f x >>= g)
≡ RWST $ \ r s -> do {~(a, s', w)  <- runRWST m r s; ~(b, s'',w') <- runRWST ((\x -> f x >>= g) a) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- runRWST m r s; ~(b, s'',w') <- runRWST (f a >>= g) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- runRWST m r s; ~(b, s'',w') <- runRWST (RWST $ \ r s -> do {~(a, s', w)  <- runRWST (f a) r s; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- runRWST (RWST (\r s -> n (a1, s1, w1))) r s; ~(b, s'',w') <- runRWST (RWST $ \ r s -> do {~(a, s', w)  <- runRWST (f a) r s; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- (\r s -> n (a1, s1, w1)) r s; ~(b, s'',w') <- (\ r s -> do {~(a, s', w)  <- runRWST (f a) r s; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(a, s', w)  <- n (a1, s1, w1); ~(b, s'',w') <- (\ r s -> do {~(a, s', w)  <- runRWST (f a) r s; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}) r s'; return (b, s'', w <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- (\ r s -> do {~(a, s', w)  <- runRWST (f a1) r s; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}) r s1; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- (\ r s -> do {~(a, s', w)  <- runRWST (RWST (\r s1 -> n (a2, s2, w2))) r s; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}) r s1; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- do {~(a, s', w)  <- runRWST (RWST (\r s1 -> n (a2, s2, w2))) r s1; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- do {~(a, s', w)  <- (\r s1 -> n (a2, s2, w2)) r s1; ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- do {~(a, s', w)  <- n (a2, s2, w2); ~(b, s'',w') <- runRWST (g a) r s'; return (b, s'', w <> w')}; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- do {~(b, s'',w') <- runRWST (g a2) r s2; return (b, s'', w2 <> w')}; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- do {~(b, s'',w') <- runRWST (RWST (\r s2 -> n (a3, s3, w3))) r s2; return (b, s'', w2 <> w')}; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- do {~(b, s'',w') <- (\r s2 -> n (a3, s3, w3)) r s2; return (b, s'', w2 <> w')}; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- do {~(b, s'',w') <- n (a3, s3, w3); return (b, s'', w2 <> w')}; return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> do {~(b, s'',w') <- n (a3, s3, w2 <> w3); return (b, s'', w1 <> w')}
≡ RWST $ \ r s -> n (a3, s3, w1 <> (w2 <> w3))
根据 Monoid 的法则：(x <> y) <> z = x <> (y <> z)
RWST $ \ r s -> n (a3, s3, (w1 <> w2) <> w3)
≡ RWST $ \ r s -> n (a3, s3, w1 <> (w2 <> w3))
```

### lift 函数

```
instance (Monoid w) => MonadTrans (RWST r w s) where
    lift m = RWST $ \ _ s -> do
        a <- m
        return (a, s, mempty)
```

```
证明 RWST 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
lift . return $ a
≡ lift (m a)
≡ RWST $ \ _ s -> do {a <- m a; return (a, s, mempty)}
≡ RWST $ \ _ s -> m (a, s, mempty)
≡ return a
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ lift (n b)
≡ RWST $ \ _ s -> do {a <- n b; return (a, s, empty)}
≡ RWST $ \ _ s -> n (b, s, empty)
lift m >>= (lift . f)
≡ (RWST $ \ _ s -> do {a <- n a; return (a, s, empty)}) >>= (\x -> RWST $ \ _ s -> do {a <- f x; return (a, s, empty)})
≡ (RWST $ \ _ s -> n (a, s, empty)) >>= (\x -> RWST $ \ _ s -> do {a <- f x; return (a, s, empty)})
≡ RWST $ \ _ s -> do {~(a, s', w)  <- runRWST (RWST $ \ _ s -> n (a, s, empty)) r s; ~(b, s'',w') <- runRWST ((\x -> RWST $ \ _ s -> do {a <- f x; return (a, s, empty)} a) r s';return (b, s'', w <> w')}
≡ RWST $ \ _ s -> do {~(a, s', w)  <- (\ _ s -> n (a, s, empty)) _ s; ~(b, s'',w') <- runRWST (RWST $ \ _ s -> do {a <- f a; return (a, s, empty)}) _ s';return (b, s'', w <> w')}
≡ RWST $ \ _ s -> do {~(a, s', w)  <- n (a, s, empty); ~(b, s'',w') <- do {a <- n b; return (a, s', empty)};return (b, s'', w <> w')}
≡ RWST $ \ _ s -> do {~(a, s', w)  <- n (a, s, empty); ~(b, s'',w') <- n (b, s', empty);return (b, s'', empty <> w')}
≡ RWST $ \ _ s -> do {~(b, s'',w') <- n (b, s, empty);return (b, s'', empty <> w')}
≡ RWST $ \ _ s -> n (b, s, empty)
```

### RWST 是 Functor 也是 Applicative

```
instance (Functor m) => Functor (RWST r w s m) where
    fmap f m = RWST $ \ r s ->
        fmap (\ ~(a, s', w) -> (f a, s', w)) $ runRWST m r s

instance (Monoid w, Functor m, Monad m) => Applicative (RWST r w s m) where
    pure a = RWST $ \ _ s -> return (a, s, mempty)
    RWST mf <*> RWST mx  = RWST $ \ r s -> do
        ~(f, s', w)  <- mf r s
        ~(x, s'',w') <- mx r s'
        return (f x, s'', w `mappend` w')
```

### RWST 是 Alternative 也是 MonadPlus

```
instance (Monoid w, Functor m, MonadPlus m) => Alternative (RWST r w s m) where
    empty = RWST $ \ _ _ -> mzero
    RWST m <|> RWST n = RWST $ \ r s -> m r s `mplus` n r s

instance (Monoid w, MonadPlus m) => MonadPlus (RWST r w s m) where
    mzero = RWST $ \ _ _ -> mzero
    RWST m `mplus` RWST n = RWST $ \ r s -> m r s `mplus` n r s
```

### RWST Monad转换器的函数

```
evalRWST :: (Monad m) => RWST r w s m a -> r -> s -> m (a, w)
evalRWST m r s = do
    ~(a, _, w) <- runRWST m r s
    return (a, w)

execRWST :: (Monad m) => RWST r w s m a -> r -> s -> m (s, w)
execRWST m r s = do
    ~(_, s', w) <- runRWST m r s
    return (s', w)

mapRWST :: (m (a, s, w) -> n (b, s, w')) -> RWST r w s m a -> RWST r w' s n b
mapRWST f m = RWST $ \ r s -> f (runRWST m r s)

withRWST :: (r' -> s -> (r, s)) -> RWST r w s m a -> RWST r' w s m a
withRWST f m = RWST $ \ r s -> uncurry (runRWST m) (f r s)

-- Reader operations

reader :: (Monoid w, Monad m) => (r -> a) -> RWST r w s m a
reader = asks

ask :: (Monoid w, Monad m) => RWST r w s m r
ask = RWST $ \ r s -> return (r, s, mempty)

local :: (r -> r) -> RWST r w s m a -> RWST r w s m a
local f m = RWST $ \ r s -> runRWST m (f r) s

asks :: (Monoid w, Monad m) => (r -> a) -> RWST r w s m a
asks f = RWST $ \ r s -> return (f r, s, mempty)

-- Writer operations

writer :: (Monad m) => (a, w) -> RWST r w s m a
writer (a, w) = RWST $ \ _ s -> return (a, s, w)

tell :: (Monad m) => w -> RWST r w s m ()
tell w = RWST $ \ _ s -> return ((),s,w)

listen :: (Monad m) => RWST r w s m a -> RWST r w s m (a, w)
listen m = RWST $ \ r s -> do
    ~(a, s', w) <- runRWST m r s
    return ((a, w), s', w)

listens :: (Monad m) => (w -> b) -> RWST r w s m a -> RWST r w s m (a, b)
listens f m = RWST $ \ r s -> do
    ~(a, s', w) <- runRWST m r s
    return ((a, f w), s', w)

pass :: (Monad m) => RWST r w s m (a, w -> w) -> RWST r w s m a
pass m = RWST $ \ r s -> do
    ~((a, f), s', w) <- runRWST m r s
    return (a, s', f w)

censor :: (Monad m) => (w -> w) -> RWST r w s m a -> RWST r w s m a
censor f m = RWST $ \ r s -> do
    ~(a, s', w) <- runRWST m r s
    return (a, s', f w)

-- State operations

state :: (Monoid w, Monad m) => (s -> (a,s)) -> RWST r w s m a
state f = RWST $ \ _ s -> let (a,s') = f s  in  return (a, s', mempty)

get :: (Monoid w, Monad m) => RWST r w s m s
get = RWST $ \ _ s -> return (s, s, mempty)

put :: (Monoid w, Monad m) => s -> RWST r w s m ()
put s = RWST $ \ _ _ -> return ((), s, mempty)

modify :: (Monoid w, Monad m) => (s -> s) -> RWST r w s m ()
modify f = RWST $ \ _ s -> return ((), f s, mempty)

gets :: (Monoid w, Monad m) => (s -> a) -> RWST r w s m a
gets f = RWST $ \ _ s -> return (f s, s, mempty)
```

以下是针对 RWST 整体的函数：
- evalRWST :: (Monad m) => RWST r w s m a -> r -> s -> m (a, w)

evalRWST函数在计算之后返回结果和输出信息。
- execRWST :: (Monad m) => RWST r w s m a -> r -> s -> m (s, w)

execRWST函数在计算之后返回状态和输出信息。
- mapRWST :: (m (a, s, w) -> n (b, s, w')) -> RWST r w s m a -> RWST r w' s n b

mapRWST函数在计算之后针对结果三元组调用函数f。
- withRWST :: (r' -> s -> (r, s)) -> RWST r w s m a -> RWST r' w s m a

evalRWST函数在计算之前针对环境参数和状态参数调用函数f。

以下是涉及 Reader 操作的函数：
- reader :: (Monoid w, Monad m) => (r -> a) -> RWST r w s m a

reader函数与asks同义。
- ask :: (Monoid w, Monad m) => RWST r w s m r

ask函数返回环境参数和状态参数。（输出信息为空）
- local :: (r -> r) -> RWST r w s m a -> RWST r w s m a

local f m 通过调用函数 f 局部性地修改环境参数 r，然后调用RWST Monad m 中封装的函数。
- asks :: (Monoid w, Monad m) => (r -> a) -> RWST r w s m a

asks f 通过调用函数f修改环境参数，然后返回环境参数和状态参数。（输出信息为空）

```
Prelude Control.Monad.RWS> runRWST ask "abc" "!!!"
("abc","!!!",())
Prelude Control.Monad.RWS> runRWST (local (++ "def") ask) "abc" "!!!"
("abcdef","!!!",())
Prelude Control.Monad.RWS> runRWST (withRWST (\r s -> (length r, s))  ask) "abc" "!!!"
(3,"!!!",())
Prelude Control.Monad.RWS> runRWST (mapRWST (++ [("def","***",())]) ask) "abc" "!!!"
[("abc","!!!",()),("def","***",())]
Prelude Control.Monad.RWS> runRWST (asks (++ "def")) "abc" "!!!"
("abcdef","!!!",())
Prelude Control.Monad.RWS> runRWST (local (++ "def") ask >> ask) "abc" "!!!"
("abc","!!!",())
Prelude Control.Monad.RWS> let ioTask = do {a <- ask; liftIO $ print a}
Prelude Control.Monad.RWS> :t ioTask
ioTask :: (MonadReader a m, MonadIO m, Show a) => m ()
Prelude Control.Monad.RWS> runRWST ioTask "abc" "!!!"
"abc"
((),"!!!",())
Prelude Control.Monad.RWS> runRWST (local (++ "def") ioTask) "abc" "!!!"
"abcdef"
((),"!!!",())
Prelude Control.Monad.RWS> runRWST (local (++ "def") ioTask >> ioTask) "abc" "!!!"
"abcdef"
"abc"
((),"!!!",())
```

以下是涉及 Writer 操作的函数：
- writer :: (Monad m) => (a, w) -> RWST r w s m a

writer函数将结果值和输出信息封装进RWST Monad内带环境和状态的函数之中，
- tell :: (Monad m) => w -> RWST r w s m ()

tell w 记录输出信息 w。（结果值为空）（状态值不变）
- listen :: (Monad m) => RWST r w s m a -> RWST r w s m (a, w)

listen m 在计算之后将结果值设置为由结果值和输出信息组成的二元组 (a, w)，输出信息不变。（状态值不变）
- listens :: (Monad m) => (w -> b) -> RWST r w s m a -> RWST r w s m (a, b)

listens f m 在计算之后将结果值设置为由结果值和输出信息（调用函数 f 修改后的返回值）组成的二元组 (a, f w)，输出信息不变。（状态值不变）
- pass :: (Monad m) => RWST r w s m (a, w -> w) -> RWST r w s m a

pass m 在计算之后将结果值解析为结果值 a 和函数 f 组成的二元组，设置结果值 a, 然后对输出信息 w 调用函数 f。（状态值不变）
- censor :: (Monad m) => (w -> w) -> RWST r w s m a -> RWST r w s m a

censor f m 在计算之后对输出信息 w 调用函数 f，结果值不变。（状态值不变）

```
Prelude Control.Monad.RWS> runRWST (tell "abc") "---" "!!!"
((),"!!!","abc")
Prelude Control.Monad.RWS> runRWST (tell "abc" >> tell "def") "---" "!!!"
((),"!!!","abcdef")
Prelude Control.Monad.RWS> runRWST (tell "abc" >> return 3) "---" "!!!"
(3,"!!!","abc")
Prelude Control.Monad.RWS> runRWST (tell "abc" >> listen (tell "def")) "---" "!!!"
(((),"def"),"!!!","abcdef")
Prelude Control.Monad.RWS> runRWST (listens (++ "def") $ tell "abc") "---" "!!!"
(((),"abcdef"),"!!!","abc")
Prelude Control.Monad.RWS> runRWST (pass $ tell "abc" >> return (0, (++ "def"))) "---" "!!!"
(0,"!!!","abcdef")
Prelude Control.Monad.RWS> runRWST (censor (++ "def") $ tell "abc") "---" "!!!"
((),"!!!","abcdef")
```

以下是涉及 State 操作的函数：
- state :: (Monoid w, Monad m) => (s -> (a,s)) -> RWST r w s m a

state f 将函数 f 封装进 Monad。（输出信息为空）
- get :: (Monoid w, Monad m) => RWST r w s m s

get 将结果值设置为状态值 s，状态值 s 保持不变。（输出信息为空）
- put :: (Monoid w, Monad m) => s -> RWST r w s m ()

put s 将结果值设为空，将状态值设为 s。（输出信息为空）
- modify :: (Monoid w, Monad m) => (s -> s) -> RWST r w s m ()

modify f 将结果值设为空，将状态值设为 f s。（输出信息为空）
- gets :: (Monoid w, Monad m) => (s -> a) -> RWST r w s m a

gets f 将结果值设为 f s，状态值 s 保持不变。（输出信息为空）

```
Prelude Control.Monad.RWS> runRWST (return 15) "---" 1
(15,1,())
Prelude Control.Monad.RWS> runRWST get "---" 1
(1,1,())
Prelude Control.Monad.RWS> runRWST (put 3) "---" 1
((),3,())
Prelude Control.Monad.RWS> runRWST (modify (+1)) "---" 1
((),2,())
Prelude Control.Monad.RWS> runRWST (gets (+1)) "---" 1
(2,1,())
Prelude Control.Monad.RWS> evalRWST (gets (+1)) "---" 1
(2,())
Prelude Control.Monad.RWS> execRWST (gets (+1)) "---" 1
(1,())
Prelude Control.Monad.RWS> runRWST (do put 3; return 15) "---" 1
(15,3,())
Prelude Control.Monad.RWS> runRWST (put 3 >> return 15) "---" 1
(15,3,())
```

综合利用三者的功能：

```
Prelude Control.Monad.RWS> runRWST (ask >>= tell) "abc" 0
((),0,"abc")
Prelude Control.Monad.RWS> runRWST (local (++ "def") ask >>= tell) "abc" 0
((),0,"abcdef")
Prelude Control.Monad.RWS> runRWST (asks (++ "def") >>= tell) "abc" 0
((),0,"abcdef")
Prelude Control.Monad.RWS> runRWST (listen $ ask >>= tell) "abc" 0
(((),"abc"),0,"abc")
Prelude Control.Monad.RWS> runRWST (listens (++ "def") $ ask >>= tell) "abc" 0
(((),"abcdef"),0,"abc")
Prelude Control.Monad.RWS> runRWST (censor (++ "def") $ ask >>= tell) "abc" 0
((),0,"abcdef")
Prelude Control.Monad.RWS> runRWST (get >>= tell) 0 "abc"
((),"abc","abc")
Prelude Control.Monad.RWS> runRWST (get >>= tell >> tell "def") 0 "abc"
((),"abc","abcdef")
Prelude Control.Monad.RWS> runRWST (get >>= tell >> put "def") 0 "abc"
((),"def","abc")
Prelude Control.Monad.RWS> runRWST (get >>= tell >> return "def") 0 "abc"
("def","abc","abc")
```

### RWS Monad

```
type RWS r w s = RWST r w s Identity

rws :: (r -> s -> (a, s, w)) -> RWS r w s a
rws f = RWST (\ r s -> Identity (f r s))

runRWS :: RWS r w s a -> r -> s -> (a, s, w)
runRWS m r s = runIdentity (runRWST m r s)

evalRWS :: RWS r w s a -> r -> s -> (a, w)
evalRWS m r s = let
    (a, _, w) = runRWS m r s
    in (a, w)

execRWS :: RWS r w s a -> r -> s -> (s, w)
execRWS m r s = let
    (_, s', w) = runRWS m r s
    in (s', w)

mapRWS :: ((a, s, w) -> (b, s, w')) -> RWS r w s a -> RWS r w' s b
mapRWS f = mapRWST (Identity . f . runIdentity)

withRWS :: (r' -> s -> (r, s)) -> RWS r w s a -> RWS r' w s a
withRWS = withRWST
```

RWS Monad 是 RWST Monad（转换器） 的一个特例。


