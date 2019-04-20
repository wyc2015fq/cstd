# 如何设置myeclipse编码 - 纯洁的微笑 - CSDN博客
2011年12月31日 16:01:28[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：20
个人分类：[杂项](https://blog.csdn.net/ityouknow/article/category/8364190)
                设置GBK的方法
1、windows->Preferences……打开"首选项"对话框，左侧导航树，导航到general->Workspace，右侧Text file encoding，选择Other，改变为gbk（如果下拉里面没有GBK，你可以点击那个下拉的地方，是可以手动输入的），以后新建立工程其属性对话框中的Text file encoding即为gbk. 
2、windows->Preferences……打开"首选项"对话框，左侧导航树，导航到general->Content Types，右侧Context Types树，点开Text中每一颗子项，并在中输入"gbk"，点update！ 其他java应用开发相关的文件如：properties、XML等已经由Eclipse缺省指定，分别为ISO8859-1，gbk，如开发中确需改变编码格式则可以在此指定。 
3、window——>preference——>MyEclipse——>Files and Editors，将每个子项的"Encoding"改为"ISO 10645/Unicode（gbk）"，点Apply！ 
4、经过上述三步，新建java文件即为gbk编码，Eclipse编译、运行、调试都没问题。            
