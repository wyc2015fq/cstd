# 搜索相关专题_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月14日 21:12:55[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：42
个人分类：[搜索、深度、回溯																[数据结构和算法](https://blog.csdn.net/zhuzhuxia183/article/category/8608092)](https://blog.csdn.net/zhuzhuxia183/article/category/8611658)

所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)








## leetcode上面很多关于状态求解的题目，本质上都是属于搜索问题，对于基本的搜索，主要可以分成BFS和DFS两种。

可能部分小伙伴对于搜索不是很熟悉，但是一说深度(DFS)和广度遍历(BFS)就会联想到图的广度和深度遍历，其实除了图有BFS、DFS，树也有嘛，层次遍历就是广度遍历嘛，前序、中序、后序遍历就是深度遍历嘛。当然除了很明显的遍历搜索，有些属于隐性的深度、广度搜索，一般来说深度搜索会比较常见点，例如，回溯法大多都是采用深度递归遍历的嘛。废话不多说，我们来上题目吧。
- 
1、N皇后 [https://leetcode-cn.com/problems/n-queens/](https://leetcode-cn.com/problems/n-queens/)

n 皇后问题研究的是如何将 n 个皇后放置在 n×n 的棋盘上，并且使皇后彼此之间不能相互攻击。

给定一个整数 n，返回所有不同的 n 皇后问题的解决方案。

每一种解法包含一个明确的 n 皇后问题的棋子放置方案，该方案中 ‘Q’ 和 ‘.’ 分别代表了皇后和空位。

示例:

输入: 4

输出: [

[".Q…",  // 解法 1

“…Q”,

“Q…”,

“…Q.”],

["…Q.",  // 解法 2

“Q…”,

“…Q”,

“.Q…”]

]

解释: 4 皇后问题存在两个不同的解法。
如果你说你会回溯法，N皇后都不会，那怕是说不过去噢。代码如下：

```
public List<List<String>> solveNQueens(int n) {
        List<List<String>> res = new ArrayList<>();
        int[] flag = new int[n+1];
        traceBack(flag,1,n+1,res);
        return res;
    }

    private void traceBack(int[] flag, int index, int length, List<List<String>> res) {
        if (index==length){//这个时候是全部填完了
            List<String> item = new ArrayList<>();
            StringBuilder builder = new StringBuilder();
            for (int i = 1; i < length; i++) {
                builder.delete(0,length);
                for (int j = 1; j < length; j++) {
                    if (flag[i]==j){
                        builder.append("Q");
                    }else {
                        builder.append(".");
                    }
                }
                item.add(builder.toString());
            }
            res.add(item);
            return;
        }

        for (int i = 1; i <length; i++) {
            if(isValid(flag,index,i)){
                flag[index] = i;
                traceBack(flag,index+1,length,res);
            }
        }

    }

    private boolean isValid(int[] flag, int index, int col) {
        for (int j = 1; j < index; j++) {
            if (flag[j]==col||Math.abs(j-index)==Math.abs(flag[j]-col)){
                return false;
            }
        }
        return true;
    }
```

对于N皇后这个问题，可以说这种解法是一种十分经典，也是十分基础的解法。但是我们仍然可以从上述的解答中学习到一些东西，比如我们是如何进行搜索的呢，是通过不断的index+1来进行搜索的，默认来看，这里的搜索顺序是从1-length-1。所以说回溯这个词也是十分有趣的嘛，像这种题目，都是有明显的顺序，然后index值等于某个边界时，便完成一次深度搜索。
- 
2、解数独 [https://leetcode-cn.com/problems/sudoku-solver/](https://leetcode-cn.com/problems/sudoku-solver/)

编写一个程序，通过已填充的空格来解决数独问题。

一个数独的解法需遵循如下规则：

数字 1-9 在每一行只能出现一次。

数字 1-9 在每一列只能出现一次。

数字 1-9 在每一个以粗实线分隔的 3x3 宫内只能出现一次。

空白格用 ‘.’ 表示。
这个题目，同样是一个经典的搜索题目，还是采用深度搜索可以求解的，代码如下：

```
public static void solveSudoku(char[][] board) {
        tryNextValue(board);
    }

    private static boolean tryNextValue(char[][] board) {
        //首先判断有没有填充完全
        boolean find = false;
        int i=0,j=0;
        
        for (i=0; i < board.length; i++) {
            for (j=0; j < board[0].length; j++) {
                if (board[i][j]=='.'){
                    find = true;
                    break;
                }
            }
            if (find) break;
        }
        
        if (i==9){
            return true;
        }
            
        //此时我们得到i,j
        for (char num = '1'; num <= '9'; num++) {
            if (isValidSudoku(board,i,j,num)){
                board[i][j] = num;
                if (tryNextValue(board)){
                    return true;
                }
                board[i][j]='.';
            }
        }
        return false;
    }

    public static boolean isValidSudoku(char[][] board,int i,int j,char c) {
        for (int k = 0; k < 9; k++) {
            if (board[i][k]==c) return false;
            if (board[k][j]==c) return false;
            if (board[(i/3)*3+k/3][(j/3)*3+(k%3)]==c) return false;
        }
        return true;
    }
```

这里呢，我们是利用我们的一个二维数组board来作为我们搜索的一个依据，依次进行遍历，碰到空格，就跳出来，并且用全局变量i、j来进行记录，并且通过i==9是否满足来判断当前是不是已经填满了。其中

```
//此时我们得到i,j
        for (char num = '1'; num <= '9'; num++) {
            if (isValidSudoku(board,i,j,num)){
                board[i][j] = num;
                if (tryNextValue(board)){
                    return true;
                }
                board[i][j]='.';
            }
        }
```

这一块的代码中，我们可以看到，这里就是深度搜索的基本套路，判断当前是不是可以进行搜索的，如果可以，

1、先修改相关的变量

2、深度搜索

3、撤回修改- 
3、 全排列  [https://leetcode-cn.com/problems/permutations/](https://leetcode-cn.com/problems/permutations/)

给定一个没有重复数字的序列，返回其所有可能的全排列。

示例:

输入: [1,2,3]

输出:

[

[1,2,3],

[1,3,2],

[2,1,3],

[2,3,1],

[3,1,2],

[3,2,1]

]
这个题目，有两种办法

一种是利用一个boolean类型的数组来标记我们有没有使用第几个数，然后进行深度遍历，直到长为length的数组全部都标记也使用的时候，遍历结束，

另外一种，可以利用交换，从左边开始，依次与后边进行交换，然后index+1,直到index== length-1为止，当然为什么不是index==length呢，是由于如果index ==  length-1的话，后面就不存在交换，自然不需要搜索，代码如下：
```
public List<List<Integer>> permute(int[] nums) {
        List<List<Integer>> res = new ArrayList<>();
        traceback(nums,0,nums.length-1,res);
        return res;
    }

    private void traceback(int[] nums, int start, int length, List<List<Integer>> res) {
        if (start==length){
            List<Integer> item = new ArrayList<>();
            for (int num:nums) {
                item.add(num);
            }
            res.add(item);
            return;
        }

        for (int i = start; i <=length; i++) {
            swap(nums,start,i);
            traceback(nums,start+1,length,res);
            swap(nums,start,i);
        }

    }
    
    private void swap(int[] nums, int i, int start) {
        int temp = nums[i];
        nums[i] = nums[start];
        nums[start] = temp;
    }
```
- 4、全排列 II  [https://leetcode-cn.com/problems/permutations-ii/](https://leetcode-cn.com/problems/permutations-ii/)

给定一个可包含重复数字的序列，返回所有不重复的全排列。

示例:

输入: [1,1,2]

输出:

[

[1,1,2],

[1,2,1],

[2,1,1]

]
这个题目主要是为了筛去重复的，加一个isDuplicate函数进行判断即可。

```
public List<List<Integer>> permuteUnique(int[] nums) {
        Arrays.sort(nums);
        List<List<Integer>> result = new ArrayList<>();
        traceback(nums,0,nums.length-1,result);
        return result;

    }

    private void traceback(int[] nums, int start, int length, List<List<Integer>> res) {
        if (start==length){
            List<Integer> item = new ArrayList<>();
            for (int num:nums) {
                item.add(num);
            }
            res.add(item);
            return;
        }

        for (int i = start; i <=length; i++) {
            if(isDuplicate(nums,start,i)){
                swap(nums,start,i);
                traceback(nums,start+1,length,res);
                swap(nums,start,i);
            }

        }

    }

    private boolean isDuplicate(int[] nums, int start, int end) {
        for (int j = start; j <end; j++) {
            if (nums[j]==nums[end]){
                return false;
            }
        }
        return true;
    }

    private void swap(int[] nums, int i, int start) {
        int temp = nums[i];
        nums[i] = nums[start];
        nums[start] = temp;
    }
```

其他的和上面给出的代码很像，关键是其中判断重复的函数：

```
private boolean isDuplicate(int[] nums, int start, int end) {
        for (int j = start; j <end; j++) {
            if (nums[j]==nums[end]){
                return false;
            }
        }
        return true;
    }
```

这里从start-end-1来判断是否有等于nums[end]是为了保证，如果之前有与end相同的元素的话，就不会进行搜索，就可以避免重复！～

嗯，码字好累，歇会歇会～













