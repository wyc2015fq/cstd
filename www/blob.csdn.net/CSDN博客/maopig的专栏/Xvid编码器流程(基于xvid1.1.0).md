# Xvid编码器流程(基于xvid1.1.0) - maopig的专栏 - CSDN博客
2011年09月01日 20:28:28[maopig](https://me.csdn.net/maopig)阅读数：2692
                
调用Xvid编码器流程(基于xvid1.1.0)
编者按：本文为Xvid最新源代码V1.1.0版本的学习笔记，内
部资料，仅供学习参考，未经授权，不得转载 
xvid有两种编码方式：single pass和twopass 
single pass模式编码简单，速度也快，但最终效果不如twopass。 
twopass就是视频压制需要经过两次编码，分别为twopass－1st pass（简称1pass）和twopass－2nd pass（简称2pass）
1pass时，编码器会用最高质量编码采集可供第2次运算参考的画面信息，而在2 pass时。编码器会根据第一次压缩获得的信息和用户指定的文件大小，自动分配比特率，使需要高流量的运动画面分配到更多的空间，更高的比特率来保证画面质量。相对的，对于那些不包含太多运动信息的静态画面则用较低的比特率。追求画质的朋友当然会选择这种方式，但运算比single pass更费时。
接下来介绍一些基本概念： 
Q值——量化值，它被用来描述1帧的质量，每帧都有一个Q值，取值范围在1－31之间。Q值越小，画质越好，比特率越大
I－frame——关键帧，常被缩写为IF。关键帧是构成一个帧组的第一帧。IF保留了一个场景的所有信息 
P－frame——未来单项预测帧，缩写为PF，只储存与之前一个已解压画面的差值 
B－frame——双向预测帧，缩写为BF，除了参考之前解压的画面以外，也会参考后一帧的画面信息 
编码流程： 
各变量的设置：创建xvid_enc_frame_t和xvid_enc_stats_t，分别用于传入参数和统计编码结果。
具体过程： 
设置传入图像数据和图像色彩空间 
设置传出的码流 
设置vol的标志 
设置帧的编码类型 
设置量化因子 
设置运动估计算法集合 
设置vop的标志
编码器提供的函数
1，                         xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);
含义：根据cpu的特性使用相应汇编优化的函数 
2， xvid_encore(NULL, XVID_ENC_CREATE, &xvid_enc_create, NULL);
含义：初始化编码器。 
具体过程： 
创建编码器句柄，并根据传入的参数设置各变量的值，并且分配要使用的内存，用于存放重建帧，参考帧（1/2像素精度）。以及各种临时变量。并且做好码率控制的初始化。
3， xvid_encore(enc_handle, XVID_ENC_ENCODE, &xvid_enc_frame, &xvid_enc_stats);
目的：编码一帧 
具体过程： 
{ 
初始化写码流。 
如果有必要，转换色彩空间，并且把原始图像拷贝到有边框的图像空间，但是没有扩展边框。 
将重建帧交换成参考帧 
从帧队列中获取当前帧 
设置Encoder结构体的current结构体的vol_flags，vop_flags，motion_flags，fcode，bcode和quant字段。
调用call_plugins，在里面调用rc_single_before做码率控制的初始化，以及对current结构体的其他变量进一步设置通过帧号或者MEanalysis函数分析来确定编码类型，并且根据用户的设置作修正。
MEanalysis的原理是，如果某个宏块的残差的sad大于该宏块的平均值的偏离，那么使用intra方式，否则使用inter方式，然后对这些宏块进行统计，得到整帧的编码方式。
如果编码类型是I_VOP
{ 
设置Encoder->mbParam->vol_flags 
设置Encoder->mbParam.par 
根据vol_flags设置vop_flags 
调用FrameCodeI以I帧的方式编码 
调用call_plugins，在里面调用rc_single_after，进行码率控制。 
} 
如果编码类型是P_VOP 
{ 
用mbParam.vol_flags固定pEnc->current->vol_flags 
调用FrameCodeP以P帧的方式编码 
调用call_plugins，在里面调用rc_single_after，进行码率控制。 
} 
}// xvid_encore 
编者按：本文为Xvid最新源代码V1.1.0版本的学习笔记，内部资料，仅供学习参考，未经授权，不得转载 
xvid有两种编码方式：single pass和twopass 
single pass模式编码简单，速度也快，但最终效果不如twopass。 
twopass就是视频压制需要经过两次编码，分别为twopass－1st pass（简称1pass）和twopass－2nd pass（简称2pass）
1pass时，编码器会用最高质量编码采集可供第2次运算参考的画面信息，而在2 pass时。编码器会根据第一次压缩获得的信息和用户指定的文件大小，自动分配比特率，使需要高流量的运动画面分配到更多的空间，更高的比特率来保证画面质量。相对的，对于那些不包含太多运动信息的静态画面则用较低的比特率。追求画质的朋友当然会选择这种方式，但运算比single pass更费时。
接下来介绍一些基本概念： 
Q值——量化值，它被用来描述1帧的质量，每帧都有一个Q值，取值范围在1－31之间。Q值越小，画质越好，比特率越大
I－frame——关键帧，常被缩写为IF。关键帧是构成一个帧组的第一帧。IF保留了一个场景的所有信息 
P－frame——未来单项预测帧，缩写为PF，只储存与之前一个已解压画面的差值 
B－frame——双向预测帧，缩写为BF，除了参考之前解压的画面以外，也会参考后一帧的画面信息 
编码流程： 
各变量的设置：创建xvid_enc_frame_t和xvid_enc_stats_t，分别用于传入参数和统计编码结果。
具体过程： 
设置传入图像数据和图像色彩空间 
设置传出的码流 
设置vol的标志 
设置帧的编码类型 
设置量化因子 
设置运动估计算法集合 
设置vop的标志 
编码器提供的函数 
1， xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);
含义：根据cpu的特性使用相应汇编优化的函数 
2， xvid_encore(NULL, XVID_ENC_CREATE, &xvid_enc_create, NULL);
含义：初始化编码器。 
具体过程： 
创建编码器句柄，并根据传入的参数设置各变量的值，并且分配要使用的内存，用于存放重建帧，参考帧（1/2像素精度）。以及各种临时变量。并且做好码率控制的初始化。
3， xvid_encore(enc_handle, XVID_ENC_ENCODE, &xvid_enc_frame, &xvid_enc_stats);
目的：编码一帧 
具体过程： 
{ 
初始化写码流。 
如果有必要，转换色彩空间，并且把原始图像拷贝到有边框的图像空间，但是没有扩展边框。 
将重建帧交换成参考帧 
从帧队列中获取当前帧 
设置Encoder结构体的current结构体的vol_flags，vop_flags，motion_flags，fcode，bcode和quant字段。
调用call_plugins，在里面调用rc_single_before做码率控制的初始化，以及对current结构体的其他变量进一步设置
通过帧号或者MEanalysis函数分析来确定编码类型，并且根据用户的设置作修正。 
MEanalysis的原理是，如果某个宏块的残差的sad大于该宏块的平均值的偏离，那么使用intra方式，否则使用inter方式，然后对这些宏块进行统计，得到整帧的编码方式。
如果编码类型是I_VOP 
{ 
设置Encoder->mbParam->vol_flags 
设置Encoder->mbParam.par 
根据vol_flags设置vop_flags 
调用FrameCodeI以I帧的方式编码 
调用call_plugins，在里面调用rc_single_after，进行码率控制。 
} 
如果编码类型是P_VOP 
{ 
用mbParam.vol_flags固定住pEnc->current->vol_flags 
调用FrameCodeP以P帧的方式编码 
调用call_plugins，在里面调用rc_single_after，进行码率控制。 
} 
}// xvid_encore 
4， static int FrameCodeI(Encoder * pEnc, Bitstream * bs)
目的：将一帧图像编码成一个I帧 
具体过程： 
以XVID_PLG_FRAME参数调用call_plugins，该函数目前的作用是设置dquant，可以在该函数中设置最好质量。 调用SetMacroblockQuants，为每个宏块设置量化因子，所以也可以在这里设置最好质量
调用BitstreamWriteVolHeader，写vol 
调用set_timecodes，设置时间编码。 
调用BitstreamPad，填充bit至字节对齐 
调用BitstreamWriteVopHeader，填写vop头 
依次读取每一个宏块，进行编码 
{ 
调用CodeIntraMB设置编码模式为intra，将所有和运动有关的变量设为0 
调用MBTransQuantIntra进行变换编码 
{ 
调用MBTrans8to16将像素的表示方法从8bit扩大到16bit 
调用MBfDCT对像素进行变换编码 
调用MBQuantIntra对dct系数进行intra方式的量化 
调用MBDeQuantIntra对dct系数进行intra方式的反量化 
调用MBiDCT将恢复的dct系数进行反变换 
调用MBTrans16to8将恢复的16bit像素饱和到8bit，组成重建宏块 
}//MBTransQuantIntra 
调用MBPrediction作acdc预测 
{ 
调用get_dc_scaler函数得到量化系数 
调用predict_acdc得到预测方向以及在该预测方向上的和当前块的同一量化水平的预测值 
调用calc_acdc_bits以确定是只使用DC预测，还是DCAC预测。原理是分别作DC预测和DCAC预测，分别计算在这2种情况下需要的码流长度，以确定哪种方式更节约码流。
调用CodeCoeffIntra_CalcBits，用于确定各种方式下的码流长度 
根据预测模式的不同，恢复成相应的系数 
最后计算该宏块的cbp 
}//MBPrediction 
调用MBCoding将宏块编制成码流 
{ 
调用CodeBlockIntra将intra宏块编制成码流 
{ 
编码mcbpc 
编码ac预测标记 
编码cbpy 
对于6个块里的每个块 
首先编码DC系数 
调用CodeCoeffIntra对剩下的63个系数进行编码 
}//CodeBlockIntra 
}//MBCoding 
}//依次读取每一个宏块，进行编码 填充bit，直到字节对齐 
5， static int FrameCodeP(Encoder * pEnc, Bitstream * bs)
含义：将一帧图片编码成P帧具体过程： 
{ 
如果参考帧还没有设置边框，那么就调用image_setedges设置边框 
如果需要半像素运动估计，那么就调用image_interpolate进行插值 
将一帧填充边框后的参考帧，分成8*8的小块，对于每个小块进行插值，如下： 
调用interpolate8x8_halfpel_h进行水平插值 
调用interpolate8x8_halfpel_v进行垂直插值 
调用interpolate8x8_halfpel_hv进行对角线插值 
用参数XVID_PLG_FRAME调用call_plugins，该函数目前的作用是设置dquant，可以在该函数中设置最好质量。
调用SetMacroblockQuants，为每个宏块设置量化因子，所以也可以在这里设置最好质量 
调用MotionEstimation做运动估计 
{ 
使用MotionFlags变量保存要使用的运动算法集合 
使用skip_thresh保存要达到skip模式的阀值 
使用Data保存运动估计要用到的相应变量 
对于每个宏块，依次执行如下操作 
{ 
调用sad16v计算本宏块与参考帧对应位置宏块的亮度的残差，将其保存在pMB->sad16中，并按照4个块的方式分别存放pMB->sad8[0-3]中 用sad00记录最大亮度块残差的4倍
如果还需要考虑色差块的因素 
调用sad8两次，分别计算u分量和v分量的残差，都加入pMB->sad16中，并且也加入sad00中 
如果该宏块的量化差值为0，并且sad00又没有超过skip模式的阀值 
如果已经考虑了色差因素，或者使用xvid_me_SkipDecisionP确认符合skip模式。 
调用ZeroMacroblockP将其编码为skip模式，并置标记pMB->mode = MODE_NOT_CODED
根据采用的运动估计算法不同，做相应的设置 
调用SearchP做该宏块的运动估计 
{ 
确定是否使用inter4v模式，并记录之 
调用get_range确定运动搜索的范围，并记录在Data中 
调用get_pmvdata2，以获得左，上，右上的运动向量，以及它们对应的sad，存入pmv[1-3]和Data->temp[1-3]。然后计算它们的中值，并且存放于pmv[0]，并且把最小的sad存放于Data->temp[0]
设置Data的当前宏块的yuv字段。设置Data->RefP[0-5]为参考帧的同一宏块的整像素y，水平半象素y，垂直半象素y，对角线y，u，v。
设置Data->lambda16和Data->lambda8，其含义可能是运动向量对带宽的占用折合到sad的值
设置qpel和方向 
如果采用qpel，调用get_qpmv2计算用qple方式下的估计中值，存入ata->predMV；否则，Data->predMV为0。
调用d_mv_bits计算mv需要的编码bit，用于修正pMB->sad16和pMB->sad8[0]，并将Data->iMinSAD[0-4]设置为pMB->sad16和pMB->sad8[0-3]，也就是0向量对应的各SAD。
如果不采用率失真决策模型，并且不是当前帧的第一宏块，那么使用一种方法设置阀值threshA，否则阀值threshA为512。
调用PreparePredictionsP，对pmv作进一步的设置，做运算前的准备。 
{ 
设置pmv[0]为0向量 
设置pmv[1]为中值向量的偶数值 
设置pmv[2]为参考帧相同位置宏块的第0块运动向量的偶数值 
如果该宏块有左边宏块，设置pmv[3]为左边宏块的第1块的运动向量的偶数值，否则为0 
如果该宏块有上面宏块，设置pmv[4]为上面宏块的第2块的运动向量的偶数值，否则为0 
如果该宏块有右上宏块，设置pmv[5]为右上宏块的第2块的运动向量的偶数值，否则为0 
如果该宏块有右下宏块，设置pmv[6]为参考帧的相同宏块的右下宏块的第0块的运动向量的偶数值，否则为0。 
}//PreparePredictionsP 
如果使用inter4v，设置CheckCandidate为CheckCandidate16，否则设置为CheckCandidate16no4v
逐一检查mpv[1-6]这六个最可能运动向量，如果发现他们与以前的运动不同，就调用CheckCandidate做运动估计，过程如下：
{ 
检查要做运动估计的运动向量是否越界 
通过该运动向量获得所指向数据块的指针 
调用sad16v，记录下4个8*8块的SAD值，存入data->temp[0-3]中，并将他们的和存入临时变量sad中。 对sad和data->temp[0]做基于运动向量的修正。
如果要考虑色差因素，调用xvid_me_ChromaSAD计算额外的SAD，累加至sad中。 
如果sad小于data->iMinSAD[0]，那么设置data->iMinSAD[0]，data->currentMV[0]，和data->dir。注意，此时的data->dir记录的不是钻石搜索的方向，而是当前向量是pmv数组的第几个元素。
逐一检查data->temp[0-3]，如果他们小于data->iMinSAD[1-4]，那么修改data->iMinSAD[1-4]和data->currentMV[1-4]
}//CheckCandidate 
如果当前最优运动向量，即Data->iMinSAD[0]，小于threshA？或者当前最优运动向量等于参考帧相同位置宏块的运动向量，并且对应的SAD值又比他的小？
就不再做inter4v的搜索 
否则，就做inter4v的搜索 
{ 
使用make_mask逐一检查存放于pmv的所有运动向量，察看是否位于欲搜索的钻石形的顶点。如果是，则在mask变量中标记之。
根据MotionFlags确定使用的搜索函数，根据当前设置，MainSearchPtr = xvid_me_AdvDiamondSearch
调用xvid_me_AdvDiamondSearch进行搜索，过程如下： 
{ 
bDirection既表明了上次尝试的方向，又表明本次可以尝试的方向 
x，y为钻石搜索的位置的中心点坐标 
for(;;) 
{ 
如果可以尝试左边，那么调用CheckCandidate尝试左边 
如果可以尝试右边，那么调用CheckCandidate尝试右边 
如果可以尝试上边，那么调用CheckCandidate尝试上边 
如果可以尝试下边，那么调用CheckCandidate尝试下边 
如果有更好的方向 
{ 
bDirection = 更好的方向 
如果更好的方向是左右方向，那么测试该位置的上下方向 
否则，那么测试该位置的左右方向 
如果这次又找到了更好的方向 
将更好的方向累加到bDirection 
将更好的位置存入x，y 
} 
否则 
{ 
根据去搜索临近未搜索的点，具体规则如下： 
如果bDirection = = 2，表明搜索方向是趋向右边的，那么搜索当前中心点的右上点和右下点。 
如果bDirection = = 1，表明搜索方向是趋向左边的，那么搜索当前中心点的左上点和左下点。 
如果bDirection = = 2+4，表明搜索方向是趋向右上的，那么再搜索当前中心点的左上点，右上点和右下点。
如果bDirection = = 4，表明搜索方向是趋向上边的，那么搜索当前中心点的左上点和右上点。 
如果bDirection = = 8，表明搜索方向是趋向下边的，那么搜索当前中心点的左下点和右下点。 
如果bDirection = = 1+4，表明搜索方向是趋向左上的，那么再搜索当前中心点的左下点，左上点和右上点。
如果bDirection = = 2+8，表明搜索方向是趋向右下的，那么再搜索当前中心点的左下点，左上点和右上点。
如果bDirection = = 1+8，表明搜索方向是趋向左下的，那么再搜索当前中心点的左上点，左下点和右下点。
否则的话，则认为本轮搜索没有找到更好的点，那么再搜索当前中心点的左上点，左下点，右上点，右下点。 
} 
如果没有找到更好的方向，从函数中返回 
更新bDirection为更好的方向 
更新x，y为更好的位置 
}//for(;;) 
}//xvid_me_AdvDiamondSearch 
如果运动估计算法使用了XVID_ME_EXTSEARCH16，那么 
{ 
设置startMV = Data->predMV 
设置backupMV为当前最佳运动向量 
如果startMV和backupMV不相等 
{ 
调用CheckCandidate计算位置为startMV的SAD 
调用xvid_me_DiamondSearch做以startMV为起点的搜索，过程如下： 
{ 
for(;;) 
{ 
如果可以尝试左边，那么调用CheckCandidate尝试左边 
如果可以尝试右边，那么调用CheckCandidate尝试右边 
如果可以尝试上边，那么调用CheckCandidate尝试上边 
如果可以尝试下边，那么调用CheckCandidate尝试下边 
如果没有更好的方向，退出 
bDirection = 更好的方向 
x，y = 更好的位置 
如果更好的方向是左右方向，那么测试该位置的上下方向 
否则，那么测试该位置的左右方向 
如果这次又找到了更好的方向 
{ 
bDirection += 更好的方向 
x，y = 更好的位置 
} 
} 
}//xvid_me_DiamondSearch 
将这次搜索结果和上次搜索结果比较，记录最佳的SAD和位置。 
}//如果startMV和backupMV不相等 
设置startMV = {1，1} 
设置backupMV为当前最佳运动向量 
如果startMV和backupMV不相等 
{ 
调用CheckCandidate计算位置为startMV的SAD 
调用xvid_me_DiamondSearch做以startMV为起点的搜索，过程如下： 
将这次搜索结果和上次搜索结果比较，记录最佳的SAD和位置。 
} 
}//如果运动估计算法使用了XVID_ME_EXTSEARCH16 
}//否则，就做inter4v的搜索 
如果没有采用1/4像素运动估计算法 
{ 
如果采用了XVID_ME_HALFPELREFINE16算法 
调用xvid_me_SubpelRefine 
按顺时针方向8次调用CheckCandidate16，得到最好的1/2像素位置 
} 
否则 
略 
如果当前SAD足够小，那么inter4v = 0 
如果采用inter4v 
{ 
4次调用Search8来搜索4个8*8块的最佳运动向量，每一次搜索的规则如下： 
{ 
如果采用1/4像素运动估计，略。否则 
调用get_pmv2取得本块的中值 
计算第一块以外快的d_mv_bits 
用Data->lambda8修正该块当前的SAD，但是第0块是不用修正的。 
如果使用了XVID_ME_EXTSEARCH8 | XVID_ME_HALFPELREFINE8 | XVID_ME_QUARTERPELREFINE8，那么
{ 
Data->RefP[0-3] = 参考帧的整像素，水平半象素，垂直半象素，对角线半象素的对应宏块的对应块的起始地址。
Data->Cur = 当前帧的当前宏块的当前块的起始地址 
利用get_range得到运动搜索的范围 
根据MotionFlags的指示，设定运动估计MainSearchPtr的算法，当前设置为MainSearchPtr = xvid_me_AdvDiamondSearch。
调用xvid_me_AdvDiamondSearch做运动估计，其中做SAD的函数是CheckCandidate8，该函数类似于CheckCandidate16
如果不采用1/4像素运动估计，并且又采用了XVID_ME_HALFPELREFINE8，那么调用xvid_me_SubpelRefine
按顺时针方向8次调用CheckCandidate8，得到最好的1/2像素位置 
如果采用了1/4像素运动估计，略 
}// XVID_ME_EXTSEARCH8 | XVID_ME_HALFPELREFINE8 | XVID_ME_QUARTERPELREFINE8
如果采用1/4运动估计 
略 
否则 
记录pMB->pmvs[block] = 当前找到的最佳位置与预测位置的差值 
将这次的搜索存入相应OldData的字段，以及pMB的相应字段 
}// Search8 
如果考虑色差的因素，并且又不考虑率失真算法 ＝
{ 
根据是否采用1/4像素运动估计算出色差的运动向量 
计算u，v的SAD，将其作为Data->iMinSAD[1]的修正 
} 
} //如果采用inter4v 
否则，Data->iMinSAD[1]为足够大的值 
}//SearchP 
调用ModeDecision_SAD确定该宏块的类型 
判断该宏块要采取的编码方式，MODE_INTER，MODE_INTER4V，MODE_NOT_CODED，MODE_INTRA
调用motionStatsPVOP做一些统计工作 
具体过程略 
}//对于每个宏块，依次执行如下操作 
做一些最后的设置 
}//MotionEstimation 
调用set_timecodes设置时间戳 
调用BitstreamWriteVopHeader写VOP头 
具体过程略 
对于每一个宏块，依次执行如下操作 
{ 
如果该宏块的编码模式是MODE_INTRA或者MODE_INTRA_Q 
{ 
调用CodeIntraMB设置编码模式为intra，将所有和运动有关的变量设为0 
调用MBTransQuantIntra进行变换编码 
调用MBCoding将该宏块编制成码流 
Continue 
} 
调用MBMotionCompensation做运动补偿 
{ 
如果编码模式是MODE_NOT_CODED 
用参考帧的相应宏块替代当前帧的当前宏块 
Return 
如果编码模式是MODE_NOT_CODED或者MODE_INTER或者MODE_INTER_Q 
{ 
如果mb->mcsel不为0 
做GMC的处理 
Return 
计算运动向量dx，dy 
调用compensate16x16_interpolate进行运动补偿 
{ 
如果采用1/4像素运动估计 
略 
否则，调用get_ref计算用于运动补偿的参考宏块的指针 
调用4次transfer_8to16sub做亮度块的运动补偿，使得临时数组里存放的是残差，而原始图像里存放的是参考快的数据。
}//compensate16x16_interpolate 
计算出用于色差运动补偿的dx，dy 
}//MODE_NOT_CODED或者MODE_INTER或者MODE_INTER_Q 
否则，那就是MODE_INTER4V 
{ 
根据是否使用1/4像素运动估计，计算出4个色度块的运动向量 
以这4个运动向量为参数，调用4次compensate8x8_interpolate ，该操作类似于compensate16x16_interpolate，不同在于一次只计算一个块。
计算出用于色差运动补偿的dx，dy 
} 
调用CompensateChroma计算色差块的运动补偿 
{ 
调用interpolate8x8_switch2计算出u的插值 
调用interpolate8x8_halfpel_v或者interpolate8x8_halfpel_h或者interpolate8x8_halfpel_hv做实际的插值操作，或者直接返回
调用transfer_8to16sub_c做u份量的运动补偿 
调用interpolate8x8_switch2计算出v的插值 
调用interpolate8x8_halfpel_v或者interpolate8x8_halfpel_h或者interpolate8x8_halfpel_hv做实际的插值操作，或者直接返回
调用transfer_8to16sub_c做v份量的运动补偿 
}//CompensateChroma 
}//MBMotionCompensation 
如果需要编码，那么用MBTransQuantInter进行编码，并把结果返回给pMB->cbp 
{ 
调用MBfDCT进行宏块变换编码 
调用6次fdct 
调用MBQuantInter进行量化 
{ 
对于宏块里的每一块 
{ 
调用quant_h263_inter进行量化 
如果在量化后，前三个系数为0，并且系数的绝对值之和小于阀值，那么标记该块为全0块，将标记存入cbp。否则，标记为非全0块，也将标记存入cbp
} 
}//MBQuantInter 
调用MBDeQuantInter反量化 
{ 
确定要使用的反量化函数 
对于六个块里的每个块，如果cbp表示许可，都调用dequant_h263_inter反量化 
}//MBDeQuantInter 
调用MBiDCT做反离散余弦变换 
对于六个块里的每个块，如果cbp表示许可，都调用idct_int32反量化 
调用MBTrans16to8将恢复出的残差构成重建图像 
{ 
确定具体执行的函数，分为transfer_16to8copy和transfer_16to8add 
找到该宏块的y，u，v分量起始地址 
对于六个块里的每个块，如果cbp表示许可，调用相应得函数执行重建。 
}// MBTrans16to8 
}//MBTransQuantInter 
如果无残差，并且编码方式为MODE_INTER，并且帧方式是P帧，并且向量2分量都为0，那么可以考虑skip模式
如果可以考虑skip模式，则做进一步检验，如果检验通过，那么 
{ 
编码模式为MODE_NOT_CODED，并且在码流里做标记 
Continue 
} 
调用MBCoding将这个宏块写入码流 
{ 
写入非NOT_CODED标记 
调用CodeBlockInter写入码流 
{ 
编码mcbpc 
编码cbpy 
调用CodeVector编码运动向量 
对六个块，如果cbp只是需要编码，调用CodeCoeffInter进行编码 
}//CodeBlockInter 
}// MBCoding 
}//对于每一个宏块，依次执行如下操作 
更新fcode 
为下一帧的编码做简单的更新设置 
统计该帧编码长度 
}// FrameCodeP
