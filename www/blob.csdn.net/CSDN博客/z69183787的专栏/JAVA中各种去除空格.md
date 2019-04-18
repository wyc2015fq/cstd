# JAVA中各种去除空格 - z69183787的专栏 - CSDN博客
2014年05月23日 19:12:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2700
1. String.trim()
trim()是去掉首尾空格
2.str.replace(" ", ""); 去掉所有空格，包括首尾、中间
String str = " hell o ";
String str2 = str.replaceAll("", "");
System.out.println(str2);
3.或者replaceAll(" +",""); 去掉所有空格
4.str = .replaceAll("\\s*","");
可以替换大部分空白字符， 不限于空格 
\s 可以匹配空格、制表符、换页符等空白字符的其中任意一个
5.或者下面的代码也可以去掉所有空格，包括首尾、中间
public String remove(String resource,char ch)
    {
        StringBuffer buffer=new StringBuffer();
        int position=0;
        char currentChar;
        while(position<resource.length())
        {
            currentChar=resource.charAt(position++);
            if(currentChar!=ch) buffer.append(currentChar); }return buffer.toString();
    }
测试的全部代码如下：
**public class** test1 {
**public static void** main(String[] args) {
        String QJstr = " hello wch ";
         String QJstr1 = *remove*(QJstr,' ');
         System.*out*.println(QJstr + "\n" + QJstr1);
}
**public static** String remove(String resource,**char** ch)
    {
        StringBuffer buffer=**new** StringBuffer();
**int** position=0;
**char** currentChar;
**while**(position<resource.length())
        {
            currentChar=resource.charAt(position++);
**if**(currentChar!=ch)    buffer.append(currentChar);
        }
**return** buffer.toString();
    }
}
