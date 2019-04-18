# Haskell语言学习笔记（34）System.Environment - zwvista - 博客园

## [Haskell语言学习笔记（34）System.Environment](https://www.cnblogs.com/zwvista/p/7726566.html)

### System.Environment

getArgs :: IO [String]

getProgName :: IO String

getExecutablePath :: IO FilePath

getArgs 这个 IO Action 读取所有的命令行参数，返回一个字符串数组。

getProgName 这个 IO Action 返回程序的名字。

getExecutablePath 这个 IO Action 返回程序所在路径。

```
import System.Environment 
import Data.List

main = do
    args <- getArgs
    progName <- getProgName
    exePath <- getExecutablePath
    putStrLn "The arguments are:"
    mapM putStrLn args
    putStrLn "The program name is:"
    putStrLn progName
    putStrLn "The executable path is:"
    putStrLn exePath
```

### :main

```
*Main> :main a b c "d e f"
The arguments are:
a
b
c
d e f
The program name is:
<interactive>
The executable path is:
/Library/Frameworks/GHC.framework/Versions/8.2.1-x86_64/usr/lib/ghc-8.2.1/bin/ghc
```

### :set args

```
*Main> :set args one two three
*Main> main
The arguments are:
one
two
three
...
```


