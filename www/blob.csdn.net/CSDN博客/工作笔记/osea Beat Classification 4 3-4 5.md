# osea/Beat Classification 4.3-4.5 - 工作笔记 - CSDN博客





2012年01月17日 14:18:27[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5401









4.3 Dominant Normal Beat Selection  ------------主导正常拍选择



New beats are classified as NORMAL or PVC largely based on how they compare to the dominant normal beat type.


新检到的拍 被分类成 正常或PVC,  大多数是基于它们与主导正常心拍类型匹配情况。 



Dominant beat selection is handled primarily by the function DomMonitor in classify.cpp. DomMonitor keeps track of the beat types, rhythm classifications, and beat classifications for the most recent 180 beats.

主导拍选择主要由文件classify.cpp中函数DomMonitor 完成。DomMonitor 对最近180 个心拍的 拍类型（----指对应的模板），节律类型，拍分类  进行记录和跟踪。



In deciding which is the dominant normal beat, DomMonitor uses a simplified set of rules to classify beats. The rules that DomMonitor uses are biased in favor of misclassifying some normal beats in order to be sure that ventricular beats are not classified
 as normal. 

在确定哪一个是主导正常心拍过程中，DomMonitor 利用一组简化的规则来分类心拍。DomMonitor 使用的规则偏向于错分类一些正常心拍，以确保室性心拍不被分类为正常拍。



DomMonitor classifies beats as normal if:

DomMonitor 将心拍分类为正常，如果满足：



• The rhythm is NORMAL, the beat width is less than 120 ms, and there have been at least two in a row of this type; or

节律正常，宽度小于120 ms，这个类型中至少有连续两拍； 或者



• The previous beat was within the P-R interval of this beat and the one before that was the same type (this rule allows for accidental detections of P-waves on 108).

前一拍在当前拍的PR间期内 并且 与倒数第二个心拍有相同的类型（由于记录108中P波偶尔被检出 此规则会被满足）。  



DomMonitor chooses the dominant normal type as the beat type with the most beats classified as normal out of the last 180 beats.


 在前180 拍以外 与某心拍类型 匹配的心拍 大多数被分类成正常拍， DomMonitor 选择此心拍类型作为主导正常拍类型 。



（-----特殊情况是所有模板中尚未发现正常拍；

 -------或者发现有某个类型有被分类为正常的拍，找出含正常拍最多的类型后，发现正常拍数量不多， 尚未达到匹配总数的一半；

--------或者是与该类型匹配的心拍数量不够多 尚未达到 匹配心拍数量最多的 拍类型的 一半；

以下说的就是后面两种特殊情况下的处理方法，选择从一开始到现在 匹配心拍数量最多的 拍类型作为主导正常拍类型。 

如果从一开始到现在一直输入异常拍 这个做法就会出问题，但是这与实际情况不符，异位心拍是偶然才发生的。）



If there aren’t any beat types that have had at least  half of their beats classified as normal in the last 180 beats, or the most frequently occurring beat type in the last 180 beats occurred at least twice as often as the most frequently occurring normal
 beat type, DomMonitor selects as dominant the type that has occurred most frequently since the beginning of the record.

在之前180拍中，如果没有任何一个拍类型  与之匹配的心拍有一半以上被分类为正常拍，或者 在之前180 拍中 出现次数最多的心拍类型 至少是 出现最多正常拍的心拍类型匹配心拍数量的 2倍以上，DomMonitor 选择从这个记录开始分析到现在 出现次数最多的拍类型。



4.4 Dominant Rhythm  --------主导节律

DomMonitor also characterizes the general rhythm. If the rhythm is regular, than the premature occurrence of a beat indicates that the beat is abnormal. However, if the dominant rhythm is irregular (as occurs during atrial fibrillation of flutter) than the
 relative prematurity of a beat does not tell us much about whether or not the beat is normal. DomMonitor classifies the dominant beat type as regular if at least 60 (IRREG_RR_LIMIT) of the last 180 (DM_BUFFER_LENGTH) R-to-R intervals were classified as UNKNOWN
 (QQ). 

 DomMonitor 也刻画一般的节律。如果节律是规整的，那么一个心拍提早出现意味着这个拍是异常的。然而，如果主导节律是不规整的（如出现房颤或房扑），那么心拍相对提早不能告诉我们这个拍是不是正常的。如果最近180 个RR间期中至少有60个 被分类成 未知类型，DomMonitor 分类主导心拍类型 为规则的。



4.5 Beat Classification --------心拍分类

Beat classification is handled by the function Classify in classify.cpp. 

心拍分类 由classify.cpp 文件中的 函数Classify 处理。



After a beat has been matched (or not matched) to a beat type it is classified as NORMAL, PVC, or UNKNOWN.

在一个拍已经匹配（或未匹配）一个心拍类型之后 它将被分类成 正常，PVC，或未知类型。



If the type that the beat was matched to has been classified as NORMAL or PVC, the beat takes on the classification of its type.


如果心拍匹配的 拍类型（模板）已经被分类为 正常或PVC， 模板的分类 赋给 这个心拍。（------单个拍置为同族拍的类型）



If the beat type has not been classified, but the three of the last four or six of the last eight of this type were classified as PVCs by post classification, then the beat is classified as a PVC.


如果此拍类型（心拍组，心拍族）没有被分类，但是这个类型的 四拍中有三个 或前八拍中有六个 被post 分类为PVC，那么这个心拍也分类成PVC。



Otherwise, the beat is classified according to its features relative to the features of the dominant normal beat.

否则，根据此心拍 相对于主导正常心拍的特征 被分类。



4.5.1 Type Classification  -------类型分类，心拍族，或模板的分类

When a beat is matched to a type that does not have a classification yet, Classify attempts to classify the type. A type is classified as NORMAL if:

当一个心拍匹配上的类型还没被分类，函数Classify 尝试分类此类型。一个类型被分类成正常，如果满足条件：



• There have been at least four in a row and the width for the QRS is less than 20 ms wider than the dominant type; or

已有至少接连4拍，QRS宽度比主导类型QRS宽度 不能宽过 20 ms;  或者



• No dominant type has yet been found and there have been at least six in a row; or

没有主导类型 ，且 连续出现6 拍以上； 或者



• The beats of this type are NORMAL rhythm beats in bigeminy.

这个类型的心拍 是二联律中的 正常节律 心拍(-----VN)。



Types are only classified as PVC if they occur as the premature beats in a bigeminal rhythm.

类型仅在一种情况下被分类为 PVC，如果 它们是 二联律中的早搏拍。-----------------模板被分类为 PVC 的条件很严格！



If a beat type has occurred at least 10 times in the last 180 beats, none of the beats appear to be normal (regular with width less than 130 ms), and the type has previously been set to NORMAL, DomMonitor resets the type classification to UNKNOWN.

如果一个心拍类型 在最近180拍中至少出现10次，这些拍没一个是正常的（节律规整，宽度小于130 ms), 并且这个类型先前被置为 NORMAL，DomMonitor 重置此类型的分类为UNKNOWN。



