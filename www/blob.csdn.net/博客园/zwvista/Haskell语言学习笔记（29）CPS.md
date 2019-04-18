# Haskell语言学习笔记（29）CPS - zwvista - 博客园

## [Haskell语言学习笔记（29）CPS](https://www.cnblogs.com/zwvista/p/7683912.html)

### CPS (Continuation Passing Style)

CPS（延续传递风格）是指函数不把处理结果作为返回值返回而是把处理结果传递给下一个函数的编码风格。

与此相对，函数把处理结果作为返回值返回的编码风格被称为直接编码风格。

```
add :: Int -> Int -> Int
add x y = x + y

square :: Int -> Int
square x = x * x

pythagoras :: Int -> Int -> Int
pythagoras x y = add (square x) (square y)

add_cps :: Int -> Int -> ((Int -> r) -> r)
add_cps x y = \k -> k (add x y)

square_cps :: Int -> ((Int -> r) -> r)
square_cps x = \k -> k (square x)

pythagoras_cps :: Int -> Int -> ((Int -> r) -> r)
pythagoras_cps x y = \k ->
    square_cps x $ \x_squared ->
    square_cps y $ \y_squared ->
    add_cps x_squared y_squared $ k
    
chainCPS :: ((a -> r) -> r) -> (a -> ((b -> r) -> r)) -> ((b -> r) -> r)
chainCPS s f = \k -> s $ \x -> f x $ k

pythagoras_cps' :: Int -> Int -> ((Int -> r) -> r)
pythagoras_cps' x y =
    square_cps x `chainCPS` \x_squared ->
    square_cps y `chainCPS` \y_squared ->
    add_cps x_squared y_squared

main = do
    print $ pythagoras 3 4
    pythagoras_cps 3 4 print
    pythagoras_cps' 3 4 print

{-
25
25
25
-}
```

这里 add, square, pythagoras 函数把处理结果直接返回，是直接编码风格。

而 add_cps, square_cps, pythagoras_cps 函数把处理结果传递给下一个函数，是CPS编码风格。

chainCPS 函数实现了一个通用的CPS风格函数，它能将CPS编码风格的函数串联起来。

### Samples

```
module Continuation where

idCPS :: a -> (a -> r) -> r
idCPS a ret = ret a

mysqrt :: Floating a => a -> a
mysqrt x = sqrt x

test1 :: IO ()
test1 = print (mysqrt 4)

mysqrtCPS :: Floating a => a -> (a -> r) -> r
mysqrtCPS a k = k (sqrt a)

test2 :: IO ()
test2 = mysqrtCPS 4 print

test3 :: Floating a => a
test3 = mysqrt 4 + 2

test4 :: Floating a => a
test4= mysqrtCPS 4 (+ 2)

fac :: Integral a => a -> a
fac 0 = 1
fac n = n * fac (n - 1)

test5 :: Integral a => a
test5 = fac 4 + 2

facCPS :: Integral a => a -> (a -> r) -> r
facCPS 0 k = k 1
facCPS n k = facCPS (n - 1) $ \ret -> k (n * ret)

test6 :: Integral a => a
test6 = facCPS 4 (+ 2)

test7 :: IO ()
test7 = facCPS 4 print

newSentence :: Char -> Bool
newSentence = flip elem ".?!"

newSentenceCPS :: Char -> (Bool -> r) -> r
newSentenceCPS c k = k (elem c ".?!")

mylength :: [a] -> Integer
mylength [] = 0
mylength (_ : as) = succ (mylength as)

mylengthCPS :: [a] -> (Integer -> r) -> r
mylengthCPS [] k = k 0
mylengthCPS (_ : as) k = mylengthCPS as (k . succ)

test8 :: Integer
test8 = mylengthCPS [1..2006] id

test9 :: IO ()
test9 = mylengthCPS [1..2006] print

main = do
    test1
    test2
    print test3
    print test4
    print test5
    print test6
    test7
    print test8
    test9

{-
2.0
2.0
4.0
4.0
26
26
24
2006
2006
-}
```


