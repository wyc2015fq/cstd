# 正则表达式（Java，C#，C++） - zwvista - 博客园

## [正则表达式（Java，C#，C++）](https://www.cnblogs.com/zwvista/p/7899250.html)

### 课题
- 使用正则表达式匹配字符串

使用正则表达式 "\d{3}-(\d{4})-\d{2}" 匹配字符串 "123-4567-89"

返回匹配结果：’"123-4567-89" 以及 "4567"
- 使用正则表达式替换字符串（模式）

使用正则表达式 "(\d+)-(\d+)-(\d+)" 匹配字符串 "123-4567-89"

使用模式字符串 "$3-$1-$2" 替换匹配结果，返回结果 "89-123-4567"。
- 使用正则表达式替换字符串（回调）

使用正则表达式 "\d+" 匹配字符串 "123-4567-89"

将匹配结果即三个数字串全部翻转过来，返回结果 "321-7654-98"。
- 使用正则表达式分割字符串

使用正则表达式 "%(begin|next|end)%" 分割字符串"%begin%hello%next%world%end%"

返回正则表达式分隔符之间的两个字符串 "hello" 和 "world"。

### Java

```
import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Test {

    public static void main(String[] args) {
        String s = "123-4567-89";
        Pattern r = Pattern.compile("\\d{3}-(\\d{4})-\\d{2}");
        Matcher m = r.matcher(s);
        if (m.find())
            for (int i = 0; i <= m.groupCount(); i++)
                System.out.printf("group %d : %s\n", i, m.group(i));

        System.out.println(s.replaceFirst("(\\d+)-(\\d+)-(\\d+)", "$3-$1-$2"));

        // https://stackoverflow.com/questions/19737653/what-is-the-equivalent-of-regex-replace-with-function-evaluation-in-java-7
        r = Pattern.compile("\\d+");
        m = r.matcher(s);
        StringBuffer sb = new StringBuffer();
        while (m.find())
            m.appendReplacement(sb, new StringBuffer(m.group(0)).reverse().toString());
        m.appendTail(sb);
        System.out.println(sb.toString());

        r = Pattern.compile("%(begin|next|end)%");
        s = "%begin%hello%next%world%end%";
        System.out.println(Arrays.asList(r.split(s)));
    }
}

/*
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
[, hello, world]
 */
```

### C#

```
using System;
using System.Text.RegularExpressions;

namespace Sample
{
    class Test
    {
        public static void Main(string[] args)
        {
            var r = new Regex(@"\d{3}-(\d{4})-\d{2}");
            var s = "123-4567-89";
            var m = r.Match(s);
            foreach (Group g in m.Groups)
                Console.WriteLine($"group {g.Index} : {g.Value}");

            r = new Regex(@"(\d+)-(\d+)-(\d+)");
            Console.WriteLine(r.Replace(s, "$3-$1-$2"));
            
            r = new Regex(@"\d+");
            s = r.Replace(s, m2 => {
                var arr = m2.Groups[0].Value.ToCharArray();
                Array.Reverse(arr);
                return new string(arr);
            });
            Console.WriteLine(s);

            r = new Regex("%(?:begin|next|end)%");
            s = "%begin%hello%next%world%end%";
            Console.WriteLine(String.Join(",", r.Split(s)));
        }
    }
}

/*
group 0 : 123-4567-89
group 4 : 4567
89-123-4567
321-7654-98
,hello,world,
*/
```

### C++

```
#include <iostream>
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;

int main()
{
    string s = "123-4567-89";
    boost::regex r(R"(\d{3}-(\d{4})-\d{2})");
    boost::match_results<string::const_iterator> mr;
    boost::regex_search(s, mr, r);
    for (size_t i = 0; i < mr.size(); ++i)
        cout << "group " << i << " : " << mr[i] << endl;
     
    boost::regex r2(R"((\d+)-(\d+)-(\d+))");
    cout << boost::regex_replace(s, r2, "$3-$1-$2") << endl;
     
    boost::regex r3(R"(\d+)");
    s = boost::regex_replace(s, r3, [](auto& match) {
        string s = match.str();
        reverse(s.begin(), s.end());
        return s;
    });
    cout << s << endl;
     
    boost::regex r4("%(?:begin|next|end)%");
    s = "%begin%hello%next%world%end%";
    vector<string> v;
    for (boost::sregex_token_iterator begin(s.begin(), s.end(), r4, -1), end, i = begin; i != end; ++i)
        v.push_back(i->str());
    cout << boost::algorithm::join(v, ",") << endl;
}

/*
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
,hello,world
*/
```


