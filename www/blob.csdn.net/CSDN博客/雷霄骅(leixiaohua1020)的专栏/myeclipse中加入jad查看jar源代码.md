# myeclipse中加入jad查看jar源代码 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月24日 17:21:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：25
个人分类：[J2EE](https://blog.csdn.net/leixiaohua1020/article/category/1362940)










1、 下载反编译工具jad

 a) 下载地址：[http://varaneckas.com/jad/](http://varaneckas.com/jad/)，根据相对操作界面的版本进行下载，因为我们的操作界面是window所以下载：jad 1.5.8 for Window 9x/NT/2000即可。或是使用本文件夹中已下载的jad/jadnt158.zip

 b) 将下载下来的jadnt158.zip解压缩到任意目录，只要记住就行，这里我们将解压缩出来的jad.exe文件放入%JAVA_HOME%\bin文件目录下。

 2、 下载eclipse反编译插件jadclipse

 a) 下载地址：[http://sourceforge.net/projects/jadclipse/](http://sourceforge.net/projects/jadclipse/)，根据eclipse的版本进行下载，这里注意了，不是MyEclipse的版本，我们用的MyEclipse5.1是自动装载eclipse，而它的版本可以在help中查看，在这里它的版本为eclipse3.2所以我们就要下载jadclipse3.2，或是net.sf.jadclipse_3.2.4.jar

 b) 因为是jadclipse3.2的版本，所以可直接将net.sf.jadclipse_3.2.4.jar复制到%eclipse_home%\plugins下，而这里我们的目录为C:\Program Files\MyEclipse Enterprise Workbench 5.1.0 GA\eclipse\plugins下

 3、 重启eclipse

 4、 配制jadclipse

 a) Window/Preferences/java/jadClipse中的Path to decomiler设置为jad.exe的决对路径，刚才我们给它放到了我们jdk的bin中，为C:\j2sdk1.4.2\bin\jad.exe。Directory for temporary files设置为存放临时文件的决对路径这里我把它设置到桌面上了C:\Documents and Settings\LuckyStar\桌面\.net.sf.jadclipse

 b) General/Editors/File Associations中把*.class的Associated Editors设置为JadClipse Class File Viewer为默认。如果没有jadclipse则说明net.sf.jadclipse_3.2.4.ja没有复制到正确的位置

 5、 可以试一下双击class文件，看能否反编译成java代码了

 注：

 jad是命令行式的反编译程序，将class文件反编译java操作的工具，在命令行中执行操作，如下说明几个简单的反编译操作：

 将当前文件夹下的abc.class文件反编译成abc.jad文件

 D：\jad>jad abc.class

 将当前文件夹下的abc.class文件反编译成abc.java文件

 D：\jad>jad -sjava abc.class

 可以单独使用。Jadclipse是eclipse中的一个插件，是用于在eclipse中双击*.class文件时直接查它的*.java代码，不可以单独使用。




