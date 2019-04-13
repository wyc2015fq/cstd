
# kubelet启动常见问题 - 知行合一 止于至善 - CSDN博客

2019年03月05日 07:44:06[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：192所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



kubelet无法启动，最常见的问题之一在于cgroup-driver的设定不正确，以下列出问题示例和对应方法。
# 现象示例
提示以下信息
`Feb 11 08:19:11 host141 kubelet[5278]: F0211 08:19:11.385889    5278 server.go:261] failed to run Kubelet: failed to create kubelet: misconfiguration: kubelet cgroup driver: "cgroupfs" is different from docker cgroup driver: "systemd"`
# 问题确认
确认启动信息
`08:16:06 host141 kubelet[5278]: I0211 08:16:06.214804    5278 flags.go:33] FLAG: --cgroup-driver="cgroupfs"`确认docker info的Cgroup
`[root@host141 node]# docker info |grep Cgroup
  WARNING: You're not using the default seccomp profile
Cgroup Driver: systemd
[root@host141 node]#`原因确认，两者不符合，保持一直即可
# 对应方式
做如下修正
–cgroup-driver=“cgroupfs”
->
–cgroup-driver=“systemd”
# 再次重启
再次重启之后，问题解决
`[root@host141 node]# kubectl get nodes -o wide
NAME              STATUS   ROLES    AGE   VERSION   INTERNAL-IP       EXTERNAL-IP   OS-IMAGE                KERNEL-VERSION          CONTAINER-RUNTIME
192.168.163.141   Ready    <none>   51s   v1.13.2   192.168.163.141   <none>        CentOS Linux 7 (Core)   3.10.0-693.el7.x86_64   docker://1.13.1
[root@host141 node]#`

