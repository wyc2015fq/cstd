# java中判断字符是否是数字的几种方法 - z69183787的专栏 - CSDN博客
2012年11月13日 16:23:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3469
java判断数字的几种方法，整理如下，个人比较推荐用正则表达式判断。其它的方法，实现不是很美观，并且如果判断是不是小数比较麻烦 
1.直接用jdk自带的判断字符是不是数字的方法做循环
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)publicstaticboolean isNumeric(String str){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)for (int i = str.length();--i>=0;){  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)if (!Character.isDigit(str.charAt(i))){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returnfalse;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returntrue;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
2.用正则表达式
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)publicstaticboolean isNumeric(String str){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     Pattern pattern = Pattern.compile("[0-9]*");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return pattern.matcher(str).matches();   
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
3.逐一判断ascII码
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)publicstaticboolean isNumeric(String str){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)for(int i=str.length();--i>=0;){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int chr=str.charAt(i);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(chr<48|| chr>57)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returnfalse;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returntrue;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
4.强制转换
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)publicstaticboolean isNumeric(String str){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    BigDecimal result =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)try{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        result =new BigDecimal(Str);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)    }catch (Exception e){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (result==null)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returnfalse;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)else
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returntrue;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
