# Haskell语言学习笔记（40）Arrow（1） - zwvista - 博客园

## [Haskell语言学习笔记（40）Arrow（1）](https://www.cnblogs.com/zwvista/p/7837138.html)

### Arrow

```
class Category a => Arrow a where
    arr :: (b -> c) -> a b c

    first :: a b c -> a (b,d) (c,d)
    first = (*** id)
 
    second :: a b c -> a (d,b) (d,c)
    second = (id ***)

    (***) :: a b c -> a b' c' -> a (b,b') (c,c')
    f *** g = first f >>> arr swap >>> first g >>> arr swap
      where swap ~(x,y) = (y,x)

    (&&&) :: a b c -> a b c' -> a b (c,c')
    f &&& g = arr (\b -> (b,b)) >>> f *** g
```

Arrow（箭头）是个类型类，它是 Category（范畴）的子类。

Arrow 是函数的进一步抽象。
- arr :: (b -> c) -> a b c

arr 函数将函数转换为 Arrow。
- first :: a b c -> a (b,d) (c,d)

first 函数接收一个二元组，然后使用一个 Arrow 来修改元组的第1个成员。
- second :: a b c -> a (d,b) (d,c)

second 函数接收一个二元组，然后使用一个 Arrow 来修改元组的第2个成员。
- (***) :: a b c -> a b' c' -> a (b,b') (c,c')

(***) 函数接收一个二元组，然后分别使用两个 Arrow 来修改这个元组的两个成员。
- (&&&) :: a b c -> a b c' -> a b (c,c')

(&&&) 函数只接收一个参数，拷贝一份，组成一个二元组，然后分别使用两个 Arrow 来修改这个元组的两个成员。

### Arrow 的法则

```
arr id = id
arr (f >>> g) = arr f >>> arr g
first (arr f) = arr (first f)
first (f >>> g) = first f >>> first g
first f >>> arr fst = arr fst >>> f
first f >>> arr (id *** g) = arr (id *** g) >>> first f
first (first f) >>> arr assoc = arr assoc >>> first f
    where
        assoc ((a,b),c) = (a,(b,c))
```

### (->) 是个 Arrow

```
instance Arrow (->) where
    arr f = f
    (***) f g ~(x,y) = (f x, g y)
```

```
Prelude Control.Arrow> (+2) &&& (*2) $ 3
(5,6)
Prelude Control.Arrow> (+2) *** (*2) $ (1,3)
(3,6)
Prelude Control.Arrow> (+2) <<< (*2) $ 3
8
Prelude Control.Arrow> (*2) >>> (+2) $ 3
8
Prelude Control.Arrow> first (*2) (1,3)
(2,3)
Prelude Control.Arrow> second (*2) (1,3)
(1,6)
Prelude Control.Arrow> arr (*2) 3
6
```

### Kleisli Arrow

```
newtype Kleisli m a b = Kleisli { runKleisli :: a -> m b }

instance Monad m => Category (Kleisli m) where
    id = Kleisli return
    (Kleisli f) . (Kleisli g) = Kleisli (\b -> g b >>= f)

instance Monad m => Arrow (Kleisli m) where
    arr f = Kleisli (return . f)
    first (Kleisli f) = Kleisli (\ ~(b,d) -> f b >>= \c -> return (c,d))
    second (Kleisli f) = Kleisli (\ ~(d,b) -> f b >>= \c -> return (d,c))
```

Kleisli m a b封装了一个返回值为 Monad 的函数：a -> m b。

如果 m 是 Monad，那么 Kleisli m 是 Category，也是 Arrow。

```
Prelude Control.Arrow> runKleisli (Kleisli (\x -> [x * 2]) >>> Kleisli (\x -> [x, -x])) 2
[4,-4]
Prelude Control.Monad> (\x -> [x * 2]) >=> (\x -> [x, -x]) $ 2
[4,-4]
Prelude Control.Arrow> runKleisli (arr length >>> Kleisli print) [2,2]
2
```

### Arrow 的函数

```
returnA :: Arrow a => a b b
returnA = arr id

(^>>) :: Arrow a => (b -> c) -> a c d -> a b d
f ^>> a = arr f >>> a

(>>^) :: Arrow a => a b c -> (c -> d) -> a b d
a >>^ f = a >>> arr f

(<<^) :: Arrow a => a c d -> (b -> c) -> a b d
a <<^ f = a <<< arr f

(^<<) :: Arrow a => (c -> d) -> a b c -> a b d
f ^<< a = arr f <<< a
```

```
Prelude Control.Arrow> (returnA :: Int -> Int) 5
5
Prelude Control.Arrow> runKleisli (returnA :: Kleisli [] Int Int) 5
[5]
Prelude Control.Arrow> runKleisli (length ^>> Kleisli print) [2,2]
2
Prelude Control.Arrow> runKleisli (Kleisli print <<^ length) [2,2]
2
```


