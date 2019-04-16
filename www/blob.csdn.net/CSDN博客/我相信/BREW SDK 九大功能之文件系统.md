# BREW SDK 九大功能之文件系统 - 我相信...... - CSDN博客





2010年04月06日 11:42:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1243








BREW支持全面的文件系统I/O操作，对文件或目录的访问。BREW3.X之前的BREW应用程序只能对应用自身目录下的文件或子目录的访问，或者对共享目录下的文件或子目录进行访问。BREW3.X引入 "fs:/" 作为文件前缀，是区别之前版本的标志，“fs:/” 代表BREW的根目录，扩大了应用访问范围，支持MMC/SD/Flash等外存卡的访问，"fs:/card0/" 代表外存卡的目录，“fs:/~/” 代表应用模块本身所在目录，如图11-2所示。





图11-2： 文件系统中的目录结构



BREW文件系统的文件名大小写敏感的，下面是BREW文件系统中的常见目录：

"fs:/"                (AEEFS_ROOT_DIR)

"fs:/~/"                         (AEEFS_HOME_DIR): mapped to "fs:/mod/<modid>"

"fs:/sys/"                      (AEEFS_SYS_DIR)

"fs:/mod/"        (AEEFS_MOD_DIR)

"fs:/mif/"                      (AEEFS_MIF_DIR)

"fs:/shared/"      (AEEFS_SHARED_DIR)

"fs:/address/"    (AEEFS_ADDRESS_DIR)

"fs:/ringers/"     (AEEFS_RINGERS_DIR)

"fs:/card0/"       (AEEFS_CARD0_DIR)

"fs:/~<clsid>/"

不带“fs:/”前缀的文件名仍然按照以前版本的规则处理，文件名中的大写字母自动变为小写，(“/”) 变换为 ("/") ，应用所在目录作为根目录。通过IFILEMGR_ResolvePath()  将传入的路径转为BREW标准路径(fs:/)。

BREW3.x文件系统的一大特色就是引入了ACL功能，使将本应用模块的目录共享给其它应用访问。通过在MIF中设置，来实现共享功能。共享给其它应用的目录，其根目录”/”代表本应用模块所在目录，而不是BREW文件系统根目录 “fs:/”，即在ACL中，应用只能共享自己模块目录的权限，没有共享其它应用目录的权限。关于MIF文件编辑管理讲座下一章给予说明。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/03/31/5435852.aspx](http://blog.csdn.net/wireless_com/archive/2010/03/31/5435852.aspx)



