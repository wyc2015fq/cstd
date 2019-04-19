# 剑指offer——最小的K个数 - 刘炫320的博客 - CSDN博客
2017年08月06日 23:10:59[刘炫320](https://me.csdn.net/qq_35082030)阅读数：549标签：[剑指offer-java实现																[最小的k个数																[动态规划																[快速排序																[partition](https://so.csdn.net/so/search/s.do?q=partition&t=blog)](https://so.csdn.net/so/search/s.do?q=快速排序&t=blog)](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=最小的k个数&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer-java实现&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
输入n个整数，找出其中最小的K个数。例如输入4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4,。
# 2.问题解答
这相当于部分排序，想法也有可能是多种多样，基本上是在查找与排序上进行一些优化。如何让两个子任务获得平衡的衔接，是要思考的问题。
## 2.1 level1
最简单的办法就是查找K次，每次查找最小的那个数加入到最终结果中，这样的时间复杂度是O（KN），这还是使用了hash表的情况。 
//查找K次，时间复杂度是O（KN）
```
public static ArrayList<Integer> GetLeastNumbers_Solution(int [] input, int k) {
        ArrayList result = new ArrayList();
        if(k>input.length)
            return result;
        Set<Integer> indexs = new LinkedHashSet();
        for (int i = 0; i < k; ++i) {
            int min = Integer.MAX_VALUE;
            int minIndex=-1;
            for (int j = 0; j < input.length; ++j) {
                if (min > input[j]) {
                    if (!indexs.contains(j)) {
                        minIndex=j;
                        min = input[minIndex];
                    }
                }
            }
            if(minIndex==-1)
                break;
            indexs.add(minIndex);
        }
        for (int index : indexs) {
            result.add(input[index]);
        }
        return result;
    }
```
## 2.2 level2
当然，我们会想运用类似动态规划的思想来做，那就是遍历一次数组，每经历一个数字，就去判断是否应当把当前数字添加进去。理想状态的话，时间复杂度应当是O（N），但是由于我们过分理想化判断是否应当把当前数字添加进去。而下面的版本实现中，实现了O（K）的时间复杂度，因此最终的时间复杂度是O（KN）。
```
public static ArrayList<Integer> GetLeastNumbers_Solution(int [] input, int k) {
        ArrayList<Integer> result=new ArrayList<Integer>();
        if(input.length<=0||k<=0||k>input.length){
            return result;
        }
        for(int i=0;i<input.length;i++){
            result=Insert(result,input[i],k);
        }
        return result; 
    }
    public static ArrayList<Integer> Insert(ArrayList<Integer> res,int aim,int k){
        int i=0;
        for(;i<res.size()&&i<k;i++){
            //如果有比它大的数
            if(res.get(i)>=aim){
                res.add(i, aim);
                //如果已经增加过度了，就移除多的那个。
                if(res.size()>k)
                    res.remove(k);
                return res;
            }
        }
        //当还不够n的时候
        if(i<k)
            res.add(aim);
        return res;
    }
```
当然，已经可以证明，查找的最低时间复杂度为O（logn），因此这种方法最低也只能达到O（nlogk）。
## 2.3 level3
当然，如果想省事的话，可以直接使用快速排序，这样的话，时间复杂度能达到O（nlogn），比之前的两个O(KN)要小上一些。
```cpp
public ArrayList<Integer> GetLeastNumbers_Solution(int [] input, int k) {
        Arrays.sort(input);//快速排序
        ArrayList<Integer> list=new ArrayList<Integer>();
        if(k>input.length|k<=0){//判断是否存在越界
            return list;
        }else{for (int i = 0; i < k; i++) {
        list.add(input[i]);
         }
                return list; 
            } 
    }
```
## 2.4 level4
如果提到了快排，当然不能少了它的著名优化partition方法，它经过优化后，最快可以达到O（N）的时间复杂度。
```
public ArrayList<Integer> GetLeastNumbers_Solution(int[] input, int k) {
        ArrayList<Integer> res = new ArrayList<>();
        if (input == null || input.length < k) return res;
        quickSolve(input, 0, input.length - 1, k);
        for (int i = 0; i < k; i++) {
            res.add(input[i]);
        }
        return res;
    }
    public void quickSolve(int[] arr, int left, int right, int k) {
        if (left > right) {
            return;
        }
        int id = partition(arr, left, right);
        if (id == k) {
            return;
        }
        if (id > k) {
            quickSolve(arr, left, id - 1, k);
        } else {
            quickSolve(arr, id + 1, right, k);
        }
    }
    public int partition(int[] arr, int left, int right) {
        int i = left + 1;
        int j = right;
        while (true) {
            while (j >= left + 1 && arr[j] > arr[left]) {
                j--;
            }
            while (i <= right && arr[i] < arr[left]) {
                i++;
            }
            if (i > j) break;
            swap(arr, i, j);
            i++;
            j--;
        }
        swap(arr, left, j);
        return j;
    }
    public void swap(int[] arr, int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
```
## 2.5 level5
如果使用最大堆的话，时间复杂度就可以达到理想的O（NlogK）了。
```
// 使用最大堆
    public ArrayList<Integer> GetLeastNumbers_Solution(int [] input, int k) {
        ArrayList<Integer> list = new ArrayList<>();
        if(k <= 0 || k > input.length){
            return list;
        }
        // 将数组input的前k的元素构造成堆
        for(int i = (k - 1) / 2; i >= 0; i --){
            down(input, i, k);
        }
        for(int i = k; i < input.length; i++){
            if(input[i] >= input[0]){
                continue;
            }
            // 该元素小于堆顶元素
            input[0] = input[i];
            down(input, 0, k);
        }
        for(int i = 0;i < k; i++){
            list.add(input[i]);
        }
        Collections.sort(list);
        return list;
    }
    // 下滤操作,k表示堆的大小，hole表示需要下滤的元素的位置
    public void down(int[] a, int hole, int k){
        // 左孩子和右孩子位置
        int left = 2 * hole + 1;
        int right = 2 * hole + 2;
        if(left >= k){
            // 左孩子位置大于等于k，则超出堆的大小，即该结点没有孩子了
            return;
        }else if(right >= k){
            // 只有左孩子
            if(a[hole] < a[left]){
                swap(a, hole, left);
            }
            return;
        }else{
            // 左右孩子都有
            if(a[hole] > a[left] && a[hole] > a[right]){
                // 比两个孩子的值都大，则下滤结束
                return;
            }else if(a[left] > a[right]){
                // 不是比两个孩子都大，那么如果左孩子大于右孩子，则下滤到左孩子的位置
                swap(a, hole, left);
                // 对左孩子进行递归下滤
                down(a, left, k);
            }else{
                // 下滤到右孩子
                swap(a, hole, right);
                // 对右孩子进行递归下滤
                down(a, right, k);
            }
        }
    }
    // 交换两个元素
    public void swap(int[] a, int i, int j){
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
```
当然看起来可能很复杂，但是使用Java自带的优先队列来构建堆，就会容易很多了，这里，我们构建一个小顶堆（堆顶最小）。
```cpp
public ArrayList<Integer> GetLeastNumbers_Solution(int [] input, int k) {
        ArrayList<Integer> list = new ArrayList<>();
        if(input == null || input.length == 0 || k > input.length) return list;
        PriorityQueue<Integer> minPQ = new PriorityQueue<>();
        for(int i = 0; i < input.length; i++) {
            minPQ.offer(input[i]);
        }
        for(int i = 0; i < k; i++) {
            list.add(minPQ.poll());
        }
        return list;
    }
```
# 3. 小结
上面的前2种方法，都已经知道，要把这个任务分为2个部分，一个部分是外循环，另一个是内循环，外循环是固定的，必须达到O（N），而在内循环部分，尚有优化空间，Level5就是对前两种的优化。而Level3和4则是从另外一个角度，先进行排序，后进行查找，这样的话，就可以把两个子任务合二为一，通过最优的算法，可以实现时间复杂度为O（N）的算法，这也不失为一种明智的选择。
