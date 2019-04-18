# Haskell语言练习 - zwvista - 博客园

## [Haskell语言练习](https://www.cnblogs.com/zwvista/p/9208815.html)

### Monad

```
inc n = Just (n + 1)

add1 n = [n + 1]

main = do
    print $ Nothing >> (Just 0)                     -- Nothing
    print $ (Just 0) >> (Nothing :: Maybe Int)      -- Nothing
    print $ (Just 0) >> Nothing >> (Just 1)         -- Nothing
    print $ (Just 0) >> (Just 1) >> (Just 2)        -- Just 2

    print $ Nothing >>= inc >>= inc >>= inc         -- Nothing
    print $ (Just 0) >>= inc >>= inc >>= inc        -- Just 3

    print $ [] >> [1, 2]                            -- []
    print $ [1, 2] >> ([] :: [Int])                 -- []
    print $ [1] >> [3, 4, 5]                        -- [3,4,5]
    print $ [1, 2] >> [3, 4, 5]                     -- [3,4,5,3,4,5]
    print $ [1, 2, 3] >> [3, 4, 5]                  -- [3,4,5,3,4,5,3,4,5]

    print $ [] >>= add1 >>= add1 >>= add1           -- []
    print $ [1, 2, 3] >>= add1                      -- [2,3,4]
    print $ [1, 2, 3] >>= add1 >>= add1             -- [3,4,5]
    print $ [1, 2, 3] >>= add1 >>= add1 >>= add1    -- [4,5,6]
```

根据 Monad Maybe 的定义、

```
instance  Monad Maybe  where
    (Just x) >>= k      = k x
    Nothing  >>= _      = Nothing

    Just _m1 >> m2      = m2
    Nothing  >> _m2     = Nothing
```

可得

```
Nothing >>= inc = Nothing
(Just 0) >>= inc
= (Just 0) >>= (\n -> Just (n + 1))
= (\n -> Just (n + 1)) 0
= Just (0 + 1) = Just 1

Nothing >> (Just 0)  = Nothing
(Just 0) >> (Nothing :: Maybe Int)  = Nothing
(Just 1) >> (Just 2) = Just 2
```

根据 Monad [] 的定义、

```
instance Monad []  where
 xs >>= f             = [y | x <- xs, y <- f x]
 xs >> ys  = [y | _ <- xs, y <- ys]
```

可得

```
[1, 2, 3] >>= add1
= [1, 2, 3] >>= (\n -> [n + 1])
= [y | x <- [1, 2, 3], y <- [x + 1]]
= [2,3,4]
[1, 2, 3] >> [3, 4, 5] 
= [y | _ <- [1, 2, 3], y <- [3, 4, 5]]
= [3,4,5,3,4,5,3,4,5]
```

### State Monad

```
import Control.Monad.State

inc :: State Int Int
inc = do
    n <- get
    put (n + 1)
    return n

incBy :: Int -> State Int Int
incBy x = do
    n <- get
    modify (+x)
    return n

main = do
    print $ evalState inc 1                                         -- 1
    print $ execState inc 1                                         -- 2
    print $ runState inc 1                                          -- (1,2)
    print $ runState (withState (+3) inc) 1                         -- (4,5)
    print $ runState (mapState (\(a, s) -> (a + 3, s + 4)) inc) 1   -- (4,6)

    print $ runState (incBy 5) 10                                   -- (10,15)
```

关于 State Monad

get 将结果值设置为状态值 s，状态值 s 保持不变。

put s 将结果值设为空，将状态值设为 s。

return a 将结果值设为 a，状态值 s 保持不变。

modify f 将结果值设为空，将状态值设为 f s。

gets f 将结果值设为 f s，状态值 s 保持不变。

由此可得

```
--  假设初始状态值为 s
inc = do
    n <- get        -- (s,s)
    put (n + 1)     -- ((),s + 1)
    return n        -- (s,s + 1)
--  假设初始状态值为 s
incBy x = do
    n <- get        -- (s,s)
    modify (+x)     -- ((),s + x)
    return n        -- (s,s + x)
```

关于 State Monad

evalState s 针对 State Monad 利用初始状态值 s 进行状态计算，然后返回最终结果值 a’。

execState s 针对 State Monad 利用初始状态值 s 进行状态计算，然后返回最终状态值 s’。

runState s 针对 State Monad 利用初始状态值 s 进行状态计算，然后返回最终结果值和最终状态值组成的一对值 (a’, s')。

mapState f 针对 State Monad 进行状态计算之后，对最终结果值和状态值调用函数 f。

withState f 针对 State Monad 进行状态计算之前，对初始状态值调用函数 f。

由此可得

```
runState inc 1 = (1,1 + 1) = (1,2)
evalState inc 1 = 1
execState inc 1 = 2
runState (incBy 5) 10 = (10,10 + 5) = (10,15)
runState (withState (+3) inc) 1
= runState inc ((+3) 1)
= runState inc 4
= (4,5)
runState (mapState (\(a, s) -> (a + 3, s + 4)) inc) 1
= (\(a, s) -> (a + 3, s + 4)) (runState inc 1)
= (\(a, s) -> (a + 3, s + 4)) (1,2)
= (4,6)
```

### Reader Monad

```
import Control.Monad.Reader

data Environment = Environment { text1 :: String, text2 :: String }

getText :: Reader Environment String
getText = do
    t1 <- asks text1                -- Hello
    t2 <- asks text2                -- world!
    t3 <- withReader text1 ask      -- Hello
    t4 <- mapReader text2 ask       -- world!
    return $ t1 ++ ", " ++ t2 ++ ", " ++ t3 ++ ", " ++ t4

main = print $ runReader getText $ Environment "Hello" "world!"
```

### Writer Monad

```
import Control.Monad.Writer

write :: Int -> Writer [Int] String
write n = do
    tell [1..n]
    return "Done"

main = do
    print $ runWriter $ write 10    -- ("Done",[1,2,3,4,5,6,7,8,9,10])
    print $ execWriter $ write 10   -- [1,2,3,4,5,6,7,8,9,10]
```


