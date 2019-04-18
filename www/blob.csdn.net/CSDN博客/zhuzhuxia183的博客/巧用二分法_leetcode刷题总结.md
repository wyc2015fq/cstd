# 巧用二分法_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月13日 14:08:15[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：48
个人分类：[数据结构和算法																[二分搜索](https://blog.csdn.net/zhuzhuxia183/article/category/8608093)](https://blog.csdn.net/zhuzhuxia183/article/category/8608092)

所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)








# 二分法巧解题

## 谈到二分法，你会想到啥?

```
嗯，本人作为一名非ACMer的小菜鸡，一开始的时候，脑子想到的就是上过的算法课上老师说可以用二分法在有序数组里面进行快速查找，并且又一个十分显著的特征，时间复杂度是log2n 量级的。
```

记忆中二分查找的代码模版：

```
public void find(int[] num,int target){
	int left = 0,right = num.lenth;
	while(left<right){
		int mid = (left+mid)/2;
		if(num[mid]<target){
			left = mid+1;
		}else if(num[mid]>target){
			right = mid-1;
		}else{
			return mid;
		}
	}
	if(num[left==target])	{
		return left;
	}
	return -1;
}
```

哈哈哈，其实也就是顺手一写的，嗯，大致也就这个样子。用while循环里面包裹的代码，是不断的让left\right来逼近我们最有可能的结果。

## 但是刷完一些leetcode上面的题目，才发现原来二分法不只是应用在一个有序数组中进行查找，嗯，我准备先下个结论

```
二分查找，首先得明确查找的是什么目标，他满足什么条件，如何进行逼近，并且确保最终能够得到结果。
```

二分查找，可以认为是有序数组查找的一种加速策略，如果只存在局部有序，便可以考虑局部加速嘛

当然我们之前学习的二分法，是整个数组都是有序的嘛，那么问题来了，如果不是整体有序，该怎么做呢？来，我们来撕撕几个题目。（尽可能的循序渐进）
- 1、寻找插入位置：[https://leetcode-cn.com/problems/search-insert-position/](https://leetcode-cn.com/problems/search-insert-position/)

题目：

给定一个排序数组和一个目标值，在数组中找到目标值，并返回其索引。如果目标值不存在于数组中，返回它将会被按顺序插入的位置。

你可以假设数组中无重复元素。

示例 1:

输入: [3,4,5,1,2]

输出: 1

示例 2:

输入: [4,5,6,7,0,1,2]

输出: 0

上代码，再剖析：

```
public static int searchInsert(int[] nums, int target) {
		int left = 0,right = nums.length;
		while(left<right){
			int mid = (left+right)/2;
			if(num[mid]<target){
				left = mid+1;
			}else{
				right = mid;
			}
		}
		return left;
}
```

emmm，上面的代码可以说是十分的简洁，分析起来，我们可以这样想，left是搜索的最左边界，right是 搜索的最右边界。重点是在判断里面

```
if(num[mid]<target){
				left = mid+1;
			}else{
				right = mid;
			}
```

首先是如果num[mid]<target的情况下，结果的左边界一定得为mid的右边一个，所以left = mid+1,然后else代表的意思的是 num[mid]>=target的情况，此时此刻，我们能够确定的事结果的右边界为 mid。
- 嗯，这里需要去思考的一个问题是，为什么是要在满足 “<” 的时候执行 left = mid+1，在">" 的时候执行 right = mid; 提示下，这里指的是插入的位置噢

这块的代码，需要着重理解下，原因是接下来你会看到重要的变种主要是在这里做手脚的，啧啧啧。
- 2、在排序数组中查找元素的第一个和最后一个位置：[https://leetcode-cn.com/problems/find-first-and-last-position-of-element-in-sorted-array/](https://leetcode-cn.com/problems/find-first-and-last-position-of-element-in-sorted-array/)

给定一个按照升序排列的整数数组 nums，和一个目标值 target。找出给定目标值在数组中的开始位置和结束位置。

你的算法时间复杂度必须是 O(log n) 级别。

如果数组中不存在目标值，返回 [-1, -1]。

示例 1:

输入: nums = [5,7,7,8,8,10], target = 8

输出: [3,4]

示例 2:
输入: nums = [5,7,7,8,8,10], target = 6

输出: [-1,-1]

这里我们该怎么做呢，如果用常规的二分查找的话，并没有要求返回元素的第一个位置和最后一个位置，代码如下：

```
public int[] searchRange(int[] nums, int target) {
	 int[] res = {-1,-1};
	int left = 0,right = nums.length-1;
	//首先求第一个位置
	while(left<right){
		int mid = (left+right)/2;
		if(nums[mid]>target){
			right = mid-1;
		}else{
			left = mid;
		}
	}
	if(nums[left]==target){
		res[0] = left;
	}else{
		return res;//不存在
	}
	//然后求最后一个位置
	right = nums.length -1;
	while(left<right){
		int mid = (left+right+1)/2;
		if(nums[mid]<target){
			left = mid+1;
		}else{
			right = mid;
		}
	}
	res[1] = right;
	return res;
}
```

emmm,这里我们注意两个地方哈：

第一个地方是：如果求最左边，其实我们想要的动作是不断的利用最右边界来靠近最左边界，这样求得的就是第一个；反之，利用最左边界不断靠近最右边界，就是求得最后一个；

第二个地方：如果记得求中间值的时候，我们默认是靠左边的，所以如果想要求最后一个的时候，需要(left+right+1)/2;- 3、搜索旋转排序数组：[https://leetcode-cn.com/problems/find-first-and-last-position-of-element-in-sorted-array/](https://leetcode-cn.com/problems/find-first-and-last-position-of-element-in-sorted-array/)

假设按照升序排序的数组在预先未知的某个点上进行了旋转。

( 例如，数组 [0,1,2,4,5,6,7] 可能变为 [4,5,6,7,0,1,2] )。

搜索一个给定的目标值，如果数组中存在这个目标值，则返回它的索引，否则返回 -1 。

你可以假设数组中不存在重复的元素。

你的算法时间复杂度必须是 O(log n) 级别。

emmmm，分析下，这个属于局部有序嘛，都提示了要O(log n) ，上代码：

```
//	大体思路是：我们先找出来是是从哪里开始旋转的，找出来之后，再开始
public static int search(int[] nums, int target) {

		//这里的left\right是指旋转点的最左边界、最右边界
		int left = 0,right = nums.length;
		while(left<right){
			int mid  = (left+right)/2;
			if(nums[mid]<nums[right]){
					right = mid;
			}else {
					left  = mid+1;
			}
		}
		//接下里我们按照0-left-1,和 left-num.length-1两段来进行二分搜索嘛，
}
```

emmm，上面的代码，我们来分析，如果if(nums[mid]<nums[right])条件成立时，即是我们可以确定的是，当前mid-right是属于同一段，所以我们可以知道那个旋转点的最右边界为mid，反之，可以知道mid到righr之间一定不属于同一段，所以那个旋转点一定在mid到right之间，且不是mid,所以left = mid+1;
- 4搜索旋转排序数组 II [https://leetcode-cn.com/problems/search-in-rotated-sorted-array-ii/](https://leetcode-cn.com/problems/search-in-rotated-sorted-array-ii/)

假设按照升序排序的数组在预先未知的某个点上进行了旋转。

( 例如，数组 [0,0,1,2,2,5,6] 可能变为 [2,5,6,0,0,1,2] )。

编写一个函数来判断给定的目标值是否存在于数组中。若存在返回 true，否则返回 false。

示例 1:

输入: nums = [2,5,6,0,0,1,2], target = 0

输出: true

示例 2:
输入: nums = [2,5,6,0,0,1,2], target = 3

输出: false

这里和上面的思路还是一样，但是如何处理这些重复的值，还是十分麻烦的，我们的解决思路是，再进行遍历的时候，从left向右遍历去掉相同的值，然后right向左遍历去掉相同的值，哈哈哈，就这样,还是一样，找到旋转点，就可以啦

```
public boolean search(int[] nums, int target) {
	int left  =0,right = nums.length-1;
	while(left<right){
		while(left+1<right&&nums[left]==nums[left+1]){
			left++;
		}
		while(right-1>left && nums[right]==nums[right-1]){
			right --;
		}
		int mid = (left+right)/2;
		if(nums[mid]<=nums[right]){
			right = mid;
		}else{
			left = mid+1;
		}
	}
	//嗯，这里的right/left就是我们旋转点，剩下的和上面一样，运用常规的二分查找就OK啦
}
```

当然我觉得这里值得思考的地方是这里，当nums[mid]=nums[right]是应该算在哪里，我认为是处在同一个有序序列中，所以和 <的情况一致，这里很重要噢

```
if(nums[mid]<=nums[right]){
			right = mid;
		}else{
			left = mid+1;
		}
```
- 5、寻找旋转排序数组中的最小值 [https://leetcode-cn.com/problems/find-minimum-in-rotated-sorted-array/](https://leetcode-cn.com/problems/find-minimum-in-rotated-sorted-array/)

假设按照升序排序的数组在预先未知的某个点上进行了旋转。

( 例如，数组 [0,1,2,4,5,6,7] 可能变为 [4,5,6,7,0,1,2] )。

请找出其中最小的元素。

你可以假设数组中不存在重复元素。

示例 1:

输入: [3,4,5,1,2]

输出: 1

示例 2:
输入: [4,5,6,7,0,1,2]

输出: 0

这个思路其实更加的直接，emmm,就是直接求旋转点

```
public int findMin(int[] nums) {
	int left=0,right = nums.length;
	while(left<right){
		int mid = (left+right)/2;
		if(num[mid]<nums[right]){
			right = mid;
		}else{
			left = mid+1;
		}
	}
	return nums[left];
}
```
- 6、寻找两个有序数组的中位数 [https://leetcode-cn.com/problems/median-of-two-sorted-arrays/](https://leetcode-cn.com/problems/median-of-two-sorted-arrays/)

给定两个大小为 m 和 n 的有序数组 nums1 和 nums2。

请你找出这两个有序数组的中位数，并且要求算法的时间复杂度为 O(log(m + n))。

你可以假设 nums1 和 nums2 不会同时为空。

示例 1:

nums1 = [1, 3]

nums2 = [2]

则中位数是 2.0

示例 2:

nums1 = [1, 2]

nums2 = [3, 4]

则中位数是 (2 + 3)/2 = 2.5

刚刚开始的时候难免会一脸蒙蔽，但是可以这样想：我们求中位数，可以想像成我们是想把我们的两个数组分割成为两个部分，num1,num2的两个数组分别分成 num11,num12,num21,num22四个部分，然后我们认为num11,num21是前一半部分，num12,num22是后一个部分。需要做的事情是求出这个两个部分的分割，比如我们用i变量来标示 num1[0:i]、num1[i,num1.length];(左边包含，右边不包含)这个两个部分，用变量j来标示num2[0:j]、num2[j,num2.length];两个部分，其实我们是可以知道i+j是满足一定的关系的，因为我们是均分成两个部分嘛，所以有 i+j = (m+n+1)/2的。也就是如果求出来了i,便可以求出j，所以我们只需要对于i,任意求一个就可以。当然我们是选择是长度较小的那个来进行求解。

```
public double findMedianSortedArrays(int[] A, int[] B) {
        int length_A = A.length;
        int length_B = B.length;
        if(length_A>length_B){
            int [] temp = A;	A = B; B = temp;
            int temp_length= length_A;length_A = length_B;length_B = temp_length;
        }
        int left = 0, right = length_A;//这里是针对i来进行二分的，所以left、right分别描述的是i的左右边界
        while(left<=right){
            int i = (left+right)/2;
            int j = (length_A+length_B+1)/2-i;
            if(i<right&&A[i]<B[j-1]){
                left = i+1;
            }else if(i>left&&B[j]<A[i-1]){
                right = i-1;
            }else{
                int left_num=0;
                if(i==0){
                    left_num = B[j-1];
                }else if (j==0){
                    left_num = A[i-1];
                }else{
                    left_num = Math.max(A[i-1],B[j-1]);
                }
                if((length_A+length_B)%2==1) return left_num;
                int right_num = 0;
                if(i==length_A) right_num = B[j];
                else if(j==length_B) right_num= A[i];
			    else  right_num= Math.min(A[i],B[j]);
                return (left_num+right_num)/2.0;
            }
        }
        return 0.0;
    }
```

emmm,这里使用二分法注意的是，while(left<=right)这个里面用的是= 号，原因是结果肯定是存在的，当满足想等的时候，肯定是会跳入else中，肯定会返回结果，所以直接就是答案啦。当然程序不知道，所以最后return 随便一个double类型的数值就行惹

```
if(i<right&&A[i]<B[j-1]){
                left = i+1;
            }else if(i>left&&B[j]<A[i-1]){
                right = i-1;
            }else{
            	……
            }
```

针对二分法，复习就到这里，之后有新的感受再舔舔补补。








