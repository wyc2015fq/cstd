# 数据打通！谷歌、微软、脸书、Twitter宣布数据传输计划 - 知乎
# 



选自the Verge，**作者： Russell Brandom，机器之心编译，参与：路、李泽南、张倩。**

> 今日，美国四家科技巨头——谷歌、微软、Facebook 和 Twitter 联合发布了一项新标准计划「数据传输计划」（Data Transfer Project），旨在为不同平台之间的数据迁移打造新的方式。在刚刚发布的谷歌开源博客中，谷歌将该项目描述为：让用户「直接将数据从一个服务迁移到另一个服务，无需下载和重新上传。」
![](https://pic3.zhimg.com/v2-fb90c9e2d0818c85153d5ae7e005e94e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='907' height='593'></svg>)
该系统当前版本支持图像、电子邮件、合同、日历和任务的数据迁移，这些数据通过谷歌、微软、Twitter、Flickr、Instagram、Remember the Milk 和 SmugMug 的公开 API 获取。其中很多种数据迁移可以使用其他方式完成，但是参与者希望该项目能够比传统 API 更加稳健、灵活。微软也在一篇相关博客中号召更多企业加入，该公司补充道：「可移植性（portability）和互操作性（interoperability）是云创新和竞争的关键。」

> 「未来，可移植性需要变得更加包容、灵活、开放。」
- 该项目的已有代码可在 GitHub 上获取：[https://github.com/google/data-transfer-project](https://link.zhihu.com/?target=https%3A//github.com/google/data-transfer-project)
- 白皮书：[https://datatransferproject.dev/](https://link.zhihu.com/?target=https%3A//datatransferproject.dev/)

其中的一些代码库包含将专用 API 转换成互操作性迁移的「adapter」，可使 Instagram 数据在 Flickr 上可用，反之亦然。在这些 adapter 中，工程师还构建了一个系统来加密传输数据，为每次数据迁移发布前向保密密钥（forward-secret key）。值得注意的是，该系统专注于一次性迁移，而不是很多 API 支持的连续互操作。

「未来，可移植性需要变得更加包容、灵活、开放。」白皮书中写道。「我们希望该项目能够连接任意两个面向公众的产品接口，以直接进行数据导入和导出。」

目前大部分代码由谷歌和微软的工程师完成，他们长期以来试图构建一个更加稳健的数据迁移系统。根据谷歌 Takeout 产品经理 Greg Fair 所述，这个想法源于数据下载后在管理方面遇到的挫折。如果没有清晰的方式将同样的数据导入不同的服务，类似 Takeout 的工具就只解决了问题的一半。

「当人们拥有数据时，他们想把数据从一个产品迁移到另一个产品，但是却无法做到。」Fair 说道，「这个问题仅凭一己之力无法真正解决。」

大部分平台已经提供数据下载工具，但是很少有工具能够与其他服务连接。欧洲最近开始执行的 GDPR 法案要求工具提供指定用户的所有可用数据，这比你从 API 中获取的数据要全面得多。比如电子邮件或照片，你会发现定位历史和人脸识别文件等棘手的数据在很多用户并不知情的时候已经被工具收集。一些项目尝试利用这些数据，最突出的是 [http://Digi.me](https://link.zhihu.com/?target=http%3A//Digi.me)，它构建了一整个 APP 生态系统；但是大部分项目只停留在用户的硬盘。下载工具被作为用户真正拥有自己数据的证据，但是拥有自己的数据和使用数据是完全是两码事。
![](https://pic2.zhimg.com/v2-b550178504f2d714a9d7bc02673bb74d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='730'></svg>)
> 「我们总是想要把用户数据安全放在第一位。」

该项目会以开源的标准开展，很多参与该项目的工程师称，如果该标准成功推广，管理方面的大幅转变将会是必要的。」长期来看，我们希望有一个业界领导者、消费者组织、政府组织组成的联盟。」Fair 称，「但是直到我们达到合理的临界点（很多组织参与该项目）之前，这仍然不是一件轻松的事。」

对于数据共享项目来说，这是一个微妙的节点。Facebook 的 API 处于「剑桥分析」丑闻的中心，业界人士仍在摸索用户应该对自己的数据抱有多大的信任。谷歌也被自己的 API 丑闻困扰，因第三方电子邮件应用程序处理 Gmail 用户数据不当而遭到强烈抗议。在某种程度上，项目中提及的联盟可以将责任分散到更多的团体，从而成为一种管控风险的方式。

「剑桥分析」丑闻带来的阴霾使得数据公司更加不愿意分享数据。当被问及新项目的数据隐私问题时，Facebook 强调了维持 API 级别控制的重要性。

「我们总是想要把用户数据安全放在第一位。」从事 Facebook 数据下载产品工作的 David Baser 表示，「API 的优点之一就是，作为数据提供商，我们可以切断供应通道或为使用者设定使用条件。而有了下载工具，数据就会脱离我们，真正进入无法管控的状态。一旦有人想利用数据做坏事，Facebook 完全无计可施。」

同时，科技公司还面临前所未有的严重反垄断担忧，许多公司的业务集中在数据获取。最大的几家科技公司几无敌手。当他们面临联邦法规和垄断力量的新问题时，分享数据可能成为痛感最小的自控方式之一。

对于饱尝数据隐私丑闻苦果的公司来说，这种做法的效果可能非常有限，但像开放技术研究所（Open Technology Institute）负责人 Kevin Bankston 这样的局外人比以往任何时候都更加强调这一点，尤其是对 Facebook 来说。「我们的主要目标一直都是确保开放性的价值不被遗忘，」Bankston 表示，「如果你对这些平台的力量怀有疑虑，可移植性可以消除这种疑虑。」
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[https://www.theverge.com/2018/7/20/17589246/data-transfer-project-google-facebook-microsoft-twitter](https://link.zhihu.com/?target=https%3A//www.theverge.com/2018/7/20/17589246/data-transfer-project-google-facebook-microsoft-twitter)


