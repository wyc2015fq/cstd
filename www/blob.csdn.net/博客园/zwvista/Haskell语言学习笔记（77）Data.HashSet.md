# Haskell语言学习笔记（77）Data.HashSet - zwvista - 博客园

## [Haskell语言学习笔记（77）Data.HashSet](https://www.cnblogs.com/zwvista/p/9256626.html)

### 安装 unordered-containers

```
$ cabal install unordered-containers
Installed unordered-containers-0.2.9.0
Prelude> import Data.HashSet as HashSet
Prelude HashSet> :set -XOverloadedLists
Prelude HashSet>
```

### Construction

```
Prelude HashSet> empty
fromList []
Prelude HashSet> singleton "a"
fromList ["a"]
```

### Combine

```
Prelude HashSet> union [3,4,5] [4,5,6]
fromList [3,4,5,6]
Prelude HashSet> unions [[3,4,5],[4,5,6]]
fromList [3,4,5,6]
```

### Basic interface

```
Prelude HashSet> HashSet.null empty
True
Prelude HashSet> HashSet.null [3]
False
Prelude HashSet> size [3,5,7]
3
Prelude HashSet> member 3 [3,5]
True
Prelude HashSet> insert 3 [5,7]
fromList [3,5,7]
Prelude HashSet> delete 3 [3,5,7]
fromList [5,7]
```

### Transformations

```
Prelude HashSet> HashSet.map (+2) [1..6]
fromList [3,4,5,6,7,8]
```

### Difference and intersection

```
Prelude HashSet> difference [3,4,5] [4,5,6]
fromList [3]
Prelude HashSet> intersection [3,4,5] [4,5,6]
fromList [4,5]
```

### Folds

```
Prelude HashSet> HashSet.foldl' (+) 0 [1..6]
21
Prelude HashSet> HashSet.foldr (+) 0 [1..6]
21
```

### Lists

```
Prelude HashSet> toList [1..6]
[1,2,3,4,5,6]
Prelude HashSet> fromList [1..6]
fromList [1,2,3,4,5,6]
```

### Maps

```
Prelude HashSet> import Data.HashMap.Lazy as HashMap
Prelude HashSet HashMap> toMap [3,4,5]
fromList [(3,()),(4,()),(5,())]
Prelude HashSet HashMap> fromMap [(3,()),(4,()),(5,())]
fromList [3,4,5]
```


