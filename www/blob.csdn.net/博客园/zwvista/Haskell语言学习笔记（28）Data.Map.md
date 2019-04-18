# Haskell语言学习笔记（28）Data.Map - zwvista - 博客园

## [Haskell语言学习笔记（28）Data.Map](https://www.cnblogs.com/zwvista/p/7679450.html)

### Map

```
Prelude> import Data.Map as Map
Prelude Map> :set -XOverloadedLists
Prelude Map>
```

### OverloadedLists

GHC 提供了语言扩展 OverloadedLists。
- 不使用这个语言扩展，所有的列表字面量都属于 [] 类型。
- 如果使用这个语言扩展，那么所有的列表字面量就都属于 `IsList l => l` 类型。
- Map, Set, Vector, Text, Array 都是 IsList 类的实例。

### Operators

```
Prelude Map> [(5,'a'), (3,'b')] ! 5
'a'
Prelude Map> [(5, 'a'), (3, 'b')] !? 1 
Nothing
Prelude Map> [(5, 'a'), (3, 'b')] !? 5
Just 'a'
```

### Query

```
Prelude Map> Map.null (singleton 1 'a')
False
Prelude Map> size [(1,'a'), (2,'c'), (3,'b')]
3
Prelude Map> member 5 [(5,'a'), (3,'b')]
True
Prelude Map> Map.lookup "John" [("John","Sales"), ("Bob","IT")]
Just "Sales"
Prelude Map> findWithDefault 'x' 1 [(5,'a'), (3,'b')]
'x'
Prelude Map> findWithDefault 'x' 5 [(5,'a'), (3,'b')]
'a'
```

### Construction

```
Prelude Map> singleton 1 'a'
fromList [(1,'a')]
Prelude Map> empty
fromList []
```

### Insertion

```
Prelude Map> insert 5 'x' [(5,'a'), (3,'b')]
fromList [(3,'b'),(5,'x')]
Prelude Map> insert 7 'x' [(5,'a'), (3,'b')]
fromList [(3,'b'),(5,'a'),(7,'x')]
Prelude Map> insert 5 'x' empty
fromList [(5,'x')]
```

### Delete/Update

```
Prelude Map> delete 5 [(5,"a"), (3,"b")]
fromList [(3,"b")]
Prelude Map> adjust ("new " ++) 5 [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"new a")]
Prelude Map> let f x = if x == "a" then Just "new a" else Nothing
Prelude Map> update f 5 [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"new a")]
Prelude Map> let f _ = Nothing
Prelude Map> alter f 5 [(5,"a"), (3,"b")]
fromList [(3,"b")]
Prelude Map> let f _ = Just "c"
Prelude Map> alter f 7 [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"a"),(7,"c")]
```

### Combine

```
Prelude Map> union [(5, "a"), (3, "b")] [(5, "A"), (7, "C")]
fromList [(3,"b"),(5,"a"),(7,"C")]
Prelude Map> unions [[(5, "a"), (3, "b")], [(5, "A"), (7, "C")], [(5, "A3"), (3, "B3")]]
fromList [(3,"b"),(5,"a"),(7,"C")]
```

### Difference

```
Prelude Map> difference [(5, "a"), (3, "b")] [(5, "A"), (7, "C")]
fromList [(3,"b")]
```

### Intersection

```
Prelude Map> intersection [(5, "a"), (3, "b")] [(5, "A"), (7, "C")]
fromList [(5,"a")]
```

### Traversal

```
Prelude Map> Map.map (++ "x") [(5,"a"), (3,"b")]
fromList [(3,"bx"),(5,"ax")]
Prelude Map> traverseWithKey (\k v -> if odd k then Just (succ v) else Nothing) [(1, 'a'), (5, 'e')]
Just [(1,'b'),(5,'f')]
Prelude Map> let f a b = (a ++ b, b ++ "X")
Prelude Map> mapAccum f "Everything: " [(5,"a"), (3,"b")]
("Everything: ba",fromList [(3,"bX"),(5,"aX")])
Prelude Map> mapKeys (+ 1) [(5,"a"), (3,"b")]
fromList [(4,"b"),(6,"a")]
```

