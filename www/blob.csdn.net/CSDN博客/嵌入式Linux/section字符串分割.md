
# section字符串分割 - 嵌入式Linux - CSDN博客

2017年03月20日 14:44:46[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：531


QString QString::section(QChar sep, int start, int end = -1, SectionFlags flags = SectionDefault) const
This function returns a section of the string.

This string is treated as a sequence of fields separated by the character, sep. The returned string consists of the fields from position start to position end inclusive. If end is not specified, all fields from position start to the end of the string are included.
 Fields are numbered 0, 1, 2, etc., counting from the left, and -1, -2, etc., counting from right to left.
The flags argument can be used to affect some aspects of the function's behavior, e.g. whether to be case sensitive, whether to skip empty fields and how to deal with leading and trailing separators; see SectionFlags.

QString str;
QString csv = "forename,middlename,surname,phone";
QString path = "/usr/local/bin/myapp"; // First field is empty
QString::SectionFlag flag = QString::SectionSkipEmpty;

str = csv.section(',', 2, 2);   // str == "surname"  从左到右是分成 0，1,2 个块 从右到左是分成 -1 -2 -3 个块序号
str = path.section('/', 3, 4);  // str == "bin/myapp" 这个第一个块是空白，第三块是bin,第四块是myapp
str = path.section('/', 3, 3, flag); // str == "myapp"



