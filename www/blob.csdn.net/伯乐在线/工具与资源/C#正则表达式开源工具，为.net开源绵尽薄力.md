# C#正则表达式开源工具，为.net开源绵尽薄力 - 文章 - 伯乐在线
原文出处： [头头哥](http://www.cnblogs.com/toutou/p/regextool.html)
博客前言
> 
先交代一下背景，最近工作中经常用到正则表达式，而正则表达式这个东西我个人觉得很鸡肋，不用吧，有些功能实现起来会很麻烦。用吧，又不是说工作中经常用到，只是有时候有些需要求用到而已。但是正则表达式只要一段时间不用，就会被遗忘，甚至是忘的一干二净。为了一定程度上解决这个鸡肋的问题，就有了这篇博客和我打算写的这个开源的正则表达式验证工具的想法。关于正则网上的资料其实已经很多了，之所以还写这篇博客只是为了记录一些现在或者以后工作中可能会用到的正则，然后分享出来，同时也是希望为.net开源做一些事情。
## 0.写在前面
记得上一次正儿八经的用正则表达式还是三年前，虽然期间也零零散散的用过一些，但是基本已经忘的差不多了，所以这篇博客中如果有什么不对的地方欢迎和感谢大家指正！这里我只是写出了一些个人工作中常用到的正则匹配，如果同时有园友愿意贡献一份力量参与到正则开源工具的编码中，我会非常欢迎。
最近园子里的”月经贴”越来越多，我就很纳闷，这群人是不是闲的蛋疼？你有时间在这”指点江山”批评这个语言抨击那个环境的，你还不如张开双手拥抱.net开源大潮呢。随着.net开源的到来，.net的春天也来了，我们应该伸出双手为.net开源做一些事情，让.net社区越来越开源。当然这是题外话，那些月经贴也不关我啥事。
## 1.源码地址
[https://github.com/toutouge/TouTou.RegexTool](https://github.com/toutouge/TouTou.RegexTool)
## 2.正文开始
### **2.1.： **通用匹配
C#通用匹配规则，可以在调用的时候传入源字符串和对应的正则

C#
```
/// <summary>
/// 检测字符串中是否包含符合正则的子集
/// </summary>
/// <param name="source">源字符串</param>
/// <param name="reg">正则, e.g. \d+</param>
/// <returns>true:包含，反之不包含</returns>
publicbool CheckContainsByReg(string source, string reg) {
    return Regex.Match(source, reg).Success;
}
/// <summary>
/// 检测整个字符串是否能匹配正则,而不是包含
/// </summary>
/// <param name="source">源字符串</param>
/// <param name="reg">正则, e.g. ^\d+$</param>
/// <returns>true:匹配，反之不匹配</returns>
publicbool CheckStringByReg(string source, string reg) {
    Regex rg = new Regex(reg, RegexOptions.IgnoreCase);
    return rg.IsMatch(source);
}
/// <summary>
/// 从指定字符串中过滤出第一个符合正则匹配的子集
/// </summary>
/// <param name="source">源字符串</param>
/// <param name="reg">正则, e.g. \d+</param>
/// <returns>源字符串的第一个匹配的子集</returns>
publicstring GetFirstStringByReg(string source, string reg) {
    return Regex.Match(source, reg).Groups[0].Value;
}
/// <summary>
/// 从指定字符串中过滤出所有符合正则匹配的子集
/// </summary>
/// <param name="source">源字符串</param>
/// <param name="reg">正则, e.g. \d+</param>
/// <returns>true:匹配，反之不匹配</returns>
publicList < string > GetStringByReg(string source, string reg) {
    var regex = Regex.Matches(source, reg);
    List < string > list = new List < string > ();
    foreach(Match item in regex) {
        list.Add(item.Value);
    }
    return list;
}
```
### **2.2.： **数字匹配
C#正则表达式根据各种需求匹配数字

C#
```
/// <summary>
/// 从指定字符串中过滤出第一个数字
/// </summary>
/// <param name="source">源字符串</param>
/// <returns>源字符串的第一个数字</returns>
publicstring GetFirstNumberByString(string source) {
    return Regex.Match(source, @"\d+").Groups[0].Value;
}
/// <summary>
/// 从指定字符串中过滤出最后一个数字
/// </summary>
/// <param name="source">源字符串</param>
/// <returns>源字符串的最后一个数字</returns>
publicstring GetLastNumberByString(string source) {
    var reg = Regex.Matches(source, @"\d+");
    return reg[reg.Count - 1].Value;
}
/// <summary>
/// 从指定字符串中过滤出所有数字
/// </summary>
/// <param name="source">源字符串</param>
/// <returns>源字符串的所有数字</returns>
publicList < string > GetAllNumberByString(string source) {
    var reg = Regex.Matches(source, @"\d+");
    List < string > list = new List < string > ();
    foreach(Match item in reg) {
        list.Add(item.Value);
    }
    return list;
}
/// <summary>
/// 检车源字符串中是否包含数字
/// </summary>
/// <param name="source">源字符串</param>
/// <returns>true:源字符串包含数字;false:源字符串不包含数字</returns>
publicbool CheckNumberByString(string source) {
    return Regex.Match(source, @"\d").Success;
}
/// <summary>
/// 判断字符串是否全部是数字且长度等于指定长度
/// </summary>
/// <param name="source">源字符串</param>
/// <param name="length">指定长度</param>
/// <returns>返回值</returns>
publicbool CheckLengthByString(string source, int length) {
    Regex rg = new Regex(@"^\d{" + length + "}$");
    return rg.IsMatch(source);
}
```
### **2.3.： **正则截串
C#根据给定的起始字符截取起始之间的字符串

C#
```
/// <summary>
/// 截取字符串中开始和结束字符串中间的字符串
/// </summary>
/// <param name="source">源字符串</param>
/// <param name="startStr">开始字符串</param>
/// <param name="endStr">结束字符串</param>
/// <returns>中间字符串</returns>
publicstring Substring(string source, string startStr, string endStr) {
    Regex rg = new Regex("(?<=(" + startStr + "))[.\\s\\S]*?(?=(" + endStr + "))", RegexOptions.Multiline |
        RegexOptions.Singleline);
    return rg.Match(source).Value;
}
```
### **2.4.： **邮箱匹配
C#正则表达式匹配邮箱

C#
```
/// <summary>
 /// 匹配邮箱是否合法
 /// </summary>
 /// <param name="source">待匹配字符串</param>
 /// <returns>匹配结果true是邮箱反之不是邮箱</returns>
publicbool CheckEmailByString(string source) {
    Regex rg = new Regex("^\\s*([A-Za-z0-9_-]+(\\.\\w+)*@(\\w+\\.)+\\w{2,5})\\s*$", RegexOptions.IgnoreCase);
    return rg.IsMatch(source);
}
```
### **2.5.： **URL匹配
C#正则表达式匹配URL

C#
```
/// <summary>
 /// 匹配URL是否合法
 /// </summary>
 /// <param name="source">待匹配字符串</param>
 /// <returns>匹配结果true是URL反之不是URL</returns>
publicbool CheckURLByString(string source) {
    Regex rg = new Regex(@
        "^(https?|s?ftp):\/\/(((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:)*@)?(((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5]))|((([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.)+(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.?)(:\d*)?)(\/((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)+(\/(([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)*)*)?)?(\?((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)|[\uE000-\uF8FF]|\/|\?)*)?(#((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)|\/|\?)*)?$",
        RegexOptions.IgnoreCase);
    return rg.IsMatch(source);
}
```
### **2.6.： **日期匹配
C#正则表达式匹配日期

C#
```
/// <summary>
 /// 匹配日期是否合法
 /// </summary>
 /// <param name="source">待匹配字符串</param>
 /// <returns>匹配结果true是日期反之不是日期</returns>
publicbool CheckDateByString(string source) {
    Regex rg = new Regex(@
        "^(\d{4}[\/\-](0?[1-9]|1[0-2])[\/\-]((0?[1-9])|((1|2)[0-9])|30|31))|((0?[1-9]|1[0-2])[\/\-]((0?[1-9])|((1|2)[0-9])|30|31)[\/\-]\d{4})$");
    return rg.IsMatch(source);
}
 /// <summary>
 /// 从字符串中获取第一个日期
 /// </summary>
 /// <param name="source">源字符串</param>
 /// <returns>源字符串中的第一个日期</returns>
publicstring GetFirstDateByString(string source) {
    return Regex.Match(source, @
        "(\d{4}[\/\-](0?[1-9]|1[0-2])[\/\-]((0?[1-9])|((1|2)[0-9])|30|31))|((0?[1-9]|1[0-2])[\/\-]((0?[1-9])|((1|2)[0-9])|30|31)[\/\-]\d{4})")
        .Groups[0].Value;
}
 /// <summary>
 /// 从字符串中获取所有的日期
 /// </summary>
 /// <param name="source">源字符串</param>
 /// <returns>源字符串中的所有日期</returns>
publicList < string > GetAllDateByString(string source) {
    var all = Regex.Matches(source, @
        "(\d{4}[\/\-](0?[1-9]|1[0-2])[\/\-]((0?[1-9])|((1|2)[0-9])|30|31))|((0?[1-9]|1[0-2])[\/\-]((0?[1-9])|((1|2)[0-9])|30|31)[\/\-]\d{4})");
    List < string > list = new List < string > ();
    foreach(Match item in all) {
        list.Add(item.Value);
    }
    return list;
}
```
### **2.7.： **密码匹配
C#正则表达式匹配密码

C#
```
/// <summary>
/// 检测密码复杂度是否达标：密码中必须包含字母、数字、特称字符，至少8个字符，最多16个字符。
/// </summary>
/// <param name="source">待匹配字符串</param>
/// <returns>密码复杂度是否达标true是达标反之不达标</returns>
publicbool CheckPasswordByString(string source) {
    Regex rg = new Regex(@"^(?=.*\d)(?=.*[a-zA-Z])(?=.*[^a-zA-Z0-9]).{8,16}$");
    return rg.IsMatch(source);
}
```
### **2.8.： **邮编匹配
C#正则表达式匹配邮编

C#
```
/// <summary>
/// 匹配邮编是否合法
/// </summary>
/// <param name="source">待匹配字符串</param>
/// <returns>邮编合法返回true,反之不合法</returns>
publicbool CheckPostcodeByString(string source) {
    Regex rg = new Regex(@"^\d{6}$");
    return rg.IsMatch(source);
}
```
### **2.9.： **电话号码
C#正则表达式匹配电话

C#
```
/// <summary>
 /// 匹配电话号码是否合法
 /// </summary>
 /// <param name="source">待匹配字符串</param>
 /// <returns>电话号码合法返回true,反之不合法</returns>
publicbool CheckTelephoneByString(string source) {
    Regex rg = new Regex(@"^(\(\d{3,4}-)|\d{3.4}-)?\d{7,8}$");
    return rg.IsMatch(source);
}
 /// 
 /// ???????????
 /// 
 /// ????
 /// ?????????
publicstring GetTelephoneByString(string source) {
    return Regex.Match(source, @"(\)\d{3,4}-)|\d{3.4}-)?\d{7,8}").Groups[0].Value;
}
```
### **2.10.： **手机号码
C#正则表达式匹配手机号码

C#
```
/// <summary>
        /// 匹配手机号码是否合法
        /// </summary>
        /// <param name="source">待匹配字符串</param>
        /// <returns>手机号码合法返回true,反之不合法</returns>
        publicbool CheckMobilephoneByString(string source)
        {
            Regex rg = new Regex(@"^[1]+[3,5,7,8]+\d{9}$");
            return rg.IsMatch(source);
        }
        /// <summary>
        /// 从字符串中获取手机号码
        /// </summary>
        /// <param name="source">源字符串</param>
        /// <returns>源字符串中手机号码</returns>
        publicstring GetMobilephoneByString(string source)
        {
            return Regex. Match(source, @"[1]+[3,5,7,8]+\d{9}").Groups[0].Value;
        }
```
### **2.11.： **身份证匹配
C#正则表达式匹配身份证号码

C#
```
/// <summary>
/// 匹配身份证号码是否合法
/// </summary>
/// <param name="source">待匹配字符串</param>
/// <returns>身份证号码合法返回true,反之不合法</returns>
publicbool CheckIDCardByString(string source) {
    Regex rg = new Regex(@"^(^\d{15}$|^\d{18}$|^\d{17}(\d|X|x))$");
    return rg.IsMatch(source);
}
/// <summary>
/// 从字符串中获取身份证号码
/// </summary>
/// <param name="source">源字符串</param>
/// <returns>源字符串中身份证号码</returns>
publicstring GetIDCardByString(string source) {
    return Regex.Match(source, @"(^\d{15}$|^\d{18}$|^\d{17}(\d|X|x))").Groups[0].Value;
}
```
## 3.博客总结
> 
关于C#正则表达式开源工具暂时就只积累了这么多。因为目前实际工作中遇到的也就这些，**关于这个C#正则表达式开源工具现在只是一个雏形**，先把这一分不分享出来，后续得会持续更新C#正则表达式开源工具。希望在以后这个C#正则表达式工具会越来越strong，**更希望能得到园友们的support**.
