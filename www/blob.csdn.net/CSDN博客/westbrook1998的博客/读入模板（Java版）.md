# 读入模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月25日 22:19:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45








用处应该不大因为蓝桥杯应该用不到这个

主要写了一道floyd c++ 7ms java 1500ms …

用了这个模板读入之后是1000ms左右
```java
import java.io.*;
import java.util.StringTokenizer;

public class Main{

    static class InputReader{
        public BufferedReader reader;
        public StringTokenizer tokenizer;
        public InputReader(InputStream stream) {
            reader = new BufferedReader(new InputStreamReader(stream), 32768);
            tokenizer = null;
        }
        public String next() {
            while (tokenizer == null || !tokenizer.hasMoreTokens()) {
                try {
                    tokenizer = new StringTokenizer(reader.readLine());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return tokenizer.nextToken();
        }
        public int nextInt(){
            return Integer.parseInt(next());
        }
    }

    public static void main(String[] args) {
        InputReader cin=new InputReader(new BufferedInputStream(System.in));
    }
}
```




