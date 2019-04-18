# 微信公众平台昵称乱码emoji表情过滤 - z69183787的专栏 - CSDN博客
2015年09月15日 11:13:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5451
#### 问题描述
最近在对线上排错过程中发现很多入库时产生的错误日志,表现形态如下
Incorrect string value : '\xF0\x9F\x91\x8D\xE6\x94...' for column 'column_x' at row 1
在查阅资料后发现这其实是一种emoji表情符号,普遍存在iOS与android系统中,而这种特殊字符合作用的Unicode 6标准来统一,采用4个bytes来存储一个emoji表情,而将这种表情不处理直接存储到MySQL5.5以下的版本会报错,当然想要MySQL存储这种字符也不困难,只需要修改数据库字符集为utf8mb4即可,但数据回传给网页或者移动客户端时则需要做兼容处理,所以我们暂时忽略这种需求,直接将其过滤掉.
#### 解决方案
对于字符串处理,首选就是正则表达式去处理,而在android系统中可以自定义InputFilter去过滤需要处理掉的字符串,代码如下
InputFilter emojiFilter = new InputFilter ( ) {
     @Override
     public CharSequence filter ( CharSequence source , int start , int end , Spanned dest , int dstart ,
         int dend ) {
     }
} ;
随后我查阅了 [emoji](http://en.wikipedia.org/wiki/Emoji) 的wikipedia与 [Github](http://punchdrunker.github.io/iOSEmoji/table_html/flower.html) ,从中提取出表情的一个大概unicode范围,由于Java可以直接对unicode进行匹配,这样我们可以很省事直接写出Pattern即可,代码如下
InputFilter emojiFilter = new InputFilter ( ) {
     Pattern emoji = Pattern . compile (
         "[\ud83c\udc00-\ud83c\udfff]|[\ud83d\udc00-\ud83d\udfff]|[\u2600-\u27ff]" ,
         Pattern . UNICODE_CASE | Pattern . CASE_INSENSITIVE ) ;
     @Override
     public CharSequence filter ( CharSequence source , int start , int end , Spanned dest , int dstart ,
         int dend ) {
       Matcher emojiMatcher = emoji . matcher ( source ) ;
       if ( emojiMatcher . find ( ) ) {
         return "" ;
       }
       return null ;
     }
} ;
基本上这样就能过滤掉emoji表情了
