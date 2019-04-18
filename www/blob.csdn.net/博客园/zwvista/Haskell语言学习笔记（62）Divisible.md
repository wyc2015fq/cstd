# Haskell语言学习笔记（62）Divisible - zwvista - 博客园

## [Haskell语言学习笔记（62）Divisible](https://www.cnblogs.com/zwvista/p/8017325.html)

### Divisible

```
class Contravariant f => Divisible f where
  divide  :: (a -> (b, c)) -> f b -> f c -> f a

  conquer :: f a

divided :: Divisible f => f a -> f b -> f (a, b)
divided = divide id

conquered :: Divisible f => f ()
conquered = conquer
```

### Predicate 是个 Divisible

```
newtype Predicate a = Predicate { getPredicate :: a -> Bool }

instance Divisible Predicate where
  divide f (Predicate g) (Predicate h) = Predicate $ \a -> case f a of
    (b, c) -> g b && h c
  conquer = Predicate $ const True
```

### Comparison 是个 Divisible

```
newtype Comparison a = Comparison { getComparison :: a -> a -> Ordering }

instance Divisible Comparison where
  divide f (Comparison g) (Comparison h) = Comparison $ \a b -> case f a of
    (a',a'') -> case f b of
      (b',b'') -> g a' b' `mappend` h a'' b''
  conquer = Comparison $ \_ _ -> EQ
```

### 应用 Divisible

```
Prelude Data.Functor.Contravariant Data.Functor.Contravariant.Divisible> getPredicate (divide (\x->(x,x)) (Predicate (>3)) (Predicate (<5))) 4
True
Prelude Data.Functor.Contravariant Data.Functor.Contravariant.Divisible> getPredicate conquer 2
True
```

### 参考链接

[ZuriHac 2015 - Discrimination is Wrong: Improving Productivity](https://www.youtube.com/watch?v=cB8DapKQz-I)


