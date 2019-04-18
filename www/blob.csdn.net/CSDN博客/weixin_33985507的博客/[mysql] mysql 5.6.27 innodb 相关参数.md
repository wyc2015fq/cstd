# [mysql] mysql 5.6.27 innodb 相关参数 - weixin_33985507的博客 - CSDN博客
2016年03月12日 22:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
mysql> show variables like '%innodb%';
+------------------------------------------+------------------------+
| Variable_name                            | Value                  |
+------------------------------------------+------------------------+
| ignore_builtin_innodb                    | OFF                    |
| innodb_adaptive_flushing                 | ON                     |
| innodb_adaptive_flushing_lwm             | 10                     |
| innodb_adaptive_hash_index               | ON                     |
| innodb_adaptive_max_sleep_delay          | 150000                 |
| innodb_additional_mem_pool_size          | 8388608                |
| innodb_api_bk_commit_interval            | 5                      |
| innodb_api_disable_rowlock               | OFF                    |
| innodb_api_enable_binlog                 | OFF                    |
| innodb_api_enable_mdl                    | OFF                    |
| innodb_api_trx_level                     | 0                      |
| innodb_autoextend_increment              | 64                     |
| innodb_autoinc_lock_mode                 | 1                      |
| innodb_buffer_pool_dump_at_shutdown      | OFF                    |
| innodb_buffer_pool_dump_now              | OFF                    |
| innodb_buffer_pool_filename              | ib_buffer_pool         |
| innodb_buffer_pool_instances             | 8                      |
| innodb_buffer_pool_load_abort            | OFF                    |
| innodb_buffer_pool_load_at_startup       | OFF                    |
| innodb_buffer_pool_load_now              | OFF                    |
| innodb_buffer_pool_size                  | 134217728              |
| innodb_change_buffer_max_size            | 25                     |
| innodb_change_buffering                  | all                    |
| innodb_checksum_algorithm                | innodb                 |
| innodb_checksums                         | ON                     |
| innodb_cmp_per_index_enabled             | OFF                    |
| innodb_commit_concurrency                | 0                      |
| innodb_compression_failure_threshold_pct | 5                      |
| innodb_compression_level                 | 6                      |
| innodb_compression_pad_pct_max           | 50                     |
| innodb_concurrency_tickets               | 5000                   |
| innodb_data_file_path                    | ibdata1:12M:autoextend |
| innodb_data_home_dir                     |                        |
| innodb_disable_sort_file_cache           | OFF                    |
| innodb_doublewrite                       | ON                     |
| innodb_fast_shutdown                     | 1                      |
| innodb_file_format                       | Antelope               |
| innodb_file_format_check                 | ON                     |
| innodb_file_format_max                   | Antelope               |
| innodb_file_per_table                    | ON                     |
| innodb_flush_log_at_timeout              | 1                      |
| innodb_flush_log_at_trx_commit           | 1                      |
| innodb_flush_method                      |                        |
| innodb_flush_neighbors                   | 1                      |
| innodb_flushing_avg_loops                | 30                     |
| innodb_force_load_corrupted              | OFF                    |
| innodb_force_recovery                    | 0                      |
| innodb_ft_aux_table                      |                        |
| innodb_ft_cache_size                     | 8000000                |
| innodb_ft_enable_diag_print              | OFF                    |
| innodb_ft_enable_stopword                | ON                     |
| innodb_ft_max_token_size                 | 84                     |
| innodb_ft_min_token_size                 | 3                      |
| innodb_ft_num_word_optimize              | 2000                   |
| innodb_ft_result_cache_limit             | 2000000000             |
| innodb_ft_server_stopword_table          |                        |
| innodb_ft_sort_pll_degree                | 2                      |
| innodb_ft_total_cache_size               | 640000000              |
| innodb_ft_user_stopword_table            |                        |
| innodb_io_capacity                       | 200                    |
| innodb_io_capacity_max                   | 2000                   |
| innodb_large_prefix                      | OFF                    |
| innodb_lock_wait_timeout                 | 50                     |
| innodb_locks_unsafe_for_binlog           | OFF                    |
| innodb_log_buffer_size                   | 8388608                |
| innodb_log_compressed_pages              | ON                     |
| innodb_log_file_size                     | 50331648               |
| innodb_log_files_in_group                | 2                      |
| innodb_log_group_home_dir                | .\                     |
| innodb_lru_scan_depth                    | 1024                   |
| innodb_max_dirty_pages_pct               | 75                     |
| innodb_max_dirty_pages_pct_lwm           | 0                      |
| innodb_max_purge_lag                     | 0                      |
| innodb_max_purge_lag_delay               | 0                      |
| innodb_mirrored_log_groups               | 1                      |
| innodb_monitor_disable                   |                        |
| innodb_monitor_enable                    |                        |
| innodb_monitor_reset                     |                        |
| innodb_monitor_reset_all                 |                        |
| innodb_old_blocks_pct                    | 37                     |
| innodb_old_blocks_time                   | 1000                   |
| innodb_online_alter_log_max_size         | 134217728              |
| innodb_open_files                        | 2000                   |
| innodb_optimize_fulltext_only            | OFF                    |
| innodb_page_size                         | 16384                  |
| innodb_print_all_deadlocks               | OFF                    |
| innodb_purge_batch_size                  | 300                    |
| innodb_purge_threads                     | 1                      |
| innodb_random_read_ahead                 | OFF                    |
| innodb_read_ahead_threshold              | 56                     |
| innodb_read_io_threads                   | 4                      |
| innodb_read_only                         | OFF                    |
| innodb_replication_delay                 | 0                      |
| innodb_rollback_on_timeout               | OFF                    |
| innodb_rollback_segments                 | 128                    |
| innodb_sort_buffer_size                  | 1048576                |
| innodb_spin_wait_delay                   | 6                      |
| innodb_stats_auto_recalc                 | ON                     |
| innodb_stats_method                      | nulls_equal            |
| innodb_stats_on_metadata                 | OFF                    |
| innodb_stats_persistent                  | ON                     |
| innodb_stats_persistent_sample_pages     | 20                     |
| innodb_stats_sample_pages                | 8                      |
| innodb_stats_transient_sample_pages      | 8                      |
| innodb_status_output                     | OFF                    |
| innodb_status_output_locks               | OFF                    |
| innodb_strict_mode                       | OFF                    |
| innodb_support_xa                        | ON                     |
| innodb_sync_array_size                   | 1                      |
| innodb_sync_spin_loops                   | 30                     |
| innodb_table_locks                       | ON                     |
| innodb_thread_concurrency                | 0                      |
| innodb_thread_sleep_delay                | 10000                  |
| innodb_undo_directory                    | .                      |
| innodb_undo_logs                         | 128                    |
| innodb_undo_tablespaces                  | 0                      |
| innodb_use_native_aio                    | ON                     |
| innodb_use_sys_malloc                    | ON                     |
| innodb_version                           | 5.6.27                 |
| innodb_write_io_threads                  | 4                      |
+------------------------------------------+------------------------+
120 rows in set (0.00 sec)
|新参数项|旧参数|参数说明|
|----|----|----|
|innodb_read_io_threads innodb_write_io_threads|innodb_file_io_threads(默认为4)|需要开启异步IO，不然没有什么意义。网上有测试分别设置为2为性能最好。不要超过4。所以对高速设备如ssd的影响特别大，传统的硬盘会没那么影响大。|
|-||可以设置为Barracuda格式，可以设置为dynaic和compressed格式。只在独立表空间生效|
|innodb_io_capacity||I/O容量，MySQL根据这个参数来评估你磁盘的io能力，默认200page，是对一般的sas盘。在磁盘性能较高的情况下，可以设置大一些|
|innodb_rollback_segments||回滚段的个数，默认是128|
|innodb_support_xa||参数设置是否支持分布式事务，默认值是ON或者1，表示支持分布式事务。如果确认应用中不需要使用分布式事务，则可以关闭这个参数，减少磁盘刷新的次数并获得更好的InnoDB性能|
|- s||多实例的bp，前提是buffer_pool_size大于1G以上，最多支持64个实例|
|innodb_change_buffering||以前只有默认的insert缓存，现在包括delete,purge，默认情况是all|
|Innodb_purge_batch_size||是否开启独立的线程清除undo页，默认是0不开启,每次删除的页数由Innodb_purge_batch_size控制，默认是20|
|skip-external-locking|skip-locking||
|-|default-character-set||
|-|log_bin_trust_routine_creators||
||||
- 数据行格式的设置
|文件格式||对blog支持|
|----|----|----|
|Antelope(羚羊)|Compact[紧簇]默认的格式|放768个字节，其余溢出|
|Redundant[冗余]是为兼容以前的版本5.0之前| | |
|Barracuda(梭鱼类)|Compressed[压缩]用CPU换IO性能|放20个字节，其余完全溢出存储|
在5.5中，information_schema 库中增加了三个关于锁的表（MEMORY引擎）：
innodb_trx         ## 当前运行的所有事务
innodb_locks       ## 当前出现的锁
innodb_lock_waits  ## 锁等待的对应关系
