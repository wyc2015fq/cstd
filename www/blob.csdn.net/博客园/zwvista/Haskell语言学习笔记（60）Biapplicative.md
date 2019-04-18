# Haskell语言学习笔记（60）Biapplicative - zwvista - 博客园

## [Haskell语言学习笔记（60）Biapplicative](https://www.cnblogs.com/zwvista/p/8016556.html)

### Biapplicative

```
class Bifunctor p => Biapplicative p where
  bipure :: a -> b -> p a b

  (<<*>>) :: p (a -> b) (c -> d) -> p a c -> p b d

  (*>>) :: p a b -> p c d -> p c d
  a *>> b = bimap (const id) (const id) <<$>> a <<*>> b

  (<<*) :: p a b -> p c d -> p a b
  a <<* b = bimap const const <<$>> a <<*>> b

(<<$>>) :: (a -> b) -> a -> b
(<<$>>) = id
```

Biapplicative 是个类型类。主要用于在二元数据结构上应用 Applicative。

### (,) 是个 Biapplicative

```
instance Biapplicative (,) where
  bipure = (,)
  (f, g) <<*>> (a, b) = (f a, g b)
```

### Const 是个 Biapplicative

```
instance Biapplicative Const where
  bipure a _ = Const a
  Const f <<*>> Const x = Const (f x)
```

### 应用 Biapplicative

```
Prelude Data.Biapplicative> ((+2),(*3)) <<*>> (3,4)
(5,12)
Prelude Data.Biapplicative Control.Applicative> Const (+2) <<*>> Const 3
Const 5
Prelude Data.Biapplicative> bimap (+) (*) <<$>> (2,3) <<*>> (3,4)
(5,12)
Prelude Data.Biapplicative Control.Applicative> bimap (+) (*) <<$>> Const 2 <<*>> Const 3
Const 5
```

### (<<**>>), biliftA2, biliftA3

```
(<<**>>) :: Biapplicative p => p a c -> p (a -> b) (c -> d) -> p b d
(<<**>>) = biliftA2 (flip id) (flip id)

biliftA2 :: Biapplicative w => (a -> b -> c) -> (d -> e -> f) -> w a d -> w b e -> w c f
biliftA2 f g a b = bimap f g <<$>> a <<*>> b

biliftA3 :: Biapplicative w => (a -> b -> c -> d) -> (e -> f -> g -> h) -> w a e -> w b f -> w c g -> w d h
biliftA3 f g a b c = bimap f g <<$>> a <<*>> b <<*>> c
```

```
Prelude Data.Biapplicative> (2,3) <<**>> ((2^), (3*))
(4,9)
Prelude Data.Biapplicative> biliftA2 (+) (*) (2,3) (4,5)
(6,15)
Prelude Data.Biapplicative Control.Applicative> biliftA2 (+) (*) (Const 2) (Const 3)
Const 5
```


