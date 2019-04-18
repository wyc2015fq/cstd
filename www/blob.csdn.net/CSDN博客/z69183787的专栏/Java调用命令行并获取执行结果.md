# Java调用命令行并获取执行结果 - z69183787的专栏 - CSDN博客
2018年06月07日 10:21:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：671
[https://blog.csdn.net/prstaxy/article/details/30050175](https://blog.csdn.net/prstaxy/article/details/30050175)
```java
import java.io.BufferedReader;  
import java.io.InputStreamReader;  
  
public class Command {  
    public static void exeCmd(String commandStr) {  
        BufferedReader br = null;  
        try {  
            Process p = Runtime.getRuntime().exec(commandStr);  
            br = new BufferedReader(new InputStreamReader(p.getInputStream()));  
            String line = null;  
            StringBuilder sb = new StringBuilder();  
            while ((line = br.readLine()) != null) {  
                sb.append(line + "\n");  
            }  
            System.out.println(sb.toString());  
        } catch (Exception e) {  
            e.printStackTrace();  
        }   
        finally  
        {  
            if (br != null)  
            {  
                try {  
                    br.close();  
                } catch (Exception e) {  
                    e.printStackTrace();  
                }  
            }  
        }  
    }  
  
    public static void main(String[] args) {  
        String commandStr = "ping www.taobao.com";  
        //String commandStr = "ipconfig";  
        Command.exeCmd(commandStr);  
    }  
}
```
