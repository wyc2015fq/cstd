# Haskell语言学习笔记（23）MonadReader, Reader, ReaderT - zwvista - 博客园

## [Haskell语言学习笔记（23）MonadReader, Reader, ReaderT](https://www.cnblogs.com/zwvista/p/7635970.html)

### MonadReader 类型类

```
class Monad m => MonadReader r m | m -> r where
    ask   :: m r
    ask = reader id

    local :: (r -> r) -> m a -> m a

    reader :: (r -> a) -> m a
    reader f = do
      r <- ask
      return (f r)

instance Monad m => MonadReader r (ReaderT r m) where
    ask = ReaderT.ask
    local = ReaderT.local
    reader = ReaderT.reader

asks :: MonadReader r m => (r -> a) -> m a
asks = reader
```
- 
class Monad m => MonadReader r m | m -> r where

MonadReader 是个类型类，它为 ReaderT, RWST 等具有 Reader 功能的 Monad 定义了通用接口。

所谓 Reader 功能是指第一个参数固定的函数，也就是具有固定环境变量的函数。

MonadReader 包含三个函数：ask, local, reader。

ask 获取环境变量 r。

local f m 通过调用函数 f 局部性地修改环境变量 r，然后调用 Monad m 中封装的函数。

reader f 对环境变量 r 调用指定函数 f。

