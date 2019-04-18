# Haskell语言学习笔记（61）Distributive - zwvista - 博客园

## [Haskell语言学习笔记（61）Distributive](https://www.cnblogs.com/zwvista/p/8016689.html)

### Distributive

```
class Functor g => Distributive g where
  distribute  :: Functor f => f (g a) -> g (f a)
  distribute  = collect id

  collect     :: Functor f => (a -> g b) -> f a -> g (f b)
  collect f   = distribute . fmap f

  distributeM :: Monad m => m (g a) -> g (m a)
  distributeM = fmap unwrapMonad . distribute . WrapMonad

  collectM    :: Monad m => (a -> g b) -> m a -> g (m b)
  collectM f  = distributeM . liftM f

cotraverse :: (Distributive g, Functor f) => (f a -> b) -> f (g a) -> g b
cotraverse f = fmap f . distribute

comapM :: (Distributive g, Monad m) => (m a -> b) -> m (g a) -> g b
comapM f = fmap f . distributeM
```

Distributive 是个类型类。功能上与 Traversable 类型类呈现对偶（dual）关系。

Traversable 类型将一个函数应用到多个数值，而

Distributive 类型将一个数值应用到多个函数。

### ((->)e) 是个 Distributive

```
instance Distributive ((->)e) where
  distribute a e = fmap ($e) a
  collect f q e = fmap (flip f e) q
```

### Sum 是个 Distributive

```
instance Distributive Monoid.Sum where
  collect = coerce (fmap :: (a -> b) -> f a -> f b)
    :: forall f a b . Functor f
    => (a -> Monoid.Sum b) -> f a -> Monoid.Sum (f b)
  distribute = Monoid.Sum . fmap Monoid.getSum
```

### 应用 Distributive

```
Prelude Data.Distributive> distribute [(+1),(+2)] 1
[2,3]
Prelude Data.Distributive> collect (^) [1,2] 3
[1,8]
Prelude Data.Distributive> cotraverse sum [(+1),(*2)] 3
10
Prelude Data.Distributive> comapM product [(+1),(*2)] 3
24
Prelude Data.Distributive Data.Monoid> distribute [Sum 1, Sum 2]
Sum {getSum = [1,2]}
Prelude Data.Distributive Data.Monoid> collect (\x->Sum[x]) [3,4]
Sum {getSum = [[3],[4]]}
```

### 手动计算

```
distribute [(+1),(+2)] 1
= fmap ($1) [(+1),(+2)]
= [1+1,1+2] = [2,3]
collect (^) [1,2] 3
= fmap (flip (^) 3) [1,2]
= fmap (^3) [1,2]
= [1^3,2^3] = [1,8]
cotraverse sum [(+1),(*2)] 3
= (fmap sum . distribute) [(+1),(*2)] 3
= sum $ distribute [(+1),(*2)] 3
= sum [4, 6] = 10

这里
(fmap f . g)  x y
= (f . g x) y
= f $ g x y
```


