# GitHub常见错误 - weixin_33985507的博客 - CSDN博客
2018年10月24日 22:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
## 1、git remote add origin [git@github.com](mailto:git@github.com):WadeLeng/hello-world.git 报错
- 错误提示：fatal: remote origin already exists.
- 解决办法：git remote rm origin
- 然后再执行：$ git remote add origin [git@github.com](mailto:git@github.com):WadeLeng/hello-world.git 就不会报错误了
## 2、git push 提交后报403错误
- 网上都说是要修改项目目录下的隐藏文件 .git里面的config文件，但本人试过，行不通；
- 403主要原因是权限不足，不能访问；
解决方法:在github官网的个人账号要验证邮箱，如果不验证的话，就会提交push时报403的错误。
## 3、 使用git push -u 远程库名 master 命令将本地提交的内容传到git远程库时出现错误，如图示：
![12233994-4866cecdafb663c4.jpg](https://upload-images.jianshu.io/upload_images/12233994-4866cecdafb663c4.jpg)
原图.jpg
解决方法：使用 git push -f 命令重新传一遍即可；
结果显示：
![12233994-6252935d5f2655e1.png](https://upload-images.jianshu.io/upload_images/12233994-6252935d5f2655e1.png)
正确.png
