# Java反射代码 得到某类的所有字段 getFields()与getDeclaredFields()区别  getMethod()和getDeclaredMethod()区别 - z69183787的专栏 - CSDN博客
2014年04月16日 16:00:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7153
**
* 得到某类的所有字段
*
*/
public void reflect2(){
      try {
          Class cls = Class.forName("entity.UserBean");//加载UserBean类到内存中！获取一个Class对象
          Field[]fields = cls.getDeclaredFields();//通过class取出当前class的所有字段
           for (Field field : fields) { //完全等同于上面的for循环
                  System.out.println(field.getName()+" "+field.getType());
          }
     } catch (Exception e) {
         // TODO Auto-generated catch block
           e.printStackTrace();
     }
}
getFields()获得某个类的所有的公共（public）的字段，包括父类。 
getDeclaredFields()获得某个类的所有申明的字段，即包括public、private和proteced，但是不包括父类的申明字段。 
同样类似的还有getConstructors()和getDeclaredConstructors()，getMethods()和getDeclaredMethods()。
Method getDeclaredMethod(String name, Class… parameterTypes)d
          返回一个 Method 对象，该对象反映此 Class 对象所表示的类或接口的指定已声明方法。
Method[] getDeclaredMethods()
          返回 Method 对象的一个数组，这些对象反映此 Class 对象表示的类或接口声明的所有方法，包括公共、保护、默认（包）访问和私有方法，但不包括继承的方法。
Method getMethod(String name, Class… parameterTypes)
          返回一个 Method 对象，它反映此 Class 对象所表示的类或接口的指定公共成员方法。
Method[] getMethods()
          返回一个包含某些 Method 对象的数组，这些对象反映此 Class 对象所表示的类或接口（包括那些由该类或接口声明的以及从超类和超接口继承的那些的类或接口）的公共 member 方法。
getDeclaredField(String name)
          返回一个 Field 对象，该对象反映此 Class 对象所表示的类或接口的指定已声明字段。
Field[] getDeclaredFields()
          返回 Field 对象的一个数组，这些对象反映此 Class 对象所表示的类或接口所声明的所有字段，包括公共、保护、默认（包）访问和私有字段，但不包括继承的字段。
* getConstructors()与getDeclaredConstructors()区别:getConstructors()只能访问类中声明为public的构造函数.getDeclaredConstructors()能访问类中所有的构造函数,与public,private,protect无关
