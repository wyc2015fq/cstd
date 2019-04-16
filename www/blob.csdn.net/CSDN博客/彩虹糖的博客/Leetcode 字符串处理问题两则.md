# Leetcode 字符串处理问题两则 - 彩虹糖的博客 - CSDN博客





2019年03月05日 20:55:10[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：18








每次遇到字符串相关的问题，就会出项相当多次的提交错误。总是会因为读题，手残，逻辑漏洞忽略各种特殊情况，这样的问题难度一般不是很多，关键是要考虑周到，不要漏掉情况。

# 394. 字符串解码

给定一个经过编码的字符串，返回它解码后的字符串。

编码规则为: `k[encoded_string]`，表示其中方括号内部的 *encoded_string* 正好重复 *k* 次。注意 *k* 保证为正整数。

你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。

此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 *k* ，例如不会出现像 `3a` 或 `2[4]` 的输入。

**示例:**

s = "3[a]2[bc]", 返回 "aaabcbc".
s = "3[a2[c]]", 返回 "accaccacc".
s = "2[abc]3[cd]ef", 返回 "abcabccdcdcdef".
### 题目分析

看到这个题目的第一个反应是编译原理，语法分析~

整体的思路还是非常熟悉和明确的，需要引入两个栈，一个记录字符串，一个记录重复次数。在遇到右括号之前，无论是字符串栈还是整数栈，只需要不断入栈即可，再遇到右括号后，将字符串栈和整数栈同时出栈，得到新的字符串，然后再把字符串压入栈，如此循环，最后字符串栈所留下的唯一元素就是我们的目标。

```java
class Solution {
    public String decodeString(String s) {
        String result = "";
        char[] chars = s.toCharArray();
        Stack<Integer> numStack = new Stack<Integer>();
        Stack<String> stringStack = new Stack<String>();
        int i = 0;
        while(i<chars.length)
        {
            char curChar = chars[i];
            if(curChar>='1'&&curChar<='9')
            {
                String numString = "";
                while(chars[i]>='0'&&chars[i]<='9')
                {
                    numString+=String.valueOf(chars[i]);
                    i++;
                }
                int num = Integer.valueOf(numString);
                numStack.push(num);
            }
            else if(curChar=='[')
            {
                String stringValue = "";
                stringStack.push(String.valueOf('['));
                i++;
                while((chars[i]>='a'&&chars[i]<='z')||(chars[i]>='A'&&chars[i]<='Z'))
                {
                    stringValue+=String.valueOf(chars[i]);
                    i++;
                }
                stringStack.push(stringValue);
            }
            else if(curChar==']')
            {
                String stringValue = stringStack.peek();
                stringStack.pop();
                stringStack.pop();
                int num = numStack.peek();
                numStack.pop();
                String newStringValue="";
                if(stringStack.size()>0)
                    newStringValue = stringStack.peek();
                for(int j=0;j<num;j++)
                {
                    newStringValue = newStringValue + stringValue;
                }
                if(stringStack.size()>0)
                    stringStack.pop();
                stringStack.push(newStringValue);
                i++;
            }
            else if(curChar>='a'&&curChar<='z')
            {
                String curSting = "";
                while(i<chars.length&&((chars[i]>='a'&&chars[i]<='z')||(chars[i]>='A'&&chars[i]<='Z')))
                {
                    curSting+=String.valueOf(chars[i]);
                    i++;
                }
                if(stringStack.size()>0)
                {
                    String string = stringStack.peek();
                    stringStack.pop();
                    string = string + curSting;
                    stringStack.push(string);
                }
                else
                {
                    stringStack.push(curSting);
                }
            }
            else
            {
                i++;
            }
        }
        if(stringStack.size()==0)
            return "";
        return stringStack.peek();
    }
}
```

# 71. 简化路径

以 Unix 风格给出一个文件的**绝对路径**，你需要简化它。或者换句话说，将其转换为规范路径。

在 Unix 风格的文件系统中，一个点（`.`）表示当前目录本身；此外，两个点 （`..`） 表示将目录切换到上一级（指向父目录）；两者都可以是复杂相对路径的组成部分。更多信息请参阅：[Linux / Unix中的绝对路径 vs 相对路径](https://blog.csdn.net/u011327334/article/details/50355600)

请注意，返回的规范路径必须始终以斜杠 `/` 开头，并且两个目录名之间必须只有一个斜杠 `/`。最后一个目录名（如果存在）**不能**以 `/` 结尾。此外，规范路径必须是表示绝对路径的**最短**字符串。



**示例 1：**
**输入："**/home/"
**输出："**/home"
**解释：**注意，最后一个目录名后面没有斜杠。

**示例 2：**
**输入："**/../"
**输出："**/"
**解释：**从根目录向上一级是不可行的，因为根是你可以到达的最高级。

**示例 3：**
**输入："**/home//foo/"
**输出："**/home/foo"
**解释：**在规范路径中，多个连续斜杠需要用一个斜杠替换。

**示例 4：**
**输入："**/a/./b/../../c/"
**输出："**/c"

**示例 5：**
**输入："**/a/../../b/../c//.//"
**输出："**/c"

**示例 6：**
**输入："**/a//b////c/d//././/.."
**输出："**/a/b/c"
### 题目分析

这个题目在思路上更简单一些，但是却花了更多的时间，确实是长知识了，可以给Unix系统的文件夹起名为“...”。以斜线为分解，逐段逐段的分析路径，并记录上一次斜线的位置，以方便返回上一级目录。

```java
class Solution {
    public String simplifyPath(String path) {
        char[] chars = path.toCharArray();
        String result = "";
        Stack<Integer> lastSlashIndexes = new Stack<Integer>();
        lastSlashIndexes.push(0);
        for(int i=0;i<chars.length;i++)
        {
            if(chars[i]=='/')
            {
                while(i<chars.length&&chars[i]=='/')
                    i++;
                if(i<chars.length-1&&chars[i]=='.'&&chars[i+1]=='.'&&(i+2==chars.length||chars[i+2]=='/'))
                {
                    i = i + 2;
                    result = result.substring(0,lastSlashIndexes.peek());
                    if(lastSlashIndexes.size()>1)
                        lastSlashIndexes.pop();
                }
                else if(i<chars.length&&chars[i]=='.'&&(i+1==chars.length||chars[i+1]=='/'))
                {
                }
                else
                {
                    result = result + '/';
                    int lastSlashIndex = result.length()-1;
                    if(lastSlashIndex!=0)
                    {
                        lastSlashIndexes.push(lastSlashIndex);
                    }
                    while(i<chars.length&&chars[i]!='/')
                    {
                        result = result + String.valueOf(chars[i]);
                        i++;
                    }
                }
                i--;
            }
        }
        if(result.endsWith("/"))
            result = result.substring(0,result.length()-1);
        if(result.length()==0)
            result = "/";
        return result;
    }
}
```





