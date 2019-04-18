# Haskell语言学习笔记（47）Arrow（2） - zwvista - 博客园

## [Haskell语言学习笔记（47）Arrow（2）](https://www.cnblogs.com/zwvista/p/7871894.html)

### Function, Monad, Arrow

```
f :: Int -> (Int, Int)
f = \x ->
  let y  = 2 * x
      z1 = y + 3
      z2 = y - 5
  in (z1, z2)

-- ghci> f 10
-- (23, 15)

fM :: Int -> Identity (Int, Int)
fM = \x -> do
  y  <- return (2 * x)
  z1 <- return (y + 3)
  z2 <- return (y - 5)
  return (z1, z2)

-- ghci> runIdentity (fM 10)
-- (23,15)

fA :: Int -> (Int, Int)
fA = proc x -> do
  y  <- (2 *) -< x
  z1 <- (+ 3) -< y
  z2 <- (subtract 5) -< y
  returnA -< (z1, z2)

-- ghci> fA 10
-- (23,15)
```

[24 Days of GHC Extensions: Arrows](https://ocharles.org.uk/blog/guest-posts/2014-12-21-arrows.html)

### ArrowZero, ArrowPlus, ArrowChoice, ArrowApply

```
class Arrow a => ArrowZero a where
    zeroArrow :: a b c

class ArrowZero a => ArrowPlus a where
    (<+>) :: a b c -> a b c -> a b c

class Arrow a => ArrowChoice a where
    left :: a b c -> a (Either b d) (Either c d)
    left = (+++ id)

    right :: a b c -> a (Either d b) (Either d c)
    right = (id +++)

    (+++) :: a b c -> a b' c' -> a (Either b b') (Either c c')
    f +++ g = left f >>> arr mirror >>> left g >>> arr mirror
      where
        mirror :: Either x y -> Either y x
        mirror (Left x) = Right x
        mirror (Right y) = Left y

    (|||) :: a b d -> a c d -> a (Either b c) d
    f ||| g = f +++ g >>> arr untag
      where
        untag (Left x) = x
        untag (Right y) = y

class Arrow a => ArrowApply a where
    app :: a (a b c, b) c
```

```
instance MonadPlus m => ArrowZero (Kleisli m) where
    zeroArrow = Kleisli (\_ -> mzero)

instance MonadPlus m => ArrowPlus (Kleisli m) where
    Kleisli f <+> Kleisli g = Kleisli (\x -> f x `mplus` g x)

instance ArrowChoice (->) where
    left f = f +++ id
    right f = id +++ f
    f +++ g = (Left . f) ||| (Right . g)
    (|||) = either

instance Monad m => ArrowChoice (Kleisli m) where
    left f = f +++ arr id
    right f = arr id +++ f
    f +++ g = (f >>> arr Left) ||| (g >>> arr Right)
    Kleisli f ||| Kleisli g = Kleisli (either f g)

instance ArrowApply (->) where
    app (f,x) = f x

instance Monad m => ArrowApply (Kleisli m) where
    app = Kleisli (\(Kleisli f, x) -> f x)
```

```
Prelude Control.Arrow> runKleisli (Kleisli (\x -> [x * 2]) <+> Kleisli (\x -> [x, -x])) 2
[4,2,-2]
Prelude Control.Arrow> either (+2) (*3) (Left 3)
5
Prelude Control.Arrow> either (+2) (*3) (Right 3)
9
Prelude Control.Arrow> (+2) ||| (*3) $ (Left 3)
5
Prelude Control.Arrow> (+2) +++ (*3) $ (Left 3)
Left 5
Prelude Control.Arrow> (+2) ||| (*3) $ (Right 3)
9
Prelude Control.Arrow> (+2) +++ (*3) $ (Right 3)
Right 9
Prelude Control.Arrow> left (+2) (Left 3)
Left 5
Prelude Control.Arrow> right (*3) (Right 3)
Right 9
Prelude Control.Arrow> left (+2) (Right 3)
Right 3
Prelude Control.Arrow> right (*3) (Left 3)
Left 3
Prelude Control.Arrow> runKleisli (Kleisli (\x -> [x * 2]) ||| Kleisli (\x -> [x, -x])) (Left 3)
[6]
Prelude Control.Arrow> runKleisli (Kleisli (\x -> [x * 2]) ||| Kleisli (\x -> [x, -x])) (Right 3)
[3,-3]
Prelude Control.Arrow> runKleisli (Kleisli (\x -> [x * 2]) +++ Kleisli (\x -> [x, -x])) (Left 3)
[Left 6]
Prelude Control.Arrow> runKleisli (Kleisli (\x -> [x * 2]) +++ Kleisli (\x -> [x, -x])) (Right 3)
[Right 3,Right (-3)]
Prelude Control.Arrow> (first (+) >>> app) (1,2)
3
Prelude Control.Arrow> (second (-) >>> snd &&& fst >>> app) (1,2)
1
```

### Arrow 含义

[How to read arrow combinators](https://kseo.github.io/posts/2016-12-11-how-to-read-arrow-combinators.html)


