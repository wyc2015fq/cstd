# LeetCode Rotate Array 翻转数组 - xcw0754 - 博客园
# [LeetCode Rotate Array 翻转数组](https://www.cnblogs.com/xcw0754/p/4415771.html)
题意：给定一个数组，将该数组的后k位移动到前n-k位之前。（本题在编程珠玑中第二章有讲）
思路：
方法一：将后K位用vector容器装起来，再移动前n-k位到后面，再将容器内k位插到前面。
![](https://images0.cnblogs.com/blog2015/641737/201504/102135155559218.png)
```
1 class Solution {
 2 public:
 3     void rotate(int nums[], int n, int k) {
 4         if( !k || !n || n==1 || k==n ) return;
 5         k %= n;
 6         vector<int> cha;
 7         cha.reserve(k);
 8         int i;
 9         for(i=n-k; i<n; i++)//装进容器
10             cha.push_back(nums[i]);
11         for(i=n-k-1; i>=0; i--)//移动前n-k位
12             nums[i+k]=nums[i];
13         for(i=0; i<k; i++)//将容器内k个元素移到前面
14             nums[i]=cha[i];
15     }
16 };
Rotate Array
```
方法二：将后k位自身旋转，再将前n-k位自身旋转，在将整个数组旋转。例如：[1,2,3,4,5,6,7]，第一步[1,2,3,4,7,65]，第二步[4,3,2,1,7,6,5]，第三步`[5,6,7,1,2,3,4]。主要工作就是旋转，`
![](https://images0.cnblogs.com/blog2015/641737/201504/102138311962815.png)
```
1 class Solution {
 2 public:
 3     void rev(int *q,int *p) //将q与p所指向的区间旋转
 4     {
 5         while( q!=p && q!=p+1 )
 6         {
 7             *q ^= *p;
 8             *p ^= *q;
 9             *q ^= *p;
10             q++;
11             p--;
12         }
13     }
14     
15     void rotate(int nums[], int n, int k) {
16         if( !k || !n || n==1 || k==n ) return;
17             k %= n;//这一步很必要
18         rev(&nums[n-k],&nums[n-1]);//转后k个
19         rev(&nums[0],&nums[n-k-1]);//转前n-k个
20         rev(&nums[0],&nums[n-1]);//转整个
21     }
22 };
Rotate Array
```
方法三：为了省空间，当n%k==0时，就可以用这招了。将最后一位取出来，该位置-k的位置上的值就可以放在最后一位上了，其实就是按周期k，将各个元素一步到达其最终的位置上。例如`[1,2,3,4,5,6,7，8，9]，k=3，先将9用一个变量存起来，那么6就可以移到9的位置上，3就可以移到6的位置上，9可以移到3的位置上。对于8、5、2也是如此。这样就能完成节省空间的目的了。但是，当n%k不为0时，要注意了，按上面做法移动完之后，最前面的k位并不在其最终位置上，比如`[1,2,3,4,5,6,7]，k=3，结果却是`[7,5,6,1,2,3,4]，第一步是7、4、1没错吧？那7就到了1的位置了，第二步是6、3没错吧，那么只是交换了他们的位置。那么7在第1位，6在第3位，5在第2位，这可以想办法解决的。办法是，前k位中的前(n%k)个元素得接在后面k-(n%k)位的后面，又需要进行一次旋转，太麻烦了，如果这次k%(n%k)又不为0，那么还有得继续！我用的方法是对这前k位直接套用上面第2种方法。速度奇慢！```
![](https://images0.cnblogs.com/blog2015/641737/201504/102156498058884.png)
```
1 class Solution {
 2 public:
 3     void rev(int *q,int *p)
 4     {
 5         while( q!=p && q!=p+1 )
 6         {
 7             *q ^= *p;
 8             *p ^= *q;
 9             *q ^= *p;
10             q++;
11             p--;
12         }
13     }
14 
15     void rotate(int nums[], int n, int k) {
16         if( !k || !n || n==1 || k==n ) return;
17             k %= n;
18         int temp, i, j;
19         for(i=1; i<=k; i++)
20         {
21             temp = nums[n-i];
22             cout<<temp<<endl;
23             for(j=n-i-k; j>=0; j-=k)
24             {
25                  nums[j+k] = nums[j];
26                  cout<<nums[j+k]<<endl;
27             }
28             nums[j+k] = temp;
29         }
30         temp=n%k;
31         //套用第2种方法
32         rev(&nums[0],&nums[temp-1]);
33         rev(&nums[temp],&nums[k-1]);
34         rev(&nums[0],&nums[k-1]);
35     }
36 };
Rotate Array
```

