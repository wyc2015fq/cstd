# Haskell语言学习笔记（84）Concurrent - zwvista - 博客园

## [Haskell语言学习笔记（84）Concurrent](https://www.cnblogs.com/zwvista/p/9339587.html)

### Control.Concurrent

```
Prelude> import Control.Concurrent
Prelude Control.Concurrent>
```

Control.Concurrent 模块属于标准库，不需要安装。

### forkIO threadDelay
- `data ThreadId`

代表线程句柄的类型。
- `forkIO :: IO () -> IO ThreadId`

创建线程返回线程句柄，在新的线程中运行指定的 IO 操作。
- `threadDelay :: Int -> IO ()`

当前线程挂起 n 微秒（百万分之一秒，10负6次方分之一秒）。

### 示例 1

[haskell-simple-concurrency](https://github.com/crabmusket/haskell-simple-concurrency/blob/master/src/tutorial.md)

```
module Threads where

import Control.Concurrent (forkIO, threadDelay)
import Data.Foldable (for_)

main = do
    -- Synchronously perform some work.
    printMessagesFrom "main"

    -- Fork a new thread to do some work in the background.
    forkIO $ printMessagesFrom "fork"

    -- Fork another thread using an inline function!
    forkIO $ do
        putStrLn "starting!"
        sleepMs 5
        putStrLn "ending!"

    -- Wait for threads to finish.
    sleepMs 10

-- A simple function that prints three messages with a little delay between them.
printMessagesFrom name = for_ [1..3] printMessage
    where printMessage i = do
            sleepMs 1
            putStrLn (name ++ " number " ++ show i)

-- A utility function - threadDelay takes microseconds, which is slightly annoying.
sleepMs n = threadDelay (n * 1000)
```

```
*Threads> main
main number 1
main number 2
main number 3
starting!
fork number 1
fork number 2
fork number 3
ending!
```
- `forkIO $ printMessagesFrom "fork"`
`forkIO $ do putStrLn "starting!"; sleepMs 5; putStrLn "ending!"`

这里使用 forkIO 先后启动了两个线程：打印数字的线程和打印开始结束的线程

打印开始结束的过程为：打印开始，然后等待 5 毫秒，最后打印结束。
- `printMessagesFrom name = for_ [1..3] printMessage`

打印数字函数一共打印三个数字
- `printMessage i = do sleepMs 1; putStrLn (name ++ " number " ++ show i)`

每次打印数字之前先等待 1 毫秒
- 所以最后结果为

```
starting!          -- 0 毫秒
fork number 1      -- 1 毫秒
fork number 2      -- 2 毫秒
fork number 3      -- 3 毫秒
ending!            -- 5 毫秒
```

### STM

```
$ cabal install stm
stm-2.4.5.0 installed
```

STM（Software Transactional Memory，软件事务内存），是由软件实现的一种用于控制线程间同步的机制。

Haskell语言中，STM 是个 Monad。
- atomically :: STM a -> IO a

在 IO Monad 中作为原子操作来执行 STM Action。

### TMVar
- `data TMVar a`

TMVar 是同步的变量，用于线程间的同步。TMVar 变量可以想象成一个只能放一个东西的盒子，状态可以为满也可以为空。
- `newTMVar :: a -> STM (TMVar a)`

创建初期状态为满的 TMVar 变量，内容为指定的值。
- `newEmptyTMVar :: STM (TMVar a)`

创建初期状态为空的 TMVar 变量。
- `takeTMVar :: TMVar a -> STM a`

取出 TMVar 变量中的值，状态由满变空。如果TMVar 变量中没有值状态为空，就一直等待（阻塞）并不断重新尝试。
- `putTMVar :: TMVar a -> a -> STM ()`

将指定的值放入 TMVar 变量中，状态由空变满。如果TMVar 变量中已经有值状态为满，就一直等待（阻塞）并不断重新尝试。
- `readTMVar :: TMVar a -> STM a`

takeTMVar 和 putTMVar 的结合体，先从 TMVar 变量中取出值，然后再放回，最后返回这个值。

### 示例 2

```
module TMVars where

import Threads (sleepMs)

import Control.Concurrent.STM (atomically)
import Control.Concurrent (forkIO)
import Control.Concurrent.STM.TMVar (newEmptyTMVar, takeTMVar, putTMVar)

main = do
    result <- atomically $ newEmptyTMVar

    forkIO $ do
        -- Pretend there is some actual work to do.
        sleepMs 5
        putStrLn "Calculated result!"
        atomically $ putTMVar result 42

    putStrLn "Waiting..."
    value <- atomically $ takeTMVar result
    putStrLn ("The answer is: " ++ show value)
```

```
*TMVars> main
Waiting...
Calculated result!
The answer is: 42
```
- `result <- atomically newEmptyTMVar`

主线程创建空的 TMVar 变量 result。
- `value <- atomically $ takeTMVar result`

主线程不断尝试从 result 这个 TMVar 变量中取值。
- `forkIO (do sleepMs 5; putStrLn "Calculated result!"; atomically $ putTMVar result 42)`

forkIO 启动一个新的线程，先挂起 5 毫秒，然后将 42 放入 result 这个 TMVar 变量当中。

### 示例 3

```
module TMVarSharedState where

import Threads (sleepMs)

import Control.Concurrent (forkIO)
import Control.Concurrent.STM (atomically)
import Control.Concurrent.STM.TMVar (newTMVar, takeTMVar, putTMVar)
import Control.Monad (replicateM)

main = do
    counter <- atomically $ newTMVar 0

    let increment = do
            count <- atomically $ takeTMVar counter
            atomically $ putTMVar counter $! count + 1
        incrementer = do
            replicateM 1000 increment
            return ()

    threads <- replicateM 5 (forkIO incrementer)

    sleepMs 100
    count <- atomically $ takeTMVar counter
    print count
```

```
*TMVarSharedState> main
5000
```
- `counter <- atomically $ newTMVar 0`

主线程创建一个值为 0 的 TMVar 变量 counter。
- `ncrement = do count <- atomically $ takeTMVar counter; atomically $ putTMVar counter $! count + 1`

increment 函数将 TMVar 变量 counter 中的值加一（取出，加一，放回）。
- `incrementer = do replicateM 1000 increment; return ()`

incrementer 函数执行 1000 次 increment，即 1000 次“取出，加一，放回”。
- `replicateM 5 (forkIO incrementer)`

启动 5 个线程各自执行一次 incrementer 函数，总共 5000 次“取出，加一，放回”。
- `sleepMs 100; count <- atomically $ takeTMVar counter`

主线程等待 0.1 秒之后尝试取值
- 5000

结果正确，5 个线程之间没有发生数据竞争（data races）。

### TChan
- `data TChan a`

TChan 是一种没有边界的先进先出（FIFO）的 channel 类型。
- `newTChan :: STM (TChan a)`

新建一个 TChan。
- `readTChan :: TChan a -> STM a`

从 TChan 中读出下一个值。
- `writeTChan :: TChan a -> a -> STM ()`

向 TChan 中写入一个值。
- `dupTChan :: TChan a -> STM (TChan a)`

复制一个 TChan，在此之后任何向源 TChan 写入的数据将被复制到作为复制品的那个 TChan，反之亦然。

### 示例 4

```
module TChan1 where

import Control.Concurrent.STM (atomically)
import Control.Concurrent.STM.TChan (newTChan, writeTChan, readTChan)

main = do
    messages <- atomically newTChan
    atomically $ writeTChan messages "unbounded"
    atomically $ writeTChan messages "channels"

    -- Read a message from the channel, then output it.
    msg <- atomically $ readTChan messages
    putStrLn msg
    -- Do the same thing again, but more concisely.
    putStrLn =<< (atomically $ readTChan messages)
```

```
*TChan1> main
unbounded
channels
```

程序先创建一个 TChan，向其中写入两个字符串，然后再读出。

### 示例 5

```
module TChan2 where

import Control.Monad.STM
import Control.Concurrent
import Control.Concurrent.STM.TChan

oneSecond = 1000000

writerThread :: TChan Int -> IO ()
writerThread chan = do
        atomically $ writeTChan chan 1
        threadDelay oneSecond
        atomically $ writeTChan chan 2
        threadDelay oneSecond
        atomically $ writeTChan chan 3
        threadDelay oneSecond

readerThread :: TChan Int -> IO ()
readerThread chan = do
        newInt <- atomically $ readTChan chan
        putStrLn $ "read new value: " ++ show newInt
        readerThread chan

main = do
        chan <- atomically $ newTChan
        forkIO $ readerThread chan
        forkIO $ writerThread chan
        threadDelay $ 5 * oneSecond
```

```
*TChan2> main
read new value: 1
read new value: 2
read new value: 3
```
- writerThread 线程每隔一秒向 chan 里面写入一个数。
- readerThread 线程不断地尝试从 chan 里读出数并打印。

### 示例 6

```
module DuplicatingTChan where

import Threads (sleepMs)

import Control.Concurrent.STM
import Control.Concurrent (forkIO)
import Control.Concurrent.STM.TChan (newTChan, writeTChan, readTChan, dupTChan)

nonDuplicatedTest = do
    messages <- atomically newTChan
    forkIO $ messageReader messages "First"
    forkIO $ messageReader messages "Second"
    atomically $ writeTChan messages "Hi!"

messageReader channel name = do
    msg <- atomically $ readTChan channel
    putStrLn (name ++ " read: " ++ msg)

duplicatedTest = do
    broadcast <- atomically newTChan
    forkIO $ broadcastReader broadcast "Third"
    forkIO $ broadcastReader broadcast "Fourth"
    sleepMs 1
    atomically $ writeTChan broadcast "Bye!"

broadcastReader channel name = do
    channel' <- atomically $ dupTChan channel
    messageReader channel' name

main = do
    nonDuplicatedTest
    duplicatedTest
    sleepMs 10
```

```
*DuplicatingTChan> main
First read: Hi!
Third read: Bye!
Fourth read: Bye!
```
- 对于一个 channel A，写入 A 的信息只能通过 A 读出一次。
- 调用 dupTChan 函数将 channel A 复制为 channel B 时，写入 A 的信息可以通过 B 再次读出。

### TVar
- data TVar a

支持 STM 的可共享的内存变量。
- newTVar :: a -> STM (TVar a)

新建一个 TVar，值为 a。
- readTVar :: TVar a -> STM a

读取 TVar 的值
- writeTVar :: TVar a -> a -> STM ()

将值写入 TVar
- modifyTVar :: TVar a -> (a -> a) -> STM ()

修改 TVar 的值

### 示例 7

```
module TVarSharedState where

import Threads (sleepMs)

import Control.Concurrent (forkIO)
import Control.Concurrent.STM (atomically)
import Control.Concurrent.STM.TVar (newTVar, readTVar, writeTVar)
import Control.Monad (replicateM)

main = do
    counter <- atomically $ newTVar 0

    let increment = atomically $ do
            count <- readTVar counter
            writeTVar counter $! count + 1
        incrementer = do
            replicateM 1000 increment
            return ()

    threads <- replicateM 5 (forkIO incrementer)

    sleepMs 100
    count <- atomically $ readTVar counter
    print count
```

```
*TVarSharedState> main
5000
```
- `increment = atomically $ do count <- readTVar counter; writeTVar counter $! count + 1`

注意这里读写必须组合成原子操作，如果改成
`increment = do count <- atomically $ readTVar counter; atomically $ writeTVar counter $! count + 1`

的话，不同线程之间会发生数据竞争，即所有线程可能都会先读取同一个值，再写同一个值

最终结果不定，可以是5000，也可以是1000。

### TMVar 和 TVar

`newtype TMVar a = TMVar (TVar (Maybe a))`

从目前的实现上看，TMVar 封装的是 TVar (Maybe a)，所谓状态为空为满是通过 Maybe 类型来实现的。

### TMVar 和 MVar

MVar 是标准库所实现的的 IO Monad 中的同步变量，已不推荐使用。

与此相对应，TMVar 是 STM Monad 中的同步变量，更为健壮。

### TArray

支持 STM 以及 MArray 接口的数组类型。


