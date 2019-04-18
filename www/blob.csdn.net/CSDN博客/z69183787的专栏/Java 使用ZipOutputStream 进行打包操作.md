# Java 使用ZipOutputStream 进行打包操作 - z69183787的专栏 - CSDN博客
2012年12月13日 14:58:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：27843
打包单个文件：
```java
public static void main(String[] args) throws IOException{
        File file = new File("d:" + File.separator + "aaa.java");
        File zipFile = new File("d:" + File.separator + "hello.zip");
        InputStream input = new FileInputStream(file);
        ZipOutputStream zipOut = new ZipOutputStream(new FileOutputStream(
                zipFile));
        zipOut.putNextEntry(new ZipEntry(file.getName()));
        // 设置注释
        zipOut.setComment("hello");
        int temp = 0;
        while((temp = input.read()) != -1){
            zipOut.write(temp);
        }
        input.close();
        zipOut.close();
    }
```
打包多个文件：
```java
public static void main(String[] args) throws IOException{
        // 要被压缩的文件夹
        File file = new File("d:" + File.separator + "temp");
        File zipFile = new File("d:" + File.separator + "zipFile.zip");
        InputStream input = null;
        ZipOutputStream zipOut = new ZipOutputStream(new FileOutputStream(
                zipFile));
        zipOut.setComment("hello");
        if(file.isDirectory()){
            File[] files = file.listFiles();
            for(int i = 0; i < files.length; ++i){
                input = new FileInputStream(files[i]);
                zipOut.putNextEntry(new ZipEntry(file.getName()
                        + File.separator + files[i].getName()));
                int temp = 0;
                while((temp = input.read()) != -1){
                    zipOut.write(temp);
                }
                input.close();
            }
        }
        zipOut.close();
    }
```
大家自然想到，既然能压缩，自然能解压缩，在谈解压缩之前，我们会用到一个ZipFile类，先给一个这个例子吧。java中的每一个压缩文件都是可以使用ZipFile来进行表示的
```java
import java.io.File;
import java.io.IOException;
import java.util.zip.ZipFile;
 
/**
 * ZipFile演示
 * */
public class ZipFileDemo{
    public static void main(String[] args) throws IOException{
        File file = new File("d:" + File.separator + "hello.zip");
        ZipFile zipFile = new ZipFile(file);
        System.out.println("压缩文件的名称为：" + zipFile.getName());
    }
}
```
解压单个文件：
```java
public static void main(String[] args) throws IOException{
        File file = new File("d:" + File.separator + "hello.zip");
        File outFile = new File("d:" + File.separator + "unZipFile.txt");
        ZipFile zipFile = new ZipFile(file);
        ZipEntry entry = zipFile.getEntry("hello.txt");//hello.txt 为压缩包中文件的名称
        InputStream input = zipFile.getInputStream(entry);
        OutputStream output = new FileOutputStream(outFile);
        int temp = 0;
        while((temp = input.read()) != -1){
            output.write(temp);
        }
        input.close();
        output.close();
    }
```
解压多个文件：
当我们需要解压缩多个文件的时候，ZipEntry就无法使用了，如果想操作更加复杂的压缩文件，我们就必须使用ZipInputStream类
```java
public static void main(String[] args) throws IOException{
        File file = new File("d:" + File.separator + "zipFile.zip");
        File outFile = null;
        ZipFile zipFile = new ZipFile(file);
        ZipInputStream zipInput = new ZipInputStream(new FileInputStream(file));
        ZipEntry entry = null;
        InputStream input = null;
        OutputStream output = null;
        while((entry = zipInput.getNextEntry()) != null){
            System.out.println("解压缩" + entry.getName() + "文件");//entry.getName()获得压缩包内的文件及文件路径（aaaa/bb.txt）
            outFile = new File("d:" + File.separator + entry.getName());
            if(!outFile.getParentFile().exists()){
                outFile.getParentFile().mkdir();
            }
            if(!outFile.exists()){
                outFile.createNewFile();
            }
            input = zipFile.getInputStream(entry);
            output = new FileOutputStream(outFile);
            int temp = 0;
            while((temp = input.read()) != -1){
                output.write(temp);
            }
            input.close();
            output.close();
        }
    }
```
