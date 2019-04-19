# Hessian和Java反序列化问题小结 - 零度的博客专栏 - CSDN博客
2016年09月29日 10:30:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：1910
Hessian反序列化问题
众所周知，Hessian框架提供的序列化方式，在性能上要优于Java自己的序列化方式。他将对象序列化，生成的字节数组的数量要相对于Java自带的序列化方式要更简洁。
目前公司的一个项目中，有RPC调用的需要，这里我们使用了公司自己的开源RPC框架Dubbo作为远程调用框架，进行业务方法的调用和对象的序列化。这里，我们没有对Dubbo做出特殊配置，Dubbo在Remoting层组件默认的序列化方式就是采用的Hessian协议处理。但是在真正部署测试时，走到需要远程调用的方式时，报出了一下异常（只截取了最核心的异常堆栈）：
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- Caused by: com.alibaba.com.caucho.hessian.io.HessianProtocolException: 'com.alibaba.ais.bdc.person.vo.CountVO$CountObject' could not be instantiated  
-     at com.alibaba.com.caucho.hessian.io.JavaDeserializer.instantiate(JavaDeserializer.java:275)  
-     at com.alibaba.com.caucho.hessian.io.JavaDeserializer.readObject(JavaDeserializer.java:155)  
-     at com.alibaba.com.caucho.hessian.io.SerializerFactory.readObject(SerializerFactory.java:397)  
-     at com.alibaba.com.caucho.hessian.io.Hessian2Input.readObjectInstance(Hessian2Input.java:2070)  
-     at com.alibaba.com.caucho.hessian.io.Hessian2Input.readObject(Hessian2Input.java:2005)  
-     at com.alibaba.com.caucho.hessian.io.Hessian2Input.readObject(Hessian2Input.java:1990)  
-     at com.alibaba.com.caucho.hessian.io.CollectionDeserializer.readLengthList(CollectionDeserializer.java:93)  
-     at com.alibaba.com.caucho.hessian.io.Hessian2Input.readObject(Hessian2Input.java:1678)  
-     at com.alibaba.com.caucho.hessian.io.JavaDeserializer$ObjectFieldDeserializer.deserialize(JavaDeserializer.java:396)  
-     ... 42 more  
- Caused by: java.lang.reflect.InvocationTargetException  
-     at sun.reflect.NativeConstructorAccessorImpl.newInstance0(Native Method)  
-     at sun.reflect.NativeConstructorAccessorImpl.newInstance(NativeConstructorAccessorImpl.java:39)  
-     at sun.reflect.DelegatingConstructorAccessorImpl.newInstance(DelegatingConstructorAccessorImpl.java:27)  
-     at java.lang.reflect.Constructor.newInstance(Constructor.java:513)  
-     at com.alibaba.com.caucho.hessian.io.JavaDeserializer.instantiate(JavaDeserializer.java:271)  
-     ... 50 more  
- Caused by: java.lang.IllegalArgumentException: [Assertion failed] - this argument is required; it must not be null
-     at org.springframework.util.Assert.notNull(Assert.java:112)  
-     at org.springframework.util.Assert.notNull(Assert.java:123)  
-     at com.alibaba.ais.bdc.person.vo.CountVO$CountObject.<init>(CountVO.java:101)  
-     ... 55 more  
![](http://static.blog.csdn.net/images/save_snippets.png)
从最下面的异常信息可以看出，CountObject这个内部类在对象初始化时，报了参数校验的失败。这个看一下CountObject的出问题的构造函数就一目了然了：
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- public CountObject(SimplePerson simplePerson, String imagePrefix){  
-     Assert.notNull(simplePerson);  
- if (StringUtils.isEmpty(imagePrefix)) {  
- thrownew IllegalArgumentException("imagePrefix [" + imagePrefix + "] is meaningless.");  
-     }  
- this.id = simplePerson.getEmployeeId();  
- this.name = simplePerson.getRealName();  
- this.imagePath = StringUtils.isEmpty(simplePerson.getImagePathSuffix()) ? null : imagePrefix  
-                                                                                      + simplePerson.getImagePathSuffix();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
现在在构造函数的第一行的Assert就失败了。可是哪里调用这个构造函数导致失败呢？继续网上翻看异常堆栈给出的信息。可以看出在JavaDeserializer.instantiate中抛出了HessianProtocolException异常。进去看一下Hessian这块的源码如下：
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- protected Object instantiate()  
- throws Exception  
- {  
- try {  
- if (_constructor != null)  
- eturn _constructor.newInstance(_constructorArgs);  
- else
- eturn _type.newInstance();  
-   } catch (Exception e) {  
- thrownew HessianProtocolException("'" + _type.getName() + "' could not be instantiated", e);  
-   }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这里结合上面的异常堆栈可以知道，上面出问题的关键是_constructor和_constructorArgs。这两个东东又到底是啥呢？继续来看代码：
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- public JavaDeserializer(Class cl)  
-  {  
-    _type = cl;  
-    _fieldMap = getFieldMap(cl);  
- 
-    _readResolve = getReadResolve(cl);  
- 
- if (_readResolve != null) {  
-      _readResolve.setAccessible(true);  
-    }  
- 
-    Constructor []constructors = cl.getDeclaredConstructors();  
- long bestCost = Long.MAX_VALUE;  
- 
- for (int i = 0; i < constructors.length; i++) {  
-      Class []param = constructors[i].getParameterTypes();  
- long cost = 0;  
- 
- for (int j = 0; j < param.length; j++) {  
- cost = 4 * cost;  
- 
- if (Object.class.equals(param[j]))  
-   cost += 1;  
- elseif (String.class.equals(param[j]))  
-   cost += 2;  
- elseif (int.class.equals(param[j]))  
-   cost += 3;  
- elseif (long.class.equals(param[j]))  
-   cost += 4;  
- elseif (param[j].isPrimitive())  
-   cost += 5;  
- else
-   cost += 6;  
-      }  
- 
- if (cost < 0 || cost > (1 << 48))  
- cost = 1 << 48;  
- 
-      cost += (long) param.length << 48;  
- // _constructor will reference to the constructor with least parameters.
- if (cost < bestCost) {  
-        _constructor = constructors[i];  
-        bestCost = cost;  
-      }  
-    }  
- 
- if (_constructor != null) {  
-      _constructor.setAccessible(true);  
-      Class []params = _constructor.getParameterTypes();  
-      _constructorArgs = new Object[params.length];  
- for (int i = 0; i < params.length; i++) {  
-        _constructorArgs[i] = getParamArg(params[i]);  
-      }  
-    }  
-  }  
![](http://static.blog.csdn.net/images/save_snippets.png)
从JavaDeserializer的构造方法中可以看出，这里_constructor会被赋予参数最少的那个构造器。再回过头去看看CountObject的构造器（就上面列出来的那一个），不难看出，这里的_constructor就是上面的那个构造器了。
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- /**
-  * Creates a map of the classes fields.
-  */
- protectedstatic Object getParamArg(Class cl)  
- {  
- if (! cl.isPrimitive())  
- returnnull;  
- elseif (boolean.class.equals(cl))  
- return Boolean.FALSE;  
- elseif (byte.class.equals(cl))  
- returnnew Byte((byte) 0);  
- elseif (short.class.equals(cl))  
- returnnew Short((short) 0);  
- elseif (char.class.equals(cl))  
- returnnew Character((char) 0);  
- elseif (int.class.equals(cl))  
- return Integer.valueOf(0);  
- elseif (long.class.equals(cl))  
- return Long.valueOf(0);  
- elseif (float.class.equals(cl))  
- return Float.valueOf(0);  
- elseif (double.class.equals(cl))  
- return Double.valueOf(0);  
- else
- thrownew UnsupportedOperationException();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
参看上面的getParamArg方法，就可以知道，由于CountObject唯一的一个构造器的两个参数都不是基本类型，所以这里_constructorArgs所包含的值全部是null。
OK，到这里，上面的异常就搞清楚了，Hessian反序列化时，使用反射调用构造函数生成对象时，传入的参数不合法，造成了上面的异常。知道了原因，解决的方法也很简单，就是添加了一个无参的构造器给CountObject，于是上面的问题就解决了。。。
这里，需要注意的是，如果序列化机制使用的是Hessian，序列化的对象又没有提供默认的无参构造器时，需要注意上面类似的问题了。
Java本身反序列化问题
Java本身的反序列化机制虽然性能稍差一些，但本身使用的约束条件相对却要宽松一些，其实只要满足下面两条，一个类对象就是可以完美支持序列化机制了：
- 类实现java.io.Serializable接口。
- 类包含的所有属性都是实现了java.io.Serializable接口的，或者被标记为了transient。
对于构造函数本身没有任何约束。这里，Java序列化本身其实也是和new以及Java反射机制“平级”的实例化对象的方式。所以，对于单例模式的场景，还是需要考虑是否会有序列化因素造成的单例失效（因为他实例化对象不依赖于构造器，所以一个private的构造器显然没法阻止他的“胡作非为”）。当然，对于这种情况，也可以自己实现下面的方法：
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- private Object readResolve()  
![](http://static.blog.csdn.net/images/save_snippets.png)
通过实现上面的方法，自己可以在其中明确指定，放回的对象的实例是哪一个。但对于通过如上方式保证的单例模式至少需要注意一下两点：
- readResolve方法的可见性（public/protected/private）问题：因为如果这个方法不是private的，就有可能被起子类直接继承过去。这可能造成你在反序列化子类对象时出错（因为这个方法返回了父类的某个固定的对象）。
- 使用readResolve方法时，往往比较容易返回某个固定的对象。但这其实和真正的对象反序列化其实是有点矛盾的。因为你反序列化对象时，多数场景都是希望恢复原来的对象的“状态”，而不是固定的某个对象。所以只要你的类内的属性有没有被标识成transient的，就要格外小心了。
鉴于上面所说的稍微复杂的现象，如果单纯的考虑单例的需要，更好的方式是通过枚举来实现，因为枚举至少可以在JVM层面，帮你保证每个枚举实例一定是单例的，即使使用反序列化机制，也无法绕过这个限制，所以可以帮你省不少心。
好了，上面扯的有点远了，关于Java本身的序列化机制，下面写了一个简单的把对象序列化成字节数组，再由字节数组反序列化回来的例子，看完之后应该会更明了一些：
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- publicclass Person implements Serializable {  
- 
-     String name;  
- int age;  
- 
- public Person(String name, int age) {  
- this.name = name;  
- this.age = age;  
-     }  
- 
- @Override
- public String toString() {  
- return"Person{" +  
- "name='" + name + '\'' +  
- ", age=" + age +  
- '}';  
-     }  
- 
- privatestaticclass Employee extends Person{  
- 
-         String title;  
- 
- private Employee(String name, int age, String title) {  
- super(name, age);  
- this.title = title;  
-         }  
- 
- @Override
- public String toString() {  
- return"Employee{" + "name='" + name + '\'' +  
- ", age=" + age + '\'' +  
- ", title='" + title + '\'' +  
- '}';  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) {  
- byte[] bytes;  
-         Person person1 = new Person( "test1",20 );  
-         Person person2;  
-         Employee employee1 = new Employee( "employee1",25,"Manager" );  
-         Employee employee2;  
- 
-         ByteArrayOutputStream byteOutputStream = null;  
-         ObjectOutputStream objectOutputStream = null;  
- 
-         ByteArrayInputStream byteArrayInputStream = null;  
-         ObjectInputStream objectInputStream = null;  
- 
- try {  
- //generate byteArray.
-             byteOutputStream = new ByteArrayOutputStream( );  
-             objectOutputStream = new ObjectOutputStream( byteOutputStream);  
- //serialize person1
-             objectOutputStream.writeObject( person1 );  
- //serialize employee1
-             objectOutputStream.writeObject( employee1 );  
- 
-             bytes = byteOutputStream.toByteArray();  
- 
- for (byte aByte : bytes) {  
-                 System.out.print(aByte);  
-             }  
-             System.out.println();  
-             System.out.println("Bytes's length is :"+bytes.length);  
- 
- //generate Object from byteArray.
-             byteArrayInputStream = new ByteArrayInputStream( bytes );  
-             objectInputStream = new ObjectInputStream( byteArrayInputStream );  
- //deserialize person1
-             person2 = (Person)objectInputStream.readObject();  
- //deserialize employee1
-             employee2 = (Employee)objectInputStream.readObject();  
-             System.out.println("person2 got from byteArray is : "+person2);  
-             System.out.println("employee2 got from byteArray is : "+employee2);  
- 
-             System.out.println("person1's memory id :"+Integer.toHexString(person1.hashCode()));  
-             System.out.println("person2's memory id :"+Integer.toHexString(person2.hashCode()));  
-             System.out.println("employee1's memory id :"+Integer.toHexString(employee1.hashCode()));  
-             System.out.println("employee2's memory id :"+Integer.toHexString(employee2.hashCode()));  
- 
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }catch ( ClassNotFoundException ce ){  
-             ce.printStackTrace();  
-         }  
- finally {  
- try {  
-                 byteOutputStream.close();  
-                 objectOutputStream.close();  
-                 byteArrayInputStream.close();  
-                 objectInputStream.close();  
-             } catch (IOException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面代码执行的结果如下：
Java代码  ![收藏代码](http://hittyt.iteye.com/images/icon_star.png)
- -84-19051151140329911110946115107121461191191194611510111410597108105122971161051111104680101114115111110-97-123-26-11-111120-40-115202730397103101760411097109101116018761069711897471089711010347831161141051101035912011200020116051161011151164911511404199111109461151071214611911911946115101114105971081051229711610511111046801011141151111103669109112108111121101101-11-66110-28-62-10611536201760511610511610810111301260112011301260000025116091011091121081111211011014911607779711097103101114
- Bytes's length is :200
- person2 got from byteArray is : Person{name='test1', age=20}  
- employee2 got from byteArray is : Employee{name='employee1', age=25', title='Manager'}  
- person1's memory id :29173ef  
- person2's memory id :96fa474  
- employee1's memory id :6c121f1d  
- employee2's memory id :95c083  
![](http://static.blog.csdn.net/images/save_snippets.png)
最后再补充一个Java序列化规范的地址，有时间时再细读一下：[http://docs.oracle.com/javase/7/docs/platform/serialization/spec/serial-arch.html](http://docs.oracle.com/javase/7/docs/platform/serialization/spec/serial-arch.html)
