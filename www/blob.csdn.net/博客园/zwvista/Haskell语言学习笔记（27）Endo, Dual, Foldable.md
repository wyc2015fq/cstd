# Haskell语言学习笔记（27）Endo, Dual, Foldable - zwvista - 博客园

## [Haskell语言学习笔记（27）Endo, Dual, Foldable](https://www.cnblogs.com/zwvista/p/7665349.html)

### Endo Monoid

```
newtype Endo a = Endo { appEndo :: a -> a }
instance Monoid (Endo a) where
        mempty = Endo id
        Endo f `mappend` Endo g = Endo (f . g)
```

Endo 是个 newtype，也就是对现有类型的封装。

Endo a 封装的是一个自反射的函数，即 a->a。通过 appEndo 字段可以取出这个函数。

Endo a 在结合时结合两个函数，因此它本质上是对函数合成运算符 (.) 的封装。

Endo a 是一个幺半群。这是因为自反射函数在函数合成时满足结合律。

### Dual Monoid

```
newtype Dual a = Dual { getDual :: a }
instance Monoid a => Monoid (Dual a) where
        mempty = Dual mempty
        Dual x `mappend` Dual y = Dual (y `mappend` x)
```

Dual 是个 newtype，也就是对现有类型的封装。

Dual a 封装的是一个值，即 a。通过 getDual 字段可以取出这个值。

Dual a 是一个幺半群，前提是 a 是个幺半群。

Dual a 在结合时交换两个操作数的值，因此它本质上是对 flip 函数的封装。

```
证明 Dual a 满足结合律。
(Dual x <> Dual y) <> Dual z
= Dual (y <> x) <> Dual z
= Dual (z <> (y <> x))
Dual x <> (Dual y <> Dual z)
= Dual x <> Dual (z <> y)
= Dual ((z <> y) <> x)
由于 a 是 Monoid 类型, 满足结合律
所以 (z <> y) <> x = z <> (y <> x)
=> Dual (z <> (y <> x)) = Dual ((z <> y) <> x)
=> Dual x <> (Dual y <> Dual z) = (Dual x <> Dual y) <> Dual z
```

### Foldable 的法则

```
class Foldable t where
    fold :: Monoid m => t m -> m
    fold = foldMap id

    foldMap :: Monoid m => (a -> m) -> t a -> m
    foldMap f = foldr (mappend . f) mempty

    foldr :: (a -> b -> b) -> b -> t a -> b
    foldr f z t = appEndo (foldMap (Endo #. f) t) z

    foldl :: (b -> a -> b) -> b -> t a -> b
    foldl f z t = appEndo (getDual (foldMap (Dual . Endo . flip f) t)) z
```

Foldable 类型类的实例都必须符合以下法则。

```
1. foldr f z t = appEndo (foldMap (Endo . f) t ) z
2. foldl f z t = appEndo (getDual (foldMap (Dual . Endo . flip f) t)) z
3. fold = foldMap id
```

### [] 是个 Foldable

```
instance Foldable [] where
    foldl   = List.foldl
    foldr   = List.foldr
```

```
证明 Foldable 类型类的实例 [] 符合 Foldable 的法则
1. foldr f z t = appEndo (foldMap (Endo . f) t ) z
foldr f z t
= List.foldr f z [x1, x2, ..., xn]
= x1 `f` (x2 `f` (...(xn `f` z)...))
appEndo (foldMap (Endo . f) t ) z
= appEndo (foldr (mappend . (Endo . f)) mempty [x1, x2, ..., xn]) z
= appEndo (foldr (mappend . (Endo . f)) (Endo id) [x1, x2, ..., xn]) z
= appEndo ( (mappend . (Endo . f)) x1 ( (mappend . (Endo . f)) x2 ( ... ( (mappend . (Endo . f)) xn (Endo id) ) ... ) ) ) z
= appEndo ( (Endo . (x1 `f`)) <> ( (Endo . (x2 `f`)) <> ( ... <> ( (Endo . (f . id)) xn ) ... ) ) ) z
= appEndo ( Endo . ((x1 `f`) . (x2 `f`) . ... . (xn `f`)) ) $ z
= (x1 `f`) . (x2 `f`) . ... . (xn `f`) $ z
= x1 `f` (x2 `f` (...(xn `f` z)...))
2. foldl f z t = appEndo (getDual (foldMap (Dual . Endo . flip f) t)) z
foldl f z t
= List.foldl f z [x1, x2, ..., xn]
= (...((z `f` x1) `f` x2)...) `f` xn
appEndo (getDual (foldMap (Dual . Endo . flip f) t)) z
= appEndo (getDual (foldr (mappend . (Dual . Endo . flip f)) mempty [x1, x2, ..., xn])) z
= appEndo (getDual (foldr (mappend . (Dual . Endo . flip f)) (Dual . Endo . id) [x1, x2, ..., xn])) z
= appEndo ( getDual ( (Dual . Endo . flip f) x1 <> ( (Dual . Endo . flip f) x2 <> ( ... <> ( (Dual . Endo . (flip f . id)) xn ) ... ) ) ) ) z
= appEndo ( getDual ( (Dual . Endo . (`f` x1)) <> ( (Dual . Endo . (`f` x2)) <> ( ... <> ( (Dual . Endo . (`f` xn)) ) ... ) ) ) ) z
= appEndo ( getDual ( (Dual . Endo . ((`f` xn) . ... . (`f` x2) . (`f` x1)) ) ) z
=  (`f` xn) . ... . (`f` x2) . (`f` x1) $ z
= (...((z `f` x1) `f` x2)...) `f` xn
```

