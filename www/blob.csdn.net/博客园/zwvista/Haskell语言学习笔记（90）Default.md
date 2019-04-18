# Haskell语言学习笔记（90）Default - zwvista - 博客园

## [Haskell语言学习笔记（90）Default](https://www.cnblogs.com/zwvista/p/9441301.html)

### 安装 data-default-class

```
$ cabal install data-default-class
Installed data-default-class-0.1.2.0
Prelude> :m +Data.Default.Class
Prelude Data.Default.Class>
```

data-default-class 这个库为类型提供了缺省值。

```
Prelude Data.Default.Class> def :: Int
0
Prelude Data.Default.Class> def :: Double
0.0
Prelude Data.Default.Class> def :: (Int, Int)
(0,0)
Prelude Data.Default.Class> def :: Maybe Int
Nothing
```

### 示例

```
{-# LANGUAGE DeriveGeneric #-}
{-# LANGUAGE DeriveAnyClass #-}

import GHC.Generics
import Data.Default.Class

data A = A Int Double deriving (Show, Generic)
instance Default A
 
data B = B Int Double deriving (Show, Generic, Default)
 
main = do
    print (def :: A) -- A 0 0.0
    print (def :: B) -- B 0 0.0
```


