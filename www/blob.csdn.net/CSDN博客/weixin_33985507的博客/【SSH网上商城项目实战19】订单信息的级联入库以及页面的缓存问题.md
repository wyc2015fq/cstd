# 【SSH网上商城项目实战19】订单信息的级联入库以及页面的缓存问题 - weixin_33985507的博客 - CSDN博客
2018年11月06日 17:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
购物车这一块还剩最后两个问题，就是订单信息的级联入库和页面缓存，这里的信息是指购物车和购物项，即我们将购物车的信息存入数据库的同时，也存入每个购物项的信息，而且外键都关联好，这涉及到了Hibernate中的级联入库问题；页面缓存问题指的是当用户确认了订单后，如果点后退，又会回到订单确认页面，刚刚的订单确认页面又出来了，而且session还在，信息还是刚刚的信息，这明显不是我们想要的结果，我们会在后面一一分析。这一节主要来讨论订单信息的级联入库以及页面的缓存问题。
1. 订单信息的级联入库
        Hibernate中两张关联表的级联入库，需要配置一下，这里主要介绍注解的配置方式，订单的POJO是Forder，购物项的POJO是Sorder，Forder与Sorder是一对多的关系，首先我们设置一下它们的注解配置，如下：
```
1 @Entity
 2 public class Forder implements java.io.Serializable {
 3     
 4     //省略无关代码……
 5  
 6     private List<Sorder> sorders = new ArrayList<Sorder>();
 7  
 8     @OneToMany(cascade = CascadeType.ALL, fetch = FetchType.LAZY, mappedBy = "forder")
 9     public List<Sorder> getSorders() {
10         return this.sorders;
11     }
12  
13     public void setSorders(List<Sorder> sorders) {
14         this.sorders = sorders;
15     }
16 }
17  
18 @Entity
19 public class Sorder implements java.io.Serializable {
20  
21     //省略无关代码……
22  
23     private Forder forder;
24  
25     @ManyToOne(fetch = FetchType.LAZY)
26     @JoinColumn(name = "fid")
27     public Forder getForder() {
28         return this.forder;
29     }
30  
31     public void setForder(Forder forder) {
32         this.forder = forder;
33     }
34 }
```
这样配置后，当我们save订单项时，也会save购物项，而且自动关联外键。但是前提是，我们要将它们之间的关系设置好，即forder中要setSorders()，sorder中要setForder()，以及其他相关联的外键对应的实体中的属性。
        之前我们将购物项加入购物车的时候已经将执行了forder.setSorder(sorder)了，现在我们需要在sorder中加入forder，所以我们在原来的代码上添加一下，如下：
```
1 //这是17节中的代码，我们在中间插上一句
 2 @Service("sorderService")
 3 public class SorderServiceImpl extends BaseServiceImpl<Sorder> implements
 4         SorderService {
 5  
 6     @Override
 7     public Forder addSorder(Forder forder, Product product) {
 8         boolean isHave = false; //用来标记有没有重复购物项
 9         //拿到当前的购物项
10         Sorder sorder = productToSorder(product);
11         //判断当前购物项是否重复，如果重复，则添加数量即可
12         for(Sorder old : forder.getSorders()) {
13             if(old.getProduct().getId().equals(sorder.getProduct().getId())) {
14                 //购物项有重复，添加数量即可
15                 old.setNumber(old.getNumber() + sorder.getNumber());
16                 isHave = true;
17                 break;
18             }
19         }
20         //当前购物项在购物车中不存在，新添加即可
21         if(!isHave) {
22                         //我们在这里插入一句：
23             //在向购物中添加购物项之前，先建立购物项与购物车的关联，但是此时forder.id为null，
24             //但是在入库的时候是先入库购物车，再入库购物项，那时候就有主键了
25             sorder.setForder(forder);
26             forder.getSorders().add(sorder);
27         }
28         
29         return forder;
30     }
31  
32     @Override
33     public Sorder productToSorder(Product product) {
34         Sorder sorder = new Sorder();
35         sorder.setName(product.getName());
36         sorder.setNumber(1);
37         sorder.setPrice(product.getPrice());
38         sorder.setProduct(product);
39         return sorder;
40     }
41 }
```
        好了，我们看一下订单确认时跳转到了哪个Action：
