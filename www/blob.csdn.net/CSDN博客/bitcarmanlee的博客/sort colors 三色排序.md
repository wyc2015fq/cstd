
# sort colors 三色排序 - bitcarmanlee的博客 - CSDN博客


2019年03月09日 23:04:11[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：98



## 1.问题
给定一个包含红色、白色和蓝色，一共 n 个元素的数组，原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。
此题中，我们使用整数 0、 1 和 2 分别表示红色、白色和蓝色。
示例:
输入: [2,0,2,1,1,0]
输出: [0,0,1,1,2,2]
## 2.解法一之计数排序
计数排序的思路比较简单，就是得到各个元素出现的次数，然后重新写入原来的数组中。
直接上代码
`public static void sortColorsOnCardinality() {
        int[] colors = {0, 1, 2, 0, 1, 2, 0, 0, 2, 2, 1};
        int count0= 0, count1 = 0, count2 = 0;
        for(int num: colors) {
            if (num == 0) count0++;
            else if (num == 1) count1++;
            else count2++;
        }
        for(int i=0; i<colors.length; i++) {
            if (i < count0) {
                colors[i] = 0;
            } else if (count0 <= i && i < count0 + count1) {
                colors[i] = 1;
            } else {
                colors[i] = 2;
            }
        }
    }`很容易分析出计数算法的时间复杂度为O(2n)，要对原数组遍历两遍。
## 3.解法二之三路快排
上面的算法需要遍历两遍数组，能不能遍历一遍就搞定？答案是可以的，三路快排就可以搞定。
回忆一下快速排序的思想：每次选定一个pivot之后，将大于pivot的元素放一边，将小于pivot的元素放另外一边，递归最后可以使整个序列有序。
快排的平均时间复杂度为 nlogn。当数组基本有序或者数组中大量元素相等时，时间复杂度会退化到n^2。这个时候可以采取三路排序的思想：普通的快排是将序列分成了两部分，现在分成三部分，分别是大于pivot，等于pivot，小于pivot。
先看代码
`public static void sortColors() {
        int[] colors = {0, 1, 2, 0, 1, 2, 0, 0, 2, 2, 1};
        int left = 0;
        int right = colors.length - 1;
        int i = 0;
        while (i <= right) {
            if (colors[i] == 0) {
                swap(colors, i, left);
                left++;
                i++;
            } else if(colors[i] == 1) {
                i++;
            } else {
                swap(colors, i, right);
                right--;
            }
        }
    }
    public static void swap(int[] nums, int i, int j) {
        int tmp = nums[i];
        nums[i] = nums[j];
        nums[j] = tmp;
    }`其中，left是记录第一个1出现的位置，left的左边为0，右边为1，而right则是记录的第一个不是2出现的位置，这样right右边的数字就全是2。
i的作用是遍历数组，遍历的时候有三种情况：
1.如果数字是0，则将colors[i]与colors[left]元素互换，这样保证left之前的元素均为0，同时left与i同时加1。
2.如果数字为1,这个时候i指针加1。
3.如果数字为2，将colors[i]与colors[right]互换，这样保证数字2去了数组的右边，同时right减1，表示这个时候right右边的数字已经全为2。

