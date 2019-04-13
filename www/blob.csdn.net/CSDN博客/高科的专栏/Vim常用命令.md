
# Vim常用命令 - 高科的专栏 - CSDN博客

2011年12月02日 18:12:39[高科](https://me.csdn.net/pbymw8iwm)阅读数：2016标签：[vim																](https://so.csdn.net/so/search/s.do?q=vim&t=blog)[search																](https://so.csdn.net/so/search/s.do?q=search&t=blog)[buffer																](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)[tags																](https://so.csdn.net/so/search/s.do?q=tags&t=blog)[浏览器																](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)[microsoft																](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=tags&t=blog)个人分类：[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)
[
																								](https://so.csdn.net/so/search/s.do?q=tags&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=search&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=vim&t=blog)
多窗口模式|:split
|创建新窗口
|
|ctrl + w
|切换
|
|Ctrl-w =
|所有窗口一样高
|
|Ctrl-w+方向键
|多个视窗间切换
|
2、标记
|ma
|在光标所在处做一个名为|a|的标记。最多可以在文本中做|26|个标记|(a-z)
|
|`标记
|移动到一个标记
|
|'标记
|移动到一个标记所在行的行首
|
|:marks
|查看所有标记
|
|d`a
|删除从当前位置到标记|a|间的内容
|
|``
|回到先前编辑位置
|
3、多文件编辑
|:next
|打开多个文件，用|n(ext)|，|p(revious)|｜|N(ext)|切换
|
|:files
|列出当前打开的所有文件
|
|:args
|正在编辑的文件名
|
4、其他快捷操作
|H、|M|、|L
|移动到屏幕内上中下三行；|"H"|表示|"Home"|，|"M"|表示|"Middle"|而|"L"|表示|"Last"|。
|
|ZZ
|保存退出
|
|*
|光标置于变量按|*|，再按则向下查找次变量出现的地方
|
|\#
|向上查找整字，和|*|相反
|
|:jumps
|曾经编辑过的记录
|
|xp
|交换字符
|
|nw
|按词前行，|n|为数字，省略为|1|；|nb
|
|dw
|删除一下单词
|
|d$
|删除从当前位置到行末的所有内容；反之，|d0
|
|J
|合并行
|
|~
|更改当前光标所在位置字符的大小写
|
5、~/.viminfo记录vi操作信息，/etc/vimrc放置vi设置信息。
6、vi其他设置
|:set hlsearch
|查找高亮显示，|nohlsearch
|
|:set incsearch
|增量查找，|noincsearch|，|n|N|查找下一个｜上一个
|
|:set nu
|显示行号，|nonu
|
|set autoindent
|自动缩排，|回车时与上一行的第一个非空格符处对齐
|
|set backspace=(012)
|2|：|可以删除任意值；|0|或|1|：|仅可删除刚刚输入的字符，而无法删除原本已经存在的文字
|
|set
|显示自己设定的参数
|
|set all
|
|:syntax on | off
|依据程序语法显示不同颜色
|
|:set bg=dark
|:set bg=light
|显示颜色色调
|
7、进行命令行模式
/ 为查询模式
： 为写命令模式
8、退出
|w
|保存
|
|w
|filename另存为|filename
|
|n1,n2 w filename
|把|n1,n2|间的内容另存为|filename
|
|w!
|只读时强制写入
|
|q
|退出
|
|q!
|不可在退出
|
|wq
|保存退出
|
|x
|保存退出
|
|wq!
|强制保存退出
|
|:!
|command暂时离开|vi|到指令模式下执行|command
|
9、可视模式
|v
|字符选择，会将游标经过的地方反白选择
|
|V
|行选择，会将游标经过的行反白选择
|
|c|trl+v
|行可视，|区块选择，可以用长方形的方式选择资料
|
|shift+v
|列可视
|
|y
|将反白的地方复制|，|p|粘贴
|
|d
|将反白的地方删除|，|D|删除该行
|
|J(大写|)
|合并选中的行|为一行
|
|>
|选中的部分缩进|tab
|
|=
|选中的部分删除|tab
|
|K
|选中的部分查看|man page
|
|r
|选中的字符替换成同一字符
|
|c
|删除所选中的部分并切换到输入模式
|
|A str
|会在选中的块后面添加文本|,|它们是对齐的|<esc>
|
|I str
|会在每一行所选中内容的开始前添加字符串|<esc>
|
10、其他常用命令
f c ：定位字符c的前一个字符，行上查找，F为向左
t c：字位字符c的前一个字符，只定位一次
set nu
2G //跳到第2行
ctrl + G //显示当前的%
70% //跳到70%的地方


