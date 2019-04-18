# Haskell语言学习笔记（56）Lens（3） - zwvista - 博客园

## [Haskell语言学习笔记（56）Lens（3）](https://www.cnblogs.com/zwvista/p/7990821.html)

### 手动计算（view, over, set, to, _1）

```
view l = getConst . l Const

over l f = runIdentity . l (Identity . f)

set l b = runIdentity . l (\_ -> Identity b)

to k = dimap k (contramap k)

instance Field1 (a,b) (a',b) a a' where
  _1 k ~(a,b) = k a <&> \a' -> (a',b)
```

```
view _1 (1,2)
= getConst . _1 Const $ (1,2)
= getConst $ _1 Const (1,2)
= getConst $ Const 1 <&> \a' -> (a',2)
= getConst $ Const 1
= 1

over _1 (+1) (1,2)
= runIdentity . _1 (Identity . (+1)) $ (1,2)
= runIdentity $ _1 (Identity . (+1)) (1,2)
= runIdentity $ (Identity . (+1)) 1 <&> \a' -> (a',2)
= runIdentity $ Identity 2 <&> \a' -> (a',2)
= runIdentity $ Identity (2,2)
= (2,2)

set _1 3 (1,2)
= runIdentity . _1 (\_ -> Identity 3) $ (1,2)
= runIdentity $ _1 (\_ -> Identity 3) (1,2)
= runIdentity $ (\_ -> Identity 3) 1 <&> \a' -> (a',2)
= runIdentity $ Identity 3 <&> \a' -> (a',2)
= runIdentity $ Identity (3,2)
= (3,2)

view (_1 . to abs) (-1,2)
= getConst $ (_1 . to abs) Const (-1,2)
= getConst $ _1 (to abs Const) (-1,2)
= getConst $ (to abs Const) (-1) <&> \a' -> (a',2)
= getConst $ (dimap abs (contramap abs) (\a -> Const a)) (-1) <&> \a' -> (a',2)
= getConst $ ((contramap abs) . (\a -> Const a) . abs $ (-1)) <&> \a' -> (a',2)
= getConst $ ((contramap abs) . (\a -> Const a) $ 1) <&> \a' -> (a',2)
= getConst $ ((contramap abs) $ Const 1) <&> \a' -> (a',2)
= getConst $ Const 1 <&> \a' -> (a',2)
= getConst $ Const 1
= 1
```

### 参考内容：

### Const a 是 Functor，也是 Contravariant

```
newtype Const a b = Const { getConst :: a }

instance Functor (Const m) where
    fmap _ (Const v) = Const v

instance Contravariant (Const a) where
  contramap _ (Const a) = Const a
```

### (->) 是 Profunctor

```
instance Profunctor (->) where
  dimap ab cd bc = cd . bc . ab
```

### Identity 是 Functor

```
newtype Identity a = Identity { runIdentity :: a }

instance Functor Identity where
    fmap f m = Identity (f (runIdentity m))
```

### 手动计算 preview _Left (Left 5)

```
Prelude Control.Lens> preview _Left (Left 5)
Just 5
```

```
_Left = prism Left $ either Right (Left . Right)

prism bt seta = dimap seta (either pure (fmap bt)) . right'

instance Choice (->) where
  right' = fmap

preview l = getFirst . foldMapOf l (First . Just)

foldMapOf l f = getConst . l (Const . f)

instance Profunctor (->) where
  dimap ab cd bc = cd . bc . ab

instance Functor (Const m) where
    fmap _ (Const v) = Const v
```

```
preview _Left (Left 5)
= getFirst . foldMapOf _Left (First . Just) $ Left 5
= getFirst $ foldMapOf _Left (First . Just) $ Left 5
= getFirst $ getConst . _Left (Const . (First . Just)) $ Left 5
= getFirst $ getConst $ _Left (Const . (First . Just)) $ Left 5
= getFirst $ getConst $ prism Left (either Right (Left . Right)) (Const . First . Just) (Left 5)
= getFirst $ getConst $ (dimap (either Right (Left . Right)) (either pure (fmap Left)) . right') (Const . First . Just) (Left 5)
= ①

(dimap f g . h) x y
= (dimap f g $ h x) y
= g . (h x) . f $ y
= g . (h x) $ f y

①
= getFirst $ getConst $ (either pure (fmap Left)) . (right' (Const . First . Just)) $ (either Right (Left . Right)) (Left 5)
= getFirst $ getConst $ (either pure (fmap Left)) . (right' (Const . First . Just)) $ (Right 5)
= getFirst $ getConst $ (either pure (fmap Left)) $ right' (Const . First . Just) (Right 5)
= getFirst $ getConst $ (either pure (fmap Left)) $ fmap (Const . First . Just) (Right 5)
= getFirst $ getConst $ (either pure (fmap Left)) $ Right (Const . First $ Just 5)
= getFirst $ getConst $ fmap Left (Const . First $ Just 5)
= getFirst $ getConst $ fmap Left (Const . First $ Just 5)
= getFirst $ getConst $ Const . First $ Just 5
= Just 5
```

