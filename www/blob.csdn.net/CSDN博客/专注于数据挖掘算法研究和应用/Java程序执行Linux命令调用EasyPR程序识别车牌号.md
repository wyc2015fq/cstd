# Java程序执行Linux命令调用EasyPR程序识别车牌号 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年09月26日 15:33:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2688








                
1)下载解压JDK并配置环境变量

     #vi /etc/profile     

     编辑：

     #Java Environment Path

     export JAVA_HOME=/home/jdk1.8.0_20

     export JRE_HOME=${JAVA_HOME}/jre

     export PATH=$PATH:${JAVA_HOME}/bin

     export CLASSPATH=$CLASSPATH.:${JAVA_HOME}/lib:${JRE_HOME}/lib

   2)程序：

     Java程序中要执行linux命令主要依赖2个类：Process和Runtime

     输出：EasyPR.jar



```java
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class EasyPR {
	public static void main(String[] args){  
        InputStream in = null;  
        try {   
            //String cmd1="cd /home/EasyPR-master";
            String cmd2="./demo_linux_amd64 recognize -p resources/image/plate_recognize.jpg --svm resources/model/svm.xml --ann resources/model/ann.xml";
            //Process pro = Runtime.getRuntime().exec(new String[]{cmd1,cmd2});
            Process pro = Runtime.getRuntime().exec(cmd2);
            pro.waitFor();  
            in = pro.getInputStream();  
            BufferedReader read = new BufferedReader(new InputStreamReader(in));  
            while(read.readLine()!=null){
            	String result = read.readLine(); 
            	System.out.println("INFO:"+result);
            }                    
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }  
}
```
3)执行：

   #/bin/sh

   #cd /home/EasyPR-master 

   #java -jar /tmp/EasyPR.jar

   也可以将执行命令制作sh脚本，再由Java调用sh脚本执行。






