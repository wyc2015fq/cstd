# Property 'submit' of object # is not a function - z69183787的专栏 - CSDN博客
2014年06月26日 10:49:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1658
个人分类：[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)

今天使用jquery的时候使用脚本提交表单$("#submit").submit();
然后就出现了:Property 'submit' of object #<HTMLFormElement> is not a function
网上查了下原因:说是form里面有个name="submit"的按钮导致submit()方法和button的name混淆
然而我的却不是name,二是id为 "submit"
再次补充下,无论是name还是id为"submit"都是不行的,只要把submit改掉就OK 了,我改成了"Submit",也是OK 的!
