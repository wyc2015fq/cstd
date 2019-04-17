# ALiBABa fastjson去除JSON对象不需要的属性字段 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年05月27日 16:02:26[boonya](https://me.csdn.net/boonya)阅读数：29689








﻿﻿

引入包：



```java
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
```



TEST测试用例：



```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class JsonTest {

        @Test
	public void testJson()
	{
	/*	JSONObject json=new JSONObject();
		json.put("dd", "ss");
		json.put("mm", "ss");
		System.out.println("====1"+json.toJSONString()); 
		json.remove("mm");
		System.out.println("====1"+json.toJSONString()); */
		
		DataObject obj=new DataObject();
		System.out.println(obj.toString());
		JSONObject jsonObject=JSON.parseObject(obj.toString());
		System.out.println("====all attributes:"+jsonObject.toJSONString()); 
		jsonObject.remove("num1");
		System.out.println("====remove num1"+jsonObject.toJSONString()); 
		jsonObject.remove("num3");
		System.out.println("====remove num3"+jsonObject.toJSONString()); 
	}
	
	public class DataObject implements Serializable
	{
		public int num1=0;
		public int num2=0;
		public double num3=0.0;
		public double num4=0.0;
		public int getNum1() {
			return num1;
		}
		public void setNum1(int num1) {
			this.num1 = num1;
		}
		public int getNum2() {
			return num2;
		}
		public void setNum2(int num2) {
			this.num2 = num2;
		}
		public double getNum3() {
			return num3;
		}
		public void setNum3(double num3) {
			this.num3 = num3;
		}
		public double getNum4() {
			return num4;
		}
		public void setNum4(double num4) {
			this.num4 = num4;
		}
		@Override
		public String toString() {
			String s="{";
			s+="\"num1"+"\":"+"\""+num1+"\",";
			s+="\"num2"+"\":"+"\""+num2+"\",";
			s+="\"num3"+"\":"+"\""+num3+"\",";
			s+="\"num4"+"\":"+"\""+num4+"\"";
			s+="}";
			return s;
		}
		
		
	}
}
```

就是一个JSONObject 的remove方法就搞定了。以下是测试结果：



```
{"num1":"0","num2":"0","num3":"0.0","num4":"0.0"}
====all attributes:{"num1":"0","num2":"0","num3":"0.0","num4":"0.0"}
====remove num1{"num2":"0","num3":"0.0","num4":"0.0"}
====remove num3{"num2":"0","num4":"0.0"}
```









