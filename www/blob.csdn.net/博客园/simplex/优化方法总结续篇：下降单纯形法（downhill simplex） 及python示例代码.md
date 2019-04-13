
# 优化方法总结续篇：下降单纯形法（downhill simplex） 及python示例代码 - simplex - 博客园|[simplex](https://www.cnblogs.com/simplex/)
|
|[博客园](https://www.cnblogs.com/)|::|[首页](https://www.cnblogs.com/simplex/)|::|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|::|[联系](https://msg.cnblogs.com/send/simplex)|::|[订阅](https://www.cnblogs.com/simplex/rss)![订阅](//www.cnblogs.com/images/xml.gif)|::|[管理](https://i.cnblogs.com/)|17 				Posts ::				0 Stories				::				5 Comments				::				0 Trackbacks|
|

|公告


|下降单纯形法(downhill simplex method)是一个广泛使用的“derivative free”的优化算法。一般来说它的效率不高，但是文献[1]提到|“the downhill simplex method may frequently be the *best* method to use if the figure of merit is “get something working quickly” for a problem whose computational burden is small.”
|单纯形法的基本思路是在\(N\)维空间中，构造一个非退化的初始单纯形，然后做一系列的几何操作，如反射、扩展、收缩等，逐步往极值点移动该单纯形。由于这些几何操作的目的基本上都是让单纯形往极小值移动，所以叫下降单纯形法。
|假设待优化的函数为\(f(\mathbf{x})\)，\(N\)维空间里的单纯形\(Z\)的\(N+1\)个顶点按照函数值从小到大排列分别为\(\mathbf{x}_{0},\mathbf{x}_{2},\cdots,\mathbf{x}_{N}\)，定义
|\(\bar{\mathbf{x}}=\sum_{i=0}^{N-1}\mathbf{x}_{i}\)
|为\(Z\)中除了顶点\(\mathbf{x}_{N}\)之外其余顶点的中心点。
|连接\(\bar{\mathbf{x}}\)和\(\mathbf{x}_{N}\)的直线公式可以写成：
|\(\bar{\mathbf{x}}(t)=(1-t)\bar{\mathbf{x}}+t\mathbf{x}_{N}\)
|下降单纯形法就是从沿着直线$\bar{\mathbf{x}}(t)$方向的几个特殊步长寻找\(\mathbf{x}_{N}\)的替代点，使该替代点处的函数值比\(\mathbf{x}_{N}\)更小，如果没有找到这种替代点，那么就把除了\(\mathbf{x}_{0}\)点之外的其余点往\(\mathbf{x}_{0}\)靠拢。
|假设是要求函数的极小值，可以把对应函数值越小的点认为越好，越大的点认为越差。下降单纯形法每步迭代过程简述如下：
|首先计算最差点沿着直线$\bar{\mathbf{x}}(t)$关于平均点$\bar{\mathbf{x}}$的对称点
|如果对称点介于|最好|和|次差|点之间，那么就接受它(reflection)；
|如果对称点比|最好|点还好，那么做沿该方向更大胆的尝试，令\(t=-2\)，如果新尝试点比对称点更好则接受新尝试点(expand)，否则接受当前对称点(reflection)；
|如果对称点介于|次差|点和|最差|点之间，那么沿该方向做更小心的尝试，即令\(t=-0.5\)，如果新尝试点比对称点更好则接受新尝试点(outside contraction)
|如果对称点比|最差|点还差，那么沿反方向做尝试，即令\(t=0.5\)，如果新尝试点比对称点更好则接受新尝试点(inside contraction)
|如果4和5均失败，即对称点比|次差|还要差而且outside contraction与inside contraction均失败，那么把|最好|点之外的其他点都朝|最好|点收缩(shrink)
|上述过程如果用区间图表示会更清晰，区间的三个分界点就是|最好|、|次差|和|最差|点。对应的伪代码可以参考文献 [3] 第9.5节。
|上述迭代算法需要提供一个初始单纯形，该单纯形可以参考文献[1]给出的方法得到。首先任选一点\(\mathbf{x}_{0}\)，然后利用以下公式：
|\(\mathbf{x}_{i}=\mathbf{x}_{0}+\lambda\mathbf{e}_{i}\)
|其中\(\mathbf{e}_{i}\)代表\(N\)维空间的单位矢量，\(\lambda\)表示步长。

|另外，在实际应用中还有一个很重要的诀窍，即重启(restart)。因为单纯形在迭代更新的时候很容易就卡在某个中间位置上，这时单纯形的 *最好* 和 *最差* 点几乎相同，单纯形的体积收缩的很小，会大大减慢迭代速度。为了解决这个问题，可以合理设置初始单纯形的大小。更有效的，就是可以在单纯形卡住的时候通过重新初始化单纯形来加快收敛速度。在利用初始化公式的时候，把当前单纯形的 *最好* 点作为\(\mathbf{x}_{0}\)保留下来，这样保证重启就不会影响之前已经计算的结果。

|下面给出了单纯形法求解一个简单问题的python实现，其中待优化函数为\(f(\mathbf{x})=(\mathbf{x}-\mathbf{x}_{0})(\mathbf{x}-\mathbf{x}_{1})\)
|首先定义一些需要用到的函数
|import|numpy as np|from|matplotlib|import|pyplot as plt|import|seaborn as sns|def|vertice_init(vertex_0, step_length):|'''|initialize vertice of the simplex
    using the following formula:
    $xi=x0+step_length*ei$|'''|emat|= np.eye(vertex_0.size) *|step_length
    vertice|=|[vertex_0]|for|ii|in|range(vertex_0.size):
        vertice.append(vertex_0|+|emat[:, ii])|return|vertice|def|f(v):|'''|Evaluation of Function $f$|'''|dim|=|v.size
    v0|= np.ones(dim) * 5|v1|= np.ones(dim) * 3|return|0.5 * np.dot(v - v0, v -|v1)|def|line(t, v1, v2):|return|(1 - t) * v1 + t * v2
|接下来定义算法主函数，注意里面的restart部分：
|def|simplex(f, vertice, maxit=1000, step_length=100, tol=1e-3|):
    vertice_max_list|= []|\#|store the max vertex during each iteration|vertice_min_list = []|\#|store the min vertex during each iteration|for|jj|in|range(maxit):
        y|=|[]|for|ii|in|vertice:
            y.append(f(ii))
        y|=|np.array(y)|\#|only the highest (worst), next-highest, and lowest (best) vertice|\#|are neeed|idx =|np.argsort(y)
        vertice_max_list.append(vertice[idx[|-1|]])
        vertice_min_list.append(vertice[idx[0]])|\#|centroid of the best n vertice|\#|NOTE: the worst vertex should be excluded, but for simplicity we don't do this|v_mean =|np.mean(vertice)|\#|compute the candidate vertex and corresponding function vaule|v_ref = line(-1, v_mean, vertice[idx[-1|]])
        y_ref|=|f(v_ref)|if|y_ref >= y[idx[0]]|and|y_ref < y[idx[-2|]]:|\#|y_0<=y_ref<y_n, reflection (replace v_n+1 with v_ref)|vertice[idx[-1]] =|v_ref|\#|print('reflection1')|elif|y_ref <|y[idx[0]]:|\#|y_ref<y_0, expand|v_ref_e = line(-2, v_mean, vertice[idx[-1|]])
            y_ref_e|=|f(v_ref_e)|if|y_ref_e <|y_ref:
                vertice[idx[|-1]] =|v_ref_e|\#|print('expand')|else|:
                vertice[idx[|-1]] =|v_ref|\#|print('reflection2')|elif|y_ref >= y[idx[-2|]]:|if|y_ref < y[idx[-1|]]:|\#|y_ref<y_{n+1}, outside contraction|v_ref_c = line(-0.5, v_mean, vertice[idx[-1|]])
                y_ref_c|=|f(v_ref_c)|if|y_ref_c <|y_ref:
                    vertice[idx[|-1]] =|v_ref_c|\#|print('outside contraction')|else|:|\#|y_ref>=y_{n+1} inside contraction|v_ref_c = line(0.5, v_mean, vertice[idx[-1|]])
                y_ref_c|=|f(v_ref_c)|if|y_ref_c <|y_ref:
                    vertice[idx[|-1]] =|v_ref_c|\#|print('inside contraction')|continue|\#|shrinkage|for|ii|in|range(1|, len(vertice)):
                    vertice[ii]|= 0.5 * (vertice[0] +|vertice[ii])|print|(|'|shrinkage|'|)
                vertice|=|vertice_init(vertice[idx[0]], step_length)|\#|restart|\#|restarting is very important during iteration, for the simpex|\#|can easily stucked into a nonoptimal point|rtol = 2.0 * abs(y[idx[0]] - y[idx[-1]]) /|(
            abs(y[idx[0]])|+ abs(y[idx[-1]]) + 1e-9|)|if|rtol <=|tol:
            vertice|=|vertice_init(vertice[idx[0]], step_length)|return|vertice_max_list, vertice_min_list
