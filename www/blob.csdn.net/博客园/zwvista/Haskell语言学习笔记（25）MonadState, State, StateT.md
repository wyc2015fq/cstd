# Haskell语言学习笔记（25）MonadState, State, StateT - zwvista - 博客园

## [Haskell语言学习笔记（25）MonadState, State, StateT](https://www.cnblogs.com/zwvista/p/7638938.html)

### MonadState 类型类

```
class Monad m => MonadState s m | m -> s where
    get :: m s
    get = state (\s -> (s, s))

    put :: s -> m ()
    put s = state (\_ -> ((), s))

    state :: (s -> (a, s)) -> m a
    state f = do
      s <- get
      let ~(a, s') = f s
      put s'
      return a

modify :: MonadState s m => (s -> s) -> m ()
modify f = state (\s -> ((), f s))

modify' :: MonadState s m => (s -> s) -> m ()
modify' f = state (\s -> let s' = f s in s' `seq` ((), s'))

gets :: MonadState s m => (s -> a) -> m a
gets f = do
    s <- get
    return (f s)

instance Monad m => MonadState s (Lazy.StateT s m) where
    get = Lazy.get
    put = Lazy.put
    state = Lazy.state

instance Monad m => MonadState s (Strict.StateT s m) where
    get = Strict.get
    put = Strict.put
    state = Strict.state
```
- 
class Monad m => MonadState s m | m -> s where

MonadState 是个类型类，它为 StateT, RWST 等具有 State 功能的 Monad 定义了通用接口。

所谓 State 功能是指对状态计算环境的封装，也就是对函数 \s -> (a, s) 的封装。

MonadWriter 包含三个函数：get, put, state。

get 将结果值设置为状态值 s，状态值 s 保持不变。

put s 将结果值设为空，将状态值设为 s。

state f 将函数 f 封装进 Monad。

另外同一个模块中还定义了 modify 和 gets 函数。

modify f 将结果值设为空，将状态值设为 f s。

