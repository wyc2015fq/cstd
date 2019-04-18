# 使用spring RestTemplate进行rest接口测试 - z69183787的专栏 - CSDN博客
2017年07月03日 16:07:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1502
# 使用spring RestTemplate进行rest接口测试
# Rest
代表性状态传输（Representational State Transfer，REST）。
由于约定大于配置的讨论，主流 Web 2.0 服务提供者（包括 Yahoo、Google 和 Facebook）采用了更易于使用、面向资源的模型来公开其服务，而放弃了SOAP或者WSDL。
# RestTemplate
在我们的项目中，提供了REST服务。如下为使用[spring](http://lib.csdn.net/base/javaee)[测试](http://lib.csdn.net/base/softwaretest)时的接口测试代码程序。
## 开放的接口和结果输出
接口地址：
```
http://10.xx.xx.xx:8100/xxx/monthlyrackfees.json?period=201404
```
返回结果：
```
{"data":[{"labNum":"53","period":201404,"feeAmount":5000},              {"labNum":"53","period":201404,"feeAmount":1000}],
"status":200,"msg":"OK"}
```
## 建立JSON返回值结构
```
//{"labNum":"53","period":201404,"feeAmount":5000} 
//这个结构在SupplierRackFee中，不贴出来了
import idc.tco.SupplierRackFee;
import java.util.List;
import com.google.common.collect.Lists;
public class JsonResultRackFee {
    protected int status;
    protected String msg = "OK";    
    private List<SupplierRackFee> data = Lists.newArrayList();
    public int getStatus() {
        return status;
    }
    public void setStatus(int status) {
        this.status = status;
    }
    public String getMsg() {
        return msg;
    }
    public void setMsg(String msg) {
        this.msg = msg;
    }
    public List<SupplierRackFee> getData() {
        return data;
    }
    public void setData(List<SupplierRackFee> data) {
        this.data = data;
    }
}
```
需要注意，必须要有set方法，不然会提示报错。
## 测试函数主体
调用json的url获得返回值，非常简单。 
1，使用 new RestTemplate()，获得一个restTemplate。 
2，然后调用getForObject(String url, Class responseType）.
```
public class RackFeesTest {
    static String url="http://10.xx.xx.xx:8100/xxx/monthlyrackfees.json?period=%d";
    static int  period=201404;
    static RestTemplate restTemplate = null;
    @BeforeClass
    public static void setup(){
        url = String.format(url, period);
        System.out.println(url);
        restTemplate = new RestTemplate();
    }
    @Test
    public void testRackFees() {
        JsonResultRackFee result = restTemplate.getForObject(url, JsonResultRackFee.class);
        Assert.assertTrue(result.getData().size()>0);
        for(int i=0;i<result.getData().size();i++) {
            System.out.println(result.getData().get(i).getSupplierName());
            System.out.println(result.getData().get(i).getFeeAmount());
        }
    }
}
```
## 测试引用的jar包
```
<!-- 用于 RestTemplate -->
<dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-web</artifactId>
        <version>${org.springframework.version}</version>
</dependency>
<!-- 用于 Lists-->
 <dependency>
    <groupId>com.google.guava</groupId>
    <artifactId>guava</artifactId>
    <version>11.0.1</version>
</dependency>
```
