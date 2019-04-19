# Airflow基础知识 - sxf_123456的博客 - CSDN博客
2018年03月07日 17:08:31[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：700
转载:http://blog.csdn.net/u012965373/article/details/72878491
安装跳过，我的另一篇有专门的安装笔记
[Airflow安装](http://blog.csdn.net/u012965373/article/details/70877311)
安装过后开始看代码：
**[python]**[view plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- default_args = {  
- 'owner': 'airflow',  
- 'depends_on_past': False,  
- 'start_date': datetime.datetime(2015, 6, 1),  
- 'email': ['morefreeze@gmail.com'],  
- 'email_on_failure': False,  
- 'email_on_retry': False,  
- 'retries': 1,  
- 'retry_delay': datetime.timedelta(minutes=5),  
- # 'end_date': datetime(2016, 1, 1),
- }  
第三行的`depends_on_past` 表示就是是否依赖上一个自己的执行状态。 如果设置了 `email` 相关的配置，
需要在 `airflow.cfg` 中配置下发件邮箱。 因为这个任务会一直执行下去，所以我把结束时间注释掉了。
以上只是配置了 DAG 的参数，下面生成了一个 dag 对象：
```
dag = DAG(
    'tutorial', default_args=default_args, schedule_interval='40 09 * * *')
```
表示的是在9点40 的时候运行
这里我修改了下官方的例子，`schedule_interval` 表示执行的周期， 我改成了 crontab（时间触发）的形式，
这样更直观也方便修改， airflow 也提供一些字面意思的值表示执行周期，比如`@hourly`等，这会让脚本在X时0点执行，
 但如果真在线上执行，我们一般会将不同脚本错锋执行，不会全设成X时0分执行，所以我建议用 crontab 形式的写法更好。
下面就开始定义任务了，实际上，在定义这个任务的过程，就像是在写一个 shell 脚本，只是这个脚本的每个操作可以有依赖。
 不同的操作对应了不同的 Operator，比如 shell 就需要用 BashOperator 来执行。就像这样：
```
t1 = BashOperator(
    task_id='print_date',
    bash_command='date',
    dag=dag)
text = '{{ ds }} [%s] has been done' % (dag.dag_id)
t2 = SlackAPIPostOperator(
    task_id='post_slack',
    token='xoxp-your-key-here',
    channel='#random',
    username='airflow',
    text=text,
    dag=dag
)
t1 >> t2  # t2.set_upstream(t1)
```
再看一眼发的消息`text`，airflow 执行的命令或这种消息是支持 jinja2 模板语言，`{{ ds }}`是一种宏，表示当前的日期，
形如`2016-12-16`，支持的宏在[这里](https://airflow.incubator.apache.org/code.html#macros)。
最后一行就是设置依赖关系，显而易见，这是 t1 先执行，t2 在 t1 完成后执行， 也可以用注释里的写法
|Variable|Description|
|----|----|
|`{{ ds }}`|the execution date as `YYYY-MM-DD`|
|`{{ ds_nodash }}`|the execution date as `YYYYMMDD`|
|`{{ yesterday_ds }}`|yesterday’s date as `YYYY-MM-DD`|
|`{{ yesterday_ds_nodash }}`|yesterday’s date as `YYYYMMDD`|
|`{{ tomorrow_ds }}`|tomorrow’s date as `YYYY-MM-DD`|
|`{{ tomorrow_ds_nodash }}`|tomorrow’s date as `YYYYMMDD`|
|`{{ ts }}`|same as `execution_date.isoformat()`|
|`{{ ts_nodash }}`|same as `ts` without `-` and `:`|
|`{{ execution_date }}`|the execution_date, (datetime.datetime)|
|`{{ prev_execution_date }}`|the previous execution date (if available) (datetime.datetime)|
|`{{ next_execution_date }}`|the next execution date (datetime.datetime)|
|`{{ dag }}`|the DAG object|
|`{{ task }}`|the Task object|
|`{{ macros }}`|a reference to the macros package, described below|
|`{{ task_instance }}`|the task_instance object|
|`{{ end_date }}`|same as `{{ ds }}`|
|`{{ latest_date }}`|same as `{{ ds }}`|
|`{{ ti }}`|same as `{{ task_instance }}`|
|`{{ params }}`|a reference to the user-defined params dictionary|
|`{{ var.value.my_var }}`|global defined variables represented as a dictionary|
|`{{ var.json.my_var.path }}`|global defined variables represented as a dictionary with deserialized JSON object, append the path to the key within the JSON object|
|`{{ task_instance_key_str }}`|a unique, human-readable key to the task instance formatted `{dag_id}_{task_id}_{ds}`|
|`conf`|the full configuration object located at `airflow.configuration.conf` which represents the content of your `airflow.cfg`|
|`run_id`|the `run_id` of the current DAG run|
|`dag_run`|a reference to the DagRun object|
|`test_mode`|whether the task instance was called using the CLI’s test subcommand|
## 命令
airflow 的所有执行操作都需要在命令行下完成，这里不得不吐槽下，界面只能看任务的依赖， 包括任务执行状态，但如果任务失败了，还是要在命令行下执行，有些不人性化（当然你可以提个PR， :P）。
airflow 的命令总的来说很符合直觉，常用的有如下几个：
- test： 用于测试特定的某个task，不需要依赖满足
- run: 用于执行特定的某个task，需要依赖满足
- backfill: 执行某个DAG，会自动解析依赖关系，按依赖顺序执行
- unpause: 将一个DAG启动为例行任务，默认是关的，所以编写完DAG文件后一定要执行这和要命令，相反命令为pause
- scheduler: 这是整个 airflow 的调度程序，一般是在后台启动
- clear: 清除一些任务的状态，这样会让scheduler来执行重跑
## 一些概念
前面急于介绍 airflow 的例子，步子大有点扯着蛋，这里回过头来补充一些基础概念。
### DAG (Directed Acyclic Graph)
它表示的是一些任务的集合，描述了任务之间的依赖关系，以及整个DAG的一些属性， 比如起止时间，执行周期，重试策略等等。通常一个.py文件就是一个DAG。 你也可以理解为这就是一个完整的shell脚本，只是它可以保证脚本中的命令有序执行。
### task 任务
它就是DAG文件中的一个个Operator，它描述了具体的一个操作。
### Operator 执行器
airflow定义了很多的 Operator，通常一个操作就是一个特定的 Operator， 比如调用 shell 命令要用 BashOperator，调用 python 函数要用 PythonOperator， 发邮件要用 EmailOperator，连SSH要用 SSHOperator。社区还在不断地贡献新的 Operator。
## ds 日期
前面的脚本里用到了`{{ ds }}`变量，每个DAG在执行时都会传入一个具体的时间（datetime对象）， 这个`ds`就会在 render 命令时被替换成对应的时间。这里要特别强调一下， 对于周期任务，airflow传入的时间是上一个周期的时间（划重点），比如你的任务是每天执行， 那么今天传入的是昨天的日期，如果是周任务，那传入的是上一周今天的值。
## Macros
上一条说了`ds`变量，你肯定会说我的脚本里如果需要不同的时间格式或者不同的时间段怎么办， 这时候就到Macro出场了，airflow本身提供了几种时间格式，比如`ds_nodash`，顾名思义就是不带短横`-`的时间格式， 而且还会有一些相关的函数可以直接调用，比如`ds_add`可以对时间进行加减。
### executor
这是airflow最关键的一个配置，它指示了airflow以何种方式来执行任务。它有三个选项：
- SequentialExecutor：表示单进程顺序执行，通常只用于测试
- LocalExecutor：表示多进程本地执行，它用python的多进程库从而达到多进程跑任务的效果。
- CeleryExecutor：表示使用celery作为执行器，只要配置了celery，就可以分布式地多机跑任务，一般用于生产环境。
### sql_alchemy_conn
这个配置让你指定 airflow 的元信息用何种方式存储，默认用 sqlite，如果要部署到生产环境，推荐使用 mysql。
### smtp
如果你需要邮件通知或用到 EmailOperator 的话，需要配置发信的 smtp 服务器。