另外同一个模块中还定义了 asks 函数，它与 reader 函数同义。
[What's the “|” for in a Haskell class definition?](https://stackoverflow.com/questions/2675655/whats-the-for-in-a-haskell-class-definition)

- 
instance Monad m => MonadReader r (ReaderT r m) where

ask = ReaderT.ask

对于 ReaderT 这个Monad转换器来说，ask等函数的定义均由 ReaderT 模块来提供。注意这里点运算符的含义不是函数的合成而是受限名字。
[Hackage - Where is the MonadReader implementation for ReaderT defined?](https://stackoverflow.com/questions/31437398/hackage-where-is-the-monadreader-implementation-for-readert-defined)

### ReaderT Monad转换器

```
newtype ReaderT r m a = ReaderT { runReaderT :: r -> m a }

instance (Monad m) => Monad (ReaderT r m) where
    return   = lift . return
    m >>= k  = ReaderT $ \ r -> do
        a <- runReaderT m r
        runReaderT (k a) r

instance MonadTrans (ReaderT r) where
    lift   = liftReaderT

liftReaderT :: m a -> ReaderT r m a
liftReaderT m = ReaderT (const m)
```
- newtype ReaderT r m a = ReaderT { runReaderT :: r -> m a }

ReaderT 类型是个 newtype，也就是对现有类型的封装。该类型有三个类型参数：内部 Monad 类型参数 m，共享环境类型参数 r 以及结果类型参数 a。

ReaderT r m a 类型封装了一个共享环境计算函数：\r -> m a，通过 runReaderT 字段可以从 ReaderT 类型中取出这个函数。
- instance (Monad m) => Monad (ReaderT r m) where

如果 m 是个 Monad，那么 ReaderT r m 也是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: a -> ReaderT r m a

大致相当于 a -> r -> m a

而 bind 函数的类型签名为：

(>>=) :: ReaderT r m a -> (a -> ReaderT r m b) -> ReaderT r m b

大致相当于 (r -> m a) -> (a -> r -> m b) -> (r -> m b)
- return = lift . return

return 函数首先将类型为 a 的值封装进内部 Monad m 中，然后通过 lift 函数将它封装进 ReaderT 这个Monad 转换器之中。

这里左侧的 return 是 ReaderT 这个 Monad 的 return，而右侧的 return 是内部 Monad m 的 return。
- lift = liftReaderT

liftReaderT m = ReaderT (const m)

lift 函数的定义由 liftReaderT 函数提供。

liftReaderT 函数首先将封装在内部 Monad m 中的值封装进常值函数 const 中，然后再将它封装进 ReaderT 这个Monad 转换器之中。

return a

= lift . return $ a

= liftReaderT . return $ a

= ReaderT (const (m a))

= ReaderT $ _ -> m a
- m >>= k = ReaderT $ \r -> do

对比函数签名，可知 m 的类型是 ReaderT r m a

而 k 的类型是 a -> Reader r m b
- a <- runReaderT m r

runReaderT m 让 m 脱离了 ReaderT 这个 Monad，而 <- 运算符让 runReaderT m r 脱离了内部 Monad m。
- runReaderT (k a) r

k a 的类型是 ReaderT r m b

runReaderT (k a) 让 k a 脱离了 ReaderT 这个 Monad，重新进入内部 Monad m。

```
证明 ReaderT r m 符合Monad法则：
1. return a >>= f ≡ f a
return a >>= f
≡ (ReaderT $ \_ -> m a) >>= f
≡ ReaderT $ \r -> do {a <- runReaderT (ReaderT $ \_ -> m a) r; runReaderT (f a) r}
≡ ReaderT $ \r -> do {a <- (\_ -> m a) r; runReaderT (f a) r}
≡ ReaderT $ \r -> runReaderT (f a) r
≡ ReaderT $ runReaderT (f a)
≡ f a
2. m >>= return ≡ m
m = ReaderT $ \r -> n a
m >>= return
≡ ReaderT $ \r -> do {a <- runReaderT m r; runReaderT (return a) r}
≡ ReaderT $ \r -> do {a <- runReaderT (ReaderT $ \r -> n a) r; runReaderT (ReaderT $ \_ -> n a) r}
≡ ReaderT $ \r -> do {a <- (\r -> n a) r; (\_ -> n a) r}
≡ ReaderT $ \r -> n a
≡ m
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(m >>= f) >>= g
≡ (ReaderT $ \r -> do {a <- runReaderT m r; runReaderT (f a) r}) >> g
≡ ReaderT $ \r -> do {a <- runReaderT (ReaderT $ \r -> do {a <- runReaderT m r; runReaderT (f a) r}) r; runReaderT (g a) r}
≡ ReaderT $ \r -> do {a <- (\r -> do {a <- runReaderT m r; runReaderT (f a) r}) r; runReaderT (g a) r}
≡ ReaderT $ \r -> do {a <- do {a <- runReaderT m r; runReaderT (f a) r}; runReaderT (g a) r}
≡ ReaderT $ \r -> (runReaderT m r >>= \a -> runReaderT (f a) r}) >>= \a -> runReaderT (g a) r
m >>= (\x -> f x >>= g)
≡ ReaderT $ \r -> do {a <- runReaderT m r; runReaderT ((\x -> f x >>= g) a) r}
≡ ReaderT $ \r -> do {a <- runReaderT m r; runReaderT (f a >>= g) r}
≡ ReaderT $ \r -> do {a <- runReaderT m r; runReaderT (ReaderT $ \r -> do {a <- runReaderT (f a) r; runReaderT (g a) r}) r}
≡ ReaderT $ \r -> do {a <- runReaderT m r; (\r -> do {a <- runReaderT (f a) r; runReaderT (g a) r}) r}
≡ ReaderT $ \r -> do {a <- runReaderT m r; do {a <- runReaderT (f a) r; runReaderT (g a) r}}
≡ ReaderT $ \r -> runReaderT m r >>= (\a -> runReaderT (f a) r >>= \a -> runReaderT (g a) r)
根据内部 Monad 的法则：(m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
ReaderT $ \r -> (runReaderT m r >>= \a -> runReaderT (f a) r}) >>= \a -> runReaderT (g a) r
≡ ReaderT $ \r -> runReaderT m r >>= (\a -> (\a -> runReaderT (f a) r}) a >>= \a -> runReaderT (g a) r)
≡ ReaderT $ \r -> runReaderT m r >>= (\a -> runReaderT (f a) r >>= \a -> runReaderT (g a) r)
```

```
证明 ReaderT 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
lift . return $ a
≡ ReaderT (const (return a))
≡ ReaderT (const (m a))
≡ ReaderT $ \_ -> m a
≡ return a
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ ReaderT (const (n b))
≡ ReaderT $ \_ -> n b
lift m >>= (lift . f)
≡ ReaderT (const (n a)) >>= (ReaderT . const . f)
≡ (ReaderT $ \_ -> n a) >>= (\x -> ReaderT . const . f $ x)
≡ ReaderT $ \_ -> runReaderT (ReaderT . const . f $ a) _ 
≡ ReaderT $ \_ -> runReaderT (ReaderT (const (n b)) _ 
≡ ReaderT $ \_ -> runReaderT (ReaderT (\_ -> n b) _ 
≡ ReaderT $ \_ -> n b
```

### ReaderT Monad转换器的函数

```
ask :: (Monad m) => ReaderT r m r
ask = ReaderT return

local :: (Monad m) => (r -> r) -> ReaderT r m a -> ReaderT r m a
local = withReaderT

reader :: (Monad m) => (r -> a) -> ReaderT r m a
reader f = ReaderT (return . f)

asks :: (Monad m) => (r -> a) -> ReaderT r m a
asks f = ReaderT (return . f)

mapReaderT :: (m a -> n b) -> ReaderT r m a -> ReaderT r n b
mapReaderT f m = ReaderT $ f . runReaderT m

withReaderT :: (r' -> r) -> ReaderT r m a -> ReaderT r' m a
withReaderT f m = ReaderT $ runReaderT m . f
```
- 
withReaderT :: (r' -> r) -> ReaderT r m a -> ReaderT r' m a

withReaderT f m = ReaderT $ runReaderT m . f

withReaderT f m 通过调用函数 f 局部性地修改环境变量 r，然后调用 Monad m 中封装的函数。

- 
local :: (Monad m) => (r -> r) -> ReaderT r m a -> ReaderT r m a

local = withReaderT

withReaderT 与 local 的区别在于 withReaderT 能够改变环境变量 r 的类型。

local 不能改变环境变量 r 的类型，它可以被看做 withReaderT 的特例。

```
Prelude Control.Monad.Reader> runReaderT ask "abc"
"abc"
Prelude Control.Monad.Reader> runReaderT (local (++ "def") ask) "abc"
"abcdef"
Prelude Control.Monad.Reader> runReaderT (withReaderT length ask) "abc"
3
Prelude Control.Monad.Reader> runReaderT (mapReaderT (++ ["def"]) ask) "abc"
["abc","def"]
Prelude Control.Monad.Reader> runReaderT (asks (++ "def")) "abc"
"abcdef"
Prelude Control.Monad.Reader> runReaderT (local (++ "def") ask >> ask) "abc"
"abc"
Prelude Control.Monad.Reader> let ioTask = do {v <- ask; liftIO $ print v}
Prelude Control.Monad.Reader> :t ioTask
ioTask :: (MonadReader a m, MonadIO m, Show a) => m ()
Prelude Control.Monad.Reader> runReaderT ioTask "abc"
"abc"
Prelude Control.Monad.Reader> runReaderT (local (++ "def") ioTask) "abc"
"abcdef"
Prelude Control.Monad.Reader> runReaderT (local (++ "def") ioTask >> ioTask) "abc"
"abcdef"
"abc"
```

### Reader Monad

```
type Reader r = ReaderT r Identity

runReader :: Reader r a -> r -> a
runReader m = runIdentity . runReaderT m

mapReader :: (a -> b) -> Reader r a -> Reader r b
mapReader f = mapReaderT (Identity . f . runIdentity)

withReader :: (r' -> r) -> Reader r a -> Reader r' a
withReader = withReaderT
```

Reader Monad 是 ReaderT Monad（转换器） 的一个特例。

```
Prelude Control.Monad.Reader> runReader (mapReader (++"def") ask) "abc"
"abcdef"
Prelude Control.Monad.Reader> runReader (withReader (++"def") ask) "abc"
"abcdef"
```

### 应用实例

```
import Control.Monad.Reader

hello :: Reader String String
hello = do
    name <- ask
    return ("hello, " ++ name ++ "!")

bye :: Reader String String
bye = do
    name <- ask
    return ("bye, " ++ name ++ "!")

convo :: Reader String String
convo = do
    c1 <- hello
    c2 <- bye
    return $ c1 ++ c2

main = print . runReader convo $ "adit"
```

```
import Control.Monad.Reader

hello :: Reader String String
hello = asks $ \name -> ("hello, " ++ name ++ "!")

bye :: Reader String String
bye = asks $ \name -> ("bye, " ++ name ++ "!")

convo :: Reader String String
convo = asks (const (++)) <*> hello <*> bye

main = print . runReader convo $ "adit"
```

[Reader monad example](https://gist.github.com/egonSchiele/5752172)


