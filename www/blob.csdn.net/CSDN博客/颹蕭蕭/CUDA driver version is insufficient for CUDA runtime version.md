
# CUDA driver version is insufficient for CUDA runtime version - 颹蕭蕭 - CSDN博客


2019年03月29日 12:56:22[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：234


用 conda 安装完 tensorflow-gpu : （**注意：conda 自动帮你装了最新的 cuda10！**）
`>>> conda install tensorflow-gpu
Solving environment: done
## Package Plan ##
  environment location: D:\ProgramFilesNoSpace\Miniconda3\envs\py36
  added / updated specs:
    - tensorflow-gpu

The following NEW packages will be INSTALLED:
    cudatoolkit:     10.0.130-0                https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    cudnn:           7.3.1-cuda10.0_0          https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow:      1.13.1-gpu_py36h9006a92_0 https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow-base: 1.13.1-gpu_py36h871c8ca_0 https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow-gpu:  1.13.1-h0d30ee6_0         https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
Proceed ([y]/n)? n  # 这个 n 是后来加的`一跑程序就出现：
`Traceback (most recent call last):
  File "D:/workspace/pythonProj/test2.py", line 61, in <module>
    sess = tf.Session()
  File "D:\ProgramFilesNoSpace\Miniconda3\envs\py36\lib\site-packages\tensorflow\python\client\session.py", line 1551, in __init__
    super(Session, self).__init__(target, graph, config=config)
  File "D:\ProgramFilesNoSpace\Miniconda3\envs\py36\lib\site-packages\tensorflow\python\client\session.py", line 676, in __init__
    self._session = tf_session.TF_NewSessionRef(self._graph._c_graph, opts)
tensorflow.python.framework.errors_impl.InternalError: cudaGetDevice() failed. Status: CUDA driver version is insufficient for CUDA runtime version`看看 conda 安装的 cuda 版本
`>>> conda list
# packages in environment at D:\ProgramFilesNoSpace\Miniconda3\envs\py36:
#
# Name                    Version                   Build  Channel
...
cudatoolkit               10.0.130                      0    https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
cudnn                     7.3.1                cuda10.0_0    https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main`检查显卡驱动版本：
`>>> nvidia-smi
Fri Mar 29 11:54:57 2019
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 399.24                 Driver Version: 399.24                    |
|-------------------------------+----------------------+----------------------+
| GPU  Name            TCC/WDDM | Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  GeForce MX150      WDDM  | 00000000:01:00.0 Off |                  N/A |
| N/A   35C    P8    N/A /  N/A |     64MiB /  2048MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+
+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID   Type   Process name                             Usage      |
|=============================================================================|
|  No running processes found                                                 |
+-----------------------------------------------------------------------------+`版本为 399
再一查 tensorflow 系统要求：
[https://www.tensorflow.org/install/gpu\#software_requirements](https://www.tensorflow.org/install/gpu#software_requirements)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190329121101361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
搞什么嘛，只支持到 cuda 9.0
[
](https://img-blog.csdnimg.cn/20190329121101361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)只好把 cuda 给卸了
[
](https://img-blog.csdnimg.cn/20190329121101361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)`>>> conda uninstall cudnn
Solving environment: done
## Package Plan ##
  environment location: D:\ProgramFilesNoSpace\Miniconda3\envs\py36
  removed specs:
    - cudnn

The following packages will be REMOVED:
    cudnn:           7.3.1-cuda10.0_0          https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow:      1.13.1-gpu_py36h9006a92_0 https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow-base: 1.13.1-gpu_py36h871c8ca_0 https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow-gpu:  1.13.1-h0d30ee6_0         https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main

>>> conda uninstall cudatoolkit
Solving environment: done
## Package Plan ##
  environment location: D:\ProgramFilesNoSpace\Miniconda3\envs\py36
  removed specs:
    - cudatoolkit

The following packages will be REMOVED:
    cudatoolkit: 10.0.130-0 https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main`[
](https://img-blog.csdnimg.cn/20190329121101361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)现在重装 tensorflow-gpu，选择版本 1.9，对应 cuda9.0
[
](https://img-blog.csdnimg.cn/20190329121101361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)`>conda install tensorflow-gpu==1.9
Collecting package metadata: done
Solving environment: done
## Package Plan ##
  environment location: D:\ProgramFilesNoSpace\Miniconda3\envs\py36
  added / updated specs:
    - tensorflow-gpu==1.9

The following packages will be downloaded:
    package                    |            build
    ---------------------------|-----------------
    cudatoolkit-9.0            |                1       339.8 MB  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    cudnn-7.3.1                |        cuda9.0_0       170.9 MB  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorboard-1.9.0          |   py36he025d50_0         3.3 MB  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow-1.9.0           |gpu_py36hfdee9c2_1           4 KB  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow-base-1.9.0      |gpu_py36h6e53903_0        78.7 MB  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    tensorflow-gpu-1.9.0       |       hf154084_0           3 KB  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
    ------------------------------------------------------------
                                           Total:       592.7 MB
The following NEW packages will be INSTALLED:
  cudatoolkit        anaconda/pkgs/main/win-64::cudatoolkit-9.0-1
  cudnn              anaconda/pkgs/main/win-64::cudnn-7.3.1-cuda9.0_0
  tensorflow         anaconda/pkgs/main/win-64::tensorflow-1.9.0-gpu_py36hfdee9c2_1
  tensorflow-base    anaconda/pkgs/main/win-64::tensorflow-base-1.9.0-gpu_py36h6e53903_0
  tensorflow-gpu     anaconda/pkgs/main/win-64::tensorflow-gpu-1.9.0-hf154084_0
The following packages will be DOWNGRADED:
  tensorboard                         1.13.1-py36h33f27b4_0 --> 1.9.0-py36he025d50_0`[
](https://img-blog.csdnimg.cn/20190329121101361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)发现 conda 还是不错的， 会自动把刚才没有删干净的 tensorboard 自动降级（downgrade）
[
            ](https://img-blog.csdnimg.cn/20190329121101361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)

