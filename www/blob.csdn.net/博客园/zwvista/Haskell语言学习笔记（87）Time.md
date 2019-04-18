# Haskell语言学习笔记（87）Time - zwvista - 博客园

## [Haskell语言学习笔记（87）Time](https://www.cnblogs.com/zwvista/p/9354855.html)

### 安装 time

```
$ cabal install time
Installed time-1.9.1
Prelude> import Data.Time
Prelude Data.Time>
```

time 是一个关于时间（日，时，时区，日历）的库。

### 示例

```
Prelude Data.Time> fromGregorianValid 2008 10 22
Just 2008-10-22
Prelude Data.Time> fromGregorianValid 2014 2 31
Nothing
Prelude Data.Time> getCurrentTime
2018-07-23 07:26:58.5692 UTC
Prelude Data.Time> utctDay <$> getCurrentTime
2018-07-23
Prelude Data.Time> toGregorian . utctDay <$> getCurrentTime
(2018,7,23)
Prelude Data.Time> now <- getCurrentTime
Prelude Data.Time> now
2018-07-23 07:52:23.092741 UTC
```


