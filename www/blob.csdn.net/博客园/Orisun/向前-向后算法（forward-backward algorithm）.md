# 向前-向后算法（forward-backward algorithm） - Orisun - 博客园







# [向前-向后算法（forward-backward algorithm）](https://www.cnblogs.com/zhangchaoyang/articles/2220398.html)





本文承接上篇博客《[隐马尔可夫模型及的评估和解码问题](http://www.cnblogs.com/zhangchaoyang/articles/2219571.html)》，用到的概念和例子都是那里面的。

**学习问题**

在HMM模型中，已知隐藏状态的集合S，观察值的集合O，以及一个观察序列（o1,o2,...,on），求使得该观察序列出现的可能性最大的模型参数（包括初始状态概率矩阵π，状态转移矩阵A，发射矩阵B）。这正好就是[EM算法](http://www.cnblogs.com/zhangchaoyang/articles/2623364.html)要求解的问题：已知一系列的观察值X，在隐含变量Y未知的情况下求最佳参数θ*，使得：

![](https://pic002.cnblogs.com/images/2012/103496/2012080423285592.png)

在中文词性标注里，根据为训练语料，我们观察到了一系列的词（对应EM中的X），如果每个词的词性（即隐藏状态）也是知道的，那它就不需要用EM来求模型参数θ了，因为Y是已知的，不存在隐含变量了。当没有隐含变量时，直接用maximum likelihood就可以把模型参数求出来。

**预备知识**

首先你得对下面的公式表示认同。

以下都是针对相互独立的事件，

P(A,B)=P(B|A)*P(A)

P(A,B,C)=P(C)*P(A,B|C)=P(A,C|B)*P(B)=P(B,C|A)*P(A)

P(A,B,C,D)=P(D)*P(A,B|D)*P(C|A)=P(D)*P(A,B|D)*P(C|B)

P(A,B|C)=P(D1,A,B|C)+P(D2,A,B|C)     D1,D2是事件D的一个全划分

理解了上面几个式子，你也就能理解本文中出现的公式是怎么推导出来的了。

### EM算法求解

我们已经知道如果隐含变量Y是已知的，那么求解模型参数直接利用Maximum Likelihood就可以了。EM算法的基本思路是：随机初始化一组参数θ(0)，根据后验概率Pr(Y|X;θ)来更新Y的期望E(Y)，然后用E(Y)代替Y求出新的模型参数θ(1)。如此迭代直到θ趋于稳定。

在HMM问题中，隐含变量自然就是状态变量，要求状态变量的期望值，其实就是求时刻ti观察到xi时处于状态si的概率，为了求此概率，需要用到向前变量和向后变量。

**向前变量**

向前变量 是假定的参数

它表示t时刻满足状态$S_i$，且t时刻之前（包括t时刻）满足给定的观测序列$(O_{1}O_{2}{\cdots}O_t)$的概率。
- 令初始值$$\alpha_1(i)=\pi_ib_i(O_1)$$
- 归纳法计算$$\alpha_{t+1}(j)=\left[\sum_{i=1}^N{\alpha_t(i)a_{ij}}\right]b_j(O_{t+1})$$
- 最后计算$$P(O|\lambda)=\sum_{i=1}^N{\alpha_T(i)}$$

复杂度$O(N^2T)$

**向后变量**


向后变量




$$\beta_t(i)=p(O_{t+1}O_{t+2}{\ldots}O_T|q_t=S_i\;\lambda),1{\le}t{\le}T\;\;1{\le}i{\le}N$$



它表示在时刻t出现状态$S_i$，且t时刻以后的观察序列满足$(O_{t+1}O_{t+2}{\cdots}O_T)$的概率。

- 初始值$\beta_T(i)=1$
- 归纳计算$$\beta_t(i)=\sum_{j=1}^N{a_{ij}b_j(O_{t+1})\beta_{t+1}(j)}$$

**E-Step**

定义变量$\xi_t(i,j)$为t时刻处于状态i，t+1时刻处于状态j的概率。



$$\xi_t(i,j)=p(q_t=S_i,q_{t+1}=S_j|O;\lambda),1{\le}t{\le}T-1$$

![](https://pic002.cnblogs.com/images/2011/103496/2011102210085266.jpg)

定义变量$\gamma_t(i)$表示t时刻呈现状态i的概率。

$\gamma_t(i)=p(q_t=S_i|O;\lambda)$

实际上$$\gamma_t(i)=\sum_{j=1}^N\xi_t(i,j),1{\le}t{\le}T-1$$       

$$\gamma_t(i)=\frac{\alpha_t(i)\beta_t(i)}{\sum_{i=1}^N{\alpha_t(i)\beta_t(i)}},1{\le}t{\le}T$$ 

$\sum_{t=1}^{T}\gamma_t(i)$是从其他所有状态转移到状态i的次数的期望值。

$\sum_{t=1}^{T-1}\gamma_t(i)$是从状态i转移出去的次数的期望值。

$\sum_{t=1}^{T-1}\xi_t(i,j)$是从状态i转移到状态j的次数的期望值。

**M-Step**


$\overline{\pi_i}$是在初始时刻出现状态i的频率的期望值，$\overline{\pi_i}=\gamma_1(i)$

$\overline{\alpha}_{ij}$是从状态i转移到状态j的次数的期望值  **除以**  从状态i转移出去的次数的期望值，$$\overline{a}_{ij}=\frac{\sum_{t=1}^{T-1}\xi_t(i,j)}{\sum_{t=1}^{T-1}\gamma_t(i)}$$

$\overline{b}_j(k)$是在状态j下观察到活动为k的次数的期望值  **除以**  从其他所有状态转移到状态j的次数的期望值,$$\overline{b}_j(k)=\frac{\sum_{t=1,s.t.O_t=k}^T\gamma_t(j)}{\sum_{t=1}^T\gamma_t(j)}$$

然后用新的参数$(\Pi,A,B)$再来计算向前变量、向后变量、$\xi_t(i,j)$和$\gamma_t(i)$。如此循环迭代，直到前后两次参数的变化量小于某个值为止。

下面给出我的java代码：


```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


  1 import java.io.BufferedReader;
  2 import java.io.File;
  3 import java.io.FileReader;
  4 import java.io.IOException;
  5 import java.util.Arrays;
  6 import java.util.HashMap;
  7 import java.util.LinkedList;
  8 import java.util.List;
  9 import java.util.Map;
 10 import java.util.Map.Entry;
 11 
 12 /**
 13  * 隐马尔可夫模型参数学习。
 14  *
 15  * @Author:zhangchaoyang
 16  * @Since:2015年4月4日
 17  * @Version:1.0
 18  */
 19 public class HmmLearn {
 20 
 21     private int stateCount;// 状态的个数
 22     private Map<String, Integer> observeIndexMap = new HashMap<String, Integer>();// 观察值及其索引编号
 23     /**
 24      * 通过学习得到的模型参数
 25      */
 26     private double[] stateProb;// 初始状态概率矩阵
 27     private double[][] stateTrans;// 状态转移矩阵
 28     private double[][] emission;// 混淆矩阵
 29 
 30     private List<String> observeSeqs = new LinkedList<String>();// 训练集中所有的观察序列
 31 
 32     /**
 33      * 迭代终止条件
 34      */
 35     private final int ITERATION_MAX = 100;
 36     private final double DELTA_PI = 1E-3;
 37     private final double DELTA_A = 1E-2;
 38     private final double DELTA_B = 1E-2;
 39 
 40     /**
 41      * 
 42      * @param stateCount
 43      *            指定状态取值有多少种
 44      * @param observeFile
 45      *            存储观察序列的文件，各个观察序列用空白符或换行符隔开即可
 46      * @throws IOException
 47      */
 48     public void initParam(int stateCount, String observeFile)
 49             throws IOException {
 50         this.stateCount = stateCount;
 51         int observeCount = 0;
 52         BufferedReader br = new BufferedReader(new FileReader(new File(
 53                 observeFile)));
 54         String line = null;
 55         while ((line = br.readLine()) != null) {
 56             String[] arr = line.split("\\s+");
 57             for (String seq : arr) {
 58                 if (seq.length() > 1) {// 长度为1的观察序列必须过滤掉，不然在更新stateTrans时会出现NaN的情况
 59                     observeSeqs.add(seq);
 60                     for (int i = 0; i < seq.length(); i++) {
 61                         String observe = seq.substring(i, i + 1);
 62                         if (!observeIndexMap.containsKey(observe)) {
 63                             observeIndexMap.put(observe, observeCount++);
 64                         }
 65                     }
 66                 }
 67             }
 68         }
 69         br.close();
 70 
 71         stateProb = new double[stateCount];
 72         initWeightRandomly(stateProb, 1E5);
 73         // initWeightEqually(stateProb);
 74         stateTrans = new double[stateCount][];
 75         for (int i = 0; i < stateCount; i++) {
 76             stateTrans[i] = new double[stateCount];
 77             initWeightRandomly(stateTrans[i], 1E5);
 78             // initWeightEqually(stateTrans[i]);
 79         }
 80         emission = new double[stateCount][];
 81         for (int i = 0; i < stateCount; i++) {
 82             emission[i] = new double[observeCount];
 83             initWeightRandomly(emission[i], 1E9);
 84             // initWeightEqually(emission[i]);
 85         }
 86     }
 87 
 88     /**
 89      * 随机地初始化权重，使得各权重非负，且和为1.
 90      * 
 91      * @param arr
 92      * @param precision
 93      */
 94     public void initWeightRandomly(double[] arr, double precision) {
 95         int len = arr.length - 1;
 96         int[] position = new int[len];
 97         for (int i = 0; i < len; i++) {
 98             position[i] = (int) (Math.random() * precision);
 99         }
100         Arrays.sort(position);
101         int pre = 0;
102         for (int i = 0; i < len; i++) {
103             arr[i] = 1.0 * (position[i] - pre) / precision;
104             pre = position[i];
105         }
106         arr[len] = 1.0 * (precision - pre) / precision;
107     }
108 
109     /**
110      * 均等地初始化权重，使得各权重非负，且和为1.
111      * 
112      * @param arr
113      */
114     public void initWeightEqually(double[] arr) {
115         int len = arr.length;
116         for (int i = 0; i < len; i++) {
117             arr[i] = 1.0 / len;
118         }
119     }
120 
121     /**
122      * BaumWelch算法学习HMM的模型参数
123      */
124     public void baumWelch() {
125         long begin = System.currentTimeMillis();
126         int iter = 0;
127         while (iter++ < ITERATION_MAX) {
128             double[] stateProb_new = new double[stateCount];
129             double[][] stateTrans_new = new double[stateCount][];
130             double[][] emission_new = new double[stateCount][];
131             for (int i = 0; i < stateCount; i++) {
132                 stateTrans_new[i] = new double[stateCount];
133             }
134             for (int i = 0; i < stateCount; i++) {
135                 emission_new[i] = new double[observeIndexMap.size()];
136             }
137             for (String seq : observeSeqs) {
138                 int T = seq.length();
139                 double[][] alpha = new double[T][];
140                 double[][] beta = new double[T][];
141                 double[][] gamma = new double[T][];
142                 for (int i = 0; i < T; i++) {
143                     alpha[i] = new double[stateCount];
144                     beta[i] = new double[stateCount];
145                     gamma[i] = new double[stateCount];
146                 }
147                 double[][][] xi = new double[T - 1][][];
148                 for (int i = 0; i < T - 1; i++) {
149                     xi[i] = new double[stateCount][];
150                     for (int j = 0; j < stateCount; j++) {
151                         xi[i][j] = new double[stateCount];
152                     }
153                 }
154                 String observe = seq.substring(0, 1);
155                 int observeIndex = observeIndexMap.get(observe);
156                 // 计算alpha
157                 for (int i = 0; i < stateCount; i++) {
158                     alpha[0][i] = stateProb[i] * emission[i][observeIndex];
159                 }
160                 for (int t = 1; t < T; t++) {
161                     observe = seq.substring(t, t + 1);// 当前时刻的观察值
162                     observeIndex = observeIndexMap.get(observe);
163                     for (int j = 0; j < stateCount; j++) {
164                         double sum = 0;
165                         for (int i = 0; i < stateCount; i++) {
166                             sum += alpha[t - 1][i] * stateTrans[i][j];
167                         }
168                         alpha[t][j] = sum * emission[j][observeIndex];
169                     }
170                 }
171                 // 计算beta
172                 for (int i = 0; i < stateCount; i++) {
173                     beta[T - 1][i] = 1;
174                 }
175                 for (int t = T - 2; t >= 0; t--) {
176                     observe = seq.substring(t + 1, t + 2);// 当前时刻的后一个观察值
177                     observeIndex = observeIndexMap.get(observe);
178                     for (int i = 0; i < stateCount; i++) {
179                         double sum = 0;
180                         for (int j = 0; j < stateCount; j++) {
181                             sum += beta[t + 1][j] * stateTrans[i][j]
182                                     * emission[j][observeIndex];
183                         }
184                         beta[t][i] = sum;
185                     }
186                 }
187                 double[] denominator = new double[T];// 在计算gamma和xi时都要计算的一个变量
188                 // 计算gamma
189                 for (int t = 0; t < T; t++) {
190                     double sum = 0;
191                     for (int j = 0; j < stateCount; j++) {
192                         sum += alpha[t][j] * beta[t][j];
193                     }
194                     denominator[t] = sum;
195                     for (int i = 0; i < stateCount; i++) {
196                         gamma[t][i] = alpha[t][i] * beta[t][i] / sum;
197                     }
198                 }
199                 // 计算xi
200                 for (int t = 0; t < T - 1; t++) {
201                     observe = seq.substring(t + 1, t + 2);// 当前时刻的后一个观察值
202                     observeIndex = observeIndexMap.get(observe);
203                     for (int i = 0; i < stateCount; i++) {
204                         for (int j = 0; j < stateCount; j++) {
205                             xi[t][i][j] = alpha[t][i] * stateTrans[i][j]
206                                     * beta[t + 1][j]
207                                     * emission[j][observeIndex]
208                                     / denominator[t];
209                         }
210                     }
211                 }
212                 // 计算stateProb
213                 double[] curr_stateProb = new double[stateCount];
214                 for (int i = 0; i < stateCount; i++) {
215                     curr_stateProb[i] = gamma[0][i];
216                     stateProb_new[i] += curr_stateProb[i];
217                 }
218                 // 计算stateTrans
219                 double[][] curr_stateTrans = new double[stateCount][];
220                 for (int i = 0; i < stateCount; i++) {
221                     curr_stateTrans[i] = new double[stateCount];
222                     for (int j = 0; j < stateCount; j++) {
223                         double up = 0;
224                         double down = 0;
225                         for (int t = 0; t < T - 1; t++) {
226                             up += xi[t][i][j];
227                             down += gamma[t][i];
228                         }
229                         if (down > 0) {
230                             curr_stateTrans[i][j] = up / down;// 任何浮点操作，只要它的一个或多个操作数为NaN，其结果就是NaN。NaN不等于任何浮点数，包括它自身在内。
231                             stateTrans_new[i][j] += curr_stateTrans[i][j];
232                         } else {
233                             stateTrans_new[i][j] += stateTrans[i][j];// 如果分母为0无法相除，就找个值来替代
234                             System.err.println("up=" + up + ",down=" + down);
235                         }
236                     }
237                 }
238                 // 计算emission
239                 double[][] curr_emission = new double[stateCount][];
240                 for (int i = 0; i < stateCount; i++) {
241                     curr_emission[i] = new double[observeIndexMap.size()];
242                     for (Entry<String, Integer> entry : observeIndexMap
243                             .entrySet()) {
244                         String obs = entry.getKey();
245                         int index = entry.getValue();
246                         double up = 0;
247                         double down = 0;
248                         for (int t = 0; t < T; t++) {
249                             observe = seq.substring(t, t + 1);
250                             if (obs.equals(observe)) {
251                                 up += gamma[t][i];
252                             }
253                             down += gamma[t][i];
254                         }
255                         curr_emission[i][index] = up / down;
256                         emission_new[i][index] += curr_emission[i][index];
257                     }
258                 }
259             }
260             // 批量更新模型参数
261             double delta_pi = 0;
262             double delta_a = 0;
263             double delta_b = 0;
264             int seqCount = observeSeqs.size();
265             for (int i = 0; i < stateCount; i++) {
266                 stateProb_new[i] /= seqCount;
267                 delta_pi += Math.abs(stateProb_new[i] - stateProb[i]);
268             }
269             for (int i = 0; i < stateCount; i++) {
270                 for (int j = 0; j < stateCount; j++) {
271                     stateTrans_new[i][j] /= seqCount;
272                     delta_a += Math
273                             .abs(stateTrans_new[i][j] - stateTrans[i][j]);
274                 }
275             }
276             for (int i = 0; i < stateCount; i++) {
277                 for (int j = 0; j < observeIndexMap.size(); j++) {
278                     emission_new[i][j] /= seqCount;
279                     delta_b += Math.abs(emission_new[i][j] - emission[i][j]);
280                 }
281             }
282             System.out.println("iteration " + iter + ", delta_pi=" + delta_pi
283                     + ", delta_a=" + delta_a + ", delta_b=" + delta_b);
284             if (delta_pi <= DELTA_PI && delta_a <= DELTA_A
285                     && delta_b <= DELTA_B) {
286                 break;
287             } else {
288                 stateProb = stateProb_new;
289                 stateTrans = stateTrans_new;
290                 emission = emission_new;
291             }
292         }
293         long end = System.currentTimeMillis();
294         System.out.println("time elapse " + (end - begin) / 1000 + " seconds.");
295     }
296 
297     public int getStateCount() {
298         return stateCount;
299     }
300 
301     public double[] getStateProb() {
302         return stateProb;
303     }
304 
305     public double[][] getStateTrans() {
306         return stateTrans;
307     }
308 
309     public double[][] getEmission() {
310         return emission;
311     }
312 
313     public Map<String, Integer> getObserveIndexMap() {
314         return observeIndexMap;
315     }
316 
317 }


View Code
```

测试代码：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


  1 import java.io.BufferedWriter;
  2 import java.io.File;
  3 import java.io.FileWriter;
  4 import java.io.IOException;
  5 import java.util.ArrayDeque;
  6 import java.util.ArrayList;
  7 import java.util.Deque;
  8 import java.util.LinkedList;
  9 import java.util.List;
 10 import java.util.Map;
 11 import java.util.Map.Entry;
 12 
 13 import org.junit.Assert;
 14 import org.junit.BeforeClass;
 15 import org.junit.Test;
 16 
 17 import xxzl.dm.core.sequence.HmmInference;
 18 import xxzl.dm.core.sequence.HmmLearn;
 19 import xxzl.dm.utility.FileUtil;
 20 import xxzl.dm.utility.Pair;
 21 
 22 public class TestHmmLearn {
 23 
 24     private static String corpusPath;
 25     private static String modelPath;
 26 
 27     @BeforeClass
 28     public static void setup() throws IOException {
 29         corpusPath = TestHmmLearn.class.getResource("/").getPath()
 30                 + "/resources/corpus/";
 31         modelPath = TestHmmLearn.class.getResource("/").getPath()
 32                 + "/resources/model/hmm/";
 33     }
 34 
 35     /**
 36      * 先通过BaumWelch算法去学习模型参数,并把学习到的参数写入文件
 37      */
 38     @Test
 39     public void printParam() throws IOException {
 40         HmmLearn hmmLearn = new HmmLearn();
 41         hmmLearn.initParam(4, corpusPath + "msr_test");
 42         hmmLearn.baumWelch();
 43         double[] PI = hmmLearn.getStateProb();
 44         double[][] A = hmmLearn.getStateTrans();
 45         double[][] B = hmmLearn.getEmission();
 46         int stateCount = PI.length;
 47         List<String> stateSet = new ArrayList<String>();
 48         for (int i = 0; i < stateCount; i++) {
 49             stateSet.add(getLabelName(i + 1));
 50         }
 51         FileUtil.writeLines(modelPath + "state", stateSet);
 52         List<String> observeSet = new LinkedList<String>();
 53         Map<String, Integer> observeIndex = hmmLearn.getObserveIndexMap();
 54         for (Entry<String, Integer> entry : observeIndex.entrySet()) {
 55             observeSet.add(entry.getKey());
 56         }
 57         FileUtil.writeLines(modelPath + "observe", observeSet);
 58         List<String> stateProb = new ArrayList<String>();
 59         for (double pi : PI) {
 60             stateProb.add(String.valueOf(pi));
 61         }
 62         FileUtil.writeLines(modelPath + "PI", stateProb);
 63         BufferedWriter bw = new BufferedWriter(new FileWriter(new File(
 64                 modelPath + "A")));
 65         for (int i = 0; i < A.length; i++) {
 66             for (int j = 0; j < A[i].length; j++) {
 67                 bw.write(A[i][j] + "\t");
 68             }
 69             bw.newLine();
 70         }
 71         bw.close();
 72         bw = new BufferedWriter(new FileWriter(new File(modelPath + "B")));
 73         for (int i = 0; i < B.length; i++) {
 74             for (int j = 0; j < B[i].length; j++) {
 75                 bw.write(B[i][j] + "\t");
 76             }
 77             bw.newLine();
 78         }
 79         bw.close();
 80     }
 81 
 82     /**
 83      * 再用学习好的参数去做inference
 84      */
 85     @Test
 86     public void testInference() {
 87         HmmInference hmmInference = new HmmInference();
 88         hmmInference.initStateSet(modelPath + "state");
 89         hmmInference.initObserveSet(modelPath + "observe");
 90         hmmInference.initStateProb(modelPath + "PI");
 91         hmmInference.initStateTrans(modelPath + "A");
 92         hmmInference.initConfusion(modelPath + "B");
 93         String sentence = "他们的成长与优越的家庭背景没有任何正相关的关系";
 94         List<String> obs_seq = str2List(sentence);
 95         Pair<Double, LinkedList<String>> states = hmmInference.viterbi(obs_seq);
 96         System.out.println("最可能的状态序列：" + states.second + "，其概率为："
 97                 + states.first);
 98         double ratio = hmmInference.estimate(obs_seq);
 99         System.out.println("句子1出现的概率：" + ratio);
100         sentence = "为报倾城随太守亲射虎看孙郎会挽雕弓如满月西北望";// 要保证两个句子长度相等，比较其出现概率才有意义
101         obs_seq = str2List(sentence);
102         double ratio2 = hmmInference.estimate(obs_seq);
103         System.out.println("句子2出现的概率：" + ratio2);
104         Assert.assertTrue(ratio > ratio2);
105     }
106 
107     /**
108      * 按照Excel的规则，给一个列号，给出列名称。比如1对应A，26对应Z，27对应AA，53对应BA
109      * 
110      * @param a
111      * @return
112      */
113     private static String getLabelName(int a) {
114         StringBuilder sb = new StringBuilder();
115         char[] arr = new char[26];
116         arr[0] = 'A';
117         for (int i = 1; i < 26; i++) {
118             arr[i] = (char) (arr[i - 1] + 1);
119         }
120         Deque<String> deque = new ArrayDeque<String>();
121         while (a > 0) {
122             char c = arr[(a - 1) % 26];
123             deque.push(String.valueOf(c));
124             a = (a - 1) / 26;
125         }
126         while (deque.size() > 0) {
127             sb.append(deque.pop());
128         }
129         return sb.toString();
130     }
131 
132     private List<String> str2List(String sentence) {
133         List<String> obs_seq = new ArrayList<String>();
134         int i = 0;
135         while (i < sentence.length()) {
136             obs_seq.add(sentence.substring(i, i + 1));
137             i++;
138         }
139         return obs_seq;
140     }
141 }


View Code
```

 其中用到的类HmmInference原代码在[上篇博客](http://www.cnblogs.com/zhangchaoyang/articles/2219571.html)中。













