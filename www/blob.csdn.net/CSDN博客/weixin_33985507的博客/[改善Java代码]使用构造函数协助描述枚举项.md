# [改善Java代码]使用构造函数协助描述枚举项 - weixin_33985507的博客 - CSDN博客
2016年04月29日 16:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5

一、分析
一般来说，我们经常使用的枚举项只有一个属性，即排序号，其默认值是从0、1、2... ...。但是除了排序号外，枚举还有一个（或多个）属性:枚举描述,它的含义是通过枚举的构造函数,声明每个枚举项(也就是枚举实例)必须具有的属性和行为,这是对枚举项的描述或补充,目的是使枚举项表述的意义更加清晰准确.
二、场景
比如，可以通过枚举构造函数声明业务值，定义可选项，添加属性，看如下代码：
```
1 public class Client {
 2     public static void main(String[] args) {
 3         System.out.println(Season.Spring.getDesc());
 4         
 5     }
 6 }
 7 
 8 enum Season {
 9     Spring("春"), Summer("夏"), Autumn("秋"), Winter("冬");
10     
11     private String desc;
12     Season(String _desc){
13         desc = _desc;
14     }
15     
16     //获得枚举值
17     public String getDesc(){
18         return desc;
19     }
20 }
```
运行输出: 春
其枚举项是英文的,描述是英文的,这样使其描述更加准确.方便了多个协作者共同引用常量.若不考虑描述的使用(即访问getDes方法),它与如下定义的描述很相似.
```
1 interface Season{
2     //春
3     int Spring = 0;
4     //夏
5     int Summer =1
6     .....
7 }
```
比较上面两段代码,很容易看出使用枚举项是一个很好的解决方案,非常简单,清晰.
可以通过枚举构造函数声明业务值,定义可选项,添加属性等.看如下代码:
```
1 enum Role{ 
 2     Admin("管理员",new Lifetime(),new Scope()); 
 3     User("普通用户",new Lifetime(),new Scope()); 
 4  
 5     //中文描述 
 6     private String name; 
 7     //角色生命周期 
 8     private Lifetime lifeTime; 
 9     //权限范围 
10     private Scope scope; 
11  
12     Role(String _name,Lifetime _lt,Scope _scope){ 
13         name = _name; 
14         lifeTime = _lifeTime; 
15         scope = _scope; 
16     } 
17     /**name,lifeTime,scope的get方法较简单，不再赘述*/ 
18 }
```
 这是一个角色定义类,描述了两个角色:管理员(Admin)和普通用户(User),同时它还通过构造函数对这两个角色进行了描述.:
1.name 表示的是该角色的中文名称
2.lifeTime 表示的是该角色的生命周期,也就是多长时间角色失效
3.scope 表示的是该角色的权限范围.
这样 一个描述可以使开发者对Admin和User两个常量有一个立体多维度的认知.有名称,生命期还有权限范围.而且还可以在程序中方便的获得这些属性.
建议在枚举定义中改为每个枚举项定义描述,特别是在大规模的项目开发中.大量的常量项定义使用枚举比在接口常量或者类常量中增加注释的方式友好简洁很多.
