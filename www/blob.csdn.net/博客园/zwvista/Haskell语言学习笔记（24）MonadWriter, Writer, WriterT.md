# Haskell语言学习笔记（24）MonadWriter, Writer, WriterT - zwvista - 博客园

## [Haskell语言学习笔记（24）MonadWriter, Writer, WriterT](https://www.cnblogs.com/zwvista/p/7637869.html)

### MonadWriter 类型类

```
class (Monoid w, Monad m) => MonadWriter w m | m -> w where
    writer :: (a,w) -> m a
    writer ~(a, w) = do
      tell w
      return a

    tell   :: w -> m ()
    tell w = writer ((),w)

    listen :: m a -> m (a, w)
    pass   :: m (a, w -> w) -> m a

listens :: MonadWriter w m => (w -> b) -> m a -> m (a, b)
listens f m = do
    ~(a, w) <- listen m
    return (a, f w)

censor :: MonadWriter w m => (w -> w) -> m a -> m a
censor f m = pass $ do
    a <- m
    return (a, f)

instance (Monoid w, Monad m) => MonadWriter w (Lazy.WriterT w m) where
    writer = Lazy.writer
    tell   = Lazy.tell
    listen = Lazy.listen
    pass   = Lazy.pass

instance (Monoid w, Monad m) => MonadWriter w (Strict.WriterT w m) where
    writer = Strict.writer
    tell   = Strict.tell
    listen = Strict.listen
    pass   = Strict.pass
```
- 
class (Monoid w, Monad m) => MonadWriter w m | m -> w where

MonadWriter 是个类型类，它为 WriterT, RWST 等具有 Writer 功能的 Monad 定义了通用接口。

所谓 Writer 功能是指将程序的行为记录下来，也可以理解为日志。

MonadWriter 包含四个函数：writer, tell, listen, pass。

writer (a,w) 记录输出信息 w，返回结果值 a。

tell w 记录输出信息 w，返回结果值 ()。

listen m 将结果值设置为由结果值和输出信息组成的二元组 (a, w)，输出信息不变。

pass m 将结果值解析为结果值 a 和函数 f 组成的二元组，设置结果值 a, 然后对输出信息 w 调用函数 f。

另外同一个模块中还定义了 listens 和 censor 函数。

listens f m 将结果值设置为由结果值和输出信息（调用函数 f 修改后的返回值）组成的二元组 (a, f w)，输出信息不变。

