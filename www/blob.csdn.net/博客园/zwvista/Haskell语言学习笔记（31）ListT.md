# Haskell语言学习笔记（31）ListT - zwvista - 博客园

## [Haskell语言学习笔记（31）ListT](https://www.cnblogs.com/zwvista/p/7710722.html)

### Control.Monad.Trans.List

标准库中的 ListT 的实现由于有 bug，已经被废弃。

### list-t 模块

这里使用 list-t 模块中的 ListT。

list-t 模块需要安装
[ListT done right](https://wiki.haskell.org/ListT_done_right)

```
$ cabal install list-t
list-t-1.0.0.1
Prelude> :m +ListT
Prelude ListT>
```

### ListT Monad转换器

```
newtype ListT m a = ListT (m (Maybe (a, ListT m a)))

-- * Execution in the inner monad
-------------------------

-- Execute in the inner monad,
-- getting the head and the tail.
-- Returns nothing if it's empty.
uncons :: ListT m a -> m (Maybe (a, ListT m a))

-- Execute, getting the head. Returns nothing if it's empty.
head :: Monad m => ListT m a -> m (Maybe a)

-- Execute, getting the tail. Returns nothing if it's empty.
tail :: Monad m => ListT m a -> m (Maybe (ListT m a))

-- Execute, checking whether it's empty.
null :: Monad m => ListT m a -> m Bool

-- Execute, applying a left fold.
fold :: Monad m => (r -> a -> m r) -> r -> ListT m a -> m r

-- A version of 'fold', which allows early termination.
foldMaybe :: Monad m => (r -> a -> m (Maybe r)) -> r -> ListT m a -> m r

-- Execute, folding to a list.
toList :: Monad m => ListT m a -> m [a]

-- Execute, folding to a list in the reverse order.
-- Performs more efficiently than 'toList'.
toReverseList :: Monad m => ListT m a -> m [a]

-- Execute, traversing the stream with a side effect in the inner monad. 
traverse_ :: Monad m => (a -> m ()) -> ListT m a -> m ()

-- Execute, consuming a list of the specified length and returning the remainder stream.
splitAt :: Monad m => Int -> ListT m a -> m ([a], ListT m a)

-- * Construction
-------------------------

-- Prepend an element.
cons :: Monad m => a -> ListT m a -> ListT m a

-- Construct from any foldable.
fromFoldable :: (Monad m, Foldable f) => f a -> ListT m a

-- Construct by unfolding a pure data structure.
unfold :: Monad m => (b -> Maybe (a, b)) -> b -> ListT m a

-- Construct by unfolding a monadic data structure
-- This is the most memory-efficient way to construct ListT where
-- the length depends on the inner monad.
unfoldM :: Monad m => (b -> m (Maybe (a, b))) -> b -> ListT m a

-- Produce an infinite stream.
repeat :: Monad m => a -> ListT m a

-- * Transformation
-------------------------

-- A transformation,
-- which traverses the stream with an action in the inner monad.
traverse :: Monad m => (a -> m b) -> ListT m a -> ListT m b

-- A transformation,
-- reproducing the behaviour of @Data.List.'Data.List.take'@.
take :: Monad m => Int -> ListT m a -> ListT m a

-- A transformation,
-- reproducing the behaviour of @Data.List.'Data.List.drop'@.
drop :: Monad m => Int -> ListT m a -> ListT m a

-- A transformation,
-- which slices a list into chunks of the specified length.
slice :: Monad m => Int -> ListT m a -> ListT m [a]
```

```
Prelude ListT> toList (return 1 :: ListT IO Int)
[1]
Prelude ListT> toList (return 1 :: ListT Maybe Int)
Just [1]
Prelude ListT> toList (fromFoldable [1,2,3] :: ListT Maybe Int)
Just [1,2,3]
Prelude ListT Control.Monad.Trans> toList $ (return 1 :: ListT IO Int) >>= lift . print >> return (1,2)
1
[(1,2)]
```

### 应用实例

```
import Control.Monad
import Control.Monad.Trans
import ListT

myTest :: Int -> ListT IO (Int, Int)
myTest n = do
    let squares = fromFoldable . takeWhile (<=n) $ map (^(2::Int)) [0..]
    x <- squares
    y <- squares
    lift $ print (x,y)
    guard $ x + y == n
    lift $ putStrLn "Sum of squares."
    return (x,y)

main = toList $ myTest 5
```

```
(0,0)
(0,1)
(0,4)
(1,0)
(1,1)
(1,4)
Sum of squares.
(4,0)
(4,1)
Sum of squares.
(4,4)
[(1,4),(4,1)]
```