![](https://img2018.cnblogs.com/blog/893718/201811/893718-20181106170401694-953344112.png)
        所以我们去完成forderAction中的逻辑：
```
1 @Controller("forderAction")
 2 @Scope("prototype")
 3 public class ForderAction extends BaseAction<Forder> {
 4     
 5     @Override
 6     public Forder getModel() {
 7         model = (Forder) session.get("forder");
 8         return model;
 9     }
10  
11     //实现购物车（订单）与购物项（订单项）级联入库功能
12     public String save() {
13 //        // 把session中的购物项交给当前的model对象
14 //        Forder forder = (Forder) session.get("forder");
15 //        //model.setSorders(forder.getSorders());
16 //        forder.setAddress(model.getAddress());
17 //        forder.setName(model.getName());
18 //        forder.setPhone(model.getPhone());
19 //        forder.setRemark(model.getRemark());
20 //        forder.setUser((User)session.get("user"));
21 //        forder.setStatus(new Status(1));
22 //        forder.setPost(model.getPost());
23 //        //级联入库(需要在xml或者POJO的注解中配置)，需要sorder关联forder
24 //        //在SorderServiceImpl类中追加sorder.setForder(forder);
25 //        forderService.save(forder);
26         
27         model.setUser((User)session.get("user"));
28         model.setStatus(new Status(1));
29         forderService.save(model);
30         
31         return "bank";
32     }
33 }
```
从上面的代码中可以看出，有两种方法：第一种不覆写getModel方法（我注释掉的部分），这个方法比较笨，由于ForderAction继承了BaseAction，而BaseAction实现了ModelDriven接口，所以传过来的数据会封装到model中，model是BaseAction中的一个属性，然后我们需要将model中的信息全部传到session中的forder中，然后forder中数据有了才能与Sorder一起级联入库，不过这种方法有点笨……所以我们采用第二种方法，重写getModel方法，直接将forder赋给model即可，然后我们只要将model中级联的项添加一下即可，即上面非注释的代码。这样用户点击订单确认后，信息入库，跳转到支付页面（支付页面接下来要做，目前先随便跳转到一个jsp即可）。
2. 页面缓存问题
        现在订单信息的级联入库解决了，但是如果用户点击确认订单后，再后退，我们发现还是原来的订单确认页面，而且信息还是刚刚的信息，session也没关，也就是说相当于我又要确认订单信息，这明显是不妥的，也就是说，当用户点击确认订单后，我们不能让页面缓存，这样的话，当用户点击后退，就会显示页面已经失效了，我们让它跳到首页即可。
        我们知道，在前台jsp页面可以设置让浏览器不缓存数据，所以我们可以在前台confirm.jsp页面进行如下设置：
![](https://img2018.cnblogs.com/blog/893718/201811/893718-20181106170427198-1907123867.png)
但是问题没那么简单，仅仅这样做是不行的，这样做的话，用户点击后退是会出现页面已过期的提示，但是当用户刷新一下又不行了，又会显示缓存加载原来的数据。所以我们明白了一点，由于session还没关闭，session中有订单的信息forder，用户刷新一下肯定会继续拿到这个forder，就会显示原来的订单信息，所以仅仅在前台这样设置根本无法解决问题，我们在后台也要做相关处理才行。
        既然知道问题所在了，我们可以这样做：因为当用户点击确认订单后，会交给ForderAction，然后ForderAction处理完后会跳转到支付页面，我们可以在ForderAction中做些手脚：我们将session中原来的forder给清掉，那就不OK了么？这是可行的，但是考虑到后面支付的时候还是需要订单的相关信息，所以我们可以将session中原来的forder保存到另一个地方，然后将原来的forder清空，所以我们在上面的ForderAction中最后加上两行代码，如下：
```
1 @Controller("forderAction")
 2 @Scope("prototype")
 3 public class ForderAction extends BaseAction<Forder> {
 4     
 5     @Override
 6     public Forder getModel() {
 7         model = (Forder) session.get("forder");
 8         return model;
 9     }
10  
11     //实现购物车（订单）与购物项（订单项）级联入库功能
12     public String save() {
13 //        // 把session中的购物项交给当前的model对象
14 //        Forder forder = (Forder) session.get("forder");
15 //        //model.setSorders(forder.getSorders());
16 //        forder.setAddress(model.getAddress());
17 //        forder.setName(model.getName());
18 //        forder.setPhone(model.getPhone());
19 //        forder.setRemark(model.getRemark());
20 //        forder.setUser((User)session.get("user"));
21 //        forder.setStatus(new Status(1));
22 //        forder.setPost(model.getPost());
23 //        //级联入库(需要在xml或者POJO的注解中配置)，需要sorder关联forder
24 //        //在SorderServiceImpl类中追加sorder.setForder(forder);
25 //        forderService.save(forder);
26         
27         model.setUser((User)session.get("user"));
28         model.setStatus(new Status(1));
29         forderService.save(model);
30         
31         //此时购物车已经入库，那么原来session中的购物车就应该清空
32         session.put("oldForder", session.get("forder"));//先将原来的购物车信息保存下来，因为后面付款的时候还需要相关信息
33         session.put("forder", new Forder());//new一个新的空购物车（相当于清空了购物车），还可以方便用户再买~
34         return "bank";
35     }
36 }
```
        然后还没完，我们在前台确认订单页面也得加上下面的代码：
![](https://img2018.cnblogs.com/blog/893718/201811/893718-20181106170450721-120880676.png)
现在的逻辑就清楚了，首先到订单确认页面，forder是有数据的，所以不为空，这个判断无效的，当用户点击确认订单后，在ForderAction中我们将forder换成了一个空的Forder对象，也就是说原来的数据都没了（我们保存在session中另一个键值对中，供后面支付用），这样当用户点后退又回到刚刚订单确认页面时，那个判断就生效了，就会跳转到首页去，到这里，整个逻辑就完整了，页面缓存问题解决好了。
