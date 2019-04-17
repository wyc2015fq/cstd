# 机器翻译新突破：谷歌实现完全基于attention的翻译架构 - 知乎
# 



> 
*近两年来，机器翻译的发展速度很快，从去年谷歌的神经网络颠覆性突破开始到一个月前 Facebook 的全新 CNN 翻译技术，人工智能的巴别塔正越建越高。近日，谷歌再次宣布又在机器翻译上更进了一步，实现了完全基于 attention 的 Transformer 机器翻译网络架构，并且还在 WMT 2014 的多种语言对的翻译任务上超越了之前 Facebook 的成绩，实现了新的最佳水平。*


[重磅 | 谷歌翻译整合神经网络：机器翻译实现颠覆性突破（附论文）](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650719470%26idx%3D1%26sn%3D3368dea980517ea7d7942967da2740dc%26chksm%3D871b0090b06c89863620be4e75c757940d03d8a43cd3c1d9a8309b6594c1bccd769cab193177%26scene%3D21%23wechat_redirect)
[重磅 | Facebook 提出全新 CNN 机器翻译：准确度超越谷歌而且还快九倍（已开源）](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650726462%26idx%3D1%26sn%3D144e68df0057ac69002e3927073fc540%26chksm%3D871b2440b06cad565eb98857d057cf8dbb6cbde7786da2ffb676ae76c6246090640591e76572%26scene%3D21%23wechat_redirect)

**论文：**Attention Is All You Need


论文链接：[[1706.03762] Attention Is All You Need](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.03762)

![](https://pic4.zhimg.com/v2-b6db82b78ebf76ee08cdc31023317147_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='826' height='484'></svg>)

在编码器-解码器配置中，显性序列显性转导模型（dominant sequence transduction model）基于复杂的 RNN 或 CNN。表现最佳的模型也需通过注意力机制（attention mechanism）连接编码器和解码器。我们提出了一种新型的简单网络架构——Transformer，它完全基于注意力机制，彻底放弃了循环和卷积。两项机器翻译任务的实验表明，这些模型的翻译质量更优，同时更并行，所需训练时间也大大减少。我们的模型在 WMT 2014 英语转德语的翻译任务中取得了 BLEU 得分 28.4 的成绩，领先当前现有的最佳结果（包括集成模型）超过 2 个 BLEU 分值。WMT 2014 英语转法语翻译任务中，在 8 块 GPU 上训练了 3.5 天之后，我们的模型获得了新的单模型顶级 BLEU 得分 41.0，只是目前文献中最佳模型训练成本的一小部分。我们表明 Transformer 在其他任务上也泛化很好，把它成功应用到了有大量训练数据和有限训练数据的英语组别分析上。
![](https://pic2.zhimg.com/v2-ac27e6c63b4e7d120fde894eac2e0b81_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='694' height='968'></svg>)

*图 1：转换器-模型架构*


*![](https://pic4.zhimg.com/v2-8ef2e2eb23396215155af2286d219efb_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1198' height='612'></svg>)图 2：（左）可延展的 Dot-Product 注意，（右）Multi-Head 注意保护数并行运行的注意层*


*![](https://pic3.zhimg.com/v2-00379405f6a39ae779916fe57e51f32e_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1202' height='638'></svg>)图 3：在 5、6 层编码器自注意中遵循长期依存关系的注意机制的例子。*
![](https://pic2.zhimg.com/v2-85d40b72e25377eb387d6976d886a86d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='703' height='302'></svg>)
*表2. Transformer 在 newstest2014 英语-德语和英语-法语测试中超过了此前的业内最佳水平*

谷歌研究人员表示会在未来将这次研究的成果推广到其他任务中，利用注意力机制处理其他大规模输入和输出数据，包括图片、声音和视频。此外，研究人员打算在未来将本论文训练和研究内容的代码公布。

选自[arXiv](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.03762)**机器之心编译**


