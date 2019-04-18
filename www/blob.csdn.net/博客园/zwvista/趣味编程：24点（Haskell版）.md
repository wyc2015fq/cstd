# 趣味编程：24点（Haskell版） - zwvista - 博客园

## [趣味编程：24点（Haskell版）](https://www.cnblogs.com/zwvista/p/7674731.html)

[24 game/Solve](https://rosettacode.org/wiki/24_game/Solve#Haskell)

```
import Data.List
import Data.Ratio
import Control.Monad
 
data Expr = Constant Rational |
    Expr :+ Expr | Expr :- Expr |
    Expr :* Expr | Expr :/ Expr
    deriving (Eq)
    
data Result = Result {value :: Rational, lastOp :: String, lastValues :: [Rational]}
 
ops = [((:+), "+"), ((:-), "-"), ((:*), "*"), ((:/), "/")]
 
instance Show Expr where
    show (Constant x) = show $ numerator x
    show (a :+ b)     = strexp "+" a b
    show (a :- b)     = strexp "-" a b
    show (a :* b)     = strexp "*" a b
    show (a :/ b)     = strexp "/" a b
 
strexp :: String -> Expr -> Expr -> String
strexp op a b = "(" ++ show a ++ " " ++ op ++ " " ++ show b ++ ")"
 
templates :: [[Expr] -> Expr]
templates = do
    (op1, ch1) <- ops
    (op2, ch2) <- ops
    (op3, ch3) <- ops
    let t1 = \[a, b, c, d] -> ((a `op1` b) `op2` c) `op3` d
    let t2 = \[a, b, c, d] -> (a `op1` b) `op2` (c `op3` d)
    let t3 = \[a, b, c, d] -> (a `op1` (b `op2` c)) `op3` d
    let t4 = \[a, b, c, d] -> a `op1` ((b `op2` c) `op3` d)
    let t5 = \[a, b, c, d] -> a `op1` (b `op2` (c `op3` d))
    case (ch1, ch2, ch3) of 
        ("+", "+", "+") -> [t1]
        ("*", "*", "*") -> [t1]
        ("+", "+",  _ ) -> [t1,t2,t4]
        ( _ , "+", "+") -> [t1,t3,t4]
        ("*", "*",  _ ) -> [t1,t2,t4]
        ( _ , "*", "*") -> [t1,t3,t4]
        otherwise -> [t1,t2,t3,t4,t5]
 
isSorted :: (Ord a) => [a] -> Bool
isSorted []       = True
isSorted [x]      = True
isSorted (x:y:xs) = x <= y && isSorted (y:xs)

eval :: Expr -> Maybe Result
eval (Constant c) = Just Result{value=c, lastOp="", lastValues=[c]}
eval (a :+ b)     = do
    Result{value=va, lastOp=opa, lastValues=lva} <- eval a
    let lva' = if opa == "+" then lva else [va]
    Result{value=vb, lastOp=opb, lastValues=lvb} <- eval b
    let lvb' = if opb == "+" then lvb else [vb]
    let lv = lva' ++ lvb'
    guard $ isSorted lv
    return Result{value=va + vb, lastOp="+", lastValues=lv}
eval (a :- b)     = do
    Result{value=va} <- eval a
    Result{value=vb} <- eval b
    let v = va - vb
    return Result{value=v, lastOp="", lastValues=[v]}
eval (a :* b)     = do
    Result{value=va, lastOp=opa, lastValues=lva} <- eval a
    let lva' = if opa == "*" then lva else [va]
    Result{value=vb, lastOp=opb, lastValues=lvb} <- eval b
    let lvb' = if opb == "*" then lvb else [vb]
    let lv = lva' ++ lvb'
    guard $ isSorted lv
    return Result{value=va * vb, lastOp="*", lastValues=lv}
eval (a :/ b)     = do
    Result{value=va} <- eval a
    Result{value=vb} <- eval b
    guard $ vb /= 0
    let v = va / vb
    return Result{value=v, lastOp="", lastValues=[v]}
 
solve :: Rational -> [Rational] -> [Expr]
solve target r4 = filter (maybe False (\r -> value r == target) . eval) $
    liftM2 ($) templates $
    nub $ permutations $ map Constant r4 
 
main = mapM_ (\x -> putStrLn $ show x ++ " = 24") . solve 24 $ [1,2,3,4]
```

```
((1 + 3) * (2 + 4)) = 24
(4 * ((1 + 2) + 3)) = 24
(((1 * 2) * 3) * 4) = 24
(((2 * 3) * 4) / 1) = 24
((2 * 3) * (4 / 1)) = 24
(3 * ((2 * 4) / 1)) = 24
(4 * ((2 * 3) / 1)) = 24
(2 * ((3 * 4) / 1)) = 24
((2 * (3 / 1)) * 4) = 24
(2 * ((3 / 1) * 4)) = 24
((2 * 3) / (1 / 4)) = 24
((2 * 4) / (1 / 3)) = 24
((3 * 4) / (1 / 2)) = 24
(3 * (2 / (1 / 4))) = 24
(2 * (3 / (1 / 4))) = 24
(4 * (2 / (1 / 3))) = 24
(2 * (4 / (1 / 3))) = 24
(4 * (3 / (1 / 2))) = 24
(3 * (4 / (1 / 2))) = 24
(((2 / 1) * 3) * 4) = 24
(2 / (1 / (3 * 4))) = 24
(3 / (1 / (2 * 4))) = 24
(4 / (1 / (2 * 3))) = 24
(2 / ((1 / 3) / 4)) = 24
(3 / ((1 / 2) / 4)) = 24
(4 / ((1 / 2) / 3)) = 24
(2 / ((1 / 4) / 3)) = 24
(4 / ((1 / 3) / 2)) = 24
(3 / ((1 / 4) / 2)) = 24
```


