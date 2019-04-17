# 动态规划 ——  线性 DP - Alex_McAvoy的博客 - CSDN博客





2018年08月20日 14:32:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：303








# 【概述】

线性动态规划，是较常见的一类动态规划问题，其是在线性结构上进行状态转移，这类问题不像背包问题、区间DP等有固定的模板。

线性动态规划的目标函数为特定变量的线性函数，约束是这些变量的线性不等式或等式，目的是求目标函数的最大值或最小值。

因此，除了少量问题（如：LIS、LCS、LCIS等）有固定的模板外，大部分都要根据实际问题来推导得出答案。

# 【常见问题】
- 序列问题：[点击这里](https://blog.csdn.net/u011815404/article/details/88552909)
- 最大和问题：[点击这里](https://blog.csdn.net/u011815404/article/details/88552975)

# 【例题】

## 1.序列问题：
- Monkey and Banana（HDU-1069）**(LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79703841)
- 求最长不下降序列（信息学奥赛一本通-T1259）**(LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529637)
- 最长上升子序列（信息学奥赛一本通-T1289）**(LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80542410)
- 怪盗基德的滑翔翼（信息学奥赛一本通-T1286）**(LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529631)
- 登山（信息学奥赛一本通-T1283）**(LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80542422)
- 导弹拦截（洛谷-P1020）**(LIS+记忆化+二分查找)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81808802)

	相似题：拦截导弹（信息学奥赛一本通-T1260）**(LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529634)

	              拦截导弹（信息学奥赛一本通-T1289）**(LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529638)
- 合唱队形（洛谷-P1091）**(两遍LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81808803)

	同题：合唱队形（信息学奥赛一本通-T1264）：[点击这里](https://blog.csdn.net/u011815404/article/details/80529629)
- Eating Together（POJ-3670）**(两端寻找求 LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80616564)
- 友好城市（信息学奥赛一本通-T1289）**(排序+LIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529630)
- Common Subsequence（HDU-1159）**(LCS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79703677)
- 最长公共子序列（信息学奥赛一本通-T1289）**(LCS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529627)
- 回文字符串（51Nod-1092）**(LCS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/89321540)
- 公共子序列（信息学奥赛一本通-T1297）**(LCS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80588818)
- 最长公共子上升序列（信息学奥赛一本通-T1306）**(LCIS)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80609767)

## 2.求和问题
- Super Jumping! Jumping! Jumping!（HDU-1087）**(LIS 的和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79703723)
- 最大上升子序列和（信息学奥赛一本通-T1285）**(LIS 的和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80542420)
- Maximum sum（信息学奥赛一本通-T1305）**(最大子段和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80609768)
- 小a的子序列（2019牛客寒假算法基础集训营 Day1-F）**(子序列和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/88380539)
- 最低通行费（信息学奥赛一本通-T1287）**(最小和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80542417)
- 超级楼梯（HDU-2040）**(最大和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79721179)
- Max Sum（HDU-1003）**(最大和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79703612)
- Milking Time（POJ-3616）**(最大和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80589277)
- 糖果（信息学奥赛一本通-T1299）**(最大和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80588815)
- Labyrinth（HDU-4826）**(最大和+三维 DP)**：[点击这](https://blog.csdn.net/u011815404/article/details/86483328)
- 乘积最大（信息学奥赛一本通-T1275）**(最大乘积)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80574163)
- Vasya And The Mushrooms（CF-1016C）**(矩阵最大和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81667133)
- 摘花生（信息学奥赛一本通-T1284）**(矩阵最大和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80542421)
- Likecloud-吃、吃、吃（洛谷-P1508）**(最大子矩阵和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81874225)
- 最大子矩阵（信息学奥赛一本通-T1224）**(最大子矩阵和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80298641)
- 方格取数（信息学奥赛一本通-T1277）**(最大子矩阵和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80574215)
- 创意吃鱼法（洛谷-P1736）**(最大子矩阵和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81913620)
- 命运（HDU-2571）**(最大矩阵和+负数处理)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79721178)
- 最大正方形（洛谷-P1387）**(寻找最大子矩阵)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81874227)
- 最大子矩阵（信息学奥赛一本通-T1282）**(寻找最大子矩阵)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80542411)
- 数塔（HDU-2084）**(数字三角形和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79721177)
- 数字三角形（洛谷-P1216）**(数字三角形和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81808373)

	同题：三角形最佳路径问题（信息学奥赛一本通-T1288）：[点击这里](https://blog.csdn.net/u011815404/article/details/80542416)
- 数字金字塔（信息学奥赛一本通-T1258）**(数字三角形和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529638)
- 免费馅饼（HDU-1176）**(数塔问题思想)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79721180)

## 3.应用问题
- 数的划分（洛谷-P1025）**(方案数的统计)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81808366)

	同题：数的划分（信息学奥赛一本通-T1304）：[点击这里](https://blog.csdn.net/u011815404/article/details/80609770)
- 相似基因（洛谷-P1140）**(打表匹配)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81873214)
- Race（LightOJ-1326）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/88817218)
- Telephone Wire（POJ-3612）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80598657)
- 奇怪的电梯（洛谷-P1135）**(线性DP+模拟****)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81808363)

	同题：奇怪的电梯（信息学奥赛一本通-T1360）[点击这里](https://blog.csdn.net/u011815404/article/details/80629040)
- Problem Solving（POJ-3265）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80790799)
- 复制书稿（信息学奥赛一本通-T1278）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80588813)
- 编辑距离（信息学奥赛一本通-T1276）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80574187)
- 橱窗布置（信息学奥赛一本通-T1279）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80588812)
- 鸡蛋的硬度（信息学奥赛一本通-T1300）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80588814)
- 判断整除（信息学奥赛一本通-T1195）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80196554)
- 股票买卖（信息学奥赛一本通-T1302）**(线性DP+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80609771)
- 尼克的任务（洛谷-P1280）**(逆序DP****)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81873213)
- 山区建小学（信息学奥赛一本通-T1197）**(最短距离)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80196556)
- 城市交通路网（信息学奥赛一本通-T1261）**(最短距离)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529632)
- 计算字符串距离（信息学奥赛一本通-T1298）**(最短距离)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80588816)
- Crossing River（信息学奥赛一本通-T1232）**(线性DP+贪心)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80299153)
- Increasing Frequency（CF-1082E）**(区间变换)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84842955)
- 机器分配（信息学奥赛一本通-T1266）**(递归输出)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529638)
- 挖地雷（信息学奥赛一本通-T1262）**(单向最大值+递归输出)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80529631)
- 传纸条（洛谷-P1006）**(四维DP)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81874226)



