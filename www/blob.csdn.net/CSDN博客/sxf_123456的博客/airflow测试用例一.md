# airflow测试用例一 - sxf_123456的博客 - CSDN博客
2018年04月24日 10:52:05[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：262
from airflow.models import DAG
from airflow.operators.bash_operator import BashOperator
from datetime import datetime, timedelta
#default_args = dict()
default_args = {
    'owner': 'airflow',
'start_date': datetime(2018,4,24,10,23),
'depends_on_past': False,
'email': ['1574044873@qq.com'],
'email_on_failure': False,
'email_on_retry': False,
'retries': 1,
'retry_delay': timedelta(minutes=5)
}
#每5分钟执行一次
dag = DAG(dag_id='bash_test',default_args=default_args,schedule_interval='*/5 * * * *')
task_1 = BashOperator(
    task_id='bash_1',
bash_command='date',
dag=dag
)
#获取参数的值
cmd = """
{% for i in range(4) %}
    echo '{{ds}}'
    echo '{{params.my_param}}'
{% endfor %}
"""
task_2 = BashOperator(
    task_id='bash_2',
bash_command=cmd,
params={'my_param':'task_2'},
dag=dag
)
task_1.set_downstream(task_2)
