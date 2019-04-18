# Java正则之贪婪匹配、惰性匹配 - z69183787的专栏 - CSDN博客
2017年01月04日 16:15:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1379
**贪婪量词：**
先看整个字符串是不是一个匹配。如果没有发现匹配，它去掉最后字符串中的最后一个字符，并再次尝试。如果还是没有发现匹配，那么    再次去掉最后一个字符串，这个过程会一直重复直到发现一个匹配或者字符串不剩任何字符。简单量词都是贪婪量词。
**惰性量词：**
先看字符串中的第一个字母是不是一个匹配，如果单独着一个字符还不够，就读入下一个字符，组成两个字符的字符串。如果还没有发现匹配，惰性量词继续从字符串中添加字符直到发现一个匹配或者整个字符串都检查过也没有匹配。惰性量词和贪婪量词的工作方式恰好相反。
**支配量词：**
只尝试匹配整个字符串。如果整个字符串不能产生匹配，不做进一步尝试。
    贪婪量词   惰性量词    支配量词                      描述
    -------------------------------------------------------------------------------------
      ?             ??             ?+                      可以出现0次或1次，但至多出现1次
      *             *?            *+                      可以出现任意次，也可以不出现
      +             +?            ++                      出现1次或多次，但至少出现1次
      {n}        {n}?           {n}+                   一定出现n次
      {n,m}    {n,m}?       {n,m}+               至少出现n次，但至多不能超过m次
      {n,}       {n,}?          {n,}+                 可以出现任意次，但至少出现n次
    例如：我们要从字符串abbbaabbbaaabbb1234中获得abbb，aabbb，aaabbb的匹配
**    1、贪婪量词**
复制代码代码如下:
       var regexp = /.*bbb/g;
        var a = str.match(regexp);
        alert(a.length);   //output:1
        alert(a[0]);       //output:abbbaabbbaaabbb
    贪婪量词的工作过程可以这样表示：
      a)abbbaabbbaaabbb1234
      b)abbbaabbbaaabbb123
      c)abbbaabbbaaabbb12
      d)abbbaabbbaaabbb1
      e)abbbaabbbaaabbb //true
    可以看到，贪婪量词在取得一次匹配后就会停止工作，虽然我们加了'g'(全局匹配)
**    2、惰性量词**
复制代码代码如下:
        var regexp = /.*?bbb/g;
        var a = str.match(regexp);
        alert(a.length);   //output:3
        alert(a[0]);       //output:abbb
        alert(a[1]);       //output:aabbb
        alert(a[2]);       //output:aaabbb
    惰性量词的工作过程可以这样表示：
      a)a
      b)ab
      c)abb
      d)abbb //保存结果，并从下一个位置重新开始
      e)a 
      f)aa 
      g)aab
      h)aabb
      j)aabbb //保存结果，并从下一个位置重新开始
      e)a
      e)aa
      e)aaa
      e)aaab 
      e)aaabb 
      e)aaabbb  //保存结果，并从下一个位置重新开始
    由于JS是不支持支配量词的，所以支配量词我们只能用JAVA来演示：
复制代码代码如下:
        String string = "abbbaabbbaaabbb1234";
        Pattern p = Pattern.compile(".*+bbb");
        Matcher m = p.matcher(string);
        System.out.println(m.find());   //output:false
    因为支配量词采用一刀切的匹配方式，如：
    a）abbbaabbbaaabbb1234 //false
