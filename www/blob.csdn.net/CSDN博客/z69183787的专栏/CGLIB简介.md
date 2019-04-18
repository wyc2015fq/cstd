# CGLIB简介 - z69183787的专栏 - CSDN博客
2016年10月27日 06:53:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：722
CGlib是什么？
CGlib是一个强大的,高性能,高质量的Code生成类库。它可以在运行期扩展Java类与实现Java接口。
当然这些实际的功能是asm所提供的，asm又是什么？Java字节码操控框架，具体是什么大家可以上网查一查，毕竟我们这里所要讨论的是cglib，
cglib就是封装了asm，简化了asm的操作，实现了在运行期动态生成新的class。
可能大家还感觉不到它的强大，现在就告诉你。
实际上CGlib为spring aop提供了底层的一种实现;为hibernate使用cglib动态生成VO/PO (接口层对象)。
下面我们将通过一个具体的事例来看一下CGlib体验一下CGlib。
* CGlib 2.13
* ASM 2.23
以一个实例在简单介绍下cglib的应用。
我们模拟一个虚拟的场景，模拟对表的操作。
1. 开始我们对表提供了CRUD方法。
我们现在创建一个对Table操作的DAO类。
Java代码  ![收藏代码](http://llying.iteye.com/images/icon_star.png)
- publicclass TableDAOFactory {  
- privatestatic TableDAO tDao = new TableDAO();  
- publicstatic TableDAO getInstance(){  
- return tDao;  
-     }  
- }  
接下来我们创建客户端，用来调用CRUD方法。
Java代码  ![收藏代码](http://llying.iteye.com/images/icon_star.png)
- publicclass AuthProxy implements MethodInterceptor {  
- private String name ;  
- //传入用户名称
- public AuthProxy(String name){  
- this.name = name;  
-     }  
- public Object intercept(Object arg0, Method arg1, Object[] arg2,  
-             MethodProxy arg3) throws Throwable {  
- //用户进行判断
- if(!"张三".equals(name)){  
-             System.out.println("你没有权限！");  
- returnnull;  
-         }  
- return arg3.invokeSuper(arg0, arg2);  
-     }  
- }  
当然不能忘了对我们的dao工厂进行修改，我们提供一个使用代理的实例生成方法
Java代码  ![收藏代码](http://llying.iteye.com/images/icon_star.png)
- publicstaticvoid haveAuth(){  
-     TableDAO tDao = TableDAOFactory.getAuthInstance(new AuthProxy("张三"));  
-     doMethod(tDao);  
- }  
- publicstaticvoid haveNoAuth(){  
-     TableDAO tDao = TableDAOFactory.getAuthInstance(new AuthProxy("李四"));  
-     doMethod(tDao);  
- }  
OK,"张三"的正常执行，"李四"的没有执行。
看到了吗？简单的aop就这样实现了
难道就这样结束了么？
3. Boss又来训话了，不行不行，现在除了"张三"其他人都用不了了，现在不可以这样。他们都来向我反映了，必须使用开放查询功能。
哈哈，现在可难不倒我们了，因为我们使用了CGlib。当然最简单的方式是去修改我们的方法拦截器，不过这样会使逻辑变得复杂，且
不利于维护。还好CGlib给我们提供了方法过滤器（CallbackFilter）,CallbackFilte可以明确表明，被代理的类中不同的方法，
被哪个拦截器所拦截。下面我们就来做个过滤器用来过滤query方法。
Java代码  ![收藏代码](http://llying.iteye.com/images/icon_star.png)
- publicstatic TableDAO getAuthInstanceByFilter(AuthProxy authProxy){  
-     Enhancer en = new Enhancer();  
-     en.setSuperclass(TableDAO.class);  
-     en.setCallbacks(new Callback[]{authProxy,NoOp.INSTANCE});  
-     en.setCallbackFilter(new AuthProxyFilter());  
- return (TableDAO)en.create();  
- }  
看到了吗setCallbacks中定义了所使用的拦截器，其中NoOp.INSTANCE是CGlib所提供的实际是一个没有任何操作的拦截器，
他们是有序的。一定要和CallbackFilter里面的顺序一致。明白了吗？上面return返回的就是返回的顺序。也就是说如果调用query方法就使用NoOp.INSTANCE进行拦截。
现在看一下客户端代码。
<div class="dp-highlighter" dejavu="" sans="" mono",="" "bitstream="" vera="" consolas,="" "courier="" new",="" monospace;="" margin-left:="" 9px;="" font-size:="" 12px;="" padding:="" 1px;"="" style="font-family: Monaco, 'DejaVu Sans Mono', 'Bitstream Vera
 Sans Mono', Consolas, 'Courier New', monospace; width: 694px; overflow: auto; margin-left: 9px; padding: 1px; word-break: break-all; word-wrap: break-word; line-height: 25px; background-color: transparent;">
Java代码  ![收藏代码](http://llying.iteye.com/images/icon_star.png)
- publicstaticvoid haveAuthByFilter(){  
-     TableDAO tDao = TableDAOFactory.getAuthInstanceByFilter(new AuthProxy("张三"));  
-     doMethod(tDao);  
- 
-     tDao = TableDAOFactory.getAuthInstanceByFilter(new AuthProxy("李四"));  
-     doMethod(tDao);  
- }  
ok,现在"李四"也可以使用query方法了，其他方法仍然没有权限。
哈哈，当然这个代理的实现没有任何侵入性，无需强制让dao去实现接口
