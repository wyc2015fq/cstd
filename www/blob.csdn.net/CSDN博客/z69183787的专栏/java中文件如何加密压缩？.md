# java中文件如何加密压缩？ - z69183787的专栏 - CSDN博客
2019年01月25日 11:31:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：174
[https://blog.csdn.net/qq_29175301/article/details/83444557](https://blog.csdn.net/qq_29175301/article/details/83444557)
终于等到你文件加密压缩1.单个文件加密压缩2.多文件加密压缩
文件加密压缩
在现如今已经可以实现大部分功能的互联网需求中，安全成为了需要的非必需品，工作需求中会遇到对导出的 excel 报表 做加密压缩的问题，今天有时间总结一下，我这里简单列举功能代码，连带其它代码文末会一并给出。
1.单个文件加密压缩
单个文件加密方法，需传输 3 个参数：被压缩源文件，压缩目标文件和加密密钥，如下
```java
/**
     * @Title: encrypt_zip 
     * @Description:将指定路径下的文件压缩至指定zip文件，并以指定密码加密,若密码为空，则不进行加密保护 
     * @param src_file 待压缩文件路径
     * @param dst_file zip路径+文件名 
     * @param encode 加密密码  
     * @return 
     */
    public static void encrypt_zip(String src_file, String dst_file, String encode) {
        File file = new File(src_file);
        ZipParameters parameters = new ZipParameters();
        parameters.setCompressionMethod(Zip4jConstants.COMP_DEFLATE);//压缩方式
        parameters.setCompressionLevel(Zip4jConstants.DEFLATE_LEVEL_NORMAL); // 压缩级别
        parameters.setEncryptFiles(true);
        parameters.setEncryptionMethod(Zip4jConstants.ENC_METHOD_STANDARD);//加密方式
        parameters.setPassword(encode.toCharArray());//设置密码
        try {
            ZipFile zipFile = new ZipFile(dst_file);
            zipFile.setFileNameCharset("gbk");
            zipFile.addFile(file, parameters);
        } catch (ZipException e) {
            e.printStackTrace();
        }
    }
```
1234567891011121314151617181920212223242526272829
2.多文件加密压缩
多文件加密压缩和单文件类似，但是，这个方法包含上面的方法，甚至功能上可以替换上面方法，具体看下 code
```java
/**
     * @Title: zipFilesAndEncrypt 
     * @Description: 将指定路径下的文件压缩至指定zip文件，并以指定密码加密,若密码为空，则不进行加密保护 
     * @param srcFileName 待压缩文件路径 或 文件夹路径
     * @param zipFileName zip路径+文件名 
     * @param password 加密密码  
     * @return 
     */
    public static void zipFilesAndEncrypt(String srcFileName,String zipFileName,String password){  
        
        if(StrTool.is_empty(srcFileName) || StrTool.is_empty(zipFileName)){  
            System.out.println("请求的压缩路径或者文件名有误");
            return;
        }  
        try {  
            ZipParameters parameters = new ZipParameters();  
            parameters.setCompressionMethod(Zip4jConstants.COMP_DEFLATE);//压缩方式
            parameters.setCompressionLevel(Zip4jConstants.DEFLATE_LEVEL_NORMAL);//压缩级别
            if(!StrTool.is_empty(password)){  
                parameters.setEncryptFiles(true);     
                parameters.setEncryptionMethod(Zip4jConstants.ENC_METHOD_STANDARD);//加密方式
                parameters.setPassword(password);  
            }  
            
            ArrayList<File> filesToAdd=getFiles(srcFileName);
            
            ZipFile zipFile = new ZipFile(zipFileName); 
            zipFile.setFileNameCharset("gbk");
            zipFile.addFiles(filesToAdd, parameters); 
        } catch (ZipException e) {  
            System.out.println("文件压缩出错");
            e.printStackTrace();
        } 
    }
    
    //通过路径获取压缩文件集合
    private static ArrayList<File> getFiles(String srcFileName){
        ArrayList<File> filesToAdd = new ArrayList<File>();
        File file=new File(srcFileName);
        File[] files = new File[0]; 
        if(file.isDirectory()){
            files = file.listFiles();
            for(int i=0;i<files.length;i++){
                if(files[i].getName().endsWith(".xls")){//xls结尾的excel添加压缩集合
                    filesToAdd.add(new File(files[i].getPath()));
                    System.out.println("文件："+files[i].getPath());
                }
            }
        } else {
            filesToAdd.add(new File(file.getPath()));
        }
        
        return filesToAdd;
    }
```
