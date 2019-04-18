# Haskell语言学习笔记（36）Data.List.Zipper - zwvista - 博客园

## [Haskell语言学习笔记（36）Data.List.Zipper](https://www.cnblogs.com/zwvista/p/7802046.html)

### ListZipper 模块

```
$ cabal install ListZipper
Installed ListZipper-1.2.0.2
Prelude> :m +Data.List.Zipper
Prelude Data.List.Zipper>
```

### Zipper 和 ListZipper

Zipper 是一种带焦点（focus，也称游标, cursor）的数据结构。使用 Zipper 可以很方便的在底层数据结构中进行遍历，更新，插入，删除等操作。

ListZipper 是建立在 List 这种数据结构上的 Zipper，可以理解为带焦点（游标）的List。

### ListZipper 函数示例

```
Prelude Data.List.Zipper> z = fromList [3,4,5]
Prelude Data.List.Zipper> z
Zip [] [3,4,5]
Prelude Data.List.Zipper> replace 6 z
Zip [] [6,4,5]
Prelude Data.List.Zipper> toList z
[3,4,5]
Prelude Data.List.Zipper> emptyp z
False
Prelude Data.List.Zipper> beginp z
True
Prelude Data.List.Zipper> end z
Zip [5,4,3] []
Prelude Data.List.Zipper> cursor z
3
Prelude Data.List.Zipper Data.Function> z & right & right
Zip [4,3] [5]
Prelude Data.List.Zipper Data.Function> z & insert 6
Zip [] [6,3,4,5]
Prelude Data.List.Zipper Data.Function> z & push 6
Zip [6] [3,4,5]
Prelude Data.List.Zipper Data.Function> z & right & pop
Zip [] [4,5]
Prelude Data.List.Zipper Data.Function> z & right & foldrz (\z b -> cursor z + b) 0
9
Prelude Data.List.Zipper Data.Function> z & right & foldlz (\b z -> cursor z + b) 0
9
Prelude Data.List.Zipper> extractz z
3
Prelude Data.List.Zipper> duplicatez z
Zip [] [Zip [] [3,4,5],Zip [3] [4,5],Zip [4,3] [5]]
Prelude Data.List.Zipper> extendz (\z -> show (cursor z)) z
Zip [] ["3","4","5"]
```

```
import Control.Monad.State
import Data.List.Zipper

f :: State (Zipper Int) [Int]
f = do
    modify right -- [3],[4,5]
    modify $ replace 6 -- [3],[6,5]
    modify $ insert 7 -- [3],[7,6,5]
    gets toList -- [3,7,6,5]

main = print . evalState f $ fromList [3,4,5]
```

### 理解 ListZipper

```
data Zipper a = Zip ![a] ![a] deriving (Eq,Show)

fromList :: [a] -> Zipper a
fromList as = Zip [] as

toList :: Zipper a -> [a]
toList (Zip ls rs) = reverse ls ++ rs

left :: Zipper a -> Zipper a
left  (Zip (a:ls) rs) = Zip ls (a:rs)
left  z               = z

right :: Zipper a -> Zipper a
right (Zip ls (a:rs)) = Zip (a:ls) rs
right z               = z

insert :: a -> Zipper a -> Zipper a
insert a (Zip ls rs) = Zip ls (a:rs)

delete :: Zipper a -> Zipper a
delete (Zip ls (_:rs)) = Zip ls rs
delete z               = z

push :: a -> Zipper a -> Zipper a
push   a (Zip ls rs) = Zip (a:ls) rs

pop :: Zipper a -> Zipper a
pop    (Zip (_:ls) rs) = Zip ls rs
pop    z               = z

replace :: a -> Zipper a -> Zipper a
replace a (Zip ls (_:rs)) = Zip ls (a:rs)
replace _ z               = z
```
- data Zipper a = Zip [a] [a] deriving (Eq,Show)

ListZipper 是建立在List 上的数据结构，它包含两个List，其中第二个List的第一个元素是当前的焦点。

也就是说左边的 List 包含焦点之前的元素，而右边的 List 则包含焦点及其之后的元素。
- fromList as = Zip [] as

fromList 将 List 参数 as 转换成 Zipper，当前焦点在as的第一个元素。
- toList (Zip ls rs) = reverse ls ++ rs

toList 将 Zipper 转换成 List。
- left (Zip (a:ls) rs) = Zip ls (a:rs)

left 函数将当前焦点左移一个元素。即左边 List 的第一个元素被移至右边 List 的最前端。
- right (Zip ls (a:rs)) = Zip (a:ls) rs

right 函数将当前焦点右移一个元素。即右边 List 的第一个元素被移至左边 List 的最前端。
- insert a (Zip ls rs) = Zip ls (a:rs)

insert 函数在当前焦点处插入新元素。即在右边 List 的最前端插入新元素。
- delete (Zip ls (_:rs)) = Zip ls rs

delete 函数移除处位于焦点处的元素。即右边 List 的第一个元素被移除。
- push a (Zip ls rs) = Zip (a:ls) rs

push 函数在当前焦点之前插入新元素。即在左边 List 的最前端插入新元素。
- pop (Zip (_:ls) rs) = Zip ls rs

pop 函数移除位于焦点之前的一个元素。即左边 List 的第一个元素被移除。
- replace a (Zip ls (_:rs)) = Zip ls (a:rs)

replace 函数替换当前焦点处的元素。即右边 List 的第一个元素被替换。


