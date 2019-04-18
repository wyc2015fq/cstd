# Jackson 枚举序列化/反序列化 - z69183787的专栏 - CSDN博客
2017年01月09日 20:52:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9315
枚举类：
```java
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonValue;
/**
 * Created by Administrator on 2017/1/9.
 */
public enum TestEnum {
    A(1,"1"),
    B(2,"2");
    private int code;
    private String description;
    private TestEnum(int code, String description){
        this.code = code;
        this.description = description;
    }
    @JsonCreator
    public static TestEnum getItem(int code){
        for(TestEnum item : values()){
            if(item.getCode() == code){
                return item;
            }
        }
        return null;
    }
    @JsonValue
    public int getCode() {
        return code;
    }
    public void setCode(int code) {
        this.code = code;
    }
    public String getDescription() {
        return description;
    }
    public void setDescription(String description) {
        this.description = description;
    }
}
```
实例类：
```java
public static class TestEnums{
        int code =1;
        TestEnum testEnum = TestEnum.B;
        public int getCode() {
            return code;
        }
        public void setCode(int code) {
            this.code = code;
        }
        public TestEnum getTestEnum() {
            return testEnum;
        }
        public void setTestEnum(TestEnum testEnum) {
            this.testEnum = testEnum;
        }
    }
```
方案：
利用 @JsonCreator 与  @JsonValue 注解：
JsonCreator ：反序列化时的 初始化函数，入参为 对应该枚举的 json值
JsonVale：序列化时 枚举对应生成的值
测试类：
```java
@org.junit.Test
    public void enumJsonTest(){
        TestEnums testEnums = new TestEnums();
        String json = JsonUtil.toJson(testEnums);
        System.out.println(json);
        TestEnums target = JsonUtil.toObject(json,TestEnums.class);
        System.out.println("");
    }
```
输出：
{"code":1,"testEnum":2}
