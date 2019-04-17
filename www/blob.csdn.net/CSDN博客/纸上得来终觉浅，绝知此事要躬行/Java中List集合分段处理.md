# Java中List集合分段处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月29日 09:58:20[boonya](https://me.csdn.net/boonya)阅读数：10510








**核心处理：使用List.subList(int start,int end);进行段位截取。**



```java
import java.util.ArrayList;
import java.util.List;

public class Test {
	
	public static void main(String[] args) {
		List<Integer> list=new ArrayList<Integer>();
		
		for (int i = 0; i < 99; i++) {
			list.add(i);
		}
		
		int perCount=10,index=0;
		
		int times=list.size()/perCount;
		
		do  {
			List<Integer> listTemp=null;
			if(list.size()>=perCount){
				listTemp=list.subList(0, perCount);// listTemp是分段处理逻辑的参数
			}else{
				listTemp=list.subList(0, list.size());// listTemp是分段处理逻辑的参数

			}
			//遍历当前的值是否正确
			String result="";
			for (int i = 0; i < listTemp.size(); i++) {
				result+=listTemp.get(i)+",";
			}
			System.out.println("第"+index+"轮:>>"+result);
		
			list.removeAll(listTemp);
			
			System.out.println("当前剩余集合长度:>>"+list.size());
			
			index++;
		}
		while (index<=times);
	}

}
```


上面是10个一组分段，控制台输出：



```
第0轮:>>0,1,2,3,4,5,6,7,8,9,
当前剩余集合长度:>>89
第1轮:>>10,11,12,13,14,15,16,17,18,19,
当前剩余集合长度:>>79
第2轮:>>20,21,22,23,24,25,26,27,28,29,
当前剩余集合长度:>>69
第3轮:>>30,31,32,33,34,35,36,37,38,39,
当前剩余集合长度:>>59
第4轮:>>40,41,42,43,44,45,46,47,48,49,
当前剩余集合长度:>>49
第5轮:>>50,51,52,53,54,55,56,57,58,59,
当前剩余集合长度:>>39
第6轮:>>60,61,62,63,64,65,66,67,68,69,
当前剩余集合长度:>>29
第7轮:>>70,71,72,73,74,75,76,77,78,79,
当前剩余集合长度:>>19
第8轮:>>80,81,82,83,84,85,86,87,88,89,
当前剩余集合长度:>>9
第9轮:>>90,91,92,93,94,95,96,97,98,
当前剩余集合长度:>>0
```