|测试部分，设置未知参数维度为15维，根据未知函数\(f(\mathbf{x})\)的定义易得该函数的最小值为\(-0.5\times 15=-7.5\)。
|dim = 15|step_length|= 5|v|=|np.random.randn(dim)
vertice|= vertice_init(v, step_length)|\#|the chioce of step length is cruical|vertice_max_list, vertice_min_list|=|simplex(
    f, vertice, maxit|=2000, step_length=step_length, tol=1e-5|)|print|(|'|min value is %s|'|% f(vertice_min_list[-1]))
|作图展示优化步骤。把每步迭代中单纯形的最高点和最低点对应函数值画成曲线，从图中可以清晰地看出，算法的确卡在了0以上的位置，而且此时最大值和最小值非常接近，说明单纯形已经收缩的很厉害。而重启方法可以帮助我们摆脱这些陷阱，加快收敛进度。通过几次重启之后，算法很快收敛到了真正的极值点。注意restart并不会影响单纯形的最小值，而是把最大值变的很大（图中红线的跳变）。
|f_max_list =|[]
f_min_list|=|[]|for|ii,jj|in|zip(vertice_max_list,vertice_min_list):
    f_max_list.append(f(ii))
    f_min_list.append(f(jj))
plt.plot(f_max_list,|'|r|'|,linewidth=2,label=|'|max|'|)
plt.plot(f_min_list,|'|b|'|,linewidth=2,label=|'|min|'|)
plt.legend(fontsize|=15|)
plt.show()
![](https://images2015.cnblogs.com/blog/1006242/201704/1006242-20170427230155115-820574691.png)
|最后，用单纯形法训练了一个三层神经网络，并用它来做简单的预测。其中神经网络输入层、隐藏层、输出层的神经元数分别是3、2、1，真实的函数关系为\(y=\sum_{i=1}^{N}x_{i}\)。
|import|numpy as np|from|matplotlib|import|pyplot as plt|class|opt_variables:|def|__init__|(self, input_dim, hidden_dim, output_dim, v):|assert|(input_dim + 1) * hidden_dim +|(
            hidden_dim|+ 1) * output_dim == v.size,|'|dimension mismatch!|'|self.input_dim|=|input_dim
        self.hidden_dim|=|hidden_dim
        self.output_dim|=|output_dim
        self.v|=|v
        self.dim|=|v.size
        w1_ptr|= input_dim *|hidden_dim
        b1_ptr|= hidden_dim +|w1_ptr
        w2_ptr|= hidden_dim * output_dim +|b1_ptr
        b2_ptr|= output_dim +|w2_ptr
        self.ptrs|=|np.array([w1_ptr, b1_ptr, w2_ptr, b2_ptr])|def|__add__|(self, other):|if|type(other)|is|type(self):|assert|self.dim == other.dim,|'|dimension mismatch!|'|return|opt_variables(self.input_dim, self.hidden_dim,
                                 self.output_dim, self.v|+|other.v)|return|opt_variables(self.input_dim, self.hidden_dim, self.output_dim,
                             self.v|+|other)|def|__iadd__|(self, other):|if|type(other)|is|type(self):|assert|self.dim == other.dim,|'|dimension mismatch!|'|self.v|+=|other.v|else|:
            self.v|+=|other|return|self|def|__sub__|(self, other):|if|type(other)|is|type(self):|assert|self.dim == other.dim,|'|dimension mismatch!|'|return|opt_variables(self.input_dim, self.hidden_dim,
                                 self.output_dim, self.v|-|other.v)|return|opt_variables(self.input_dim, self.hidden_dim, self.output_dim,
                             self.v|-|other)|def|__isub__|(self, other):|if|type(other)|is|type(self):|assert|self.dim == other.dim,|'|dimension mismatch!|'|self.v|-=|other.v|else|:
            self.v|-=|other|return|self|def|__mul__|(self, constant):|return|opt_variables(self.input_dim, self.hidden_dim, self.output_dim,
                             self.v|*|constant)|def|__rmul__|(self, constant):|return|opt_variables(self.input_dim, self.hidden_dim, self.output_dim,
                             self.v|*|constant)|def|__imul__|(self, constant):
        self.v|*=|constant|return|self|def|__truediv__|(self, constant):|return|opt_variables(self.input_dim, self.hidden_dim, self.output_dim,
                             self.v|/|constant)|def|__rtruediv__|(self, constant):|return|opt_variables(self.input_dim, self.hidden_dim, self.output_dim,
                             constant|/|self.v)|def|__itruediv__|(self, constant):
        self.v|/=|constant|return|self|def|__str__|(self):|return|'|%s|'|%|self.v|def|vertice_init(vertex_0, step_length):
    emat|= np.eye(vertex_0.dim) *|step_length
    vertice|=|[vertex_0]|for|ii|in|range(vertex_0.dim):
        vertice.append(vertex_0|+|emat[:, ii])|return|vertice|def|sigmoid(x):|return|1 / (1 + np.exp(-|x))|def|f(v, X, y):|assert|v.ptrs.size == 4,|'|dimension mismatch!|'|assert|v.input_dim == X.shape[1],|'|dimension mismatch!|'|w1|=|np.reshape(v.v[:v.ptrs[0]], (v.hidden_dim, v.input_dim))
    b1|= np.reshape(v.v[v.ptrs[0]:v.ptrs[1|]], v.hidden_dim)
    w2|= np.reshape(v.v[v.ptrs[1]:v.ptrs[2|]], (v.output_dim, v.hidden_dim))
    b2|= np.reshape(v.v[v.ptrs[2|]:], v.output_dim)
    loss|= 0.0|for|ii|in|range(X.shape[0]):
        loss|+=|(
            np.dot(w2, sigmoid(np.dot(w1, X[ii, :])|+ b1)) + b2 - y[ii])**2|return|loss[0]|def|pred(v, X):|assert|v.ptrs.size == 4,|'|dimension mismatch!|'|assert|v.input_dim == X.shape[1],|'|dimension mismatch!|'|w1|=|np.reshape(v.v[:v.ptrs[0]], (v.hidden_dim, v.input_dim))
    b1|= np.reshape(v.v[v.ptrs[0]:v.ptrs[1|]], v.hidden_dim)
    w2|= np.reshape(v.v[v.ptrs[1]:v.ptrs[2|]], (v.output_dim, v.hidden_dim))
    b2|= np.reshape(v.v[v.ptrs[2|]:], v.output_dim)
    y_pred|=|[]|for|ii|in|range(X.shape[0]):
        y_pred.append(np.dot(w2, sigmoid(np.dot(w1, X[ii, :])|+ b1)) +|b2)|return|np.array(y_pred)|def|line(t, v1, v2):|return|(1 - t) * v1 + t *|v2|def|simplex(f, X, y_real, vertice, maxit=1000, tol=1e-7, step_length=100|):
    vertice_max_list|=|[]
    vertice_min_list|=|[]|for|jj|in|range(maxit):
        y|=|[]|\#|evaluate the function value|for|ii|in|vertice:
            y.append(f(ii, X, y_real))
        y|=|np.array(y)
        idx|= np.argsort(y)|\#|in descend order|vertice_max_list.append(vertice[idx[-1|]])
        vertice_min_list.append(vertice[idx[0]])
        v_mean|=|np.mean(vertice)
        v_ref|= line(-1, v_mean, vertice[idx[-1|]])
        y_ref|=|f(v_ref, X, y_real)|if|y_ref >= y[idx[0]]|and|y_ref < y[idx[-2|]]:|\#|y_0<=y_ref<y_n, reflection (replace v_n+1 with v_ref)|vertice[idx[-1]] =|v_ref|\#|print('reflection1')|elif|y_ref <|y[idx[0]]:|\#|y_ref<y_0, expand|v_ref_e = line(-2, v_mean, vertice[idx[-1|]])
            y_ref_e|=|f(v_ref_e, X, y_real)|if|y_ref_e <|y_ref:
                vertice[idx[|-1]] =|v_ref_e|\#|print('expand')|else|:
                vertice[idx[|-1]] =|v_ref|\#|print('reflection2')|elif|y_ref >= y[idx[-2|]]:|if|y_ref < y[idx[-1|]]:|\#|y_ref<y_{n+1}, outside contraction|v_ref_c = line(-0.5, v_mean, vertice[idx[-1|]])
                y_ref_c|=|f(v_ref_c, X, y_real)|if|y_ref_c <|y_ref:
                    vertice[idx[|-1]] =|v_ref_c|\#|print('outside contraction')|else|:|\#|y_ref>=y_{n+1} inside contraction|v_ref_c = line(0.5, v_mean, vertice[idx[-1|]])
                y_ref_c|=|f(v_ref_c, X, y_real)|if|y_ref_c <|y_ref:
                    vertice[idx[|-1]] =|v_ref_c|\#|print('inside contraction')|continue|\#|shrinkage|for|ii|in|range(1|, len(vertice)):
                    vertice[ii]|= 0.5 * (vertice[0] +|vertice[ii])|print|(|'|shrinkage|'|)
                vertice|=|vertice_init(vertice[idx[0]], step_length)
        rtol|= 2.0 * abs(y[idx[0]] - y[idx[-1]]) /|(
            abs(y[idx[0]])|+ abs(y[idx[-1]]) + 1e-9|)|if|rtol <=|tol:
            vertice|=|vertice_init(vertice[idx[0]], step_length)|return|vertice_max_list, vertice_min_list|\#|define the 3 layer NN|input_dim = 3|hidden_dim|= 2|output_dim|= 1|total_dim|= (input_dim + 1) * hidden_dim + (hidden_dim + 1) *|output_dim|\#|simplex initialize|v =|opt_variables(input_dim, hidden_dim, output_dim, np.random.rand(total_dim))
step_length|= 3|vertice|= vertice_init(v, step_length)|\#|the chioce of step length is cruical|\#|training data|X = np.random.rand(100, 3|)
y_real|= X.sum(axis=1|)|\#|model training|vertice_max_list, vertice_min_list =|simplex(
    f, X, y_real, vertice, maxit|=200, tol=1e-3, step_length=|step_length)|\#|plot|f_max_list =|[]
