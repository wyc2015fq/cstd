# Python简单实现表白藏头诗 - zhusongziye的博客 - CSDN博客





2018年04月04日 20:54:20[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：2632








导入数据

![](https://img-blog.csdn.net/201804042050104?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

数据分行

![](https://img-blog.csdn.net/20180404205026192?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
存在很多空行

- 
长度为24和16的行可能是正确的诗句（分别对应五言和七言）

- 
部分长度正确的行，可能是目录行，标题等，需要仔细甄别


数据清洗

![](https://img-blog.csdn.net/2018040420504531?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

203 秦川雄帝宅，函谷壮皇居。绮殿千寻起，离宫百雉馀。 24

204 连薨遥接汉，飞观迥凌虚。云日隐层阙，风烟出绮疏。 24

206 岩廊罢机务，崇文聊驻辇。玉匣启龙图，金绳披凤篆。 24

207 韦编断仍续，缥帙舒还卷。对此乃淹留，欹案观坟典。 24

209 移步出词林，停舆欣武宴。雕弓写明月，骏马疑流电。 24

210 惊雁落虚弦，啼猿悲急箭。阅赏诚多美，于兹乃忘倦。 24

212 鸣笳临乐馆，眺听欢芳节。急管韵朱弦，清歌凝白雪。 24

213 彩凤肃来仪，玄鹤纷成列。去兹郑卫声，雅音方可悦。 24

215 芳辰追逸趣，禁苑信多奇。桥形通汉上，峰势接云危。 24

216 烟霞交隐映，花鸟自参差。何如肆辙迹，万里赏瑶池。 24

数据整理
句子拆分
![](https://img-blog.csdn.net/20180404205113695?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
维度转换
![](https://img-blog.csdn.net/2018040420512932?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
取头取尾
![](https://img-blog.csdn.net/20180404205139177?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

0 秦川雄帝宅 秦 宅

1 函谷壮皇居 函 居

2 连薨遥接汉 连 汉

3 飞观迥凌虚 飞 虚

4 岩廊罢机务 岩 务
区分五言和七言
![](https://img-blog.csdn.net/20180404205150719?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




定义函数
五言藏头诗
![](https://img-blog.csdn.net/20180404205202976?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
五言藏尾
![](https://img-blog.csdn.net/20180404205217992?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
七言藏头
![](https://img-blog.csdn.net/20180404205323917?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
七言藏尾
![](https://img-blog.csdn.net/20180404205247127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

测试

Poet_Libai_5_tou()

请输入四个藏头字

天气真好

天花近足春

气骄陵上都

真耀蕴轩仪

好古笑流俗




Poet_Libai_5_wei()

请输入四个藏尾字

天气真好

广乐奏钧天

昔年怀壮气

清心常保真

不如归山好

作者：防潮同学

源自：https://zhuanlan.zhihu.com/p/33917614

声明：文章著作权归作者所有，如有侵权，请联系小编删除



