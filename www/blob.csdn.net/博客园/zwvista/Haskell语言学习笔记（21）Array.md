# Haskell语言学习笔记（21）Array - zwvista - 博客园

## [Haskell语言学习笔记（21）Array](https://www.cnblogs.com/zwvista/p/7591421.html)

### Ix

数组下标类型

```
Prelude> :m +Data.Array
Prelude Data.Array> data Colour = Red | Orange | Yellow | Green | Blue | Indigo | Violet deriving (Show,Eq,Ord,Ix)
Prelude Data.Array> range (Yellow,Blue)
[Yellow,Green,Blue]
Prelude Data.Array> index (Yellow,Blue) Green
1
Prelude Data.Array> inRange (Yellow,Blue) Red
False
```

### Array UArray

数组类型，类型参数有两个，下标类型 i 和元素类型 e。

Array是装箱数组类型（元素为指针）

UArray是非装箱数组类型（元素为实体）

```
Prelude> :m +Data.Array
Prelude Data.Array> a = array (1,10) ((1,1) : [(i, i + a!(i-1)) | i <- [2..10]]) 
Prelude Data.Array> :t a
a :: (Ix e, Num e, Enum e) => Array e e
Prelude Data.Array> a!10
55
Prelude Data.Array> bounds a
(1,10)
Prelude Data.Array> indices a
[1,2,3,4,5,6,7,8,9,10]
Prelude Data.Array> elems a
[1,3,6,10,15,21,28,36,45,55]
Prelude Data.Array> assocs a
[(1,1),(2,3),(3,6),(4,10),(5,15),(6,21),(7,28),(8,36),(9,45),(10,55)]
Prelude Data.Array> array ((1,1),(2,2)) [((2,1),"C"),((1,2),"B"),((1,1),"A"),((2,2),"D")] ! (2,2)
"D"
Prelude Data.Array> array ('a','c') [('a',"AAA"),('b',"BBB"),('c',"CCC")] ! 'b'
"BBB"
```

```
Prelude> :m +Data.Array.Unboxed
Prelude Data.Array.Unboxed> a = listArray (1,10) [1..10] :: UArray Int Int
Prelude Data.Array.Unboxed> a
array (1,10) [(1,1),(2,2),(3,3),(4,4),(5,5),(6,6),(7,7),(8,8),(9,9),(10,10)]
Prelude Data.Array.Unboxed> a // [(1,100),(2,99)]
array (1,10) [(1,100),(2,99),(3,3),(4,4),(5,5),(6,6),(7,7),(8,8),(9,9),(10,10)]
```

### IOArray IOUArray

在IO monad中使用的数组。
|函数|参数|功能|
|----|----|----|
|newArray|区间和初值|新建带初值的数组|
|readArray|数组和下标|读取数组某个元素的值|
|writeArray|数组,下标和值|设置数组某个元素的值|
|getElems|数组|读取数组所有元素的值|

```
Prelude> :m +Data.Array.IO
Prelude Data.Array.IO> a <- newArray (0, 2) 0 :: IO (IOUArray Int Int)
Prelude Data.Array.IO> getElems a
[0,0,0]
Prelude Data.Array.IO> writeArray a 0 3
Prelude Data.Array.IO> readArray a 0
3
Prelude Data.Array.IO> writeArray a 1 6
Prelude Data.Array.IO> getElems a
[3,6,0]
Prelude Data.Array.IO> writeArray a 2 7
Prelude Data.Array.IO> getElems a
[3,6,7]
```

### STArray STUArray

在ST monad中使用的数组。

```
-- https://stackoverflow.com/questions/8197032/starray-documentation-for-newbies-and-state-st-related-questions

import Control.Monad
import Control.Monad.ST
import Data.Array.ST
import Data.Array.Unboxed

primesUpto :: Int -> [Int]
primesUpto n = [p | (p, True) <- assocs $ sieve n]

sieve :: Int -> UArray Int Bool
sieve n = runSTUArray $ do
    sieve <- newArray (2, n) True
    forM_ [2..n] $ \p -> do
        isPrime <- readArray sieve p
        when isPrime $ do
            forM_ [p*2, p*3 .. n] $ \k -> do
                writeArray sieve k False
    return sieve
    
main = print $ primesUpto 100
```


