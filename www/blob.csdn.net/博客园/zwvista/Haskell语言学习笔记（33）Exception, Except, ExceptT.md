# Haskell语言学习笔记（33）Exception, Except, ExceptT - zwvista - 博客园

## [Haskell语言学习笔记（33）Exception, Except, ExceptT](https://www.cnblogs.com/zwvista/p/7711576.html)

### Exception

```
class (Typeable e, Show e) => Exception e where
    toException   :: e -> SomeException
    fromException :: SomeException -> Maybe e

    toException = SomeException
    fromException (SomeException e) = cast e

    displayException :: e -> String
    displayException = show

instance Exception SomeException where
    toException se = se
    fromException = Just
    displayException (SomeException e) = displayException e
```

### ExceptT Monad转换器

```
newtype ExceptT e m a = ExceptT (m (Either e a))

instance (Monad m) => Monad (ExceptT e m) where
    return a = ExceptT $ return (Right a)
    m >>= k = ExceptT $ do
        a <- runExceptT m
        case a of
            Left e -> return (Left e)
            Right x -> runExceptT (k x)

runExceptT :: ExceptT e m a -> m (Either e a)
runExceptT (ExceptT m) = m
```
- newtype ExceptT e m a = ExceptT (m (Either e a))

ExceptT 类型是个 newtype，也就是对现有类型的封装。该类型有两个类型参数：内部 Monad 类型 m 以及基础 Monad Either 的参数类型 e 和 a。

ExceptT e m a 封装了一个 m (Either e a) 类型的值，通过 runExceptT 函数可以取出这个值。
- instance (Monad m) => Monad (ExceptT e m) where

如果 m 是个 Monad，那么 ExceptT e m 也是一个 Monad。

对比 Monad 类型类的定义，可知 return 函数的类型签名为：

return :: a -> ExceptT e m a

大致相当于 a -> m (Either e a)

而 bind 操作符的类型签名为：

(>>=) :: ExceptT e m a -> (a -> ExceptT e m b) -> ExceptT e m b

大致相当于 m (Either e a) -> (a -> m (Either e b)) -> m (Either e b)
- return a = ExceptT $ return (Right a)

return 函数首先将 Right a 封装进内部 Monad m 中，然后再把它封装进 ExceptT 这个 Monad 转换器之中。

这里左侧的 return 是 ExceptT 这个 Monad 的 return，而右侧的 return 是内部 Monad m 的 return。
- m >>= k = ExceptT $ do

对比函数签名，可知 m 的类型是 ExceptT e m a，大致相当于 m (Either e a)

而 k 的类型是 a -> ExceptT e m b，大致相当于 a -> m (Either e b)
- a <- runExceptT m

对比 m 的类型，可知 a 的类型是 Either e a

这是因为 runMaybeT 函数让 m 脱离了 ExceptT Monad， 而 <- 运算符又让 runExceptT m 脱离了内部 Monad m。
- case a of
- Left e -> return (Left e)

这里 return 是内部 Monad m 的 return，所以 return (Left e) 的类型是 m (Either e a)。
- Right x -> runExceptT (k x)

k 的类型是 a -> ExceptT e m b

所以 k x 的类型是 ExceptT e m b

而 runExceptT (k x) 的类型是 m (Either e b)

```
证明 ExceptT e m 符合 Monad 法则。
1. return a >>= f ≡ f a
return a >>= f
≡ (ExceptT $ return (Right a)) >>= f
≡ ExceptT (m (Right a)) >>= f
≡ ExceptT $ runExceptT (f a)
≡ f a
2. m >>= return ≡ m
ExceptT (m (Right x)) >>= return
≡ ExceptT $ runExceptT (return x)
≡ ExceptT (m (Right x))
ExceptT (m (Left e)) >>= return
≡ ExceptT $ runExceptT (return (Left e))
≡ ExceptT (m (Left e))
3. (m >>= f) >>= g ≡ m >>= (\x -> f x >>= g)
(ExceptT (m (Right x)) >>= f) >>= g ≡ f x >>= g
(ExceptT (m (Left e)) >>= f) >>= g ≡ ExceptT (m (Left e)) >>= g ≡ ExceptT (m (Left e))
ExceptT (m (Right x) >>= (\x -> f x >>= g) ≡ (\x -> f x >>= g) x ≡ f x >>= g
ExceptT (m (Left e)) >>= (\x -> f x >>= g) ≡ ExceptT (m (Left e))
```

### lift, liftIO 函数

```
instance MonadTrans (ExceptT e) where
    lift = ExceptT . liftM Right

instance (MonadIO m) => MonadIO (ExceptT e m) where
    liftIO = lift . liftIO
```

```
证明 MaybeT 中 lift 函数的定义符合 lift 的法则。
1. lift . return ≡ return
lift . return $ a
≡ ExceptT . liftM Right . return $ a
≡ ExceptT (m (Right a))
≡ return a
2. lift (m >>= f) ≡ lift m >>= (lift . f)
假设 m = n a 并且 f a = n b
于是 m >>= f = n b
lift (m >>= f)
≡ ExceptT . liftM Right $ m >>= f
≡ ExceptT . liftM Right $ n b
≡ ExceptT (n (Right b))
lift m >>= (lift . f)
≡ (ExceptT . liftM Right $ m) >>= (ExceptT . liftM Right . f)
≡ (ExceptT (n (Right a))) >>= (\x -> ExceptT . liftM Right . f $ x)
≡ ExceptT $ runExceptT $ ExceptT . liftM Right . f $ a
≡ ExceptT $ runExceptT $ ExceptT . liftM Right $ n b
≡ ExceptT $ runExceptT $ ExceptT (n (Right b))
≡ ExceptT (n (Right b))
```

### Except Monad
`type Except e = ExceptT e Identity`
Except Monad 是 ExceptT Monad（转换器） 的一个特例。

### 应用实例

```
-- https://stackoverflow.com/questions/26385809/catch-someexception-with-exceptt
-- cabal install lifted-base

import Control.Exception.Lifted
import Control.Monad.Trans.Except

badFunction :: ExceptT SomeException IO ()
badFunction = throw DivideByZero

intercept
  :: ExceptT SomeException IO a
  -> ExceptT SomeException IO a
intercept a = do
  r <- try $ a
  case r of
    Right x -> return x
    Left e -> throwE e

intercept'
  :: ExceptT SomeException IO a
  -> ExceptT SomeException IO a
intercept' = handle throwE

main :: IO ()
main = do
    r <- runExceptT $ intercept badFunction
    case r of Left _ -> putStrLn "caught error"
              Right _ -> putStrLn "nope, didn't catch no error"
```


