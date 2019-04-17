# nvidia-docker + caffe2 + cuda - qq_30006593的博客 - CSDN博客





2018年08月18日 15:21:11[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：228








首先安装docker: [官网](https://docs.docker-cn.com/engine/installation/linux/docker-ce/centos/)

```
sudo yum remove docker \
                  docker-common \
                  docker-selinux \
                  docker-engine
sudo yum install -y yum-utils device-mapper-persistent-data lvm2
sudo yum-config-manager \
     --add-repo \
     https://download.docker.com/linux/centos/docker-ce.repo
sudo yum makecache fast
sudo yum install docker-ce

# start
sudo systemctl start docker

# test
sudo docker run hello-world
```

然后安装nvidia-docker: [官网](https://github.com/NVIDIA/nvidia-docker),这样才能使用gpu

```
# If you have nvidia-docker 1.0 installed: we need to remove it and all existing GPU containers
docker volume ls -q -f driver=nvidia-docker | xargs -r -I{} -n1 docker ps -q -a -f volume={} | xargs -r docker rm -f
sudo yum remove nvidia-docker

# Add the package repositories
distribution=$(. /etc/os-release;echo $ID$VERSION_ID)
curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.repo | \
  sudo tee /etc/yum.repos.d/nvidia-docker.repo

# Install nvidia-docker2 and reload the Docker daemon configuration
sudo yum install -y nvidia-docker2
sudo pkill -SIGHUP dockerd

# Test nvidia-smi with the latest official CUDA image
docker run --runtime=nvidia --rm nvidia/cuda nvidia-smi
```

找到caffe2的[docker image](https://hub.docker.com/r/caffe2ai/caffe2/),这里使用cuda8+cudnn7+ubuntu16的镜像
`docker pull caffe2ai/caffe2:c2v0.8.1.cuda8.cudnn7.ubuntu16.04`