4.5.2 Post Classification  ---------延后的分类, 利用后面出现的心拍信息,  后验分类



Post classification attempts to more accurately classify beats types and rhythms by using information from the following beat.


Post 分类利用后出现心拍的信息试图更准确地分类心拍类型和节律。



It is easier to detect a premature R-to-R interval followed by a compensatory pause than it is to decide whether a given interval is premature.


更容易检测到一个早搏RR间期后跟一个代偿间歇，那样它就可以来判断所给间期是否是提早的。



A running record of the post classifications for the last eight beats of each beat type is used to determine a post classification for that beat type. This classification may change whenever a new beat of a given type is detected.

每种心拍类型的前八个心拍的post 分类结果流动地记录下来，用来确定那种心拍类型的post 分类。每当新检到一个已知类型的心拍这个分类可能改变。



A beat is considered by post classification to have a PVC type rhythm if the preceding R-to-R interval is less than 87.5% of the R-to-R interval that preceded it and the following R-to-R interval is 12.5% larger than the R-to-R interval that preceded the interval
 that preceded the beat. 

post 分类认为一个心拍有PVC类型的节律，如果前一个RR间期小于它前面间期的87.5% 并且随后的RR间期大于 此拍之前的间期的前一个间期的12.5%。 （---------NNVN 模式， NV 小于87.5%NN且VN 大NN12.5%以上）



If a beat  has a PVC type rhythm and is preceded and followed by the dominant beat type, it is post classified as a PVC. Also, if a beat is significantly different in shape (amplitude sensitive similarity index > 2.5) and it is preceded and followed by the
 dominant beat type, it is classified as a PVC.

如果一个心拍有PVC 类型的节律并且前后都是主导拍类型，后验分类为PVC。还有，如果一个心拍形状显著不同（对幅度敏感的相似度指标 > 2.5) 并且前后都是主导心拍类型，也被分类为PVC。



