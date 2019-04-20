# Java中定义常量(Constant) 的几种方法 - Spark高级玩法 - CSDN博客
2018年07月29日 09:50:28[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：5794
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
为了方便大家交流Spark大数据，浪尖建了微信群，目前人数过多，只能通过浪尖或者在群里的朋友拉入群。纯技术交流，偶有吹水，但是打广告，不提醒，直接踢出。有兴趣加浪尖微信。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
**常量使用目的**
1，为什么要将常亮提取出来？ 2，提取出来怎么定义，定义在interface中，还是class中？有什么区别？
1）常亮提取出来有利于代码阅读，而且下次再做这种判断不用手写或复制，直接通过常量类就能得到，能够避免有的地方是username,有的地方是name来判断同一个逻辑含义相同变量问题。不过我觉得提取出来并不会有利于代码性能提升，因为常量分配在内存的常亮池中，所以内存不会出现多个相同常量字符串。总的来说提取常量主要是为了避免魔法数字和提高代码可读性。
2）常亮定义在final的class中，防止被其它类继承和实例化。定义在interface中性质也是一样，static final属性。但是interface看起来就是会被继承的。
**interface中定义常量**
采用接口(Interface)的中变量默认为static final的特性。
```
public interface ConstantInterface {  
    String SUNDAY = "SUNDAY";  
    String MONDAY = "MONDAY";  
    String TUESDAY = "TUESDAY";  
    String WEDNESDAY = "WEDNESDAY";  
    String THURSDAY = "THURSDAY";  
    String FRIDAY = "FRIDAY";  
    String SATURDAY = "SATURDAY";  
}
```
**Enum定义常量**
```
enum ConstantEnum {  
        SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY  
    }
```
**普通类中定义常量**
采用了在普通类中使用static final修饰变量的方法。
```
public class ConstantClassField {  
        public static final String SUNDAY = "SUNDAY";  
        public static final String MONDAY = "MONDAY";  
        public static final String TUESDAY = "TUESDAY";  
        public static final String WEDNESDAY = "WEDNESDAY";  
        public static final String THURSDAY = "THURSDAY";  
        public static final String FRIDAY = "FRIDAY";  
        public static final String SATURDAY = "SATURDAY";  
    }
```
也可以使用get方法
```
public class ConstantClassFunction {  
        private static final String SUNDAY = "SUNDAY";  
        private static final String MONDAY = "MONDAY";  
        private static final String TUESDAY = "TUESDAY";  
        private static final String WEDNESDAY = "WEDNESDAY";  
        private static final String THURSDAY = "THURSDAY";  
        private static final String FRIDAY = "FRIDAY";  
        private static final String SATURDAY = "SATURDAY";  
        public static String getSunday() {  
            return SUNDAY;  
        }  
        public static String getMonday() {  
            return MONDAY;  
        }  
        public static String getTuesday() {  
            return TUESDAY;  
        }  
        public static String getWednesday() {  
            return WEDNESDAY;  
        }  
        public static String getThursday() {  
            return THURSDAY;  
        }  
        public static String getFirday() {  
            return FRIDAY;  
        }  
        public static String getSaturday() {  
            return SATURDAY;  
        }  
    }
```
**final class定义常量**
```
public final class ConstantClassField {  
       public static final String SUNDAY = "SUNDAY";  
       public static final String MONDAY = "MONDAY";  
       public static final String TUESDAY = "TUESDAY";  
       public static final String WEDNESDAY = "WEDNESDAY";  
       public static final String THURSDAY = "THURSDAY";  
       public static final String FRIDAY = "FRIDAY";  
       public static final String SATURDAY = "SATURDAY";  
   }
```
推荐，使用final class定义，表面意思更清楚。虽然为了简单编码，浪尖采用的是interface。
欢迎关注浪尖的微信公众号：Spark学习技巧。
**推荐阅读：**
[Java面试中常问的数据库方面问题](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485041&idx=1&sn=879eead8b97d8182955223dd1c918475&chksm=9f38e559a84f6c4f50948577cb37c8c90523f0e89e7fba9433a7a58d6ded818fff33c696aac9&scene=21#wechat_redirect)
[Java面试知识点解析——JVM篇](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484940&idx=1&sn=c58300507839260ce276aaaa5f478c59&chksm=9f38e524a84f6c321f606434cc53dc354ea6254131f628777550121c45c4ddb7ef349147a888&scene=21#wechat_redirect)
[面试的角度诠释Java工程师（二）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484696&idx=1&sn=587f1e653fabc068c0308ff1eddd1e7c&chksm=9f38e630a84f6f2672d9040460e603958727fbec331ed037e6ef68d48d45849809bbba760b03&scene=21#wechat_redirect)
