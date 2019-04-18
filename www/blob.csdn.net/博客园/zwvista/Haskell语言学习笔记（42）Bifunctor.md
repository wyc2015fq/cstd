# Haskell语言学习笔记（42）Bifunctor - zwvista - 博客园

## [Haskell语言学习笔记（42）Bifunctor](https://www.cnblogs.com/zwvista/p/7848403.html)

### Bifunctor

```
class Bifunctor p where
  bimap :: (a -> b) -> (c -> d) -> p a c -> p b d
  bimap f g = first f . second g

  first :: (a -> b) -> p a c -> p b c
  first f = bimap f id

  second :: (b -> c) -> p a b -> p a c
  second = bimap id
```

Bifunctor（双协变函子） 是个类型类。

Bifunctor类型类带两个协变类型参数。

Bifunctor类型类包含三个函数。
- bimap :: (a -> b) -> (c -> d) -> p a c -> p b d

bimap函数同时修改 Bifunctor 的两个参数。
- first :: (a -> b) -> p a c -> p b c

first函数只修改 Bifunctor 的第一个参数。
- second :: (b -> c) -> p a b -> p a c

second函数只修改 Bifunctor 的第二个参数。

### Bifunctor 的法则

法则

```
bimap id id ≡ id
first id ≡ id
second id ≡ id
bimap f g ≡ first f . second g
```

推论

```
bimap  (f . g) (h . i) ≡ bimap f h . bimap g i
first  (f . g) ≡ first  f . first  g
second (f . g) ≡ second f . second g
```

### Either 是个Bifunctor

```
instance Bifunctor Either where
  bimap f _ (Left a) = Left (f a)
  bimap _ g (Right b) = Right (g b)
```

### (,) 是个Bifunctor

```
instance Bifunctor (,) where
  bimap f g ~(a, b) = (f a, g b)
```

### Const 是个Bifunctor

```
instance Bifunctor Const where
  bimap f _ (Const a) = Const (f a)
```

### 应用 Bifunctor

```
Prelude Data.Bifunctor> bimap (+2) (*3) (1,2)
(3,6)
Prelude Data.Bifunctor> bimap (+2) (*3) (Left 2)
Left 4
Prelude Data.Bifunctor> bimap (+2) (*3) (Right 2)
Right 6
Prelude Data.Bifunctor> first (+2) (1,2)
(3,2)
Prelude Data.Bifunctor> second (*3) (1,2)
(1,6)
Prelude Data.Bifunctor> first (+2) (Left 2)
Left 4
Prelude Data.Bifunctor> second (*3) (Right 2)
Right 6
Prelude Data.Bifunctor Control.Applicative> first (+2) (Const 2)
Const 4
Prelude Data.Bifunctor Control.Applicative> second (+2) (Const 2)
Const 2
```


