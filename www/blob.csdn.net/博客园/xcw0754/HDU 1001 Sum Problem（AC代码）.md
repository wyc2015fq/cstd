# HDU 1001  Sum Problem（AC代码） - xcw0754 - 博客园
# [HDU 1001  Sum Problem（AC代码）](https://www.cnblogs.com/xcw0754/p/4051892.html)
```
1 #include <stdio.h>
 2 int main(){
 3         int k,sum;
 4         while(scanf("%d",&k)!=EOF){
 5             if(k%2==0){
 6                 sum=(1+k)*(k/2);
 7             }
 8             else{
 9                 sum=(1+k)*(k/2)+k/2+1;
10             }
11             printf("%d\n\n",sum);
12         }
13     return 0;
14 }
```

