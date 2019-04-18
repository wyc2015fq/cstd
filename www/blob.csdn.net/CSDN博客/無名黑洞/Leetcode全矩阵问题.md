# Leetcode全矩阵问题 - 無名黑洞 - CSDN博客
2014年02月05日 15:55:41[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1443
**目录**
**1、编号48 Rotate Image2、编号54 Spiral Matrix3、编号59 Spiral Matrix II4、编号73 Set Matrix Zeroes5、编号74 Search a 2D Matrix**
**1、编号48 Rotate Image**
You are given an n x n 2D matrix representing an image.
Rotate the image by 90 degrees (clockwise).
Follow up: Could you do this in-place?
可以硬算转了九十度后的目标矩阵和原矩阵各元素的对应关系。但是更简单的是先求原矩阵的转置矩阵，然后取反...
```cpp
class Solution {
public:
    void rotate(vector<vector<int> > &matrix) {
		for(int i = 0, n = matrix.size(); i < n; ++i){
			for(int j = i+1; j < n; ++j) swap(matrix[i][j], matrix[j][i]);
			reverse(matrix[i].begin(), matrix[i].end());        
		}
	}
   
};
```
**2、编号54 Spiral Matrix**
Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.
For example, Given the following matrix:
[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
You should return [1,2,3,6,9,8,7,4,5].
思路是使用包围盒Bounding Box方法，先确定四个边，然后向内缩。
```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int> > &matrix) {
        vector<int> r;
        int m = matrix.size();
        if(m == 0) return r;
        int n = matrix[0].size();
        
        int beginX = 0 , endX = n - 1;
        int beginY = 0 , endY = m - 1;
        
        while(true){/*Critical point here!*/
            for(int i = beginX; i <= endX; i++) r.push_back(matrix[beginY][i]);
            beginY++;
            if(beginY > endY) break;
            
            for(int i = beginY; i <= endY; i++) r.push_back(matrix[i][endX]);
            endX--;
            if(beginX > endX) break;
            
            for(int i = endX; i >= beginX; i--) r.push_back(matrix[endY][i]);
            endY--;
            if(beginY > endY) break;
            
            for(int i = endY; i >= beginY; i--) r.push_back(matrix[i][beginX]);
            beginX++;
            if(beginX > endX) break;
        }
        return r;
    }
};
```
**3、编号59 Spiral Matrix II**
Given an integer n, generate a square matrix filled with elements from 1 to n2 in spiral order.
上面一题的逆过程。所以也用逆包围盒方法就可以了。
```cpp
class Solution {
public:
    vector<vector<int> > generateMatrix(int n) {
        int left = 0;
        int right = n-1;
        int up = 0;
        int down = n-1;
        
        vector<vector<int> > result;
        for(int i = 0; i < n; i++ ){
            vector<int> tmp(n, 0);
            result.push_back(tmp);
        }
        
        int index = 1;
        while(true){
            for(int i = left; i <= right; i++ ) result[up][i] = index++;
            if(index == n*n+1) return result;
            up++;
            
            for(int i = up; i <= down; i++) result[i][right] = index++;
            if(index == n*n+1) return result;
            right--;
           
            for(int i = right; i >= left; i--) result[down][i] = index++;
            if(index == n*n+1) return result;
            down--;
             
            for(int i = down; i >= up; i--) result[i][left] = index++;
            if(index == n*n+1) return result;
            left++;
        }
    }
};
```
**4、编号73 Set Matrix Zeroes**
Given a m x n matrix, if an element is 0, set its entire row and column to 0. Do it in place.
Follow up:
Did you use extra space?
A straight forward solution using O(mn) space is probably a bad idea.
A simple improvement uses O(m + n) space, but still not the best solution.
Could you devise a constant space solution?
要求不能用额外的空间。这里使用了一个很巧妙的方法，就是把矩阵的第一行和第一列拿来储存是否要轻零另外的列和行的仓库。当然在当仓库前先要判定第一行和第一列自己是否需要清零。
```cpp
class Solution {
public:
    void setZeroes(vector<vector<int> > &matrix) {
        int m = matrix.size();
        if(m == 0) return;
        int n = matrix[0].size();
        
        bool firstRowNeedSet = false;
        bool firstColNeedSet = false;
        
        /*Check First Row and Col*/
        for(int i = 0; i < m; i++) 
            if(matrix[i][0] == 0){
                firstColNeedSet = true;
                break;
            }
        for(int i = 0; i < n; i++) 
            if(matrix[0][i] == 0){
                firstRowNeedSet = true;
                break;
            }
            
        /*Record info to first row and col*/
        for(int i = 1; i < m; i++)
            for(int j = 1; j < n; j++)
                if(matrix[i][j] == 0){
                    matrix[i][0] = 0;
                    matrix[0][j] = 0;
                }
        /*Reset matrix*//*!!!!!!!!*/
        for(int i = 1; i < m; i++) 
            for(int j = 1; j < n; j++) 
                if(matrix[i][0] == 0 || matrix[0][j] == 0) matrix[i][j] = 0;
  
            
        /*Reset first row and col*/
        if(firstColNeedSet) for(int i = 0; i < m; i++) matrix[i][0] = 0;
        if(firstRowNeedSet) for(int i = 0; i < n; i++) matrix[0][i] = 0;
    }
};
```
**5、编号74 Search a 2D Matrix**
Write an efficient algorithm that searches for a value in an m x n matrix. This matrix has the following properties:
Integers in each row are sorted from left to right.
The first integer of each row is greater than the last integer of the previous row.
For example,
Consider the following matrix:
[
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
Given target = 3, return true.
思路是折半查找。专门写了一个函数把矩阵的行列数转化为一维数组的index。
```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int> > &matrix, int target) {
        int m = matrix.size();
        if(m == 0) return false;
        int n = matrix[0].size();
        
        int startX = 0;
        int startY = 0;
        int endX = m-1;
        int endY = n-1;
        
        while(true){
            int mX, mY;
            bool check = GetMedian(startX, startY, endX, endY, mX, mY, n);
            
            /*!!!!Compare mid and end(for only 2 elements left)!!!!!*/
            if(matrix[mX][mY] == target) return true;
            if(!check) {
                if(matrix[endX][endY] == target) return true;
                break;
            }
            
            if(matrix[mX][mY] < target){
                startX = mX; startY = mY;
            }else{
                endX = mX; endY = mY;
            }
        }
        
        return false;
    }
    
    bool GetMedian(int startX, int startY, int endX, int endY, int &mX, int &mY, int n){
        int startIndex = startX * n + startY;
        int endIndex = endX * n + endY;
        int midIndex = (startIndex + endIndex)/2;
        if(midIndex == startIndex){/*!!!!!!!!!!*/
            mX = startX; mY = startY;
            return false;
        }else{
            mX = midIndex / n; mY = midIndex % n;
            return true;
        }
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
