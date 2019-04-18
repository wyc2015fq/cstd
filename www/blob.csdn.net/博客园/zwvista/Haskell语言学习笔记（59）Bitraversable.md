# Haskell语言学习笔记（59）Bitraversable - zwvista - 博客园

## [Haskell语言学习笔记（59）Bitraversable](https://www.cnblogs.com/zwvista/p/8016422.html)

### Bitraversable

```
class (Bifunctor t, Bifoldable t) => Bitraversable t where
  bitraverse :: Applicative f => (a -> f c) -> (b -> f d) -> t a b -> f (t c d)
  bitraverse f g = bisequenceA . bimap f g

bisequenceA :: (Bitraversable t, Applicative f) => t (f a) (f b) -> f (t a b)
bisequenceA = bitraverse id id
```

Bitraversable 是个类型类。主要用于遍历二元数据结构。

### Either 是个 Bitraversable

```
instance Bitraversable Either where
  bitraverse f _ (Left a) = Left <$> f a
  bitraverse _ g (Right b) = Right <$> g b
```

### (,) 是个 Bitraversable

```
instance Bitraversable (,) where
  bitraverse f g ~(a, b) = (,) <$> f a <*> g b
```

### Const 是个 Bitraversable

```
instance Bitraversable Const where
  bitraverse f _ (Const a) = Const <$> f a
```

### 应用 Bitraversable

```
Prelude Data.Bitraversable> bitraverse (\x -> [x, x+1]) (\x -> [x, x*2]) (Left 2)
[Left 2,Left 3]
Prelude Data.Bitraversable> bitraverse (\x -> [x, x+1]) (\x -> [x, x*2]) (Right 3)
[Right 3,Right 6]
Prelude Data.Bitraversable> bitraverse (\x -> [x, x+1]) (\x -> [x, x*2]) (2,3)
[(2,3),(2,6),(3,3),(3,6)]
Prelude Data.Bitraversable Control.Applicative> bitraverse (\x -> [x, x+1]) (\x -> [x, x*2]) (Const 2)
[Const 2,Const 3]
Prelude Data.Bitraversable> bisequenceA (Left [2,3])
[Left 2,Left 3]
Prelude Data.Bitraversable> bisequenceA (Right [2,3])
[Right 2,Right 3]
Prelude Data.Bitraversable> bisequenceA ([2,3],[4,5])
[(2,4),(2,5),(3,4),(3,5)]
Prelude Data.Bitraversable Control.Applicative> bisequenceA (Const [2,3])
[Const 2,Const 3]
```


