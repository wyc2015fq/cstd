# Haskell语言学习笔记（44）Lens（2） - zwvista - 博客园

## [Haskell语言学习笔记（44）Lens（2）](https://www.cnblogs.com/zwvista/p/7862987.html)

### 自定义 Lens 和 Isos

```
-- Some of the examples in this chapter require a few GHC extensions:
-- TemplateHaskell is needed for makeLenses; RankNTypes is needed for
-- a few type signatures later on. 
{-# LANGUAGE TemplateHaskell, RankNTypes #-}

import Control.Lens
import Control.Monad.State

data Point = Point
    { _positionX :: Double
    , _positionY :: Double
    } deriving (Show)
makeLenses ''Point

data Segment = Segment
    { _segmentStart :: Point
    , _segmentEnd :: Point
    } deriving (Show)
makeLenses ''Segment

makePoint :: (Double, Double) -> Point
makePoint (x, y) = Point x y

makeSegment :: (Double, Double) -> (Double, Double) -> Segment
makeSegment start end = Segment (makePoint start) (makePoint end)

testPoint = makePoint (2,3)
testSeg = makeSegment (0, 1) (2, 4)

pointCoordinates :: Traversal Point Point Double Double
-- :: Applicative f => (Double -> f Double) -> Point -> f Point
pointCoordinates g (Point x y) = Point <$> g x <*> g y

deleteIfNegative x = if x < 0 then Nothing else Just x

extremityCoordinates :: Traversal Segment Segment Double Double
-- :: Applicative f => (Double -> f Double) -> Segment -> f Segment
extremityCoordinates g (Segment start end) =
    Segment <$> pointCoordinates g start <*> pointCoordinates g end

scaleSegment :: Double -> Segment -> Segment
scaleSegment x = over extremityCoordinates (x *)

stateExample :: State Segment ()
stateExample = do
    segmentStart .= makePoint (0,0)
    zoom segmentEnd $ do
        positionX += 1
        positionY *= 2
        pointCoordinates %= negate

unmakePoint :: Point -> (Double, Double)
unmakePoint (Point x y) = (x,y)

pointPair :: Iso' Point (Double, Double)
pointPair = iso unmakePoint makePoint
```

