# oracle函数之instr()和substr()实现列拆分 - 零度的博客专栏 - CSDN博客
2016年06月22日 17:01:31[零度anngle](https://me.csdn.net/zmx729618)阅读数：3586
#### 1、INSTR()函数
        在Oracle中可以使用instr()函数对某个字符串进行判断，判断其是否含有指定的字符。 在一个字符串中查找指定的字符,返回被查找到的指定的字符的位置。 
语法：
        instr(sourceString,destString,start,appearPosition) 
        instr('源字符串' , '目标字符串' ,'开始位置','第几次出现') 
        其中，sourceString代表源字符串； 
        destString代表要从源字符串中查找的子串； 
        start代表查找的开始位置，这个参数可选的，默认为1； 
        appearPosition代表想从源字符中查找出第几次出现的destString，这个参数也是可选的， 默认为1； 
        如果start的值为负数，则代表从右往左进行查找，但是位置数据仍然从左向右计算。 
返回值为：
        查找到的字符串的位置。 
        对于instr()函数，我们经常这样使用：从一个字符串中查找指定子串的位置。
举例： 
        SQL> select instr('abcdefgh','de') position from dual; 
POSITION 
---------- 
   4 
         从1开始算 d排第四所以返回4。
          SQL>select instr('abcdefghbc','bc',3) position from dual; 
POSITION 
---------- 
  9 
         从第3个字符开始算起 第3个字符是c，所以从3开始以后的字符串找查找bc,返回9。 
          SQL> select instr('qinyinglianqin','qin', 1, 2) position from dual; 
POSITION 
---------- 
  12 
         从第1个字符开始，查找第2次出现子串的位置。
#### 2、SUBSTR()函数
      用处:是从给定的字符表达式或备注字段中返回一个子字符串。 　　 
      语法格式:SUBSTR(cExpression，nStartPosition [，nCharactersReturned]) 　　 
      其中：
              cExpression指定要从其中返回字符串的字符表达式或备注字段； 
              nStartPosition用于指定返回的字符串在字符表达式或备注字段中的位置， 
              nCharactersReturned用于指定返回的字符数目，缺省时返回字符表达式的值结束前的全部字符。 　 
      举例:
             STORE：'abcdefghijlkm'  To mystring 　　
             SUBSTR(mystring ,1,5) 显示 "abcde" 　1 从第一个字符开始 截取的字符中，包括第一个字符　 
             SUBSTR(mystring ,6) 显示 "fghijklm" 　　 
             SUBSTR(mystring,-2)显示“km” 　　最右边一个字符是-1,最右边左边的字符是-2，然后默认是从左向有取剩下的全部的字符
             SUBSTR(mystrng,-4)显示“jlkm”
