# Java判断字符串是否为纯数字（0-9） - Koma Hub - CSDN博客
2017年09月08日 11:52:42[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：2332
```java
public static boolean isNumeric(String str){
        / * Copyright: http://javapub.iteye.com/blog/666544 * /
                for (int i = 0; i < str.length(); i++){
                        if (!Character.isDigit(str.charAt(i))){
                                return false;
                              }
                      }
                return true;
          }
```
