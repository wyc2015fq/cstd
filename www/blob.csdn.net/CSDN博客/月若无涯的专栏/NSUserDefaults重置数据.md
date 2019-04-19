# NSUserDefaults重置数据 - 月若无涯的专栏 - CSDN博客
2015年05月19日 19:06:41[月若无涯](https://me.csdn.net/u010124617)阅读数：1595
最近遇到一个很奇怪的问题，通常情况下，重置NSUserDefaults使用的方法为：
`[NSUserDefaults resetStandardUserDefaults];`
但是实际使用中偶尔出现了重置不成功的情况，因此另辟蹊径，改为遍历UserDefaults的所有属性后逐条删除，代码如下：
```
NSDictionary *dic = [[NSUserDefaults standardUserDefaults]dictionaryRepresentation];
    for (id key in [dic allKeys]) {
        [[NSUserDefaults standardUserDefaults]removeObjectForKey:key];
    }
    [[NSUserDefaults standardUserDefaults]synchronize];
```
看起来没上面那种方便，但是胜在简单粗暴直接有效！
