# class.getClassLoader().getResource()和class.getResource()的不同 - z69183787的专栏 - CSDN博客
2013年12月03日 16:32:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1303
1.两者都是读取配置文件的方式。前者是正规的实现，即先由class获得classLoader，再由classLoader获得文件。后者是封装的一个方法，二者的区别在哪呢？在参数上。
2.二者的参数类型都是String，假设为path。如果读一个当前包中的文件test.properties，而包名是com.xxx；前者的参数path就是“com/xxx/test.properties”，后者的参数是“test.properties”，也就是后者不会写包名，为什么呐？
3.因为后者是class自己封装的方法，他的方法内部，实现了前面包名的添加，如何获得包名的呢？当然由class获取，例如一个程序放在com.xxx包中，名字是test.java。则由Test.class.getName（）方法就可以获得完整的包名加类名，然后用subString方法获得包名，再加上传入的文件名参数即可。
**[java]**[view
 plain](http://blog.csdn.net/knxw0001/article/details/8212017#)[copy](http://blog.csdn.net/knxw0001/article/details/8212017#)
- /**
-     * Add a package name prefix if the name is not absolute Remove leading "/"
-     * if name is absolute
-     */
- private String resolveName(String name) {  
- if (name == null) {  
- return name;  
-        }  
- if (!name.startsWith("/")) {  
-            Class c = this;  
- while (c.isArray()) {  
-                c = c.getComponentType();  
-            }  
-            String baseName = c.getName();  
- int index = baseName.lastIndexOf('.');  
- if (index != -1) {  
-                name = baseName.substring(0, index).replace('.', '/')  
-                    +"/"+name;  
-            }  
-        } else {  
-            name = name.substring(1);  
-        }  
- return name;  
-    }  
5.如果name不是绝对路径，就去掉“/”；如果是绝对的路径，就不要去掉“/”；当然写相对路径；可以看到后面会由calss获得baseName，再加上原参数name，返回完整的name，如果是绝对路径，删掉第一个“/”，返回即可。
