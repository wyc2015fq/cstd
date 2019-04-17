# Intel Edison ssh connect refuse的错误 - V__KING__的专栏 - CSDN博客





2015年12月15日 19:03:43[v__king__](https://me.csdn.net/V__KING__)阅读数：1109
个人分类：[linux](https://blog.csdn.net/V__KING__/article/category/1733387)









[https://communities.intel.com/message/259141](https://communities.intel.com/message/259141)

vi /lib/systemd/system/sshd.socket

To manually remove the SSH restriction, edit the file ‘/lib/systemd/system/sshd.socket’ and prepend a ‘#’ character to the line containing ‘BindToDevice=usb0’ so that it looks like ‘# BindToDevice=usb0’.  The ‘#’ converts the line into a comment and will enable SSH access on all interfaces.



