# Java正则 Mathcer 获取第N次出现某个字符的位置 - z69183787的专栏 - CSDN博客
2017年06月24日 10:13:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3682
```java
public static void main(String[] args){
        String string = "213213/13123/14432432";
        //这里是获取"/"符号的位置
        Matcher slashMatcher = Pattern.compile("/1").matcher(string);
        int mIdx = 0;
        while(slashMatcher.find()) {
            System.out.println(slashMatcher.group());
            mIdx++;
            //当"/"符号第三次出现的位置
            if(mIdx == 2){
                break;
            }
        }
        System.out.println(slashMatcher.start());
    }
```
