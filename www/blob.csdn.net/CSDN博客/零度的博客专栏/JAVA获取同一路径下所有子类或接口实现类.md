# JAVA获取同一路径下所有子类或接口实现类 - 零度的博客专栏 - CSDN博客
2016年05月04日 17:32:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：3021
```
package zmx.util;
import java.io.File;  
import java.io.IOException;  
import java.net.URL;  
import java.util.ArrayList;  
import java.util.List;  
import zmx.spring.aop.test.IHelloWorldService;
   
  
public class ClassUtil {  
 
  
    /** 
     * 获取同一路径下所有子类或接口实现类 
     *  
     * @param intf 
     * @return 
     * @throws IOException 
     * @throws ClassNotFoundException 
     */  
    public static List<Class<?>> getAllAssignedClass(Class<?> cls) throws IOException,  
            ClassNotFoundException {  
        List<Class<?>> classes = new ArrayList<Class<?>>();  
        for (Class<?> c : getClasses(cls)) {  
            if (cls.isAssignableFrom(c) && !cls.equals(c)) {  
                classes.add(c);  
            }  
        }  
        return classes;  
    }  
  
    /** 
     * 取得当前类路径下的所有类 
     *  
     * @param cls 
     * @return 
     * @throws IOException 
     * @throws ClassNotFoundException 
     */  
    public static List<Class<?>> getClasses(Class<?> cls) throws IOException,  
            ClassNotFoundException {  
        String pk = cls.getPackage().getName();  
        String path = pk.replace('.', '/');  
        ClassLoader classloader = Thread.currentThread().getContextClassLoader();  
        URL url = classloader.getResource(path);  
        return getClasses(new File(url.getFile()), pk);  
    }  
  
    /** 
     * 迭代查找类 
     * @param dir 
     * @param pk 
     * @return 
     * @throws ClassNotFoundException 
     */  
    private static List<Class<?>> getClasses(File dir, String pk) throws ClassNotFoundException {  
        List<Class<?>> classes = new ArrayList<Class<?>>();  
        if (!dir.exists()) {  
            return classes;  
        }  
        for (File f : dir.listFiles()) {  
            if (f.isDirectory()) {  
                classes.addAll(getClasses(f, pk + "." + f.getName()));  
            }  
            String name = f.getName();  
            if (name.endsWith(".class")) {  
                classes.add(Class.forName(pk + "." + name.substring(0, name.length() - 6)));  
            }  
        }  
        return classes;  
    }
    
    
    public static void main(String[] args) {  
        try {  
            System.out.println("获取所有子类和实现类：");  
            for (Class<?> c : getAllAssignedClass(IHelloWorldService.class)) {  
                System.out.println(c.getName());  
            }  
            System.out.println("获取所有类：");  
            for (Class<?> c : getClasses(IHelloWorldService.class)) {  
                System.out.println(c.getName());  
            }  
        } catch (ClassNotFoundException e) {            
            e.printStackTrace();  
        } catch (IOException e) {              
            e.printStackTrace();  
        }  
    } 
    
    
    
  
}
```
输出：
获取所有子类和实现类：
zmx.spring.aop.test.HelloWorldService
获取所有类：
zmx.spring.aop.test.HelloWorldAspect
zmx.spring.aop.test.HelloWorldService
zmx.spring.aop.test.IHelloWorldService
zmx.spring.aop.test.LoggerAspect
zmx.spring.aop.test.MainTest
