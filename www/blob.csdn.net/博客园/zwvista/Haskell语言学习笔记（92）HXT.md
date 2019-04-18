# Haskell语言学习笔记（92）HXT - zwvista - 博客园

## [Haskell语言学习笔记（92）HXT](https://www.cnblogs.com/zwvista/p/9519335.html)

### HXT

[The Haskell XML Toolbox (hxt)](http://www.fh-wedel.de/~si/HXmlToolbox/) 是一个解析 XML 的库。

```
$ cabal install hxt
Installed hxt-9.3.1.16
Prelude> :m +Text.XML.HXT.Parser.XmlParsec
Prelude Text.XML.HXT.Parser.XmlParsec>
```

```
Prelude Text.XML.HXT.Parser.XmlParsec> xread "<foo>abc<bar/>def</foo>"
[NTree (XTag "foo" []) [NTree (XText "abc") [],NTree (XTag "bar" []) [],NTree (XText "def") []]]
Prelude Text.XML.HXT.Parser.XmlParsec> :m +Text.XML.HXT.DOM.FormatXmlTree
Prelude Text.XML.HXT.Parser.XmlParsec Text.XML.HXT.DOM.FormatXmlTree> putStrLn $ formatXmlTree $ head $ xread "<foo>abc<bar/>def</foo>"
---XTag "foo"
   |
   +---XText "abc"
   |
   +---XTag "bar"
   |
   +---XText "def"
```

### 参考链接

[Haskell/XML - Wikibooks](https://en.wikibooks.org/wiki/Haskell/XML)
[HXT - HaskellWiki](https://wiki.haskell.org/HXT)
[HXT Arrow Lessons](https://www.vex.net/~trebla/haskell/hxt-arrow/)
[XML Processing with Error checking](https://www.schoolofhaskell.com/school/advanced-haskell/xml-parsing-with-validation)
[Working With Html In Haskell](http://adit.io/posts/2012-04-14-working_with_HTML_in_haskell.html)


