# Qt错误解决方案 no ducments matching - DumpDoctorWang的博客 - CSDN博客





2017年06月24日 18:40:12[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1051








##         no ducments matching 'ui_fileDialog.h' could be found. Rebuilding the project might help


今天在编写一个Qt程序时，由于新建界面时打错了一个字母，然后就重命名程序文件，在头文件中批量替换的时候把**ui_filedialog.h**替换成了**ui_fileDialog.h**，导致在新建槽的时候出现了标题所示错误，解决方案是，将ui_fileDialog.h替换成ui_filedialog.h，也就是将大写字母换为小写字母。



