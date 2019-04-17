# linux下移动一个文件 - miner_zhu的博客 - CSDN博客





2018年08月03日 14:26:22[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：503








用mv命令
1.作用
mv命令来为文件或目录改名或将文件由一个目录移入另一个目录中。该命令等同于DOS系统下的ren和move命令的组合。它的使用权限是所有用户。
2.格式
mv [options] 源文件或目录 目标文件或目录
3.[options]主要参数
－i：交互方式操作。如果mv操作将导致对已存在的目标文件的覆盖，此时系统询问是否重写，要求用户回答”y”或”n”，这样可以避免误覆盖文件。
－f：禁止交互操作。mv操作要覆盖某个已有的目标文件时不给任何指示，指定此参数后i参数将不再起作用。
4.第二个参数
当第二个参数类型是文件时，mv命令完成文件重命名，它将所给的源文件或目录重命名为给定的目标文件名。
当第二个参数是已存在的目录名称时，源文件或目录参数可以有多个，mv命令将各参数指定的源文件均移至目标目录中。在跨文件系统移动文件时，mv先拷贝，再将原有[文件删除](https://www.baidu.com/s?wd=%E6%96%87%E4%BB%B6%E5%88%A0%E9%99%A4&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dbrj9bP1u9nW0dnyn1Pvf0IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtkP1b3n1cYnWcYPjRdrHb4rHfL)，而链至该文件的链接也将丢失。
5.应用实例
(1)将/usr/udt中的所有文件移到[当前目录](https://www.baidu.com/s?wd=%E5%BD%93%E5%89%8D%E7%9B%AE%E5%BD%95&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dbrj9bP1u9nW0dnyn1Pvf0IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtkP1b3n1cYnWcYPjRdrHb4rHfL)(用”.”表示)中：
$ mv /usr/udt/* .
(2)将文件test.txt重命名为wbk.txt：
$ mv test.txt wbk.txt

（3）把[当前目录](https://www.baidu.com/s?wd=%E5%BD%93%E5%89%8D%E7%9B%AE%E5%BD%95&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dbrj9bP1u9nW0dnyn1Pvf0IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtkP1b3n1cYnWcYPjRdrHb4rHfL)的一个子目录里的文件移动到另一个子目录里

       mv  文件名/*  另一个目录

（4）移动当前文件夹下的所有文件到上[一级目录](https://www.baidu.com/s?wd=%E4%B8%80%E7%BA%A7%E7%9B%AE%E5%BD%95&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dbrj9bP1u9nW0dnyn1Pvf0IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtkP1b3n1cYnWcYPjRdrHb4rHfL)

       mv * ../
cp命令是用于复制的。
cp(copy) 复制文件或目录。
cp参数：
-a或–archive 　此参数的效果和同时指定”-dpR”参数相同。
-b或–backup 　删除，覆盖目标文件之前的备份，备份文件会在字尾加上一个备份字符串。
-d或–no-dereference 　当复制符号连接时，把目标文件或目录也建立为符号连接，并指向与源文件或目录连接的原始文件或目录。
-f或–force 　强行复制文件或目录，不论目标文件或目录是否已存在。
-i或–interactive 　覆盖既有文件之前先询问用户。
-l或–link 　对源文件建立硬连接，而非复制文件。
-p或–preserve 　保留源文件或目录的属性。
-P或–parents 　保留源文件或目录的路径。
-r 　递归处理，将指定目录下的文件与子目录一并处理。
-R或–recursive 　递归处理，将指定目录下的所有文件与子目录一并处理。
-s或–symbolic-link 　对源文件建立符号连接，而非复制文件。
-S<备份字尾字符串>或–suffix=<备份字尾字符串> 　用”-b”参数备份目标文件后，备份文件的字尾会被加上一个备份字符串，预设的备份字尾字符串是符号”~”。
-u或–update 　使用这项参数后只会在源文件的更改时间较目标文件更新时或是　名称相互对应的目标文件并不存在，才复制文件。
-v或–verbose 　显示指令执行过程。
-V<备份方式>或–version-control=<备份方式> 　用”-b”参数备份目标文件后，备份文件的字尾会被加上一个备份字符串，这字符串不仅可用”-S”参数变更，当使用”-V”参数指定不同备份方式时，也会产生不同字尾的备份字串。
-x或–one-file-system 　复制的文件或目录存放的文件系统，必须与cp指令执行时所处的文件系统相同，否则不予复制。
–help 　在线帮助。
–sparse=<使用时机> 　设置保存稀疏文件的时机。
–version 　显示版本信息。


