# etcd官方教程 - walkabc - 博客园
# [etcd官方教程](https://www.cnblogs.com/hitfire/articles/6407630.html)
This series of examples shows the basic procedures for working with an etcd cluster.
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#set-up-a-cluster)Set up a cluster
![01_etcd_clustering_2016050601](https://camo.githubusercontent.com/c65fb09982d11f34aa9f6951d9c11fa27f9913ff/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30315f657463645f636c7573746572696e675f323031363035313030312e676966)
On each etcd node, specify the cluster members:
```
TOKEN=token-01
CLUSTER_STATE=new
NAME_1=machine-1
NAME_2=machine-2
NAME_3=machine-3
HOST_1=10.240.0.17
HOST_2=10.240.0.18
HOST_3=10.240.0.19
CLUSTER=${NAME_1}=http://${HOST_1}:2380,${NAME_2}=http://${HOST_2}:2380,${NAME_3}=http://${HOST_3}:2380
```
Run this on each machine:
```
# For machine 1
THIS_NAME=${NAME_1}
THIS_IP=${HOST_1}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 --listen-client-urls http://${THIS_IP}:2379 \
    --initial-cluster ${CLUSTER} \
    --initial-cluster-state ${CLUSTER_STATE} --initial-cluster-token ${TOKEN}
# For machine 2
THIS_NAME=${NAME_2}
THIS_IP=${HOST_2}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 --listen-client-urls http://${THIS_IP}:2379 \
    --initial-cluster ${CLUSTER} \
    --initial-cluster-state ${CLUSTER_STATE} --initial-cluster-token ${TOKEN}
# For machine 3
THIS_NAME=${NAME_3}
THIS_IP=${HOST_3}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 --listen-client-urls http://${THIS_IP}:2379 \
    --initial-cluster ${CLUSTER} \
    --initial-cluster-state ${CLUSTER_STATE} --initial-cluster-token ${TOKEN}
```
Or use our public discovery service:
```
curl https://discovery.etcd.io/new?size=3
https://discovery.etcd.io/a81b5818e67a6ea83e9d4daea5ecbc92
# grab this token
TOKEN=token-01
CLUSTER_STATE=new
NAME_1=machine-1
NAME_2=machine-2
NAME_3=machine-3
HOST_1=10.240.0.17
HOST_2=10.240.0.18
HOST_3=10.240.0.19
DISCOVERY=https://discovery.etcd.io/a81b5818e67a6ea83e9d4daea5ecbc92
THIS_NAME=${NAME_1}
THIS_IP=${HOST_1}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 --listen-client-urls http://${THIS_IP}:2379 \
    --discovery ${DISCOVERY} \
    --initial-cluster-state ${CLUSTER_STATE} --initial-cluster-token ${TOKEN}
THIS_NAME=${NAME_2}
THIS_IP=${HOST_2}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 --listen-client-urls http://${THIS_IP}:2379 \
    --discovery ${DISCOVERY} \
    --initial-cluster-state ${CLUSTER_STATE} --initial-cluster-token ${TOKEN}
THIS_NAME=${NAME_3}
THIS_IP=${HOST_3}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 --listen-client-urls http://${THIS_IP}:2379 \
    --discovery ${DISCOVERY} \
    --initial-cluster-state ${CLUSTER_STATE} --initial-cluster-token ${TOKEN}
```
Now etcd is ready! To connect to etcd with etcdctl:
```
export ETCDCTL_API=3
HOST_1=10.240.0.17
HOST_2=10.240.0.18
HOST_3=10.240.0.19
ENDPOINTS=$HOST_1:2379,$HOST_2:2379,$HOST_3:2379
etcdctl --endpoints=$ENDPOINTS member list
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#access-etcd)Access etcd
![02_etcdctl_access_etcd_2016051001](https://camo.githubusercontent.com/cfb0307c64d6add2fa594db105e64d0a3789eb4a/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30325f6574636463746c5f6163636573735f657463645f323031363035313030312e676966)
`put` command to write:
```
etcdctl --endpoints=$ENDPOINTS put foo "Hello World!"
```
`get` to read from etcd:
```
etcdctl --endpoints=$ENDPOINTS get foo
etcdctl --endpoints=$ENDPOINTS --write-out="json" get foo
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#get-by-prefix)Get by prefix
![03_etcdctl_get_by_prefix_2016050501](https://camo.githubusercontent.com/2f9b03e94050d156673d046120da994e8bc89b75/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30335f6574636463746c5f6765745f62795f7072656669785f323031363035303530312e676966)
```
etcdctl --endpoints=$ENDPOINTS put web1 value1
etcdctl --endpoints=$ENDPOINTS put web2 value2
etcdctl --endpoints=$ENDPOINTS put web3 value3
etcdctl --endpoints=$ENDPOINTS get web --prefix
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#delete)Delete
![04_etcdctl_delete_2016050601](https://camo.githubusercontent.com/b10572beab21615030d047720f20c1f3ac1087c5/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30345f6574636463746c5f64656c6574655f323031363035303630312e676966)
```
etcdctl --endpoints=$ENDPOINTS put key myvalue
etcdctl --endpoints=$ENDPOINTS del key
etcdctl --endpoints=$ENDPOINTS put k1 value1
etcdctl --endpoints=$ENDPOINTS put k2 value2
etcdctl --endpoints=$ENDPOINTS del k --prefix
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#transactional-write)Transactional write
`txn` to wrap multiple requests into one transaction:
![05_etcdctl_transaction_2016050501](https://camo.githubusercontent.com/c0a786c3272e3c9d0a8947be38519a47a2d63a6e/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30355f6574636463746c5f7472616e73616374696f6e5f323031363035303530312e676966)
```
etcdctl --endpoints=$ENDPOINTS put user1 bad
etcdctl --endpoints=$ENDPOINTS txn --interactive
compares:
value("user1") = "bad"      
success requests (get, put, delete):
del user1  
failure requests (get, put, delete):
put user1 good
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#watch)Watch
`watch` to get notified of future changes:
![06_etcdctl_watch_2016050501](https://camo.githubusercontent.com/4b75085b2bc84985c8799a231c383fc56b188515/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30365f6574636463746c5f77617463685f323031363035303530312e676966)
```
etcdctl --endpoints=$ENDPOINTS watch stock1
etcdctl --endpoints=$ENDPOINTS put stock1 1000
etcdctl --endpoints=$ENDPOINTS watch stock --prefix
etcdctl --endpoints=$ENDPOINTS put stock1 10
etcdctl --endpoints=$ENDPOINTS put stock2 20
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#lease)Lease
`lease` to write with TTL:
![07_etcdctl_lease_2016050501](https://camo.githubusercontent.com/841d86d73ee978b8d369672bdfc8c6178376feaa/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30375f6574636463746c5f6c656173655f323031363035303530312e676966)
```
etcdctl --endpoints=$ENDPOINTS lease grant 300
# lease 2be7547fbc6a5afa granted with TTL(300s)
etcdctl --endpoints=$ENDPOINTS put sample value --lease=2be7547fbc6a5afa
etcdctl --endpoints=$ENDPOINTS get sample
etcdctl --endpoints=$ENDPOINTS lease keep-alive 2be7547fbc6a5afa
etcdctl --endpoints=$ENDPOINTS lease revoke 2be7547fbc6a5afa
# or after 300 seconds
etcdctl --endpoints=$ENDPOINTS get sample
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#distributed-locks)Distributed locks
`lock` for distributed lock:
![08_etcdctl_lock_2016050501](https://camo.githubusercontent.com/e113052cd2c40b181f142c53964b0e839ea3ae7c/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30385f6574636463746c5f6c6f636b5f323031363035303530312e676966)
```
etcdctl --endpoints=$ENDPOINTS lock mutex1
# another client with the same name blocks
etcdctl --endpoints=$ENDPOINTS lock mutex1
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#elections)Elections
`elect` for leader election:
![09_etcdctl_elect_2016050501](https://camo.githubusercontent.com/9ff3e0b8a58fc17b56ae153a0b293cc23989ee7b/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f30395f6574636463746c5f656c6563745f323031363035303530312e676966)
```
etcdctl --endpoints=$ENDPOINTS elect one p1
# another client with the same name blocks
etcdctl --endpoints=$ENDPOINTS elect one p2
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#cluster-status)Cluster status
Specify the initial cluster configuration for each machine:
![10_etcdctl_endpoint_2016050501](https://camo.githubusercontent.com/1149413481c6c83f935ab83c450f290165676af1/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f31305f6574636463746c5f656e64706f696e745f323031363035303530312e676966)
```
etcdctl --write-out=table --endpoints=$ENDPOINTS endpoint status
+------------------+------------------+---------+---------+-----------+-----------+------------+
|     ENDPOINT     |        ID        | VERSION | DB SIZE | IS LEADER | RAFT TERM | RAFT INDEX |
+------------------+------------------+---------+---------+-----------+-----------+------------+
| 10.240.0.17:2379 | 4917a7ab173fabe7 | 3.0.0   | 45 kB   | true      |         4 |      16726 |
| 10.240.0.18:2379 | 59796ba9cd1bcd72 | 3.0.0   | 45 kB   | false     |         4 |      16726 |
| 10.240.0.19:2379 | 94df724b66343e6c | 3.0.0   | 45 kB   | false     |         4 |      16726 |
+------------------+------------------+---------+---------+-----------+-----------+------------+
```
```
etcdctl --endpoints=$ENDPOINTS endpoint health
10.240.0.17:2379 is healthy: successfully committed proposal: took = 3.345431ms
10.240.0.19:2379 is healthy: successfully committed proposal: took = 3.767967ms
10.240.0.18:2379 is healthy: successfully committed proposal: took = 4.025451ms
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#snapshot)Snapshot
`snapshot` to save point-in-time snapshot of etcd database:
![11_etcdctl_snapshot_2016051001](https://camo.githubusercontent.com/0dc4f3345f6d6d58ab3590640779ecb12297c0f4/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f31315f6574636463746c5f736e617073686f745f323031363035313030312e676966)
```
etcdctl --endpoints=$ENDPOINTS snapshot save my.db
Snapshot saved at my.db
```
```
etcdctl --write-out=table --endpoints=$ENDPOINTS snapshot status my.db
+---------+----------+------------+------------+
|  HASH   | REVISION | TOTAL KEYS | TOTAL SIZE |
+---------+----------+------------+------------+
| c55e8b8 |        9 |         13 | 25 kB      |
+---------+----------+------------+------------+
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#migrate)Migrate
`migrate` to transform etcd v2 to v3 data:
![12_etcdctl_migrate_2016061602](https://camo.githubusercontent.com/a99362d96c74b3fdb742aabb404a91cc9c12025f/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f31325f6574636463746c5f6d6967726174655f323031363036313630322e676966)
```
# write key in etcd version 2 store
export ETCDCTL_API=2
etcdctl --endpoints=http://$ENDPOINT set foo bar
# read key in etcd v2
etcdctl --endpoints=$ENDPOINTS --output="json" get foo
# stop etcd node to migrate, one by one
# migrate v2 data
export ETCDCTL_API=3
etcdctl --endpoints=$ENDPOINT migrate --data-dir="default.etcd" --wal-dir="default.etcd/member/wal"
# restart etcd node after migrate, one by one
# confirm that the key got migrated
etcdctl --endpoints=$ENDPOINTS get /foo
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#member)Member
`member` to add,remove,update membership:
![13_etcdctl_member_2016062301](https://camo.githubusercontent.com/bfa38f316207466af990859db176b16d835da9e3/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f31335f6574636463746c5f6d656d6265725f323031363036323330312e676966)
```
# For each machine
TOKEN=my-etcd-token-1
CLUSTER_STATE=new
NAME_1=etcd-node-1
NAME_2=etcd-node-2
NAME_3=etcd-node-3
HOST_1=10.240.0.13
HOST_2=10.240.0.14
HOST_3=10.240.0.15
CLUSTER=${NAME_1}=http://${HOST_1}:2380,${NAME_2}=http://${HOST_2}:2380,${NAME_3}=http://${HOST_3}:2380
# For node 1
THIS_NAME=${NAME_1}
THIS_IP=${HOST_1}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 \
    --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 \
    --listen-client-urls http://${THIS_IP}:2379 \
    --initial-cluster ${CLUSTER} \
    --initial-cluster-state ${CLUSTER_STATE} \
    --initial-cluster-token ${TOKEN}
# For node 2
THIS_NAME=${NAME_2}
THIS_IP=${HOST_2}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 \
    --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 \
    --listen-client-urls http://${THIS_IP}:2379 \
    --initial-cluster ${CLUSTER} \
    --initial-cluster-state ${CLUSTER_STATE} \
    --initial-cluster-token ${TOKEN}
# For node 3
THIS_NAME=${NAME_3}
THIS_IP=${HOST_3}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 \
    --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 \
    --listen-client-urls http://${THIS_IP}:2379 \
    --initial-cluster ${CLUSTER} \
    --initial-cluster-state ${CLUSTER_STATE} \
    --initial-cluster-token ${TOKEN}
```
Then replace a member with `member remove` and `member add` commands:
```
# get member ID
export ETCDCTL_API=3
HOST_1=10.240.0.13
HOST_2=10.240.0.14
HOST_3=10.240.0.15
etcdctl --endpoints=${HOST_1}:2379,${HOST_2}:2379,${HOST_3}:2379 member list
# remove the member
MEMBER_ID=278c654c9a6dfd3b
etcdctl --endpoints=${HOST_1}:2379,${HOST_2}:2379,${HOST_3}:2379 \
    member remove ${MEMBER_ID}
# add a new member (node 4)
export ETCDCTL_API=3
NAME_1=etcd-node-1
NAME_2=etcd-node-2
NAME_4=etcd-node-4
HOST_1=10.240.0.13
HOST_2=10.240.0.14
HOST_4=10.240.0.16 # new member
etcdctl --endpoints=${HOST_1}:2379,${HOST_2}:2379 \
    member add ${NAME_4} \
    --peer-urls=http://${HOST_4}:2380
```
Next, start the new member with `--initial-cluster-state existing` flag:
```
# [WARNING] If the new member starts from the same disk space,
# make sure to remove the data directory of the old member
#
# restart with 'existing' flag
TOKEN=my-etcd-token-1
CLUSTER_STATE=existing
NAME_1=etcd-node-1
NAME_2=etcd-node-2
NAME_4=etcd-node-4
HOST_1=10.240.0.13
HOST_2=10.240.0.14
HOST_4=10.240.0.16 # new member
CLUSTER=${NAME_1}=http://${HOST_1}:2380,${NAME_2}=http://${HOST_2}:2380,${NAME_4}=http://${HOST_4}:2380
THIS_NAME=${NAME_4}
THIS_IP=${HOST_4}
etcd --data-dir=data.etcd --name ${THIS_NAME} \
    --initial-advertise-peer-urls http://${THIS_IP}:2380 \
    --listen-peer-urls http://${THIS_IP}:2380 \
    --advertise-client-urls http://${THIS_IP}:2379 \
    --listen-client-urls http://${THIS_IP}:2379 \
    --initial-cluster ${CLUSTER} \
    --initial-cluster-state ${CLUSTER_STATE} \
    --initial-cluster-token ${TOKEN}
```
## [](https://github.com/coreos/etcd/blob/master/Documentation/demo.md#auth)Auth
`auth`,`user`,`role` for authentication:
![14_etcdctl_auth_2016062301](https://camo.githubusercontent.com/cb714b8e0578ebd84f3c93ab630de447b50ae943/68747470733a2f2f73746f726167652e676f6f676c65617069732e636f6d2f657463642f64656d6f2f31345f6574636463746c5f617574685f323031363036323330312e676966)
```
export ETCDCTL_API=3
ENDPOINTS=localhost:2379
etcdctl --endpoints=${ENDPOINTS} role add root
etcdctl --endpoints=${ENDPOINTS} role grant-permission root readwrite foo
etcdctl --endpoints=${ENDPOINTS} role get root
etcdctl --endpoints=${ENDPOINTS} user add root
etcdctl --endpoints=${ENDPOINTS} user grant-role root root
etcdctl --endpoints=${ENDPOINTS} user get root
etcdctl --endpoints=${ENDPOINTS} auth enable
# now all client requests go through auth
etcdctl --endpoints=${ENDPOINTS} --user=root:123 put foo bar
etcdctl --endpoints=${ENDPOINTS} get foo
etcdctl --endpoints=${ENDPOINTS} --user=root:123 get foo
etcdctl --endpoints=${ENDPOINTS} --user=root:123 get foo1
```

