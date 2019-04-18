# Haskell语言学习笔记（51）Comonad - zwvista - 博客园

## [Haskell语言学习笔记（51）Comonad](https://www.cnblogs.com/zwvista/p/7887111.html)

### Comonad

```
class Functor w => Comonad w where
  extract :: w a -> a

  duplicate :: w a -> w (w a)
  duplicate = extend id

  extend :: (w a -> b) -> w a -> w b
  extend f = fmap f . duplicate
```

Comonad 是个类型类。

### 比较 Monad 和 Comonad

```
class Functor m => Monad m where
  return :: a -> m a
  bind :: (a -> m b) -> (m a -> m b)
  join :: m (m a) -> m a

  join = bind id
  bind f = fmap f . join
  
  (>>=) :: m a -> (a -> m b) -> m b
  (>>=) = flip bind

class Functor w => Comonad w where
  extract :: w a -> a
  extend :: (w a -> b) -> w a -> w b
  duplicate :: w a -> w (w a)

  duplicate = extend id
  extend f = fmap f . duplicate
  
  (=>>) :: w b -> (w b -> a) -> a
  (=>>) = flip extend
```

### ((,) e) 是个 Comonad

也称作 CoReader(Env) Comonad。

```
instance Comonad ((,)e) where
  duplicate p = (fst p, p)
  extract = snd
```

```
Prelude Control.Comonad> duplicate (3,4)
(3,(3,4))
Prelude Control.Comonad> extract (3,4)
4
Prelude Control.Comonad> extend fst (3,4)
(3,3)
Prelude Control.Comonad> extend snd (3,4)
(3,4)
```

### ((->)m) 是个 Comonad

也称作 CoWriter(Traced) Comonad。

```
instance Monoid m => Comonad ((->)m) where
  duplicate f m = f . mappend m
  extract f = f mempty
```

### 参考链接

[Haskell for all: Comonads are objects](http://www.haskellforall.com/2013/02/you-could-have-invented-comonads.html)
[Comonads in Haskell](https://www.slideshare.net/davidoverton/comonad)


