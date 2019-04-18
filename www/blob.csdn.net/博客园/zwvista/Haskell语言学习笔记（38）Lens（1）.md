# Haskell语言学习笔记（38）Lens（1） - zwvista - 博客园

## [Haskell语言学习笔记（38）Lens（1）](https://www.cnblogs.com/zwvista/p/7811898.html)

### Lens

Lens是一个接近语言级别的库，使用它可以方便的读取，设置，修改一个大的数据结构中某一部分的值。

### view, over, set

```
Prelude> :m +Control.Lens
Prelude Control.Lens> view _1 ("abc", "def")
"abc"
Prelude Control.Lens> over _1 (++ "!!!") ("abc", "def")
("abc!!!","def")
Prelude Control.Lens> set _1 "!!!" ("abc", "def")
("!!!","def")
Prelude Control.Lens> view _2 ("abc", "def")
"def"
```

这里 _1, _2 则相当于元组的属性名，在 Lens 库中被称为 lens。

view 是一个 Getting，相当于Java语言中用来读取属性的 .getXXX()。

over 以及 set 是一个 Setting，相当于Java语言中用来设置属性的 .setXXX()。

view l s 读取数据结构 s 中字段 l' 的值。

set l s 设置数据结构 s 中字段 l' 的值。

而 over l f s 则通过调用函数 f 修改数据结构 s 中字段 l' 的值。

### view, over, set的操作符版本

```
Prelude> :m +Control.Lens
Prelude Control.Lens> ("abc", "def") ^. _1 
"abc"
Prelude Control.Lens> ("abc", "def") & _1 %~ (++ "!!!") 
("abc!!!","def")
Prelude Control.Lens> ("abc", "def") & _1 .~ "!!!" 
("!!!","def")
Prelude Control.Lens> ("abc", "def") ^. _2
"def"
Prelude Control.Lens> _2 %~ (++ "***") $ ("abc", "def") 
("abc","def***")
Prelude Control.Lens> _2 .~ "***" $ ("abc", "def") 
("abc","***")
```

view l s ≡ s ^. l

set l v s ≡ l .~ v $ s ≡ s & l .~ v

over l f s ≡ l %~ f $ s ≡ s & l %~ f

### view, over, set 的 State 版本

```
Prelude Control.Lens Control.Monad.State> evalState (use _1) ("abc","def")
"abc"
Prelude Control.Lens Control.Monad.State> execState (do _1 .= "!!!"; _2 .= "***") ("abc", "def")
("!!!","***")
Prelude Control.Lens Control.Monad.State> execState (do _1 %= (++ "!!!"); _2 %= (++ "***")) ("abc", "def")
("abc!!!","def***")
```

### preview, review

```
Prelude Control.Lens> view _Left (Left "abc")
"abc"
Prelude Control.Lens> view _Right (Right "abc")
"abc"
Prelude Control.Lens> view _Just (Just "abc")
"abc"
Prelude Control.Lens> preview _Left (Left "abc")
Just "abc"
Prelude Control.Lens> review _Left "abc"
Left "abc"
```
- preview 和 review 函数处理 Either 这样的和类型

preview 函数向上走一个分支。

review 函数向下走一个分支。

### preview, review 的操作符版本

```
Prelude Control.Lens> Left "abc" ^?! _Left
"abc"
Prelude Control.Lens> Left "abc" ^? _Left
Just "abc"
Prelude Control.Lens> Right "abc" ^? _Left
Nothing
Prelude Control.Lens> Right "abc" ^? _Right
Just "abc"
Prelude Control.Lens> _Left # "abc"
Left "abc"
```

preview l x ≡ x ^? l

review l x ≡ l # x

### toListOf 及其操作符版本

```
Prelude Control.Lens> toListOf traverse [1,2,3]
[1,2,3]
Prelude Control.Lens> toListOf (traverse.traverse) [[1,2],[3]]
[1,2,3]
Prelude Control.Lens> toListOf both (1,2)
[1,2]
Prelude Control.Lens> toListOf _1 (4, 1)
[4]
Prelude Control.Lens> [[1,2],[3]] ^..traverse.traverse
[1,2,3]
Prelude Control.Lens> (1,2) ^..both
[1,2]
Prelude Control.Lens> (4, 1) ^.._2
[1]
```

### to

```
Prelude Control.Lens> view (_1 . to negate) (3,5)
-3
Prelude Control.Lens> (3,5)^._1.to negate
-3
Prelude Control.Lens> views _1 negate (3,5)
-3
Prelude Control.Lens Control.Monad.State> evalState (use (_1 . to negate)) (3,5) 
-3
Prelude Control.Lens Control.Monad.State> evalState (uses _1 negate) (3,5)
-3
Prelude Control.Lens> preview (_Just .to (+2)) (Just 3)
Just 5
Prelude Control.Lens> previews _Just (+2) (Just 3)
Just 5
```

to 操作符可以把普通函数提升为 Getter。

view + to = views

use + to = uses

preview + to = previews

### mapped

```
Prelude Control.Lens> over mapped (+1) [1,2,3]
[2,3,4]
Prelude Control.Lens> set mapped 5 [1,2,3]
[5,5,5]
Prelude Control.Lens> over (mapped._1) (+1) [(1,2),(3,4)]
[(2,2),(4,4)]
Prelude Control.Lens> set (mapped.mapped) 5 [[1],[2]]
[[5],[5]]
Prelude Control.Lens> [[1],[2]] & mapped.mapped .~ 5
[[5],[5]]
Prelude Control.Lens> mapped %~ (+1) $ [1,2,3]
[2,3,4]
```

mapped 操作符可以把普通函数提升为 Functor。

### ix

```
Prelude Control.Lens> [1,2,3] ^? ix 1
Just 2
Prelude Control.Lens> [1,2,3] ^?! ix 1
2
Prelude Control.Lens> [1,2,3] & (ix 1) .~ 20
[1,20,3]
```

### at

```
relude Control.Lens> import qualified Data.Map as Map
Prelude Control.Lens Map> Map.fromList [(1,"world")] ^.at 1
Just "world"
Prelude Control.Lens Map> Map.fromList [(1,"world")] ^.ix 1
"world"
Prelude Control.Lens Map> at 1 ?~ "hello" $ Map.empty
fromList [(1,"hello")]
Prelude Control.Lens Map> ix 1 ?~ "hello" $ Map.empty
fromList []
```

### traverse

```
Prelude Control.Lens> [[1], [2], [3]] ^. traverse
[1,2,3]
Prelude Control.Lens> [1,2,3] ^.. traverse
[1,2,3]
Prelude Control.Lens> [[1], [2], [3]] ^.. traverse . traverse
[1,2,3]
```

### non

```
Prelude Control.Lens> Just 3 ^. non 1
3
Prelude Control.Lens> Nothing ^. non 1
1
```

### 参考链接

[Haskell/Lenses and functional references](https://en.wikibooks.org/wiki/Haskell/Lenses_and_functional_references)
[Control.Lens.Tutorial](https://hackage.haskell.org/package/lens-tutorial-1.0.3/docs/Control-Lens-Tutorial.html)
[A Little Lens Starter Tutorial](https://www.schoolofhaskell.com/school/to-infinity-and-beyond/pick-of-the-week/a-little-lens-starter-tutorial)


