# 一个22万张NSFW图片的鉴黄数据集？我有个大胆的想法…… - 知乎
# 



> 如果你想训练一个内容审核系统过滤不合适的信息，或用 GAN 做一些大胆的新想法，那么数据集是必不可少的。例如图像鉴黄，我们需要使用卷积神经网络训练一个分类器，以区分正常图像与限制级图像。但限制级的图像很难收集，也很少会开源。因此最近有开发者在 GitHub 上开源了一份 NSFW 图像数据集，这是不是你们想要的？

内容审核在很多领域都有非常重要的作用，它不仅需要通过分类器识别图像或其它数据不适合展示，同时还能结合语义分割模型对这些限制级图像进行处理（Mask 掉敏感部分）。这样在不过多影响内容的情况下去除掉不合适的信息。开发者 alexkimxyz 构建的这个项目大概收集了 20 多万张敏感图像，且通过 URL 的形式展示了 GitHub 中。

项目地址：[https://github.com/alexkimxyz/nsfw_data_scrapper](https://link.zhihu.com/?target=https%3A//github.com/alexkimxyz/nsfw_data_scrapper)

这 20 万多张图像大概分为以下 5 个类别，它们可以用 CNN 训练不同的分类器。这里我们就保留 GitHub 中的原描述了：



![](https://pic4.zhimg.com/v2-50229ef141ff233a3614d971f948e0bb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='211'></svg>)
其中每一个类别都是一个 Text 文本，文本中的每一行都对应一个 URL，所以读取并下载都非常方便，自己写也就几行代码。如下简单展示了 sexy 类别下文本与图像：
![](https://pic1.zhimg.com/v2-b6f59cefa228a8e6a19ff1fc20590f68_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='669'></svg>)
此外值得注意的是，有少量图像 URL 是失效的，因此在处理的过程中需要把这些情况考虑进去。一般如果 URL 是失效的，它会返回一张 161×81 的声明图像。

当然，作者同样提供了获取 URL 和下载图像的脚本，我们只需要运行就行了。目前，这些脚本仅在 Ubuntu 16.04 Linux 发行版本中进行了测试。

以下是重要脚本（位于 scripts 目录下）及它们的作用：
- 1_get_urls.sh：遍历 scripts / source_urls 下的文本文件，下载上述 5 个类别中每个类别的图像 URL。Ripme 应用程序执行所有关键部分。源 URL 主要是链接到各种 subreddits，但可以是 Ripme 支持的任何网站。注意：作者已经运行了此脚本，其输出位于 raw_data 目录中。除非在 scripts / source_urls 下编辑文件，否则无需重新运行。
- 2_download_from_urls.sh：下载 raw_data 目录中的文本文件中找到的 URL 的实际图像。
- 5_create_train.sh：创建 data/train 目录并从 raw_data 将所有* .jpg 和* .jpeg 文件复制到其中。并删除损坏的图像。
- 6_create_test.sh：创建 data/test 目录，并从 data / trainto 为每个类随机移动 N = 2000 个文件（如果需要不同的训练/测试分割，则在脚本内更改此数字）到 data / test。或者，可以多次运行它，每次它将从 data/train 到 data/test 将每个类别移动 N 个图像。

注意运行 get_urls.sh 后，生成的 URL 文本文件会覆盖 raw_data 下已有的文本文件。所以在复制 GitHub 项目后，我们也可以直接运行 2_download_from_urls.sh 从已有 raw_data 文件下载图像。

**环境配置**
- Python3 环境：conda env create -f environment.yml
- Java 运行时环境（Ubuntu linux）：sudo apt-get install default-jre
- Linux 命令行工具：wget, convert (imagemagick 工具套件), rsync, shuf

**怎么运行**

将工作目录转到 scripts，并按文件名中的数字指示的顺序执行每个脚本，例如：


```
$ bash 1_get_urls.sh # has already been run
$ find ../raw_data -name "urls_*.txt" -exec sh -c "echo Number of urls in {}: ; cat {} | wc -l" \;
Number of urls in ../raw_data/drawings/urls_drawings.txt:
   25732
Number of urls in ../raw_data/hentai/urls_hentai.txt:
   45228
Number of urls in ../raw_data/neutral/urls_neutral.txt:
   20960
Number of urls in ../raw_data/sexy/urls_sexy.txt:
   19554
Number of urls in ../raw_data/porn/urls_porn.txt:
  116521
$ bash 2_download_from_urls.sh
$ bash 3_optional_download_drawings.sh # optional
$ bash 4_optional_download_neutral.sh # optional
$ bash 5_create_train.sh
$ bash 6_create_test.sh
$ cd ../data
$ ls train
drawings hentai neutral porn sexy
$ ls test
drawings hentai neutral porn sexy
```


如上所示为脚本的执行方法，五类一共 227995 张敏感图像。这个脚本同样会把它们分割为训练集与测试集，因此直接利用它们实现 5 类别的分类任务会很简单。当然如果我们需要用于其它的任务，就没有必要直接分割了。

使用简单的卷积神经网络直接实现分类任务可以达到 91% 的准确率，这已经非常高了，因为敏感数据手动分为 5 类本来就有一些模糊性存在。以下展示了在测试集上，5 分类任务的混淆矩阵：
![](https://pic2.zhimg.com/v2-53e80cfd7a96d6631ce8a1768b66d95d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='716' height='726'></svg>)
其中对角线表示正确预测的样本数，其它为误分类样本数。这个分类任务至少说明了 5 类别是有区分度的，不论我们用于正常内容与敏感内容的二分类，还是使用 GAN 做一些新奇的模型，类别都是很有区分度的特征。


