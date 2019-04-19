# iOS6.1下secureTextEntry失效的问题 - 月若无涯的专栏 - CSDN博客
2014年07月25日 14:40:57[月若无涯](https://me.csdn.net/u010124617)阅读数：1367
今天做UITextField的密码显示功能时，有个功能是按下一个按钮密码变明文，松开又恢复成星号，结果发现一个奇怪的问题，
当显示成明文后，iOS6.1下调用textField.secureTextEntry=true居然无效，无法恢复成星号。而iOS7下则正常，怀疑是6.1系统的Bug。
最后在StackOverflow找到了答案：[http://stackoverflow.com/questions/23782671/ios-uitextflied-securetextentry-not-working-on-ios-6-1/23782982#23782982](http://stackoverflow.com/questions/23782671/ios-uitextflied-securetextentry-not-working-on-ios-6-1/23782982#23782982)
对于6.x的系统，使用如下方式改变secureTextEntry即可。
    textField.enabled = false;//iOS6.1Bug fix
    textField.secureTextEntry = true;
    textField.enabled = true;
