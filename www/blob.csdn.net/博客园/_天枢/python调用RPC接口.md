# python调用RPC接口 - _天枢 - 博客园
## [python调用RPC接口](https://www.cnblogs.com/yhleng/p/9505813.html)
2018-08-20 15:33 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9505813)
要调用RPC接口，python提供了一个框架grpc，这是google开源的
rpc相关文档:
[https://grpc.io/docs/tutorials/basic/python.html](https://grpc.io/docs/tutorials/basic/python.html)
**需要安装的python包如下：**
1.grpc安装
pip install grpcio
2.grpc的python protobuf相关的编译工具
pip install grpcio-tools
3.protobuf相关python依赖库
pip install protobuf
4.一些常见原型的生成python类的集合：
pip install googleapis-common-protos
**编译protobuf文件：使用以下命令生成Python代码：**
python3 -m grpc_tools.protoc -I<目标路径目录> --python_out=. --grpc_python_out=<目标文件所在目录路径> <目标文件data.proto>
python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. data.proto
注意:**protobuf**文件，为定义服务接口代码文件，这里是data.proto
会生成：data_pb2.py 与 data_pb2_grpc.py
data_pb2.py是服务接口映射
data_pb2_grpc.py方法映射
protobuf内容示例：
```
syntax = "proto3";
package grpcDemo;
message HelloRequest {
   string name = 1;
}
message HelloReply {
   string message = 1;
}
service gRPC {
  rpc SayHello (HelloRequest) returns (HelloReply) {}
}
```
接口调用内容示例：
```
# -*- coding: utf-8 -*-
import grpc
import data_pb2,data_pb2_grpc
_HOST = 'localhost'
_PORT = '8080'
def run():
    conn = grpc.insecure_channel(_HOST + ':' + _PORT)
    client = data_pb2_grpc.gRPCStub(channel=conn)
    response = client.SayHello(data_pb2.HelloRequest(name='hello,world!'))
    print("received: " + response.text)
if __name__ == '__main__':
    run()
```

