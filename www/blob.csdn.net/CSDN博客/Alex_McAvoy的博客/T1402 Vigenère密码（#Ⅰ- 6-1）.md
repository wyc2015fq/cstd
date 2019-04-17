# T1402	Vigenère密码（#Ⅰ- 6 - 1） - Alex_McAvoy的博客 - CSDN博客





2018年03月08日 21:00:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：1687








### 【题目描述】

6世纪法国外交家Blaise de Vigenère设计了一种多表密码加密算法——Vigenère密码。Vigenère密码的加密解密算法简单易用，且破译难度比较高，曾在美国南北战争中为南军所广泛使用。

   在密码学中，我们称需要加密的信息为明文，用M表示；称加密后的信息为密文，用C表示；而密钥是一种参数，是将明文转换为密文或将密文转换为明文的算法中输入的数据，记为k。 在Vigenère密码中，密钥k是一个字母串，k=k1k2…kn。当明文M=m1m2…mn时，得到的密文C=c1c2…cn，其中ci=mi®ki，运算®的规则如下表所示：

![](http://ybt.ssoier.cn:8088/pic/1402a.gif)

Vigenère加密在操作时需要注意：

1.®运算忽略参与运算的字母的大小写，并保持字母在明文M中的大小写形式；

2.当明文M的长度大于密钥k的长度时，将密钥k重复使用。

例如，明文M=Helloworld，密钥k=abc时，密文C=Hfnlpyosnd。

![](http://ybt.ssoier.cn:8088/pic/1402b.gif)


### 【输入】

第一行为一个字符串，表示密钥k，长度不超过100，其中仅包含大小写字母。第二行    为一个字符串，表示经加密后的密文，长度不超过1000，其中仅包含大小写字母。
    对于100%的数据，输入的密钥的长度不超过100，输入的密文的长度不超过1000，且都仅包含英文字母。


### 【输出】

输出共1行，一个字符串，表示输入密钥和密文所对应的明文。


### 【输入样例】

CompleteVictory
Yvqgpxaimmklongnzfwpvxmniytm

### 【输出样例】

Wherethereisawillthereisaway

### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
void turn(char &x);
int main()
{
	char key[1000],secret[1000];
	int len1,len2;
	int i,j;

	gets(key);//输入密钥
	gets(secret);//输入密文
	len1=strlen(key);//求密钥长度
	len2=strlen(secret);//求密文长度
	
	for(i=0;i<len1;i++)//将密钥转换为大写字母
		turn(key[i]);

	for(i=0,j=0;i<len2;i++)//从头开始解密
	{
		if(secret[i]<='Z')//大写的情况
		{
			secret[i]=secret[i]-(key[j]-'A');//按规则解密
			j++;
			if(secret[i]<'A')//若明文小于A
				secret[i]='Z'-('A'-secret[i])+1;//从字母表倒序转换
		}
		else//小写的情况
		{
			secret[i]=secret[i]-(key[j]-'A');//按规则解密
			j++;
			if(secret[i]<'a')//若明文小于a
				secret[i]='z'-('a'-secret[i])+1;//从字母表倒序转换
		}
		if(j>len1-1)//若明文长度大于密钥长度，重复使用密钥
			j=0;
	}
	cout<<secret<<endl;
    return 0;
}
void turn(char &x)
{
	if((x>='a')&&(x<='z'))
		x-=32;
}
```




