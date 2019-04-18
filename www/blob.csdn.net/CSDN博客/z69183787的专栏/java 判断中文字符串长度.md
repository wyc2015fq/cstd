# java 判断中文字符串长度 - z69183787的专栏 - CSDN博客
2013年08月14日 16:47:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2057
```java
/** 
     * 获取字符串的长度，中文占一个字符,英文数字占半个字符 
     * 
     * @param value  指定的字符串           
     * @return 字符串的长度 
     */  
    public static double length(String value) {  
        double valueLength = 0;  
        String chinese = "[\u4e00-\u9fa5]";  
        // 获取字段值的长度，如果含中文字符，则每个中文字符长度为2，否则为1  
        for (int i = 0; i < value.length(); i++) {  
            // 获取一个字符  
            String temp = value.substring(i, i + 1);  
            // 判断是否为中文字符  
            if (temp.matches(chinese)) {  
                // 中文字符长度为1  
                valueLength += 1;  
            } else {  
                // 其他字符长度为0.5  
                valueLength += 0.5;  
            }  
        }  
        //进位取整  
        return  Math.ceil(valueLength);  
    }
```