4.5.3 Beat Classification  -------心拍分类， 跟据单个拍的提取特征分类



If a beat is does not match a previously classified type it is classified according to a set of rules that are part of the function TempClass.

如果一个心拍未匹配上先前的已经被分类的类型，会根据函数TempClass中的一组规则对它进行分类。



 The rules in TempClass are arranged in an if-else structure as follows:

TempClass中的规则安排成如下的if-else 结构：



• If no dominant beat type has been identified, classify the beat as UNKNOWN;

如果尚未识别出主导拍类型，分类为UNKNOWN;



• If the dominant rhythm is normal, the dominant beat type doesn't vary much, this beat is premature and looks sufficiently different than the dominant beat classify as PVC;

如果主导节律是正常，主导心拍类型变化不大，此拍是提早的且和主导拍看起来非常不同，分类为PVC;         ------主导心拍已经存在



• Else, if the beat is sufficiently narrow (< 90 ms), classify as NORMAL;

心拍很窄（< 90  ms) ,分类为 正常；                                                                




• Else, if the beat cannot be matched to any previously morphology and it is not premature, consider it NORMAL (probably noisy);

如果此拍不能匹配任何先前的拍形态 并且不提早出现，认为 正常（可能是噪声）；



• Else, if the maximum number of beat types have been stored, the beat is not regular or premature and only one beat of this morphology has been seen, call it normal (probably noisy);

如果心拍类型数量已经达到最大，此拍不规则也不提早 并且仅发现一个有这样形态的心拍，分类为正常（可能是噪声）；



• Else, if this beat looks like the dominant beat and the rhythm is regular, call it NORMAL;

如果此拍像主导心拍 并且 节律规整， 分类为正常；



• Else, if the post classification rhythm is normal for this type and its shape is close to the dominant shape, classify as NORMAL;

如果对这个类型post 分类 节律正常 并且 形状和主导拍形状接近， 分类为正常；



• Else, if the beat is not premature, it looks similar to the dominant beat type, and the dominant beat type is variable (noisy), classify as NORMAL;

如果此拍不提早，看起来和主导拍类型相似，并且主导拍类型容易变化（噪声大），分类为正常；



• Else, if this beat is significantly different from the dominant beat, there have previously been matching beats, the post rhythm classification of this type is PVC, and the dominant rhythm is regular, classify as PVC.

如果此拍和主导拍显著不同，存在与之匹配的先前的心拍， post 节律分类是PVC, 且主导节律规整，分类为 PvC.



• Else, if the beat is wide ( > 110 ms), wider than the dominant beat (at least 40 ms), doesn't appear to be noisy, and matches a previous type, classify it as a PVC.

如果此拍宽大（> 110 ms), 比主导拍宽（至少40 ms），没出现噪声，匹配上先前的一个类型，分类为 PVC。



• Else, if the dominant rhythm is regular and this beat is premature then classify as PVC.

主导节律规整并且此拍提早，那么分类为PVC。



• Else, if the beat is regular and the dominant rhythm is regular call the beat NORMAL.

如果此拍间期规整 且 主导节律规整，分类为正常；



• Else, if the beat is wider than the dominant normal beat type, wider than 100 ms , and significantly different from the dominant beat, call it a PVC.

如果此拍比主导正常拍类型宽大，宽过 100 ms， 且与主导拍显著不同，分类为PVC。



• Else, if the beat is sufficiently narrow (< 100 ms), call it NORMAL.

如果此拍充分 窄（< 100 ms) ,分类为正常。



• Else, if the beat isn't much wider (< 20 ms) than the dominant beat classify it as NORMAL.

如果此拍比主导拍宽的不是非常多（ < 20 ms), 分类为正常。



• Else, if the beat is similar to the dominant beat, call it NORMAL.

如果此拍和主导拍相似，分类为正常。



• Else, if the beat is noisy, we can't trust our width estimate and we have no useful rhythm information, so guess NORMAL.

如果此拍噪声大，我们不能相信我们的宽度估计 并且我们没有有用的节律信息，猜测为正常。



• Else, classify it as a PVC.   

分类为 PVC。



When a rule indicates that a beat or beat type should be similar to normal, this means that the amplitude sensitive similarity index for the beat compared to the dominant beat is must be below a given threshold.

当一个准则表明一个心拍或心拍类型类似正常，这意味着 用来将此拍和主导拍比较 对幅度敏感的相似性指标 必须低于一个给定阈值。





