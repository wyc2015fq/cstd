# HDU 1002  A + B Problem II（AC代码） - xcw0754 - 博客园
# [HDU 1002  A + B Problem II（AC代码）](https://www.cnblogs.com/xcw0754/p/4051917.html)
```
1 #include <stdio.h>
 2 #include <string.h>
 3 #define MAX 1009
 4 int main()
 5 {
 6     char a[MAX]={0},b[MAX]={0};
 7     int n,i,len1,len2,j=0,z=0,r=0,k=0;
 8     char *p=NULL;
 9     scanf("%d",&n);    
10     if( 0<n&&n<21 ){
11         for(i=0;i<n;i++){
12             j=0;
13             z=0;
14             r=0;
15             scanf("%s",a);
16             scanf("%s",b);
17             printf("Case %d:\n",i+1);
18             printf("%s + %s = ",a,b);
19             len1=strlen(a);
20             len2=strlen(b);
21             if( len1>len2 ){
22                 b[len1]='\0';
23                 len1--;
24                 len2--;
25                 while( len2>=0 ){
26                     b[len1--]=b[len2--];
27                 }
28                 while(len1>=0){
29                     b[len1--]='0';
30                 }
31             }
32             else if( len2>len1 ){
33                 a[len2]='\0';
34                 len1--;
35                 len2--;
36                 while( len1>=0 ){
37                     a[len2--]=a[len1--];
38                 }
39                 while(len2>=0){
40                     a[len2--]='0';
41                 }
42             }
43              len1=strlen(a);
44               len2=strlen(b);
45             len1--;
46             len2--;
47             while( len1>=0 && len2>=0 )   {
48                 z= (b[len2]-'0') + (a[len1]-'0') + r;  
49                 r=z/10;
50                 z=z-r*10;
51                 b[len2]=z+'0';
52                 len1--;
53                 len2--;
54             };
55             if( r!=0 ){
56                 printf("1");
57                 printf("%s\n",b);    
58             }
59             else{
60                 len2=strlen(b);
61                 while( b[k]=='0'&&k<len2 ){
62                     k++;
63                 }
64                 p=&b[k];
65                 printf("%s\n",p);
66             }
67             if(i!=n-1)
68                 printf("\n");
69 
70         }
71     }
72     return 0;
73 }
```
格式很重要：
Case+空格+i+：（冒号后无空格）
a+空格+b+空格+=+空格+c（直接把+号去掉，再按空格键代替空格）
（空一行）
<<-继续上面格式->>
注意：最后一个case输完后是不需要空行的，所以会有代码
  if(i!=n-1)
      printf("\n");　　//当非最后一个case时，输出一个空行
解题思路：
1、数字可能太大，得用数组逐个记下来，第一个存在a，另一个存在b。
2、strlen算它们各自的长度(strlen不计算'\0')，从数组的第strlen个开始
3、相加之后要考虑是否需要进一位，需要的话把十位上数字提出来，提取出来和第strlen-1的那两个数相加。
4、刚被提取个位上的数字存到长度较长的数组中，节省点空间。
5、一直重复。
要注意：
输入可能是00001 00002，那么你要输出3，不能是00003。
输入可能是99999 99998，那么考虑进位时，因两个数组一样长，所以可能进位会填不进数组，要单独输出。
输入可能是99999 1，那么也是考虑进位的问题。 
输入可能是1 9999，也是进位问题，要考虑不同长度的数字的前后顺序。