自定义 Lens 的几个步骤
- {-# LANGUAGE TemplateHaskell, RankNTypes #-}

使用语言扩展
- import Control.Lens

使用Lens库
- data Point = Point { _positionX :: Double, _positionY :: Double }

使用 data 关键字定义数据结构以及字段名，注意字段名必须用下划线开头。
- makeLenses ''Segment

使用 makeLenses ''TypeName 来定义 lens。

自定义 Isos 的几个步骤
- import Control.Lens

使用Lens库
- makePoint :: (Double, Double) -> Point

makePoint (x, y) = Point x y

unmakePoint :: Point -> (Double, Double)

unmakePoint (Point x y) = (x,y)

准备两个输入和输出正好相反的函数。
- pointPair :: Iso' Point (Double, Double)

pointPair = iso unmakePoint makePoint

使用 iso 函数将两者粘合为一个 Iso 。

```
*Main> view segmentEnd testSeg
Point {_positionX = 2.0, _positionY = 4.0}
*Main> set segmentEnd (makePoint (2, 3)) testSeg
Segment {_segmentStart = Point {_positionX = 0.0, _positionY = 1.0}, _segmentEnd = Point {_positionX = 2.0, _positionY = 3.0}}
*Main> view (segmentEnd . positionY) testSeg
4.0
*Main> over (segmentEnd . positionY) (2 *) testSeg
Segment {_segmentStart = Point {_positionX = 0.0, _positionY = 1.0}, _segmentEnd = Point {_positionX = 2.0, _positionY = 8.0}}
*Main> testSeg ^. segmentEnd
Point {_positionX = 2.0, _positionY = 4.0}
*Main> testSeg & segmentEnd .~ makePoint (2, 3)
Segment {_segmentStart = Point {_positionX = 0.0, _positionY = 1.0}, _segmentEnd = Point {_positionX = 2.0, _positionY = 3.0}}
*Main> testSeg ^. segmentEnd . positionY
4.0
*Main> testSeg & segmentEnd . positionY %~ (2*)
Segment {_segmentStart = Point {_positionX = 0.0, _positionY = 1.0}, _segmentEnd = Point {_positionX = 2.0, _positionY = 8.0}}
```

(.) 可以用来组合两个lens。

```
*Main> pointCoordinates deleteIfNegative (makePoint (1, 2))
Just (Point {_positionX = 1.0, _positionY = 2.0})
*Main> pointCoordinates deleteIfNegative (makePoint (-1, 2))
Nothing
*Main> over pointCoordinates negate (makePoint (1, 2))
Point {_positionX = -1.0, _positionY = -2.0}
*Main> set pointCoordinates 7 (makePoint (1, 2)) 
Point {_positionX = 7.0, _positionY = 7.0}
*Main> toListOf extremityCoordinates (makeSegment (0, 1) (2, 3))
[0.0,1.0,2.0,3.0]
*Main> scaleSegment 2 (makeSegment (0, 1) (2, 3))
Segment {_segmentStart = Point {_positionX = 0.0, _positionY = 2.0}, _segmentEnd = Point {_positionX = 4.0, _positionY = 6.0}}
*Main> execState stateExample (makeSegment (1,2) (5,3))
Segment {_segmentStart = Point {_positionX = 0.0, _positionY = 0.0}, _segmentEnd = Point {_positionX = -6.0, _positionY = -6.0}}
```

```
*Main> import Data.Tuple (swap)
*Main Data.Tuple> view pointPair testPoint -- Equivalent to unmakePoint
(2.0,3.0)
*Main Data.Tuple> view (pointPair . _2) testPoint
3.0
*Main Data.Tuple> over pointPair swap testPoint
Point {_positionX = 3.0, _positionY = 2.0}
*Main Data.Tuple> view (from pointPair) (2,3) -- Equivalent to makePoint
Point {_positionX = 2.0, _positionY = 3.0}
*Main Data.Tuple> view (from pointPair . positionY) (2,3)
3.0
```

### 自定义 Prisms

```
{-# LANGUAGE TemplateHaskell, RankNTypes #-}

import Control.Lens

data NewTask =
  SimpleTask String |
  HarderTask String Int |
  CompoundTask String [NewTask]
  deriving (Show)

makePrisms ''NewTask

a = SimpleTask "Clean"
b = HarderTask "Clean Kitchen" 15
c = CompoundTask "Clean House" [a,b]
```

自定义 prisms 的几个步骤
- {-# LANGUAGE TemplateHaskell, RankNTypes #-}

使用语言扩展
- import Control.Lens

使用Lens库
- data NewTask = SimpleTask | HarderTask | CompoundTask

使用 data 关键字定义联合类型。
- makePrisms ''NewTask

使用 makePrisms ''TypeName 来定义 prisms。
- 注意这里的情况和 makeLenses 正相反，字段名没有带下划线，属性名带下划线。

```
*Main> a ^? _SimpleTask
Just "Clean"
*Main> b ^? _HarderTask
Just ("Clean Kitchen",15)
*Main> b ^? _HarderTask._2
Just 15
*Main> b & _SimpleTask .~ "Clean Garage"
HarderTask "Clean Kitchen" 15
*Main> b & _HarderTask._2 .~ 30
HarderTask "Clean Kitchen" 30
```

### 参考链接

[Haskell/Lenses and functional references](https://en.wikibooks.org/wiki/Haskell/Lenses_and_functional_references)
[Control.Lens.Tutorial](https://hackage.haskell.org/package/lens-tutorial-1.0.3/docs/Control-Lens-Tutorial.html)
[A Little Lens Starter Tutorial](https://www.schoolofhaskell.com/school/to-infinity-and-beyond/pick-of-the-week/a-little-lens-starter-tutorial)


