# ftp服务器为我方，对方定时提供数据(客户基本信息及客户关联账户信息)导入es - sxf_123456的博客 - CSDN博客
2018年05月19日 16:11:49[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：46
*"""**ftp服务器为我方,对方定时提供数据,(客户基本信息及客户关联账户信息)导入es**ftp conn_id 在airflow webserver 中的Admin->Connections 按照ftp模板配置**003_客户基本信息**004_客户关联业务账号信息**"""*from airflow import DAG
from airflow.operators.python_operator import PythonOperator
from sctetl.airflow.contrib.sensors.ftp_sensor import FTPSensorOperator
from sctetl.airflow.contrib.hooks.es_hook import ElasticsearchHook
from sctetl.airflow.utils import dateutils
from datetime import datetime, timedelta
import os
default_args = {
    'owner': 'airflow',
'depends_on_past': False,
'start_date': dateutils.get_start_date_local(2018, 5, 23, 1, 30),
'email': [''],
'email_on_failure': False,
'email_on_retry': False,
'retries': 1,
'retry_delay': timedelta(minutes=5),
'provide_context': True
}
dag = DAG('m_003_004_crm_base_account',
default_args=default_args,
schedule_interval=dateutils.get_schedule_interval_local(1, 30))
def get_ftp(p_task, remote_path, **kwargs):
    *"""**    获取ftp文件路径****:param**** p_task:****:param**** remote_path: 本地存储ftp文件路径****:param**** kwargs:****:return****:**    """*ti = kwargs['ti']
    files = ti.xcom_pull(task_ids=p_task)
    filename = files[0]
    kwargs.get("")
    local_file = os.path.join(remote_path, filename)
    return local_file
def add_parameters(**kwargs):
    params = {}
    params.update(kwargs)
    return params
def str_to_float(s, dot_len=2):
    rs = None
    try:
        rs = round(float(s), dot_len)
    finally:
        return rs
def unix_str_to_date(s):
    rs = None
    try:
        rs = datetime.utcfromtimestamp(int(s))
    finally:
        return rs
def write_es_003(p_task, **kwargs):
    *"""**     客户基本信息数据写入es****:param**** p_task:****:param**** kwargs:****:return****:**    """*file_base = kwargs['ti'].xcom_pull(p_task)
    time_ = dateutils.get_local_ds_nodash(kwargs['execution_date'])
    index_ = "gather-003-{0}".format(time_)
    type_ = "003"
def foo():
        with open(file_base, 'r', encoding="GB18030") as f:
            i = 0
for row_ in f:
                row = row_.strip().encode("utf-8").decode("utf-8").split("|#")
                i += 1
if i <= 1 or len(row) != 9:  # 过滤字段名称及异常数据过滤
continue
source = add_parameters(
                    CUST_ID=row[0],
CUST_NAME=row[1],
ADDRESS=row[2],
GRADE=row[3],
REGION_ID=row[4],
SVR_LEVEL=row[5],
NO=row[6],
CUST_TYPE_ID=row[7],
GRADE_ID=row[8]
                )
                source['@timestamp'] = datetime.now()
                action = {
                    "_index": index_,
"_type": type_,
"_source": source
                }
                yield action
    es_hook = ElasticsearchHook(conn_id='es_hn')
    if es_hook.exists(index_):
        es_hook.delete_index(index_)
    es_hook.bulk(foo())
    es_hook.add_alias_and_delete_old("gather-003", index_)
def write_es_004(p_task, **kwargs):
    *"""**    客户关联业务账号写入es****:param**** p_task:****:param**** kwargs:****:return****:**    """*time_ = dateutils.get_local_ds_nodash(kwargs['execution_date'])
    index_ = "gather-004-{0}".format(time_)
    type_ = "004"
file_account = kwargs['ti'].xcom_pull(p_task)
    def foo():
        with open(file_account, 'r', encoding="GB18030") as f:
            i = 0
for row_ in f:
                row = row_.strip().encode("utf-8").decode("utf-8").split("|#")
                i += 1
if i <= 1 or len(row) != 10:  # 过滤字段名称及异常数据过滤
continue
source = add_parameters(
                    ID=row[0],
STAND_ADDR=row[1],
INSTALL_ADDR=row[2],
NAME=row[3],
TYPE=row[4],
CUSTOMER_ID=row[5],
ACCOUNT_NO=row[6],
JN_TIME=row[7],
AREA_ID=row[8],
ACCESS_TYPE=row[9],
)
                source['@timestamp'] = datetime.now()
                action = {
                    "_index": index_,
"_type": type_,
"_source": source
                }
                yield action
    es_hook = ElasticsearchHook(conn_id="es_hn")
    if es_hook.exists(index_):
        es_hook.delete_index(index_)
    es_hook.bulk(foo())
    es_hook.add_alias_and_delete_old("gather-004", index_)
with dag:
    # 等待003文件到达
t_0 = FTPSensorOperator(conn_id="ftp_crm_id",
dir_name="/",
pattern="PUB_CUST_{0}.txt".format(dateutils.get_macro_ds_local()),
task_id="PUB_CUST_file_exists",
timeout=60 * 10)
    # 等待004文件到达
t0 = FTPSensorOperator(conn_id="ftp_crm_id",
dir_name="/",
pattern="RTS_CUSTOMER_PROD_{0}.txt".format(dateutils.get_macro_ds_local()),
task_id="RTS_CUSTOMER_PROD_file_exists",
timeout=60 * 10)
    # 003客户基础信息文件路径
t_1 = PythonOperator(python_callable=get_ftp,
op_kwargs={"p_task":"PUB_CUST_file_exists",
"remote_path": "/data/ftpuserhome/crm"},
task_id="get_ftp_base")
    # 004客户关联业务账号信息文件路径
t1 = PythonOperator(python_callable=get_ftp,
op_kwargs={"p_task":"RTS_CUSTOMER_PROD_file_exists",
"remote_path": "/data/ftpuserhome/crm"},
task_id="get_ftp_account")
    # 003写入es
t_2 = PythonOperator(python_callable=write_es_003,
op_kwargs={"p_task":"get_ftp_base"},
task_id="base_insert_es")
    # 004写入es
t2 = PythonOperator(python_callable=write_es_004,
op_kwargs={"p_task": "get_ftp_account"},
task_id="account_insert_es")
# 003客户基础信息流程
t_0 >> t_1 >> t_2
# 004客户关联业务账号信息同步流程
t0 >> t1 >> t2
