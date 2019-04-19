# ftp服务器为我方,对方发送数据,巡检光衰数据导入es - sxf_123456的博客 - CSDN博客
2018年05月25日 15:48:38[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：129
*"""**ftp服务器为我方,对方发送数据,巡检光衰数据导入es**ftp conn_id 在airflow webserver 中的Admin->Connections 按照ftp模板配置**001_巡检光衰数据**"""*from airflow.models import DAG
from airflow.operators.python_operator import PythonOperator
from datetime import datetime, timedelta
from sctetl.airflow.contrib.sensors.ftp_sensor import FTPSensorOperator
from sctetl.airflow.contrib.hooks.es_hook import ElasticsearchHook
from sctetl.airflow.utils import dateutils
import os
default_args = {
    "owner": "airflow",
"depends_on_past": False,
"start_date": dateutils.get_start_date_local(2018, 5, 23, 2, 10),
"email": [''],
"email_on_failure": False,
"email_on_retry": False,
"retries": 1,
"retry_delay": timedelta(minutes=5),
"provide_context": True
}
dag = DAG(dag_id="m_001_light_decay_task",
default_args=default_args,
schedule_interval=dateutils.get_schedule_interval_local(2, 10))
def get_ftp(p_task, remote_path, **kwargs):
    *"""**    获取ftp文件路径****:param**** p_task:****:param**** remote_path:****:param**** kwargs:****:return****:**    """*ti = kwargs['ti']
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
        rs = float(s)
        rs = round(rs, dot_len)
    finally:
        return rs
def unix_str_to_date(s):
    rs = None
    try:
        rs = datetime.utcfromtimestamp(int(s))
    finally:
        return rs
def write_es_001(p_task, **kwargs):
    *"""**     巡检光衰数据写入es****:param**** p_task:****:param**** kwargs:****:return****:**    """*ti = kwargs['ti']
    file_path = ti.xcom_pull(task_ids=p_task)
    time_ = dateutils.get_local_ds_nodash(kwargs['execution_date'])
    index = "gather-001-{0}".format(time_)
    type = "001"
def foo():
        with open(file_path, "r", encoding="iso-8859-1") as f:
            for line_ in f:
                line = line_.strip().encode("utf-8").decode("utf-8").split("|^|")
                collect_time = unix_str_to_date(line[0])
                if len(line) != 24:
                    continue
source = add_parameters(
                    COLLECT_TIME=collect_time,
OLT_DEVICE_ID=line[1],
OLT_DEVICE_IP=line[2],
OLT_DEVICE_NAME=line[3],
OLT_PORT_NAME=line[4],
OLT_DEVICE_VENDER_ID=line[5],
OLT_PORT_SEND_LIGHT_POWER=str_to_float(line[6], ),
OLT_PORT_RECEIVE_LIGHT_POWER=str_to_float(line[7], ),
ONU_DEVICE_ID=line[8],
ONU_DEVICE_IP=line[9],
ONU_DEVICE_MAC=line[10],
LOID=line[11],
ONU_PORT_SEND_LIGHT_POWER=str_to_float(line[12], ),
ONU_PORT_RECEIVE_LIGHT_POWER=str_to_float(line[13], ),
DOWN_DECAY=str_to_float(line[14], ),
UP_DECAY=str_to_float(line[15], ),
OLT_DEVICE_PON_ELECTRICITY=str_to_float(line[16], ),
OLT_DEVICE_PON_TEMPERATURE=str_to_float(line[17], ),
OLT_DEVICE_PON_VOLTAGE=str_to_float(line[18], ),
ONU_DEVICE_UP_LIGHT_ELECTRICITY=str_to_float(line[19], ),
ONU_DEVICE_UP_LIGHT_TEMPERATURE=str_to_float(line[20], ),
ONU_DEVICE_UP_LIGHT_VOLTAGE=str_to_float(line[21], ),
TERMINAL_SN=line[22],
OLT_TO_ONU=str_to_float(line[23], )
                )
                source["@timestamp"] = datetime.now()
                action = {
                    "_index": index,
"_type": type,
"_source": source
                }
                yield action
    es_hook = ElasticsearchHook(conn_id="es_hn")
    if es_hook.exists(index):
        es_hook.delete_index(index)
    es_hook.bulk(foo())
    es_hook.add_alias_and_delete_old("gather-001", index)
with dag:
    # 等待000数据送达
t0 = FTPSensorOperator(conn_id="ftp_ligth_id",
dir_name="/",
pattern='{0}.log'.format(dateutils.get_macro_ds_local()),
task_id="sensor_ftp_optical_001",
timeout=60 * 10)
    # 获取ftp文件路径
t1 = PythonOperator(python_callable=get_ftp,
op_kwargs={"p_task": "sensor_ftp_optical_001",
"remote_path": "/data/ftpuserhome/ipwg"},
task_id="get_ftp")
    # 数据存入es
t2 = PythonOperator(python_callable=write_es_001,
op_kwargs={"p_task": "get_ftp"},
task_id="write_es_001")
# 任务调度顺序
t0 >> t1 >> t2
