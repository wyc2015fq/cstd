# George Moody 对OSEA 软件的修改 - 工作笔记 - CSDN博客





2012年01月17日 14:10:12[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5407








[http://www.eplimited.com/00README.txt](http://www.eplimited.com/00README.txt)

网址

George B. Moody ----修改者

Harvard-MIT Division of Health Sciences and Technology---- 哈佛-麻省理工卫生科学与技术部

MIT Room E25-505A 

Cambridge, MA 02139 USA 

file: 00README           G. Moody          10 April 2003 -----文件名，修改者，修改日期



The source files in this directory can be used to compile 'easytest' (a program that annotates a batch of ECG records) and 'bxbep' (a program that derives performance statistics from the output of 'easytest', based on 'bxb' from the WFDB Software Package;
 note that 'bxbep' has been renamed to avoid confusion with the original 'bxb'). 

 这个目录中的源码文件可用来编译‘easytest’（注释一批ECG记录的程序）和‘bxbep’（从‘easytest’程序的输出推出性能统计的程序，‘bxbep’基于WFDB软件包中的‘bxb’程序；注意到重新命名为‘bxbep’是避免和原始的‘bxb’混淆）



These source files are based on those contained in osea20.zip, with minor modifications needed to compile them using gcc. They have been tested under GNU/Linux and under MS-Windows with Cygwin; they should work on any other platform for which gcc is available,
 including Mac OS/X (Darwin) and other versions of Unix. 

 这些源代码文件是基于osea20.zip中包含的文件，为利用gcc编译它们做了很小的修改。它们已经在GNU/Linux 环境下、在MIS-Windows的Cygwin环境下被测试通过；它们应该能够在可以使用gcc的任何其他平台上工作，包括Mac OS/X(Darwin) 和Unix的其它版本



To compile these sources and run the programs: 

为编译这些代码并运行这个程序：



1. Install gcc if you have not already done so. Under MS-Windows, install the Cygwin software package available freely from http://www.cygwin.com/. Under Mac OS/X, install the Mac OS/X Developer Tools, available freely from
[ttp://developer.apple.com/macosx/](http://developer.apple.com/macosx/).


 第1步：安装gcc. 在MS-Windows环境下，安装Cygwin软件包，可从网站[http://www.cygwin.com/](http://www.cygwin.com/)免费获取。在Mac OS/X环境下，安装Mac OS/X 开发工具，可从网站[ttp://developer.apple.com/macosx/](http://developer.apple.com/macosx/). 免费获取。



2. Download the sources for the WFDB Software Package, available freely from http://www.physionet.org/physiotools/wfdb.shtml. Follow the instructions there to compile and install the WFDB software using gcc.


 第2步：下载WFDB软件波源代码，可从网站 [http://www.physionet.org/physiotools/wfdb.shtml](http://www.physionet.org/physiotools/wfdb.shtml) 免费获得。按照那里的说明用gcc编译和安装WFDB软件包



3. Unpack the osea 2.0 software by typing: 

tar xfvz osea20-gcc.tar.gz  --------------------命令行，用来解压osea2.0软件

This creates a directory called osea20-gcc, containing the sources for 'easytest' and 'bxbep'.


这个命令行创建了一个名为osea20-gcc的目录，包含供‘easytest’和‘bxbep’使用的源代码

Enter this directory by typing:

 cd osea20-gcc  ---------------------命令行，进入osea20-gcc目录



4. The sources are set up by default so that easytest and bxbep will read the MIT-BIH Arrhythmia Database files directly from PhysioNet (you don't need to have local copies of these files). If you do have local copies, be sure that they are in the current
 directory.

 若使用源代码默认设置，那么easytest和bxbep将直接从PhysioNet网站读取MIT-BIH心律失常数据库文件（你不需要有这些文件的本地拷贝）。如果你确实有这些文件的本地拷贝，确保它们被放置在当前目录中



 If you want to run 'easytest' and 'bxbep' on a different set of records, edit 'inputs.h' to comment out the definition of PHYSIOBANK near the beginning, and define either MITDB or AHADB as you wish. You can also define your own set of records following
 the same model. You will need to recompile (next step) before any changes you make in 'inputs.h' are effective.


 如果你想对一组不同的记录运行‘easytest’和‘bxbep’，编辑‘input.h’文件注释掉起始位置附近的PHYSIOBANK的定义，并且按你的意愿定义其它的MITDB或AHADB。你也可以按照同样的模式定义你自己的记录集合。你将需要重新编译（下一个步骤）从而使你在‘input.h’中做的修改起作用



5. Compile the sources by typing: 

make   ---------------------------编译源代码

There should be no compilation errors or warnings.  

不应该有任何编译错误或警告 



The file named 'Makefile' controls what happens when you run these 'make' commands. 'Makefile' is a text file that you can edit if necessary.


 当你运行‘make’命令时名为‘Makefile’的文件控制发生的事情。‘Makefile’是一个文本文件，必要时你可以编辑它



6. Run the osea 2.0 software by typing:

 ./easytest        [or './easytest2']      -------------运行osea2.0软件

./bxbep                                           -------------运行统计程序     

There should be no run-time errors or warnings. ---------不该有运行错误或警告



'easytest2' is a version of 'easytest' that includes 'qrsdet2' rather than the original 'qrsdet'. It is otherwise identical to 'easytest'.


 ‘easytest2’是‘easytest’的改版，包含‘qrsdet2’而不包含原来的‘qrsdet’，在其他方面与‘easytest’相同



Note that 'easytest' (or 'easytest2') may take a long time to run if it is reading data directly from PhysioNet and if your connection to the PhysioNet master server at MIT is slow.


 注意到，如果直接从PhysioNet读取数据而且你与在MIT的PhysioNet主服务器的链接很慢的话，‘easytest’（或‘easytest2’）可能运行很长时间



The output of 'easytest' is a set of annotation files with the suffix (annotator name) 'atest', one for each record processed.

‘easytest’程序的输出是一组注释文件，带有‘atest’后缀（注释器名），每一个处理的记录带一个注释文件



 The output of 'bxbep' is a pair of files (adtstat.txt and testrpt.txt) that are described in the osea documentation available from EP Limited.


‘bxbep’程序的输出是一对文件（adtstat.txt 和testrpt.txt），osea文档中有对它们的描述，文档可以从EP Limited 获得。



All of the output files are written to the current directory. 

所有输出文件被写入到当前目录中。



