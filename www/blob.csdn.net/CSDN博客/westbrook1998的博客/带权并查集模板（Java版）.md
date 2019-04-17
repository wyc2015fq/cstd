# 带权并查集模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月05日 15:37:22[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：51








带权并查集就是向量的关系处理，大概理解了这句话了

然后吐槽一下这个java的读入效率，c++250ms的题特么能超时，加了个快读才勉强卡过

```java
import java.io.*;
import java.util.Scanner;
import java.util.StringTokenizer;

public class Main {
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

    static final int N = 50050;
    static int n, k, q, a, b;
    static int[] p = new int[N], num = new int[N];

    static void init() {
        for (int i = 1; i <= n; i++) {
            p[i] = i;
        }
    }

    static int find(int x) {
        if (x == p[x]) {
            return x;
        }
        int fa = p[x];
        p[x] = find(p[x]);
        num[x] = (num[x] + num[fa]) % 3;
        return p[x];
    }

    static boolean join(int a, int b, int q) {
        int fa = find(a);
        int fb = find(b);
        if (fa == fb) {
            if (num[b] != (num[a] + (q - 1)) % 3) {
                return false;
            } else {
                return true;
            }
        } else {
            p[fb] = fa;
            num[fb] = (num[a] - num[b] + (q - 1) + 3) % 3;
            return true;
        }
    }

    public static void main(String[] args) {
        //Scanner cin = new Scanner(System.in);
        InputReader cin=new InputReader(new BufferedInputStream(System.in));
        n = cin.nextInt();
        k = cin.nextInt();
        init();
        int cnt = 0;
        while (k-- > 0) {
            q = cin.nextInt();
            a = cin.nextInt();
            b = cin.nextInt();
            if (a > n || b > n || (q == 2 && a == b)) {
                cnt++;
            } else {
                if (!join(a, b, q)) {
                    cnt++;
                }
            }
        }
        System.out.println(cnt);
    }
}
```



