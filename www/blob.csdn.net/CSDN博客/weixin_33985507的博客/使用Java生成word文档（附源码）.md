# 使用Java生成word文档（附源码） - weixin_33985507的博客 - CSDN博客
2013年11月25日 14:15:48[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
　　当我们使用Java生成word文档时，通常首先会想到iText和POI，这是因为我们习惯了使用这两种方法操作Excel，自然而然的也想使用这种生成word文档。但是当我们需要动态生成word时，通常不仅要能够显示word中的内容，还要能够很好的保持word中的复杂样式。这时如果再使用IText和POI去操作，就好比程序员去搬砖一样痛苦。
　　这时候，我们应该考虑使用FreeMarker的模板技术快速实现这个复杂的功能，让程序员在喝咖啡的过程中就把问题解决。实现思路是这样的：先创建一个word文档，按照需求在word中填好一个模板，然后把对应的数据换成变量${}，然后将文档保存为xml文档格式，使用文档编辑器打开这个xml格式的文档，去掉多余的xml符号，使用Freemarker读取这个文档然后替换掉变量，输出word文档即可。
　　具体过程如下：
　　1.创建带有格式的word文档，将该需要动态展示的数据使用变量符替换。
![140726908.png](http://blog.51cto.com/attachment/201311/140726908.png)
2. 将刚刚创建的word文档另存为xml格式。
![140752129.png](http://blog.51cto.com/attachment/201311/140752129.png)
3．编辑这个XMl文档去掉多余的xml标记，如图中蓝色部分
![140826202.png](http://blog.51cto.com/attachment/201311/140826202.png)
　　4．从[Freemarker](http://freemarker.org/)官网[【下载】](http://sourceforge.net/projects/freemarker/files/freemarker/2.3.20/freemarker-2.3.20.tar.gz/download)最新的开发包，将freemarker.jar拷贝到自己的开发项目中。
　　5．新建DocUtil类，实现根据Doc模板生成word文件的方法
```java
package com.favccxx.secret.util;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Map;
import freemarker.template.Configuration;
import freemarker.template.DefaultObjectWrapper;
import freemarker.template.Template;
import freemarker.template.TemplateExceptionHandler;
public class DocUtil {
       privateConfiguration configure = null;
       publicDocUtil(){
              configure= new Configuration();
              configure.setDefaultEncoding("utf-8");
       }
       /**
        * 根据Doc模板生成word文件
        * @param dataMap Map 需要填入模板的数据
        * @param fileName 文件名称
        * @param savePath 保存路径
        */
       publicvoid createDoc(Map<String, Object> dataMap, String downloadType, StringsavePath){
              try{
                     //加载需要装填的模板
                     Templatetemplate  = null;
                     //加载模板文件
                     configure.setClassForTemplateLoading(this.getClass(),"/com/favccxx/secret/templates");
                     //设置对象包装器
                     configure.setObjectWrapper(newDefaultObjectWrapper());
                     //设置异常处理器
                     configure.setTemplateExceptionHandler(TemplateExceptionHandler.IGNORE_HANDLER);
                     //定义Template对象,注意模板类型名字与downloadType要一致
                     template= configure.getTemplate(downloadType + ".xml");
                     //输出文档
                     FileoutFile = new File(savePath);
                     Writerout = null;
                     out= new BufferedWriter(new OutputStreamWriter(new FileOutputStream(outFile),"utf-8"));                                    
                     template.process(dataMap,out);
                     outFile.delete();
              }catch (Exception e) {
                     e.printStackTrace();
              }
       }
}
```
　　6．用户根据自己的需要，调用使用getDataMap获取需要传递的变量，然后调用createDoc方法生成所需要的文档。
```java
/**
 * 根据下载类型获取需要传递的Map参数
 * @param oid 对象Id
 * @param downloadType 下载类型
 */
private Map<String, Object> getDataMap(String oid,String downloadType){
    Map<String, Object> dataMap = new HashMap<String, Object>();
    if("Parameter1".equals(downloadType)){
        ...
        ...
        dataMap = DataMapUtil.setObjToMap(Object1);
    }else{
        ...
        ...
        dataMap = DataMapUtil.setObjToMap(Object2);
    }
    return dataMap;
}
```
```java
public class DataMapUtil {
      
    private static Map<String, Object> dataMap = new HashMap<String, Object>();
      
    /**
     * 将对象转换成Map
     * @param obj 对象类
     * @return
     */
    public static Map<String,Object> setObjToMap(Object obj){
        Class c;
        try {
            c = Class.forName(obj.getClass().getName());
            Method[] methods = c.getMethods();
            for(int i=0,l=methods.length;i<l;i++){
                String method = methods[i].getName();
                System.out.println("The method is:" + method);
                if(method.startsWith("get")){
                    Object value = methods[i].invoke(obj);
                    if(value != null){
                        if(value.getClass().getClassLoader() != null){  //处理自定义的对象类型
                            setObjToMap(value);
                        }
                        String key = method.substring(3);
                        key = key.substring(0, 1).toLowerCase() + key.substring(1);
                        if("java.util.Date".equals(value.getClass().getName())){
                            value = DateUtil.dateToString((Date)value);
                        }
                        dataMap.put(key, value);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return dataMap;
    }
}
```
　　7．赶紧把这个方法，应用到自己的项目中吧。