### Folds

```
Prelude Map> let f a len = len + (length a)
Prelude Map> Map.foldr f 0 [(5,"a"), (3,"bbb")]
4
Prelude Map> let f len a = len + (length a)
Prelude Map> Map.foldl f 0 [(5,"a"), (3,"bbb")]
4
```

### Conversion

```
Prelude Map> elems [(5,"a"), (3,"b")]
["b","a"]
Prelude Map> keys [(5,"a"), (3,"b")]
[3,5]
Prelude Map> assocs [(5,"a"), (3,"b")]
[(3,"b"),(5,"a")]
```

### Lists

```
Prelude Map> toList [(5,"a"), (3,"b")]
[(3,"b"),(5,"a")]
Prelude Map> fromList [(5,"a"), (3,"b"), (5, "c")]
fromList [(3,"b"),(5,"c")]
Prelude Map> fromListWith (++) [(5,"a"), (5,"b"), (3,"b"), (3,"a"), (5,"a")]
fromList [(3,"ab"),(5,"aba")]
```

### Ordered lists

```
Prelude Map> toAscList [(5,"a"), (3,"b")]
[(3,"b"),(5,"a")]
Prelude Map> toDescList [(5,"a"), (3,"b")]
[(5,"a"),(3,"b")]
Prelude Map> fromAscList [(3,"b"), (5,"a")]
fromList [(3,"b"),(5,"a")]
Prelude Map> fromDescList [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"a")]
```

### Filter

```
Prelude Map> Map.filter (> "a") [(5,"a"), (3,"b")]
fromList [(3,"b")]
Prelude Map> partition (> "a") [(5,"a"), (3,"b")]
[(3,"b")],fromList [(5,"a")]
Prelude Map> let f x = if x == "a" then Just "new a" else Nothing
Prelude Map> mapMaybe f [(5,"a"), (3,"b")] 
fromList [(5,"new a")]
Prelude Map> split 4 [(5,"a"), (3,"b")]
[(3,"b")],fromList [(5,"a")]
```

### Submap

```
Prelude Map> isSubmapOfBy (==) [('a',1)] [('a',1),('b',2)]
True
Prelude Map> isSubmapOfBy (<=) [('a',1)] [('a',1),('b',2)]
True
Prelude Map> isSubmapOfBy (<)  [('a',1)] [('a',1),('b',2)]
False
```

### Indexed

```
Prelude Map> lookupIndex 2 [(5,"a"), (3,"b")]
Nothing
Prelude Map> lookupIndex 3 [(5,"a"), (3,"b")]
Just 0
Prelude Map> findIndex 3 [(5,"a"), (3,"b")]
0
Prelude Map> elemAt 0 [(5,"a"), (3,"b")]
(3,"b")
Prelude Map> elemAt 1 [(5,"a"), (3,"b")]
(5,"a")
Prelude Map> updateAt (\ _ _ -> Just "x") 0    [(5,"a"), (3,"b")]
fromList [(3,"x"),(5,"a")]
Prelude Map> deleteAt 0  [(5,"a"), (3,"b")]
fromList [(5,"a")]
```

### Min/Max

```
Prelude Map> lookupMin [(5,"a"), (3,"b")]
Just (3,"b")
Prelude Map> lookupMax [(5,"a"), (3,"b")]
Just (5,"a")
Prelude Map> findMin [(5,"a"), (3,"b")]
(3,"b")
Prelude Map> findMax [(5,"a"), (3,"b")]
(5,"a")
Prelude Map> deleteMin [(5,"a"), (3,"b"), (7,"c")]
fromList [(5,"a"),(7,"c")]
Prelude Map> updateMax (\ a -> Just ("X" ++ a)) [(5,"a"), (3,"b")]
fromList [(3,"b"),(5,"Xa")]
```

### Debugging

```
Prelude Map> valid (fromAscList [(3,"b"), (5,"a")])
True
Prelude Map> valid (fromAscList [(5,"a"), (3,"b")])
False
```