censor f m 对输出信息 w 调用函数 f，结果值不变。
[What's the “|” for in a Haskell class definition?](https://stackoverflow.com/questions/2675655/whats-the-for-in-a-haskell-class-definition)

- 
instance (Monoid w, Monad m) => MonadWriter w (Lazy.WriterT w m) where

writer = Lazy.writer

对于 WriterT 这个Monad转换器来说，writer等函数的定义均由 WriterT 模块来提供。注意这里点运算符的含义不是函数的合成而是受限名字。
[Hackage - Where is the MonadReader implementation for ReaderT defined?](https://stackoverflow.com/questions/31437398/hackage-where-is-the-monadreader-implementation-for-readert-defined)

### WriterT Monad转换器

```
newtype WriterT w m a = WriterT { runWriterT :: m (a, w) }

instance (Monoid w, Monad m) => Monad (WriterT w m) where
    return a = writer (a, mempty)
    m >>= k  = WriterT $ do
        ~(a, w)  <- runWriterT m
        ~(b, w') <- runWriterT (k a)
        return (b, w `mappend` w')

writer :: (Monad m) => (a, w) -> WriterT w m a
writer = WriterT . return
```
- newtype WriterT w m a = WriterT { runWriterT :: m (a, w) }

WriterT 类型是个 newtype，也就是对现有类型的封装。该类型有三个类型参数：内部 Monad 类型参数 m，输出信息类型参数 w 以及结果类型参数 a。

WriterT 类型封装的是一对封装在内部 Monad m 中的值：m (a,w)，包括结果值 a 和输出信息 w。通过 runWriterT 字段可以从 Writer 类型中取出这对值。
- instance (Monoid w, Monad m) => Monad (WriterT w m) where

如果 w 是个 Monoid 并且 m 是个 Monad，那么 WriterT w m 也是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：
- return :: a -> WriterT w m a

大致相当于 a -> m (a,w)

而 bind 函数的类型签名为：

(>>=) :: WriterT w m a -> (a -> Writer w m a) -> Writer w m a

大致相当于 m (a,w) -> (a -> m (a,w)) -> m (a,w)
- return a = writer (a, mempty)

writer = WriterT . return

return 函数将结果值设为 a, 输出信息设为单位元，然后调用 writer 函数依次把这对值封装进了内部 Monad m 和 Monad 转换器 WriterT 之中。
- m >>= k = WriterT $ do

对比函数签名，可知 m 的类型为 Writer w m a，大致相当于 m (a,w)。

而 k 的类型为 a -> WriterT w m a，大致相当于 a -> m (a,w)。
- ~(a, w) <- runWriterT m

这里首先利用 runWriterT 字段和 <- 运算符取出封装在 WriterT Monad m 中的一对值：结果值 a 和输出信息 w。
- ~(b, w') <- runWriterT (k a)

根据 k 的类型 a -> WriterT w m a，可知 k a 的类型为 WriterT w m a，即 k a 也是一个 WriterT Monad。

这里再次利用 runWriterT 字段和 <- 运算符取出封装在 WriterT Monad (k a) 中的一对值：新的结果值 b 和新的输出信息 w'。
- return (b, w `mappend` w')

最后将结果值设为新的结果值 b，输出信息则设为原来的输出信息 w 与新的输出信息 w' 联结后的值。

return 函数让这对值重新进入内部 Monad m 之中。

```
证明 WriterT r m 符合Monad法则：
1. return a >>= f ≡ f a
return a >>= f
≡ writer (a, mempty) >>= f
≡ (WriterT . return) (a, mempty) >>= f
≡ WriterT (m (a, mempty)) >>= f
≡ WriterT $ do {~(a, w)  <- runWriterT (WriterT (m (a, mempty))); ~(b, w') <- runWriterT (f a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- m (a, mempty)); ~(b, w') <- runWriterT (f a); return (b, w <> w')}
≡ WriterT $ do {~(b, w') <- runWriterT (f a); return (b, empty <> w')}
≡ WriterT $ do {~(b, w') <- runWriterT (f a); return (b, w')}
≡ WriterT $ runWriterT (f a)
≡ f a
2. m >>= return ≡ m
m = WriterT (n (a w))
m >>= return
≡ WriterT (n (a w)) >>= return
≡ WriterT $ do {~(a, w)  <- runWriterT (WriterT (n (a, w))); ~(b, w') <- runWriterT (return a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- n (a, w)); ~(b, w') <- runWriterT (WriterT (m (a, mempty))); return (b, w <> w')}
≡ WriterT $ do {~(b, w') <- runWriterT (WriterT (m (a, mempty))); return (b, w <> w')}
≡ WriterT $ do {~(b, w') <- (m (a, mempty)); return (b, w <> w')}
≡ WriterT $ do {return (a, w <> empty)}
≡ WriterT $ do {return (a, w)}
≡ WriterT (n (a w))
≡ m
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
假设 m = WriterT (n (a1, w1)), f a1 = WriterT (n (a2, w2)), g a2 = WriterT (n (a3, w3))
(m >>= f) >>= g
≡ (WriterT $ do {~(a, w)  <- runWriterT m; ~(b, w') <- runWriterT (f a); return (b, w <> w')}) >> g
≡ WriterT $ do {~(a, w)  <- runWriterT (WriterT $ do {~(a, w)  <- runWriterT m; ~(b, w') <- runWriterT (f a); return (b, w <> w')}); ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- do {~(a, w)  <- runWriterT m; ~(b, w') <- runWriterT (f a); return (b, w <> w')}; ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- do {~(a, w)  <- runWriterT (WriterT (n (a1, w1))); ~(b, w') <- runWriterT (f a); return (b, w <> w')}; ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- do {~(a, w)  <- n (a1, w1); ~(b, w') <- runWriterT (f a); return (b, w <> w')}; ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- do {~(b, w') <- runWriterT (f a1); return (b, w1 <> w')}; ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- do {~(b, w') <- runWriterT (WriterT (n (a2, w2))); return (b, w1 <> w')}; ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- do {~(b, w') <- n (a2, w2); return (b, w1 <> w')}; ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- n (a2, w1 <> w2); ~(b, w') <- runWriterT (g a); return (b, w <> w')}
≡ WriterT $ do {~(b, w') <- runWriterT (g a2); return (b, (w1 <> w2) <> w')}
≡ WriterT $ do {~(b, w') <- runWriterT (WriterT (n (a3, w3))); return (b, (w1 <> w2) <> w')}
≡ WriterT $ do {~(b, w') <- n (a3, w3); return (b, (w1 <> w2) <> w')}
≡ WriterT (n (a3, (w1 <> w2) <> w3))
m >>= (\x -> f x >>= g)
≡ WriterT $ do {~(a, w)  <- runWriterT m; ~(b, w') <- runWriterT ((\x -> f x >>= g) a); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- runWriterT m; ~(b, w') <- runWriterT (f a >>= g); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- runWriterT m; ~(b, w') <- runWriterT (WriterT $ do {~(a, w)  <- runWriterT (f a); ~(b, w') <- runWriterT (g a); return (b, w <> w')}); return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- runWriterT m; ~(b, w') <- do {~(a, w)  <- runWriterT (f a); ~(b, w') <- runWriterT (g a); return (b, w <> w')}; return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- runWriterT (WriterT (n (a1, w1))); ~(b, w') <- do {~(a, w)  <- runWriterT (f a); ~(b, w') <- runWriterT (g a); return (b, w <> w')}; return (b, w <> w')}
≡ WriterT $ do {~(a, w)  <- n (a1, w1); ~(b, w') <- do {~(a, w)  <- runWriterT (f a); ~(b, w') <- runWriterT (g a); return (b, w <> w')}; return (b, w <> w')}
≡ WriterT $ do {~(b, w') <- do {~(a, w)  <- runWriterT (f a1); ~(b, w') <- runWriterT (g a); return (b, w <> w')}; return (b, w1 <> w')}
≡ WriterT $ do {~(b, w') <- do {~(a, w)  <- runWriterT (WriterT (n (a2, w2))); ~(b, w') <- runWriterT (g a); return (b, w <> w')}; return (b, w1 <> w')}
≡ WriterT $ do {~(b, w') <- do {~(a, w)  <- n (a2, w2); ~(b, w') <- runWriterT (g a); return (b, w <> w')}; return (b, w1 <> w')}
≡ WriterT $ do {~(b, w') <- do {~(b, w') <- runWriterT (g a2); return (b, w2 <> w')}; return (b, w1 <> w')}
≡ WriterT $ do {~(b, w') <- do {~(b, w') <- runWriterT (WriterT (n (a3, w3))); return (b, w2 <> w')}; return (b, w1 <> w')}
≡ WriterT $ do {~(b, w') <- do {~(b, w') <- n (a3, w3); return (b, w2 <> w')}; return (b, w1 <> w')}
≡ WriterT $ do {~(b, w') <- n (a3, w2 <> w3); return (b, w1 <> w')}
≡ WriterT (n (a3, w1 <> (w2 <> w3))
根据 Monoid 的法则：(x <> y) <> z = x <> (y <> z)
WriterT (n (a3, (w1 <> w2) <> w3))
≡ WriterT (n (a3, w1 <> (w2 <> w3))
```

### lift 函数

```
instance (Monoid w) => MonadTrans (WriterT w) where
    lift m = WriterT $ do
        a <- m
        return (a, mempty)
```

```
证明 WriterT 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
lift . return $ a
≡ lift (m a)
≡ WriterT $ do {a <- m a; return (a, mempty)}
≡ WriterT $ m (a, mempty)
≡ return a
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ lift (n b)
≡ WriterT $ do {a <- n b; return (a, empty)}
≡ WriterT $ n (b, empty)
lift m >>= (lift . f)
≡ (WriterT $ do {a <- n a; return (a, empty)}) >>= (\x -> WriterT $ do {a <- f x; return (a, empty)})
≡ (WriterT $ n (a empty)) >>= (\x -> WriterT $ do {a <- f x; return (a, empty)})
≡ WriterT $ do {runWriterT $ WriterT $ do {a <- f a; return (a, empty <> empty)}}
≡ WriterT $ do {runWriterT $ WriterT $ do {a <- n b; return (a, empty)}}
≡ WriterT $ do {runWriterT $ WriterT $ n (b empty)}
≡ WriterT $ n (b empty)
```

### WriterT Monad转换器的函数

```
tell :: (Monad m) => w -> WriterT w m ()
tell w = writer ((), w)

listen :: (Monad m) => WriterT w m a -> WriterT w m (a, w)
listen m = WriterT $ do
    ~(a, w) <- runWriterT m
    return ((a, w), w)

listens :: (Monad m) => (w -> b) -> WriterT w m a -> WriterT w m (a, b)
listens f m = WriterT $ do
    ~(a, w) <- runWriterT m
    return ((a, f w), w)

pass :: (Monad m) => WriterT w m (a, w -> w) -> WriterT w m a
pass m = WriterT $ do
    ~((a, f), w) <- runWriterT m
    return (a, f w)

censor :: (Monad m) => (w -> w) -> WriterT w m a -> WriterT w m a
censor f m = WriterT $ do
    ~(a, w) <- runWriterT m
    return (a, f w)

execWriterT :: (Monad m) => WriterT w m a -> m w
execWriterT m = do
    (_, w) <- runWriterT m
    return w

mapWriterT :: (m (a, w) -> n (b, w')) -> WriterT w m a -> WriterT w' n b
mapWriterT f m = WriterT $ f (runWriterT m)
```

```
Prelude Control.Monad.Writer> runWriterT $ tell "abc"
((),"abc")
Prelude Control.Monad.Writer> runWriterT $ tell "abc" >> tell "def"
((),"abcdef")
Prelude Control.Monad.Writer> runWriterT $ tell "abc" >> return 3
(3,"abc")
Prelude Control.Monad.Writer> runWriterT $ tell "abc" >> listen (tell "def")
(((),"def"),"abcdef")
Prelude Control.Monad.Writer> runWriterT $ listens (++ "def") $ tell "abc"
(((),"abcdef"),"abc")
Prelude Control.Monad.Writer> runWriterT $ pass $ tell "abc" >> return (0, (++ "def"))
(0,"abcdef")
Prelude Control.Monad.Writer> runWriterT $ censor (++ "def") $ tell "abc"
((),"abcdef")
```

### Writer Monad

```
type Writer w = WriterT w Identity

runWriter :: Writer w a -> (a, w)
runWriter = runIdentity . runWriterT

execWriter :: Writer w a -> w
execWriter m = snd (runWriter m)

mapWriter :: ((a, w) -> (b, w')) -> Writer w a -> Writer w' b
mapWriter f = mapWriterT (Identity . f . runIdentity)
```

Writer Monad 是 WriterT Monad（转换器） 的一个特例。

### 应用实例

```
import Control.Monad.Trans.Writer
      
logNumber :: Int -> Writer [String] Int  
logNumber x = writer (x, ["Got number: " ++ show x])

logNumber2 :: Int -> Writer [String] Int  
logNumber2 x = do
    tell ["Got number: " ++ show x]
    return x            
      
multWithLog :: Writer [String] Int
multWithLog = do  
    a <- logNumber 3  
    b <- logNumber 5
    tell ["multiplying " ++ show a ++ " and " ++ show b ]
    return (a*b)
    
main :: IO ()
main = print $ runWriter multWithLog -- (15,["Got number: 3","Got number: 5","multiplying 3 and 5"])
```


