# Unix目录结构的来历 - 文章 - 伯乐在线
原文出处： [阮一峰（ @ruanyf ）](http://www.ruanyifeng.com/blog/2012/02/a_history_of_unix_directory_structure.html)
Unix（包含Linux）的初学者，常常会很困惑，不明白目录结构的含义何在。
![](http://jbcdn2.b0.upaiyun.com/2016/10/31f1619b48ce451b825081c9b656fa63.jpg)
举例来说，根目录下面有一个子目录/bin，用于存放二进制程序。但是，/usr子目录下面还有/usr/bin，以及/usr/local/bin，也用于存放二进制程序；某些系统甚至还有/opt/bin。它们有何区别？
长久以来，我也感到很费解，不明白为什么这样设计。像大多数人一样，我只是根据《Unix文件系统结构标准》（Filesystem Hierarchy Standard），死记硬背不同目录的区别。
昨天，我读到了Rob Landley的简短解释，这才恍然大悟，原来Unix目录结构是历史造成的。
话说1969年，Ken Thompson和Dennis Ritchie在小型机PDP-7上发明了Unix。1971年，他们将主机升级到了PDP-11。
![](http://jbcdn2.b0.upaiyun.com/2016/10/a3eae2c9ba432d7d763d4f465d5db8d7.jpg)
当时，他们使用一种叫做RK05的储存盘，一盘的容量大约是1.5MB。
![](http://jbcdn2.b0.upaiyun.com/2016/10/615964214d20733d521d3909619429ec.jpg)
没过多久，操作系统（根目录）变得越来越大，一块盘已经装不下了。于是，他们加上了第二盘RK05，并且规定第一块盘专门放系统程序，第二块盘专门放用户自己的程序，因此挂载的目录点取名为/usr。也就是说，根目录”/”挂载在第一块盘，”/usr”目录挂载在第二块盘。除此之外，两块盘的目录结构完全相同，第一块盘的目录（/bin, /sbin, /lib, /tmp…）都在/usr目录下重新出现一次。
后来，第二块盘也满了，他们只好又加了第三盘RK05，挂载的目录点取名为/home，并且规定/usr用于存放用户的程序，/home用于存放用户的数据。
从此，这种目录结构就延续了下来。随着硬盘容量越来越大，各个目录的含义进一步得到明确。
**/**：存放系统程序，也就是At&t开发的Unix程序。
**/usr**：存放Unix系统商（比如IBM和HP）开发的程序。
**/usr/local**：存放用户自己安装的程序。
**/opt**：在某些系统，用于存放第三方厂商开发的程序，所以取名为option，意为”选装”。
