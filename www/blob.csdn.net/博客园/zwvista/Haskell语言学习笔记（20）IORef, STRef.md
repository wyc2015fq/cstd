# Haskell语言学习笔记（20）IORef, STRef - zwvista - 博客园

## [Haskell语言学习笔记（20）IORef, STRef](https://www.cnblogs.com/zwvista/p/7591262.html)

### IORef

一个在IO monad中使用变量的类型。
|函数|参数|功能|
|----|----|----|
|newIORef|值|新建带初值的引用|
|readIORef|引用|读取引用的值|
|writeIORef|引用和值|设置引用的值|
|modifyIORef|引用以及修改值的函数|修改引用的值|

```
Prelude> :m +Data.IORef
Prelude Data.IORef> a <- newIORef 1
Prelude Data.IORef> v <- readIORef a
Prelude Data.IORef> writeIORef a $ v + 1
Prelude Data.IORef> readIORef a
2
Prelude Data.IORef> modifyIORef a (+1)
Prelude Data.IORef> readIORef a
3
Prelude Data.IORef> a <- sequence [newIORef 0, newIORef 1, newIORef 2]
Prelude Data.IORef> readIORef (a !! 0)
0
```

### STRef

一个在ST monad中使用变量的类型。
|函数|参数|功能|
|----|----|----|
|newSTRef|值|新建带初值的引用|
|readSTRef|引用|读取引用的值|
|writeSTRef|引用和值|设置引用的值|
|modifySTRef|引用以及修改值的函数|修改引用的值|

### Counter

counter闭包

Javascript语言实现

```
function counter() {
    var c = 0;
    return function() {
        return ++c;
    };
}

var f = counter();
console.log(f());
console.log(f());
console.log(f());
```

Haskell语言实现（使用 IORef 和 STRef）

```
import Control.Monad.ST
import Data.STRef
import Data.IORef

counterIO :: IO (IO Int)
counterIO = do
    c <- newIORef 0
    return $ do
        modifyIORef c (+1)
        readIORef c

counterST :: ST s (ST s Int)
counterST = do
    c <- newSTRef 0
    return $ do
        modifySTRef c (+1)
        readSTRef c

main = do
    f <- counterIO
    print =<< f
    print =<< f
    print =<< f
    print $ runST $ do
        f2 <- counterST
        x <- f2
        y <- f2
        z <- f2
        return (x, y, z)
```

```
1
2
3
(1,2,3)
```


