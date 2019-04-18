# ASP.NET 对字符串大小写切换 - weixin_33985507的博客 - CSDN博客
2013年12月29日 01:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
来源：[http://www.codeproject.com/Tips/297011/How-to-Toggle-String-Case-in-NET#alternative22](http://www.codeproject.com/Tips/297011/How-to-Toggle-String-Case-in-NET#alternative22)
**使用LINQ:**
```
public static string ToggleCase(this string str)
{
     if (string.IsNullOrEmpty(str)) return str;
     return string.Join("", (from i in str
                             select (char.IsLetter(i) ?
                                  (char.IsUpper(i) ?
                                        char.ToLower(i) :
                                        char.ToUpper(i)) :
                                   i).ToString()
                             ).ToArray());
}
```
来源：[http://www.oschina.net/code/snippet_54100_7585](http://www.oschina.net/code/snippet_54100_7585)
```
public string ToggleCase(string input)
{
    string result = string.Empty;
    char[] inputArray = input.ToCharArray();
    foreach (char c in inputArray)
    {
        if (char.IsLower(c))
            result += c.ToString().ToUpper();
        else if (char.IsUpper(c))
            result += c.ToString().ToLower();
        else
            result += c.ToString();
    }
    return result;
}
```
```
public string ToggleCase(string input)
{
    char[] array = input.ToCharArray();
    char temp;
    for (int i = 0; i < array.Length; i++)
    {
        temp = array[i];
        if (char.IsUpper(temp))
            array[i] = char.ToLower(temp);
        else if (char.IsLower(temp))
            array[i] = char.ToUpper(temp);
    }
    return new string(array);
}
```
