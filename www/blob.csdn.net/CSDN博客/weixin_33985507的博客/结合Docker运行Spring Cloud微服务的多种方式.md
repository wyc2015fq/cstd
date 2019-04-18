# 结合Docker运行Spring Cloud微服务的多种方式 - weixin_33985507的博客 - CSDN博客
2019年01月13日 03:07:34[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
如何通过以下方式在Docker中运行Spring-Cloud MicroService拓扑：
- Docker端口映射到外部主机网络
- Docker Bridge网络
**1. 从Docker机器的外部访问Docker容器中微服务**
Eureka和MicroServices可通过docker机器的静态IP地址访问。客户端只需获取eureka注册表并通过docker-machine的IP调用特定的MicroService，并将容器的端口映射到客户端的网络端口。
如果设置eureka.instance.prefer-ip-address = true，则使用Eureka的MicroService注册将使用eureka.instance.ip-address指定的IP地址（解析后默认为主机的IP）。
因此MicroService Spring Boot属性文件将是：
```bash
eureka.client.serviceUrl.defaultZone=http:<font><i>//${DOCKER_HOST_IP}:9761/eureka</i></font><font>
.
eureka.instance.preferIpAddress=<b>true</b>
eureka.instance.ip-address=${DOCKER_HOST_IP}
.
</font>复制代码
```
设置说明：
- 在Eureka上注册此MicroService，运行时间为： [http://${DOCKER_HOST_IP}:9761 / eureka](https://link.juejin.im?target=http%3A%2F%2F%2524%257Bdocker_host_ip%257D%3A9761%2Feureka)
- MicroService将通过$ {DOCKER_HOST_IP}系统属性指定的IP暴露给docker的外部世界。我们将在这里注入docker机器的当前IP地址。
同样在 eureka server端配置：
```bash
eureka.instance.prefer-ip-address=<b>true</b>
eureka.instance.ip-address=${DOCKER_HOST_IP}复制代码
```
Eureka服务器实例将在$ {DOCKER_HOST_IP}上运行并可访问。
**Docker maven集成**
作为Docker Maven插件，我选择了 [Spotify的Docker-Maven-Plugin](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fspotify%2Fdocker-maven-plugin) 。功能齐全。
Eureka Dockerfile：
```bash
FROM java:8-jre
MAINTAINER Tomas Kloucek <tomas.kloucek@embedit.cz>
ADD ./demo-0.0.1-SNAPSHOT.war /app/
CMD [<font>"java"</font><font>, </font><font>"-Xmx512m"</font><font>, </font><font>"-jar"</font><font>, </font><font>"/app/demo-0.0.1-SNAPSHOT.war"</font><font>]
</font>复制代码
```
Eureka pom.xml (plugin configuration)：
```bash
<plugin>
                <groupId>com.spotify</groupId>
                <artifactId>docker-maven-plugin</artifactId>
                <version>0.4.11</version>
                <configuration>
                    <imageName>${docker.image.prefix}/${project.artifactId}</imageName>
                    <dockerDirectory>src/docker</dockerDirectory>
                    <resources>
                        <resource>
                            <targetPath>/</targetPath>
                            <directory>${project.build.directory}</directory>
                            <include>${project.build.finalName}.war</include>
                        </resource>
                    </resources>
                </configuration>
            </plugin>复制代码
```
构建Docker镜像并在容器中运行它们：
```bash
(in the spring-microservice-registry folder)
mvn clean install docker:build
docker images (verify your image was built)
docker run --env DOCKER_HOST_IP=$(boot2docker ip) -it --name eureka -p 9761:9761 registry/demo复制代码
```
访问 [http://DOCKER_HOST_IP:9761/](https://link.juejin.im?target=http%3A%2F%2Fdocker_host_ip%3A9761%2F) 确认eureka服务器正在运行
微服务客户端启动：
```bash
(in the spring-microservice-service folder)
mvn clean install -DskipTests docker:build
docker images (verify your image was built)
docker run --env DOCKER_HOST_IP=$(boot2docker ip) -it --name service -p 8080:8080 service/demo
docker ps (verify that service container is running)复制代码
```
**从外部调用MicroService**
外部客户只需要知道eureka服务器的地址
application.properties：
```bash
eureka.client.serviceUrl.defaultZone=http:<font><i>//${DOCKER_HOST_IP}:9761/eureka</i></font><font>
</font>复制代码
```
测试在Docker中启动的微服务：
```bash
(in the spring-microservice-client folder) mvn clean install java -jar -DDOCKER_HOST_IP=$(boot2docker ip) target/demo-0.0.1-SNAPSHOT.war复制代码
```
2.Docker Links
eureka服务器application.properties：
```bash
eureka.instance.prefer-ip-address = false 
eureka.instance.hostname = eureka复制代码
```
通过此设置，Eureka服务器注册表将可以在地址 [http://eureka:<port>/eureka](https://link.juejin.im?target=https%3A%2F%2Fwww.jdon.com%2Fhttp%3A%2F%2Feureka%3A%253Cport%253E%2Feureka) 下载给客户端,
在Docker中启动Eureka注册表:
```bash
(in the spring-microservice-registry folder)
mvn clean install docker:build
docker images (verify your image was built)
docker run -it -p 9761:9761 --name eureka registry/demo复制代码
```
我只使用端口映射，因为我想使用eureka网页。重要的是使用容器名。
让我们将先前配置的Eureka设置为MicroService的注册表源，在微服务项目中配置：
```bash
spring.application.name=personsService
eureka.client.serviceUrl.defaultZone=http:<font><i>//eureka:9761/eureka</i></font><font>
eureka.client.healthcheck.enabled=<b>true</b>
eureka.instance.preferIpAddress=false
eureka.instance.hostname=service
ribbon.eureka.enabled=<b>true</b>
</font>复制代码
```
启动Docker中微服务：
```bash
(in the spring-microservice-service folder)
mvn clean install -DskipTests docker:build
docker images (verify your image was built)
docker run -it --link eureka:eureka --name service service/demo
docker ps (verify that service container is running)复制代码
```
使用“docker run”在service/demo容器将在别名eureka下看到Eureka服务器，让我们验证（运行以下命令）：
```bash
docker exec -it service bash (<b>this</b> will connect you to the MicroService cont.)
ping eureka复制代码
```
输出：
```bash
ping eureka
PING eureka (172.17.0.2): 56 data bytes
64 bytes from 172.17.0.2: icmp_seq=0 ttl=64 time=0.269 ms
64 bytes from 172.17.0.2: icmp_seq=1 ttl=64 time=0.084 ms复制代码
```
**网关容器**
网关容器也需要链接到eureka和服务容器，因为eureka将以 [http://service：8080](https://link.juejin.im?target=http%3A%2F%2Fservice%3A8080%2F) 的形式提供ribbon负载平衡的MicroService地址。因此网关需要了解“服务”的含义。
application.properties:
```bash
server.port=8888
hystrix.command.invokePersonsMicroService.fallback.enabled=<b>true</b>
hystrix.command.invokePersonsMicroService.metrics.rollingStats.timeInMilliseconds=35000
hystrix.command.invokePersonsMicroService.circuitBreaker.sleepWindowInMilliseconds=5000
hystrix.command.invokePersonsMicroService.circuitBreaker.requestVolumeThreshold=6
hystrix.command.invokePersonsMicroService.circuitBreaker.errorThresholdPercentage=100
hystrix.command.invokePersonsMicroService.execution.isolation.strategy=THREAD
eureka.client.serviceUrl.defaultZone=http:<font><i>//eureka:9761/eureka</i></font><font>
eureka.client.healthcheck.enabled=<b>true</b>
eureka.client.registryFetchIntervalSeconds=5
eureka.client.fetch-registry=<b>true</b>
</font>复制代码
```
运行Docker中网关：
```bash
(in the spring-microservice-client folder)
mvn clean install -DskipTests docker:build
docker images (verify that image was built)
docker run -it --link eureka:eureka --link service:service --name client client/demo复制代码
```
运行镜像后，您应该再次看到重复输出：
```bash
Simple MicroService Feign invocation result :{<font>"persons"</font><font>:[{</font><font>"name"</font><font>:</font><font>"Tomas"</font><font>,</font><font>"surname"</font><font>:</font><font>"Kloucek"</font><font>,</font><font>"department"</font><font>:</font><font>"Programmer"</font><font>}]}
</font>复制代码
```
**3. Docker桥接网络**
展示如何在所有节点之间创建基本的“桥梁”，这样他们就能看到对方！那么不需要手动Docker链接。
运行以下命令：
`docker network create spring-cloud-network复制代码`
既然我们现在有了一个网络，让我们把所有的节点放在其中
Maven编译和镜像构建与前面部分相同。
要启动eureka容器，请使用：
```bash
docker run -i -t --name eureka --net spring-cloud-network -p 9761:971 registry/demo复制代码
```
要启动MicroService容器，请使用：
`docker run -it --name service --net spring-cloud-network service/demo复制代码`
网格客户端，启动：
`docker run -it --name client --net spring-cloud-network client/demo复制代码`
让我们测试是否看到彼此：
```bash
docker exec -it service bash
60623256:/# ping eureka
PING eureka (172.19.0.2): 56 data bytes
64 bytes from 172.19.0.2: icmp_seq=0 ttl=64 time=0.085 ms
64 bytes from 172.19.0.2: icmp_seq=1 ttl=64 time=0.100 ms
64 bytes from 172.19.0.2: icmp_seq=2 ttl=64 time=0.110 ms复制代码
```
或
```bash
ping client
PING client (172.19.0.4): 56 data bytes
64 bytes from 172.19.0.4: icmp_seq=0 ttl=64 time=0.110 ms
64 bytes from 172.19.0.4: icmp_seq=1 ttl=64 time=0.106 ms
64 bytes from 172.19.0.4: icmp_seq=2 ttl=64 time=0.089 ms复制代码
```
如果您对每个容器彼此看到的拓扑结构都很好，那么基本的docker bridge网络就可以实现了，但请记住它只能在一个主机上运行。对于多个主机，您需要使用Overlay网络或服务网格
