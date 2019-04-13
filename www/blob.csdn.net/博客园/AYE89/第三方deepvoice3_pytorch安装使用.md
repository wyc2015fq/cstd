
# 第三方deepvoice3_pytorch安装使用 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[第三方deepvoice3_pytorch安装使用](https://www.cnblogs.com/eniac1946/p/9213509.html)
|Posted on|2018-06-22 15:02|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9213509)|[收藏](#)
|项目地址：https://github.com/r9y9/deepvoice3_pytorch
![](https://images2018.cnblogs.com/blog/1181483/201806/1181483-20180622144514514-1917235834.png)
|解决两个问题：一使用外部数据集训练，二加载pre-trained模型做fine-tune
|1：安装必要依赖库
|其中pytorch如下
|pip3 install http://download.pytorch.org/whl/cpu/torch-0.4.0-cp36-cp36m-win_amd64.whl
|pip3 install torchvision
|安装完成后，需下载|[win-64/intel-openmp-2018.0.0-8.tar.bz2](https://anaconda.org/anaconda/intel-openmp/files)|，把bin文件夹加到PATH环境变量
|2：生成两个json文件
|alignment.json （wav - text对）
|my_presets_dv.json （模型配置文件，可用相应模型的json修改，比如/presets/|[deepvoice3_ljspeech.json](https://github.com/r9y9/deepvoice3_pytorch/blob/master/presets/deepvoice3_ljspeech.json)|）
|注意修改wav文件的采样率、训练集合适的batch_size
|3：preprocess
|如果新数据集路径./datasets/audio
|$ python preprocess.py json_meta "./datasets/alignment.json" "./datasets/audio" --preset=./datasets/my_presets_dv.json
|完成后如下图
![](https://images2018.cnblogs.com/blog/1181483/201806/1181483-20180622145334202-1148287045.png)
|4：下载pre-trained模型文件
|放到路径./model/20180505_deepvoice3_checkpoint_step000640000.pth
|下载对应commit的preset文件（json）
|5：train
|命令 python train.py  <option>
|python train.py --data-root=./datasets/audio --checkpoint-dir=checkpoints_adaptation --checkpoint=./model/20180505_deepvoice3_checkpoint_step000640000.pth --preset=./datasets/my_presets_dv.json --log-event-path=log/deepvoice3_spk_adaptation --speaker-id=0








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
