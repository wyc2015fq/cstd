# Haskell语言学习笔记（26）Identity, IdentityT - zwvista - 博客园

## [Haskell语言学习笔记（26）Identity, IdentityT](https://www.cnblogs.com/zwvista/p/7640682.html)

### Identity Monad

```
newtype Identity a = Identity { runIdentity :: a }

instance Functor Identity where
    fmap     = coerce

instance Applicative Identity where
    pure     = Identity
    (<*>)    = coerce

instance Monad Identity where
    m >>= k  = k (runIdentity m)
```
- 
newtype Identity a = Identity { runIdentity :: a }

Identity 类型是个 newtype，也就是对现有类型的封装。该类型只有一个类型参数 a。

Identity a 封装了一个值：a，用 runIdentity 字段可以取出这个值。

Identity 是一个用于占位的 Monad。

- 
instance Monad Identity where

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: Identity a

而 bind 函数的类型签名为：

(>>=) :: a -> (a -> Identity b) -> Identity b

- 
m >>= k = k (runIdentity m)

```
证明 Identity 符合Monad法则：  
1. return a >>= f ≡ f a
return a >>= f ≡ Identity a >>= f ≡ f (runIdentity (Identity a)) ≡ f a   
2. m >>= return ≡ m
m >>= return ≡ (Identity a) >> Identity ≡ Identity (runIdentity (Identity a)) ≡ Identity a ≡ m  
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(m >>= f) >>= g
≡ ((Identity a) >>= f) >>= g
≡ f (runIdentity (Identity a)) >> = g
≡ f a >> g
m >>= (\x -> f x >>= g)  
≡ (Identity a) >>= (\x -> f x >>= g)  
≡ (\x -> f x >>= g) (runIdentity (Identity a))
≡ (\x -> f x >>= g) a
≡ f a >> g
```

### IdentityT Monad转换器

```
newtype IdentityT f a = IdentityT { runIdentityT :: f a }

instance (Monad m) => Monad (IdentityT m) where
    return = IdentityT . return
    m >>= k = IdentityT $ runIdentityT . k =<< runIdentityT m

instance MonadTrans IdentityT where
    lift = IdentityT

instance (MonadIO m) => MonadIO (IdentityT m) where
    liftIO = IdentityT . liftIO
```
- 
newtype IdentityT f a = IdentityT { runIdentityT :: f a }

IdentityT 类型是个 newtype，也就是对现有类型的封装。该类型有两个类型参数：内部 Monad 类型参数 f，以及值类型参数 a。

IdentityT f 类型封装了一个封装在内部 Monad f 中的值：f a，用 runIdentity 字段可以取出这个值。

Identity 是一个用于占位的 Monad转换器。

- 
instance (Monad m) => Monad (IdentityT m) where

如果 m 是个 Monad，那么 IdentityT m 也是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: IdentityT m a

而 bind 函数的类型签名为：

(>>=) :: a -> (a -> IdentityT m b) -> IdentityT m b

- 
m >>= k = IdentityT $ runIdentityT . k =<< runIdentityT m

```
证明 IdentityT 符合Monad法则：  
1. return a >>= f ≡ f a
return a >>= f
≡ (IdentityT . return) a >>= f
≡ IdentityT (m a) >>= f
≡ IdentityT $ runIdentityT . f =<< runIdentityT (IdentityT (m a))
≡ IdentityT $ runIdentityT . f =<< m a
≡ IdentityT $ runIdentityT (f a)
≡ f a
2. m >>= return ≡ m
假设 m = IdentityT (n a)
m >>= return
≡ IdentityT $ runIdentityT . return =<< runIdentityT m
≡ IdentityT $ runIdentityT . (IdentityT . return) =<< runIdentityT (IdentityT (n a))
≡ IdentityT $ runIdentityT . (IdentityT . return) =<< n a
≡ IdentityT $ runIdentityT (IdentityT (n a))
≡ IdentityT (n a) ≡ m  
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(m >>= f) >>= g
≡ (IdentityT $ runIdentityT . f =<< runIdentityT m) >>= g
≡ IdentityT $ runIdentityT . g =<< runIdentityT (IdentityT $ runIdentityT . f =<< runIdentityT m)
≡ IdentityT $ runIdentityT . g =<< (runIdentityT . f =<< runIdentityT m)
≡ IdentityT $ (runIdentityT m >>= runIdentityT . f) >>= runIdentityT . g
m >>= (\x -> f x >>= g)  
≡ IdentityT $ runIdentityT . (\x -> f x >>= g) =<< runIdentityT m
≡ IdentityT $ runIdentityT . (\x -> IdentityT $ runIdentityT . g =<< runIdentityT (f x)) =<< runIdentityT m
≡ IdentityT $ (\x -> runIdentityT $ IdentityT $ runIdentityT . g =<< runIdentityT (f x)) =<< runIdentityT m
≡ IdentityT $ (\x -> runIdentityT . g =<< runIdentityT (f x)) =<< runIdentityT m
≡ IdentityT $ runIdentityT m >>= (\x -> runIdentityT (f x) >>= runIdentityT . g)
根据内部 Monad 的法则：(m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
IdentityT $ (runIdentityT m >>= runIdentityT . f) >>= runIdentityT . g
≡ IdentityT $ runIdentityT m >>= (\x -> (runIdentityT . f) x >>= runIdentityT . g)
≡ IdentityT $ runIdentityT m >>= (\x -> runIdentityT (f x) >>= runIdentityT . g)
```

```
证明 StateT 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
lift . return $ a
≡ IdentityT (m a)
≡ return a
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ lift (n b)
≡ IdentityT (n b)
lift m >>= (lift . f)
≡ IdentityT (n a) >>= (\x -> lift . f $ x)
≡ IdentityT $ runIdentityT . IdentityT . f =<< runIdentityT (IdentityT (n a))
≡ IdentityT $ n a >>= f
≡ IdentityT (f a)
≡ IdentityT (n b)
```


