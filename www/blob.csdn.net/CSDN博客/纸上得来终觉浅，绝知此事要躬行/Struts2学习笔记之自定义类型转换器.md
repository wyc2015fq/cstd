# Struts2学习笔记之自定义类型转换器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月22日 22:35:44[boonya](https://me.csdn.net/boonya)阅读数：848标签：[struts																[date																[string																[object																[exception](https://so.csdn.net/so/search/s.do?q=exception&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记

第七记：自定义类型转换器（以时间类型转换器为例）

**1、局部类型转换器**

   1-1：遵循时间规范的字符串自动类型转换

   如：UserAction中的时间

     public class UserAction{

            private  Date   today;

            public void setToday(Date today){

                    this.today=today;


            }

           public Date getToday(){

                  return  this.today;


            }

           public String printDate(){

                  System.out.println(this.getToday());

                  return "SUCCESS";


           }


     }

  若struts.xml中UserAction的别名是users，那么它的访问路径是users!printDate.action?today=2012-7-22。

   注意：若today=20120722这不能通过自动类型转换，程序判定today为String类型而不是Date类型。

   1-2：String类型和Date类型匹配策略


   例如：这里存在一个DateConverter

   public class DateConverter extends DefaultTypeConverter{

         @override

         public  Object  convertvalue(Map<String,Object>  context,Object value, Class toType){

                SimpleDateFormat  sdf=new SimpleDateFormat ("yyyyMMdd");

                try{

                     if(toType==Date.class){

                           String [] params=String [] | value;

                           return sdf.parse(params[0]);


                    }else if(toType==Date.class){

                          Date date=(Date)value;

                          return sdf.format(date);


                   }


               }catch(Exception e){   }      


               return null;


         }


   }


     注册Action对应的转换器的属性文件：规则ActionName-conversion.properties

    如：UserAction-conversion.properties

    文件内容：如today=com.xxxx.DateConverter#key是要转换的字段，value是类型转换器的路径

**2、全局类型转换器**

   此时的属性文件改成：xwork-conversion.properties

   如定义所有的时间类型转换器都通过DateConverter，则：

 java.utils.Date=com.xxxx.DateConverter](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




