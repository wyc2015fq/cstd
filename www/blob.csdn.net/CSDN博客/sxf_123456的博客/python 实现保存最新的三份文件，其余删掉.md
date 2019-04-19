# python 实现保存最新的三份文件，其余删掉 - sxf_123456的博客 - CSDN博客
2018年08月28日 21:52:40[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：164
```
"""
对于每天存储文件，文件数量过多，占用空间
采用保存最新的三个文件
"""
from airflow import DAG
from airflow.operators.python_operator import PythonOperator
from airflow.models import Variable
from sctetl.airflow.utils import dateutils
from datetime import datetime,timedelta
import logging
import os
import shutil
"""
base_dir = "/data"
data_dir = "/gather"
"gather下边存在不同的文件夹"
"/data/gather/test"
"test路径下有以下文件夹"
"20180812、20180813、20180814、20180815、20180816"
"""
base_dir = Variable.get("base_dir")
data_dir = Variable.get("data_dir")
keep = 3
default_arg = {
    "owner":"airflow",
    "depends_on_past":False,
    "start_date":dateutils.get_start_date_local(2018,8,27,18,5),
    "email":[''],
    "email_on_failure":False,
    "email_on_retry":False,
    "retries":1,
    "retry_delay":timedelta(minutes=5)
}
dag = DAG(dag_id="keep_three_day",default_args=default_arg,schedule_interval=dateutils.get_schedule_interval_local(18,5))
def keep_three_day():
    path = os.path.join(base_dir, data_dir)
    date_cates = os.listdir(path)
    for cate in date_cates:
        p = os.path.join(base_dir, data_dir, cate)
        if os.path.isdir(p):
            dir_names = os.listdir(p)
            dir_names.sort()
            for i in dir_names[:-keep]:
                logging.info("删除目录 {path}".format(path=os.path.join(p, i)))
                shutil.rmtree(os.path.join(p, i))
with dag:
    keep_three_file = PythonOperator(task_id="keep_three_file",python_callable=keep_three_day(),dag=dag)
keep_three_file
```
