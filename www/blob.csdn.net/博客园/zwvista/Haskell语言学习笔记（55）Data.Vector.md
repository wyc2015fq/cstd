# Haskell语言学习笔记（55）Data.Vector - zwvista - 博客园

## [Haskell语言学习笔记（55）Data.Vector](https://www.cnblogs.com/zwvista/p/7954505.html)

### Data.Vector

### Construction

```
Prelude V> import Data.Vector as V
Prelude V> V.empty
[]
Prelude V> V.singleton 1
[1]
Prelude V> V.replicate 5 1
[1,1,1,1,1]
Prelude V> generate 5 (+1)
[1,2,3,4,5]
Prelude V> iterateN 5 (*2) 1
[1,2,4,8,16]
```


