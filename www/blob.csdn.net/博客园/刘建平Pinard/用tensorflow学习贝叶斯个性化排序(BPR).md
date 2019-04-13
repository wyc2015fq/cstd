
# 用tensorflow学习贝叶斯个性化排序(BPR) - 刘建平Pinard - 博客园






# [用tensorflow学习贝叶斯个性化排序(BPR)](https://www.cnblogs.com/pinard/p/9163481.html)
在[贝叶斯个性化排序(BPR)算法小结](https://www.cnblogs.com/pinard/p/9128682.html)中，我们对贝叶斯个性化排序(Bayesian Personalized Ranking, 以下简称BPR)的原理做了讨论，本文我们将从实践的角度来使用BPR做一个简单的推荐。由于现有主流开源类库都没有BPR，同时它又比较简单，因此用tensorflow自己实现一个简单的BPR的算法，下面我们开始吧。
# 1. BPR算法回顾
BPR算法是基于矩阵分解的排序算法，它的算法训练集是一个个的三元组$<u,i,j>$，表示对用户u来说，商品i的优先级要高于商品j。训练成果是两个分解后的矩阵$W$和$H$,假设有m个用户，n个物品，那么$W$的维度是$m \times k$, $H$的维度是$n \times k$。其中k是一个需要自己定义的较小的维度。对于任意一个用户u，我们可以计算出它对商品i的排序评分为$\overline{x}_{ui} = w_u \bullet h_i$。将u对所有物品的排序评分中找出最大的若干个，就是我们对用户u的真正的推荐集合。
# 2. 基于movieLens 100K做BPR推荐
本文我们基于MovieLens 100K的数据做BPR推荐示例，[数据下载链接在这](http://files.grouplens.org/datasets/movielens/ml-100k.zip)。这个数据集有943个用户对1682部电影的打分。由于BPR是排序算法，因此数据集里的打分会被我们忽略，主要是假设用户看过的电影会比用户满意看的电影的排序评分高。由于tensorflow需要批量梯度下降，因此我们需要自己划分若干批训练集和测试集。
# 3. 算法流程
下面我们开始算法的流程，参考了github上一个较旧的BPR代码[于此](https://github.com/dongx-duan/bpr/blob/master/bpr_mf.ipynb)，有删改和增强。
完整代码参见我的github:https://github.com/ljpzzz/machinelearning/blob/master/classic-machine-learning/bpr.ipynb
首先是载入类库和数据，代码如下：
importnumpyimporttensorflow as tfimportosimportrandomfromcollectionsimportdefaultdictdefload_data(data_path):
    user_ratings=defaultdict(set)
    max_u_id= -1max_i_id= -1with open(data_path,'r') as f:forlineinf.readlines():
            u, i, _, _= line.split("\t")
            u=int(u)
            i=int(i)
            user_ratings[u].add(i)
            max_u_id=max(u, max_u_id)
            max_i_id=max(i, max_i_id)print("max_u_id:", max_u_id)print("max_i_id:", max_i_id)returnmax_u_id, max_i_id, user_ratings
    
data_path= os.path.join('D:\\tmp\\ml-100k','u.data')
user_count, item_count, user_ratings= load_data(data_path)
输出为数据集里面的用户数和电影数。同时，每个用户看过的电影都保存在user_ratings中。
max_u_id: 943
max_i_id: 1682下面我们会对每一个用户u，在user_ratings中随机找到他评分过的一部电影i,保存在user_ratings_test，后面构造训练集和测试集需要用到。
defgenerate_test(user_ratings):
    user_test=dict()foru, i_listinuser_ratings.items():
        user_test[u]= random.sample(user_ratings[u], 1)[0]returnuser_test
user_ratings_test= generate_test(user_ratings)
接着我们需要得到TensorFlow迭代用的若干批训练集，获取训练集的代码如下，主要是根据user_ratings找到若干训练用的三元组$<u,i,j>$，对于随机抽出的用户u，i可以从user_ratings随机抽出，而j也是从总的电影集中随机抽出，当然j必须保证$(u,j)$不出现在user_ratings中。
defgenerate_train_batch(user_ratings, user_ratings_test, item_count, batch_size=512):
    t=[]forbinrange(batch_size):
        u= random.sample(user_ratings.keys(), 1)[0]
        i= random.sample(user_ratings[u], 1)[0]whilei ==user_ratings_test[u]:
            i= random.sample(user_ratings[u], 1)[0]
        
        j= random.randint(1, item_count)whilejinuser_ratings[u]:
            j= random.randint(1, item_count)
        t.append([u, i, j])returnnumpy.asarray(t)
下一步是产生测试集三元组$<u,i,j>$。对于每个用户u，它的评分电影i是我们在user_ratings_test中随机抽取的，它的j是用户u所有没有评分过的电影集合，比如用户u有1000部电影没有评分，那么这里该用户的测试集样本就有1000个。
defgenerate_test_batch(user_ratings, user_ratings_test, item_count):foruinuser_ratings.keys():
        t=[]
        i=user_ratings_test[u]forjinrange(1, item_count+1):ifnot(jinuser_ratings[u]):
                t.append([u, i, j])yieldnumpy.asarray(t)
有了训练集和测试集，下面是用TensorFlow构建BPR算法的数据流，代码如下，其中hidden_dim就是我们矩阵分解的隐含维度k。user_emb_w对应矩阵$W$, item_emb_w对应矩阵$H$。如果大家看过之前写的BPR的算法原理篇，下面的损失函数的构造，相信大家都会很熟悉。
defbpr_mf(user_count, item_count, hidden_dim):
    u=tf.placeholder(tf.int32, [None])
    i=tf.placeholder(tf.int32, [None])
    j=tf.placeholder(tf.int32, [None])
    with tf.device("/cpu:0"):
        user_emb_w= tf.get_variable("user_emb_w", [user_count+1, hidden_dim], 
                            initializer=tf.random_normal_initializer(0, 0.1))
        item_emb_w= tf.get_variable("item_emb_w", [item_count+1, hidden_dim], 
                                initializer=tf.random_normal_initializer(0, 0.1))
        
        u_emb=tf.nn.embedding_lookup(user_emb_w, u)
        i_emb=tf.nn.embedding_lookup(item_emb_w, i)
        j_emb=tf.nn.embedding_lookup(item_emb_w, j)\#MF predict: u_i > u_jx = tf.reduce_sum(tf.multiply(u_emb, (i_emb - j_emb)), 1, keep_dims=True)\#AUC for one user:\#reasonable iff all (u,i,j) pairs are from the same user\#\#average AUC = mean( auc for each user in test set)mf_auc = tf.reduce_mean(tf.to_float(x >0))
    
    l2_norm=tf.add_n([
            tf.reduce_sum(tf.multiply(u_emb, u_emb)), 
            tf.reduce_sum(tf.multiply(i_emb, i_emb)),
            tf.reduce_sum(tf.multiply(j_emb, j_emb))
        ])
    
    regulation_rate= 0.0001bprloss= regulation_rate * l2_norm -tf.reduce_mean(tf.log(tf.sigmoid(x)))
    
    train_op= tf.train.GradientDescentOptimizer(0.01).minimize(bprloss)returnu, i, j, mf_auc, bprloss, train_op
有了算法的数据流图，训练集和测试集也有了，现在我们可以训练模型求解$W,H$这两个矩阵了，注意我们在原理篇是最大化对数后验估计函数， 而这里是最小化取了负号后对应的对数后验估计函数，实际是一样的。代码如下：
with tf.Graph().as_default(), tf.Session() as session:
    u, i, j, mf_auc, bprloss, train_op= bpr_mf(user_count, item_count, 20)
    session.run(tf.initialize_all_variables())forepochinrange(1, 4):
        _batch_bprloss=0forkinrange(1, 5000):\#uniform samples from training setuij =generate_train_batch(user_ratings, user_ratings_test, item_count)
            _bprloss, _train_op=session.run([bprloss, train_op], 
                                feed_dict={u:uij[:,0], i:uij[:,1], j:uij[:,2]})
            _batch_bprloss+=_bprlossprint("epoch:", epoch)print("bpr_loss:", _batch_bprloss /k)print("_train_op")
        user_count=0
        _auc_sum= 0.0\#each batch will return only one user's aucfort_uijingenerate_test_batch(user_ratings, user_ratings_test, item_count):
            _auc, _test_bprloss=session.run([mf_auc, bprloss],
                                    feed_dict={u:t_uij[:,0], i:t_uij[:,1], j:t_uij[:,2]}
                                )
            user_count+= 1_auc_sum+=_aucprint("test_loss:", _test_bprloss,"test_auc:", _auc_sum/user_count)print("")
    variable_names= [v.nameforvintf.trainable_variables()]
    values=session.run(variable_names)fork,vinzip(variable_names, values):print("Variable:", k)print("Shape:", v.shape)print(v)
这里我k取了20， 迭代次数3， 主要是为了快速输出结果。如果要做一个较好的BPR算法，需要对k值进行选择迭代，并且迭代次数也要更多一些。这里我的输出如下，供参考。
epoch:  1
bpr_loss:  0.7236263042427249
_train_op
test_loss:  0.76150036 test_auc:  0.4852939894020929
epoch:  2
bpr_loss:  0.7229681559433149
_train_op
test_loss:  0.76061743 test_auc:  0.48528061393838007
epoch:  3
bpr_loss:  0.7223725006756341
_train_op
test_loss:  0.7597519 test_auc:  0.4852617720521252
Variable:  user_emb_w:0
Shape:  (944, 20)
[[ 0.08105529  0.04270628 -0.12196594 ...  0.02729403  0.1556453
  -0.07148876]
 [ 0.0729574   0.01720054 -0.08198593 ...  0.05565814 -0.0372898
   0.11935959]
 [ 0.03591165 -0.11786834  0.04123168 ...  0.06533947  0.11889934
  -0.19697346]
 ...
 [-0.05796075 -0.00695129  0.07784595 ... -0.03869986  0.10723818
   0.01293885]
 [ 0.13237114 -0.07055715 -0.05505611 ...  0.16433473  0.04535925
   0.0701588 ]
 [-0.2069717   0.04607181  0.07822093 ...  0.03704183  0.07326393
   0.06110878]]
Variable:  item_emb_w:0
Shape:  (1683, 20)
[[ 0.09130769 -0.16516572  0.06490657 ...  0.03657753 -0.02265425
   0.1437734 ]
 [ 0.02463264  0.13691436 -0.01713235 ...  0.02811887  0.00262074
   0.08854961]
 [ 0.00643777  0.02678963  0.04300125 ...  0.03529688 -0.11161
   0.11927075]
 ...
 [ 0.05260892 -0.03204868 -0.06910443 ...  0.03732759 -0.03459863
  -0.05798787]
 [-0.07953933 -0.10924194  0.11368059 ...  0.06346208 -0.03269136
  -0.03078123]
 [ 0.03460099 -0.10591184 -0.1008586  ... -0.07162578  0.00252131
   0.06791534]]现在我们已经得到了$W,H$矩阵，就可以对任意一个用户u的评分排序了。注意输出的$W,H$矩阵分别在values[0]和values[1]中。
那么我们如何才能对某个用户推荐呢？这里我们以第一个用户为例，它在$W$中对应的$w_0$向量为value[0][0]，那么我们很容易求出这个用户对所有电影的预测评分, 代码如下：
session1 = tf.Session()
u1_dim = tf.expand_dims(values[0][0], 0)
u1_all = tf.matmul(u1_dim, values[1],transpose_b=True)
result_1 = session1.run(u1_all)
print (result_1)
输出为一个评分向量：
[[-0.01707731  0.06217583 -0.01760234 ...  0.067231    0.08989487
  -0.05628442]]现在给第一个用户推荐5部电影，代码如下：
print("以下是给用户0的推荐：")
p=numpy.squeeze(result_1)
p[numpy.argsort(p)[:-5]] =0forindexinrange(len(p)):ifp[index] !=0:print(index, p[index])
输出如下:
以下是给用户0的推荐：
54 0.1907271
77 0.17746378
828 0.17181025
1043 0.16989286
1113 0.17458326
# 4. 小结
以上就是用tensorflow来构建BPR算法模型，并用该算法模型做movieLens 100K推荐的过程。实际做产品项目中，如果要用到BPR算法，一是要注意对隐藏维度k的调参，另外尽量多迭代一些轮数。
另外我们可以在BPR损失函数那一块做文章。比如我们可以对$\overline{x}_{uij} = \overline{x}_{ui} - \overline{x}_{uj}$这个式子做改进，加上一个基于评分时间的衰减系数，这样我们的排序推荐还可以考虑时间等其他因素。
以上就是用tensorflow学习BPR的全部内容。
（欢迎转载，转载请注明出处。欢迎沟通交流： liujianping-ok@163.com）





