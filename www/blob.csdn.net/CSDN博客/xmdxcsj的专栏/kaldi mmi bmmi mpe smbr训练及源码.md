# kaldi mmi/bmmi/mpe/smbr训练及源码 - xmdxcsj的专栏 - CSDN博客





2017年01月23日 19:53:24[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2655








## 训练流程
- make_denlats.sh 
- 产生1gram语言模型对应的解码网络
- nnet-latgen-faster产生denominator lattice，作为smbr训练的分母，充当竞争路径的作用

- align.sh 
- 根据标注的reference作为G构建WFST
- 解码获得最优路径，产生每一帧对齐的最好结果numerator alignment，作为smbr训练的分子，充当标准路径的作用

- get_egs_discriminative2.sh 

重新组织数据格式，将上面两步产生的numerator alignment、denominator lattice和特征文件input features组织成以下形式的数据结构（必要时进行split和excise操作）：

```cpp
struct DiscriminativeNnetExample {
  /// The weight we assign to this example;
  /// this will typically be one, but we include it
  /// for the sake of generality.  
  BaseFloat weight; 

  /// The numerator alignment
  std::vector<int32> num_ali; 

  /// The denominator lattice.  Note: any acoustic
  /// likelihoods in the denominator lattice will be
  /// recomputed at the time we train.
  CompactLattice den_lat; 

  /// The input data-- typically with a number of frames [NumRows()] larger than
  /// labels.size(), because it includes features to the left and right as
  /// needed for the temporal context of the network.  (see also the
  /// left_context variable).
  /// Caution: when we write this to disk, we do so as CompressedMatrix.
  /// Because we do various manipulations on these things in memory, such
  /// as splitting, we don't want it to be a CompressedMatrix in memory
  /// as this would be wasteful in time and also would lead to further loss of
  /// accuracy.
  Matrix<BaseFloat> input_frames;

  /// The number of frames of left context in the features (we can work out the
  /// #frames of right context from input_frames.NumRows(), num_ali.size(), and
  /// this).
  int32 left_context;


  /// spk_info contains any component of the features that varies slowly or not
  /// at all with time (and hence, we would lose little by averaging it over
  /// time and storing the average).  We'll append this to each of the input
  /// features, if used.
  Vector<BaseFloat> spk_info;
}
```
- train_discriminative2.sh 
- nnet-combine-egs-discriminative：重新组织egs，512帧组成1个eg，对应于一个batch
- 开始区分度训练，最外层的循环控制使用的iter个数，也就是把epoch统一转化为iter的个数 

num_archive=5000 （比如degs产生的文件个数） 

num_jobs_nnet=4 

num_epochs=4 

num_iters=num_epochs*num_archive/num_jobs_nnet=5000
## kaldi源码

### 数据部分

数据部分egs_包括 

- num_ali 

维度：帧数 

align后的序列，作为reference 

- den_lat 

对应的lattice 

- input_frames 

维度：(left_context+帧数+right_context)*frame_dim 

语音特征
模型部分包括： 

- am_nnet 

用来计算P(s|o)，以及训练更新
### 训练流程
- Propagate() 

计算特征对应的后验概率P(s|o)
- LatticeComputation() 

计算准则对应的loss 
- 使用上一步计算出的P(s|o)，除以先验概率P(s)，得到似然概率P(o|s)，替换lattice边上对应的P(o|s)
- 根据不同的准则计算对应的$post=\frac{\partial J}{\partial logP(o_t|s)}$

<1> LatticeForwardBackwardMpeVariants 

 计算$post=\frac{\partial J}{\partial logP(o_t|s)}=r_q(c(q)-c_{arg}^r)$
$r_q$: 表示边q的似然概率，对应于$\frac{\alpha_q \beta_q}{\sum_r \alpha_r \beta_r}$
$c(q)$: 表示经过边q的所有句子的平均state准确率，对应于alpha_smbr[q]+beta_smbr[q] 
$c_{arg}^r$: 所有句子的平均state准确率，对应于tot_forward_score 

<2>LatticeForwardBackwardMmi 

  LatticeForwardBackward：对应于第二项$\frac{\sum_{w:s_t=i} p(o^m|s^m)^k P(w)}{\sum_w p(o^m|s^m)^k P(w)})$

  AlignmentToPosterior：对应于第一项$\delta(i=s_t^m)$- CompObjfAndDeriv 

计算对于后验概率的导数，满足如下关系 


$\frac{\partial J}{\partial P(s|o_t)}=\frac{\partial J}{\partial logP(o_t|s)} \frac{\partial logP(o_t|s)}{\partial P(s|o_t)}=post \frac{1}{P(s|o_t)}$- Backprop() 

逐层进行反向传播

### 公式推导

详细公式推导参考[这片博客](http://blog.csdn.net/xmdxcsj/article/details/52760111)

smbr训练实际用到的前后向算法，参考[1]的“Computation for approximate MPE”部分的伪代码，其中$\alpha$和$\beta$比较好理解，但是为了$A(s,s^m)$的计算引入了$\alpha'$和$\beta'$不容易理解，称之为到达边的不完整序列的state平均准确率。

### 其他点
- lattice对应的边上面有两个weight 
- graph cost: LM+transition+pronunciation
- acoustic cost: -P(o|s)

- mpe和smbr的主要体现在$A(s,s^m)$的计算上 
- mpe: 每一帧对应的phone和align是否一致
- smbr: 每一帧对应的state和align是否一致

- one_silience_class参数含义体现在对于$A(s,s^m)$的认定上面 
- true: pdf=ref_pdf或者pdf和ref_pdf都属于silience的话，都认为相等
- false: pdf=ref_pdf并且pdf不是silience的话，才认为相等

- drop_frames 

根据[2]: mmi训练当分子alignment的状态没有在分母的lattice出现的时候，导致梯度过大，舍弃该帧的梯度。这种情况大部分发生在帧数低于50帧的语音，可以使用rejected frame（drop_frames）来解决这种问题。导致这种问题的原因包括 
- search error
- poor match of the acoustics to the model
- errors in the reference

## 经验设置

根据[1]的总结： 

- lattice的beam设定较小的话影响准确率(MPE相比于MMI受影响更小)，而且迭代过程中重新产生lattice的收益很小，而且十分耗时 

- 使用1gram的语言模型 

- am scale跟解码保持一致即可，有时候更小（比如原来的1/4）会有收益
smbr只有在大数据量才会有效果

## 参考文献

[1]. [discriminative training for large vocabulary speech recognition](http://www.danielpovey.com/files/phd_2003.pdf)

[2]. sequence-discriminative training of deep neural networks












