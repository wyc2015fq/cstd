# Haskell语言学习笔记（91）Comprehension Extensions - zwvista - 博客园

## [Haskell语言学习笔记（91）Comprehension Extensions](https://www.cnblogs.com/zwvista/p/9491535.html)

### Comprehension Extensions

关于解析式的相关语言扩展。

[List and Comprehension Extensions](https://www.schoolofhaskell.com/school/to-infinity-and-beyond/pick-of-the-week/guide-to-ghc-extensions/list-and-comprehension-extensions)
[24 Days of GHC Extensions: List Comprehensions](https://ocharles.org.uk/blog/guest-posts/2014-12-07-list-comprehensions.html)

### ParallelListComp

```
Prelude> [(w,x,y,z) | ((w,x),(y,z)) <- zip [(w,x) | w <- [1..3], x <- [2..4]] [(y,z) | y <- [3..5], z <- [4..6]]]
[(1,2,3,4),(1,3,3,5),(1,4,3,6),(2,2,4,4),(2,3,4,5),(2,4,4,6),(3,2,5,4),(3,3,5,5),(3,4,5,6)]
```

语言扩展 ParallelListComp 给解析式加上了 zipWith 函数的功能。

```
Prelude> :set -XParallelListComp
Prelude> [(w,x,y,z) | w <- [1..3], x <- [2..4] | y <- [3..5], z <- [4..6]]
[(1,2,3,4),(1,3,3,5),(1,4,3,6),(2,2,4,4),(2,3,4,5),(2,4,4,6),(3,2,5,4),(3,3,5,5),(3,4,5,6)]
```

### TransformListComp

[Generalised (SQL-Like) List Comprehensions](https://downloads.haskell.org/~ghc/7.8.3/docs/html/users_guide/syntax-extns.html#generalised-list-comprehensions)

```
Prelude> reverse [(x,y) | x <- [1..3], y <- [2..4]]
[(3,4),(3,3),(3,2),(2,4),(2,3),(2,2),(1,4),(1,3),(1,2)]
Prelude> :m +GHC.Exts
Prelude GHC.Exts> sortWith (\(x,y) -> x+y) [(x,y) | x <- [1..3], y <- [2..4]]
[(1,2),(1,3),(2,2),(1,4),(2,3),(3,2),(2,4),(3,3),(3,4)]
Prelude GHC.Exts> groupWith (\(x,y) -> x+y) [(x,y) | x <- [1..3], y <- [2..4]]
[[(1,2)],[(1,3),(2,2)],[(1,4),(2,3),(3,2)],[(2,4),(3,3)],[(3,4)]]
Prelude GHC.Exts> (,) <$> (\(x,y)->x+y) . head <*> id <$> groupWith (\(x,y) -> x+y) [(x,y) | x <- [1..3], y <- [2..4]]
[(3,[(1,2)]),(4,[(1,3),(2,2)]),(5,[(1,4),(2,3),(3,2)]),(6,[(2,4),(3,3)]),(7,[(3,4)])]
Prelude GHC.Exts> :m -GHC.Exts
Prelude> :m +Data.List
Prelude Data.List> inits [1..3]
[[],[1],[1,2],[1,2,3]]
```

语言扩展 TransformListComp 给解析式加上了4个关键字：then, group, by, using。

该语言扩展在功能上接近 SQL 语句中的 order by 和 group by。

```
Prelude> :set -XTransformListComp
Prelude> [(x,y) | x <- [1..3], y <- [2..4], then reverse]
[(3,4),(3,3),(3,2),(2,4),(2,3),(2,2),(1,4),(1,3),(1,2)]
Prelude> :m +GHC.Exts
Prelude GHC.Exts> [(x,y) | x <- [1..3], y <- [2..4], then sortWith by (x+y)]
[(1,2),(1,3),(2,2),(1,4),(2,3),(3,2),(2,4),(3,3),(3,4)]
Prelude GHC.Exts> [(x,y) | x <- [1..3], y <- [2..4], then group by (x+y) using groupWith]
[([1],[2]),([1,2],[3,2]),([1,2,3],[4,3,2]),([2,3],[4,3]),([3],[4])]
Prelude GHC.Exts> [zip x y | x <- [1..3], y <- [2..4], then group by (x+y) using groupWith]
[[(1,2)],[(1,3),(2,2)],[(1,4),(2,3),(3,2)],[(2,4),(3,3)],[(3,4)]]
Prelude GHC.Exts> [(zipWith (+) x y, zip x y) | x <- [1..3], y <- [2..4], then group by (x+y) using groupWith]
[([3],[(1,2)]),([4,4],[(1,3),(2,2)]),([5,5,5],[(1,4),(2,3),(3,2)]),([6,6],[(2,4),(3,3)]),([7],[(3,4)])]
Prelude GHC.Exts> [(the (zipWith (+) x y), zip x y) | x <- [1..3], y <- [2..4], then group by (x+y) using groupWith]
[(3,[(1,2)]),(4,[(1,3),(2,2)]),(5,[(1,4),(2,3),(3,2)]),(6,[(2,4),(3,3)]),(7,[(3,4)])]
Prelude GHC.Exts> [(z,x,y) | x <- [1..3], y <- [2..4], let z = x+y, then group by z using groupWith]
[([3],[1],[2]),([4,4],[1,2],[3,2]),([5,5,5],[1,2,3],[4,3,2]),([6,6],[2,3],[4,3]),([7],[3],[4])]
Prelude GHC.Exts> [(the z, zip x y) | x <- [1..3], y <- [2..4], let z = x+y, then group by z using groupWith]
[(3,[(1,2)]),(4,[(1,3),(2,2)]),(5,[(1,4),(2,3),(3,2)]),(6,[(2,4),(3,3)]),(7,[(3,4)])]
Prelude GHC.Exts> :m +Data.List
Prelude GHC.Exts Data.List> [x | x <- [1..3], then group using inits]
[[],[1],[1,2],[1,2,3]]
```

### MonadComprehensions

```
{-# LANGUAGE TransformListComp, MonadComprehensions #-}

l :: [(String, Int)]
l = [("a", 1), ("b", 2), ("c", 3)]

main = print [ (x, y) | x <- lookup "a" l,
                        y <- lookup "b" l,
                        then (\f ->
                               maybe Nothing
                                     (\x -> if f x == 2
                                               then Just x
                                               else Nothing))
                             by (x * y) ] -- Just (1,2)
```


