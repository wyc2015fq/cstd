# kaldi data preparation - xmdxcsj的专栏 - CSDN博客





2017年02月25日 17:35:09[xmucas](https://me.csdn.net/xmdxcsj)阅读数：567








主要两个文件夹data/train和data/lang

## train

需要手动创建三个文件 

- utt2spk 

- text 

- wav.scp
以上文件需要提前按照C++方式排序

```bash
export LC_ALL=C
```

然后可以调用steps下的脚本抽特征

```
steps/make_mfcc.sh --nj 20 --cmd "$train_cmd" data/train exp/make_mfcc/train $mfccdir
steps/compute_cmvn_stats.sh data/train exp/make_mfcc/train $mfccdir
```

得到两个文件： 

- train/feats.scp 

- train/cmvn.scp
## lang

需要提前准备的文件data/local/dict： 

- extra_questions.txt 

- lexicon.txt  

- nonsilence_phones.txt  

- optional_silence.txt  

- silence_phones.txt
运行一下脚本生成data/lang
`utils/prepare_lang.sh data/local/dict "<UNK>" data/local/lang data/lang`





