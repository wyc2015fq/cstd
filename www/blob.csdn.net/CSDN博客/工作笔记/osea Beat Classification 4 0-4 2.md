# osea/Beat Classification 4.0-4.2 - 工作笔记 - CSDN博客





2012年01月17日 14:17:43[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5417








4.0 Beat Classification

心拍分类



When a beat is detected it is classified as either a Premature Ventricular Contraction (PVC), a normal beat, or an unknown beat type.


当一个心拍被检测到，它被分类成室性期前收缩（PVC），正常拍，或未知心拍。



As mentioned previously a PVC represents any beat of ventricular origin, a normal beat is any contraction of non ventricular origin, and the unknown classification is used when the beat classifier has not been running long enough to make a decision.

前面提到过，PVC代表心室起源的任何心拍，正常拍是非心室起源的任何心脏收缩，当心拍检测器还没有运行足够长时间来进行决策时 使用未知类型。



Our beat classification strategy is probably the same as that used for most commercial arrhythmia detection systems.


我们的心拍分类策略大概 与大多数商业心律失常检测系统相同。

When a beat is detected it is characterized by a number of features such as width, amplitude, and R-to-R interval.


当一个心拍被检测到，通过许多特征来进行刻画，例如宽度，幅度，RR间期。

The beat is also compared to previously detected beat types. If the beat closely matches a previously classified beat type, the beat takes on the classification of its matching type.


这个心拍也和前面检测到的心拍类型比较。如果这个心拍匹配上一个先前分类过的心拍类型，这个拍就分类成它匹配的类型。



Otherwise, the beat is classified based on its beat features, the features of the beat type that the algorithm judges to be the dominant normal type, and the general signal conditions.


否则，这个拍基于它的特征，算法判别出的主导正常类型的特征，一般的信号条件 被分类。

The following documentation details the beat features used for classification, our methods for matching beats to previously detected beat types, our rules for selecting the dominant normal beat types, and the classification rules that we use.

下面的文档详细说明用来分类的心拍特征，我们用的 和前面检测到的心拍类型匹配的方法，我们用的选主导正常心拍类型的规则，以及我们使用的分类规则。



4.1 Beat And Signal Features ----------心拍和信号特征



Our beat classification algorithm only uses a small number of features to characterize individual beats. These include width, rhythm classification, similarity to the dominant beat shape, and noise content.


我们的心拍分类算法仅使用少量的特征来描述单个拍。包括 宽度，节律分类，和主导拍形状的相似性，噪声成分。



The width and rhythm classification are considered relative to the width and rhythm classification that applies to the dominant beat type.

  相对于主导心拍类型上的宽度和节律分类 考虑宽度和节律分类；



 Beat widths and polarities are estimated by the function AnalyzeBeat in analbeat.cpp, rhythm classification is handled by the function RhythmChk in rythmchk.cpp, low-frequency noise is estimated by the function NoiseCheck in noisechk.cpp, high-frequency
 noise estimation is handled by HFNoiseCheck in classify.cpp, and shape similarity is handled by either DomCompare or DomCompare2 in match.cpp

心拍宽度和极性估计由analbeat.cpp 中的AnalyzeBeat函数完成，节律分类由rythmchk.cpp 中函数RhythmChk 完成，低频噪声估计由noisechk.cpp 中函数NoiseCheck 完成，高频噪声估计由classify.cpp 中HFNoiseCheck 完成，波形相似性由 match.cpp 中的DomCompare.cpp 或 DomCompare2 完成。



4.1.1 Beat width  ---------心拍宽度



Beat width refers the width of the QRS complex. Width is a fundamental feature used for classifying beats. Normal beats tend to be narrower (less than 100 ms) than ventricular beats, and ventricular beats tend to be wider.

心拍宽度是指QRS波宽度。宽度是一个用来分类心拍的基本特征。正常拍（少于100 ms）比室性心拍窄，室性心拍趋向于宽大。



We estimate the QRS onset and offset points two different ways. First we search out from the fiducial point (detection point from the beat detector) until we find a region that precedes the QRS complex and a region that follows the QRS complex where at least
 5 samples are within a 100 uV range. This gives us a first estimate of the QRS onset and offset. We then estimate QRS onset and offset based on the slopes of the QRS complex.

我们用两种不同的方法估计QRS起终点。第一种方法是 从基准点（心拍检测器的检测点）开始 直到在QRS波前后各找出一个区域，满足在这个区域内至少5 个样本点幅度在 100 微伏范围内。这是QRS起始点的第一种估计。然后我们基于QRS波的斜率估计QRS起始点。



In obtaining a slope based estimate, we first search the QRS complex for maximum and minimum slopes. We use ¼ of the smaller magnitude of the maximum positive and maximum negative slopes as a slope threshold. Searching out from the center of the QRS complex
 we estimate onset and offset points as points where at least three samples have slopes with magnitudes that are less than the slope threshold.

在获得基于斜率的估计中，首先搜索QRS波最大最小斜率。利用最大正斜率和最大负斜率中幅度较小者的1/4倍作为斜率阈值。从QRS的中心开始 搜索，我们将 满足 至少3点斜率幅度小于斜率阈值 的点作为起终点的估计 。



If the isoelectric onset and offset estimates are within 25 ms of the slope based onset and offset estimates, we use the isoelectric onset and offset estimates, otherwise we use the slope based onset and offset estimates. Finally, if the offset is significantly
 lower that the onset, we search for a maximum slope between the offset point and the end of the beat and move the offset to a point where the slope is half that value.

 如果等电位起终点估计在斜率估计的起终点的25 ms内，我们利用等电位起终点估计，否则用基于斜率的起终点估计。最后，如果终点显著低于起点，搜索终点 和心拍末尾间的最大斜率 并且将终点移至 斜率等于该值一半的点。



4.1.2 Rhythm Classification 

节律分类



During a regular rhythm, ventricular beats often occur prematurely. Actually, the coupling interval between a normal beat and a ventricular beat tends to be fixed for ventricular beats that originate from a given site. If the normal R-to-R interval is less
 than the coupling interval, the ventricular beats do not tend to occur because the normal contraction occurs first. If the normal R-to-R interval is longer than the coupling interval, the ventricular beat occurs earlier than a normal beat would have occurred.

节律规整时，室性心拍常常提早发生。事实上，对于同源的室性心拍 正常拍和室性拍之间的联律间期  趋于固定。如果正常RR间期小于联律间期，没有发生室性心拍的倾向，因为正常收缩首先发生。如果正常RR间期长于联律间期，室性心拍比正常拍提早的情况可能已经出现。



As a feature of each beat, we characterize the interval from the previous beat to the present beat as either NORMAL, PVC, or UNKNOWN.


作为每个心拍的一个特征，我们将 从前一个拍到当前一个拍的间期 分为正常，PVC，未知三类。



A classification of NORMAL indicates that the beat was either part of a regular rhythm, or it was a normal beat that followed a ventricular beat.

正常类型 说明这个心拍是规则节律的一部分，或者它是紧随室性心拍之后的一个正常心拍。



 A rhythm classification of PVC indicates that the beat was either a premature contraction following a normal contraction or a ventricular beat that is part of a string of ventricular beats.

 节律类型PVC 意味着 心拍是正常拍之后的早搏拍 或是 一串室性心拍中的一个室性心拍。



 A rhythm classification of UNKNOWN indicates that the present rhythm is so irregular that it is unclear whether the present beat is premature or not.

节律类型UNKNOWN 意味着当前节律不规则以致于不清楚当前拍是否是早搏。



In the function that classifies R-to-R intervals, intervals are actually characterized as NN for two consecutive normal beats, NV for a normal beat followed by a ventricular beat, VN for a ventricular beat followed by a normal beat, VV for a pair of ventricular
 beats, and QQ for an interval where it is not clear from the rhythm what either beat represents. NN and VN intervals are represent NORMAL intervals, NV and VV intervals represent PVC intervals, and QQ intervals represent UNKNOWN intervals.

在分类RR间期的函数中，NN 表示两个连续的正常拍，NV表示正常拍后跟一个室性拍，VN表示室性拍后跟一个正常拍，V V 表示一对室性拍，QQ表示两个心拍表示的节律不明确。NN 和 VN表示 正常间期，NV 和VV 表示PVC 间期，QQ表示未知间期。



We consider a string of beats to be part of a regular rhythm when at least four intervals occur where each pair of consecutive intervals matches.

当出现 至少4个间期 其中每对连续间期匹配时，我们认为这串心拍是规整节律的一部分。 



 For rhythm classification we consider that two intervals match when they are within 12.5% of the average of the two intervals.

当两个间期在它们平均值的12.5% 以内时，我们认为两个间期匹配。 



We consider an interval to be an NV interval when the interval follows an NN interval and is less than 75% of the preceding interval.

Following an NV interval, the next interval is classified as VV if it matches the previous NV interval. The next interval is classified as VN if it is longer than the previous interval.

当一个间期在NN间期后并小于前一个间期的75%，认为这个间期是 NV。下一个间期被分类为VV，如果它在NV间期之后，和前一个NV间期匹配。 下一个间期被分类为VN，如果它比前一个间期长。


Our rhythm classification function also recognizes VN and NV intervals when they are part of a bigeminal rhythm (ventricular beats alternating with normal beats). The classification function assumes that bigeminy is occurring when over six intervals alternate
 intervals match but consecutive intervals do not match. The shorter intervals are then classified as NV and the longer intervals are classified as VN.

当它们是二联律的一部分时（室性拍和正常拍交替出现）节律分类函数也识别VN 和NV间期。当出现6个以上间期 且 间隔间期匹配 连续间期不匹配，分类函数假定二联律发生。短的间期分类为NV，长的间期分类为VN。



Finally, the rhythm classification function recognizes the rhythm pattern NNVNNNV when the previous interval was classified as QQ. This pattern is characterized two matching NN intervals, two matching NV intervals that are shorter than the NN intervals, and
 two VN intervals that are longer than the NN intervals.

最后，当前面的间期分类为 QQ时 节律分类函数识别节律模式 NNVNNNV。这个模式 被描述成 两个匹配的NN 间期，两个匹配的比NN间期短的NV间期，两个比NN间期长的VN间期。



The rhythm classification could be extended to recognize other complex rhythm patterns in the QQ state (such as NNVNNV), this would allow more rapid initialization of the rhythm classification when PVCs are occurring frequently, but it has the potential to
 produce false rhythm classifications during irregular rhythms such as atrial fibrillation of flutter.

节律分类可以被扩展 来识别在QQ状态下的其它复杂节律模式（例如NNVNNV），当PVC 频繁出现时这能更快地初始化节律分类，但是在不规则节律情况下，如房颤房扑，有潜在的产生错误节律分类的可能。



4.1.3 Low Frequency Noise

低频噪声



We try to characterize the lower frequency noise by examining the signal between the end of the T-wave of the previous beat and the beginning of the P-wave of the beat being classified. Low frequency noise generally results from movements that deform the electrode
 or the skin around the electrode.

我们尝试通过 检验前一个心拍的T波终点和当前正在被分类的心拍的P波起点之间的信号 刻画低频噪声。低频噪声一般由 使电极变形的体动 或是 电极周围的皮肤 引入的。 



We estimate the end of the T-wave as the beginning of the first isoelectric segment that follows the fiducial point by at least 300 ms..

 在基准点300 ms之后的 第一个等电位段的起点作为T波终点的估计。



 We estimate the beginning of the P-wave as the first isoelectric segment that is at least 250 ms before the fiducial mark.


基准点250 ms之前 第一个等电位段 作为P波起点的估计。



If we find an isoelectric segment at 250 ms, we search inward from there to find the isoelectric region that is closest to the onset of the QRS complex.

如果在250 ms处发现等电位段，我们从那里开始向里搜索寻找最接近QRS起点的等电位区域。



If there is space between the T-wave of the previous beat and the P-wave of the present beat, the noise in the segment is characterized by the maximum and minimum signal values divided by the segment length. If the segment is longer that 250 ms the noise measurement
 is made using only the 250 ms that precedes the P-wave.

如果前一拍的T波 和 当前拍的P波之间有距离，这个片段内的噪声 用最大最小信号值除以片段长度 来刻画。 如果片段长于250 ms，仅用P波前的250 ms 做噪声测量。



4.1.4 High Frequency Noise

高频噪声

High-frequency noise is generally produced by muscle contractions close to the electrodes. We characterize the high-frequency noise surrounding a QRS complex beat by the maximum signal level of a bandpass filtered version of the beat outside the QRS complex.

高频噪声一般由接近电极的肌肉收缩产生。我们通过  在QRS波以外 带通信号的最大信号水平  来刻画QRS波周围的高频噪声 。


4.2 Beat Matching       -----------心拍匹配 



The file match.cpp contains the functions that handle matching beats to previously detected beats. Functions in match also handle storage of beat features that are associated with different beat types.

文件matc.cpp 包含 和之前检测到的心拍进行匹配处理的函数。 match 中函数也 负责存储 和不同心拍类型关联的心拍特征。


4.2.1 Beat Comparison   ---------心拍比较



Beat matching is done by comparing beats sample point by sample point after aligning the beats and removing any constant difference between beats.


心拍匹配的实现是通过 在对齐心拍和去除心拍间所有固定的差别后 逐个采样点比较心拍。



In the present implementation, we represent beats with 100 sample long buffers of data that have been down sampled from 200 samples-per-second to 100 samples-per-second by averaging consecutive samples.


当前的实现中，我们用100 个采样点长度的缓冲数据表示心拍，数据是通过平均连续样本点 从200 sps 速率抽样成100 sps。



The detection point as indicated by the QRS detector is 400 ms (FIDMARK) from the beginning of the buffer.

QRS检测器得到的检测点 在这个缓冲 从起点开始的 400 ms处（FIDMARK ------对起点位置）。



 Though we represent beats with 1000 ms sample buffers, we only compare 300 ms (MATCH_LENGTH) of data centered on the detection point.

虽然用1000 ms长的样本点缓冲表示心拍，我们仅比较 以检测点为中心的 300 ms（MATCH_LENGTH）数据。 （-----30 点）



The functions CompareBeats and CompareBeats2 compare pairs of beats to each other.


函数CompareBeats 和CompareBeats2 将一对心拍相互比较。



In CompareBeats the second beat is scaled so that its amplitude (max – min) matches the first beat.


在CompareBeats 中，第二个拍被尺度化（----按照第一拍来放缩处理）使得它的幅度和第一个拍匹配。



CompareBeats then calculates the point-by-point sums and absolute differences for the two beats for five different relative shifts and saves the sum and difference for the relative shift that produced the smallest total difference.


CompareBeats 函数接下来 对两个心拍5个不同的相对偏移 计算逐点和与绝对偏差， 并保存 产生最小总体偏差的相对偏移对应的 和与差。 



We use the ratio of the sum of the absolute differences to the sum of the absolute sums as indication of the similarity between beats.


 利用 绝对偏差和与绝对值和 的比 表示心拍相似性。



The similarity metric is also divided by the MATCH_LENGTH so that it is insensitive to changes in MATCH_LENGTH.

相似性度量也除以MATCH_LENGTH ，使它对MATCH_LENGTH的变化不敏感。



CompareBeats2 differs only slightly from CompareBeats. CompareBeats2 does not scale the second beat before comparing beats. In this way the similarity measure returned by CompareBeats is blind to amplitude differences between beats, but the similarity measure
 returned by CompareBeats2 takes amplitude differences into account.

CompareBeats2 仅与CompareBeats稍有不同。CompareBeats2 在比较心拍前不对第二个心拍进行放缩。按照这种方式，CompareBeats  返回的相似性度量 忽略心拍间的幅度差别，但是CompareBeats2 返回的相似性度量考虑了幅度差别。（------CompareBeats2 相对更严格 ）


4.2.2 Rules for Matching Beats   -----------拍匹配规则



The rules for deciding whether a beat matches any of the previously encountered beat types are implemented in Classify. 

心拍匹配的判别规则 在Classify 中实现。



The function BestMorphMatch (morph is short for morphology here) compares a new beat to templates of all the preceding beats and returns the index of the template that best matches the new beat.


函数BestMorphMatch （这里morph 是morphology的简写）将新检到的拍和所有前面的心拍模板比较 并 返回最佳匹配模板号。



BestMorphMatch initially compares the new beat to all templates using CompareBeats. If the beat closely matches two different templates, BestMorphMatch matches the new beat to the template which produced the best match with CompareBeats2. 


 函数BestMorphMatch 一开始 利用函数CompareBeats将新检到的拍和所有模板比较。如果此心拍匹配两个不同的模板，BestMorphMatch 利用函数CompareBeats2 获得的最佳匹配模板。



BestMorphMatch also returns the two similarity indexs for the two beats. If the match indexes are small enough and the beat is not premature, the new beat is considered to be of the same type template indicated.

函数BestMorphMatch 也返回两个心拍的两个相似指数。如果匹配指标足够小并且此拍不是早搏，这个新检拍 考虑用相同类型的模板表示。



When a beat matches a template the function UpdateBeatTypes is called to average the new beat into the template. UpdateBeatTypes performs an interative average by adding 7/8ths of the template to 1/8th of the new beat. If, however, the new beat is the only
 second beat, half the template is added to half the new beat. I tried combining 3/4ths of the template with 1/4th of the new beat for the third and fourth beats, but this hurt performance of the algorithm.

当一个心拍匹配上一个模板时，函数UpdateBeatTypes 被调用将新拍平均到模板。UpdateBeatTypes 执行迭代平均，将7/8 倍模板 和 1/8倍心拍相加。然而，如果新检到的拍是第二拍，模板的一半和新拍的一半相加。我试过对 第三拍和第四拍 将3/4倍模板和 1/4倍新拍 合并，但是这样做有损算法的性能表现。



UpdateBeatTypes also calculates the beat width and polarity for the new average template and stores the type of beat that preceded the new beat.

UpdateBeatTypes 也计算新的平均模板的宽度和极性 并且 保存新检测拍之前的心拍类型。--------------函数名应该是UpdateBeatType



If either match index is too large or the beat is premature and the amplitude sensitive match is too large, the new beat is considered to be a new beat type and the new beat is used to start a new beat template. However, if the beat is judged to be both noisy
 and regular a new beat type is not initialized because we assume that it is just a normal beat that would have matched if it weren’t for the noisy conditions.

如果两个匹配指标中的每一个都过大 或者 这个拍是早搏并且对幅度敏感的匹配结果太大，新检测心拍 被认为是一个新的拍类型 并用来新建一个心拍模板。然而，如果此拍 被判定噪声大、纪律规整，不用它建立新模板，因为我们假设 它是一个正常拍，如果它不是含有较大噪声它会和某个模板匹配。



4.2.3 Combining Similar Templates and Eliminating Templates  ---------------合并相似的模板 及 删除模板



Memory limits can produce problems for beat classification algorithms that use template matching.


内存限制能够给 使用模板匹配的心拍分类算法带来问题。



Low levels of noise can cause multiple beat types and templates to be created for what are actually beats of the same type.


低噪声水平 能导致 为 实际上类型相同的心拍创建多个心拍类型和模板。



This can quickly use up whatever space was allocated for beat template storage.


无论为心拍模板分配多大存储空间，都会很快用光。



If space is allocated for a large number of templates, comparing beats to a large number of possible matches can significantly eat into available processing time.

如果为大量的模板分配空间，心拍和大量模板匹配 会占用大量处理时间。



In an effort to reduce the number of redundant templates that are created, BestMorphMatch also detects when two templates are similar enough that they can be combined to reduce memory usage.

为减少已经创建的多余的模板数量， 当两个模板足够相似它们可以合并以减少内存使用时，BestMorphMatch  也会检测到。



 If a new beat closely matches two different beat types, BestMorphMatch compares those two types to each other. If the two types match closely enough, they are combined, freeing up memory for a new template type.

如果新检拍 匹配两个不同心拍类型，BestMorphMatch 比较这两个心拍类型。如果两个类型足够相似，它们被合并，释放内存以供新模板类型使用。



If the maximum number of beat types has already been detected and a new beat is detected that does not match any of the beat types, the new beat is substituted for the beat type that has been detected the fewest number of times.


如果已经检测到 最大数量的心拍类型，并且新检测拍不与任何一个心拍类型匹配，新检测拍取代 被检测出来次数最少的心拍类型。



If two beats types have been detected an equal number of times, the new beat is substituted for the beat type that was least recently detected.

 如果两个心拍类型被检测出来的次数相等，新检测拍取代   最近最少检测拍。 ---------（距离上一次出现此心拍类型的时间最久的模板）



