# 解决E: Encountered a section with no Package: header错误 - shine302的博客 - CSDN博客
2018年04月24日 09:28:22[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：261
 解决E: Encountered a section with no Package: header错误
    我的ubuntu机器上出现下面这个错误。
    Reading package lists... Error!
    E: Encountered a section with no Package: header
    E: Problem with MergeList /var/lib/apt/lists/ftp.sjtu.edu.cn_[ubuntu](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.shangxueba.com%2Fjingyan%2F1643175.html&p=baidu&c=news&n=10&t=tpclicked3_hc&q=csai_cpr&k=ubuntu&k0=apt&kdi0=1&k1=status&kdi1=1&k2=ubuntu&kdi2=8&k3=problem&kdi3=8&k4=%BB%A5%C1%AA%CD%F8&kdi4=8&sid=4bd4b2ee6a6668f&ch=0&tu=u1730417&jk=0c3d59d1669fd56c&cf=29&fv=11&stid=9&urlid=0&luki=3&seller_id=1&di=128)_dists_precise-security_restricted_binary-i386_Packages
    E: The package lists or [status](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.shangxueba.com%2Fjingyan%2F1643175.html&p=baidu&c=news&n=10&t=tpclicked3_hc&q=csai_cpr&k=status&k0=apt&kdi0=1&k1=status&kdi1=1&k2=ubuntu&kdi2=8&k3=problem&kdi3=8&k4=%BB%A5%C1%AA%CD%F8&kdi4=8&sid=4bd4b2ee6a6668f&ch=0&tu=u1730417&jk=0c3d59d1669fd56c&cf=29&fv=11&stid=9&urlid=0&luki=2&seller_id=1&di=128) file could not be parsed or opened.
    虽然不知道是怎么回事，但是google出来的结果提示可以按如下方法解决，记录之：
    sudo rm /var/lib/apt/lists/* -vf
    sudo [apt](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.shangxueba.com%2Fjingyan%2F1643175.html&p=baidu&c=news&n=10&t=tpclicked3_hc&q=csai_cpr&k=apt&k0=apt&kdi0=1&k1=status&kdi1=1&k2=ubuntu&kdi2=8&k3=problem&kdi3=8&k4=%BB%A5%C1%AA%CD%F8&kdi4=8&sid=4bd4b2ee6a6668f&ch=0&tu=u1730417&jk=0c3d59d1669fd56c&cf=29&fv=11&stid=9&urlid=0&luki=1&seller_id=1&di=128)-get update
