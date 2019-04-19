# 基于LBS的六边形热力图算法 - 毛小亮 - 博客园
# [基于LBS的六边形热力图算法](https://www.cnblogs.com/xianerwonder/p/6474903.html)
**六边形算法：**
　　我把六边形铺满的分布图进行了切分，切分为矩形，每个矩形中有一个六边形、4个三角形、两个小长方形，依次计算。边界判断上，采用主流的MP>MN的方式（M为上边界对称点，N为与六边形的交点，p为要判断的点，如下图）。
![](https://images2015.cnblogs.com/blog/688944/201702/688944-20170227161416032-1569916507.png)
**实际效果（双色对比）：**
![](https://images2015.cnblogs.com/blog/688944/201702/688944-20170227162943423-1319602500.png)
**拾取效果：**
![](https://images2015.cnblogs.com/blog/688944/201702/688944-20170227161658626-429610935.gif)
**代码：**
```
1 /// <summary>
  2         /// 根据参数获得六边形索引
  3         /// </summary>
  4         /// <param name="width">页面宽度</param>
  5         /// <param name="height">页面长度</param>
  6         /// <param name="r">六边形边长</param>
  7         /// <param name="thickness">六边形上下拓扑半间距</param>
  8         /// <param name="flat">扁平化程度（由于计算过程中，thickness在侧向拓扑中作用减小，故增加了这个系数）</param>
  9         /// <param name="x">横坐标</param>
 10         /// <param name="y">纵坐标</param>
 11         /// <param name="transparent">透明度设置，一般位置为正常值；如果是六边形边界位置的过渡色，减半</param>
 12         /// <returns></returns>
 13         public static int[] GetHexGridIndex(int width, int height, int r, int thickness, int flat, int x, int y, ref byte transparent)
 14         {
 15             //中心点索引（长方形中的六边形的索引）
 16             int indexX = (x / 3) / r * 2;
 17             int indexY = (int)((int)(y / Math.Sqrt(3)) / r + indexX / 2);
 18 
 19             //中心点坐标
 20             int centerX = r + (int)(1.5 * r * indexX);
 21             int centerY = (int)(Math.Sqrt(3) * 0.5 * r * (1 + 2 * indexY - indexX));
 22 
 23             //位于六边外框之外
 24             if (x > (centerX + r))
 25             {
 26                 if (y > centerY + thickness)//右下
 27                 {
 28                     indexX++;
 29                     indexY++;
 30                 }
 31                 else if (y < centerY - thickness)//右上
 32                 {
 33                     indexX++;
 34                 }
 35                 else//中间部分以及外延部分
 36                 {
 37                     indexX = -1;
 38                     indexY = -1;
 39                 }
 40             }
 41             else//六边外框之内
 42             {
 43                 if (x > centerX + r / 2)//右半区
 44                 {
 45                     if (y > centerY)//右下
 46                     {
 47                         var M = Math.Sqrt(3) / 2 * r + centerY;//上边界点
 48                         var N = (centerX + r - x) * Math.Sqrt(3) + centerY;//六边形边界点
 49                         var MP = (int)(M - y);
 50                         var MN = (int)(M - N);
 51                         if (MP > (MN + thickness * flat))//六边形内部
 52                         {
 53                             //索引不变
 54                         }
 55                         else if (MP < (MN - thickness * flat))//六边形外部
 56                         {
 57                             indexX++;
 58                             indexY++;
 59                         }
 60                         else//交界区
 61                         {
 62                             if (MP == MN + thickness * flat)//紧靠内部
 63                             {
 64                                 transparent = (byte)(transparent / 2);
 65                                 ////去除右侧尖尖
 66                                 if (x == centerX + r - thickness)
 67                                 {
 68                                     indexX = -1;
 69                                     indexY = -1;
 70                                 }
 71                             }
 72                             else if (MP == MN - thickness * flat)//紧靠外部
 73                             {
 74                                 indexX++;
 75                                 indexY++;
 76                                 transparent = (byte)(transparent / 2);
 77                             }
 78                             else//中间区
 79                             {
 80                                 indexX = -1;
 81                                 indexY = -1;
 82                             }
 83                         }
 84                     }
 85                     else//右上
 86                     {
 87                         var M = centerY - Math.Sqrt(3) / 2 * r;
 88                         var N = centerY - (centerX + r - x) * Math.Sqrt(3);
 89                         var MP = (int)(y - M);
 90                         var MN = (int)(N - M);
 91                         if (MP > (MN + thickness * flat))//内部
 92                         {
 93                             ////索引不变，但是要去除右侧尖尖
 94                             if (x == centerX + r - thickness)
 95                             {
 96                                 indexX = -1;
 97                                 indexY = -1;
 98                             }
 99                         }
100                         else if (MP < (MN - thickness * flat))//外部,索引单变
101                         {
102                             indexX++;
103                         }
104                         else
105                         {
106                             if (MP == MN + thickness * flat)//里侧
107                             {
108                                 transparent = (byte)(transparent / 2);
109                                 if (x == centerX + r - thickness - 1)////去除右侧尖尖
110                                 {
111                                     indexX = -1;
112                                     indexY = -1;
113                                 }
114                             }
115                             else if (MP == MN - thickness * flat)//外侧
116                             {
117                                 indexX++;
118                                 transparent = (byte)(transparent / 2);
119                             }
120                             else
121                             {
122                                 indexX = -1;
123                                 indexY = -1;
124                             }
125                         }
126                     }
127                 }
128                 else if (x < centerX - r / 2)//左半区
129                 {
130                     if (y < centerY)//左上
131                     {
132                         var M = centerY - Math.Sqrt(3) / 2 * r;
133                         var N = centerY + (centerX - r - x) * Math.Sqrt(3);
134                         var MP = (int)(y - M);
135                         var MN = (int)(N - M);
136                         if (MP > (MN + thickness * flat))
137                         {
138                             //索引不变
139                         }
140                         else if (MP < (MN - thickness * flat))//索引单变
141                         {
142                             indexX--;
143                             indexY--;
144                         }
145                         else
146                         {
147                             if (MP == MN + thickness * flat)//里侧
148                             {
149                                 transparent = (byte)(transparent / 2);
150                                 if (x == centerX - r + thickness + 1)
151                                 {
152                                     indexX = -1;
153                                     indexY = -1;
154                                 }
155                             }
156                             else if (MP == MN - thickness * flat)//外侧
157                             {
158                                 indexX--;
159                                 indexY--;
160                                 transparent = (byte)(transparent / 2);
161                             }
162                             else
163                             {
164                                 indexX = -1;
165                                 indexY = -1;
166                             }
167                         }
168                     }
169                     else//左下
170                     {
171                         var M = centerY + Math.Sqrt(3) / 2 * r;
172                         var N = centerY - (centerX - r - x) * Math.Sqrt(3);
173                         var MP = (int)(M - y);
174                         var MN = (int)(M - N);
175                         if (MP > (MN + thickness * flat))//内部
176                         {
177                             if (x == centerX - r + thickness + 1)//索引不变，但是要清除突兀部分
178                             {
179                                 indexX = -1;
180                                 indexY = -1;
181                             }
182                         }
183                         else if (MP < (MN - thickness * flat))//索引单变
184                         {
185                             indexX--;
186                         }
187                         else//隔离带
188                         {
189                             if (MP == MN + thickness * flat)//里侧
190                             {
191                                 transparent = (byte)(transparent / 2);
192                                 if (x < centerX - r + thickness * flat - 1)
193                                 {
194                                     indexX = -1;
195                                     indexY = -1;
196                                 }
197                             }
198                             else if (MP == MN - thickness * flat)//外侧
199                             {
200                                 indexX--;
201                                 transparent = (byte)(transparent / 2);
202                             }
203                             else
204                             {
205                                 indexX = -1;
206                                 indexY = -1;
207                             }
208                         }
209                     }
210                 }
211                 else//六边形竖条内部
212                 {
213                     var step = (int)Math.Round(Math.Sqrt(3) * r);
214                     var remainder = y % step;
215 
216                     if (remainder <= thickness)//上缓冲
217                     {
218                         indexX = -1;
219                         indexY = -1;
220                     }
221                     else if (step - remainder - 1 <= thickness)//下缓冲
222                     {
223                         indexX = -1;
224                         indexY = -1;
225                     }
226 
227                     if ((remainder - 1 == thickness) || (step - remainder - 2 == thickness))//六边形上下四圆角控制（分为上下半身）
228                     {
229                         //去除四点
230                         var first = (int)(centerX - r / 2);
231                         var second = (int)Math.Ceiling((double)centerX - r / 2);
232                         var third = (int)(centerX + r / 2);
233                         var fourth = (int)Math.Ceiling((double)centerX + r / 2);
234 
235                         if (x == first || x == second || x == third || x == fourth)
236                         {
237                             indexX = -1;
238                             indexY = -1;
239                         }
240                     }
241                 }
242             }
243 
244             //right超出界面的设置
245             var rightIndex = Math.Floor(Convert.ToDouble(width - r - r) / (3 * Convert.ToDouble(r) / 2));
246             bool xx = indexX > rightIndex;
247             if (indexX > rightIndex)
248             {
249                 indexX = -1;
250                 indexY = -1;
251             }
252 
253             //bottom超出界面的设置
254             var bottomIndex = Math.Floor(height / (r * Math.Sqrt(3))) - 1;
255             //基础位置为（0，bottomIndex+1）
256             if (indexY > bottomIndex)//&&((indexX-0)%(indexY-(bottomIndex+1))==0))
257             {
258                 //下分三种情况，第一种是首索引
259                 var isOrNotFirstIndex = indexX == 0 && (indexY == bottomIndex + 1);
260                 //与首索引同行位置的索引
261                 var isOrNotFirstLineIndex = indexX == (indexY - (bottomIndex + 1)) * 2;
262                 //侧移索引
263                 var isOrNotSideSwayIndex = (indexX + 1) == (indexY - (bottomIndex + 1)) * 2;
264 
265                 if (isOrNotFirstIndex || isOrNotFirstLineIndex || isOrNotSideSwayIndex)
266                 {
267                     indexX = -1;
268                     indexY = -1;
269                 }
270             }
271 
272             return new int[] { indexX, indexY };
273         }
六边形算法
```

