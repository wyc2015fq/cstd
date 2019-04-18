# glusterfs and heketi(服务器版) - weixin_33985507的博客 - CSDN博客
2017年04月19日 18:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
[http://neharawat.in/glusterfs-volume-management-using-heketi/](https://link.jianshu.com?t=http://neharawat.in/glusterfs-volume-management-using-heketi/)
[http://gluster.readthedocs.io/en/latest/Quick-Start-Guide/Quickstart/](https://link.jianshu.com?t=http://gluster.readthedocs.io/en/latest/Quick-Start-Guide/Quickstart/)
[https://github.com/heketi/heketi/wiki](https://link.jianshu.com?t=https://github.com/heketi/heketi/wiki)
# 2台服务器组成glusterfs
10.9.5.97   cloud4ourself-c1
10.9.5.96   cloud4ourself-c2
# 每个服务器安装
```
yum install centos-release-gluster -y
yum install glusterfs-server -y
systemctl enable glusterd
systemctl start glusterd
```
# 安装heketi(单台)
```
yum install epel-release -y
yum install heketi heketi-client -y
#修改/etc/heketi/heketi.json，开启认证
#"use_auth": true,
```
# 开启ssh免密访问
```
ssh-keygen -f /etc/heketi/heketi_key
chmod 600 /etc/heketi/heketi_key.pub
ssh-copy-id -i /etc/heketi/heketi_key.pub xzx@cloud4ourself-c1
ssh-copy-id -i /etc/heketi/heketi_key.pub xzx@cloud4ourself-c2
#要求xzx用户有sudo权限
chown heketi:heketi /etc/heketi/heketi_key*
```
# 配置heketi
修改/etc/heketi/heketi.json
```
"executor": "ssh",
    "_sshexec_comment": "SSH username and private key file information",
    "sshexec": {
      "keyfile": "/etc/heketi/heketi_key",
      "user": "xzx",
      "sudo": true
    },
```
# 创建 volume
```
systemctl restart heketi
heketi-cli --server http://localhost:8080 --user admin --secret "My Secret" cluster list
#初始为空
cat topology-sample.json
{
    "clusters": [
        {
            "nodes": [
                {
                    "node": {
                        "hostnames": {
                            "manage": [
                                "10.9.5.97"
                            ],
                            "storage": [
                                "10.9.5.97"
                            ]
                        },
                        "zone": 1
                    },
                    "devices": [
                        "/dev/vdb"
                    ]
                },
                {
                    "node": {
                        "hostnames": {
                            "manage": [
                                "10.9.5.96"
                            ],
                            "storage": [
                                "10.9.5.96"
                            ]
                        },
                        "zone": 2
                    },
                    "devices": [
                        "/dev/vdb"
                    ]
                }
            ]
        }
    ]
}
#创建cluster
heketi-cli --server http://localhost:8080 --user admin --secret "My Secret" topology load --json=topology-sample.json
#创建volume
heketi-cli --server http://localhost:8080 --user admin --secret "My Secret" volume create --size=10 --replica=2
#查看节点
heketi-cli --server http://localhost:8080 --user admin --secret "My Secret" node list
Id:2dde7cbda19d6a1ae9eb35aa0cf062fd Cluster:d1003037facb9c4f1dbe1ddbcea432ed
Id:4e6b2903df795c9cf3d35d355123d342 Cluster:d1003037facb9c4f1dbe1ddbcea432ed
```
# 登录gluster 查看
```
[root@cloud4ourself-c1 heketi]# gluster peer status
Number of Peers: 2
Hostname: cloud4ourself-c2
Uuid: 297f78e4-c88c-4f80-9dd3-00b24a2be594
State: Peer in Cluster (Connected)
Other names:
10.9.5.96
Hostname: cloud4ourself-c3
Uuid: 9b4b8e63-e596-4271-8779-2a34023e5674
State: Peer in Cluster (Connected)
[root@cloud4ourself-c1 heketi]# gluster volume list
vol_f6e2aa6c4489d57fec8cc20fdeba8269
```
# 后续
测试k8s 使用gluster做pv动态提供
