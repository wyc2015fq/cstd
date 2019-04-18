# Java判断素数 - z69183787的专栏 - CSDN博客
2012年11月12日 09:37:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：606
素数：除了1以外，只能被1和自己整除的数；
```java
int i;
		long N=0;
		System.out.println("输入一个数字：");
		try
		{
			Scanner input=new Scanner(System.in);
			N=input.nextLong();
		}catch(Exception e){}
		
		for(i=2;i<N;i++)
		{
			if(N==2 || N%i==0)
				{System.out.println("这个数字不是素数!");break;}
		}
		if(i==N)
			System.out.println("这个数字是素数!");
```
