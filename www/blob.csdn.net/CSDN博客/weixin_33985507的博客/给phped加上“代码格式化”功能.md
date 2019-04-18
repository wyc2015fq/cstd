# 给phped加上“代码格式化”功能 - weixin_33985507的博客 - CSDN博客
2012年01月02日 22:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**给phped加上代码格式化功能**
1.下载phpcb     [http://www.waterproof.fr/products/phpCodeBeautifier/](http://www.waterproof.fr/products/phpCodeBeautifier/)
2.解压缩后把phpcb.exe 放到phped的安装目录
3.打开phped Tools->Settings
找到 Integration 选择后在右侧点击 AddMenu 再出来的窗口中随便输入你想叫的名称 例如FormatCode
4.选择刚添加的菜单 右侧点击 Edit 在参数的窗口中按照如下配置即可
Execute with : shell
Command  line :　"C:\Program Files\NuSphere\PhpED\phpcB.exe"  --equal-align-position 50 --padding-char-count 4 --space-after-if  --space-after-switch --space-after-while --glue-amperscore  --force-large-php-code-tag --align-equal-statements  --comment-rendering-style PEAR --extra-padding-for-case-statement  --glue-arrow --optimize-eol --one-true-brace  --one-true-brace-function-declaration "@FName@"
其中C:\Program Files\NuSphere\PhpED 换成你的phped的安装路径
Shortcut : 这个是执行的快捷键 我自己设置的是 Alt+F
在窗口下面的Options 选项中 选择如下两项
Work with editor
Return results to editor
点击close关闭窗口保存
打开一个比较乱的php文件 格式化一下看看有没有效果

