# Haskell 每周新闻 - weixin_33985507的博客 - CSDN博客
2016年02月02日 18:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
29.1.2016
## 精选
- Oskar Wickström 重写了 [Oden-to-Go transpiler](https://link.jianshu.com?t=http://oden-lang.org/blog/compiler/2016/01/18/the-haskell-rewrite.html)，将其从 Racket 迁移至 Haskell。Oden 作为函数式编程语言， 结合了 ML 类型系统与 LISP 语法。Oskar 解释此次迁移可归因于 Haskell 相较 Racket 之优势：exhaustive pattern-match 检查，以类型为导向重构，monad transformers 与更快执行速度。
与此相关， [convo over at lobste.rs](https://link.jianshu.com?t=https://lobste.rs/s/8hidko/the_haskell_rewrite_the_motivation_for_moving_away_from_racket) 链接了 Gabriel Gonzalez 之论断：就写编译器而言，Haskell 惊艳无比。若你正在用其他语言写编译器，你真心可以考虑转换至 Haskell。
## 格言
- [来自HN](https://link.jianshu.com?t=https://news.ycombinator.com/item?id=10972051#up_10979314)：若有闲暇，我建议你学 Haskell，以便拓展智力而成为杰出 OCaml 开发者，如同学习拉丁语之于法语或意大利语作家。
22.1.2016
## 精选
- Haker News [高度评价此文章](https://link.jianshu.com?t=https://news.ycombinator.com/item?id=10913720)，以至于停留首页[五小时之久](https://link.jianshu.com?t=http://hnrankings.info/10913720/)。然而其评论造成了高阶 Haskell 操纵者从一众 HN 读者头顶呼啸而过之错觉。
## 格言
- huschi：以 Haskell 编程令人些许挫败。我错过了寻找错误的机会。
- bakert：我明白，若我可知晓所有此类疯狂的函数，我可将代码体积减至十分之一。
- Will Jones：写 Haskell 代码越多，越觉其似 Forth：先为我的问题发明一门语言，再以其解决问题。
15.1.2016
## 精选
- [Verity Stob](https://link.jianshu.com?t=https://en.wikipedia.org/wiki/Verity_Stob), the doyen of information technology satire, [skewers the cargo culting of Functional Programming](https://link.jianshu.com?t=http://www.theregister.co.uk/2016/01/13/stob_remember_the_monoids/) and by the by writes a monad tutorial (omg!). Haskell redditors [chuckle and cluck at the hatchet job](https://link.jianshu.com?t=https://www.reddit.com/r/haskell/comments/40wqzf/learn_you_func_prog_on_five_minute_quick_el_regs/).
## 格言
- [Tom Ellis](https://link.jianshu.com?t=https://news.ycombinator.com/item?id=10870488#up_10874714)：Haskell 中你所战斗的，并非类型系统，而是你的 bug。
- [Jeremy Bowers](https://link.jianshu.com?t=https://news.ycombinator.com/item?id=10892293#up_10896419)：Haskell 有趣在于 Haskell是唯一理论思想与实践思想相互交融之处。而他处，两种思想仅仅相互鄙夷。
- Redditor [lukewarm](https://link.jianshu.com?t=https://www.reddit.com/r/haskell/comments/40aa4n/how_to_make_industry_quality_software_with_haskell/cysqbvr)：确实，Haskell 亦可写出工业质量软件。在 IO Monad 内进行所有计算，将中间结果放至 MVar 中。仅使用 Int 和 String 类型。使用异常捕捉错误。使用 Template Haskell 自定义结构模拟 for 与 while。
21.8.2015
## 精选
- Gabriel Gonzalez 以美国地址格式[评价 Haskell](https://link.jianshu.com?t=https://github.com/Gabriel439/post-rfc/blob/master/sotu.md)。他使用二十八个头衔评价 Haskell 从不成熟到成熟再到 Best-in-class，前四个头衔为 *Compilers，**Server-side web programming，**Scripting / Command-line applications，* 与 *Numerical programming。* 他推荐了各头衔下相关库与教程。[Hacker News](https://link.jianshu.com?t=https://news.ycombinator.com/item?id=10071535) 与 [/r/haskell](https://link.jianshu.com?t=https://www.reddit.com/r/haskell/comments/3haulk/state_of_the_haskell_ecosystem_august_2015/)亦再次提及。
- Aaron Wolf 从零基础编程直接学习 Haskell 并且写下了[其经历]()。学习资源中 [Haskell Wikibook](https://link.jianshu.com?t=https://en.wikibooks.org/wiki/Haskell) 为其最爱，他可在阅读时改进。他是一 freely-licensed 产品众筹平台，[Snowdirt.coop](https://link.jianshu.com?t=https://snowdrift.coop/p/snowdrift/w/en/about) 的创立者之一。[Haskell Reddit](https://link.jianshu.com?t=https://www.reddit.com/r/haskell/comments/3hbrom/learning_haskell_from_nothing_the_technical/) 发现 Aaron 的证词改变着“Haskell 对我太难了” 的 meme。
## 格言
- [/u/kamatsu](https://link.jianshu.com?t=https://www.reddit.com/r/haskell/comments/3h5og6/i_now_tend_to_think_that_the_concepts_behind_all/cu4m6p8)：我感觉，人们认为 Haskell 是一种开天眼般的体验可以归咎于其 CS 教育的缺失。
7.8.2015
## 格言
- [Redditor fegu](https://link.jianshu.com?t=https://www.reddit.com/r/haskell/comments/3fqiff/dont_give_up_on_haskell/ctr11f0)：Haskell 犹如精神性毒品，越过最初的门槛后你将彻底陷入对其无法自拔的渴望。你将为其舍弃你多金的 .Net 职业晋升，因此你长远职业生涯将会受损。但每一天将会变得更加明媚欢乐，虽仅有渺茫希望可全职工作于薪资不匪的 Haskell 职位。我希望此止增笑耳。
译自
[https://wiki.haskell.org/Haskell_Weekly_News](https://link.jianshu.com?t=https://wiki.haskell.org/Haskell_Weekly_News)
[http://sequence.complete.org/](https://link.jianshu.com?t=http://sequence.complete.org/)
