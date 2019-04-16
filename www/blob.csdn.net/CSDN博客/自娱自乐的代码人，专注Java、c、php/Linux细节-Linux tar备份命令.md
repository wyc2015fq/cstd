# Linux细节 - Linux tar备份命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月19日 19:42:10[initphp](https://me.csdn.net/initphp)阅读数：844标签：[linux																[date](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux细节](https://blog.csdn.net/initphp/article/category/5942619)








1. 备份文件


`tar -zcvpf /backups/full-backup.tar.gz /home /etc `

2. 备份不需要的文件exclude


`tar -zcvpf /backups/full-backup.tar.gz -C / --exclude=mnt --exclude=media  `

3. 有txt需要备份文件列表  
`tar -zcvpf /backups/full-backup.tar.gz -T whatsbackup.txt  `

4. 给备份添加时间 
`tar -zcvpf /backups/full-backup_$(date +%Y%m%d-%H%M).tar.gz /home `

5. 保存备份结果


`tar -zcvpf /backups/full-backup_$(date +%F).tar.gz /home > /backups/logs/full-backup_$(date +%F).log  `
6. 增量备份


`tar -N 2008-01-29 -zcvpf /backups/inc-backup_$(date +%F).tar.gz /home`

7. 备份例子

```
tar -cvpf /dev/st0 --label="Full-backup created on $(date +%Y%m%d-%H%M)." \  
> -C / --exclude=var/spool/ etc home usr/local var/spool  
tar -cvpf /dev/st0 --label="Incremental-backup created on $(date +%F)." \  
> -N $(date -d yesterday "+%F") /etc /home
```


8. 恢复



```
tar -zxvpf /backups/full-backup_2008-01-29.tar.gz -C / \  
> etc/passwd etc/shadow
```






