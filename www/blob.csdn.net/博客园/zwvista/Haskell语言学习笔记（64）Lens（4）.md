# Haskell语言学习笔记（64）Lens（4） - zwvista - 博客园

## [Haskell语言学习笔记（64）Lens（4）](https://www.cnblogs.com/zwvista/p/8035209.html)

### 安装 lens-tutorial

[Control.Lens.Tutorial](https://hackage.haskell.org/package/lens-tutorial-1.0.3/docs/Control-Lens-Tutorial.html)

```
$ cabal install lens-tutorial
Installed lens-tutorial-1.0.3
Prelude> :m +Control.Lens.Tutorial
Prelude Control.Lens.Tutorial>
```

```
{-# LANGUAGE TemplateHaskell   #-}
{-# LANGUAGE DeriveFoldable    #-}
{-# LANGUAGE DeriveFunctor     #-}
{-# LANGUAGE DeriveTraversable #-}
 
import Control.Applicative (Applicative)
import Control.Lens hiding (element)
import Data.Foldable (Foldable)
import Data.Monoid (Monoid)
 
data Atom = Atom { _element :: String, _point :: Point } deriving (Show)
data Point = Point { _x :: Double, _y :: Double } deriving (Show)
data Molecule = Molecule { _atoms :: [Atom] } deriving (Show)
data Pair a = Pair a a deriving (Functor, Foldable, Traversable)
makeLenses ''Atom
makeLenses ''Point
makeLenses ''Molecule

shiftAtomX :: Atom -> Atom
shiftAtomX = over (point . x) (+ 1)

shiftMoleculeX :: Molecule -> Molecule
shiftMoleculeX = over (atoms . traverse . point . x) (+ 1)

shift :: ASetter' a Double -> a -> a
shift lens = over lens (+ 1)
```

### Lens 的类型

```
type Lens' a b = forall f . Functor f => (b -> f b) -> (a -> f a)
type ASetter' a b   = (b -> Identity b) -> (a -> Identity a)
-- ... equivalent to: (b ->          b) -> (a ->          a)
type Getting b a b  = (b -> Const b b) -> (a -> Const b a)
-- ... equivalent to: (b ->       b  ) -> (a ->       b  )
--                                        (a ->       b  )

--    +-- Bigger type
--    |
--    v
Lens' bigger smaller
--           ^
--           |
--           +--  Smaller type within the bigger type
element :: Lens' Atom String
point   :: Lens' Atom Point
x       :: Lens' Point Double
y       :: Lens' Point Double

-- lens :: (a -> b) -> (a -> b -> a) -> Lens' a b
point2 :: Lens' Atom Point
point2 = lens _point (\atom newPoint -> atom { _point = newPoint })

-- point3 :: Lens' Atom Point
point3 :: Functor f => (Point -> f Point) -> Atom -> f Atom
point3 k atom = fmap (\newPoint -> atom { _point = newPoint }) (k (_point atom))
```

```
(.) :: Lens' a b -> Lens' b c -> Lens' a c
type Lens' a b = forall f . Functor f => (b -> f b) -> (a -> f a)
(.) :: Functor f
    => ((b -> f b) -> (a -> f a))
    -> ((c -> f c) -> (b -> f b))
    -> ((c -> f c) -> (a -> f a))
point     :: Lens' Atom Point
x         :: Lens' Point Double
point . x :: Lens' Atom Double

view (point . x) :: Atom -> Double
over (point . x) :: (Double -> Double) -> (Atom -> Atom)
view :: Lens' a b -> a -> b
over :: Lens' a b -> (b -> b) -> a -> a
set  :: Lens' a b ->       b  -> a -> a
set lens b = over lens (\_ -> b)

view (lens1 . lens2) = (view lens2) . (view lens1)
view id = id
over (lens1 . lens2) = (over lens1) . (over lens2)
over id = id
```

```
type Traversal' a b = forall f . Applicative f => (b -> f b) -> (a -> f a)
type Lens'      a b = forall f . Functor     f => (b -> f b) -> (a -> f a)

atoms   :: Traversal' Molecule [Atom]
element :: Traversal' Atom     String
point   :: Traversal' Atom     Point
x       :: Traversal' Point    Double
y       :: Traversal' Point    Double

traverse :: Traversable t => Traversal' (t a) a
traverse :: (Applicative f, Traversable t) => (a -> f a) -> t a -> f (t a)
traverse :: Traversal' [a] a

traverse :: Traversal' (Pair a) a
over traverse :: (a -> a) -> (Pair a -> Pair a)
over traverse (+ 1) (Pair 3 4) = Pair 4 5

(.) :: Traversal' a b -> Traversal' b c -> Traversal' a c
(.) :: Applicative f
    => ((b -> f b) -> (a -> f a))
    -> ((c -> f c) -> (b -> f b))
    -> ((c -> f c) -> (a -> f a))

-- Remember that `atoms`, `point`, and `x` are also `Traversal'`s
atoms                        :: Traversal' Molecule [Atom]
traverse                     :: Traversal' [Atom]   Atom
point                        :: Traversal' Atom     Point
x                            :: Traversal' Point    Double

-- Now compose them
atoms                        :: Traversal' Molecule [Atom]
atoms . traverse             :: Traversal' Molecule Atom
atoms . traverse . point     :: Traversal' Molecule Point
atoms . traverse . point . x :: Traversal' Molecule Double

over (atoms . traverse . point . x)
    :: (Double -> Double) -> (Molecule -> Molecule)
toListOf (atoms . traverse . point . x)
    :: Molecule -> [Double]

toListOf :: Traversal' a b -> a -> [b]
over :: Traversal' a b -> (b -> b) -> a -> a
set  :: Traversal' a b ->       b  -> a -> a
set traversal b = over traversal (\_ -> b)

toListOf (traversal1 . traversal2) = (toListOf traversal1) >=> (toListOf traversal2)
toListOf id = return
```

### 参考链接

[Taking a Close look at Lenses](https://mmhaskell.com/blog/2017/6/12/taking-a-close-look-at-lenses)


