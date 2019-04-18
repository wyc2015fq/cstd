# Mockito使用 - z69183787的专栏 - CSDN博客
2016年10月14日 18:25:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1049
```java
import static org.mockito.Mockito.*;
/**
 * Created by Administrator on 2015/12/30.
 */
public class MockTest {
    protected static final Logger logger = Logger.getLogger(MockTest.class);
    //@Test
    public void easyMockTest() {
        List list = EasyMock.createMock(List.class);
        // 录制过程
        // 期望方法list.set(0,1)执行2次，返回null，不抛出异常
        expect1:
        EasyMock.expect(list.set(0, 1)).andReturn(null).times(2);
        // 期望方法list.set(0,1)执行1次，返回null，不抛出异常
        expect2:
        EasyMock.expect(list.set(0, 1)).andReturn(1);
        // 执行测试代码
        EasyMock.replay(list);
        // 执行list.set(0,1)，匹配expect1期望，会返回null
        Assert.assertNull(list.set(0, 1));
        // 执行list.set(0,1)，匹配expect1（因为expect1期望执行此方法2次），会返回null
        Assert.assertNull(list.set(0, 1));
        // 执行list.set(0,1)，匹配expect2，会返回1
        Assert.assertEquals(1, list.set(0, 1));
        // 验证期望
        EasyMock.verify(list);
        Integer i1 = 100;
        Integer i2 = 100;
        if (i1 == i2)
            System.out.println("i1 and i2 is equal");
        else
            System.out.println("i1 and i2 is not equal ");
        byte a = 127;
        byte b = 127;
        byte c = 0;
        c = (byte) (b + a);
        System.out.println(c); // error : cannot convert from int to byte
        b += a; // ok
        System.out.println(b);
        System.out.println(3 * 0.1);
    }
    @Test
    public void mockitoTest() {
        //也可以Mock具体的类，而不仅仅是接口
        LinkedList mockedList = mock(LinkedList.class);
        //Stub
        when(mockedList.get(0)).thenReturn("first"); // 设置返回值
        when(mockedList.get(1)).thenThrow(new RuntimeException()); // 抛出异常
        //第一个会打印 "first"
        System.out.println(mockedList.get(0));
        //接下来会抛出runtime异常
        System.out.println(mockedList.get(1));
        //接下来会打印"null",这是因为没有stub get(999)
        System.out.println(mockedList.get(999));
        // 可以选择性地验证行为，比如只关心是否调用过get(0)，而不关心是否调用过get(1)
        verify(mockedList).get(0);
    }
    @Rule
    public ExpectedException thrown = ExpectedException.none();
    @Test
    public void exceptionTest() {
        thrown.expect(RuntimeException.class);
        thrown.expectMessage("Runtime exception occurred");
        throw new RuntimeException("Runtime exception occurred");
    }
```
