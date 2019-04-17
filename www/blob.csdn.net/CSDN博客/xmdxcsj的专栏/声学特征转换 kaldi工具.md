# 声学特征转换 kaldi工具 - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:32:32[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1577








## 基本工具

1.transform-feats

```xml
transform-feats <transform> <input-feats> <output-feats>
```

其中transform是对应的特征转化矩阵，如果transform是rxfilename形式，认为是speaker-independent；如果transform是rspecifier形式，有-utt2spk选项，认为是per-speaker，没有-utt2spk选项，认为是per-utterance。 

- speaker-independent: LDA MLLT/STC 

- per-speaker or per-utterance: fMLLR
2.compose-transform 

将多个transform合并为一个 

3.weight-silence-post 

在说话人相关的特征变换中，需要删除silence对应的帧
## 说话人无关特征

### lda

原始mfcc特征40维，左右3帧，扩展成280维，然后经过转换矩阵（40*280）将特征降维到40维，转换矩阵即为LDA 

- acc-lda获取LDA的统计量 

- est-lda根据统计量进行LDA估计
### frame splicing
- splice-feats左右拼帧

### delta feature
- add-deltas 增加差分

### HLDA
- gmm-acc-hlda
- gmm-est-hlda

### STC／MLLT
- gmm-acc-mllt
- est-mllt

## 说话人相关特征

### CMLLR／fMLLR

Constrained Maximum Likelihood Linear Regression (CMLLR), also known as feature-space MLLR (fMLLR) 

fmllr变换得到**trans.JOB**文件，每个speaker对应一个变换矩阵。相当于去除speaker的信息 

- gmm-est-fmllr
### linear VTLN

没有收益

### ET

另外一种VTLN变换的方法

### cmvn

compute-cmvn-stats 如果提供spk2utt，以speaker为单位；如果没有，以utterance为单位 

apply-cmvn 同样如果提供utt2spk，以speaker为单位；否则，以utterance为单位

## 特征变换相关的recipe

### train_delta

train_delta.sh：使用mfcc+cmvn+delta特征

### train_lda_mllt

train_lda_mllt.sh：使用mfcc+cmvn+splice+LDA特征，获得final.mat（LDA和MLLT变换合在一起） 

主要是特征做LDA/MLLT变换，转换以后的特征重新训练GMM模型，具体步骤： 

- 估计出LDA变换，$M$，特征经过LDA转换 

- 转换后的特征重新训练GMM，在指定的迭代更新MLLT矩阵 

    - 计算MLLT统计量 

    - 更新MLLT矩阵$T$

    - 更新模型的均值$\mu_{jm} \leftarrow T\mu_{jm}$

    - 更新转换矩阵$M\leftarrow TM$
### train_sat

train_sat.sh：使用mfcc+cmvn+splice+LDA+fmllr特征，获得trans.JOB 

主要是特征做fmllr变换，转换以后的特征重新训练GMM模型

### train

dnn训练：将mfcc（没有cmvn和lda）特征和ivector特征拼在一起








