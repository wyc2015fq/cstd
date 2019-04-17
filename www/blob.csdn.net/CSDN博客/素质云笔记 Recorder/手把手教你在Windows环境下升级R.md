# 手把手教你在Windows环境下升级R - 素质云笔记/Recorder... - CSDN博客





2016年05月20日 15:15:13[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5297
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)










在Windows环境下，我们可以使用`installr`包自动将R升级到最新版本。并且可以安装软件。下面主要演示如何在Windows环境下升级R，并将旧版本安装的R包复制到更新版本的R。

#### 1、加载installr包

```
install.packages("installr")
library(installr)
```

#### 2、在“installr”菜单中选择“update R”

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjftz59WoaS1FtQFTiaf29wPrzoxiaicrjmrWgHbT9ueHMUiaKtw0ia76GVRrA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 3、installr将会检测是否发布了新版本的R，如果检查到有单击“确定”即可更新R。

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfZibLftPohKzyz68YODAuibN2gCP4K9br6hpD2ms8qGsVtcyq6b6x6z7g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 4、如果想了解新版本R的相关特性介绍，单击“是”，可在浏览器中打开这个信息。

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfiaBE1YY1icr5pWvP9MEnC8zzawCcPNTK1OUPQ7qysx0U4jEDHdJCgBtw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 5、下载并安装最新版本R，单击“是”。

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfWNxsuWXrQZzPlhTzWf8bib1P5wx2jEtzezUXiacSdzHBkibJZxyjBM2Ig/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 6、等待R的下载

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfJCEXKmhx9x3DpPDNIib4OSAIVibIMl2umLLsEnLK5IYLNx08yMZAyjdQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 7、单击安装向导中的“下一步”按钮

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfmDk0emjHJmBc5WQPwYJbxTVToP8aib8GwMBQhKic9qBkeahWhdOaRib2g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 8、等待完成R的安装

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfDgUSgSz7iajutptD3sfIcUWrx6ql0Azskh27yLlkv8JyMuZ1L5CjNlg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 9、将旧版本R中的包复制到新版本R，单击“是”




**笔者运行到这步，就一直不能往下执行了，因为library()不到包了，于是用了另外的办法，贴在最后**






![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjf0ZJJNPd3cE15JdPjPmy2Y2zam8fbnkhQ08Ah3fhbkrE3KWY9PgumSQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 10、保留包到旧版本的R（为了安全起见），单击“是”

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfqsh7sEg8R5thdQjpo1QEcJJN5BxMakBPJCQ3gmsjr9YsASYhhh7vKQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 11、复制旧版本的配置文件Rprofile.site到新版本R

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfhia2hOXUKNwhvLGJXCUmk1k3ALiatzTE1NQFYxL2ZvcBKh0qUpqFUfvw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 12、在新版R中更新包的版本，单击“是”

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfIUx44gjdL7niakGC2m5Wmm8Dria9CzEKDdhXialBZ1gxGlFdjzYdxMgnQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### 13、在RStudio中更换使用的R版本

![](http://mmbiz.qpic.cn/mmbiz/y2fhgP4leThjUaDjoTkUYZCNdHQNQMjfvpSnRLX3sVJQ2Uia6B5nCCg2EdAs8ia68asZcCUECBqRsY67HxmpR8Ew/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


至此，我们就将R升级到了最新的版本。








————————————————————————————————————

**如果在更新版本时，将包一起调用过来！！**



**##R版本升级之后，可以将原来版本的包复制到如今，然后执行下面的命令update.packages(checkBuilt=TRUE, ask=FALSE) **




























