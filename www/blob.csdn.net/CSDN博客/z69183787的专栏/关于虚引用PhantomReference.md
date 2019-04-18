# 关于虚引用PhantomReference - z69183787的专栏 - CSDN博客
2018年02月09日 10:45:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：162
[https://segmentfault.com/q/1010000004491370](https://segmentfault.com/q/1010000004491370)
在<<深入理解Java虚拟机>>3.2.3中有这么一句话
> 
“为一个对象设置虚引用关联的唯一目的就是能在这个对象被收集器回收时收到一个系统通知。”
请问利用虚引用在对象被回收时收到一个系统通知要怎么实现呢？能提供下示例代码吗？
```
import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.reflect.Field;
public class Test {
    public static boolean isRun = true;
    @SuppressWarnings("static-access")
    public static void main(String[] args) throws Exception {
        String abc = new String("abc");
        System.out.println(abc.getClass() + "@" + abc.hashCode());
        final ReferenceQueue<String> referenceQueue = new ReferenceQueue<String>();
        new Thread() {
            public void run() {
                while (isRun) {
                    Object obj = referenceQueue.poll();
                    if (obj != null) {
                        try {
                            Field rereferent = Reference.class
                                    .getDeclaredField("referent");
                            rereferent.setAccessible(true);
                            Object result = rereferent.get(obj);
                            System.out.println("gc will collect："
                                    + result.getClass() + "@"
                                    + result.hashCode() + "\t"
                                    + (String) result);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }.start();
        PhantomReference<String> abcWeakRef = new PhantomReference<String>(abc,
                referenceQueue);
        abc = null;
        Thread.currentThread().sleep(3000);
        System.gc();
        Thread.currentThread().sleep(3000);
        isRun = false;
    }
}
```
和ReferenceQueue搭配使用。
