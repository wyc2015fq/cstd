# iOS10 Keychain 使用注意事项 - 月若无涯的专栏 - CSDN博客
2016年08月15日 17:13:45[月若无涯](https://me.csdn.net/u010124617)阅读数：7335
    Keychain是iOS中一直用于存储重要信息，或者删除应用后仍然想保留的数据的地方（比如存储UUID）。 
    今天在测试iOS10 iPad情况下时，发现数据居然无法写入，而iPhone和之前版本的iPad则都是正常的，目前还不能确认这是否是bug。 
    最后经过检查，在Capabilities中打开Keychain sharing即可 
![keychain设置](https://img-blog.csdn.net/20160815171007740)
    同时在StackOverflow上找到了其他的提问者和答案。链接如下： 
[http://stackoverflow.com/questions/38812761/gidsignin-keychain-error-ios-10-xcode-8](http://stackoverflow.com/questions/38812761/gidsignin-keychain-error-ios-10-xcode-8)
[http://stackoverflow.com/questions/38689631/how-to-use-facebook-ios-sdk-on-ios-10/38799196#38799196](http://stackoverflow.com/questions/38689631/how-to-use-facebook-ios-sdk-on-ios-10/38799196#38799196)
    对于这种情况还是表示很奇怪的，因为Keychain sharing原意是用于不同应用之间共享Keychain数据而设置的。不知道后续iOS10正式版本会怎么样了。保持关注。
