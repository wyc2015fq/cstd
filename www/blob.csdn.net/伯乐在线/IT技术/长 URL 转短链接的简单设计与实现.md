# 长 URL 转短链接的简单设计与实现 - 文章 - 伯乐在线
原文出处： [悠悠然然](https://my.oschina.net/tinyframework/blog/861014)
非常多的时候，我们不想在分享的时候采用长长的链接，主要的原因有：
- URL太长占显示空间、难于输入，转成二维码点点小，难于识别
- 长的URL提供的信息太多，不利于信息安全，甚至容易造成倒库
- 其他理由……
今天的理由不是重点，重点是如何用一点点代码实现这个短链接的生成。
我们的注意点有：
- 生成的短链接地址长度可调整，毕竟不能因为4位、5位、6位的长度变化就让我们重新写一套程序
- 要有防冲突处理机制，不能因为算法导致两个长链接地址生成的短地址一样就把前一个覆盖掉
- 有时候，一个地址用过之后觉得不需要使用了，哪么这个地址失效之后，短地址可以有效收回。
首先设计接口


```
/**
 * Created by luoguo on 2017/3/16.
 */
public interface UrlShortener {
    /**
     * 设置产生短链接长度
     * @param length
     * @return
     */
    void setLength(int length);
    /**
     * 返回短链接长度
     * @return
     */
    int getLength();
    /**
     * 返回指定地址对应的短链接
     * @param url
     * @return
     */
    String get(String url);
    /**
     * 存储对应关系
     * @param url
     * @param shortUrl
     */
    void put(String url, String shortUrl);
    /**
     * 到库里查看是不是存在映射，如果不存在返回null
     * @param shortUrl
     * @return
     */
    String seek(String shortUrl);
    /**
     * 据地址产生短地址
     * @param url
     * @return
     */
    String generate(String url);
    /**
     * 根据地址和种子产生一个短地址
     * @param url
     * @param seed
     * @return
     */
    String generate(String url, int seed);
    /**
     * 清除指定URL的短链接信息
     * @param url
     */
    void clean(String url);
    /**
     * 清除指定时间以前没有使用的所有短链接
     * @param date
     */
    void clean(Date date);
}
```
只是个示例，注释比较简陋，接下来是抽象类实现，把公共的内容放这里


```
import java.util.Random;
/**
 * Created by luoguo on 2017/3/16.
 */
public abstract class AbstractUrlShortener implements UrlShortener {
    public static char[] VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890".toCharArray();
    private static Random random = new Random(System.currentTimeMillis());
    protected int length = 4;
    public AbstractUrlShortener() {
    }
    public AbstractUrlShortener(int length) {
        this.length = length;
    }
    public void setLength(int length) {
        this.length = length;
    }
    public int getLength() {
        return length;
    }
    public String get(String url) {
        String sortUrl = seek(url);
        if (sortUrl == null) {
            sortUrl = generate(url);
            put(url, sortUrl);
        }
        return sortUrl;
    }
    public String generate(String url, int seed) {
        char[] sortUrl = new char[length];
        for (int i = 0; i < length; i++) {
            sortUrl[i] = VALID_CHARS[seed % VALID_CHARS.length];
            seed = random.nextInt(Integer.MAX_VALUE) % VALID_CHARS.length;
        }
        return new String(sortUrl);
    }
    public String generate(String url) {
        String shortUrl;
        shortUrl = generate(url, random.nextInt(Integer.MAX_VALUE));
        while (seek(shortUrl) != null) {
            shortUrl = generate(url, random.nextInt(Integer.MAX_VALUE));
        }
        put(url, shortUrl);
        return shortUrl;
    }
}
```
实际的需要在数据库层实现，这里在内存里面存储的实现一个意思一下


```
import java.util.Date;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
/**
 * 内存存储的实现
 * <p>
 * Created by luoguo on 2017/3/16.
 */
public class UrlShortenerMemory extends AbstractUrlShortener {
    private Map<String, String> url2ShortUrl = new ConcurrentHashMap<String, String>();
    private Map<String, String> shortUrl2Url = new ConcurrentHashMap<String, String>();
    public UrlShortenerMemory() {
        super();
    }
    public UrlShortenerMemory(int length) {
        super(length);
    }
    public void put(String url, String shortUrl) {
        url2ShortUrl.put(url, shortUrl);
        shortUrl2Url.put(shortUrl, url);
    }
    public String seek(String shortUrl) {
        return shortUrl2Url.get(shortUrl);
    }
    public void clean(String url) {
        String sortUrl = url2ShortUrl.get(url);
        if (sortUrl != null) {
            url2ShortUrl.remove(url);
            shortUrl2Url.remove(sortUrl);
        }
    }
    public void clean(Date date) {
        throw new UnsupportedOperationException();
    }
}
```
弄点测试代码，试试看效果怎么样


```
public class UrlShortenerTest {
    public static void main(String[] args) {
        for(int j=1;j<10;j++) {
            UrlShortener urlShortener = new UrlShortenerMemory(j);
            for (int i = 0; i < 5; i++) {
                System.out.println(urlShortener.get("http://www.tinygroup.org"));
            }
            System.out.println();
        }
    }
}
```
这里是运行结果


```
T
Q
l
i
I
0R
VB
fN
so
Nw
fw1
fpu
WT7
pNL
M5V
0cZF
H09A
AjYM
1AMf
sdkz
USsgK
ZMn9U
7S48u
oBb1L
yB14g
DRVGWg
gh3oJa
BapdY7
yJgcZj
mivGN3
TEiQG2M
8SkokOY
NZww5GT
fxloZ0R
hkiOpID
3ZpdIYrF
eFlSSDyd
bB4RCFgE
25mceTMK
AAJ2Lp6S
5IUVoFSqZ
zHZeuZljY
njCGDBKVs
IgH21Tg5V
WvGCy5ZtE
```
感觉还可以，当然里面还有一些不完善的地方
- 当产生的数量达到一定程度的时候，再获取时由于冲突比较大，导致性能会降低
- 因此如果是小型系统，建议采用4个长度，大型系统建议采用6个长度
- 现在还没有多长时间之后自动失效的API，建议添加
- 需要实现支持序列化的方案
- 现在没有同步处理，实际实现中需要考虑
这个只是化几分钟展示一个思路，并没有经过系统的思考的验证，正式系统请慎重使用。
