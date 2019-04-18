# Haskell语言学习笔记（89）Unicode UTF8 - zwvista - 博客园

## [Haskell语言学习笔记（89）Unicode UTF8](https://www.cnblogs.com/zwvista/p/9426245.html)

### unicode-show

```
$ cabal install unicode-show
Installed unicode-show-0.1.0.2
Prelude> :m +Text.Show.Unicode
Prelude Text.Show.Unicode>
```

unicode-show 这个库可用于向控制台窗口输出 Unicode 字符。

```
Prelude Text.Show.Unicode> ("Хорошо!",["哈斯克尔7.6.1的力量","感じる"])
("\1061\1086\1088\1086\1096\1086!",["\21704\26031\20811\23572\&7.6.1\30340\21147\37327","\24863\12376\12427"])
Prelude Text.Show.Unicode> uprint it
"(\"Хорошо!\",[\"哈斯克尔7.6.1的力量\",\"感じる\"])"
Prelude Text.Show.Unicode> :set -interactive-print=Text.Show.Unicode.uprint
Prelude Text.Show.Unicode> it
("Хорошо!",["哈斯克尔7.6.1的力量","感じる"])
```

### hxt-unicode

```
$ cabal install hxt-unicode
Installed hxt-unicode-9.0.2.4
Prelude> import Data.String.Unicode
Prelude Data.String.Unicode>
```

hxt-unicode 这个库可用于在不同的 coding 之间转换。

```
Prelude Data.String.Unicode> unicodeToUtf8 "Хорошо!"
"\208\165\208\190\209\128\208\190\209\136\208\190!"
Prelude Data.String.Unicode> unicodeToUtf8 "哈斯克尔7.6.1的力量"
"\229\147\136\230\150\175\229\133\139\229\176\148\&7.6.1\231\154\132\229\138\155\233\135\143"
Prelude Data.String.Unicode> unicodeToUtf8 "感じる"
"\230\132\159\227\129\152\227\130\139"
```

### utf8-string

```
$ cabal install utf8-string
Installed utf8-string-1.0.1.1
Prelude> import Data.ByteString.UTF8
Prelude Data.ByteString.UTF8>
```

utf8-string 这个库可用于将字符串转换成 UTF8 字符串。

```
Prelude Data.ByteString.UTF8> fromString "Хорошо!"
"\208\165\208\190\209\128\208\190\209\136\208\190!"
Prelude Data.ByteString.UTF8> fromString "哈斯克尔7.6.1的力量"
"\229\147\136\230\150\175\229\133\139\229\176\148\&7.6.1\231\154\132\229\138\155\233\135\143"
Prelude Data.ByteString.UTF8> fromString "感じる"
"\230\132\159\227\129\152\227\130\139"
```


