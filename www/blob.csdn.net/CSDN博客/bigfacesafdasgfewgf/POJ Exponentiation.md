# POJ: Exponentiation - bigfacesafdasgfewgf - CSDN博客





2015年10月02日 16:57:48[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1361








Description


Problems involving the computation of exact values of very large magnitude and precision are common. For example, the computation of the national debt is a taxing experience for many computer systems. 


This problem requires that you write a program to compute the exact value of Rn where R is a real number ( 0.0 < R < 99.999 ) and n is an integer such that 0 < n <= 25.

Input


The input will consist of a set of pairs of values for R and n. The R value will occupy columns 1 through 6, and the n value will be in columns 8 and 9.

Output


The output will consist of one line for each line of input giving the exact value of R^n. Leading zeros should be suppressed in the output. Insignificant trailing zeros must not be printed. Don't print the decimal point if the result is an integer.

Sample Input
95.123 12
0.4321 20
5.1234 15
6.7592  9
98.999 10
1.0100 12

Sample Output
548815620517731830194541.899025343415715973535967221869852721
.00000005148554641076956121994511276767154838481760200726351203835429763013462401
43992025569.928573701266488041146654993318703707511666295476720493953024
29448126.764121021618164430206909037173276672
90429072743629540498.107596019456651774561044010001
1.126825030131969720661201
Hint

If you don't know how to determine wheather encounted the end of input: 

*s* is a string and *n* is an integer 




代码：



```cpp

```
/*1001: Exponentiation
先不考虑小数，问题就转化为整数的幂，可以使用大数相乘来解决。
然后再对结果加入小数点。
*/

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

string BigNumMultipy( string str1, string str2 );

int main()
{
	string input;
	while( getline( cin, input ) )
	{
		if( input.length() != 9 )
		{
			cout << "" << endl;
			continue;
		}
		string zhenshu = "";
		int n = 0;
		int dotCount = 0;
		int index = input.find( "." );
		if( index == string::npos )
		{
			dotCount = 0;
			zhenshu = input.substr( 0, 6 );
			n = atoi( input.substr( 7, 2 ).c_str() );
		}
		else
		{
			dotCount = 5 - index;
			//if( atoi( input.substr( index + 1, dotCount ).c_str() ) != 0 )
				zhenshu = input.substr( 0, index ) + input.substr( index + 1, dotCount );
			//else
			//	zhenshu = input.substr( 0, index );
			n = atoi( input.substr( 7, 2 ).c_str() );
		}
		//cout << zhenshu << ' ' << n << ' ' << dotCount << endl;
		//循环大数相乘
		string res = zhenshu;
		if( atoi( zhenshu.c_str() ) == 0 )
		{
			cout << "0" << endl;
			continue;
		}
		int i, j, k;  //poj不允许在for在初始化
		for( i = 1; i < n; i++ )
		{
			res = BigNumMultipy( res, zhenshu );
		}
		//cout << res << endl;
		int totalDotCount = dotCount * n;
		if( totalDotCount != 0)  //只有小数才输出'.'
			res = res.substr( 0, res.length() - totalDotCount ) + "." + res.substr( res.length() - totalDotCount, totalDotCount );

		for( j = 0; j < res.length() && res[j] == '0'; j++ );  //前导0不能输出
		for( k = res.length() - 1; k >= 0 && res[k] == '0' && dotCount != 0; k-- );  //小数点后的无效0也不输出
		if( res[k] == '.' )
			k--;

		res = res.substr( j, k + 1 - j );
		cout << res << endl;
	}
	return 0;
}


//数组实现大数相乘
string BigNumMultipy( string str1, string str2 )
{
	int len1 = str1.length();
	int len2 = str2.length();
	vector< int > res( len1 + len2, 0 );
	reverse( str1.begin(), str1.end() );
	reverse( str2.begin(), str2.end() );
	int i, j, k;
	for( i = 0; i < len1; i++ )
	{
		for( j = 0; j < len2; j++ )
		{
			res[i+j] += ( str1[i] - '0' ) * ( str2[j] - '0' );
		}
	}
	for( k = 0; k < len1 + len2; k++ )
	{
		if( res[k] >= 10 )
		{
			res[k+1] += res[k] / 10;  //这两句顺序一定要正确，先算进位，然后才是本位。
			res[k] %= 10;
		}
	}
	string resStr = "";
	for( k = 0; k < len1 + len2; k++ )
	{
		char c = '0' + res[k];
		resStr += c;
	}

	reverse( resStr.begin(), resStr.end() );
	return resStr;
}


```cpp
1) 先把小数转化为整数，问题就转化为大数的幂，可以用循环调用大数相乘解决，然后再对结果做小数处理；
```

```cpp
2）大数相乘的模块要熟练；
```

```cpp
3）需要考虑一些特殊情况，例如：输入整数则结果不能有小数点、前导零不能输出、小数点后的无效零也不能输出；
```







