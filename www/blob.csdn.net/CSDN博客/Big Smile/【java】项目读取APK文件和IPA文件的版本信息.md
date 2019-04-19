# 【java】项目读取APK文件和IPA文件的版本信息 - Big Smile - CSDN博客
2018年04月29日 20:04:45[王啸tr1912](https://me.csdn.net/tr1912)阅读数：802标签：[java																[apk																[ipa](https://so.csdn.net/so/search/s.do?q=ipa&t=blog)](https://so.csdn.net/so/search/s.do?q=apk&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        在公司项目中，在做版本管理的时候，遇到了需要识别被管理的APK(安卓应用包)和IPA(苹果应用包)的情况，需要在上传的时候识别并导入包的一些信息到数据库中，并作出管理。
# 一、如何识别信息
        在我开始研究这个的解决方法的时候，同事告诉我，网上有现成的工具类，可以直接读取各种安装包的信息。于是上网搜索了一下，果然解决方案很多，先不说优劣，从解决的思路上来，可以发现，他们的解决方法都是千篇一律的。
        和众多的项目一样，在苹果和安卓的项目中，也有很多的版本信息，那么这些都存储在哪里了呢？在每个我们打好的包里面，其实都有一个专门标注版本信息和版权，描述的文件，如果我们用winrar或者其他解压缩软件把安装包解压缩之后，我们可以得到里面的很多信息，然后一般我们都会在软件的最外层发现一个.xml的文件来看到我们的版本信息（一般都有加密）。
        所以我们获得版本信息的来源就是要从安装包中提取xml的文件，然后通过解析xml文件中的特殊节点来获得我们所需要的信息。具体思路就是  解包(类似zip)->读取文件->把信息放入构造的实体中->从实体中获得数据
        因为各种版本的安卓应用所用的AndroidStudio和SDK都是不同的，所以我们在解析的时候也会有稍许的不同，好在有兼容的存在，还有对于各种版本的识别网上都有处理。
# 二、开始动手
        我们在开始之前首先需要引入两个jar包，分别是dd-plist-1.16.jar(ios的识别)和apk-parser-2.5.3.jar(安卓的识别)
下载地址：[https://download.csdn.net/download/tr1912/10383172](https://download.csdn.net/download/tr1912/10383172)
我们在引入jar包的时候直接放入lib文件夹中就可以了。maven的话需要先上传到中央仓库去才能挂载。
然后新建一个工具类叫APKUtil，然后粘入如下代码：
```java
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import net.dongliu.apk.parser.ApkFile;
import net.dongliu.apk.parser.bean.ApkMeta;
import net.dongliu.apk.parser.bean.UseFeature;
  
public class APKUtil {
    public static void main(String[] arg){
	String filePath = "D:\\apk\\farmClient.apk";
        try (ApkFile apkFile = new ApkFile(new File(filePath))) {
            ApkMeta apkMeta = apkFile.getApkMeta();
            System.out.println(apkMeta.getName());
            System.out.println(apkMeta.getPackageName());
            System.out.println(apkMeta.getVersionCode());
            System.out.println(apkMeta.getVersionName());
            for (UseFeature feature : apkMeta.getUsesFeatures()) {
                System.out.println(feature.getName());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }                       
    }
	
    /**
    * 获取APK信息-王啸-2018年4月18日13:05:52
    * @param apkUrl
    * @return
    */
    public static Map<String,Object> readApk(String apkUrl){
	Map<String,Object> resMap=new HashMap<String,Object>();
	try (ApkFile apkFile = new ApkFile(new File(apkUrl))) {
            ApkMeta apkMeta = apkFile.getApkMeta();
            resMap.put("versionCode", apkMeta.getVersionCode());
            resMap.put("versionName", apkMeta.getVersionName());      
            for (UseFeature feature : apkMeta.getUsesFeatures()) {
                System.out.println(feature.getName());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return resMap;
    }
}
```
其中filePath为我们放apk文件的路径，然后直接运行，我们就可以看到我们想要看的版本信息：
![](https://img-blog.csdn.net/201804291954139?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上图是我识别了一个qq的安装apk的样子。
ipa文件同理，也是引入jar包后，写入如下代码：
```java
/**
 * 读取ipa
 */
public static Map<String,Object> readIPA(String ipaURL){
    Map<String,Object> map = new HashMap<String,Object>();
    try {
        File file = new File(ipaURL);
        InputStream is = new FileInputStream(file);
        InputStream is2 = new FileInputStream(file);
        ZipInputStream zipIns = new ZipInputStream(is);
        ZipInputStream zipIns2 = new ZipInputStream(is2);
        ZipEntry ze;
        ZipEntry ze2;
        InputStream infoIs = null;
        NSDictionary rootDict = null;
        String icon = null;
        while ((ze = zipIns.getNextEntry()) != null) {
            if (!ze.isDirectory()) {
                String name = ze.getName();
                if (null != name &&
                 name.toLowerCase().contains(".app/info.plist")) {
                    ByteArrayOutputStream _copy = new 
                                ByteArrayOutputStream();
                    int chunk = 0;
                    byte[] data = new byte[1024];
                    while(-1!=(chunk=zipIns.read(data))){
                        _copy.write(data, 0, chunk);
                    }
                    infoIs = new ByteArrayInputStream(_copy.toByteArray());
                    rootDict = (NSDictionary) PropertyListParser.parse(infoIs);
                    //我们可以根据info.plist结构获取任意我们需要的东西
                    //比如下面我获取图标名称，图标的目录结构请下面图片
                    //获取图标名称
                    NSDictionary iconDict = (NSDictionary) rootDict.get("CFBundleIcons");
                    while (null != iconDict) {
                        if(iconDict.containsKey("CFBundlePrimaryIcon")){
                        NSDictionary CFBundlePrimaryIcon = (NSDictionary)iconDict.get("CFBundlePrimaryIcon"); 
                            if(CFBundlePrimaryIcon.containsKey("CFBundleIconFiles")){
                            NSArray CFBundleIconFiles =(NSArray)CFBundlePrimaryIcon.get("CFBundleIconFiles"); 
                            icon = CFBundleIconFiles.getArray()[0].toString();
                                if(icon.contains(".png")){
                                    icon = icon.replace(".png", "");
                                }
                                System.out.println("获取icon名称:" + icon);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
       //根据图标名称下载图标文件到指定位置
        while ((ze2 = zipIns2.getNextEntry()) != null) {
            if (!ze2.isDirectory()) {
                String name = ze2.getName();
                System.out.println(name);
                if(name.contains(icon.trim())){
                    System.out.println(11111);
                    FileOutputStream fos = new FileOutputStream(new File("E:\\img\\icon.png"));
                       int chunk = 0;
                       byte[] data = new byte[1024];
                       while(-1!=(chunk=zipIns2.read(data))){
                           fos.write(data, 0, chunk);
                       }
                       fos.close();
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////
        //如果想要查看有哪些key ，可以把下面注释放开
//      for (String keyName : rootDict.allKeys()) {
//          System.out.println(keyName + ":" + rootDict.get(keyName).toString());
//      }
        // 应用包名
        NSString parameters = (NSString) rootDict.get("CFBundleIdentifier");
        map.put("package", parameters.toString());
        // 应用版本名
        parameters = (NSString) rootDict.objectForKey("CFBundleShortVersionString");
        map.put("versionName", parameters.toString());
        //应用版本号
        parameters = (NSString) rootDict.get("CFBundleVersion");
        map.put("versionCode", parameters.toString());
        /////////////////////////////////////////////////
        infoIs.close();
        is.close();
        zipIns.close();
    } catch (Exception e) {
        map.put("code", "fail");
        map.put("error","读取ipa文件失败");
    }
    return map;
}
```
main中的测试代码：
```java
System.out.println("======ipa==========");
String ipaUrl = "D:\\apk\\PastureRecreation.ipa";
Map<String,Object> mapIpa = APKUtil.readIPA(ipaUrl);
for (String key : mapIpa.keySet()) {
    System.out.println(key + ":" + mapIpa.get(key));
}
```
ipa的文件比较复杂，但是也能解析出来很多信息的，由于信息差不多，所以不再演示，自己体会去吧。
另外ios的包能解析出来应用的图标，大家可以尝试一下。
