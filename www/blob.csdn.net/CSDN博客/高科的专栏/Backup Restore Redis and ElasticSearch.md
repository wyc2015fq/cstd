
# Backup-Restore Redis and ElasticSearch - 高科的专栏 - CSDN博客

2017年10月23日 16:33:56[高科](https://me.csdn.net/pbymw8iwm)阅读数：613个人分类：[docker																](https://blog.csdn.net/pbymw8iwm/article/category/7215195)


Backup/Restore Redis and ElasticSearch
**Redis**
All data (by default) is stored both in an .aof and a .rdb file, which is a snapshot of your Redis dataset. They are made at
specified intervals.
The default location of this file is:
/data/redis
You can backup the files just by copying it or taking it from a backup spanpshot, but is not guaranteed an up-to-the-minute
backup. To guarantee (this will incur downtime in the source machine), first login in redis and save the snapshot:
\# redis‐cli ‐a <passwd> shutdown save
The is in the api_v1 repo, inside the file common/config.js , in redis.password .
To restore the data, first stop the target redis:
\# docker stop redis‐master
Move the .rdb and .aof files to /data/redis in the target machine and start the service
\# docker start redis‐master
**Elasticsearch**
Source: https://www.npmjs.com/package/elasticdump
Requirements:
NodeJS 5.x
Install elasticdump globally using npm
npm install elasticdump ‐g
Remove existing data in Elasticsearch. If you are usind Docker data is in /data/elasticsearch/data Stop Elasticsearch and
delete any content inside that directory.
IMPORTANT: You must always restore backups to MASTER node otherwise will fail.
Connect to Backup Droplet [ IP: 45.55.159.91 ] and download the backup you want to restore. backups are located in
/root/backup. For example tsb-dev-DATE-elasticsearch.tar.gz where date is the backup date in format YYYY-MM-DD-hhmm,
example: tsb-dev-2016-07-28-19-00-elasticsearch.tar.gz
Uncompress the backup file and change directory
tar xzvf tsb‐dev‐2016‐07‐28‐19‐00‐elasticsearch.tar.gz
cd tsb‐dev‐2016‐07‐28‐19‐00‐elasticsearch/
Here we store to files per index: data and mapping
To restore this backup, run this command for each index replacing my_index with the index name
elasticdump \
‐‐input=my_index_mapping.json \
‐‐output=http://localhost:9200/my_index \
‐‐type=mapping
elasticdump \
‐‐input=my_index_data.json \
‐‐output=http://localhost:9200/my_index \
‐‐type=data
For example, restoring public_worlds
elasticdump \
‐‐input=public_worlds_mapping.json \
‐‐output=http://localhost:9200/public_worlds \
‐‐type=mapping
elasticdump \
‐‐input=public_worlds_data.json \
‐‐output=http://localhost:9200/public_worlds \
‐‐type=data
Example for Pacman-Maker
elasticdump ‐‐input=public_worlds_mapping.json ‐‐output=http://10.132.110.168:2500/public_worlds ‐‐type=mapping
elasticdump ‐‐input=public_worlds_data.json ‐‐output=http://10.132.110.168:2500/public_worlds ‐‐type=data
elasticdump ‐‐input=world_mapping.json ‐‐output=http://10.132.110.168:2500/world ‐‐type=mapping
elasticdump ‐‐input=world_data.json ‐‐output=http://10.132.110.168:2500/world ‐‐type=data
elasticdump ‐‐input=profile_mapping.json ‐‐output=http://10.132.110.168:2500/profile ‐‐type=mapping
elasticdump ‐‐input=profile_data.json ‐‐output=http://10.132.110.168:2500/profile ‐‐type=data

