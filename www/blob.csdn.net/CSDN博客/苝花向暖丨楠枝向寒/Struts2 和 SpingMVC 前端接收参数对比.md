# Struts2 和 SpingMVC  前端接收参数对比 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月12日 23:59:15[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：40标签：[Sturts2																[SpringMVC](https://so.csdn.net/so/search/s.do?q=SpringMVC&t=blog)](https://so.csdn.net/so/search/s.do?q=Sturts2&t=blog)
个人分类：[框架](https://blog.csdn.net/weixin_40247263/article/category/7511186)


首先说明一点： domainModel 和 pojo 是一个东西  。 一个是Struts2 的叫法，一个是SpringMVC的叫法。

思想都是MVC，M：就是实体类，用来传递前端的参数，前端接收到参数，存入pojo，然后在dao层再取出来，操作数据库。

                                jsp的input标签的name值和pojo变量值名需一样。

                         V：就是页面 jsp

                         C： 就是 controller (SpringMVC) 和 action (Struts2)

SpringMVC接收参数 ：直接 把pojo 作为形参，当然pojo中要写get、set方法。

       @RequestMap（"/test"）

    public String test(User user){

            return "useradd";

    }

Struts2 接收参数：

    1、 在action中 声明 全局变量。且生成get、set方法

      String  name；

      String gender；

      getXXX()   

      setXXX()

      然后再把 成员变量 set进 domainModel 中，domainModel 作为值参数的容器进行传递。

       或者放入一个 map 集合中也可以。 然后dao层通过 get(key)拿value 再用。

   2、在action 中 声明全局变量，但这次的全局变量是 domainModel 。且生成get、set方法。

        User user ；

         getUser(){}

         setUser(){}

       domainModel 中 也要生成成员变量的get、set方法

        如    Class User{

                  String  name；

                  String gender；

                  getXXX()   

                  setXXX()

                }

          jsp 传参的时候 就是  user.name=xxx&user.gender=xxx

    3、注册页面一般都有确认密码这一项，但是从逻辑上来说，我们的domainModel中应该只有 String name 和 String pwd

         所以这是需要 引出 一个DTO的概念，DTO代替了domainModel 接前端参数的这个工作。而domainModel 只负责从               service层到dao层 参数值的传递。也就是说显示DTO从前端把值拿过来，在service层再将DTO的一部分成员变量的值传 递给domainModel ，domainModel 再拿着这些数据 去dao层。

![](http://on-img.com/chart_image/5acf7c91e4b04691062e8140.png)

        4、(1)Action implements ModelDriven<User>（这里最好用泛型指明，否则getModel需要强制类型转换） 

             (2)new 一个 domainModel

             如：   User user = new User();

             (3)重写 getModel 方法

               @Override

                   public User getModel(){

                       return user;

                     }

              (4)User 中 需要写get、set 方法

              (5)jsp 页面 传参 ： name=xxx&pwd=xxx  (注意这里和第二种情况jsp页面传参的对比)

![](http://on-img.com/chart_image/5acf80f0e4b0f5fa24d164c9.png)





