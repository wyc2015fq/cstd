# hbase的shell脚本是通过ruby实现的 - zj360202的专栏 - CSDN博客





2013年03月20日 17:39:10[zj360202](https://me.csdn.net/zj360202)阅读数：1010
个人分类：[hbase](https://blog.csdn.net/zj360202/article/category/1298894)









hbase中的shell命令是通过ruby实现的，所有的命令都在

```java
hbase-0.92.1-cdh4.1.2\src\main\ruby
```


目录下面，在shell中通过hbase shell启动hbase-->hirb.rb 加载ruby文件，通过
```java
hbase-0.92.1-cdh4.1.2\src\main\ruby\shell.rb
```

中配置的命令实现对hbase的操作，下面是shell.rb中配置部分代码。

```
# Load commands base class
require 'shell/commands'

# Load all commands
Shell.load_command_group(
  'general',
  :full_name => 'GENERAL HBASE SHELL COMMANDS',
  :commands => %w[
    status
    version
    whoami
  ]
)

Shell.load_command_group(
  'ddl',
  :full_name => 'TABLES MANAGEMENT COMMANDS',
  :commands => %w[
    alter
    create
    describe
    disable
    disable_all
    is_disabled
    drop
    drop_all
    enable
    enable_all
    is_enabled
    exists
    list
    show_filters
    alter_status
    alter_async
  ]
)

Shell.load_command_group(
  'dml',
  :full_name => 'DATA MANIPULATION COMMANDS',
  :commands => %w[
    count
    delete
    deleteall
    get
    get_counter
    incr
    put
    scan
    truncate
  ]
)

Shell.load_command_group(
  'tools',
  :full_name => 'HBASE SURGERY TOOLS',
  :comment => "WARNING: Above commands are for 'experts'-only as misuse can damage an install",
  :commands => %w[
    assign
    balancer
    balance_switch
    close_region
    compact
    flush
    major_compact
    move
    split
    unassign
    zk_dump
    hlog_roll
  ]
)

Shell.load_command_group(
  'replication',
  :full_name => 'CLUSTER REPLICATION TOOLS',
  :comment => "In order to use these tools, hbase.replication must be true. enabling/disabling is currently unsupported",
  :commands => %w[
    add_peer
    remove_peer
    list_peers
    enable_peer
    disable_peer
    start_replication
    stop_replication
  ]
)

Shell.load_command_group(
  'security',
  :full_name => 'SECURITY TOOLS',
  :comment => "NOTE: Above commands are only applicable if running with the AccessController coprocessor",
  :commands => %w[
    grant
    revoke
    user_permission
  ]
)
```



如果要更深入了解可以访问[http://www.tuicool.com/articles/BbuaQr](http://www.tuicool.com/articles/BbuaQr)



