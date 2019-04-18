# Haskell语言学习笔记（19）File IO - zwvista - 博客园

## [Haskell语言学习笔记（19）File IO](https://www.cnblogs.com/zwvista/p/6820482.html)

### 关于IO Action
- 类型为IO t。
- 运算时不执行，因而没有任何效果，只有执行时才会有效果，产生副作用。
- 一个IO Action只有在其他IO Action中才能被执行。
- 类型为IO t的IO Action被执行后的结果类型为t。

### File IO 函数
- openFile hClose 打开和关闭文件
readMode, writeMode, readwriteMode, appendMode
- hTell hSeek 文件当前读取位置
AbsoluteSeek, RelativeSeek, SeekFromEnd
- withFile 用回调函数读取文件
- readFile writeFile 直接读取写入文件

import System.IO

inputPath = "input.txt"
openFile1 = do
    handle <- openFile inputPath ReadMode
    contents <- hGetContents handle
    putStr contents
    hClose handle
withFile1 = do
    withFile inputPath ReadMode (\handle -> do
        contents <- hGetContents handle
        putStr contents)
readFile1 = do
    contents <- readFile inputPath
    putStr contents

以上代码使用三种不同的方法从文件中读取内容并打印到屏幕上。

### UTF-8文件的读写

readWriteUTF8File = do
    inputHandle <- openFile inputPath ReadMode 
    hSetEncoding inputHandle utf8
    theInput <- hGetContents inputHandle
    outputHandle <- openFile outputPath WriteMode
    hSetEncoding outputHandle utf8
    hPutStr outputHandle $ map toUpper theInput
    hClose inputHandle
    hClose outputHandle

### UTF-8文件的读写（2）

安装 extra 模块。

$ cabal install extra
Installed extra-1.6
import Extra
readWriteUTF8File' = do
    theInput <- readFileEncoding inputPath utf8
    writeFileEncoding utf8 outputPath $ upper theInput



