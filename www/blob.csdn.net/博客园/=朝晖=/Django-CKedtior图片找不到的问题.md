# Django-CKedtior图片找不到的问题 - =朝晖= - 博客园
# [Django-CKedtior图片找不到的问题](https://www.cnblogs.com/dhcn/p/7105552.html)
       从Django Packages站点上找到这个CKeditor集成组件:https://github.com/shaunsephton/django-ckeditor
       按照官方的install方法安装完发现上传完页面后在服务器端浏览页面发现显示如下：
            Browse for the image you want, then click 'Embed Image' to continue...
      后来经过调试发现：原来是无法正常加载[js](http://lib.csdn.net/base/javascript)和CSS资源所以图片目录显示不出来，到这个APP的模板目录里面发现：模板好像无法正常得到Setting设置离得STATIC_URL这个设置变量，所以导致出来的URL路径不正确，于是手动把模板里面的JS和CSS的URL路径修改了一下。终于可以看到图片了。