### 手动计算 set mapped 5 [1,2,3]

```
set l b = runIdentity . l (\_ -> Identity b)
mapped = sets fmap
sets f g = taintedDot (f (untaintedDot g))

instance Settable Identity where
  untainted = runIdentity
  untaintedDot = (runIdentity #.)
  taintedDot = (Identity #.)
```

```
set mapped 5 [1,2,3]
= runIdentity . (sets fmap) (\_ -> Identity 5) $ [1,2,3]
= runIdentity $ sets fmap (\_ -> Identity 5) $ [1,2,3]
= runIdentity $ taintedDot (fmap (untaintedDot (\_ -> Identity 5))) $ [1,2,3]
= runIdentity $ (Identity .) (fmap ((runIdentity .) (\_ -> Identity 5))) $ [1,2,3]
= runIdentity $ (Identity .) (fmap (fmap runIdentity (\_ -> Identity 5))) $ [1,2,3]
= runIdentity $ Identity . (fmap (\_ -> 5)) $ [1,2,3]
= runIdentity $ Identity $ fmap (\_ -> 5) [1,2,3]
= runIdentity $ Identity $ [5,5,5]
= [5,5,5]
```

### 手动计算 toListOf both (1,2)

```
toListOf :: Getting (Endo [a]) s a -> s -> [a]
toListOf l = foldrOf l (:) []

foldrOf :: Getting (Endo r) s a -> (a -> r -> r) -> r -> s -> r
foldrOf l f z = flip appEndo z . foldMapOf l (Endo #. f)

foldMapOf :: Getting r s a -> (a -> r) -> s -> r
foldMapOf l f = getConst #. l (Const #. f)

both :: Bitraversable r => Traversal (r a a) (r b b) a b
both f = bitraverse f f

instance Bitraversable (,) where
  bitraverse f g ~(a, b) = (,) <$> f a <*> g b

instance Functor (Const m) where
    fmap _ (Const v) = Const v

instance Monoid m => Applicative (Const m) where
    pure _ = Const mempty
    liftA2 _ (Const x) (Const y) = Const (x `mappend` y)
    (<*>) = coerce (mappend :: m -> m -> m)
```

```
toListOf both (1,2)
= foldrOf both (:) [] (1,2)
= flip appEndo [] . foldMapOf both (Endo #. (:)) $ (1,2)
= flip appEndo [] . getConst #. both (Const #. (Endo #. (:))) $ (1,2)
= flip appEndo [] . getConst #. bitraverse (Const #. (Endo #. (:))) (Const #. (Endo #. (:))) $ (1,2)
= flip appEndo [] . getConst $ bitraverse (Const #. (Endo #. (:))) (Const #. (Endo #. (:))) (1,2)
= flip appEndo [] . getConst $ (,) <$> (Const #. (Endo #. (:))) 1 <*> (Const #. (Endo #. (:))) 2
= flip appEndo [] . getConst $ (,) <$> (Const . Endo . (:)) 1 <*> (Const . Endo . (:)) 2
= flip appEndo [] . getConst $ (,) <$> (Const . Endo $ (:) 1) <*> (Const . Endo $ (:) 2)
= flip appEndo [] . getConst $ (,) <$> (Const . Endo $ (1:)) <*> (Const . Endo $ (2:))
= flip appEndo [] . getConst $ (,) <$> (Const (Endo (1:))) <*> (Const (Endo (2:)))
= flip appEndo [] . getConst $ (Const (Endo (1:))) <*> (Const (Endo (2:)))
= flip appEndo [] . getConst $ (Const (Endo (1:) <> Endo (2:)))
= flip appEndo [] $ getConst (Const (Endo ((1:) . (2:))))
= flip appEndo [] (Endo ((1:) . (2:)))
= appEndo (Endo ((1:) . (2:))) []
= [1,2]
```


