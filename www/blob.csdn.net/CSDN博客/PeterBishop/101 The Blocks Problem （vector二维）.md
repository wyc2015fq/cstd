# 101 The Blocks Problem （vector二维） - PeterBishop - CSDN博客





2018年07月11日 10:59:46[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：18








```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
using namespace std;
int n;
vector<int>pile[30];//重点在于记录实时位置，vector这种可以动态申请的数组正好可以当作二维数组用。第一维堆第二维层

void find_block(int a, int& p, int& h) {  //引用是传递地址...当在函数里修改其值,变量的值也会随着改变
for (p = 0; p<n; p++)
for (h = 0; h<pile[p].size(); h++)
if (pile[p][h] == a) return;//void中的return:退出函数体，如果在函数体结尾处不加也可自动退出，
}

void clear_above(int p, int h) {
for (int i = h + 1; i<pile[p].size(); i++) {
int t = pile[p][i];
pile[t].push_back(t);   //把标号为t的木块放回原位，即t堆

	}
 pile[p].resize(h + 1);    //重新定义p堆的长度

}
void pile_onto(int p, int h, int p2) {
for (int i = h; i<pile[p].size(); i++) pile[p2].push_back(pile[p][i]);
	     pile[p].resize(h);
	
}
 //输出木块最后的状态
 void print() {
	     for (int i = 0; i<n; i++) {
		         printf("%d:", i);
		         for (int j = 0; j<pile[i].size(); j++) {
			             printf(" %d", pile[i][j]);
			
		}
		         printf("\n");
		
	}
	
}
 int main() {
	     int a, b;
	     string s1, s2;
	     scanf("%d", &n);
	     for (int i = 0; i<n; i++) pile[i].push_back(i);
	     while (cin >> s1) {//通过对题意的归纳可以通过函数重用减少多余代码
		         if (s1 == "quit") break;
		         cin >> a >> s2 >> b;
		         int pa, pb, ha, hb;
		         find_block(a, pa, ha);
		         find_block(b, pb, hb);
		         if (pa == pb) continue;
		         if (s2 == "onto") clear_above(pb, hb);
		         if (s1 == "move") clear_above(pa, ha);
		         pile_onto(pa, ha, pb);
		
	}
	     print();
	     return 0;
	
}
```




