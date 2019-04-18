# Haskell语言学习笔记（76）Data.Tree - zwvista - 博客园

## [Haskell语言学习笔记（76）Data.Tree](https://www.cnblogs.com/zwvista/p/9248182.html)

### Data.Tree

```
data Tree a = Node {
        rootLabel :: a,
        subForest :: Forest a
    } deriving (Eq, Read, Show)
type Forest a = [Tree a]
```

Data.Tree 是一种非空（存在根节点），可以有无限分支，每个节点均可有多路分支的Tree类型。

```
Prelude Data.Tree> :t Node 1
Node 1 :: Num a => Forest a -> Tree a
Prelude Data.Tree> a = Node 1 [Node 2 [], Node 3 []]
Prelude Data.Tree> a
Node {rootLabel = 1, subForest = [Node {rootLabel = 2, subForest = []},Node {rootLabel = 3, subForest = []}]}
Prelude Data.Tree> putStr $ drawTree $ fmap show a
1
|
+- 2
|
`- 3
Prelude Data.Tree> foldTree (\x xs -> sum (x:xs)) a
6
Prelude Data.Tree> foldTree (\x xs -> maximum (x:xs)) a
3
Prelude Data.Tree> tree = Node "A" [Node "B" [], Node "C" [Node "D" [], Node "E" []]]
Prelude Data.Tree> print tree
Node {rootLabel = "A", subForest = [Node {rootLabel = "B", subForest = []},Node {rootLabel = "C", subForest = [Node {rootLabel = "D", subForest = []},Node {rootLabel = "E", subForest = []}]}]}
Prelude Data.Tree> putStr $ drawTree tree
A
|
+- B
|
`- C
   |
   +- D
   |
   `- E
Prelude Data.Tree> putStr $ drawForest $ subForest tree
B

C
|
+- D
|
`- E

Prelude Data.Tree> flatten tree
["A","B","C","D","E"]
Prelude Data.Tree> levels tree
[["A"],["B","C"],["D","E"]]
```

### 实例


