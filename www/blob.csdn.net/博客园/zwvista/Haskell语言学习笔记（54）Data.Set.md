# Haskell语言学习笔记（54）Data.Set - zwvista - 博客园

## [Haskell语言学习笔记（54）Data.Set](https://www.cnblogs.com/zwvista/p/7931782.html)

### Data.Set

```
Prelude> import Data.Set as Set
Prelude Set> :set -XOverloadedLists
```

### Construction

```
Prelude Set> empty
fromList []
Prelude Set> singleton 5
fromList [5]
Prelude Set> insert 3 [4,5]
fromList [3,4,5]
Prelude Set> delete 5 [3,4,5]
fromList [3,4]
```

### Operators

```
Prelude Set> [3,4,5] \\ [4,5,6]
fromList [3]
```

### Query

```
Prelude Set> Set.null $ []
True
Prelude Set> size [1,2,3]
3
Prelude Set> member 3 [1,2,3]
True
Prelude Set> lookupLT 5 [3, 5]
Just 3
Prelude Set> lookupGE 3 [3, 5]
Just 3
Prelude Set> isSubsetOf [3] [3,5]
True
```

### Combine

```
Prelude Set> union [3,4,5] [4,5,6]
fromList [3,4,5,6]
Prelude Set> difference [3,4,5] [4,5,6]
fromList [3]
Prelude Set> intersection [3,4,5] [4,5,6]
fromList [4,5]
```

### Filter

```
Prelude Set> Set.filter even [1,2,3,4]
fromList [2,4]
Prelude Set> partition even [1,2,3,4]
[2,4],fromList [1,3]
Prelude Set> split 3 [1,2,3,4,5]
[1,2],fromList [4,5]
Prelude Set> splitMember 3 [1,2,3,4,5]
[1,2],True,fromList [4,5]
```

### Indexed

```
Prelude Set> lookupIndex 3 [5,3]
Just 0
Prelude Set> findIndex 3 [5,3]
0
Prelude Set> elemAt 1 [5,3]
5
Prelude Set> deleteAt 1    [5,3]
fromList [3]
Prelude Set> Set.take 2 [1..6]
fromList [1,2]
Prelude Set> Set.drop 2 [1..6]
fromList [3,4,5,6]
Prelude Set> Set.splitAt 2 [1..6]
[1,2],fromList [3,4,5,6]
```

### Map

```
Prelude Set> Set.map (+2) [1..6]
fromList [3,4,5,6,7,8]
```

### Folds

```
Prelude Set> Set.foldr (+) 0 [1..6]
21
Prelude Set> Set.foldl' (+) 0 [1..6]
21
```

### Min/Max

```
Prelude Set> lookupMin [1..6]
Just 1
Prelude Set> findMax [1..6]
6
Prelude Set> deleteMin [1..6]
fromList [2,3,4,5,6]
Prelude Set> deleteFindMax [1..6]
(6,fromList [1,2,3,4,5])
Prelude Set> maxView [1..6]
Just (6,fromList [1,2,3,4,5])
Prelude Set> minView [1..6]
Just (1,fromList [2,3,4,5,6])
```

### Conversion

```
Prelude Set> toList [1..6]
[1,2,3,4,5,6]
Prelude Set> toAscList [1,5,3]
[1,3,5]
Prelude Set> toDescList [1,5,3]
[5,3,1]
```

### Debugging

```
Prelude Set> putStrLn $ showTree [1..6]
4
+--2
|  +--1
|  +--3
+--5
   +--|
   +--6

Prelude Set> putStrLn $ showTreeWith True True [1..6]
4
|
+--2
|  |
|  +--1
|  |
|  +--3
|
+--5
   |
   +--|
   |
   +--6
```


