# Haskell语言学习笔记（58）Bifoldable - zwvista - 博客园

## [Haskell语言学习笔记（58）Bifoldable](https://www.cnblogs.com/zwvista/p/8016224.html)

### Bifoldable

```
class Bifoldable p where
  bifold :: Monoid m => p m m -> m
  bifold = bifoldMap id id

  bifoldMap :: Monoid m => (a -> m) -> (b -> m) -> p a b -> m
  bifoldMap f g = bifoldr (mappend . f) (mappend . g) mempty

  bifoldr :: (a -> c -> c) -> (b -> c -> c) -> c -> p a b -> c
  bifoldr f g z t = appEndo (bifoldMap (Endo #. f) (Endo #. g) t) z

  bifoldl :: (c -> a -> c) -> (c -> b -> c) -> c -> p a b -> c
  bifoldl f g z t = appEndo (getDual (bifoldMap (Dual . Endo . flip f) (Dual . Endo . flip g) t)) z
```

Bifoldable 是个类型类。主要用于折叠二元数据结构。

### Bifoldable 的法则

```
bifold ≡ bifoldMap id id
bifoldMap f g ≡ bifoldr (mappend . f) (mappend . g) mempty
bifoldr f g z t ≡ appEndo (bifoldMap (Endo . f) (Endo . g) t) z
```

### Either 是个 Bifoldable

```
instance Bifoldable Either where
  bifoldMap f _ (Left a) = f a
  bifoldMap _ g (Right b) = g b
```

### (,) 是个 Bifoldable

```
instance Bifoldable (,) where
  bifoldMap f g ~(a, b) = f a `mappend` g b
```

### Const 是个 Bifoldable

```
instance Bifoldable Const where
  bifoldMap f _ (Const a) = f a
```

### 应用 Bifoldable

```
Prelude Data.Bifoldable> bifoldr (^) (-) 2 (Left 2)
4
Prelude Data.Bifoldable> bifoldr (^) (-) 2 (Right 3)
1
Prelude Data.Bifoldable> bifoldr (^) (-) 2 (2,3)
2
Prelude Data.Bifoldable Control.Applicative> bifoldr (^) (-) 2 (Const 2)
4
Prelude Data.Bifoldable> bifoldl (^) (-) 4 (Left 2)
16
Prelude Data.Bifoldable> bifoldl (^) (-) 4 (Right 3)
1
Prelude Data.Bifoldable> bifoldl (^) (-) 4 (2,3)
13
Prelude Data.Bifoldable Control.Applicative> bifoldl (^) (-) 4 (Const 2)
16
```

### 手动计算

```
bifoldr (^) (-) 1 (2,3)
= appEndo (bifoldMap (Endo #. (^)) (Endo #. (-)) (2,3)) 1
= appEndo ((Endo #. (^) $ 2) (Endo #. (-) $ 3)) 1
= appEndo ((Endo (2^)) `mappend` (Endo (3-))) $ 1
= (2^) . (3-) $ 1
= 2 ^ (3 - 1) = 4
bifoldl (^) (-) 4 (2,3)
= appEndo (getDual (bifoldMap (Dual . Endo . flip (^)) (Dual . Endo . flip (-)) (2,3))) 4
= appEndo (getDual ((Dual . Endo . flip (^) $ 2)  `mappend` (Dual . Endo . flip (-) $ 3))) 4
= appEndo (getDual ((Dual $ Endo (^2)) `mappend` (Dual $ Endo (subtract 3)))) 4
= (subtract 3) . (^2) $ 4
= (4 ^ 2) - 3 = 13
```

### Bifoldable 其他函数

```
bifoldrM :: (Bifoldable t, Monad m) => (a -> c -> m c) -> (b -> c -> m c) -> c -> t a b -> m c
bifoldrM f g z0 xs = bifoldl f' g' return xs z0 where
  f' k x z = f x z >>= k
  g' k x z = g x z >>= k

bifoldlM :: (Bifoldable t, Monad m) => (a -> b -> m a) -> (a -> c -> m a) -> a -> t b c -> m a
bifoldlM f g z0 xs = bifoldr f' g' return xs z0 where
  f' x k z = f z x >>= k
  g' x k z = g z x >>= k

bitraverse_ :: (Bifoldable t, Applicative f) => (a -> f c) -> (b -> f d) -> t a b -> f ()
bitraverse_ f g = bifoldr ((*>) . f) ((*>) . g) (pure ())

bimapM_:: (Bifoldable t, Monad m) => (a -> m c) -> (b -> m d) -> t a b -> m ()
bimapM_ f g = bifoldr ((>>) . f) ((>>) . g) (return ())

biforM_ :: (Bifoldable t, Monad m) => t a b ->  (a -> m c) -> (b -> m d) -> m ()
biforM_ t f g = bimapM_ f g t

bisequenceA_ :: (Bifoldable t, Applicative f) => t (f a) (f b) -> f ()
bisequenceA_ = bifoldr (*>) (*>) (pure ())

bisequence_ :: (Bifoldable t, Monad m) => t (m a) (m b) -> m ()
bisequence_ = bifoldr (>>) (>>) (return ())

biList :: Bifoldable t => t a a -> [a]
biList = bifoldr (:) (:) []

biconcat :: Bifoldable t => t [a] [a] -> [a]
biconcat = bifold

biconcatMap :: Bifoldable t => (a -> [c]) -> (b -> [c]) -> t a b -> [c]
biconcatMap = bifoldMap

biany :: Bifoldable t => (a -> Bool) -> (b -> Bool) -> t a b -> Bool
biany p q = getAny . bifoldMap (Any . p) (Any . q)

biall :: Bifoldable t => (a -> Bool) -> (b -> Bool) -> t a b -> Bool
biall p q = getAll . bifoldMap (All . p) (All . q)
```

```
Prelude Data.Bifoldable> bifoldrM (\x y -> [x+y]) (\x y -> [x*y]) 4 (2,3)
[14]
Prelude Data.Bifoldable> bifoldrM (\x y -> [x+y, x+y+1]) (\x y -> [x*y, x*y*2]) 4 (2,3)
[14,15,26,27]
Prelude Data.Bifoldable> bifoldlM (\x y -> [x+y]) (\x y -> [x*y]) 4 (2,3)
[18]
Prelude Data.Bifoldable> bifoldlM (\x y -> [x+y, x+y+1]) (\x y -> [x*y, x*y*2]) 4 (2,3)
[18,36,21,42]
Prelude Data.Bifoldable> bitraverse_ print print ("hello", "world")
"hello"
"world"
Prelude Data.Bifoldable> bimapM_ print print ("hello", "world")
"hello"
"world"
Prelude Data.Bifoldable> biforM_ ("hello", "world") print print
"hello"
"world"
Prelude Data.Bifoldable> bisequenceA_ (print "hello", print "world")
"hello"
"world"
Prelude Data.Bifoldable> bisequence_ (print "hello", print "world")
"hello"
"world"
Prelude Data.Bifoldable> biList ("hello","world")
["hello","world"]
Prelude Data.Bifoldable> biconcat (["hello"],["world"])
["hello","world"]
Prelude Data.Bifoldable> biconcatMap (++",")(++"!") ("hello","world")
"hello,world!"
Prelude Data.Bifoldable> biany ('e' `elem`)('e' `elem`) ("hello","world")
True
Prelude Data.Bifoldable> biall ('e' `elem`)('e' `elem`) ("hello","world")
False
```


