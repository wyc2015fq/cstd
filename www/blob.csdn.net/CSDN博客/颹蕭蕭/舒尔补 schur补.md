
# 舒尔补-schur补 - 颹蕭蕭 - CSDN博客


2018年10月25日 20:52:47[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2608


n x n 的矩阵可以写成分块形式：
$$
M = \left[\begin{matrix}
   A &amp; B  \\
   C &amp; D  
  \end{matrix} 
  \right]_{n \times n}
$$
其中 A 和 D 为方阵。
若 A 是非奇异的，则 A 在 M 中的舒尔补为：
$$
D - CA^{-1}B
$$
若 D 是非奇异的，则 D 在 M 中的舒尔补为：
$$
A - BD^{-1}C
$$
要记住上面的形式很容易，只要记住字母顺序 DCAB、ABDC 都是在 M 中**顺时针**排列的。
---若 A 非奇异，则有

---(1)

$$
\left[\begin{matrix}
   I &amp; 0  \\
   -CA^{-1} &amp; I  
  \end{matrix} \right]
  \left[\begin{matrix}
   A &amp; B  \\
   C &amp; D  
  \end{matrix} \right]
  \left[\begin{matrix}
   I &amp; -A^{-1}B  \\
   0 &amp; I  
  \end{matrix} \right]
  =\left[\begin{matrix}
   A &amp; 0  \\
   0 &amp; D-CA^{-1}B  
  \end{matrix} \right]\tag{1}
$$

---上式可以看作对矩阵 M 实施

---分块矩阵的初等行列变换

---。由 (1) 可得

$$
\left|\begin{matrix}
   A &amp; B  \\
   C &amp; D  
  \end{matrix} \right|
  = \left|\begin{matrix}
   A &amp; 0  \\
   0 &amp; D-CA^{-1}B  
  \end{matrix} \right|
  = |A||D-CA^{-1}B|
$$
---此时

$$
M 非奇异 \Longleftrightarrow D-CA^{-1}B 非奇异
$$

---同理，当 D 非奇异时

$$
M 非奇异 \Longleftrightarrow A-BD^{-1}C 非奇异。
$$

---分块矩阵的逆

---由式 (1) 还可以方便地求得分块矩阵的逆。

$$
\left[\begin{matrix}
   I &amp; -A^{-1}B  \\
   0 &amp; I  
  \end{matrix} \right]^{-1}
  \left[\begin{matrix}
   A &amp; B  \\
   C &amp; D  
  \end{matrix} \right]^{-1}
\left[\begin{matrix}
   I &amp; 0  \\
   -CA^{-1} &amp; I  
  \end{matrix} \right]^{-1}
  =\left[\begin{matrix}
   A^{-1} &amp; 0  \\
   0 &amp; (D-CA^{-1}B)^{-1}  
  \end{matrix} \right]
$$
---因而

$$
\left[\begin{matrix}
   A &amp; B  \\
   C &amp; D  
  \end{matrix} \right]^{-1}
  =\left[\begin{matrix}
   I &amp; -A^{-1}B  \\
   0 &amp; I  
  \end{matrix} \right]
  \left[\begin{matrix}
   A^{-1} &amp; 0  \\
   0 &amp; (D-CA^{-1}B)^{-1}
     \end{matrix} \right]
     \left[\begin{matrix}
   I &amp; 0  \\
   -CA^{-1} &amp; I  
  \end{matrix} \right]
$$


