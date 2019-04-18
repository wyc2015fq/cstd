# 疑难杂症小记 - POCO的Android编译 - tkokof1的专栏 - CSDN博客

2018年01月17日 10:34:22[tkokof1](https://me.csdn.net/tkokof1)阅读数：359


想编译下[POCO](https://pocoproject.org/)的Android版本,发现[官方](https://pocoproject.org/docs/99300-AndroidPlatformNotes.html)只支持Linux或者Mac OS X,简单尝试了下cygwin,发现NDK的Windows版本工具链因为使用Windows风格的[文件路径](https://en.wikipedia.org/wiki/Path_%28computing%29),所以基本不能与cygwin配合使用,遂而索性装了一个Ubuntu,前期准备工作完毕后,编译却总是失败:
`'...arm-linux-androideabi/bin/as': execv: Exec format error`
本来以为是as文件没有运行权限的问题,但是尝试了 chmod +x as 之后发现仍然失败,困惑了很长时间,最后终于在SO上找到了[答案](https://stackoverflow.com/questions/21434819/android-ndk-sample-build-error-linux),原来在NDK的Linux版本中,as文件(以及同目录下的一些文件)是Linux中的链接文件,在Windows中解压后却变成了文本文件(怀疑链接文件在tar打包时有特殊处理,譬如在文件内容之外标记了类型之类的,具体细节暂不清楚,了解的同学麻烦告知一下~),于是在尝试执行文本文件(as)之时便报了上面的错误~

解决方法也挺简单,直接在Linux中解压NDK即可~

