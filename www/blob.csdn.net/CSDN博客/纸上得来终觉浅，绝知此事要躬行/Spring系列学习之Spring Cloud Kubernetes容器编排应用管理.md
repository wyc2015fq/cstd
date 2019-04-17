# Spring系列学习之Spring Cloud Kubernetes容器编排应用管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月25日 20:43:55[boonya](https://me.csdn.net/boonya)阅读数：372








英文原文：[https://spring.io/projects/spring-cloud-kubernetes](https://spring.io/projects/spring-cloud-kubernetes)

**目录**

[概述](#%E6%A6%82%E8%BF%B0)

[kubernetes](#kubernetes)

[Kubernetes 特点](#Kubernetes%20%E7%89%B9%E7%82%B9)

[Kubernetes 组件](#Kubernetes%20%E7%BB%84%E4%BB%B6)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[学习](#%E5%AD%A6%E4%B9%A0)

[文档](#%E6%96%87%E6%A1%A3)

# 概述

## kubernetes

kubernetes，简称K8s，是用8代替8个字符“ubernete”而成的缩写。是一个开源的，用于管理云平台中多个主机上的容器化的应用，Kubernetes的目标是让部署容器化的应用简单并且高效（powerful）,Kubernetes提供了应用部署，规划，更新，维护的一种机制。 [1]

传统的应用部署方式是通过插件或脚本来安装应用。这样做的缺点是应用的运行、配置、管理、所有生存周期将与当前操作系统绑定，这样做并不利于应用的升级更新/回滚等操作，当然也可以通过创建虚拟机的方式来实现某些功能，但是虚拟机非常重，并不利于可移植性。

新的方式是通过部署容器方式实现，每个容器之间互相隔离，每个容器有自己的文件系统 ，容器之间进程不会相互影响，能区分计算资源。相对于虚拟机，容器能快速部署，由于容器与底层设施、机器文件系统解耦的，所以它能在不同云、不同版本操作系统间进行迁移。

容器占用资源少、部署快，每个应用可以被打包成一个容器镜像，每个应用与容器间成一对一关系也使容器有更大优势，使用容器可以在build或release 的阶段，为应用创建容器镜像，因为每个应用不需要与其余的应用堆栈组合，也不依赖于生产环境基础结构，这使得从研发到测试、生产能提供一致环境。类似地，容器比虚拟机轻量、更“透明”，这更便于监控和管理。

Kubernetes是Google开源的一个容器编排引擎，它支持自动化部署、大规模可伸缩、应用容器化管理。在生产环境中部署一个应用程序时，通常要部署该应用的多个实例以便对应用请求进行负载均衡。

在Kubernetes中，我们可以创建多个容器，每个容器里面运行一个应用实例，然后通过内置的负载均衡策略，实现对这一组应用实例的管理、发现、访问，而这些细节都不需要运维人员去进行复杂的手工配置和处理。



## Kubernetes 特点
- 
可移植: 支持公有云，私有云，混合云，多重云（multi-cloud）

- 
可扩展: 模块化, 插件化, 可挂载, 可组合

- 
自动化: 自动部署，自动重启，自动复制，自动伸缩/扩展




## Kubernetes 组件
- 
1Master 组件
- 
1.1kube-apiserver

- 
1.2ETCD

- 
1.3kube-controller-manager

- 
1.4cloud-controller-manager

- 
1.5kube-scheduler

- 
1.6插件 addons

- 
1.6.1DNS

- 
1.6.2用户界面

- 
1.6.3容器资源监测

- 
1.6.4Cluster-level Logging


- 
2节点（Node）组件
- 
2.1kubelet

- 
2.2kube-proxy

- 
2.3docker

- 
2.4RKT

- 
2.5supervisord

- 
2.6fluentd



本文介绍了Kubernetes集群所需的各种二进制组件。

[https://baike.baidu.com/item/kubernetes/22864162?fr=aladdin](https://baike.baidu.com/item/kubernetes/22864162?fr=aladdin)

## 快速开始

使用[Spring Initializr](https://start.spring.io/)引导您的应用程序。

# 学习

## 文档

每个Spring项目都有自己的; 它详细解释了如何使用项目功能以及使用它们可以实现的功能。
|2.1.0 RC1 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-kubernetes/2.1.0.RC1/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-kubernetes/2.1.0.RC1/)|
|----|----|----|
|1.0.0 RC2 PRE CURRENT|[Reference Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-kubernetes/1.0.0.RC2/)|[API Doc.](https://cloud.spring.io/spring-cloud-static/spring-cloud-kubernetes/1.0.0.RC2/)|



