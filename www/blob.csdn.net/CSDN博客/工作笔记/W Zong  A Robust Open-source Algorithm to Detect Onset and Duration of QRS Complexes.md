# W Zong / A Robust Open-source Algorithm to Detect Onset and Duration of QRS Complexes - 工作笔记 - CSDN博客





2012年01月18日 14:51:06[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5403标签：[algorithm																[signal																[each																[算法																[search																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)




|论文网址：[http://mimic.physionet.org/Archive/Publications/Zong03b.pdf](http://mimic.physionet.org/Archive/Publications/Zong03b.pdf)摘要1 引言2 方法2.1 低通滤波2.2 曲线长度变换单通道ECG曲线长度变换多通道ECG曲线长度变换   ---------------------使用多导联信号检测QRS的例子窗口宽度  ---------------130 ms尺度因子C 的作用，具体的选取值未讲。（------------横坐标u表示的是信号幅度单位时间的变化率，纵坐标表示曲线长度对u的变化率或敏感性。 当C 不等于0 时，u趋于0   dl/du 趋于0，对单位时间幅度变化较小低频成分如基线漂移，P, T波有抑制作用。对固定的非零的u，C 值选取越大曲线长度对幅度变化率敏感性变小，对同一个P或T波抑制作用变强但对同一个QRS波增强作用会减弱，所以要折中选C值。u表示一个采样间期的幅度变化，自然既和采样间隔（采样率）又和分辨率有关系。）2.3  2.3. Decision rule   -------------判定准则The decision rule consists of two procedures: (I) thresholding on the LT signal to find a possible QRS position; (2) searching locally to find the QRS onset and duration.判定准则包含两个过程：（1）在长度变换信号上用阈值法找到可能的QRS波位置；（2）局部搜索找到QRS起点和时限。(1) Adaptive thresholding         ------------自适应阈值A threshold base value is established and is initially assigned as three times the mean value of the LT signal for the initial 10-second period. The actual threshold is set to 1/3 of the threshold base value. The threshold base value is then adaptively adjusted, based on the maximum LT value of each detected QRS complex. 建立阈值基础值，对初始的10秒，基础阈值赋值为长度变换信号均值的三倍。实际阈值置为基础阈值的1/3。接下来基础阈值自适应调整，以检到的每个QRS波在LT上的最大值为调整基础。(2) Local search strategies          ----------局部搜索方法When the LT signal crosses the threshold, a probable QRS is noted. From the threshold-crossing point, tc_i, the algorithm searches backward for 125 ms to get a minimum value, Lmin_i, and forward for 125ms to get a maximum value, Lmax_i. The difference, LAi = Lmax_i - Lmin_i   is obtained. Then, from tc_i ,  again, the algorithm searches backward to find the location, Qb_i. where the LT value drops monotonically to Lmin_i = LAi/100 , and searches forward to find the location, Sb_i, where the LT increases to Lmax_i - LA_i / 20. Qb_i and Sb_i are considered the base values of QRS onset and end, respectively. The actual QRS onset is adjusted by -20 ms or -5 samples and the actual end is adjusted by +20ms or +5 samples. This widening adjustment compensates for the time interval  loss caused by the onsetlend thresholds. The adjustments are based on statistical Observation of the differences between the algorithm's onset / end estimates and human expert judgments. Finally, a 250ms eyeclosing period is applied after each detected QRS to avoid possible double detection of the same beat.（1） LT 信号与阈值相交时，一个可能的QRS波被发现。和阈值焦点记为tc_i；（2）在LT信号上，从tc_i 向后125 ms 内搜索最小值，记为Lmin_i；（3）从tc_i向前125 ms内搜索最大值，记为Lmax_i，差值LA_i = Lmax_i - Lmin_i；（4）再次从tc_i 向前或向后搜索过相应阈值点，检测到Qb_i 或 Sb_i；（5）对检测位置进行补偿。调整是基于算法定位和专家判断差别的统计观察；（6）使用不应期。3. 结果4. 总结与讨论 ------------有对应本文算法的开源程序。|
|----|](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=each&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=algorithm&t=blog)