### build 与 (++)

```
build   :: forall a. (forall b. (a -> b -> b) -> b -> b) -> [a]
build g = g (:) []

(++) xs ys = foldr (:) ys xs
```

### foldMap 与 concatMap

foldMap :: Monoid m => (a -> m) -> t a -> m

foldMap f = foldr (mappend . f) mempty

当 m := [b] 时 foldMap := concatMap

```
concatMap :: Foldable t => (a -> [b]) -> t a -> [b]
concatMap f xs = build (\c n -> foldr (\x b -> foldr c b (f x)) n xs)
```

```
concatMap f xs
= build (\c n -> foldr (\x b -> foldr c b (f x)) n xs)
= (\c n -> foldr (\x b -> foldr c b (f x)) n xs) (:) []
= foldr (\x b -> foldr (:) b (f x)) [] xs

foldMap f xs
= foldr (mappend . f) mempty xs
= foldr ((++) . f) [] xs
= foldr (\x b -> foldr (:) b (f x)) [] xs
```

### fold 与 concat

fold :: Monoid m => t m -> m

fold = foldMap id

当 m := [a] 时 fold := concat

```
concat :: Foldable t => t [a] -> [a]
concat xs = build (\c n -> foldr (\x y -> foldr c y x) n xs)
```

```
concat xs
= build (\c n -> foldr (\x y -> foldr c y x) n xs)
= (\c n -> foldr (\x y -> foldr c y x) n xs) (:) xs
= foldr (\x y -> foldr (:) y x) [] xs

fold xs
= foldMap id xs
= foldr (mappend . id) mempty xs
= foldr (++) [] xs
= foldr (\x y -> foldr (:) y x) [] xs
```

### reverse foldl foldr

用 foldl foldr 实现 reverse

[How can I write reverse by foldr efficiently in Haskell?](https://stackoverflow.com/questions/7862881/how-can-i-write-reverse-by-foldr-efficiently-in-haskell)

```
reverse' xs
= foldl (flip (:)) [] xs
= appEndo . getDual $ foldMap (Dual . Endo . flip (flip (:))) xs $ []
= appEndo . getDual $ foldr (mappend . Dual . Endo . (:)) mempty xs $ []
= appEndo . foldr (flip mappend . Endo . (:)) mempty $ xs $ []
= foldr (flip (.) . (:)) id xs []
= flip (foldr (flip (.) . (:)) id) [] xs
reverse' = flip (foldr (flip (.) . (:)) id) []
```

将 reverse' 的推导过程一般化，可以得到

```
foldl f
= flip (foldr (flip (.) . flip f) id)
```

下面证明

```
foldl f z xs
= foldr (flip (.) . flip f) id xs z
= foldr step id xs z
  where step x g a = g (f a x)

step x g a = g (f a x)
step x g a = g ((flip f) x a)
step x g a = g . (flip f) x $ a
step x g   = g . (flip f) x
step x g   = (.) g ((flip f) x)
step x g   = (flip (.)) ((flip f) x) g
step x     = (flip (.)) ((flip f) x)
step x     = flip (.) . flip f $ x
step       = flip (.) . flip f
```


