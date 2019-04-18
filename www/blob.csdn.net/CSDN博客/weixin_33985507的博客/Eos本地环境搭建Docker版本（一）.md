# Eos本地环境搭建Docker版本（一） - weixin_33985507的博客 - CSDN博客
2018年11月14日 23:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
我的电脑是Macpro，系统版本macOS High Sierra 版本 10.13.6
如果你的机器没有安装Docker容器，可以在如下链接下载：
[https://www.docker.com/community-edition](https://www.docker.com/community-edition)
第一步：获得eos的docker版本编译好的镜像
`docker pull eosio/eos:v1.4.2`
第二步：启动节点和钱包
```
docker run --name eosio \
  --publish 7777:7777 \
  --publish 127.0.0.1:5555:5555 \
  --volume /Users/wanghaidong/contracts:/Users/****/contracts \
  --detach \
  eosio/eos:v1.4.2 \
  /bin/bash -c \
  "keosd --http-server-address=0.0.0.0:5555 & exec nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::history_plugin --plugin eosio::history_api_plugin --plugin eosio::http_plugin -d /mnt/dev/data --config-dir /mnt/dev/config --http-server-address=0.0.0.0:7777 --access-control-allow-origin=* --contracts-console --http-validate-host=false --filter-on='*'"
```
第三步：
1.检查安装情况
在命令行中运行如下命令：
`docker logs --tail 10 eosio`
如果安装成功你会看到类似如下的返回
```
1929001ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000366974ce4e2a... #13929 @ 2018-05-23T16:32:09.000 signed by eosio [trxs: 0, lib: 13928, confirmed: 0]
1929502ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000366aea085023... #13930 @ 2018-05-23T16:32:09.500 signed by eosio [trxs: 0, lib: 13929, confirmed: 0]
1930002ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000366b7f074fdd... #13931 @ 2018-05-23T16:32:10.000 signed by eosio [trxs: 0, lib: 13930, confirmed: 0]
1930501ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000366cd8222adb... #13932 @ 2018-05-23T16:32:10.500 signed by eosio [trxs: 0, lib: 13931, confirmed: 0]
1931002ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000366d5c1ec38d... #13933 @ 2018-05-23T16:32:11.000 signed by eosio [trxs: 0, lib: 13932, confirmed: 0]
1931501ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000366e45c1f235... #13934 @ 2018-05-23T16:32:11.500 signed by eosio [trxs: 0, lib: 13933, confirmed: 0]
1932001ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000366f98adb324... #13935 @ 2018-05-23T16:32:12.000 signed by eosio [trxs: 0, lib: 13934, confirmed: 0]
1932501ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 00003670a0f01daa... #13936 @ 2018-05-23T16:32:12.500 signed by eosio [trxs: 0, lib: 13935, confirmed: 0]
1933001ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 00003671e8b36e1e... #13937 @ 2018-05-23T16:32:13.000 signed by eosio [trxs: 0, lib: 13936, confirmed: 0]
1933501ms thread-0   producer_plugin.cpp:585       block_production_loo ] Produced block 0000367257fe1623... #13938 @ 2018-05-23T16:32:13.500 signed by eosio [trxs: 0, lib: 13937, confirmed: 0]
```
2.查看本地钱包
打开eosio命令行，这步很关键，最开始就是没有运行如下命令导致无法进行下去。
`docker exec -it eosio bash`
查看本地钱包列表，运行如下命令：
`cleos --wallet-url http://127.0.0.1:5555 wallet list`
你能看到如下返回
```
Wallets:
[]
```
现在退出shell
`exit`
现在`keosd`已经正确运行了，输入`exit`然后回车离开`keosd`命令行。从这以后，你无须再进入容器的bash页面，可以直接从系统的命令行工具执行命令。
3.检查Nedeos 端口
有两种方法检查 远程RPC API是否正确运行，选择任意一种
- 在浏览器打开 [http://localhost:7777/v1/chain/get_info](http://localhost:7777/v1/chain/get_info)
- 在你的主机的命令行中执行如下命令
```
curl http://localhost:7777/v1/chain/get_info
```
有用的Docker命令行
启动/停止 容器
```
docker start eosio
docker stop eosio
```
打开bash命令行
`docker exec -it eosio bash`
删除 EOSIO 容器
`docker rm eosio`
