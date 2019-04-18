# Haskell语言学习笔记（63）Dicidable - zwvista - 博客园

## [Haskell语言学习笔记（63）Dicidable](https://www.cnblogs.com/zwvista/p/8017353.html)

### Dicidable

```
class Divisible f => Decidable f where
  lose :: (a -> Void) -> f a
  choose :: (a -> Either b c) -> f b -> f c -> f a

lost :: Decidable f => f Void
lost = lose id

chosen :: Decidable f => f b -> f c -> f (Either b c)
chosen = choose id
```

### Predicate 是个 Decidable

```
newtype Predicate a = Predicate { getPredicate :: a -> Bool }

instance Decidable Predicate where
  lose f = Predicate $ \a -> absurd (f a)
  choose f (Predicate g) (Predicate h) = Predicate $ either g h . f
```

### Comparison 是个 Decidable

```
newtype Comparison a = Comparison { getComparison :: a -> a -> Ordering }

instance Decidable Comparison where
  lose f = Comparison $ \a _ -> absurd (f a)
  choose f (Comparison g) (Comparison h) = Comparison $ \a b -> case f a of
    Left c -> case f b of
      Left d -> g c d
      Right{} -> LT
    Right c -> case f b of
      Left{} -> GT
      Right d -> h c d
```

### 应用 Decidable

```
Prelude Data.Functor.Contravariant Data.Functor.Contravariant.Divisible> getPredicate (choose  (\x -> if x `mod` 2==1 then Left (x `div` 2) else Right (x `div` 2)) (Predicate (<2)) (Predicate (<3))) 5
False
Prelude Data.Functor.Contravariant Data.Functor.Contravariant.Divisible> getPredicate (choose  (\x -> if x `mod` 2==1 then Left (x `div` 2) else Right (x `div` 2)) (Predicate (<2)) (Predicate (<3))) 4
True
```

### 参考链接

[ZuriHac 2015 - Discrimination is Wrong: Improving Productivity](https://www.youtube.com/watch?v=cB8DapKQz-I)```


