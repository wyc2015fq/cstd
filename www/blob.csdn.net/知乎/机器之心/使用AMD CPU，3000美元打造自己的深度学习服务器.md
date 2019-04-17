# 使用AMD CPU，3000美元打造自己的深度学习服务器 - 知乎
# 



**选自GitHub，作者：Wayde Gilliam，机器之心编译。**

> 本文作者详细描述了自己组装深度学习服务器的过程，从 CPU、GPU、主板、电源、机箱等的选取到部件的安装，再到服务器的设置，可谓面面俱到。作者指出，组装者首先要弄清自己的需求，然后根据预算做出合理的选择。

注：本文旨在讨论服务器设置及多用户协作，部件组装和软件安装过程是 Slav Ivanov 关于创建自己的 DL Box 的文章（[https://medium.com/m/global-identity?redirectUrl=https://blog.slavv.com/the-1700-great-deep-learning-box-assembly-setup-and-benchmarks-148c5ebe6415](https://link.zhihu.com/?target=https%3A//medium.com/m/global-identity%3FredirectUrl%3Dhttps%3A//blog.slavv.com/the-1700-great-deep-learning-box-assembly-setup-and-benchmarks-148c5ebe6415)）的简化版本。

我刚开始学习 fast.ai 课程的第一部分——「Practical Deep Learning for Coders」，我想搭建自己的服务器来训练模型，作为使用 AWS p2 和存储的升级。我将会使用更大的数据集，并且我不希望在训练模型时因为缺乏足够的处理能力而等待数小时，因此构建自己的 DL rig 服务器对我来说是一个不错的选择，而且从长远来看，它将为我节省大量的时间和金钱，而且可以积累组装服务器的良好经验。

## **组装**

**列出部件清单**

在准备部件之前，你要弄清楚自己到底想从机器中得到什么。个人而言，我想获得以下改进：
- 比 Amazon p2 更加强大
- 容纳额外 GPU、RAM、存储及定制液体冷却的空间
- 可以用很久的硬件
- 预算控制在 3000 美元左右

我用 [http://pcpartpicker.com](https://link.zhihu.com/?target=http%3A//pcpartpicker.com)来细化各个部件，因为它可以将对比和组装其它部件变得非常简单，而且还具备很不错的部件兼容性检查器。我将详细解释选择每个部件的原因以及它们如何与整个系统协同工作。以下是我的服务器链接地址：[https://pcpartpicker.com/b/CgpG3C](https://link.zhihu.com/?target=https%3A//pcpartpicker.com/b/CgpG3C)。




**GPU: GTX 1080 Ti Hybrid**

因为你要使用显卡来训练模型，所以这是组装过程中最重要的一部分，因此，GPU 越强大，你处理大型数据集的速度就越快。GPU 内存越大，处理能力也就越强（如：训练速度更快，批尺寸更大……）。我为自己的服务器选了两块这种显卡，因为我在预算里为它们预留了空间，这样我就能用其中一块显卡训练模型，让另一个用户在第二块卡上训练其模型。你可以根据自己的预算缩减 GPU 内存（1070、1060 等），防止与其它部件争抢预算。Hybrid 1080 GPU 也不错，因为除了大多数 GPU 具有的正常风扇冷却之外，它还预装了 aio 水冷系统。1080 Ti 在满负载运行时会过热，因此，在训练模型时，良好的冷却系统对于延长显卡寿命并保持其性能至关重要。关于显卡选择的更多细节，请参见 [http://timdettmers.com/2017/04/09/which-gpu-for-deep-learning/](https://link.zhihu.com/?target=http%3A//timdettmers.com/2017/04/09/which-gpu-for-deep-learning/)，该文帮助我真正了解了如何选择适合深度学习环境的显卡。




**CPU: AMD Threadripper 1900x**

虽然你用显卡训练神经网络，但 CPU 依然很重要，因为你要用它来进行数据准备等操作，因此那些多核 CPU 将有助于加快速度。我用的是「线程撕裂者」Treadripper，因为这是市面上非常新的一款多核 CPU（ThreadRipper 2 代有 32 个内核！），而且比因特尔价格低得多。1900x 是去年发布的基础版 TR，它只有 8 个内核，不过我对这台服务器的整体目标是保持它的可升级性。

需要注意的一点是，在选择 CPU 时，要确保有 8 或 16 个 pcie 插槽供显卡使用，这样才能保证它们在低负载下发挥最好的性能，否则就有堵塞系统的风险。在高端 CPU 上，如果你的服务器中有 4 个显卡，那么你就有足够的 pcie 插槽。




**主板：MSI X399 SLI Plus**

选择这块主板是因为它是一块完整的 ATX 板，可容纳 4 个 GPU，RAM 最多可达 128GB。正如我前面所说，这个服务器的主要目标之一是保持它可升级。




**内存：32GB Corsair Vengeance LPX DDR4 (2 x 16GB)**

内存越大，处理大型数据集就越容易。我的下一个升级计划是再添加两个 16GB RAM 内存条，这也是我没有安装四通道内存（4 个 8GB 内存条）的原因，尽管它会提高我的服务器的性能。




**存储：256GB Samsung SSD & 2TB HDD**

我把 Ubuntu、我的所有库、我在 SSD 上正在使用的数据集以及手里的其它所有数据都存在 2TB 的机械硬盘上。




**冷却器：Corsair H100i v2 液体冷却器**

「线程撕裂者」没有备用冷却器（如果你有 30 美元的余款，至少应该买一个便宜的二手冷却器），所以我想要一个可以全天候使用的、便宜又容易维护的冷却器。这款集各种优点于一身的冷却器非常容易安装，而且非常可靠（在数十万台机组中，可能只有一两台发生冷却液泄漏）、安静。




**电源：EVGA SuperNOVA 1000w 80 + Gold Certified**

最好有一个功率超过技术要求的 PSU。PCPartpicker 的功率计算器能够大致算出你需要多大的功率（我的服务器是 824w），然而它经常在数量上出错，因此最好进行安全操作，以防你的计算机无法打开。「Gold Certified」只是指 PSU 的效率（浪费了多少功率作为热量）。




**机箱：Corsair 760T Full Tower**

我选择这个机箱是因为它的价格和内部空间大小。虽然它并不能提高你的模型训练速度，但是透明的侧面板和红色的 LED 确实让你看起来更酷。
![](https://pic3.zhimg.com/v2-07662de215f67fe3f6bad50cc4c0149e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='610'></svg>)这幅图里的东西花掉了我一年的积蓄和毕业的钱
**把部件整合到一起**

你可能不太了解如何组装一台电脑，这个过程其实就像组装一套昂贵的乐高积木一样。任何人都可以做到，因为它真的很简单。我将快速介绍如何组装电脑，并且我强烈建议你在组装自己的电脑时观看完整的视频（[https://www.youtube.com/watch?](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3F)？v = IhX0fOUYd8Q）。这些说明几乎适用于任何你正在使用的电脑，就像我上面链接的视频指南一样。




**第一步：组装 CPU**
![](https://pic2.zhimg.com/v2-90fd0e867766f156a921ff5b7cbb702d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)

这可能是组装电脑过程中最可怕的部分，因为你必须遵循特定的步骤，并且可能由于一些小意外而毁掉价值 430 美元的 CPU（如弄断一个 CPU 的引脚）。不过这一步其实很简单，你只需确保组装前看过视频教程即可。对于 Threadripper 来说，安装过程略有不同，因为在大多数主板上，你不需要「锁紧」处理器，而是使用单个扣架即可。
![](https://pic3.zhimg.com/v2-877db7179e129159e8bc91f65d2d808a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='718'></svg>)照片来源：Slav Ivanov
**第二步：组装电源设备**
![](https://pic2.zhimg.com/v2-7f67d11fae793301ecec44553e900341_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='611'></svg>)
电源的安装顺序并没有对错之分，但我喜欢先把 PSU 放进盒子里，因为有些情况下你需要把 PSU 卡进一个插槽里，这样做你才能「穿过主板」。




**第三步：组装其它部件**
![](https://pic4.zhimg.com/v2-86fcefddfd5eb257f07e1ca9604d756b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
这是至关重要的一步，一旦主板安装好，那其他部分的安装会变得很容易。我将按照如下顺序进行安装：
- 安装 RAM。这很简单，只需把内存条向正确的方向移动，并把插槽正确地按进主板即可（请参阅安装手册，因为插槽要根据你拥有的内存条个数决定）。
- 安装 CPU 冷却器。唯一的小麻烦是为了与 TR 兼容，我必须更换安装支架，这需要很大的力气。安装散热器需要 8 个螺栓，我已安装完毕。
- 安装显卡。只需要把显卡安装到主板的特定卡槽中即可（像内存条一样，参照你的手册，看哪些插槽可以把显卡放入），再把散热器固定在你的机箱上。要确保你的散热器在 GPU 上方。在上图中我犯了个小错误，所以不得不把它重新挂在机箱的前板上。
- 安装存储器。为了获得更好的散热效果我卸掉了一块驱动板，所以我把 SSD 和 HDD 放在右下角的单驱动器槽里。




**第四步：安装成功？**
![](https://pic2.zhimg.com/v2-43c7b6cda5a8d7cb950b78af37a4d829_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1078' height='611'></svg>)
现在可以打开你的设备了。开始因为我设备的电源键上正负线接反了，所以我的设备并没有成功打开，但后来还是出现了预期的白光和红光。如果一切顺利，你可以看到你的电脑屏幕被点亮，然后主板开始搜索引导设备。

## **设置服务器**

**安装操作系统**

下一步是安装操作系统。我使用的是 Linux，因为大多数 DL 框架都是针对该系统设计的。使用的台式机是 Ubuntu 16.04 LTS, 用 USB 就可安装全部的东西。有很多像 UNetbootin 或 Rufus（仅用于 Windows）的免费工具，可以先下载到 U 盘中备用。这是一个介绍在 Mac 上创建可启动 USB 的完整过程的教程（[https://tutorials.ubuntu.com/tutorial/tutorial-create-a-usb-stick-on-macos?_ga=2.169997348.541383618.1529376295-1852780805.1529376295#0](https://link.zhihu.com/?target=https%3A//tutorials.ubuntu.com/tutorial/tutorial-create-a-usb-stick-on-macos%3F_ga%3D2.169997348.541383618.1529376295-1852780805.1529376295%230)），如果你用 Windows，可以打开此链接：[https://www.howtogeek.com/howto/linux/create-a-bootable-ubuntu-usb-flash-drive-the-easy-way/](https://link.zhihu.com/?target=https%3A//www.howtogeek.com/howto/linux/create-a-bootable-ubuntu-usb-flash-drive-the-easy-way/)。

## **设置 SSH**

**第一步：端口映射**

你需要对所有的路由器进行类似处理，如果是 apple 的路由器可以遵循这个指南：[https://portforward.com/apple/](https://link.zhihu.com/?target=https%3A//portforward.com/apple/)，操作如下：1. 为你的服务器设置一个静态 IP，防止它在每次关闭时更改。2. 用 Apple Airport Utility 登录你的路由器。3. 为服务器映射端口，完成这一步需要找到服务器的 MAC 地址，如何在 Ubuntu 找到地址可以查看此链接：[http://technologyinfinite.blogspot.com/2016/07/three-simple-ways-to-find-mac-address.html](https://link.zhihu.com/?target=http%3A//technologyinfinite.blogspot.com/2016/07/three-simple-ways-to-find-mac-address.html)。




**第二步：创建一个动态 IP 地址**

我之前为服务器创建过动态 IP 地址，可以允许我在终端远程连接它。你可以通过该网站（[http://canyouseeme.org/](https://link.zhihu.com/?target=http%3A//canyouseeme.org/)）验证它的有效性。

键入一个如下所示的命令连接到我的服务器：

`ssh [my_username]@[my_ip] -L 8888:[dl-rig_static_ip]:8889`

我的服务器在端口 8888 上运行，jupyter notebooks 运行在 8889 上（-L 选项将指定的本机端口重新定向到不同的主机和端口）。这样我们就可以在本地运行我们的设备，与服务器同时测试，以便训练。如果不想这样，在-L 之前把所有东西输入进去即可。在下一节我会解释如何更改运行 jupyter notebooks 的端口。




**安装深度学习/机器学习库**

现在需要安装所有的与深度学习/机器学习相关的库。我会将安装脚本进行分节，这样你就容易理解这些内容。该脚本是基于 Jeremy Howard』s 编写的 install-gpu.sh（[https://github.com/fastai/courses/blob/master/setup/install-gpu.sh](https://link.zhihu.com/?target=https%3A//github.com/fastai/courses/blob/master/setup/install-gpu.sh)），这里边很多东西不仅仅是因为它们是实用性工具，更是因为这也是我们要在 Fastai 中使用的。

首先，我们需要保证系统是最新的，并且安装了所有我们需要的基础工具包：


```
sudo apt-get update
sudo apt-get --assume-yes upgrade
sudo apt-get --assume-yes install tmux build-essential gcc g++ make binutils unzip
sudo apt-get --assume-yes install software-properties-common
sudo apt-get --assume-yes install git
```


下一步是下载和安装所有 CUDA GPU 的驱动包：


```
mkdir ~/downloadscd ~/downloads

wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1604/x86_64/cuda-repo-ubuntu1604_9.0.176-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu1604_9.0.176-1_amd64.deb
sudo apt-key adv --fetch-keys http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1604/x86_64/7fa2af80.pub
sudo apt-get update
sudo apt-get -y install cuda
sudo apt-get --assume-yes upgrade
sudo apt-get --assume-yes autoremove
sudo apt-get install cuda-toolkit-9.0 cuda-command-line-tools-9-0
```


现在我们开始验证是否正确安装了 CUDA：


```
sudo modprobe nvidia
nvcc --version
nvidia-smi
```


现在开始，我们将 CUDA（Nvidia Deep Learning api）添加到路径变量：


```
cat >> ~/.bashrc << 'EOF'
export PATH=/usr/local/cuda-9.0/bin${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-9.0/lib64\
${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
EOF

source ~/.bashrc
```


下一步是安装 CuDNN 库（创建神经网络所需）:


```
wget http://files.fast.ai/files/cudnn-9.1-linux-x64-v7.tgz
tar xf cudnn-9.1-linux-x64-v7.tgz
sudo cp cuda/include/*.* /usr/local/cuda/include/
sudo cp cuda/lib64/*.* /usr/local/cuda/lib64/
```


现在我们为当前用户安装 Anaconda：


```
wget "https://repo.continuum.io/archive/Anaconda3-5.0.1-Linux-x86_64.sh"
bash "Anaconda3-5.0.1-Linux-x86_64.sh" -b

cd ~

echo "export PATH=\"$HOME/anaconda3/bin:\$PATH\"" >> ~/.bashrc
export PATH="$HOME/anaconda3/bin:$PATH"
conda install -y bcolz
conda upgrade -y --all
```


下一步，安装 Tensorflow 和 Keras：


```
pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-1.8.0-cp36-cp36m-linux_x86_64.whl

pip install keras
mkdir ~/.keras
echo '{
    "image_dim_ordering": "tf",
    "epsilon": 1e-07,
    "floatx": "float32",
    "backend": "tensorflow"
}' > ~/.keras/keras.json
```


接下来，我们将为 Fastai 安装 Python 依赖项：


```
mkdir -p ~/development/_training/ml
cd ~/development/_training/ml

git clone https://github.com/fastai/fastai.git
cd fastai
conda env update
```


以下几节将介绍如何配置 jupyter notebook：


```
# Leaving the next line uncommented will prompt you to provide a password to
# use with your jupyter notebook.
jupass=`python -c "from notebook.auth import passwd; print(passwd())"`
# To hardcode the password to 'jupyter' comment line above and uncomment the line below.
#jupass=sha1:85ff16c0f1a9:c296112bf7b82121f5ec73ef4c1b9305b9e538af

# create ssl cert for jupyter notebook
openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout $HOME/mykey.key -out $HOME/mycert.pem -subj "/C=IE"

# configure notebook
echo "c.NotebookApp.certfile = u'/home/{user}/mycert.pem'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.keyfile = u'/home/{user}/mykey.key'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.password = u'"$jupass"'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.ip = '*'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.open_browser = False" >> $HOME/.jupyter/jupyter_notebook_config.py
```


要更改默认端口以在（端口 8888）上运行 Jupyter notebook，请取消注释，并输入所需端口。这样就可以在服务器和本地同时运行你的笔记本，也可以在你使用笔记本时让多个用户使用他们自己的笔记本。

`#echo "c.NotebookApp.port = 9999" >> $HOME/.jupyter/jupyter_notebook_config.py`

现在我们开始配置 tmux，这个工具可以使我们在终端窗口创建多个「窗口」，同时在断开连接后仍能保持程序的运行。因为这个文件能够帮助我们理解 tmux 的所有功能，所以需要检查此文件。它非常的有用，因为你可以在一个窗口中运行你的笔记本，在另一个窗口中监视 GPU 使用，并在第三个窗口中打开一个 linux 终端：


```
pip install tmuxp
mkdir ~/.tmuxp
```


接下来，我们将创建 tmuxp 的配置文件，该文件将在一个命令中对开发环境进行设置，这样我们每次想处理事务的时候就无需配置窗口、启用 jupyter notebook 了。对于 fastai 环境，我们将从 tmuxp 加载 fastai 开始。请参见使用 tmuxp 的链接，此处是 bash 脚本中的文档，此链接用于在服务器重启时保存 tmux 会话。现在让我们配置运行环境。


```
cat > $HOME/.tmuxp/fastai.yml <<tmuxp-config 
session_name: fastai
windows:
- window_name: dev window
  layout: main-vertical
  options:
    main-pane-width: 140
  shell_command_before:
    # run as a first command in all panes
    - cd ~/development/_training/ml/fastai
    - source activate fastai
  panes:
    - shell_command:
      - clear
    - shell_command:
      - clear
      - jupyter notebook
    - shell_command:
      - watch -n 0.5 nvidia-smi
Tmuxp-config
```


因为我们不再需要这些，所以可以删除安装文件：


```
cd ~/downloads
rm -rf cuda-repo-ubuntu1604_9.0.176-1_amd64.deb xf cudnn-9.1-linux-x64-v7.tgz Anaconda3-5.0.1-Linux-x86_64.sh        

cd ~
```


就是这样。在我写这篇文章的时候，服务器一直在全天候运行，无问题、无噪声、非常轻松地通过了训练。

其他参考资料来源：
- [https://www.digitalocean.com/community/tutorials/how-to-set-up-ssh-keys--2](https://link.zhihu.com/?target=https%3A//www.digitalocean.com/community/tutorials/how-to-set-up-ssh-keys--2)
- [https://towardsdatascience.com/building-your-own-deep-learning-box-47b918aea1eb](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/building-your-own-deep-learning-box-47b918aea1eb)
- [https://medium.com/impactai/setting-up-a-deep-learning-machine-in-a-lazy-yet-quick-way-be2642318850](https://link.zhihu.com/?target=https%3A//medium.com/impactai/setting-up-a-deep-learning-machine-in-a-lazy-yet-quick-way-be2642318850)
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[https://waydegg.github.io/making-a-dl-server.html#making-a-dl-server](https://link.zhihu.com/?target=https%3A//waydegg.github.io/making-a-dl-server.html%23making-a-dl-server)


