# java复制文件或文件夹 - z69183787的专栏 - CSDN博客
2012年10月27日 10:47:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：738
个人分类：[文件处理-读写](https://blog.csdn.net/z69183787/article/category/2229757)
```java
package com.xuanwu.mtoserver.util;
import java.io.*;
/**
 * @author Toby 复制文件夹或文件夹
 */
public class FileUtil {
    public static void main(String args[]) throws IOException {
        // 源文件夹
        String url1 = "D:/user/test/";
        // 目标文件夹
        String url2 = "D:/user/testcopy/";
        // 创建目标文件夹
        (new File(url2)).mkdirs();
        // 获取源文件夹当前下的文件或目录
        File[] file = (new File(url1)).listFiles();
        for (int i = 0; i < file.length; i++) {
            if (file[i].isFile()) {
                // 复制文件
                String type = file[i].getName().substring(file[i].getName().lastIndexOf(".") + 1);
                if (type.equalsIgnoreCase("txt"))// 转码处理
                    copyFile(file[i], new File(url2 + file[i].getName()), MTOServerConstants.CODE_UTF_8, MTOServerConstants.CODE_GBK);
                else
                    copyFile(file[i], new File(url2 + file[i].getName()));
            }
            if (file[i].isDirectory()) {
                // 复制目录
                String sourceDir = url1 + File.separator + file[i].getName();
                String targetDir = url2 + File.separator + file[i].getName();
                copyDirectiory(sourceDir, targetDir);
            }
        }
    }
    // 复制文件
    public static void copyFile(File sourceFile, File targetFile) throws IOException {
        BufferedInputStream inBuff = null;
        BufferedOutputStream outBuff = null;
        try {
            // 新建文件输入流并对它进行缓冲
            inBuff = new BufferedInputStream(new FileInputStream(sourceFile));
            // 新建文件输出流并对它进行缓冲
            outBuff = new BufferedOutputStream(new FileOutputStream(targetFile));
            // 缓冲数组
            byte[] b = new byte[1024 * 5];
            int len;
            while ((len = inBuff.read(b)) != -1) {
                outBuff.write(b, 0, len);
            }
            // 刷新此缓冲的输出流
            outBuff.flush();
        } finally {
            // 关闭流
            if (inBuff != null)
                inBuff.close();
            if (outBuff != null)
                outBuff.close();
        }
    }
    // 复制文件夹
    public static void copyDirectiory(String sourceDir, String targetDir) throws IOException {
        // 新建目标目录
        (new File(targetDir)).mkdirs();
        // 获取源文件夹当前下的文件或目录
        File[] file = (new File(sourceDir)).listFiles();
        for (int i = 0; i < file.length; i++) {
            if (file[i].isFile()) {
                // 源文件
                File sourceFile = file[i];
                // 目标文件
                File targetFile = new File(new File(targetDir).getAbsolutePath() + File.separator + file[i].getName());
                copyFile(sourceFile, targetFile);
            }
            if (file[i].isDirectory()) {
                // 准备复制的源文件夹
                String dir1 = sourceDir + "/" + file[i].getName();
                // 准备复制的目标文件夹
                String dir2 = targetDir + "/" + file[i].getName();
                copyDirectiory(dir1, dir2);
            }
        }
    }
    /**
     * 
     * @param srcFileName
     * @param destFileName
     * @param srcCoding
     * @param destCoding
     * @throws IOException
     */
    public static void copyFile(File srcFileName, File destFileName, String srcCoding, String destCoding) throws IOException {// 把文件转换为GBK文件
        BufferedReader br = null;
        BufferedWriter bw = null;
        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream(srcFileName), srcCoding));
            bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(destFileName), destCoding));
            char[] cbuf = new char[1024 * 5];
            int len = cbuf.length;
            int off = 0;
            int ret = 0;
            while ((ret = br.read(cbuf, off, len)) > 0) {
                off += ret;
                len -= ret;
            }
            bw.write(cbuf, 0, off);
            bw.flush();
        } finally {
            if (br != null)
                br.close();
            if (bw != null)
                bw.close();
        }
    }
    /**
     * 
     * @param filepath
     * @throws IOException
     */
    public static void del(String filepath) throws IOException {
        File f = new File(filepath);// 定义文件路径
        if (f.exists() && f.isDirectory()) {// 判断是文件还是目录
            if (f.listFiles().length == 0) {// 若目录下没有文件则直接删除
                f.delete();
            } else {// 若有则把文件放进数组，并判断是否有下级目录
                File delFile[] = f.listFiles();
                int i = f.listFiles().length;
                for (int j = 0; j < i; j++) {
                    if (delFile[j].isDirectory()) {
                        del(delFile[j].getAbsolutePath());// 递归调用del方法并取得子目录路径
                    }
                    delFile[j].delete();// 删除文件
                }
            }
        }
    }
}
```
```java
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.taskdefs.Zip;
import org.apache.tools.ant.types.FileSet;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import com.xuanwu.smap.comapi.SmapMtMessage;
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;
/**
 * @author Toby 通用工具类
 */
public class Utils {
    /**
     * @param args
     * @throws Exception
     * @throws IOException
     */
    public static void main(String[] args) throws IOException, Exception {
        // TODO Auto-generated method stub
        // File file = new File("D:/user/mms.xml");
        // System.out.println(file.renameTo(new File("D:/user/mms5.xml")));
        // 1
        // compress("D:/user/test", "D:/user/test.zip");
        /*
         * String fileName = "D:/user/88.zip"; try {
         * System.out.println(encryptBASE64(readFileToBytes(fileName))); } catch
         * (Exception e) { // TODO Auto-generated catch block
         * e.printStackTrace(); }
         */
        /*
         * String mi
         * ="UEsDBBQACAA";
         * RandomAccessFile inOut = new RandomAccessFile(
         * "D:/user/sample.","rw"); inOut.write(decryptBASE64(mi));
         * inOut.close();
         */
        // System.out.println(new String(decryptBASE64("5rWL6K+V"),"utf-8"));
        // 2
        // String xml =
        // createXML("1828","qww","123456","10658103619033","15918542546",encryptBASE64("两款茶饮润肺护肤防过敏".getBytes()),encryptBASE64(readFileToBytes("D:/user/test.zip")));
        // System.out.println(xml);
        /*
         * String xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"
         * standalone=\"yes\"?><TaskDataTransfer4ERsp
         * xmlns=\"http://www.aspirehld.com/iecp/TaskDataTransfer4ERsp\"><ResultCode>2000</ResultCode><TaskId></TaskId><ResultMSG>没有获得IP鉴权!</ResultMSG></TaskDataTransfer4ERsp>";
         * 
         * Document doc = DocumentHelper.parseText(xml); // 将字符串转为XML Element
         * rootElt = doc.getRootElement(); // 获取根节点
         * 
         * String resultCode = rootElt.element("ResultCode").getTextTrim();
         * String TaskId = rootElt.element("TaskId").getTextTrim(); String
         * ResultMSG = rootElt.element("ResultMSG").getTextTrim();
         * System.out.println(" "+resultCode+" "+TaskId+" "+ResultMSG);
         */
    }
    /**
     * BASE64解密
     * 
     * @param key
     * @return
     * @throws Exception
     */
    public static byte[] decryptBASE64(String key) throws Exception {
        return (new BASE64Decoder()).decodeBuffer(key);
    }
    /**
     * BASE64加密
     * 
     * @param key
     * @return
     * @throws Exception
     */
    public static String encryptBASE64(byte[] key) throws Exception {
        return (new BASE64Encoder()).encodeBuffer(key);
    }
    /**
     * 获取路径下所有文件名
     * 
     * @param path
     * @return
     */
    public static String[] getFile(String path) {
        File file = new File(path);
        String[] name = file.list();
        return name;
    }
    /**
     * 
     * @param sourceDirPath
     * @param targetDirPath
     * @throws IOException
     */
    public static void copyDir(String sourceDirPath, String targetDirPath) throws IOException {
        // 创建目标文件夹
        (new File(targetDirPath)).mkdirs();
        // 获取源文件夹当前下的文件或目录
        File[] file = (new File(sourceDirPath)).listFiles();
        for (int i = 0; i < file.length; i++) {
            if (file[i].isFile()) {
                // 复制文件
                String type = file[i].getName().substring(file[i].getName().lastIndexOf(".") + 1);
                if (type.equalsIgnoreCase("txt"))
                    FileUtil.copyFile(file[i], new File(targetDirPath + file[i].getName()), MTOServerConstants.CODE_UTF_8,
                            MTOServerConstants.CODE_GBK);
                else
                    FileUtil.copyFile(file[i], new File(targetDirPath + file[i].getName()));
            }
            if (file[i].isDirectory()) {
                // 复制目录
                String sourceDir = sourceDirPath + File.separator + file[i].getName();
                String targetDir = targetDirPath + File.separator + file[i].getName();
                FileUtil.copyDirectiory(sourceDir, targetDir);
            }
        }
    }
    /**
     * 读取文件中内容
     * 
     * @param path
     * @return
     * @throws IOException
     */
    public static String readFileToString(String path) throws IOException {
        String resultStr = null;
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(path);
            byte[] inBuf = new byte[2000];
            int len = inBuf.length;
            int off = 0;
            int ret = 0;
            while ((ret = fis.read(inBuf, off, len)) > 0) {
                off += ret;
                len -= ret;
            }
            resultStr = new String(new String(inBuf, 0, off, MTOServerConstants.CODE_GBK).getBytes());
        } finally {
            if (fis != null)
                fis.close();
        }
        return resultStr;
    }
    /**
     * 文件转成字节数组
     * 
     * @param path
     * @return
     * @throws IOException
     */
    public static byte[] readFileToBytes(String path) throws IOException {
        byte[] b = null;
        InputStream is = null;
        File f = new File(path);
        try {
            is = new FileInputStream(f);
            b = new byte[(int) f.length()];
            is.read(b);
        } finally {
            if (is != null)
                is.close();
        }
        return b;
    }
    /**
     * 将byte写入文件中
     * 
     * @param fileByte
     * @param filePath
     * @throws IOException
     */
    public static void byteToFile(byte[] fileByte, String filePath) throws IOException {
        OutputStream os = null;
        try {
            os = new FileOutputStream(new File(filePath));
            os.write(fileByte);
            os.flush();
        } finally {
            if (os != null)
                os.close();
        }
    }
    /**
     * 将目录文件打包成zip
     * 
     * @param srcPathName
     * @param zipFilePath
     * @return 成功打包true 失败false
     */
    public static boolean compress(String srcPathName, String zipFilePath) {
        if (strIsNull(srcPathName) || strIsNull(zipFilePath))
            return false;
        File zipFile = new File(zipFilePath);
        File srcdir = new File(srcPathName);
        if (!srcdir.exists())
            return false;
        Project prj = new Project();
        Zip zip = new Zip();
        zip.setProject(prj);
        zip.setDestFile(zipFile);
        FileSet fileSet = new FileSet();
        fileSet.setProject(prj);
        fileSet.setDir(srcdir);
        zip.addFileset(fileSet);
        zip.execute();
        return zipFile.exists();
    }
    /**
     * 判空字串
     * 
     * @param str
     * @return 为空true
     */
    public static boolean strIsNull(String str) {
        return str == null || str.equals("");
    }
    /**
     * 折分数组
     * 
     * @param ary
     * @param subSize
     * @return
     */
    public static List<List<Object>> splitAry(Object[] ary, int subSize) {
        int count = ary.length % subSize == 0 ? ary.length / subSize : ary.length / subSize + 1;
        List<List<Object>> subAryList = new ArrayList<List<Object>>();
        for (int i = 0; i < count; i++) {
            int index = i * subSize;
            List<Object> list = new ArrayList<Object>();
            int j = 0;
            while (j < subSize && index < ary.length) {
                list.add(ary[index++]);
                j++;
            }
            subAryList.add(list);
        }
        return subAryList;
    }
    /**
     * @param mobile
     * @return
     */
    public static String ArrayToString(Object[] mobile) {
        String destId = "";
        for (Object phone : mobile) {
            destId += " " + (String) phone;
        }
        return destId.trim();
    }
}
```
