# 科学网—数学排版软件LaTeX简介 - 王伟华的博文




# 数学排版软件LaTeX简介                           

已有 10238 次阅读2008-9-28 14:18|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:Latex,,,,LaTeX|[LaTeX](misc.php?mod=tag&id=4885)



注：在国内中科院数学与系统科学研究生院和有关高校特别是华东师范大学数学系的网站上也有此软件的介绍。本文中提到的下载地址并不是唯一的。

1 安装CTEX中文套件
1.1 初始安装下载ctex中文套件([www.ctex.org).](http://www.ctex.org)./)安装ctex中文套件,自定义安装到D盘.升级ghostscript.升级gsview,在options->easy configure下选择升级的ghostscript.升级winedt,直接覆盖安装.MiKTEX使用升级向导升级.
1.2 升级添加宏包已编译的,有.sty或.cls文件的,只需把文件放在localtexmftexLaTeX下的某个目录下,然后再刷新系统的文件名数据库就可以使用了.teTeX和fpTeX使用texhash命令.或者把这些宏包和TEX源文件放在同一目录下,不用刷新文件名数据库,但只有同一个目录下的文件可以使用这些宏包.如果使用的是宏包的源文件.先读readme文件,对.ins文件运行LaTeX命令生成.sty文件,对.dtx文件运行LaTeX命令生成帮助文档.
2 软件配置
2.1 winedt
2.1.1 缺省定义gsview工具按钮有可能无法使用.需要在注册表更改psfile下面的shell->open->command键值.
2.1.2 运行WinEdt目录下的宏可以添加metapost工具.
2.1.3 右键点击WinEdt状态栏,确认show custom info a已选取.点击对应的状态栏可以激活--src选项,这个是使用dvi search的必要条件.
3 新手入门
3.1 参考资料[lshort中文版],[ctex-faq],薛定宇著[LaTeX科学文件处理软件入门].清泉编制文档模板.
3.2 测试LaTeX
3.2.1 英文例子documentclass{article}begin{document}This is a test file.end{document}这个例子需要保存为.tex后缀的文件,然后用LaTeX编译,生成的DVI文件可以用DVI浏览器进行查看.或者用dvips转换成PostScript文件.假设文件名是test.ctx.生成PostScript 文件的完整编译过程是:LaTeX testdvips test
3.3 编写TEX文档的编辑器windows下使用winedt,或者是UltraEdit,Editplus.linux下使用vi,Emacs.
4 中文处理
4.1 CJK还是CCT推荐使用CJK处理中文.CJK比CCT的优越之处有以下几点:
1. 字体定义采用LaTeX NFSS标准,生成的DVI文件不必像CCT那样需要用patchdvi处理后才能预览和打印.而且一般GB编码的文件也不必进行预处理就可直接用LaTeX命令进行编译.
2. 可使用多种TrueType字体和Type1字体,生成的PDF文件更清楚,漂亮.
3. 能同时在文章中使用多种编码的文字,如中文简体繁体日文韩文等.当然,CCT在一些细节上,如字体可用中文字号,字距,段首缩进等更符合中文的习惯.而CJK就没有针对中文习惯指定特殊的排版标准.
4.2 CJK的例子documentclass{article}usepackage{CJK}begin{document}begin{CJK*}{GBK}{kai}这是中文楷体字。end{CJK*}end{document}
4.3 CJK和CJK*不建议使用CJK模式.CJK*模式自动忽略中文字符间的所有空格.加空格的方法是在空格前面加上 字符.实际上CJK*模式忽略中文字符后面的空格,因此中文后面如果接着英文,必须加上保护的空格 或者~ 符号,否则可能造成断行错误.
4.4 中英文混排时正确分行首先,英文标点符号后要留空格,这是英文的习惯,否则TEX无法断句.其次,要用空格隔开中英文字符,TEX才能正确分行.CJK宏包重新定义了符号~的意义.~的原始定义是不可断行的空格,CJK将其重定义成一个比较合适的弹性距离,大约相当于四分之一个汉字宽度,用于分隔中英文字符.我们通过使用命令CJKtilde来实现~的重定义.如果要恢复原始定义,可以用命令standardtilde注意:在CJK*模式下中文字符后面的空格会被忽略,因此中文字符后面必须加上保护的空格 或者~符号来隔开英文字符.
4.5 生成支持中文拷贝粘贴的PDF文件生成可以拷贝粘贴中文的PDF文件目前只有dvipdfmx(原名dvipdfm-cjk)可以做到,它必须配置成直接使用中文TrueType字库.
4.6 CJK中使用的字库可以使用矢量字库如Type1或是TrueType字库,也可以使用点阵字库如pk字库.在CJK中可以使用Type1字库和TrueType字库,都能产生很好效果的输出,但有许多细微的差别,输出结果取决于所用的工具.
1. 使用dvips生成PostScript 文件dvips可以直接使用Type1字库,生成的PostScript文件中包含了字符的矢量描述,具有很好的缩放效果.dvips不能直接使用TrueType字库,因此必须通过ttf2pk生成中间的pk字库,这是一种点阵字库,所以生成的PostScript文件在效果较差.
2. 使用dvipdfm生成PDF文件和dvips类似,可以直接使用Type1字库或者通过ttf2pk间接使用TrueType字库.
3. 使用pdfLaTeX生成PDF文件可以直接使用两种字库,因此都可以产生效果很好的输出.
4. 使用dvipdfmx生成PDF文件可以直接使用两种字库,因此都可以产生效果很好的输出.而且直接使用TrueType字库时生成的PDF文件是唯一能支持对中文内容进行正常拷贝粘贴的.(推荐使用)
5 帮助文档
5.1 宏包文档TEX系统安装时候带的LaTeX宏包,都可以在TEX 的安装目录下的texmfdocLaTeX找到
5.2 参考书LaTeX: A Document Preparation SystemLaTeX for Word Processor Userslshort (The Not So Short Introduction to LaTeX2e)A Gentle Introduction to TEXLaTeX2插图指南LaTeX2e for class and package writersThe TeX BookLaTeX Manual
5.3 查询LaTeX命令和参数的缺省定义首先你可以直接搜索相应的LaTeX宏包以及同一目录下的其他文件.如果你有宏包的源文件(.dtx) 的话,搜索源文件会更方便些.而且源文件中会有详细的说明.对于这些源文件,可以直接用LaTeX 编译,产生说明文档.LaTeX宏包的源文件一般放在texmfsourceLaTeX目录下.例如LaTeX标准文档类的源文件就是texmfsourceLaTeXbaseclasses.dtx如果这些文件里都找不到定义,则有可能在LaTeX内核中定义.你可以搜索texmfsourceLaTeXbase目录下的其他.dtx文件,所有的LaTeX内核定义都在那里了.如果还没有的话,那就可能是TEX 的基本命令或者定义了,查询Donald E. Knuth的The TEXbook,CTEX FTP上可以下载该书的源文件.
5.4 在ctex论坛[http://bbs.ctex.org](http://bbs.ctex.org/)查找资料6 排版查阅参考资料.7 LaTeX辅助工具使用辅助工具可以方便LaTeX的编写工作.公式编辑器:TeXaide这个是MathType的TeX专用版本.它的使用方法和Word中的公式编辑器一样.[http://www.dessci.com/en/products/texaide/](http://www.dessci.com/en/products/texaide/)图形工具:metagraf可以生成MetaPost格式图型的制图工具.[http://w3.mecanica.upm.es/metapost/metagraf.phpImageMagick](http://w3.mecanica.upm.es/metapost/metagraf.phpImageMagick)多功能图形工具,可以转换图片格式把其他格式转存成eps格式.[http://www.imagemagick.org/](http://www.imagemagick.org/)制表工具:tablas用图形界面制表,再把表格转换成LaTeX编码.[http://www.informatica.us.es/~calvo/latex.html](http://www.informatica.us.es/~calvo/latex.html)




转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-40740.html](http://blog.sciencenet.cn/blog-81613-40740.html)

上一篇：[数学公式的英文读法&数学英文词汇](blog-81613-40739.html)
下一篇：[常用数学模型及建模方法](blog-81613-40741.html)


