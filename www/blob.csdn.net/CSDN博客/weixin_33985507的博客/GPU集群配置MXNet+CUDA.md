# GPU集群配置MXNet+CUDA - weixin_33985507的博客 - CSDN博客
2018年08月04日 22:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 准备
## 批处理
为方便控制集群，写了脚本`cmd2all.sh`
```
#!/bin/bash
if [ $# -lt 3 ]; then
    echo "usage: $0 [type cmds hosts]"
    echo "for example: ./cmd2all.sh \"cmds\" \"touch t1.txt\" \"gpu1 gpu2\""
    echo "for example: ./cmd2all.sh \"path\" \"/home/gbxu/CUDA/" \"gpu1 gpu2\""
    exit -1;
fi
type=$1 # "cmds"
cmds_or_path=$2 # "touch test.txt"
#hosts=$3
hosts=(gpu10 gpu11 gpu12 gpu13 gpu14 gpu15 gpu16 gpu17 gpu18)
if [$type == "cmds"]
then
  for host in ${hosts[@]}
    do
      ssh $host nohup $cmds_or_path  &
  done
fi
if [$type == "path"]
then
  for host in ${hosts[@]}
    do
      nohup scp -r $cmds_or_path $host:~/ &
    done
fi
```
## 使用virtualenv
如果是python3的环境，需要`virtualenv -p /usr/bin/python3 mxnetGPU`
使用virtualenv，创建新的virtualenv，并修改.bashrc，使得在每次进入终端时activate虚拟环境（方便后期分布式运行）
```
hosts="gpu10 gpu11 gpu12 gpu13 gpu14 gpu15 gpu16 gpu17 gpu18 "
./cmd2all.sh "cmds" "sudo yum -y install epel-release && sudo yum -y install python-pip && sudo pip install virtualenv && virtualenv mxnetGPU" $hosts
./cmd2all.sh "cmds"  "echo \"## gbxu MXnet-GPU\" >> .bashrc" $hosts
./cmd2all.sh "cmds"  "echo \"source mxnetGPU/bin/activate\" >> .bashrc" $hosts
```
# 尝试在gpu10安装
## Install NVIDIA Driver
本身已有驱动则该操作不必要。
```
lspci | grep -i nvidia #查看设备
modinfo nvidia #查看驱动
sudo yum -y remove nvidia-*
sudo sh  NVIDIA-Linux-x86_64-390.25.run #安装驱动
```
## Install CUDA:
see documents:
- [offline安装](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&target_distro=CentOS&target_version=7&target_type=rpmlocal)，online版本可能出现依赖缺失。
- [所有版本](https://developer.nvidia.com/cuda-toolkit-archive)
CUDA是NVIDIA推出的用于自家GPU的并行计算框架，只有当要解决的计算问题是可以大量并行计算的时候才能发挥CUDA的作用。
下载：  见offline安装
```
#copy installer && run
# 若安装错误需要先卸载
sudo yum -y remove "cuda-*"
sudo rm -rf /usr/local/cuda*
sudo rpm -i cuda-repo-rhel7-9-2-local-9.2.148-1.x86_64.rpm
sudo yum clean all
sudo yum -y install cuda
```
> 
gpu10利用yum local的安装出现问题，后来下载[cuda_9.2.148_396.37_linux.run](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&target_distro=CentOS&target_version=7&target_type=runfilelocal)
`sudo sh cuda_9.2.148_396.37_linux.run`安装
并且在安装(or not, just try)时同意nvidia驱动，并且一路yes和default。
or, add /usr/local/cuda-9.2/lib64 to /etc/ld.so.conf and run ldconfig as root
## 添加CUDA环境变量
```
# export LD_LIBRARY_PATH=/usr/local/cuda/lib64/:$LD_LIBRARY_PATH
echo -e "export LD_LIBRARY_PATH=/usr/local/cuda/lib64/:\$LD_LIBRARY_PATH" >> .bashrc 
# export PATH=$PATH:/usr/local/cuda/bin
echo -e "export PATH=\$PATH:/usr/local/cuda/bin" >> .bashrc
```
## 测试CUDA
```
nvcc -V
nvidia-smi
cd /home/gbxu/NVIDIA_CUDA-9.2_Samples/1_Utilities/deviceQuery
make
./deviceQuery # 结果pass则安装成功
```
## Install cuDNN:
see [documents](https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html#installlinux-tar)
cuDNN（CUDA Deep Neural Network library）：是NVIDIA打造的针对深度神经网络的加速库，是一个用于深层神经网络的GPU加速库。如果你要用GPU训练模型，cuDNN不是必须的，但是一般会采用这个加速库。
```
tar -xzvf cudnn-9.2-linux-x64-v7.1.tgz
sudo cp cuda/include/cudnn.h /usr/local/cuda/include
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64
sudo chmod a+r /usr/local/cuda/include/cudnn.h
```
## 安装Prerequisites
see [documents](https://mxnet.incubator.apache.org/install/index.html?platform=Linux&language=Python&processor=GPU)
```
sudo yum -y install build-essential git lapack-devel openblas-devel opencv-devel atlas-devel
```
## complie MXNet
see:[documents](https://mxnet.incubator.apache.org/install/index.html?platform=Linux&language=Python&processor=GPU)
```
git clone --recursive https://github.com/apache/incubator-mxnet.git
cd incubator-mxnet
make clean_all
make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas USE_CUDA=1 USE_CUDA_PATH=/usr/local/cuda USE_CUDNN=1 USE_DIST_KVSTORE=1  USE_PROFILER=1
```
## install MXNet in python
```
cd python
pip uninstall -y mxnet
pip install -e .
```
## test MXNet in python
```
python
>>> import mxnet as mx                                                                                                                                                      
>>> a = mx.nd.zeros((2,3), mx.gpu())
```
## install python lib
请根据最后运行MXNet任务时查缺补漏
```
pip install numpy requests
```
## 预设编译参数
cd到源代码主目录，在makefile文件中预设编译参数，
```
# vim incubator-mxnet/Makefile
cmpl:
    make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas USE_DIST_KVSTORE=1
cmplgpu:
    make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas USE_DIST_KVSTORE=1 USE_CUDA=1 USE_CUDA_PATH=/usr/local/cuda USE_CUDNN=1
```
之后使用make指令编译更为便捷。
```
make cmplgpu
```
# 批量安装环境
## 在gpu11-gpu18批量安装环境
先用`1.sh`将数据传到nodes，
- `1.sh`
```
hosts=(gpu11 gpu12 gpu13 gpu14 gpu15 gpu16 gpu17 gpu18)
for host in ${hosts[@]}
do
  echo run 1.sh at $host
  scp -r process_data gbxu@$host:~/
done
```
再用`2.sh`在各nodes运行`scripts_in_nodes.sh`脚本即可。
- `2.sh`
```
hosts=(gpu12 gpu13 gpu14 gpu15 gpu16 gpu17 gpu18)
for host in ${hosts[@]}
do
  echo run 2.sh at $host
  scp process_data/scripts_in_nodes.sh gbxu@$host:~/process_data/
  ssh gbxu@$host "cd process_data && nohup ./scripts_in_nodes.sh &"
done
```
- `scripts_in_nodes.sh`
```
sudo yum -y remove "cuda-*" 
sudo rpm -i cuda-repo-rhel7-9-2-local-9.2.148-1.x86_64.rpm
sudo yum clean all
sudo yum -y install cuda
echo -e "export LD_LIBRARY_PATH=/usr/local/cuda/lib64/:\$LD_LIBRARY_PATH" >> ~/.bashrc
echo -e "export PATH=\$PATH:/usr/local/cuda/bin" >> ~/.bashrc
tar -xzvf cudnn-9.2-linux-x64-v7.1.tgz
sudo cp cuda/include/cudnn.h /usr/local/cuda/include
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64
sudo chmod a+r /usr/local/cuda/include/cudnn.h
sudo yum -y install build-essential git lapack-devel openblas-devel opencv-devel atlas-devel
pip install numpy requests # 请根据最后运行MXNet任务时查缺补漏
```
# 编译、安装MXNet
之后只需在一台host上编译mxnet即可，余下用MXNet的同步机制即可。
在gpu10上启动训练
- 需要加库文件放到同步的文件夹下：
```
cd incubator-mxnet/example/image-classification
echo -e "gpu11\ngpu12\ngpu13\ngpu14\ngpu15\ngpu16\ngpu17\ngpu18\n" > hosts
rm -rf mxnet
cp -r ../../python/mxnet .
cp -r ../../lib/libmxnet.so mxnet
```
- 然后执行命令，该命令会同步文件夹cluster上启动8个worker，1个server
```
# export DMLC_INTERFACE='ib0'; # ib尚未配置好
python ../../tools/launch.py -n 8 -s 1 --launcher ssh -H hosts --sync-dst-dir /home/gbxu/image-classification_test/  python train_mnist.py --network lenet --kv-store dist_sync --num-epochs 1 --gpus 0
```
# ENJOY
- multiple machines each containing multiple GPUs 的训练见[docs](https://mxnet.incubator.apache.org/tutorials/vision/large_scale_classification.html#run-training)
- 其中 `dist_sync_device` 替代 `dist_sync`。因为cluster为多GPU，见[docs](https://mxnet.incubator.apache.org/faq/distributed_training.html#different-modes-of-distributed-training)
- mxnet-make-install-test.sh
```
cd incubator-mxnet
make clean_all
make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas USE_CUDA=1 USE_CUDA_PATH=/usr/local/cuda USE_CUDNN=1 USE_DIST_KVSTORE=1  USE_PROFILER=1 
cd python
pip uninstall -y  mxnet
pip install -e .
cd ../example/image-classification
echo -e "gpu11\ngpu12\ngpu13\ngpu14\ngpu15\ngpu16\ngpu17\n" > hosts
rm -rf mxnet # example/image-classification下的
cp -r ../../python/mxnet .
cp -r ../../lib/libmxnet.so mxnet
export DMLC_INTERFACE='ib0';
python ../../tools/launch.py -n 8 -s 1 --launcher ssh -H hosts --sync-dst-dir /home/gbxu/image-classification_test/  python train_mnist.py --network lenet --kv-store dist_sync --num-epochs 1 --gpus 0
```
