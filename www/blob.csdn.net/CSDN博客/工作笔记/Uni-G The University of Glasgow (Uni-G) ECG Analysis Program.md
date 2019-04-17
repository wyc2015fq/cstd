# Uni-G/ The University of Glasgow (Uni-G) ECG Analysis Program - 工作笔记 - CSDN博客





2012年01月18日 14:50:11[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5965








The University of Glasgow (Uni-G) ECG Analysis Program
PW Macfarlane, B Devine, E Clark
University of Glasgow, Section of Cardiology and Exercise Medicine, Royal Infirmary, Glasgow



Abstract          ------摘要

The University of Glasgow 12/15 lead ECG analysis program has been in continuous development for over 20 years. It has been adapted to meet the needs of different users and keep abreast of changes in terminology as well as new morphological features described
 in the literature.It is applicable to neonates as well as adults and takes account of racial variation in wave amplitudes. It has

capability for comparing serially recorded ECGs using  one of two different approaches. The many varying features of the software have led to the introduction of the descriptor Uni-G (unique) ECG analysis program. 

 格拉斯哥大学12/15导ECG分析程序已连续开发超过20年。它满足不同用户的需要并且使用最新的术语及最新的文献中描述的形态特征。适用于新生儿及成人，考虑到波形幅度上种族差异。能够用两种不同的方法比较连续记录的ECG。很多软件特征的改变引出本文对Uni-G（独有的）ECG分析程序描述性的介绍。

1. Introduction   ----------引言

Methods for the analysis of electrocardiograms using automated techniques were first investigated in the University of Glasgow in the late 1960’s. The earliest of publications introduced methods for processing waveforms recorded in groups of three leads

simultaneously, whether they were from X, Y, Z orthogonal leads or carefully selected groups of three leads from the 12-lead ECG, e.g. I, aVF, V1 (1,2). At the end of the 1970’s, a decision was made to move to develop a 12-lead ECG analysis program where all
 leads

were recorded simultaneously. A digital electrocardiograph was designed and built locally (3) and with this, ECGs could be collected in digital form to permit further development of software for analysis and interpretation. Indeed, this instrument was capable
 of acquiring 11 leads simultaneously so that a complete 12-lead plus an orthogonal 3-lead ECG could be obtained simultaneously.


在二十世纪六十年代末，格拉斯哥大学最先研究使用自动技术分析心电图的方法。最早的文献引入的方法用来处理同步记录的三导波形，它们是X，Y，Z正交导联或是从12导ECG中精心选择的三个导联，如I，aVF，V1。在二十世纪七十年代末，决定开发同步12导ECG分析程序。设计制造了一个数字心电图机，利用它可以数字形式收集ECG，以供进一步开发分析和解释软件。实际上，这个仪器能同步获取11导波形，因此完整的12导加正交3导ECG可以同步获取。



Throughout the 1980’s, there was a major effort to collect databases from apparently healthy individuals of all ages from birth onwards and various publications presenting these data have appeared previously (4, 5). Full details will be published in a new
 edition of

Comprehensive Electrocardiology (6). Diagnostic criteria evolved therefrom in a variety of ways and a comprehensive 12-lead ECG analysis program was introduced for worldwide interpretation of ECGs (7).


整个二十世纪八十年代，投入很大的努力  从明显健康的所用年龄段的个体，从刚一出生的个体 收集数据，发表了多种介绍这些数据的出版物。全部详情将在新版《综合心电学》中发表出来。诊断标准以多种方式向前演变，综合12导ECG分析程序可在世界范围内解释ECG。



 2. Methods         ----------方法

2.1. Signal processing            --------信号处理

The University of Glasgow (Uni-G) ECG interpretation program is based on an analysis of 8 or 11 simultaneously recorded leads acquired at 500 samples per second. The first stage in analysis is to apply a 50 Hz or 60 Hz notch filter to remove AC interference
 if this has not already been done by the electrocardiograph itself.Thereafter, methods for detection of excessive artefact are used and if leads are found to have an unacceptable quality of recording, the five seconds in which this is found, i.e. the first
 or second half of the recording is set to be a continuous value. It was found that it could be beneficial to retain five seconds of a lead given that noise

very often occurs in short, one or two second bursts.The next stage in the analytical process is QRS detection and typing. Effectively, a function based on a combination of available leads is formed from which putative QRS complexes are determined. Thereafter,

wave typing is undertaken using an iterative process whereby the first complex in Lead I is compared with the second to look for any differences. The technique is extended to include all complexes in this lead and then repeated for four other leads, as often
 it is only one or two leads which clearly show an aberrantly conducted complex.

Uni-G ECG解释程序分析8个或11个同步记录导联，采样率500 sps。分析第一步，如果心电图机自己没有去除50 或60Hz 交流干扰，应用陷波去除。然后，检测过度的伪迹，如果发现一些导联上记录质量差到无法接受，将发现噪声大的5秒波形，即前5秒或后5秒，设置为连续值。发现保留5秒波形是有益处的，这是考虑到噪声出现的时间常常很短，一秒或两秒时间内突然剧烈的出现噪声。分析过程的下一步是QRS检测和归类。在结合可用导联基础上形成有效的检测QRS复合波的函数。然后，使用迭代过程将波形归类，I 导上第一个复合波和第二个复合波比较找出不同。这个技术扩展到这个导联的所有复合波，然后在其它四个导联上重复该过程，因为常常只在一个或两个导联上异常传导复合波清晰可见。



A complex selection procedure then decides which class of beat will be selected for averaging and subsequent interpretation. At this stage, cognisance has to be taken of whether or not any beats are paced and although the software itself has routines for detecting
 and

removing pacemaker stimuli, this is best achieved by front end processing with signals sampled at a much higher rate, e.g. 8,000 samples per second within the electrocardiograph firmware itself. If this is done, a list of pacemaker spike locations is forwarded
 to the Uni-G program and the spike artifacts are removed from the data. 

复合波选择程序确定哪一类心拍将用来叠加平均和接下来的解释。在这一步，必须识别每个心拍是否是起搏心拍，尽管软件自身有检测和去除起搏脉冲的程序，最好在心电图机自身固件里面使用非常高的采样率，例如8000sps，通过前端处理完成起搏脉冲检测。如果检测完成，起搏钉位置列表会被送到Uni-G程序，钉状的伪迹会被从数据中去除。



The program has optional approaches to computing the average QRS cycle including a simple mean, a weighted mean and a median beat. In different commercial versions, manufacturers may utilise their own proprietary software for beat averaging if desired.

程序有几种方法计算平均QRS周期，包括简单的平均，加权平均，中间心拍。在本软件不同的商业版中，如果需要制造商可以使用他们自己的软件做心拍平均。



Different approaches to finding fiducial points have been tried, including a simple form of threshold crossing to a more complex template matching technique.Ultimately, a combination of these approaches has been adopted where, for example, QRS onset was found
 to

perform best with respect to a noisy test set using a threshold technique. On the other hand, T-end performed best using a template matching method. All QRST amplitudes are referred to QRS onset as are P wave measurements, which represents a departure from

an early approach where a straight line was fitted between P onset and P termination.


尝试了不同的方法寻找基准点，包括简单形式的阈值法到更为复杂的模板匹配技术。最终，将这些方法结合起来使用，例如，在一个噪声很大的测试集上阈值技术定位QRS起点效果最好。另一方面，使用模板匹配方法定位T波终点效果最好。所有QRST幅度都是参考QRS起点，P波幅度测量也是一样，一个早期使用的方法是在P起点和P末尾之间用直线拟合，P波幅度测量值表示偏移距离。（--------英文理解上有疑问，P波幅度可能不参考QRS起点，而是用后面描述的直线拟合求偏移的方法来计算）



Individual QRS and T fiducial points are derived for all leads and a method of selecting the earliest QRS onset for example is utilised in order to determine a global QRS onset. A similar approach is adopted for QRS termination and the difference between
 the two global measurements is taken as the overall QRS duration. It was found optimum to utilise a common P onset and P termination in view of the unreliability of P wave detection in many ECGs.

对所有导联单独的QRS和T波推断出基准点，使用一种方法，例如选取最早的QRS起点确定整体的QRS起点。类似的方法应用到QRS终点，两个整体测量点之间的距离认为是总体QRS时限。考虑到在很多ECG上P波检测的不可靠性，发现使用公共的P起点和P终点是最佳的。



The wave measurement section of the program meets all the requirements of the relevant IEC test procedures as shown in Table 1.

程序波形测量部分满足全部有关IEC 测试要求，结果见表1。



Table 1. This table shows the mean and standard deviation of the difference between the measurements made by the Glasgow program and by 5 referees in the 100 ECGs in the CSE measurement set. The values in [ ]are the IEC acceptable differences and standard

deviations for global durations and intervals for biological ECGS. It can be seen that the program results are well


within the recommended tolerances. Difference Mean Standard Deviation

这个表格显示了 在CSE测量数据集100个ECG病例上 Glasgow程序和5个专家测量结果之间的差别 的均值和标准差。方括号中的值是 对生理ECG整体时限和间期测量 IEC可以接受的偏差和标准差。可见程序结果在建议的容许限度内。偏差的均值和标准差是

___________________________________________

P Duration 1.348 [10] 8.501 [15]

QRS Duration 1.609 [10] 6.354 [10]

PR Interval 1.043 [10] 6.747 [10]

QT Interval 0.602 [25] 9.669 [30]

____________________________________________

 （-------CSE 测量数据集100个测试ECG指的是MA1_系列数据，每个记录都包含专家的测量参考值么？NO。

   M--------may  indicate "Measurement"

  A --------indicate "Artifical", 

 MA_是从原始MO1_每个记录中选取一个心拍后重复此心拍的人造品。

）

 2.2. Rhythm analysis ----------------节律分析

The approach to rhythm analysis remains as before (8) in that three leads are used. These are II, V1 and a third lead selected from limb leads, usually the one with the largest P wave amplitude in the case of sinus rhythm.The basic rhythm strategy is to determine
 a dominant rhythm such as sinus rhythm or atrial fibrillation and thereafter determine any supplementary abnormalities such as first degree AV block or ventricular extrasystoles.

节律分析方法依然像以前一样（参考文献8），使用三个导联进行分析。它们是II，V1 导联和从肢体导联选出的第三个导联，通常是窦性节律情况下具有最大P波幅度的导联。基本的节律分析策略是确定主导节律，例如窦性节律或房颤，然后确定更细致的异常作为补充，例如一度房室阻滞或室性期前收缩。



A significant amount of work was done on the use of neural networks to attempt to improve the accuracy of determining atrial fibrillation (9) but ultimately it was found that deterministic methods were equally acceptable.Differentiation of atrial fibrillation
 with rapid ventricular

response from sinus tachycardia with frequent supra VES still remains a difficult problem for automated techniques.

做了大量的工作尝试用神经网络来提高确定房颤的准确度，但是最终发现确定性的方法同样可接受。区分房颤伴快速心室率 和 窦性心动过速带有频发室上性期前收缩 仍然是自动技术的一个难题。

（---------房颤时节律不规整，心率快，容易看成是窦性过速加室上性早搏。更直观地，可以看一个例子：[http://www.ecglibrary.com/af_fast.html](http://www.ecglibrary.com/af_fast.html)

）

 Relatively recently, newer methods for enhancement of reporting atrial flutter were reported by the group (10).While logic for detection of saw tooth waves has always been present, the more recent logic adopted a threshold crossing technique combined with
 regularity of intervals between peaks resulting in an improvement in the sensitivity of reporting atrial flutter from 27% to 79%,with a specificity exceeding 98% in both cases.

 最近，研究组（10）报告了更新的改进的房扑检测方法。检测锯齿波的逻辑层出不穷，最近的检测逻辑采用阈值技术结合波峰之间间期的规整性，改进后报告房扑的敏感度从27%提高到79%，特异性两种情况下都是超过98%。



2.3. Diagnostic interpretation        ------------诊断解释



The diagnostic component of the software is capable of using age, sex, race, clinical classification and drug therapy within its logic. Experience has shown,however, that many staff, particularly nursing staff, will simply not take the time to input the appropriate
 measures

to the software, even the age and sex of a patient which it is known are fundamental to accurate interpretation.

软件的诊断组件在它的逻辑中能利用年龄、性别、种族、临床分类和治疗用药信息。经验显示，很多工作人员，特别是护理人员，不会花时间输入这些信息，包括病人年龄和性别这些用来准确解释的基本信息。



The basic approach to interpretation is through the use of rule based criteria, but relatively recently this approach has been enhanced in several ways. First of all,smoothing techniques were introduced (11) to try to minimise repeat variation in interpretations
 by avoiding

the use of strict thresholds between abnormal and normal. In short, instead of a step function separating normal from abnormal an exponential or even a linear function between the normal and abnormal threshold value can be used as illustrated. This is usually
 associated with a scoring technique whereby it can be seen that a small change in voltage for example results in a small change in

score. In the case of multiple parameters, more complex combination rules apply as discussed elsewhere (12).

基本的解释方法是使用以规则为基础的标准，但最近将这个方法用好几种方式进行了改进。首先，引入平滑技术（11） 通过避免使用异常和正常之间的严格的阈值 尽量减少重复解释差异。简言之，像示范的那样用正常和异常阈值之间的指数或线性函数替换区分正常和异常的阶梯函数。这通常关系到评分技术，例如电压上小的变化导致分数上小的变化。在多个参数的情形下，像其他地方讨论的那样（文献12）应用更为复杂的组合规则。



Neural networks have also been introduced for detection of abnormal Q waves. However, it was found in practice that these perform best in combination with deterministic criteria (13).

 神经网络也引入过用来检测异常Q波。然而，在实践中发现这些方法结合确定性的准则后表现最好。



Electrocardiography has not stood still in recent years and new terminology such as ST elevation myocardial infarction (STEMI) has been introduced. The software acknowledges the newer diagnoses and a significant amount of work has been done to adapt the
 output appropriately (14). Another example of newer terminology is that of the Brugada pattern of which account has to be taken (Figure 1).

 近些年心电图并没有停滞不前，新的术语例如ST抬高心肌梗塞（STEMI）被引入。软件增加了新的诊断结论，做了大量的工作来 适当地调整输出结果。新术语的另外一个例子是Brugada模式，已经在分析中考虑进来。



The software makes extensive use of age and sex of patients in reaching an interpretation. Continuous limits of normality have been introduced particularly for children and younger males while different equations for normal limits of amplitudes are used
 for males and females especially in the younger adult age ranges. To a certain extent, the race of a patient is acknowledged through lower limits of normal voltage for Chinese individuals, for example.

软件在解释心电图时大量使用病人年龄和性别信息。引入特别针对儿童和青年的连续的正常界限，尤其是 在成人年龄范围内的年轻男性和女性 使用不同的幅度正常界限方程（------成年年轻男女的界限不同）。 在一定程度上，承认病人种族的差异，例如对中国人用低一些的正常电压界限。



Finally, the software contains methods for comparison of serial tracings. Two approaches are utilised the first of which involves integrating criteria within the main logic leading to statements such as “serial changes of myocardial infarction” (15). A newer
 approach has been to add on separate logic for serial comparison, which then functions as a secondary program that is run following the

main diagnostic logic. In this case, there are advantages of having almost all serial comparison logic in the same section of code although it is perhaps an approach more favoured in North America than elsewhere. This,therefore, highlights the question of user
 choice which also applies to the style of output presentation. Two different styles are offered, one whereby explanatory reasons are printed along with a diagnostic statement and the other where a much more brief diagnostic comment is produced. The different
 styles can be compared in Figure 2 where a research style output is produced to illustrate the different approaches.

最后，软件包含比较一系列描记图的方法。有两个方法可以使用，第一个方法在主要逻辑中使用积分标准得出形如“连续的心肌梗塞改变”的表述。一个新方法添加到另外的一个连续比较逻辑中，然后作为辅助功能程序在主诊断逻辑之后运行。这种情况下，这么做的好处是几乎全部的序列比较逻辑放在代码的同一个部分，尽管它可能是一个在北美更受青睐的方法。因此，这就凸现了用户选择的问题，在输出表现风格上又有此问题。提供了两种不同的风格，一种伴随诊断表述解释原因也被打印出来，另一种产生更加简洁的诊断评价。不同的风格在图2中可进行比较，图中产生的是研究性的风格输出来演示两种不同的方法。



Finally, it should be remarked that the program has a capability of handling 15 leads and the user is at liberty to select for example V3R, V4R and V7 even although the diagnostic logic at present does not incorporate criteria from these leads. If the additional
 leads happen to be X, Y, Z leads computed from the 12-lead ECG using an inverse Dower transformation for example (16), then additional  vectorcardiographic measurements can be made and vectorcardiographic loops output.

最后，需要指出程序有处理15导的能力，用户可自由选择例如V3R，V4R，V7，尽管目前诊断逻辑中没加入这些导联的判断标准。如果额外的导联恰好是X,Y，Z，它们从12导联ECG利用逆Dower变换计算出来，那么可进行额外的心电向量图测量，心电向量环输出。



3. Discussion and conclusions  --------讨论和总结



The Uni-G program has continued to evolve over a long period of time and could still be said to be under development, given the changing fashions in medicine and the underlying fact that the 12-lead ECG still remains the most commonly used diagnostic test in
 clinical medicine despite the availability of much more complex procedures. The ECG still provides unique information which, in many ways is complementary to the newer techniques but is obtained in a much more simple and rapid fashion as demanded in many clinical
 situations.

Uni-G 程序已经连续发展很长时间并且仍在继续开发，这是考虑到医学上潮流的改变，在临床医学上尽管有更为复杂的方法可以使用 但12导联ECG仍然是最常用的诊断测试。ECG仍然提供独有的信息，在很多方面是新技术的补充，但是获取ECG非常的简单快捷，符合很多临床条件的需要。



Figure 1: Example showing Brugada pattern       ---------Brugada 心电图



Figure 2: Example showing two styles of report presentation for illustrative purposes only.

The brief format is on the right while the long format with reasons is shown on the left.

--------对比两种报告风格。



----------注意：本文检测QRS复合波时，用的是多个导联联合使用的方法



