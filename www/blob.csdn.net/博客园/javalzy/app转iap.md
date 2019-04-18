# app转iap - javalzy - 博客园
# [app转iap](https://www.cnblogs.com/javalzy/p/8398274.html)
# ios打包ipa的四种实用方法(.app转.ipa)
http://blog.csdn.net/oiken/article/details/49535369
手动压缩改后缀方式
这种方式与4.1的方法本质是一样的。
Step1: 新建“Payload”文件夹，注意名字要一字不差；
Step2: 将你的.app包放到Payload中，注意app的名字不做任何更改，就用xcode生成的app名称；
Step3: 在Payload文件夹上右键压缩成zip，然后将生成的.zip文件后缀改成.ipa即可

