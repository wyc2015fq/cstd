# Haskell语言学习笔记（65）Data.HashMap - zwvista - 博客园

## [Haskell语言学习笔记（65）Data.HashMap](https://www.cnblogs.com/zwvista/p/8046035.html)

### 安装 unordered-containers

```
$ cabal install unordered-containers
Installed unordered-containers-0.2.9.0
Prelude> import Data.HashMap.Lazy as HashMap
Prelude HashMap> :set -XOverloadedLists
Prelude HashMap>
```

### Construction

```
Prelude HashMap> empty
fromList []
Prelude HashMap> singleton "a" 1
fromList [("a",1)]
```

### Basic interface

```
Prelude HashMap> HashMap.null (singleton "a" 1)
False
Prelude HashMap> HashMap.null empty
True
Prelude HashMap> size [(1,'a'), (2,'c'), (3,'b')]
3
Prelude HashMap> member 5 [(5,'a'), (3,'b')]
True
Prelude HashMap> HashMap.lookup "John" [("John","Sales"), ("Bob","IT")]
Just "Sales"
Prelude HashMap> lookupDefault 'x' 1 [(5,'a'), (3,'b')]
'x'
Prelude HashMap> lookupDefault 'x' 5 [(5,'a'), (3,'b')]
'a'
Prelude HashMap> [(5,'a'), (3,'b')] ! 5
'a'
Prelude HashMap> insert 5 'x' [(5,'a'), (3,'b')]
fromList [(3,'b'),(5,'x')]
Prelude HashMap> insert 7 'x' [(5,'a'), (3,'b')]
fromList [(3,'b'),(5,'a'),(7,'x')]
Prelude HashMap> insert 5 'x' empty
fromList [(5,'x')]
Prelude HashMap> delete 5 [(5,"a"), (3,"b")]
fromList [(3,"b")]
Prelude HashMap> adjust ("new " ++) 5 [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"new a")]
Prelude HashMap> let f x = if x == "a" then Just "new a" else Nothing
Prelude HashMap> update f 5 [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"new a")]
Prelude HashMap> let f _ = Nothing
Prelude HashMap> alter f 5 [(5,"a"), (3,"b")]
fromList [(3,"b")]
Prelude HashMap> let f _ = Just "c"
Prelude HashMap> alter f 7 [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"a"),(7,"c")]
```

### Union

```
Prelude HashMap> union [(5, "a"), (3, "b")] [(5, "A"), (7, "C")]
fromList [(3,"b"),(5,"a"),(7,"C")]
Prelude HashMap> unions [[(5, "a"), (3, "b")], [(5, "A"), (7, "C")], [(5, "A3"), (3, "B3")]]
fromList [(3,"b"),(5,"a"),(7,"C")]
```

### Transformations

```
Prelude HashMap> HashMap.map (++ "x") [(5,"a"), (3,"b")]
fromList [(3,"bx"),(5,"ax")]
```

### Difference and intersection

```
Prelude HashMap> difference [(5, "a"), (3, "b")] [(5, "A"), (7, "C")]
fromList [(3,"b")]
Prelude HashMap> intersection [(5, "a"), (3, "b")] [(5, "A"), (7, "C")]
fromList [(5,"a")]
```

### Folds

```
Prelude HashMap> let f len a = len + (length a)
Prelude HashMap> HashMap.foldl' f 0 [(5,"a"), (3,"bbb")]
4
Prelude HashMap> let f a len = len + (length a)
Prelude HashMap> HashMap.foldr f 0 [(5,"a"), (3,"bbb")]
4
```

### Filter

```
Prelude HashMap> HashMap.filter (> "a") [(5,"a"), (3,"b")]
fromList [(3,"b")]
Prelude HashMap> let f x = if x == "a" then Just "new a" else Nothing
Prelude HashMap> mapMaybe f [(5,"a"), (3,"b")] 
fromList [(5,"new a")]
```

### Conversions

```
Prelude HashMap> keys [(5,"a"), (3,"b")]
[3,5]
Prelude HashMap> elems [(5,"a"), (3,"b")]
["b","a"]
```

### Lists

```
Prelude HashMap> toList [(5,"a"), (3,"b")]
[(3,"b"),(5,"a")]
Prelude HashMap> fromList [(5,"a"), (3,"b"), (5, "c")]
fromList [(3,"b"),(5,"c")]
```


