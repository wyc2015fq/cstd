# SPI(Service Provider Interface)机制与demo - weixin_33985507的博客 - CSDN博客
2016年09月01日 15:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
上一篇文章中看到commons-logging，LogFactory 在查找具体实现类的时候 有一种是从
`META-INF/services/org.apache.commons.logging.LogFactory` 的第一行读取具体实现类。其实这个是java提供的一种扩展机制<b>SPI</b>，比如日志框架 会有很多其他实现，还有最著名用到<b>SPI</b>机制的就是<b>java.sql.Driver的SPI</b>，mysql，oracle 等驱动包都是不同的，<b>java可以结合不同的数据库驱动操作数据库</b>。这样的扩展机制感觉就是一种开源精神和扩展机制，还有spring中好多接口都是提供了一个默认的（default）的实现 在加载不到其他的实现的时候才走default实现，这就是留下的扩展，无论对于项目本身开发人员还是第三方的都是一大福音。
#### oracle的doc地址：[SPI-intro](https://link.jianshu.com?t=http://docs.oracle.com/javase/tutorial/sound/SPI-intro.html)
官方提供的是例子是一个<em>Java Sound API</em>，提供了一些寡欲读写混淆处理转换的接口 然后实现<em>Java Sound API</em>的提供基本的服务，但是这无法满足一些第三方开发者自己的实现，为了解决这个问题让这些第三方的实现能够像插件一样的嵌入到系统中提供服务就出现了<b>SPI</b>。
　　除了阅读官方的demo自己实现一个简单的demo
![2847417-2d7300b43a016253.png](https://upload-images.jianshu.io/upload_images/2847417-2d7300b43a016253.png)
spitestproject.png
- SPIService接口
　　定义规范spiTest方法
```
package spi.test;
/**
 * @author holly.wang
 */
public interface SPIService {
    public void spiTest();
}
```
- SPIServiceImpl class
　　SPIService的一个实现
```
package spi.test;
/**
 * @author holly.wang
 */
public class SPIServiceImpl implements SPIService {
    @Override
    public void spiTest() {
        System.out.println("holly.wang spi test");
        System.out.println("简书博客首页：http://www.jianshu.com/users/0d5a83b32e8f/latest_articles");
    }
}
```
- SpiTest类
　　测试方法
```
package spi.test;
import java.util.Iterator;
import sun.misc.Service;
/**
 * @author holly.wang
 */
public class SpiTest {
    /**
     * @param args
     * @throws IllegalAccessException
     * @throws InstantiationException
     * @throws ClassNotFoundException
     */
    public static void main(String[] args) throws InstantiationException, IllegalAccessException,
                                          ClassNotFoundException {
        Iterator it = Service.providers(SPIService.class);
        while (it.hasNext()) {
            SPIService service = (SPIService) it.next();
            service.spiTest();
        }
    }
}
```
下面我们看path中的/META-INF/services/spi.SPIService文件
```
spi.test.SPIServiceImpl
```
第一行就是指定要用的是哪个实现SPIService的类  只要有其他实现 就可以指定 这里我们指定为自己的实现`spi.test.SPIServiceImpl` 运行SpiTest的main方法
![2847417-454202e1a91291eb.png](https://upload-images.jianshu.io/upload_images/2847417-454202e1a91291eb.png)
SpiTest-runbeafore.png
跑完后：
![2847417-6129950fb12a3b70.png](https://upload-images.jianshu.io/upload_images/2847417-6129950fb12a3b70.png)
SpiTest-runafter.png
看到已经正确的输出。测试完成。
#### 关于SPI的规范和执行流程
- 在META-INF/services/目录中创建以接口全限定名命名的文件该文件内容为Api具体实现类的全限定名
- 使用ServiceLoader类动态加载META-INF中的实现类
- 如SPI的实现类为Jar则需要放在主程序classPath中
- Api具体实现类必须有一个不带参数的构造方法
这个demo很简单，感觉完全没必要这样麻烦的使用，但是SPI的机制是支持第三方的实现，你自己定义一组规范，任何人都可以有自己的实现，假如别人想用到别人的实现 不想用你默认的实现 那他的作用就来了 像插入一个插件一样easy。
