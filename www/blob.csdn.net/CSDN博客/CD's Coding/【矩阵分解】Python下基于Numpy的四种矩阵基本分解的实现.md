# 【矩阵分解】Python下基于Numpy的四种矩阵基本分解的实现 - CD's Coding - CSDN博客





2016年12月04日 22:05:44[糖果天王](https://me.csdn.net/okcd00)阅读数：8572








### 0x00 需求

完成课堂上讲的关于矩阵分解的 

· LU、 

· QR（Gram-Schmidt） 

· Orthogonal Reduction  

    Householder reduction 

    Givens reduction 

程序实现，要求一个综合程序，根据选择参数的不同，实现不同的矩阵分解。
反正也是要写，就顺手做成了实现类，可以import调用的那种，为了写作业方便，也设置了输出中间过程，方便拿过程分。

### 0x01 作者
- 陈点 学号 201618013229031
- 个人邮箱 chendian@baidu.com
- 个人主页 blog.csdn.net/okcd00
- Github github.com/okcd00

### 0x02 文件树

```
E:\UCAS\矩阵分析与应用\CD_MatrixDecomp 的目录

2016/11/28  23:11                27 A.txt       % 用于测试各种不同种类输入的文件
2016/11/28  23:11                28 B.txt       % 用于测试非方阵进行LU分解时的文件
2016/11/23  15:51                40 LU.txt      % 用于测试LU分解的输入示例
2016/11/28  23:17            10,560 MatrixDecomp.py % 主程序，python MatrixDecomp.py即可
2016/11/23  15:50                27 QR.txt      % 用于测试QR分解的输入示例
2016/11/27  16:01             2,606 Readme.txt      % 说明文档
```

### 0x03 程序输入

封装为类，返回值实际为一个dict，在LU分解时返回的dict包含三个键值对：P,A,T，类似的QR分解时范围的是Q与R，value均为np.ndarray格式矩阵 

在import该类进行矩阵计算的时候可以直接调用各类函数，将该类为主进程打开时会通过简单的main函数的提示信息引导展示输入输出

```
E:\UCAS\矩阵分析与应用\BigHomework>python MatrixDecomp.py
> Current Selection is: <Default>
> Please show me the Matrix for Decomposition
> It can be a list or path to a Matrix_File
> Example: [[1,0],[0,1]] or "A.txt"
The Matrix is:
```

可直接输入形似[[1,0,0],[0,1,0],[0,0,1]]的list，也可以输入文件地址，需要注意地址应该加上引号，如”A.txt” 

目录下提供了课件上的LU分解的实例与QR分解时使用的矩阵

```
> Current Selection is: <Mode>
> Please Select Decomposition Type
> Example: LU GS HH or GV
The Matrix's Size:
```

此处可选择LU GS HH or GV，分别代表LU分解，Gram-Schmidt、Householder、与Givens的QR分解

### 0x04 参数

mdp.Show_Process = False  

（强烈推荐调成True跑一下看看效果，也不枉coding那么久QvQ） 

该参数控制是否输出形似如下形式的中间计算过程： 

Calculation[0]: 

Q = xxx 

R = xxx 

Calculation[1]: 

Q = xxx 

R = xxx 

Calculation[x]:
mdp.setMatA(mdp.getInput(‘Default’)) 

目前已经编码的合法参数为default, random, mode，其中random为随机生成矩阵，根据提示输入矩阵的行列数，值为0-9之间随机选择

### 0x05 程序源码

```python
#coding=utf8
# ========================================================
#   Copyright (C) 2016 All rights reserved.
#   
#   filename : MatrixDecomp.py
#   author   : okcd00 / chendian@baidu.com
#   date     : 2016-11-23
#   desc     : Matrix Decomposition
#   homepage : blog.csdn.net/okcd00
# ======================================================== 

# Basic package
import random
import os,sys,time

# Numpy for Matrix Calculation
import numpy as np
from numpy import *
from numpy import linalg as La

class MatrixDecomp:

    Time = 0
    Mode = "NULL" # LU QR(Gram-Schmidt\HouseHolder\Given)
    MatA = "No Input" # Matrix_A for Calculation
    bak_MatA = "temp" # Matrix_A Back_Up
    Show_Process = False

    def __init__(self):
        self.Time = time.localtime(time.time())

    def setMatA(self, inp):
        # Judge the type of inp, then achieve the Matrix
        if isinstance(inp, np.ndarray):
            self.MatA = inp
        elif isinstance(inp, list):
            self.MatA = np.array(inp)
        elif isinstance(inp, str):
            if os.path.exists(inp): 
                self.MatA = np.array(self.readFile(inp))
            elif os.path.exists(inp + '.txt'):
                self.MatA = np.array(self.readFile(inp + '.txt'))
        else: 
            print "Invalid Input"
        self.bak_MatA = self.MatA

    def MatDecomp(self, inp):
        self.Mode = inp
        try:
            if inp.upper() == "LU": return self.LU_Decomp(self.MatA)
            if inp.upper() == "GS": return self.GS_Decomp(self.MatA)
            if inp.upper() == "HH": return self.HH_Decomp(self.MatA)
            if inp.upper() == "GV": return self.GV_Decomp(self.MatA)
            return "Invalid Decomp Type. (LU/GS/HH/GV)"
        except Exception,e:
            return "Decomposition Error for %s" % str(e)

    def Row_Swap(self, mat, ra, rb):
        ret = mat
        if mat.ndim == 1:
            ret[ra], ret[rb] = mat[rb], mat[ra]
        if mat.ndim == 2:
            ret[[ra, rb],:] = mat[[rb, ra],:]
        return ret

    def Col_Swap(self, mat, ca, cb):
        ret = mat
        if mat.ndim == 1:
            ret[ca], ret[cb] = mat[cb], mat[ca]
        if mat.ndim == 2:
            ret[:,[ca, cb]] = mat[:,[cb, ca]]
        return ret

    def MaxLine(self, colomn, row):
        ret = row
        for idx in range(row, colomn.__len__()):
            if abs(colomn[idx]) > abs(colomn[ret]):
                ret = idx
        return ret

    def LU_Operation(self, A, cur):
        (rSize, cSize) = A.shape
        for r in range(cur+1, rSize):
            A[r][cur] = A[r][cur] / A[cur][cur]
            for c in range(cur+1, cSize):
                A[r][c] = A[r][c] - A[r][cur] * A[cur][c]
        return A

    def LU_GetAns(self, P1D, A):
        (rSize, cSize) = A.shape
        # Need to transform P from 1D to 2D
        P = np.zeros([rSize, rSize])
        for idx in range(rSize):
            P[idx][P1D[idx]-1] = 1
        L = np.eye(rSize, cSize)
        U = np.zeros([rSize, cSize])
        # Split MatrixA into L-Lower & U-Upper
        for r in range(rSize):
            for c in range(cSize):
                if r <= c : U[r][c] = A[r][c]
                else : L[r][c] = A[r][c]
        return {'P':P, 'L':L, 'U':U}

    def LU_Decomp(self, A): # PA = LU
        (rSize, cSize) = A.shape
        if rSize!=cSize :
            print "> LU_Decomp needs a Nonsingular Square Matrix."
            print "> Extend Matrix into a Square Matrix filled by zero."
            Size = max(rSize, cSize)
            Zero = np.zeros([Size,Size])
            Zero[:rSize,:cSize] = np.copy(A)
            A, (rSize, cSize) = np.copy(Zero), (Size, Size)
            print "> Current Matrix_A = \n", A
        P = np.arange(rSize) + 1
        for r in range(rSize):
            # Swap MaxLine(current_Colomn's Max abs_Value) to the top
            idxML = self.MaxLine(A[:,r], r)
            A = self.Row_Swap(A, idxML, r)
            P = self.Col_Swap(P, idxML, r)
            A = self.LU_Operation(A, r)
            if self.Show_Process:
                print 'Calculation[%d]:\nP = ' % r, P, '^T\nA = \n', A
        return self.LU_GetAns(P,A)

    def GS_Decomp(self, A): # A = QR
        (rSize, cSize) = A.shape
        Q, R = np.copy(A), np.zeros([rSize, cSize])
        for c in range(cSize):
            for r in range(c):
                if r < c:
                    # R_rc = qr^T * Ac
                    R[r][c] = np.dot(np.transpose(Q[:,r]), A[:,c])
                    Q[:,c] = Q[:,c] - R[r][c] * Q[:,r]
            R[c][c] = La.norm(Q[:,c])
            Q[:,c] = Q[:,c] / R[c][c]
            if self.Show_Process:
                print 'Calculation[%d]:\nQ = \n' % c, Q, '\nR = \n', R
        return {'Q':Q, 'R':R}

    def HH_Decomp(self, A): # A = QR / PA = T
        (rSize, cSize) = A.shape
        P = np.eye(rSize, cSize)
        for c in range(cSize):
            MatA, MatU = np.copy(A[c:,c:]), np.copy(A[c:,c]) # MatU = MatA[:,0]
            MatU[0] = MatU[0]+La.norm(MatU) if MatU[0]<0 else MatU[0]-La.norm(MatU)
            MatU.shape = (1, MatU.shape[0])
            MatU = np.transpose(MatU)
            MatR = np.eye(MatU.shape[0])
            # Transposing a 1-D array returns an unchanged view of the original array / Note for np.transpose
            UTU = np.dot(np.transpose(MatU), MatU)
            MatR = MatR - 2.0 * (
                (np.dot(MatU, np.transpose(MatU)) / UTU) if UTU!=0 else 0
            )
            MatA = np.dot(MatR, MatA)
            R = np.eye(rSize, cSize)
            R[c:,c:] = np.copy(MatR)
            P = np.dot(R, P)
            A[c:,c:] = np.copy(MatA)
            if self.Show_Process:
                print 'Calculation[%d]:\nR%d = \n' % (c+1,c+1), MatR, '\nR%dA%d = \n' % (c+1,c+1), MatA, '\nCurrent P = \n', P
        return {'Q':np.transpose(P), 'R':A }#, 'T':A}

    def GV_Rotate(self, A, i, j):
        (rSize, cSize) = A.shape
        ret = np.eye(rSize, cSize)
        upValue = sum(item**2 for item in A[j:i,j])
        c = upValue**0.5 / (upValue + A[i][j]**2)**0.5
        s = A[i][j] / (upValue + A[i][j]**2)**0.5
        ret[i][i], ret[j][j] = c, c
        ret[i][j], ret[j][i] = -s, s
        return ret

    def GV_Decomp(self, A): # A = QR
        (rSize, cSize) = A.shape
        U = np.eye(rSize, cSize)
        for c in range(cSize):
            for r in range(c+1, rSize):
                if A[r,c] != 0:
                    rot = self.GV_Rotate(A,r,c)
                    U = np.dot(rot, U)
                    A = np.dot(rot, A)
                if self.Show_Process:
                    print 'Calculation[%d,%d]:\nU%d%d = \n' % (r+1,c+1,r+1,c+1,), rot, '\nCurrent U = \n', U, '\nCurrent A = \n', A
        return {'Q':np.transpose(U), 'R':A }

    def readFile(self, filename):
        # The Matrix File needs to be splited
        with open(filename,'r') as f:
            ret = []
            lines = [ line for line in f.readlines() ]
            for each in lines :
                line = [ float(num) for num in each.split() ]
                ret.append(line)
            return ret      

    def getInput(self, inp='Default'):
        print "> Current Selection is: <%s>" % inp
        # Get varName or filePath from console for Original Matrix
        if inp.upper() == 'DEFAULT':
            # print "MatrixDecomp 1.0.7 (v1.0.7, Nov 27 2016, 22:54:40) Type \"help\" for more information."
            print "> Please show me the Matrix for Decomposition"
            print "> It can be a list or path to a Matrix_File"
            print "> Example: [[1,0],[0,1]] or \"A.txt\", \"LU\" etc."
            ret = input("The Matrix is: ")
        elif inp.upper() == 'RANDOM':
            # Define constants
            print "> Please show me the Matrix's Size, split by \',\' "
            print "> Example: 5,3 or 7,7"
            sz = raw_input("The Matrix's Size: ").split(',')
            r,c = int(sz[0]), int(sz[1])
            # Generate a target Matrix
            ret = random.randint(0,9, size=(r,c))
        elif inp.upper() == 'MODE':
            # Define constants
            print "> Please Select Decomposition Type"
            print "> Example: LU GS HH or GV"
            ret = raw_input("Type of the Matrix Decomposition is: ")
        elif inp.upper() == 'HELP':
            print """
                > Help v1.0.0 Authured by Chendian / okcd00

                > mdp.Show_Process
                > 该参数控制是否输出中间计算过程, 默认为False, 可在Main函数中改为True

                > mdp.setMatA(mdp.getInput('xxx'))
                > 目前已经编码的合法参数为default, random, mode, help
                """ 
            mdp.setMatA(mdp.getInput('Default'))
        # Can also be used for passing vars,
        # i.e. list or numpyArray
        else: ret = inp 
        return ret

if __name__ == "__main__":
    np.set_printoptions(suppress=True)
    mdp = MatrixDecomp()
    mdp.Show_Process = False # True, show mid-calculation
    mdp.setMatA(mdp.getInput('Default'))
    # mdp.setMatA(mdp.getInput('RANDOM'))
    print mdp.MatA
    Ans = mdp.MatDecomp(mdp.getInput('Mode'))
    try:
        print '==========Answer Sheet=========='
        for (k,v) in Ans.items():
            print '> Matrix', k, '=\n', v
    except Exception,e:
        print e, '\n', Ans   

"""
E:\UCAS\矩阵分析与应用\BigHomework>python MatrixDecomp.py
> Current Selection is: <Default>
> Please show me the Matrix for Decomposition
> It can be a list or path to a Matrix_File
> Example: [[1,0],[0,1]] or "A.txt", "LU" etc.
The Matrix is: "A.txt"
[[  0. -20. -14.]
 [  3.  27.  -4.]
 [  4.  11.  -2.]]
> Current Selection is: <Mode>
> Please Select Decomposition Type
> Example: LU GS HH or GV
Type of the Matrix Decomposition is: GS
==========Answer Sheet==========
> Matrix Q =
[[ 0.   -0.8  -0.6 ]
 [ 0.6   0.48 -0.64]
 [ 0.8  -0.36  0.48]]
> Matrix R =
[[  5.  25.  -4.]
 [  0.  25.  10.]
 [  0.   0.  10.]]
"""
```

### 0xFF 测试数据

LU.txt

> 
1 2 -3 4 

  4 8 12 -8 

  2 3 2 1 

  -3 -1 1 -4
QR.txt

> 
0 -20 -14 

  3 27 -4 

  4 11 -2
LU.txt for non-square

> 
1 2 -3 4 

  4 8 12 -8 

  2 3 2 1










