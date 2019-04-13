
# 增广拉格朗日函数法（ALM） - 颹蕭蕭 - CSDN博客


2019年01月07日 17:13:56[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2386


## 增广拉格朗日函数法（ Augmented Lagrangian method）
### 一、等式约束
考虑问题：
$$
\min_x \;f(x)\\s.t. \;c_i(x) = 0, \quad i=1,\cdots,m.
$$
定义增广拉格朗日函数：
$$
L_t(x,\lambda) = f(x) - \sum_i \lambda_ic_i(x) + \frac{t}{2}\sum_i\big(c_i(x)\big)^2
$$
算法迭代步骤为：
固定
$$
\lambda
$$
, 更新x：
$$
x^+ = \mathop{argmin}_x L_t(x;\lambda)
$$
意味着
$$
\nabla_x L_t(x^+;\lambda) =  \nabla f(x^+) - \sum_i\big( \lambda_i-tc_i(x^+)\big)\nabla c_i(x^+) = 0
$$
更新
$$
\lambda
$$
:
$$
\lambda_i^+ = \lambda_i-tc_i(x^+)
$$

---二、不等式约束

---考虑问题：

$$
\min_x \;f(x)\\s.t. \;c_i(x) \geq 0, \quad i=1,\cdots,m.
$$

---其等价形式为：

$$
\min_x \;f(x)\\s.t. \;c_i(x) - \nu_i =0, \quad \nu_i \geq 0,\quad i=1,\cdots,m.
$$

---定义带约束的增广拉格朗日函数：

$$
L_t(x,\lambda) = f(x) - \sum_i \lambda_i \big(c_i(x)-\nu_i(x)\big) + \frac{t}{2}\sum_i\big(c_i(x)-\nu_i(x)\big)^2 \\
s.t. \quad  \nu_i \geq 0,\quad i=1,\cdots,m.
$$

---算法迭代步骤为：

---固定

$$
\lambda
$$
---, 更新

$$
x,\nu
$$
---：

---(1)

$$
(x^+,\nu^+) = arg\min_{x,\nu} \quad L_t(x;\lambda) \\ = arg\min_{x,\nu}\quad f(x) + \sum_i \bigg\{ -\lambda_i \big(c_i(x)-\nu_i(x)\big) + \frac{t}{2}\big(c_i(x)-\nu_i(x)\big)^2 \bigg\} \tag{1}\\ s.t. \quad  \nu_i \geq 0,\quad i=1,\cdots,m.
$$

---更新

$$
\lambda
$$
---:

$$
\lambda_i^+ = \lambda_i-t(c_i(x^+)-\nu_i^+)
$$

---事实上，算法中的

$$
\nu
$$
---可以消去，由(1)式

---(2)

$$
(x^+,\nu^+) =  arg\min_{x,\nu}\quad f(x) + \sum_i \bigg\{ -\lambda_i \big(c_i(x)-\nu_i(x)\big) + \frac{t}{2}\big(c_i(x)-\nu_i(x)\big)^2 \bigg\} \\ =  arg\min_{x,\nu}\quad f(x) + \frac{t}{2}\sum_i \bigg\{ -(\frac{\lambda_i}{t})^2 + \big(c_i(x)-\nu_i(x) - \frac{\lambda_i}{t}\big)^2 \bigg\} \\= arg\min_{x,\nu} \quad f(x) + \frac{t}{2}\sum_i \bigg\{  \big(c_i(x)-\nu_i(x) - \frac{\lambda_i}{t}\big)^2 \bigg\} \\ s.t. \quad  \nu_i \geq 0,\quad i=1,\cdots,m. \tag{2}
$$

---从(2)式第二项很容易看出，假如先求得

$$
x^+
$$
---，必然有

$$
\nu_i^+ = max(c_i(x^+) - \frac{\lambda_i}{t},0)
$$
---上式中取 max 是为了满足

$$
\nu
$$
---非负的约束条件。将其代回 (1) 式，得

$$
x^+ = arg\min_x \quad f(x) + \sum_i \psi(c_i(x),\lambda_i,t)
$$
---其中

$$
\psi(c_i(x),\lambda_i,t)=\left\{
             \begin{array}{lr}
             -\lambda_i c_i(x) + \frac{t}{2}c_i(x)^2 \;\;\;\; 如果 c_i(x) - \lambda_i/t &lt;0&amp;  \\
             -\frac{\lambda_i^2}{2t}, \;\;\;\;\;\;\;otherwise&amp;  
             \end{array}
\right.
$$

---然后更新

$$
\lambda
$$
---:

$$
\lambda^+ = max(\lambda_i - tc_i(x^+),0)
$$