gets f 将结果值设为 f s，状态值 s 保持不变。
[What's the “|” for in a Haskell class definition?](https://stackoverflow.com/questions/2675655/whats-the-for-in-a-haskell-class-definition)

- 
instance Monad m => MonadState s (Lazy.StateT s m) where

get = Lazy.get

对于 StateT 这个Monad转换器来说，get等函数的定义均由 StateT 模块来提供。注意这里点运算符的含义不是函数的合成而是受限名字。
[Hackage - Where is the MonadReader implementation for ReaderT defined?](https://stackoverflow.com/questions/31437398/hackage-where-is-the-monadreader-implementation-for-readert-defined)

### StateT Monad转换器

```
newtype StateT s m a = StateT { runStateT :: s -> m (a,s) }

instance (Monad m) => Monad (StateT s m) where
    return a = StateT $ \ s -> return (a, s)
    m >>= k  = StateT $ \ s -> do
        ~(a, s') <- runStateT m s
        runStateT (k a) s'
```
- newtype StateT s m a = StateT { runStateT :: s -> m (a,s) }

StateT 类型是个 newtype，也就是对现有类型的封装。该类型有三个类型参数：内部 Monad 类型参数 m，状态类型参数 s 以及结果类型参数 a。

StateT s m 类型封装了一个状态转换函数：\s -> m (a,s’)，通过 runStateT 字段可以从 StateT 类型中取出这个函数。

该函数接收一个状态参数 s，经过计算（转换）之后返回一对封装在内部 Monad m 中的值：计算结果 a 以及新的状态 s'。
- instance (Monad m) => Monad (StateT s m) where

如果 m 是个 Monad，那么 StateT s m 也是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: a -> StateT s m a

大致相当于 a -> s -> m (a,s)

而 bind 函数的类型签名为：

(>>=) :: StateT s m a -> (a -> StateT s m b) -> StateT s m b

大致相当于 (s -> m (a,s)) -> (a -> s -> m (b,s))) -> (s -> m (b,s))
- return a = StateT $ \s -> return (a, s)

return 函数将 a 封装进了状态转换函数，该函数首先把结果值设为 a，状态值 s 保持不变，然后把这对值封装进了内部 Monad m。

这里左侧的 return 是 StateT 这个 Monad 的 return，而右侧的 return 是内部 Monad m 的 return。
- m >>= k = StateT $ \s -> do

对比函数签名，可知 m 的类型为 StateT s m a，大致相当于 s -> m (a,s)。

而 k 的类型为 a -> StateT s m b，大致相当于 a -> s -> m (b,s)

bind 操作符组合两个状态转换函数，最终结果仍然是个状态转换函数。
- ~(a, s') <- runStateT m s

这里首先利用 runState 字段取出 StateT Monad m 中封装的状态转换函数，然后将它应用于状态值 s 之上，得到结果值 a 以及新的状态值 s'。

runStateT m 让 m 脱离了 StateT 这个 Monad，而 <- 运算符让 runStateT m s 脱离了内部 Monad m。
- runStateT (k a) s'

根据 k 的类型 a -> StateT s m b，可知 k a 的类型为 StateT s m b，即 k a 也是一个 StateT Monad。

这里首先利用 runStateT 字段取出 StateT Monad (k a) 中封装的状态转换函数，然后将它应用于状态值 s' 之上，得到一对封装在内部 Monad m 中的值：最终结果值 a' 和最终状态值 s''。

```
证明 StateT s m 符合Monad法则：
1. return a >>= f ≡ f a
return a >>= f
≡ (StateT $ \s -> return (a, s)) >>= f
≡ StateT (\s -> m (a, s)) >>= f
≡ StateT $ \s -> do {~(a, s') <- runStateT (StateT (\s -> m (a, s))) s; runStateT (f a) s'}
≡ StateT $ \s -> do {~(a, s') <- m (a, s); runStateT (f a) s'}
≡ StateT $ \s -> do {~(a, s') <- m (a, s); runStateT (f a) s'}
≡ StateT $ \s -> runStateT (f a) s
≡ StateT $ runStateT (f a)
≡ f a
2. m >>= return ≡ m
m = StateT (\s -> m (a, s))
m >>= return
≡ StateT $ \s -> do {~(a, s') <- runStateT m s; runStateT (return a) s'}
≡ StateT $ \s -> do {~(a, s') <- runStateT (StateT (\s -> m (a, s))) s; runStateT (StateT (\s -> m (a, s))) s'}
≡ StateT $ \s -> do {~(a, s') <- (\s -> m (a, s)) s; (\s -> m (a, s)) s'}
≡ StateT $ \s -> do {~(a, s') <- m (a, s); m (a, s')}
≡ StateT $ \s -> m (a, s)
≡ m
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(m >>= f) >>= g
≡ (StateT $ \s -> do {~(a, s') <- runStateT m s; runStateT (k a) s'}) >> g
≡ StateT $ \s -> do {~(a, s') <- runStateT (StateT $ \s -> do {~(a, s') <- runStateT m s; runStateT (f a) s'}) s; runStateT (g a) s'}
≡ StateT $ \s -> do {~(a, s') <- (\s -> do {~(a, s') <- runStateT m s; runStateT (f a) s'}) s; runStateT (g a) s'}
≡ StateT $ \s -> do {~(a, s') <- do {~(a, s') <- runStateT m s; runStateT (f a) s'}); runStateT (g a) s'}
≡ StateT $ \s -> (runStateT m s >>= \(a, s') -> runStateT (f a) s') >>= \(a, s') -> runStateT (g a) s'
m >>= (\x -> f x >>= g)
≡ StateT $ \s -> do {~(a, s') <- runStateT m s; runStateT ((\x -> f x >>= g) a) s'}
≡ StateT $ \s -> do {~(a, s') <- runStateT m s; runStateT (f a >>= g) s'}
≡ StateT $ \s -> do {~(a, s') <- runStateT m s; runStateT (StateT $ \s -> do {~(a, s') <- runStateT (f a) s; runStateT (g a) s'}) s'}
≡ StateT $ \s -> do {~(a, s') <- runStateT m s; (\s -> do {~(a, s') <- runStateT (f a) s; runStateT (g a) s'}) s'}
≡ StateT $ \s -> do {~(a, s') <- runStateT m s; do {~(a, s') <- runStateT (f a) s'; runStateT (g a) s'}}
≡ StateT $ \s -> runStateT m s >>= \(a, s') -> (runStateT (f a) s' >>= \(a, s') -> runStateT (g a) s')
根据内部 Monad 的法则：(m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
StateT $ \s -> (runStateT m s >>= \(a, s') -> runStateT (f a) s') >>= \(a, s') -> runStateT (g a) s'
≡ StateT $ \s -> runStateT m s >>= (\(a, s') -> (\(a, s') -> runStateT (f a) s')) (a, s') >>= \(a, s') -> runStateT (g a) s')
≡ StateT $ \s -> runStateT m s >>= \(a, s') -> (runStateT (f a) s' >>= \(a, s') -> runStateT (g a) s')
```

### lift 函数

```
instance MonadTrans (StateT s) where
    lift m = StateT $ \ s -> do
        a <- m
        return (a, s)
```

```
证明 StateT 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
lift . return $ a
≡ lift (m a)
≡ StateT $ \s -> do {a <- m a; return (a, s)}
≡ StateT $ \s -> m (a, s)
≡ return a
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ lift (n b)
≡ StateT $ \s -> do {a <- n b; return (a, s)}
≡ StateT $ \s -> n (b, s)
lift m >>= (lift . f)
≡ (StateT $ \s -> do {a <- n a; return (a, s)}) >>= (\x -> StateT $ \s -> do {a <- f x; return (a, s)})
≡ (StateT $ \s -> n (a s)) >>= (\x -> StateT $ \s -> do {a <- f x; return (a, s)})
≡ StateT $ \s -> do {runStateT (StateT $ \s -> do {a <- f a; return (a, s)}) s}
≡ StateT $ \s -> do {runStateT (StateT $ \s -> do {a <- n b; return (a, s)}) s}
≡ StateT $ \s -> do {runStateT (StateT $ \s -> n (b s)) s}
≡ StateT $ \s -> n (b s)
```

### StateT 是 Functor 也是 Applicative

```
instance (Functor m) => Functor (StateT s m) where
    fmap f m = StateT $ \ s ->
        fmap (\ ~(a, s') -> (f a, s')) $ runStateT m s

instance (Functor m, Monad m) => Applicative (StateT s m) where
    pure a = StateT $ \ s -> return (a, s)
    StateT mf <*> StateT mx = StateT $ \ s -> do
        ~(f, s') <- mf s
        ~(x, s'') <- mx s'
        return (f x, s'')
    m *> k = m >>= \_ -> k
```

### StateT 是 Alternative 也是 MonadPlus

```
instance (Functor m, MonadPlus m) => Alternative (StateT s m) where
    empty = StateT $ \ _ -> mzero
    StateT m <|> StateT n = StateT $ \ s -> m s `mplus` n s

instance (MonadPlus m) => MonadPlus (StateT s m) where
    mzero       = StateT $ \ _ -> mzero
    StateT m `mplus` StateT n = StateT $ \ s -> m s `mplus` n s
```

### StateT Monad转换器的函数

```
get :: (Monad m) => StateT s m s
get = state $ \ s -> (s, s)

put :: (Monad m) => s -> StateT s m ()
put s = state $ \ _ -> ((), s)

modify :: (Monad m) => (s -> s) -> StateT s m ()
modify f = state $ \ s -> ((), f s)

modify' :: (Monad m) => (s -> s) -> StateT s m ()
modify' f = do
    s <- get
    put $! f s

gets :: (Monad m) => (s -> a) -> StateT s m a
gets f = state $ \ s -> (f s, s)

state :: (Monad m) => (s -> (a, s)) -> StateT s m a
state f = StateT (return . f)

evalStateT :: (Monad m) => StateT s m a -> s -> m a
evalStateT m s = do
    ~(a, _) <- runStateT m s
    return a

execStateT :: (Monad m) => StateT s m a -> s -> m s
execStateT m s = do
    ~(_, s') <- runStateT m s
    return s'

mapStateT :: (m (a, s) -> n (b, s)) -> StateT s m a -> StateT s n b
mapStateT f m = StateT $ f . runStateT m

withStateT :: (s -> s) -> StateT s m a -> StateT s m a
withStateT f m = StateT $ runStateT m . f
```

evalStateT m s 针对 State Monad m 利用初始状态值 s 进行状态计算，然后返回最终结果值 a'。

execStateT m s 针对 State Monad m 利用初始状态值 s 进行状态计算，然后返回最终状态值 s'。

mapStateT f m 针对 State Monad m 进行状态计算之后，对最终结果值和状态值调用函数 f。

withStateT f m 针对 State Monad m 进行状态计算之前，对初始状态值调用函数 f。

```
Prelude Control.Monad.State> runStateT (return 15) 1
(15,1)
Prelude Control.Monad.State> runStateT get 1
(1,1)
Prelude Control.Monad.State> runStateT (put 3) 1
((),3)
Prelude Control.Monad.State> runStateT (modify (+1)) 1
((),2)
Prelude Control.Monad.State> runStateT (gets (+1)) 1
(2,1)
Prelude Control.Monad.State> evalStateT (gets (+1)) 1
2
Prelude Control.Monad.State> execStateT (gets (+1)) 1
1
Prelude Control.Monad.State> runStateT (do put 3; return 15) 1
(15,3)
Prelude Control.Monad.State> runStateT (put 3 >> return 15) 1
(15,3)
```

### State Monad

```
type State s = StateT s Identity

runState :: State s a -> s -> (a, s)
runState m = runIdentity . runStateT m

evalState :: State s a -> s -> a
evalState m s = fst (runState m s)

execState :: State s a -> s -> s
execState m s = snd (runState m s)

mapState :: ((a, s) -> (b, s)) -> State s a -> State s b
mapState f = mapStateT (Identity . f . runIdentity)

withState :: (s -> s) -> State s a -> State s a
withState = withStateT
```

State Monad 是 StateT Monad（转换器） 的一个特例。

### 理解 State Monad

假设存在以下的State Monad 的实例 f

f :: State s a

我们可以把 f 这个 State Monad 粗略地理解为一个参数类型为 s 返回值类型为 a 的普通函数。

在 f 这个函数之中，“参数” s 可以
- 通过调用 modify 或 gets 函数将其隐式地传给其他函数。
- 使用 get 函数读取它的值，使用 put 函数设置它的值，使用 modify 函数修改它的值。
- 通过调用 get 或 gets 函数将其反映为“返回值” a。

在 f 这个函数之中，“返回值” a 可以
- 通过调用 get 或 gets 函数间接设置它的值。
- 通过调用 return 函数直接设置它的值。
- 通过调用 modify 或 put 函数被清空。

[Why must we use state monad instead of passing state directly?](https://stackoverflow.com/questions/31475770/why-must-we-use-state-monad-instead-of-passing-state-directly)

### 应用实例

```
import Control.Monad.State

type Stack = [Int]

pop :: State Stack Int  
pop = state $ \(x:xs) -> (x,xs)  

push :: Int -> State Stack ()  
push a = state $ \xs -> ((),a:xs)  

stackManip :: State Stack Int  
stackManip = do  
    push 3  
    a <- pop  
    pop  

stackStuff :: State Stack ()  
stackStuff = do  
    a <- pop  
    if a == 5  
        then push 5  
        else do  
            push 3  
            push 8

moreStack :: State Stack ()  
moreStack = do  
    a <- stackManip  
    if a == 100  
        then stackStuff  
        else return ()

stackyStack :: State Stack ()  
stackyStack = do  
    stackNow <- get  
    if stackNow == [1,2,3]  
        then put [8,3,1]  
        else put [9,2,1]  

main = do
    print $ runState stackManip [5,8,2,1]
    print $ runState stackStuff [9,0,2,1,0]
    print $ runState stackyStack [9,0,2,1,0]
    
{-
(5,[8,2,1])
((),[8,3,0,2,1,0])
((),[9,2,1])
-}
```


