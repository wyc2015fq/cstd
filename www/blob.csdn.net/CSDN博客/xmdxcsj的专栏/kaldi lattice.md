# kaldi lattice - xmdxcsj的专栏 - CSDN博客





2017年01月23日 19:52:26[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3548








## 概况

两种lattice结构

### Lattice结构

FST的形式，weight包括两部分（graph cost和acoustic cost），输入是transition-ids，输出是words。 

其中weight的graph cost包含LM+transition+pronunciation三部分。

### CompactLattice结构

和lattice相似，区别在于它是接收机，输入和输出一样（都是words），weight包含两部分（权重和transition-ids），相比于Lattice，CompactLattice把输入的transition-ids转移到weight上面。

lattice保证每一个word-sequence只对应lattice中的一条路径。

### Lattice实现

假设a表示graph cost，b表示acoustic cost，那么 

LatticeWeight表示为(a,b) 

LexicographicWeight表示为(a+b,a-b)
lattice相关的算法在Lattice上面更为高效，因为CompactLattice的weight包含有transition-ids，当take best path的时候就涉及到transition-ids的拼接操作。

lattice写入文件时保持acoustic cost是unscaled。可以从archive中读取Lattice，即使里面包含有CompactLattice。

可以使用ConvertLattice()函数将Lattice转化为CompactLattice。lattice转化：

```cpp
Lattice lat;
  // initialize lat.
  CompactLattice compact_lat;
  ConvertLattice(lat, &compact_lat);
```

使用OpenFst算法

```cpp
Lattice lat;
  // initialize lat.
  Lattice best_path;
  fst::ShortestPath(lat, &best_path);
```

### CompactLattice实现

weight不仅包含cost，还有transition-id，对于multiplication操作，transition-id进行append，对于adding操作，cost占优的transition-id作为结果。

## lattice generation

对应的类 LatticeSimpleDecoder，流程包括： 

- 产生state级别的lattice 

- 使用lattice-delta对lattice剪枝 

- 使用特殊的确定化算法对每一个word sequence只保留最优路径
## lattice operation

```
lattice-prune --acoustic-scale=0.1 --beam=5 ark:in.lats ark:out.lats
lattice-best-path --acoustic-scale=0.1 ark:in.lats ark:out.tra ark:out.ali
lattice-nbest --n=10 --acoustic-scale=0.1 ark:in.lats ark:out.nbest
#LM rescore
lattice-lmrescore --lm-scale=-1.0 ark:in.lats G_old.fst ark:nolm.lats
lattice-lmrescore --lm-scale=1.0 ark:nolm.lats G_new.fst ark:out.lats
#probability scaling
lattice-scale --acoustic-scale=0.1 --lm-scale=0.8 ark:in.lats ark:out.lats
#用在MMI的区分度训练MMI，保证正确的transcription出现在分母
lattice-union ark:num_lats.ark ark:den_lats.ark ark:augmented_den_lats.ark
lattice-interp --alpha=0.4 ark:1.lats ark:2.lats ark:3.lats
lattice-to-phones final.mdl ark:1.lats ark:phones.lats
lattice-project ark:1.lats ark:- | lattice-compose ark:- ark:2.lats ark:3.lats
lattice-equivalent ark:1.lats ark:2.lats || echo "Not equivalent!"
lattice-rmali ark:in.lats ark:word.lats
#Boosted MMI training
lattice-boost-ali --silence-phones=1:2:3 --b=0.1 final.mdl ark:1.lats \
   ark:1.ali ark:boosted.lats
#前后向计算后验概率
lattice-to-post --acoustic-scale=0.1 ark:1.lats ark:- | \
    gmm-acc-stats 10.mdl "$feats" ark:- 1.acc
lattice-determinize ark:1.lats ark:det.lats
#计算WER
 cat $data/text | \
  sed 's:<NOISE>::g' |  sed 's:<SPOKEN_NOISE>::g'  | \
  scripts/sym2int.pl --ignore-first-field $lang/words.txt | \
  lattice-oracle --word-symbol-table=$lang/words.txt  \
    "ark:gunzip -c $dir/lats.pruned.gz|" ark:- ark,t:$dir/oracle.tra \
    2>$dir/oracle.log
#增加转移概率
lattice-add-trans-probs --transition-scale=1.0 --self-loop-scale=0.1 \
      final.mdl ark:1.lats ark:2.lats
lattice-to-fst --lm-scale=0.0 --acoustic-scale=0.0 ark:1.lats ark:1.words
lattice-copy ark:1.lats ark,t:- | head -50
lattice-to-nbest --acoustic-scale=0.1 --n=10 ark:1.lats ark:1.nbest
nbest-to-linear ark:1.nbest ark:1.ali ark:1.words ark:1.lmscore ark:1.acscore
```

对于lattice来讲，边上的word/transition-id/weight并不是完全的对应关系，同时得到的时间信息也是不准确的. 

对于CompactLattice来说，单个边上的信息意义不明确，只有组合成一条完整的path才有意义。





