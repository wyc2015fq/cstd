# Haskell语言学习笔记（53）Data.Sequence - zwvista - 博客园

## [Haskell语言学习笔记（53）Data.Sequence](https://www.cnblogs.com/zwvista/p/7931549.html)

### Data.Sequence

```
Prelude> import Data.Sequence as Seq
Prelude Seq> :set -XOverloadedLists
Prelude Seq>
```

### Construction

```
Prelude Seq> empty
fromList []
Prelude Seq> singleton 3
fromList [3]
Prelude Seq> 3 <| [4,5]
fromList [3,4,5]
Prelude Seq> [4,5] |> 6
fromList [4,5,6]
Prelude Seq> [3,4] >< [5,6]
fromList [3,4,5,6]
```

### Repetition

```
Prelude Seq> Seq.replicate 3 3
fromList [3,3,3]
Prelude Seq> Seq.replicateA 3 (Just 3)
Just [3,3,3]
Prelude Seq> Seq.replicateM 3 (Just 3)
Just [3,3,3]
Prelude Seq> cycleTaking 9 [3,4,5]
fromList [3,4,5,3,4,5,3,4,5]
```

### Iterative construction

```
Prelude Seq> iterateN 5 (+1) 1
fromList [1,2,3,4,5]
Prelude Seq> unfoldr (\b -> if b < 10 then Just (b, b+1) else Nothing) 1
fromList [1,2,3,4,5,6,7,8,9]
```

### Queries

```
Prelude Seq> Seq.null []
True
Prelude Seq> Seq.length [1,2,3]
3
```

### Views

```
Prelude Seq> viewl [1,2,3]
1 :< fromList [2,3]
Prelude Seq> viewr [1,2,3]
fromList [1,2] :> 3
```

### Scans

```
Prelude Seq> Seq.scanl (+) 0 [1,2,3]
fromList [0,1,3,6]
Prelude Seq> Seq.scanl1 (+) [1,2,3]
fromList [1,3,6]
Prelude Seq> Seq.scanr (+) 0 [1,2,3]
fromList [6,5,3,0]
Prelude Seq> Seq.scanr1 (+) [1,2,3]
fromList [6,5,3]
```

### Sublists

```
Prelude Seq> Seq.tails [1..6]
fromList [fromList [1,2,3,4,5,6],fromList [2,3,4,5,6],fromList [3,4,5,6],fromList [4,5,6],fromList [5,6],fromList [6],fromList []]
Prelude Seq> Seq.inits [1..6]
fromList [fromList [],fromList [1],fromList [1,2],fromList [1,2,3],fromList [1,2,3,4],fromList [1,2,3,4,5],fromList [1,2,3,4,5,6]]
Prelude Seq> chunksOf 2 [1..6]
fromList [fromList [1,2],fromList [3,4],fromList [5,6]]
Prelude Seq> takeWhileL even [2,4,5,6,8]
fromList [2,4]
Prelude Seq> takeWhileR even [2,4,5,6,8]
fromList [6,8]
Prelude Seq> dropWhileL even [2,4,5,6,8]
fromList [5,6,8]
Prelude Seq> dropWhileR even [2,4,5,6,8]
fromList [2,4,5]
Prelude Seq> spanl even [2,4,5,6,8]
[2,4],fromList [5,6,8]
Prelude Seq> spanr even [2,4,5,6,8]
[6,8],fromList [2,4,5]
Prelude Seq> breakl even [2,4,5,6,8]
[],fromList [2,4,5,6,8]
Prelude Seq> breakr even [2,4,5,6,8]
[],fromList [2,4,5,6,8]
Prelude Seq> Seq.partition even [2,4,5,6,8]
[2,4,6,8],fromList [5]
Prelude Seq> Seq.filter even [2,4,5,6,8]
fromList [2,4,6,8]
```

### Sorting

```
Prelude Seq Data.Function> Seq.sort ["one","two","three","four"]
fromList ["four","one","three","two"]
Prelude Seq Data.Function> Seq.sortBy (compare `on` Prelude.length) ["one","two","three","four"]
fromList ["one","two","four","three"]
Prelude Seq Data.Function> unstableSort ["one","two","three","four"]
fromList ["four","one","three","two"]
Prelude Seq Data.Function> unstableSortBy (compare `on` Prelude.length) ["one","two","three","four"]
fromList ["one","two","four","three"]
```

### Indexing

```
Prelude Seq> Seq.lookup 3 [1..6]
Just 4
Prelude Seq> [1..6] !? 3
Just 4
Prelude Seq> [1..6] `index` 3
4
Prelude Seq> adjust (+1) 3 [1..6]
fromList [1,2,3,5,5,6]
Prelude Seq> update 3 1 [1..6]
fromList [1,2,3,1,5,6]
Prelude Seq> Seq.take 3 [1..6]
fromList [1,2,3]
Prelude Seq> Seq.drop 3 [1..6]
fromList [4,5,6]
Prelude Seq> insertAt 3 3 [1..6]
fromList [1,2,3,3,4,5,6]
Prelude Seq> deleteAt 3 [1..6]
fromList [1,2,3,5,6]
Prelude Seq> Seq.splitAt 3 [1..6]
[1,2,3],fromList [4,5,6]
Prelude Seq> elemIndexL 3 [1,2,3,3,3,4,5]
Just 2
Prelude Seq> elemIndexL 3 [1,2,3,3,6,3,3,4,5]
Just 2
Prelude Seq> elemIndicesL 3 [1,2,3,3,6,3,3,4,5]
[2,3,5,6]
Prelude Seq> elemIndexR 3 [1,2,3,3,6,3,3,4,5]
Just 6
Prelude Seq> elemIndicesR 3 [1,2,3,3,6,3,3,4,5]
[6,5,3,2]
Prelude Seq> findIndexL even [1,2,3,3,6,3,3,4,5]
Just 1
Prelude Seq> findIndicesL even [1,2,3,3,6,3,3,4,5]
[1,4,7]
Prelude Seq> findIndexR even [1,2,3,3,6,3,3,4,5]
Just 7
Prelude Seq> findIndicesR even [1,2,3,3,6,3,3,4,5]
[7,4,1]
```

### Folds

```
Prelude Seq> foldMapWithIndex (\n a -> fmap (+n) a) [[1],[2]]
[1,3]
Prelude Seq> foldlWithIndex (\a b c -> a+b+c) 0 [1..3]
9
Prelude Seq> foldrWithIndex (\a b c -> a+b+c) 0 [1..3]
9
```

### Transformations

```
Prelude Seq> mapWithIndex (+) [1..3]
fromList [1,3,5]
Prelude Seq> traverseWithIndex (\a b -> [b+a,b+a+1]) [1..3]
[fromList [1,3,5],fromList [1,3,6],fromList [1,4,5],fromList [1,4,6],fromList [2,3,5],fromList [2,3,6],fromList [2,4,5],fromList [2,4,6]]
Prelude Seq> Seq.reverse [1..4]
fromList [4,3,2,1]
Prelude Seq> Seq.intersperse 9 [1..3]
fromList [1,9,2,9,3]
```

### Zips

```
Prelude Seq> Seq.zip [1..3] [1..3]
fromList [(1,1),(2,2),(3,3)]
Prelude Seq> Seq.zipWith (*) [1..3] [1..3]
fromList [1,4,9]
```


