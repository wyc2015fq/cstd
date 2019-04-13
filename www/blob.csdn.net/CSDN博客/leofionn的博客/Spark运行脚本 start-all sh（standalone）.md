
# Spark运行脚本 start-all.sh（standalone） - leofionn的博客 - CSDN博客


2019年03月02日 17:41:35[leofionn](https://me.csdn.net/qq_36142114)阅读数：60


`#!/usr/bin/env bash
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Start all spark daemons.
# Starts the master on this node.
# Starts a worker on each node specified in conf/slaves
if [ -z "${SPARK_HOME}" ]; then
  export SPARK_HOME="$(cd "`dirname "$0"`"/..; pwd)"
fi
# Load the Spark configuration
. "${SPARK_HOME}/sbin/spark-config.sh"   
# Start Master
"${SPARK_HOME}/sbin"/start-master.sh   
# Start Workers
"${SPARK_HOME}/sbin"/start-slaves.sh`1）先检查Spark的环境变量；
2）加载Spark的相关配置项：默认和自定义；
3） 执行start-master.sh，启动Master节点；
4）执行start-slaves.sh，启动所有的Worker节点。