f_min_list|=|[]|for|ii, jj|in|zip(vertice_max_list, vertice_min_list):
    f_max_list.append(f(ii, X, y_real))
    f_min_list.append(f(jj, X, y_real))
plt.plot(f_max_list,|'|r|'|, linewidth=2, label=|'|max|'|)
plt.plot(f_min_list,|'|b|'|, linewidth=2, label=|'|min|'|)
plt.legend(fontsize|=15|)
plt.show()|\#|prediction|X_test|= np.random.rand(100, 3|)
y_real_test|= X_test.sum(axis=1|)

y_pred|= pred(vertice_min_list[-1|], X_test)
plt.plot(y_real_test,|'|r|'|, linewidth=2, label=|'|real|'|)
plt.plot(y_pred,|'|b|'|, linewidth=2, label=|'|pred|'|)
plt.legend(fontsize|=15|)
plt.show()
|下面分别展示了20次迭代，100次迭代和200次迭代后的模型预测效果：
![](https://images2015.cnblogs.com/blog/1006242/201704/1006242-20170427224119506-327207295.png)
|20次迭代
![](https://images2015.cnblogs.com/blog/1006242/201704/1006242-20170427224129772-1454213377.png)
|100次迭代
![](https://images2015.cnblogs.com/blog/1006242/201704/1006242-20170427224151897-763274198.png)
|200次迭代





|posted on|2017-04-27 22:36|[simplex](https://www.cnblogs.com/simplex/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6777705)|[收藏](#)


|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
Copyright @
	simplex
Powered by:[.Text](http://scottwater.com/blog)and[ASP.NET](http://asp.net)
Theme by:[.NET Monster](http://www.DotNetMonster.com)
