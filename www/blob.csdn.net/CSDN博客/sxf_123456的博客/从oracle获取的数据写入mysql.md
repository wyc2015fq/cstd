# 从oracle获取的数据写入mysql - sxf_123456的博客 - CSDN博客
2018年03月30日 21:18:16[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：570
                from airflow.hooks import oracle_hook
from airflow.hooks import mysql_hook
from airflow import DAG
from airflow.operators.python_operator import PythonOperator
from airflow.operators.dummy_operator import DummyOperator
from datetime import  datetime,timedelta
import  time
default_args = {
    "owner":"airflow",
    "depends_on_past":False,
    "start_date":datetime(2018,1,22),
    "retries":1,
    "retry_delay":timedelta(minutes=5)
}
dag = DAG('ora_mys_crm',default_args=default_args,schedule_interval="0 1 * * *")
def write_to_mysql(**kwargs):
    """
    将oracle获取的数据写入的mysql
    :param kwargs:
    :return:
    """
    ora_hook = oracle_hook.OracleHook("oracle_crm_nmsqms")  # oracle 连接id
    mys_hook = mysql_hook.MySqlHook("mysql_healthrecord_health") #mysql 连接id
    sql_ora = """
            SELECT AREA_CODE,SUM(case when IPTV_NBR IS NULL  then 0 else 1 end),
            SUM(case when ACC_NBR IS NULL  then 0 else 1 end),COUNT(*)
            from GAT_SQMS.GAT_SQMS_BAND_IPTV_VIEW group BY AREA_CODE
            """
    conn_oracle = ora_hook.get_conn()
    conn_mysql = mys_hook.get_conn()
    cur=conn_oracle.cursor()
    mys_cur=conn_mysql.cursor()
    mys_cur.execute('SET autocommit = 0')
    try:
       cur.execute(sql_ora)
       rs = cur.fetchall()
       for line in rs:
           time_=time.strftime('%Y-%m-%d',time.localtime(time.time()))
           area_code = line[0]
           itv_count = line[1]
           band_count = line[2]
           all_count = line[3]
           sql_mys = "insert into crm_reschain_count (date,area_code,itv_count,band_count,all_count) value ('%s','%s',%d,%d,%d)"%(time_,area_code,itv_count,band_count,all_count)
           mys_cur.execute(sql_mys)
    except Exception as e:
        sql_del = "delete from crm_reschain_count where date='%s'"%time_
        mys_cur.execute(sql_del)
        conn_mysql.rollback()
        raise e
    conn_mysql.commit()
    cur.close()
    conn_oracle.close()
    mys_cur.close()
    conn_mysql.close()
with dag:
    t1 = PythonOperator(python_callable=write_to_mysql,task_id="write_mysql")
    t2 = DummyOperator(task_id="dummy")
t1 >> t2            
