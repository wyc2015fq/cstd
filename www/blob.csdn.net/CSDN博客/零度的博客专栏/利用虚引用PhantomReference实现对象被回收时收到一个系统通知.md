# 利用虚引用PhantomReference实现对象被回收时收到一个系统通知 - 零度的博客专栏 - CSDN博客
2018年03月02日 14:54:58[零度anngle](https://me.csdn.net/zmx729618)阅读数：626
虚引用PhantomReference， 在<<深入理解Java虚拟机>>一文中，它唯一的目的就是为一个对象设置虚引用关联的唯一目的就是能在这个对象被收集器回收时收到一个系统通知。那么如何实现呢？它与ReferenceQueue配合使用来实现，下面给出代码示例：
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
        PhantomReference<String> abcWeakRef = new PhantomReference<String>(abc, referenceQueue);
        abc = null;
        Thread.currentThread().sleep(3000);
        System.gc();
        Thread.currentThread().sleep(3000);
        isRun = false;
    }
}
```
运行如下：
class java.lang.String@96354
gc will collect：class java.lang.String@96354abc
