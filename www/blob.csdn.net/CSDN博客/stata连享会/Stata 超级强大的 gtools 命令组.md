# Stata: 超级强大的 gtools 命令组 - stata连享会 - CSDN博客





2019年01月03日 08:21:46[arlionn](https://me.csdn.net/arlionn)阅读数：218
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)









`2018/12/29 0:00`

### [Stata 现场班报名中……](https://gitee.com/arlionn/stata_training/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)





![Nicholas J. Cox](https://upload-images.jianshu.io/upload_images/7692714-abb02fa45f5cf989.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![Mauricio Caceres](https://upload-images.jianshu.io/upload_images/7692714-d4caf008e5697b4e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
`egen` → `egenmore` → `gtools`


Stata 中的 `egen` 命令提供了大量的函数，能大幅提高数据处理效率。

随后由 [Nicholas J. Cox](https://www.dur.ac.uk/geography/staff/geogstaffhidden/?id=335) 提供的 `egenmore` 进一步丰富了 `egen` 的函数库，在文字处理方面的功能大幅提高。

而今，由 [Mauricio Caceres](https://mcaceresb.github.io/) 编写的 `gtools` 命令组则在速度提升方面将上述工作向前推进了一大步。

> 
让我们怀着崇高的敬意访问 [「gtools 主页」](https://gtools.readthedocs.io/en/latest/index.html)。






下面是作者写的感言：

Hi! I’m [Mauricio Caceres](https://mcaceresb.github.io/); I made gtools after some of my Stata jobs were taking literally days to run because of repeat calls to `egen`, `collapse`, and similar on data with over 100M rows. Feedback and comments are welcome! I hope you find this package as useful as I do.

Along those lines, here are some other Stata projects I like:
- 
[`ftools`](https://github.com/sergiocorreia/ftools): The main inspiration for gtools. Not as fast, but it has a rich feature set; its mata API in particular is excellent.

- 
[`reghdfe`](https://github.com/sergiocorreia/reghdfe): The fastest way to run a regression with multiple fixed effects (as far as I know).

- 
[`ivreghdfe`](https://github.com/sergiocorreia/ivreghdfe): A combination of [`ivreg2`](https://ideas.repec.org/c/boc/bocode/s425401.html) and `reghdfe`.

- 
[`stata_kernel`](https://kylebarron.github.io/stata_kernel): A Stata kernel for Jupyter; extremely useful for interacting with Stata.







> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-10a0763cc5b5117a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)




