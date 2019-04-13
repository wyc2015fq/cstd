
# Android 7.1 32位apk导致的系统库找不到问题 - 嵌入式Linux - CSDN博客

2019年03月12日 15:18:26[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：32个人分类：[Android																](https://blog.csdn.net/weiqifa0/article/category/1752017)



## 出现问题
因为我们安卓7.1系统的应用都是64位的APK，我们自己开发的应用是32位的APK，这样在启动的时候，就大概率的出现库找不到的问题。
错误的日志如下：
`Line 440: 03-12 10:48:39.620   634   634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.evergrande.idpad.titket.oceanFlower/1000} requires arm whereas PackageSetting{51ca86c android.rockchip.update.service/1000} requires arm64
Line 441: 03-12 10:48:39.620   634   634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.evergrande.idpad.titket.oceanFlower/1000} requires arm whereas PackageSetting{992cb54 android/1000} requires arm64`
> --------- beginning of system

> 03-12 10:48:18.999   253   253 I vold    : Vold 3.0 (the awakening) firing up

> 03-12 10:48:18.999   253   253 V vold    : Detected support for: ext4 f2fs vfat

> 03-12 10:48:19.026   253   264 D vold    : e4crypt_init_user0

> 03-12 10:48:19.026   253   264 D vold    : e4crypt_prepare_user_storage for volume null, user 0, serial 0, flags 1

> 03-12 10:48:19.026   253   264 D vold    : Preparing: /data/system/users/0

> 03-12 10:48:19.026   253   264 E vold    : Failed to prepare /data/system/users/0: No such file or directory

> 03-12 10:48:19.026   253   264 E vold    : Failed to prepare user 0 storage

> 03-12 10:48:19.103   253   278 D Cryptfs : Just asked init to shut down class main

> 03-12 10:48:19.103   253   278 W vold    : emulated unmount requires state mounted

> 03-12 10:48:19.113   253   278 D Cryptfs : Just triggered post_fs_data

> 03-12 10:48:19.129   253   264 D vold    : e4crypt_init_user0

> 03-12 10:48:19.129   253   264 D vold    : e4crypt_prepare_user_storage for volume null, user 0, serial 0, flags 1

> 03-12 10:48:19.129   253   264 D vold    : Preparing: /data/system/users/0

> 03-12 10:48:19.129   253   264 D vold    : Preparing: /data/misc/profiles/cur/0

> 03-12 10:48:19.129   253   264 D vold    : Preparing: /data/misc/profiles/cur/0/foreign-dex

> 03-12 10:48:19.129   253   264 D vold    : Preparing: /data/system_de/0

> 03-12 10:48:19.129   253   264 D vold    : Preparing: /data/misc_de/0

> 03-12 10:48:19.130   253   264 D vold    : Preparing: /data/user_de/0

> 03-12 10:48:19.130   253   264 D vold    : e4crypt_unlock_user_key 0 serial=0 token_present=0

> 03-12 10:48:19.130   253   264 E vold    : Failed to chmod /data/system_ce/0: No such file or directory

> 03-12 10:48:19.130   253   264 E vold    : Failed to chmod /data/misc_ce/0: No such file or directory

> 03-12 10:48:19.130   253   264 E vold    : Failed to chmod /data/media/0: No such file or directory

> 03-12 10:48:19.164   253   278 D Cryptfs : post_fs_data done

> 03-12 10:48:21.166   253   278 I Cryptfs : keymaster module name is Keymaster OpenSSL HAL

> 03-12 10:48:21.166   253   278 I Cryptfs : keymaster version is 2

> 03-12 10:48:21.166   253   278 I Cryptfs : Found keymaster0 module, using keymaster0 API.

> 03-12 10:48:21.166   253   278 I Cryptfs : Using scrypt for cryptfs KDF

> 03-12 10:48:22.036   253   278 I Cryptfs : Using scrypt for cryptfs KDF

> 03-12 10:48:22.454   253   278 I Cryptfs : Enabling support for allow_discards in dmcrypt.

> 03-12 10:48:22.479   253   262 D vold    : Disk at 252:1 changed

> 03-12 10:48:22.479   253   278 E Cryptfs : Reading ext4 extent caused an exception

> 03-12 10:48:22.480   253   278 D Cryptfs : cryptfs_enable_inplace_ext4()=-1

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: ±-------------------------------------------------------+

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: | Super block                                            |

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: ±-------------------------------------------------------+

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: magic                         		[0x0xf2f52010 : 4076150800]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: major_ver                     		[0x0x000001 : 1]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: minor_ver                     		[0x0x000007 : 7]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: log_sectorsize                		[0x0x000009 : 9]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: log_sectors_per_block         		[0x0x000003 : 3]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: log_blocksize                 		[0x0x00000c : 12]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: log_blocks_per_seg            		[0x0x000009 : 9]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segs_per_sec                  		[0x0x000001 : 1]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: secs_per_zone                 		[0x0x000001 : 1]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: checksum_offset               		[0x00000000 : 0]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: block_count                   		[0x0x00000000309d78 : 3186040]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: section_count                 		[0x0x001821 : 6177]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segment_count                 		[0x0x00184e : 6222]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segment_count_ckpt            		[0x0x000002 : 2]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segment_count_sit             		[0x0x000002 : 2]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segment_count_nat             		[0x0x00001c : 28]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segment_count_ssa             		[0x0x00000d : 13]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segment_count_main            		[0x0x001821 : 6177]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: segment0_blkaddr              		[0x0x000178 : 376]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: cp_blkaddr                    		[0x0x000178 : 376]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: sit_blkaddr                   		[0x0x000578 : 1400]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: nat_blkaddr                   		[0x0x000978 : 2424]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: ssa_blkaddr                   		[0x0x004178 : 16760]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: main_blkaddr                  		[0x0x005b78 : 23416]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: root_ino                      		[0x0x000003 : 3]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: node_ino                      		[0x0x000001 : 1]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: meta_ino                      		[0x0x000002 : 2]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock: cp_payload                    		[0x00000000 : 0]

> 03-12 10:48:22.480   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: ±-------------------------------------------------------+

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: | Checkpoint                                             |

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: ±-------------------------------------------------------+

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: checkpoint_ver                		[0x0x00000000000002 : 2]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: user_block_count              		[0x0x000000002e7a00 : 3045888]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: valid_block_count             		[0x0x00000000000002 : 2]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: rsvd_segment_count            		[0x0x000077 : 119]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: overprov_segment_count        		[0x0x0000e4 : 228]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: free_segment_count            		[0x0x00181b : 6171]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: alloc_type[CURSEG_HOT_NODE]   		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: alloc_type[CURSEG_WARM_NODE]  		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: alloc_type[CURSEG_COLD_NODE]  		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_node_segno[0]             		[0x0x001820 : 6176]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_node_segno[1]             		[0x0x00181f : 6175]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_node_segno[2]             		[0x0x00181e : 6174]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_node_blkoff[0]            		[0x0x000001 : 1]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_node_blkoff[1]            		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_node_blkoff[2]            		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: alloc_type[CURSEG_HOT_DATA]   		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: alloc_type[CURSEG_WARM_DATA]  		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: alloc_type[CURSEG_COLD_DATA]  		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_data_segno[0]             		[0x0x00181d : 6173]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_data_segno[1]             		[0x0x000001 : 1]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_data_segno[2]             		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_data_blkoff[0]            		[0x0x000001 : 1]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_data_blkoff[1]            		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cur_data_blkoff[2]            		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: ckpt_flags                    		[0x0x000005 : 5]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cp_pack_total_block_count     		[0x0x000006 : 6]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: cp_pack_start_sum             		[0x0x000001 : 1]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: valid_node_count              		[0x0x000001 : 1]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: valid_inode_count             		[0x0x000001 : 1]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: next_free_nid                 		[0x0x000004 : 4]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: sit_ver_bitmap_bytesize       		[0x0x000040 : 64]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: nat_ver_bitmap_bytesize       		[0x0x000380 : 896]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: checksum_offset               		[0x0x000ffc : 4092]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: elapsed_time                  		[0x0000000000000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: sit_nat_version_bitmap[0]     		[0x00000000 : 0]

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.481   253   278 D f2fs_sparseblock: Reading cache block starting at block 1400

> 03-12 10:48:22.482   253   278 D f2fs_sparseblock: Reading cache block starting at block 1401

> 03-12 10:48:22.482   253   278 D f2fs_sparseblock: Reading cache block starting at block 1402

> 03-12 10:48:22.482   253   278 D f2fs_sparseblock: Reading cache block starting at block 1403

> 03-12 10:48:22.482   253   278 D f2fs_sparseblock: Reading cache block starting at block 1404

> 03-12 10:48:22.482   253   278 D f2fs_sparseblock: Reading cache block starting at block 1405

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1406

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1407

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1408

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1409

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1410

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1411

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1412

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1413

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1414

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1415

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1416

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1417

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1418

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1419

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1420

> 03-12 10:48:22.483   253   278 D f2fs_sparseblock: Reading cache block starting at block 1421

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1422

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1423

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1424

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1425

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1426

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1427

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1428

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1429

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1430

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1431

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1432

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1433

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1434

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1435

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1436

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1437

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1438

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1439

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1440

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1441

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1442

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1443

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1444

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1445

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1446

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1447

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1448

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1449

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1450

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1451

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1452

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1453

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1454

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1455

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1456

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1457

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1458

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1459

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1460

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1461

> 03-12 10:48:22.484   253   278 D f2fs_sparseblock: Reading cache block starting at block 1462

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1463

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1464

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1465

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1466

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1467

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1468

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1469

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1470

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1471

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1472

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1473

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1474

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1475

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1476

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1477

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1478

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1479

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1480

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1481

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1482

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1483

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1484

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1485

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1486

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1487

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1488

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1489

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1490

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1491

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1492

> 03-12 10:48:22.485   253   278 D f2fs_sparseblock: Reading cache block starting at block 1493

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1494

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1495

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1496

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1497

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1498

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1499

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1500

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1501

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1502

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1503

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1504

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1505

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1506

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1507

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1508

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1509

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1510

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1511

> 03-12 10:48:22.486   253   278 D f2fs_sparseblock: Reading cache block starting at block 1512

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: ±-------------------------------------------------------+

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: | F2FS_INFO                                              |

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: ±-------------------------------------------------------+

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: blocks_per_segment: 512

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: block_size: 4096

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: sit_bmp loc: 0x727ec40500

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: sit_bmp_size: 64

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: blocks_per_sit: 512

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: sit_blocks loc: 0x727eccb680

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: sit_sums loc: 0x727ecbc000

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: sit_sums num: 6

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: entry 0 in journal entries is for segment 6176

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: entry 1 in journal entries is for segment 6175

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: entry 2 in journal entries is for segment 6174

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: entry 3 in journal entries is for segment 6173

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: entry 4 in journal entries is for segment 1

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: entry 5 in journal entries is for segment 0

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: cp_blkaddr: 376

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: cp_valid_cp_blkaddr: 888

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: sit_blkaddr: 1400

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: nat_blkaddr: 2424

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: ssa_blkaddr: 16760

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: main_blkaddr: 23416

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: total_user_used: 2

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock: total_blocks: 3186040

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.490   253   278 D f2fs_sparseblock:

> 03-12 10:48:22.490   253   278 I Cryptfs : Encrypting from block 0

> 03-12 10:48:25.160   253   278 I Cryptfs : Encrypted to block 23415

> 03-12 10:48:25.160   253   278 I Cryptfs : Encrypting from block 3183992

> 03-12 10:48:25.180   253   278 I Cryptfs : Encrypted to block 3183992

> 03-12 10:48:25.180   253   278 I Cryptfs : Encrypting from block 3185528

> 03-12 10:48:25.180   253   278 I Cryptfs : Encrypted to block 3185528

> 03-12 10:48:26.917   253   278 I Cryptfs : cryptfs_check_passwd

> 03-12 10:48:26.917   253   278 D Cryptfs : crypt_ftr->fs_size = 25488320

> 03-12 10:48:26.917   253   278 I Cryptfs : Using scrypt for cryptfs KDF

> 03-12 10:48:27.424   253   278 I Cryptfs : Enabling support for allow_discards in dmcrypt.

> 03-12 10:48:27.427   253   262 D vold    : Disk at 252:1 changed

> 03-12 10:48:27.873   253   278 I Cryptfs : Password matches

> 03-12 10:48:27.873   253   278 D Cryptfs : test_mount_encrypted_fs(): Master key saved

> 03-12 10:48:27.873   253   278 I Cryptfs : keymaster module name is Keymaster OpenSSL HAL

> 03-12 10:48:27.873   253   278 I Cryptfs : keymaster version is 2

> 03-12 10:48:27.873   253   278 I Cryptfs : Found keymaster0 module, using keymaster0 API.

> 03-12 10:48:27.874   253   278 D Cryptfs : Just asked init to shut down class main

> 03-12 10:48:29.886   253   278 D Cryptfs : unmounting /data succeeded

> 03-12 10:48:29.979   253   278 D Cryptfs : Just triggered post_fs_data

> 03-12 10:48:29.999   253   264 D vold    : e4crypt_init_user0

> 03-12 10:48:29.999   253   264 D vold    : e4crypt_prepare_user_storage for volume null, user 0, serial 0, flags 1

> 03-12 10:48:29.999   253   264 D vold    : Preparing: /data/system/users/0

> 03-12 10:48:30.000   253   264 D vold    : Preparing: /data/misc/profiles/cur/0

> 03-12 10:48:30.001   253   264 D vold    : Preparing: /data/misc/profiles/cur/0/foreign-dex

> 03-12 10:48:30.001   253   264 D vold    : Preparing: /data/system_de/0

> 03-12 10:48:30.001   253   264 D vold    : Preparing: /data/misc_de/0

> 03-12 10:48:30.002   253   264 D vold    : Preparing: /data/user_de/0

> 03-12 10:48:30.002   253   264 D vold    : e4crypt_unlock_user_key 0 serial=0 token_present=0

> 03-12 10:48:30.002   253   264 E vold    : Failed to chmod /data/system_ce/0: No such file or directory

> 03-12 10:48:30.003   253   264 E vold    : Failed to chmod /data/misc_ce/0: No such file or directory

> 03-12 10:48:30.003   253   264 E vold    : Failed to chmod /data/media/0: No such file or directory

> 03-12 10:48:30.080   253   278 D Cryptfs : post_fs_data done

> 03-12 10:48:30.085   253   278 D Cryptfs : Just triggered restart_framework

> 03-12 10:48:30.147   367   367 E DrmService: ----------------running drmservice---------------

> 03-12 10:48:30.147   367   367 E DrmService: get prop_model = IDP110

> 03-12 10:48:30.147   367   367 E DrmService: get prop_board_platform,prop_board_platform = rk3399 , diff=0

> 03-12 10:48:30.147   367   367 E DrmService: rknand_sys_storage open fail

> 03-12 10:48:30.147   367   367 E DrmService: Get HID data:

> 03-12 10:48:30.149   375   375 D         : Assuming that device has multi-user storage layout; upgrade no longer supported

> 03-12 10:48:30.155   367   367 E DrmService: vendor read:

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 ef19f3b4

> 03-12 10:48:30.156   367   367 E DrmService: 0 ef3192f9 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 c

> 03-12 10:48:30.156   367   367 E DrmService: ef31ce10 0 8d82c72d 3

> 03-12 10:48:30.156   367   367 E DrmService: 0 c 8d849377 3

> 03-12 10:48:30.156   367   367 E DrmService: 1 ef3d5f7d 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 1 ffbd9018 ffbd8f78 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 5 0 e73e6cd

> 03-12 10:48:30.156   367   367 E DrmService: 4404357 5 ffbd9018 ffbd8f98

> 03-12 10:48:30.156   367   367 E DrmService: 5 ef262225 1 73726570

> 03-12 10:48:30.156   367   367 E DrmService: 2e747369 2e737973 646968 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 5 100000

> 03-12 10:48:30.156   367   367 E DrmService: 642f0001 732f7665 656b636f 72702f74

> 03-12 10:48:30.156   367   367 E DrmService: 7265706f 735f7974 69767265 6563

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.156   367   367 E DrmService: 0 0 0 0

> 03-12 10:48:30.157   367   367 E DrmService: 0 0 0 ffbd0000

> 03-12 10:48:30.157   367   367 E DrmService: 4404357 4404357 ffbd9110 7916030

> 03-12 10:48:30.157   367   367 E DrmService: vendor read error

> 03-12 10:48:30.157   367   367 E DrmService: ----vendor read sn error,try drmboot----

> 03-12 10:48:30.157   367   367 E DrmService: rknand_sys_storage open fail

> 03-12 10:48:30.165   367   367 E DrmService: -------------generate_device_serialno,len =6,result=VWCJB7

> 03-12 10:48:30.165   367   367 E DrmService: Unable to open path (/sys/class/android_usb/android0/iSerial),error is(No such file or directory)

> 03-12 10:48:30.165   367   367 E DrmService: try /config/usb_gadget/g1/strings/0x409/serialnumber

> 03-12 10:48:30.165   367   367 E DrmService: get serialno from idb,serialno = IDP110VWCJB7

> 03-12 10:48:34.068   634   634 I InstallerConnection: connecting…

> 03-12 10:48:34.084   634   634 I InstallerConnection: disconnecting…

> 03-12 10:48:34.149   634   634 I SystemServer: Entered the Android system server!

> 03-12 10:48:34.351   634   634 I SystemServiceManager: Starting com.android.server.pm.Installer

> 03-12 10:48:34.358   634   634 I Installer: Waiting for installd to be ready.

> 03-12 10:48:34.358   634   634 I InstallerConnection: connecting…

> 03-12 10:48:34.359   634   634 I SystemServiceManager: Starting com.android.server.am.ActivityManagerService
> KaTeX parse error: Expected '}', got 'EOF' at end of input: …id.view.Display
> HdrCapabilities@a69d6308, density 280, 213.0 x 213.0 dpi, appVsyncOff 1000000, presDeadline 16129032, touch INTERNAL, rotation 0, type BUILT_IN, state UNKNOWN, FLAG_DEFAULT_DISPLAY, FLAG_ROTATES_WITH_CONTENT, FLAG_SECURE, FLAG_SUPPORTS_PROTECTED_BUFFERS}

> 03-12 10:48:34.772   634   655 I DisplayManagerService: Display device changed state: “Built-in Screen”, ON

> 03-12 10:48:34.773   634   634 I SystemServiceManager: Starting phase 100

> 03-12 10:48:34.774   634   634 I SystemServer: StartPackageManagerService

> 03-12 10:48:34.837   634   634 W FileUtils: Failed to chmod(/data/system/packages.list): android.system.ErrnoException: chmod failed: ENOENT (No such file or directory)

> 03-12 10:48:34.851   634   634 W SystemConfig: No directory /system/etc/sysconfig, skipping

> 03-12 10:48:34.920   634   634 D SELinuxMMAC: Using policy file /system/etc/security/mac_permissions.xml

> 03-12 10:48:34.926   634   634 I PackageManager: No settings file; creating initial state

> 03-12 10:48:34.953   634   634 W PackageManager: Library not found: /system/framework/org.apache.http.legacy.jar

> 03-12 10:48:34.976   634   634 W PackageManager: Library not found: /system/framework/org.apache.http.legacy.jar

> 03-12 10:48:34.995   634   634 W PackageManager: Failed to parse /system/framework/arm: Missing base APK in /system/framework/arm

> 03-12 10:48:34.996   634   634 W PackageManager: Failed to parse /system/framework/arm64: Missing base APK in /system/framework/arm64

> 03-12 10:48:35.044   634   634 I PackageManager: /system/framework/framework-res.apk changed; collecting certs

> 03-12 10:48:35.237   634   634 W PackageManager: Failed to parse /system/framework/oat: Missing base APK in /system/framework/oat

> 03-12 10:48:35.249   634   634 I PackageManager: /system/priv-app/BackupRestoreConfirmation changed; collecting certs

> 03-12 10:48:35.266   634   634 I PackageManager: /system/priv-app/DefaultContainerService changed; collecting certs

> 03-12 10:48:35.282   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.ACCESS_FINE_LOCATION in package: com.DeviceTest at: Binary XML file line \#22

> 03-12 10:48:35.283   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.WRITE_SETTINGS in package: com.DeviceTest at: Binary XML file line \#31

> 03-12 10:48:35.283   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.INTERNET in package: com.DeviceTest at: Binary XML file line \#39

> 03-12 10:48:35.283   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.DEVICE_POWER in package: com.DeviceTest at: Binary XML file line \#43

> 03-12 10:48:35.288   634   634 I PackageManager: /system/priv-app/DeviceTest changed; collecting certs

> 03-12 10:48:35.329   634   634 I PackageManager: /system/priv-app/ExtServices changed; collecting certs

> 03-12 10:48:35.344   634   634 I PackageManager: /system/priv-app/ExternalStorageProvider changed; collecting certs

> 03-12 10:48:35.357   634   634 I PackageManager: /system/priv-app/FusedLocation changed; collecting certs

> 03-12 10:48:35.371   634   634 I PackageManager: /system/priv-app/InputDevices changed; collecting certs

> 03-12 10:48:35.384   634   634 I PackageManager: /system/priv-app/KeyChain changed; collecting certs

> 03-12 10:48:35.401   634   634 I PackageManager: /system/priv-app/ManagedProvisioning changed; collecting certs

> 03-12 10:48:35.419   634   634 I PackageManager: /system/priv-app/MediaProvider changed; collecting certs

> 03-12 10:48:35.431   634   634 I PackageManager: /system/priv-app/MtpDocumentsProvider changed; collecting certs

> 03-12 10:48:35.442   634   634 I PackageManager: /system/priv-app/OneTimeInitializer changed; collecting certs

> 03-12 10:48:35.476   634   634 I PackageManager: /system/priv-app/PackageInstaller changed; collecting certs

> 03-12 10:48:35.509   634   634 I PackageManager: /system/priv-app/Provision changed; collecting certs

> 03-12 10:48:35.521   634   634 I PackageManager: /system/priv-app/ProxyHandler changed; collecting certs

> 03-12 10:48:35.535   634   634 I PackageManager: /system/priv-app/RetailDemo changed; collecting certs

> 03-12 10:48:35.626   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.USE_CREDENTIALS in package: com.android.settings at: Binary XML file line \#56

> 03-12 10:48:35.627   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.READ_SYNC_SETTINGS in package: com.android.settings at: Binary XML file line \#61

> 03-12 10:48:35.627   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.WRITE_SYNC_SETTINGS in package: com.android.settings at: Binary XML file line \#62

> 03-12 10:48:35.661   634   634 I PackageManager: /system/priv-app/Settings changed; collecting certs

> 03-12 10:48:35.817   634   634 I PackageManager: /system/priv-app/SettingsProvider changed; collecting certs

> 03-12 10:48:35.831   634   634 I PackageManager: /system/priv-app/SharedStorageBackup changed; collecting certs

> 03-12 10:48:35.846   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.INSTALL_GRANT_RUNTIME_PERMISSIONS in package: com.android.shell at: Binary XML file line \#102

> 03-12 10:48:35.848   634   634 I PackageManager: /system/priv-app/Shell changed; collecting certs

> 03-12 10:48:35.863   634   634 I PackageManager: /system/priv-app/StatementService changed; collecting certs

> 03-12 10:48:35.889   634   634 I PackageManager: /system/priv-app/StorageManager changed; collecting certs

> 03-12 10:48:35.931   634   634 I PackageManager: /system/priv-app/StressTest changed; collecting certs

> 03-12 10:48:36.040   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.CONFIGURE_WIFI_DISPLAY in package: com.android.systemui at: Binary XML file line \#123

> 03-12 10:48:36.049   634   634 I PackageManager: /system/priv-app/SystemUI changed; collecting certs

> 03-12 10:48:36.149   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.READ_CONTACTS in package: com.android.bluetooth at: Binary XML file line \#61

> 03-12 10:48:36.151   634   634 W PackageParser: No actions in intent filter at /system/app/Bluetooth/Bluetooth.apk Binary XML file line \#223

> 03-12 10:48:36.154   634   634 I PackageManager: /system/app/Bluetooth changed; collecting certs

> 03-12 10:48:36.173   634   634 I PackageManager: /system/app/BluetoothMidiService changed; collecting certs

> 03-12 10:48:36.201   634   634 I PackageManager: /system/app/Camera2 changed; collecting certs

> 03-12 10:48:36.242   634   634 I PackageManager: /system/app/CertInstaller changed; collecting certs

> 03-12 10:48:36.261   634   634 I PackageManager: /system/app/DownloadProviderUi changed; collecting certs

> 03-12 10:48:36.275   634   634 I PackageManager: /system/app/ExtShared changed; collecting certs

> 03-12 10:48:36.293   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.RECORD_AUDIO in package: com.xingluo.engineermode.test at: Binary XML file line \#19

> 03-12 10:48:36.295   634   634 I PackageManager: /system/app/FactoryTest changed; collecting certs

> 03-12 10:48:36.341   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.RECORD_AUDIO in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#58

> 03-12 10:48:36.341   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.MODIFY_AUDIO_SETTINGS in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#59

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.PROCESS_OUTGOING_CALLS in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#60

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.READ_PHONE_STATE in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#61

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.ACCESS_WIFI_STATE in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#62

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.WAKE_LOCK in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#63

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.VIBRATE in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#64

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.CALL_PHONE in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#65

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.FLASHLIGHT in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#66

> 03-12 10:48:36.342   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.RECORD_VIDEO in package: com.evergrande.idpad.titket.oceanFlower at: Binary XML file line \#67

> 03-12 10:48:36.345   634   634 I PackageManager: /system/app/IDPad110 changed; collecting certs

> 03-12 10:48:37.598   634   634 W PackageManager: Permission android.permission.FLASHLIGHT from package com.evergrande.idpad.titket.oceanFlower in an unknown group android.permission-group.HARDWARE_CONTROLS

> 03-12 10:48:37.689   634   634 I PackageManager: /system/app/LatinIME changed; collecting certs

> 03-12 10:48:37.868   634   634 I PackageManager: /system/app/Launcher3 changed; collecting certs

> 03-12 10:48:37.879   634   634 W PackageManager: Permission com.android.launcher3.permission.READ_SETTINGS from package com.android.launcher3 in an unknown group android.permission-group.SYSTEM_TOOLS

> 03-12 10:48:37.879   634   634 W PackageManager: Permission com.android.launcher3.permission.WRITE_SETTINGS from package com.android.launcher3 in an unknown group android.permission-group.SYSTEM_TOOLS

> 03-12 10:48:37.879   634   634 W PackageManager: Permission com.android.launcher.permission.INSTALL_SHORTCUT from package com.android.launcher3 in an unknown group android.permission-group.SYSTEM_TOOLS

> 03-12 10:48:37.895   634   634 I PackageManager: /system/app/LiveWallpapersPicker changed; collecting certs

> 03-12 10:48:37.915   634   634 I PackageManager: /system/app/MediaFloat changed; collecting certs

> 03-12 10:48:37.933   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.CHANGE_WIFI_STATE in package: com.android.nfc at: Binary XML file line \#47

> 03-12 10:48:37.934   634   634 W PackageParser: Ignoring duplicate uses-permissions/uses-permissions-sdk-m: android.permission.ACCESS_WIFI_STATE in package: com.android.nfc at: Binary XML file line \#48

> 03-12 10:48:37.938   634   634 I PackageManager: /system/app/NfcNci changed; collecting certs

> 03-12 10:48:37.964   634   634 I PackageManager: /system/app/PrintRecommendationService changed; collecting certs

> 03-12 10:48:37.983   634   634 I PackageManager: /system/app/PrintSpooler changed; collecting certs

> 03-12 10:48:37.999   634   634 W PackageParser: Unknown element under : categary at /system/app/RFTestTool/RFTestTool.apk Binary XML file line \#18

> 03-12 10:48:38.000   634   634 I PackageManager: /system/app/RFTestTool changed; collecting certs

> 03-12 10:48:38.015   634   634 I PackageManager: /system/app/RKUpdateService changed; collecting certs

> 03-12 10:48:38.044   634   634 I PackageManager: /system/app/RkExplorer changed; collecting certs

> 03-12 10:48:38.064   634   634 I PackageManager: /system/app/SecretCode changed; collecting certs

> 03-12 10:48:38.080   634   634 I PackageManager: /system/app/UserDictionaryProvider changed; collecting certs

> 03-12 10:48:38.095   634   634 I PackageManager: /system/app/WallpaperPicker changed; collecting certs

> 03-12 10:48:38.118   634   634 I PackageManager: /system/app/webview changed; collecting certs

> 03-12 10:48:39.617   634   634 W PackageManager: Protected action; cap priority to 0; package: com.android.packageinstaller activity: com.android.packageinstaller.PackageInstallerActivity origPrio: 1

> 03-12 10:48:39.617   634   634 I PackageManager: Adjusting ABI for com.android.providers.media to arm64-v8a (requirer=com.android.mtp, scannedPackage=null)

> 03-12 10:48:39.619   634   634 I PackageManager: Adjusting ABI for com.android.providers.downloads.ui to arm64-v8a (requirer=com.android.mtp, scannedPackage=null)

> 03-12 10:48:39.620   634   634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.evergrande.idpad.titket.oceanFlower/1000} requires arm whereas PackageSetting{51ca86c android.rockchip.update.service/1000} requires arm64

> 03-12 10:48:39.620   634   634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.evergrande.idpad.titket.oceanFlower/1000} requires arm whereas PackageSetting{992cb54 android/1000} requires arm64

> 03-12 10:48:39.620   634   634 I PackageManager: Adjusting ABI for com.android.location.fused to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.621   634   634 I PackageManager: Adjusting ABI for com.android.rk to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.622   634   634 I PackageManager: Adjusting ABI for com.DeviceTest to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.623   634   634 I PackageManager: Adjusting ABI for com.cghs.stresstest to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.623   634   634 I PackageManager: Adjusting ABI for com.android.keychain to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.624   634   634 I PackageManager: Adjusting ABI for com.android.rk.mediafloat to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.625   634   634 I PackageManager: Adjusting ABI for com.xingluo.engineermode.test to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.626   634   634 I PackageManager: Adjusting ABI for com.android.inputdevices to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.626   634   634 I PackageManager: Adjusting ABI for com.android.providers.settings to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.628   634   634 I PackageManager: Adjusting ABI for com.android.settings to armeabi-v7a (requirer=com.evergrande.idpad.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.632   634   634 I PackageManager: Time to scan packages: 4.701 seconds

> 03-12 10:48:39.634   634   634 W PackageManager: Unknown permission android.permission.SEND_DOWNLOAD_COMPLETED_INTENTS in package com.android.providers.downloads.ui

> 03-12 10:48:39.634   634   634 W PackageManager: Unknown permission android.permission.ACCESS_ALL_DOWNLOADS in package com.android.providers.downloads.ui

> 03-12 10:48:39.634   634   634 W PackageManager: Unknown permission android.permission.ACCESS_ALL_DOWNLOADS in package com.android.certinstaller

> 03-12 10:48:39.635   634   634 W PackageManager: Unknown permission android.permission.NFC_UNLOCK in package com.android.nfc

> 03-12 10:48:39.636   634   634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.launcher3

> 03-12 10:48:39.636   634   634 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.launcher3

> 03-12 10:48:39.636   634   634 W PackageManager: Not granting permission android.permission.BIND_APPWIDGET to package com.android.launcher3 (protectionLevel=18 flags=0x3c4bbe45)

> 03-12 10:48:39.637   634   634 W PackageManager: Unknown permission android.permission.RECORD_VIDEO in package com.evergrande.idpad.titket.oceanFlower

> 03-12 10:48:39.637   634   634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.onetimeinitializer

> 03-12 10:48:39.637   634   634 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.onetimeinitializer

> 03-12 10:48:39.637   634   634 W PackageManager: Unknown permission com.google.android.permission.INSTALL_WEARABLE_PACKAGES in package com.android.packageinstaller

> 03-12 10:48:39.637   634   634 W PackageManager: Unknown permission android.permission.DOWNLOAD_WITHOUT_NOTIFICATION in package com.android.inputmethod.latin

> 03-12 10:48:39.638   634   634 W PackageManager: Unknown permission android.permission.SENSOR_INFO in package com.xingluo.engineermode.test

> 03-12 10:48:39.638   634   634 W PackageManager: Unknown permission android.permission.SENSOR_ENABLE in package com.xingluo.engineermode.test

> 03-12 10:48:39.639   634   634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.xingluo.engineermode.test

> 03-12 10:48:39.639   634   634 W PackageManager: Unknown permission com.google.android.googleapps.permission.GOOGLE_AUTH in package com.android.settings

> 03-12 10:48:39.639   634   634 W PackageManager: Unknown permission android.permission.ACCESS_DOWNLOAD_MANAGER in package com.android.settings

> 03-12 10:48:39.640   634   634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.settings

> 03-12 10:48:39.640   634   634 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.settings

> 03-12 10:48:39.644   634   634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.DeviceTest

> 03-12 10:48:39.645   634   634 W PackageManager: Unknown permission com.google.android.gallery3d.permission.GALLERY_PROVIDER in package com.android.bluetooth

> 03-12 10:48:39.645   634   634 W PackageManager: Unknown permission com.android.gallery3d.permission.GALLERY_PROVIDER in package com.android.bluetooth

> 03-12 10:48:39.646   634   634 V PackageManager: reconcileAppsData for null u0 0x3

> 03-12 10:48:39.901   634   634 V PackageManager: reconcileAppsData finished 47 packages

> 03-12 10:48:40.599   634   634 D PackageManager: Ephemeral resolver NOT found; no matching intent filters

> 03-12 10:48:40.600   634   634 I PackageManager: Ephemeral deactivated; missing resolver and installer

> 03-12 10:48:40.650   634   634 I SystemServer: StartOtaDexOptService

> 03-12 10:48:40.652   634   634 I SystemServer: StartUserManagerService

> 03-12 10:48:40.652   634   634 I SystemServiceManager: Starting com.android.server.pm.UserManagerService
$$
LifeCycle
03-12 10:48:40.672   634   634 I SystemServiceManager: Starting com.android.server.BatteryService
03-12 10:48:40.688   634   634 I SystemServiceManager: Starting com.android.server.usage.UsageStatsService
03-12 10:48:40.710   634   634 I SystemServiceManager: Starting com.android.server.webkit.WebViewUpdateService
03-12 10:48:40.718   634   634 I SystemServer: Reading configuration...
03-12 10:48:40.718   634   634 I SystemServer: StartSchedulingPolicyService
03-12 10:48:40.720   634   634 I SystemServer: StartEntropyMixer
03-12 10:48:40.721   634   634 W EntropyMixer: No existing entropy file -- first boot?
03-12 10:48:40.722   634   634 I EntropyMixer: Writing entropy...
03-12 10:48:40.727   634   634 I SystemServer: Camera Service
03-12 10:48:40.730   634   634 I SystemServiceManager: Starting com.android.server.camera.CameraService
03-12 10:48:40.734   634   685 I ServiceThread: Enabled StrictMode logging for CameraService_proxy looper.
03-12 10:48:40.735   634   634 I SystemServer: StartAccountManagerService
03-12 10:48:40.737   634   634 I SystemServiceManager: Starting com.android.server.accounts.AccountManagerService
$$
> Lifecycle

> 03-12 10:48:40.754   634   634 I SystemServer: StartContentService

> 03-12 10:48:40.756   634   634 I SystemServiceManager: Starting com.android.server.content.ContentService
$$
Lifecycle
03-12 10:48:40.762   634   634 I SystemServer: InstallSystemProviders
03-12 10:48:40.935   634   634 I SettingsState: No settings state /data/system/users/0/settings_system.xml
03-12 10:48:40.943   634   634 I SettingsState: No settings state /data/system/users/0/settings_secure.xml
03-12 10:48:40.947   634   634 D SettingsProvider: Generated and saved new ANDROID_ID [f4b72315540af3a1] for user 0
03-12 10:48:40.949   634   634 I SettingsState: No settings state /data/system/users/0/settings_global.xml
03-12 10:48:40.969   634   634 I SystemServer: StartVibratorService
03-12 10:48:40.975   634   634 I SystemServer: StartConsumerIrService
03-12 10:48:40.978   634   634 I SystemServer: StartAlarmManagerService
03-12 10:48:40.979   634   634 I SystemServiceManager: Starting com.android.server.AlarmManagerService
03-12 10:48:40.982   634   634 D AlarmAlignmentManager: loading /system/etc/alarm_alignment_conf.xml
03-12 10:48:40.985   634   634 D AlarmAlignmentManager: mAlarmStrategy=SCREEN_OFF_ALARM_STRATEGY_FIXED2
03-12 10:48:40.985   634   634 D AlarmAlignmentManager: mAppAlarmFixedAlignment=300000
03-12 10:48:40.985   634   634 D AlarmAlignmentManager: mIsStrict = true
03-12 10:48:40.985   634   634 D AlarmAlignmentManager: mSkipNonWakeup = false
03-12 10:48:40.985   634   634 D AlarmAlignmentManager: mAlignNonWakeup = false
03-12 10:48:40.985   634   634 D AlarmAlignmentManager: mAlignSystemAlarm = false
03-12 10:48:40.985   634   634 D AlarmAlignmentManager: mAppAlarmSkipShort=10000
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: mAppAlarmRepeatAlignment=300000
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: mAppAlarmIgnoreTop = false
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.google.android.deskclock, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.alarmclock.xtreme, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.alarmclock.xtreme.free, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.zdworks.android.zdclock, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.tiantian.ttclock, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.macropinch.axe, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.maize.digitalClock, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:nl.jsource.retroclock.android, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add white list:com.asus.deskclock, null
03-12 10:48:40.986   634   634 D AlarmAlignmentManager: add black list:com.android.phone, com.android.internal.telephony.data-stall
03-12 10:48:40.987   634   634 D AlarmAlignmentManager: strict mode and fixed strategy, force repeat interval to 300000
03-12 10:48:40.987   634   634 W AlarmAlignmentManager: updating App alarm config:
03-12 10:48:40.987   634   634 W AlarmAlignmentManager: old config:strategy:SCREEN_OFF_ALARM_STRATEGY_NONE, repeat alignment:0, fixed alignment:0, skip short interval:0, strict:true, ignore top:false, skip nonwakeup:true, align system alarm:false, whitelist size:0, blacklist size:0
03-12 10:48:40.987   634   634 W AlarmAlignmentManager: new config:strategy:SCREEN_OFF_ALARM_STRATEGY_FIXED2, repeat alignment:300000, fixed alignment:300000, skip short interval:10000, strict:true, ignore top:false, skip nonwakeup:false, align system alarm:false, whitelist size:9, blacklist size:1
03-12 10:48:41.001   634   634 I SystemServer: InitWatchdog
03-12 10:48:41.002   634   634 I SystemServer: StartInputManagerService
03-12 10:48:41.005   634   634 I InputManager: Initializing input manager, mUseDevInputEventForAudioJack=false
03-12 10:48:41.008   634   634 I SystemServer: StartWindowManagerService
03-12 10:48:41.070   634   655 I WindowManager: No existing display settings /data/system/display_settings.xml; starting empty
03-12 10:48:41.154   634   634 I SystemServer: StartVrManagerService
03-12 10:48:41.154   634   634 I SystemServiceManager: Starting com.android.server.vr.VrManagerService
03-12 10:48:41.166   634   634 I InputManager: Starting input manager
03-12 10:48:41.172   634   634 I SystemServiceManager: Starting com.android.server.BluetoothService
03-12 10:48:41.181   634   634 D BluetoothManagerService: Loading stored name and address
03-12 10:48:41.181   634   634 D BluetoothManagerService: Stored bluetooth Name=null,Address=null
03-12 10:48:41.181   634   634 D BluetoothManagerService: Bluetooth persisted state: 1
03-12 10:48:41.181   634   634 D BluetoothManagerService: Startup: Bluetooth persisted state is ON.
03-12 10:48:41.181   634   634 I SystemServer: ConnectivityMetricsLoggerService
03-12 10:48:41.182   634   634 I SystemServiceManager: Starting com.android.server.connectivity.MetricsLoggerService
03-12 10:48:41.187   634   634 I SystemServer: IpConnectivityMetrics
03-12 10:48:41.188   634   634 I SystemServiceManager: Starting com.android.server.connectivity.IpConnectivityMetrics
03-12 10:48:41.190   634   634 I SystemServiceManager: Starting com.android.server.InputMethodManagerService
$$
> Lifecycle

> 03-12 10:48:41.280   634   634 W InputMethodManagerService: No IME selected. Choose the most applicable IME.

> 03-12 10:48:41.282   634   634 I InputMethodManagerService: Default found, using com.android.inputmethod.latin/.LatinIME

> 03-12 10:48:41.286   634   634 I SystemServer: StartAccessibilityManagerService

> 03-12 10:48:41.298   634   634 I ActivityManager: Config changes=1df8 {1.0 ?mcc?mnc [en_US] ldltr sw457dp w457dp h731dp 280dpi nrml port ?uimode ?night -touch -keyb/h/h -nav/h s.2}

> 03-12 10:48:41.302   634   647 I UsageStatsDatabase: Upgrading from version 0 to 3

> 03-12 10:48:41.302   634   647 I UsageStatsDatabase: Deleting all usage stats files

> 03-12 10:48:41.305   634   647 I UsageStatsService: User[0] Rollover scheduled @ 2019-03-13 10:48:41(1552445321299)

> 03-12 10:48:41.306   634   647 D UsageStatsService: Initializing defaults for system apps on user 0

> 03-12 10:48:41.309   634   647 E AppIdleHistory: Unable to read app idle file for user 0

> 03-12 10:48:41.312   634   655 I ActivityManager: Config changes=408 {1.0 ?mcc?mnc [en_US] ldltr sw457dp w457dp h659dp 280dpi nrml port ?uimode ?night finger -keyb/h/h -nav/h s.3}

> 03-12 10:48:41.333   634   634 I SystemServiceManager: Starting com.android.server.MountService
$$
Lifecycle
03-12 10:48:41.348   634   634 I SystemServer: start UiModeManagerService.
03-12 10:48:41.349   634   634 I SystemServiceManager: Starting com.android.server.UiModeManagerService
03-12 10:48:41.349   634   699 D MountService: Thinking about init, mSystemReady=false, mDaemonConnected=true
03-12 10:48:41.349   634   699 D MountService: Thinking about reset, mSystemReady=false, mDaemonConnected=true
03-12 10:48:41.352   634   634 I ActivityManager: Config changes=200 {1.0 ?mcc?mnc [en_US] ldltr sw457dp w457dp h659dp 280dpi nrml port finger -keyb/h/h -nav/h s.4}
03-12 10:48:41.353   634   699 D MountService: Thinking about init, mSystemReady=false, mDaemonConnected=true
03-12 10:48:41.353   634   699 D MountService: Thinking about reset, mSystemReady=false, mDaemonConnected=true
03-12 10:48:41.354   634   699 W MountService: No primary storage mounted!
03-12 10:48:41.355   634   699 D VoldConnector: SND -&gt; {1 asec list}
03-12 10:48:41.365   634   700 D VoldConnector: RCV &lt;- {200 1 asec operation succeeded}
03-12 10:48:41.369   634   699 W PackageManager: Unknown permission android.permission.SEND_DOWNLOAD_COMPLETED_INTENTS in package com.android.providers.downloads.ui
03-12 10:48:41.370   634   699 W PackageManager: Unknown permission android.permission.ACCESS_ALL_DOWNLOADS in package com.android.providers.downloads.ui
03-12 10:48:41.370   634   699 W PackageManager: Unknown permission android.permission.ACCESS_ALL_DOWNLOADS in package com.android.certinstaller
03-12 10:48:41.370   634   699 W PackageManager: Unknown permission android.permission.NFC_UNLOCK in package com.android.nfc
03-12 10:48:41.370   634   699 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.launcher3
03-12 10:48:41.370   634   699 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.launcher3
03-12 10:48:41.370   634   699 W PackageManager: Not granting permission android.permission.BIND_APPWIDGET to package com.android.launcher3 (protectionLevel=18 flags=0x3ccbbe45)
03-12 10:48:41.371   634   699 W PackageManager: Unknown permission android.permission.RECORD_VIDEO in package com.evergrande.idpad.titket.oceanFlower
03-12 10:48:41.371   634   699 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.onetimeinitializer
03-12 10:48:41.371   634   699 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.onetimeinitializer
03-12 10:48:41.371   634   699 W PackageManager: Unknown permission com.google.android.permission.INSTALL_WEARABLE_PACKAGES in package com.android.packageinstaller
03-12 10:48:41.371   634   699 W PackageManager: Unknown permission android.permission.DOWNLOAD_WITHOUT_NOTIFICATION in package com.android.inputmethod.latin
03-12 10:48:41.372   634   699 W PackageManager: Unknown permission android.permission.SENSOR_INFO in package com.xingluo.engineermode.test
03-12 10:48:41.372   634   699 W PackageManager: Unknown permission android.permission.SENSOR_ENABLE in package com.xingluo.engineermode.test
03-12 10:48:41.372   634   699 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.xingluo.engineermode.test
03-12 10:48:41.372   634   699 W PackageManager: Unknown permission com.google.android.googleapps.permission.GOOGLE_AUTH in package com.android.settings
03-12 10:48:41.372   634   699 W PackageManager: Unknown permission android.permission.ACCESS_DOWNLOAD_MANAGER in package com.android.settings
03-12 10:48:41.373   634   699 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.settings
03-12 10:48:41.373   634   699 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.settings
03-12 10:48:41.375   634   699 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.DeviceTest
03-12 10:48:41.375   634   699 W PackageManager: Unknown permission com.google.android.gallery3d.permission.GALLERY_PROVIDER in package com.android.bluetooth
03-12 10:48:41.375   634   699 W PackageManager: Unknown permission com.android.gallery3d.permission.GALLERY_PROVIDER in package com.android.bluetooth
03-12 10:48:41.424   634   699 W MountService: No primary storage mounted!
03-12 10:48:41.424   634   699 D VoldConnector: SND -&gt; {2 asec list}
03-12 10:48:41.426   634   700 D VoldConnector: RCV &lt;- {200 2 asec operation succeeded}
03-12 10:48:44.386   634   634 W PackageManager: No disk maintenance in 1552358924386; running immediately
03-12 10:48:44.387   634   699 I MountService: Running fstrim idle maintenance
03-12 10:48:44.387   634   699 D VoldConnector: SND -&gt; {3 fstrim dotrim}
03-12 10:48:44.390   634   700 D VoldConnector: RCV &lt;- {200 3 Command succeeded}
03-12 10:48:44.396   634   634 I SystemServer: StartLockSettingsService
03-12 10:48:44.397   253   711 D vold    : Starting trim of /cache
03-12 10:48:44.398   634   634 I SystemServiceManager: Starting com.android.server.LockSettingsService
$$
> Lifecycle

> 03-12 10:48:44.428   634   634 I SystemServiceManager: Starting com.android.server.DeviceIdleController

> 03-12 10:48:44.438   634   634 I SystemServiceManager: Starting com.android.server.devicepolicy.DevicePolicyManagerService
$$
Lifecycle
03-12 10:48:44.462   634   634 I SystemServer: StartStatusBarManagerService
03-12 10:48:44.473   634   634 I SystemServer: StartClipboardService
03-12 10:48:44.478   634   634 I SystemServer: StartNetworkManagementService
03-12 10:48:44.486   634   712 I NetworkManagement: onDaemonConnected()
03-12 10:48:44.493   634   634 I SystemServiceManager: Starting com.android.server.TextServicesManagerService
$$
> Lifecycle

> 03-12 10:48:44.507   634   634 I SystemServer: StartNetworkScoreService

> 03-12 10:48:44.512   634   634 I SystemServer: StartNetworkStatsService

> 03-12 10:48:44.527   634   634 I SystemServer: StartNetworkPolicyManagerService

> 03-12 10:48:44.539   634   634 I SystemServer: No Wi-Fi NAN Service (NAN support Not Present)

> 03-12 10:48:44.562   634   634 I SystemServiceManager: Starting com.android.server.wifi.p2p.WifiP2pService

> 03-12 10:48:44.611   634   634 I SystemServiceManager: Starting com.android.server.wifi.WifiService

> 03-12 10:48:44.760   634   634 I SystemServiceManager: Starting com.android.server.wifi.scanner.WifiScanningService

> 03-12 10:48:44.765   634   634 I SystemServiceManager: Starting com.android.server.wifi.RttService

> 03-12 10:48:44.768   634   634 I SystemServer: StartEthernetService

> 03-12 10:48:44.770   634   634 I SystemServiceManager: Starting com.android.server.ethernet.EthernetService

> 03-12 10:48:44.785   634   634 I SystemServer: StartConnectivityService

> 03-12 10:48:44.795   634   634 D ConnectivityService: ConnectivityService starting up

> 03-12 10:48:44.801   634   634 D ConnectivityService: wifiOnly=true

> 03-12 10:48:44.802   634   634 D ConnectivityService: naString=wifi,1,1,2,-1,true config=android.net.NetworkConfig@c3785f9

> 03-12 10:48:44.802   634   634 D ConnectivityService: naString=mobile,0,0,0,-1,true config=android.net.NetworkConfig@60c193e

> 03-12 10:48:44.802   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 0

> 03-12 10:48:44.802   634   634 D ConnectivityService: naString=mobile_mms,2,0,2,60000,true config=android.net.NetworkConfig@6cbd39f

> 03-12 10:48:44.802   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 2

> 03-12 10:48:44.802   634   634 D ConnectivityService: naString=mobile_supl,3,0,2,60000,true config=android.net.NetworkConfig@b3dbcec

> 03-12 10:48:44.802   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 3

> 03-12 10:48:44.803   634   634 D ConnectivityService: naString=mobile_dun,4,0,2,60000,true config=android.net.NetworkConfig@36188b5

> 03-12 10:48:44.803   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 4

> 03-12 10:48:44.803   634   634 D ConnectivityService: naString=mobile_hipri,5,0,3,60000,true config=android.net.NetworkConfig@92c8b4a

> 03-12 10:48:44.803   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 5

> 03-12 10:48:44.803   634   634 D ConnectivityService: naString=mobile_fota,10,0,2,60000,true config=android.net.NetworkConfig@fa40abb

> 03-12 10:48:44.803   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 10

> 03-12 10:48:44.803   634   634 D ConnectivityService: naString=mobile_ims,11,0,2,60000,true config=android.net.NetworkConfig@bbfbbd8

> 03-12 10:48:44.803   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 11

> 03-12 10:48:44.803   634   634 D ConnectivityService: naString=mobile_cbs,12,0,2,60000,true config=android.net.NetworkConfig@ceefb31

> 03-12 10:48:44.803   634   634 D ConnectivityService: networkAttributes - ignoring mobile as this dev is wifiOnly 12

> 03-12 10:48:44.803   634   634 D ConnectivityService: naString=bluetooth,7,7,0,-1,true config=android.net.NetworkConfig@b301216

> 03-12 10:48:44.804   634   634 D ConnectivityService: naString=ethernet,9,9,9,-1,true config=android.net.NetworkConfig@bfe9797

> 03-12 10:48:44.804   634   634 D ConnectivityService: mNetworksDefined=4

> 03-12 10:48:44.804   634   634 E ConnectivityService: Ignoring protectedNetwork 10

> 03-12 10:48:44.804   634   634 E ConnectivityService: Ignoring protectedNetwork 11

> 03-12 10:48:44.804   634   634 E ConnectivityService: Ignoring protectedNetwork 12

> 03-12 10:48:44.804   634   634 E ConnectivityService: Ignoring protectedNetwork 14

> 03-12 10:48:44.804   634   634 E ConnectivityService: Ignoring protectedNetwork 15

> 03-12 10:48:44.827   634   634 I SystemServer: StartNsdService

> 03-12 10:48:44.834   634   634 I SystemServer: StartUpdateLockService

> 03-12 10:48:44.837   634   634 I SystemServiceManager: Starting com.android.server.RecoverySystemService

> 03-12 10:48:44.843   634   634 I SystemServiceManager: Starting com.android.server.notification.NotificationManagerService

> 03-12 10:48:44.872   634   634 D ConditionProviders.SCP: new ScheduleConditionProvider()

> 03-12 10:48:44.887   634   634 D ZenLog  : set_zen_mode: off,init

> 03-12 10:48:44.893   634   634 I SystemServiceManager: Starting com.android.server.storage.DeviceStorageMonitorService

> 03-12 10:48:44.898   634   634 I SystemServer: StartLocationManagerService

> 03-12 10:48:44.906   634   634 I SystemServer: StartCountryDetectorService

> 03-12 10:48:44.910   634   634 I SystemServer: StartSearchManagerService

> 03-12 10:48:44.911   634   634 I SystemServiceManager: Starting com.android.server.search.SearchManagerService
$$
Lifecycle
03-12 10:48:44.914   634   634 I SystemServiceManager: Starting com.android.server.DropBoxManagerService
03-12 10:48:44.917   634   634 I SystemServer: StartWallpaperManagerService
03-12 10:48:44.917   634   634 I SystemServiceManager: Starting com.android.server.wallpaper.WallpaperManagerService
$$
> Lifecycle

> 03-12 10:48:44.922   634   634 E WallpaperManagerService: Invalid wallpaper data

> 03-12 10:48:44.922   634   634 E WallpaperManagerService: Unable to apply new wallpaper

> 03-12 10:48:45.165   634   634 W WallpaperManagerService: no current wallpaper – first boot?

> 03-12 10:48:45.166   634   634 I SystemServer: StartAudioService

> 03-12 10:48:45.166   634   634 I SystemServiceManager: Starting com.android.server.audio.AudioService
$$
Lifecycle
03-12 10:48:45.194   634   634 I SystemServiceManager: Starting com.android.server.DockObserver
03-12 10:48:45.195   634   634 W DockObserver: This kernel does not have dock station support
03-12 10:48:45.195   634   634 I SystemServer: StartWiredAccessoryManager
03-12 10:48:45.196   634   634 W WiredAccessoryManager: This kernel does not have wired headset support
03-12 10:48:45.197   634   634 W WiredAccessoryManager: This kernel does not have usb audio support
03-12 10:48:45.197   634   634 W WiredAccessoryManager: This kernel does not have dp audio support
03-12 10:48:45.197   634   634 I SystemServiceManager: Starting com.android.server.usb.UsbService
$$
> Lifecycle

> 03-12 10:48:45.210   634   634 E UsbDeviceManager: failed to write to /sys/class/android_usb/android0/f_rndis/ethaddr

> 03-12 10:48:45.214   634   634 I UsbDeviceManager: Setting USB config to adb

> 03-12 10:48:45.337   634   634 I SystemServer: StartSerialService

> 03-12 10:48:45.342   634   634 I SystemServiceManager: Starting com.android.server.twilight.TwilightService

> 03-12 10:48:45.343   634   634 I SystemServiceManager: Starting com.android.server.job.JobSchedulerService

> 03-12 10:48:45.356   634   634 I SystemServiceManager: Starting com.android.server.soundtrigger.SoundTriggerService

> 03-12 10:48:45.359   634   634 I SystemServiceManager: Starting com.android.server.backup.BackupManagerService
$$
Lifecycle
03-12 10:48:45.365   634   634 I SystemServiceManager: Starting com.android.server.appwidget.AppWidgetService
03-12 10:48:45.381   634   634 I SystemServiceManager: Starting com.android.server.voiceinteraction.VoiceInteractionManagerService
03-12 10:48:45.386   634   634 I SystemServer: Gesture Launcher Service
03-12 10:48:45.386   634   634 I SystemServiceManager: Starting com.android.server.GestureLauncherService
03-12 10:48:45.387   634   634 I SystemServiceManager: Starting com.android.server.SensorNotificationService
03-12 10:48:45.387   634   634 I SystemServiceManager: Starting com.android.server.ContextHubSystemService
03-12 10:48:45.391   634   634 I SystemServer: StartDiskStatsService
03-12 10:48:45.394   634   634 I SystemServer: StartSamplingProfilerService
03-12 10:48:45.402   634   634 I SystemServer: StartNetworkTimeUpdateService
03-12 10:48:45.405   634   634 I SystemServer: StartCommonTimeManagementService
03-12 10:48:45.408   634   634 I SystemServer: CertBlacklister
03-12 10:48:45.411   634   634 I SystemServiceManager: Starting com.android.server.emergency.EmergencyAffordanceService
03-12 10:48:45.413   634   634 I SystemServiceManager: Starting com.android.server.dreams.DreamManagerService
03-12 10:48:45.416   634   634 I SystemServer: StartAssetAtlasService
03-12 10:48:45.422   634   634 I SystemServiceManager: Starting com.android.server.print.PrintManagerService
03-12 10:48:45.430   634   688 V UsbDeviceManager: USB UEVENT: {USB_STATE=CONNECTED, SUBSYSTEM=android_usb, SEQNUM=1975, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
03-12 10:48:45.430   634   688 D UsbDeviceManager: subSystem:android_usb,devPath:/devices/virtual/android_usb/android0
03-12 10:48:45.431   634   688 V UsbDeviceManager: USB UEVENT: {USB_STATE=CONNECTED, SUBSYSTEM=android_usb, SEQNUM=1975, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
03-12 10:48:45.431   634   688 D UsbDeviceManager: subSystem:android_usb,devPath:/devices/virtual/android_usb/android0
03-12 10:48:45.436   634   634 I SystemServiceManager: Starting com.android.server.restrictions.RestrictionsManagerService
03-12 10:48:45.439   634   634 I SystemServiceManager: Starting com.android.server.media.MediaSessionService
03-12 10:48:45.448   634   634 I SystemServer: StartMediaRouterService
03-12 10:48:45.450   634   634 I SystemServiceManager: Starting com.android.server.trust.TrustManagerService
03-12 10:48:45.451   634   688 V UsbDeviceManager: USB UEVENT: {USB_STATE=CONFIGURED, SUBSYSTEM=android_usb, SEQNUM=1976, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
03-12 10:48:45.451   634   688 D UsbDeviceManager: subSystem:android_usb,devPath:/devices/virtual/android_usb/android0
03-12 10:48:45.451   634   688 V UsbDeviceManager: USB UEVENT: {USB_STATE=CONFIGURED, SUBSYSTEM=android_usb, SEQNUM=1976, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
03-12 10:48:45.451   634   688 D UsbDeviceManager: subSystem:android_usb,devPath:/devices/virtual/android_usb/android0
03-12 10:48:45.453   634   634 I SystemServer: StartBackgroundDexOptService
03-12 10:48:45.454   634   634 I SystemServiceManager: Starting com.android.server.pm.ShortcutService
$$
> Lifecycle

> 03-12 10:48:45.460   634   634 I SystemServiceManager: Starting com.android.server.pm.LauncherAppsService

> 03-12 10:48:45.463   634   634 I SystemServiceManager: Starting com.android.server.media.projection.MediaProjectionManagerService

> 03-12 10:48:45.476   634   634 I SystemServiceManager: Starting com.android.server.MmsServiceBroker

> 03-12 10:48:45.482   634   634 I SystemServiceManager: Starting com.android.server.retaildemo.RetailDemoModeService

> 03-12 10:48:45.484   634   747 I ServiceThread: Enabled StrictMode logging for RetailDemoModeService looper.

> 03-12 10:48:45.597   634   634 I LockSettingsService: Migrated lock settings to new location

> 03-12 10:48:45.606   634   634 I LockSettingsService: Migrated per-user lock settings to new location

> 03-12 10:48:45.621   634   634 I LockSettingsService: Migrated biometric weak to use the fallback instead

> 03-12 10:48:45.628   634   634 I LockSettingsService: Migrated lockscreen disabled flag

> 03-12 10:48:45.636   634   634 E LockSettingsStorage: Cannot read file java.io.FileNotFoundException: /data/system/gatekeeper.password.key: open failed: ENOENT (No such file or directory)

> 03-12 10:48:45.637   634   634 E LockSettingsStorage: Cannot read file java.io.FileNotFoundException: /data/system/password.key: open failed: ENOENT (No such file or directory)

> 03-12 10:48:45.637   634   634 E LockSettingsStorage: Cannot read file java.io.FileNotFoundException: /data/system/gatekeeper.pattern.key: open failed: ENOENT (No such file or directory)

> 03-12 10:48:45.638   634   634 E LockSettingsStorage: Cannot read file java.io.FileNotFoundException: /data/system/gatekeeper.gesture.key: open failed: ENOENT (No such file or directory)

> 03-12 10:48:45.638   634   634 E LockSettingsStorage: Cannot read file java.io.FileNotFoundException: /data/system/gesture.key: open failed: ENOENT (No such file or directory)

> 03-12 10:48:45.638   634   634 I SystemServiceManager: Starting phase 480

> 03-12 10:48:45.645   634   634 I SystemServiceManager: Starting phase 500

> 03-12 10:48:45.658   634   634 D ConnectivityService: listenForNetwork for uid/pid:1000/634 NetworkRequest [ LISTEN id=3, [] ]

> 03-12 10:48:45.664   634   634 I WifiService: WifiService starting up with Wi-Fi disabled

> 03-12 10:48:45.684   634   724 D ConnectivityService: Got NetworkFactory Messenger for Ethernet

> 03-12 10:48:45.684   634   724 D ConnectivityService: NetworkFactory connected

> 03-12 10:48:45.707   634   634 D ZenLog  : config: cleanUpZenRules,ZenModeConfig[user=0,allowCalls=true,allowRepeatCallers=false,allowMessages=false,allowCallsFrom=contacts,allowMessagesFrom=contacts,allowReminders=true,allowEvents=true,allowWhenScreenOff=true,allowWhenScreenOn=true,automaticRules={2c21b4fcb5a540fc9587f7ad66d59524=ZenRule[enabled=false,snoozing=false,name=Event,zenMode=ZEN_MODE_ALARMS,conditionId=condition://android/event?userId=-10000&calendar=&reply=1,condition=null,component=ComponentInfo{android/com.android.server.notification.EventConditionProvider},id=2c21b4fcb5a540fc9587f7ad66d59524,creationTime=1552358924867,enabler=null], 955cd19a1a4145b5957ad73af6e34b95=ZenRule[enabled=false,snoozing=false,name=Weeknight,zenMode=ZEN_MODE_ALARMS,conditionId=condition://android/schedule?days=1.2.3.4.5&start=22.0&end=7.0&exitAtAlarm=false,condition=null,component=ComponentInfo{android/com.android.server.notification.ScheduleConditionProvider},id=955cd19a1a4145b5957ad73af6e34b95,creationTime=1552358924866,enabler=null], 1778626c28974f46a0b34b5c8b01ae0b=ZenRule[enabled=false,snoozing=false,name=Weekend,zenMode=ZEN_MODE_ALARMS,conditionId=condition://android/schedule?days=6.7&start=23.30&end=10.0&exitAtAlarm=false,condition=null,component=ComponentInfo{android/com.android.server.notification.ScheduleConditionProvider},id=1778626c28974f46a0b34b5c8b01ae0b,creationTime=1552358924866,enabler=null]},manualRule=null],Diff[]

> 03-12 10:48:45.707   634   634 D ZenLog  : set_zen_mode: off,onSystemReady

> 03-12 10:48:45.711   634   634 W SoundTriggerHelper: listModules status=0, \# of modules=0

> 03-12 10:48:45.745   634   634 D VoldConnector: SND -> {4 volume remount_uid 1000 none}

> 03-12 10:48:45.746   253   263 D vold    : Remounting 1000 as mode none

> 03-12 10:48:45.746   634   657 W KeyguardServiceDelegate: onScreenTurningOn(): no keyguard service!

> 03-12 10:48:45.752   253   263 D vold    : Found matching PID 271

> 03-12 10:48:45.752   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.752   253   263 D vold    : Found matching PID 272

> 03-12 10:48:45.752   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.753   253   263 D vold    : Found matching PID 385

> 03-12 10:48:45.753   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.753   253   263 D vold    : Found matching PID 634

> 03-12 10:48:45.798   634   700 D VoldConnector: RCV <- {200 4 Command succeeded}

> 03-12 10:48:45.800   634   634 D VoldConnector: SND -> {5 volume remount_uid 1000 none}

> 03-12 10:48:45.800   253   263 D vold    : Remounting 1000 as mode none

> 03-12 10:48:45.808   253   263 D vold    : Found matching PID 271

> 03-12 10:48:45.808   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.808   253   263 D vold    : Found matching PID 272

> 03-12 10:48:45.808   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.809   253   263 D vold    : Found matching PID 385

> 03-12 10:48:45.809   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.809   253   263 D vold    : Found matching PID 634

> 03-12 10:48:45.814   634   700 D VoldConnector: RCV <- {200 5 Command succeeded}

> 03-12 10:48:45.816   634   634 D VoldConnector: SND -> {6 volume remount_uid 1027 none}

> 03-12 10:48:45.817   253   263 D vold    : Remounting 1027 as mode none

> 03-12 10:48:45.825   634   700 D VoldConnector: RCV <- {200 6 Command succeeded}

> 03-12 10:48:45.827   634   634 D VoldConnector: SND -> {7 volume remount_uid 1027 none}

> 03-12 10:48:45.830   253   263 D vold    : Remounting 1027 as mode none

> 03-12 10:48:45.838   634   700 D VoldConnector: RCV <- {200 7 Command succeeded}

> 03-12 10:48:45.843   634   634 D VoldConnector: SND -> {8 volume remount_uid 2000 none}

> 03-12 10:48:45.844   253   263 D vold    : Remounting 2000 as mode none

> 03-12 10:48:45.853   253   263 D vold    : Found matching PID 274

> 03-12 10:48:45.853   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.854   253   263 D vold    : Found matching PID 735

> 03-12 10:48:45.854   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.855   634   700 D VoldConnector: RCV <- {200 8 Command succeeded}

> 03-12 10:48:45.856   634   634 D VoldConnector: SND -> {9 volume remount_uid 2000 none}

> 03-12 10:48:45.857   253   263 D vold    : Remounting 2000 as mode none

> 03-12 10:48:45.865   253   263 D vold    : Found matching PID 274

> 03-12 10:48:45.865   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.866   253   263 D vold    : Found matching PID 735

> 03-12 10:48:45.866   253   263 W vold    : Skipping due to root namespace

> 03-12 10:48:45.866   634   700 D VoldConnector: RCV <- {200 9 Command succeeded}

> 03-12 10:48:45.868   634   634 D VoldConnector: SND -> {10 volume remount_uid 10014 none}

> 03-12 10:48:45.869   253   263 D vold    : Remounting 10014 as mode none

> 03-12 10:48:45.872   253   711 I vold    : Trimmed 648372224 bytes on /cache in 1466ms

> 03-12 10:48:45.873   253   711 D vold    : Starting trim of /metadata

> 03-12 10:48:45.873   634   700 D VoldConnector: RCV <- {662 /cache 648372224 1466486773}

> 03-12 10:48:45.877   634   700 D VoldConnector: RCV <- {200 10 Command succeeded}

> 03-12 10:48:45.879   634   634 D VoldConnector: SND -> {11 volume remount_uid 10014 none}

> 03-12 10:48:45.881   253   263 D vold    : Remounting 10014 as mode none

> 03-12 10:48:45.888   634   700 D VoldConnector: RCV <- {200 11 Command succeeded}

> 03-12 10:48:45.890   634   634 D VoldConnector: SND -> {12 volume remount_uid 1002 none}

> 03-12 10:48:45.891   253   263 D vold    : Remounting 1002 as mode none

> 03-12 10:48:45.899   634   700 D VoldConnector: RCV <- {200 12 Command succeeded}

> 03-12 10:48:45.900   634   634 D VoldConnector: SND -> {13 volume remount_uid 1002 none}

> 03-12 10:48:45.901   253   263 D vold    : Remounting 1002 as mode none

> 03-12 10:48:45.908   253   711 I vold    : Trimmed 12242944 bytes on /metadata in 35ms

> 03-12 10:48:45.909   634   700 D VoldConnector: RCV <- {200 13 Command succeeded}

> 03-12 10:48:45.911   634   634 W VoiceInteractionManagerService: no available voice interaction services found for user 0

> 03-12 10:48:45.911   634   634 W VoiceInteractionManagerService: no available voice recognition services found for user 0

> 03-12 10:48:45.914   253   711 D vold    : Starting trim of none

> 03-12 10:48:45.914   253   711 W vold    : Failed to open none: No such file or directory

> 03-12 10:48:45.915   634   700 D VoldConnector: RCV <- {662 /metadata 12242944 35514503}

> 03-12 10:48:45.917   634   634 I SyncManager: No initial accounts

> 03-12 10:48:45.917   634   634 I SyncManager: No initial status

> 03-12 10:48:45.918   634   634 I SyncManager: No initial statistics

> 03-12 10:48:45.939   634   634 D VoldConnector: SND -> {14 volume remount_uid 10007 none}

> 03-12 10:48:45.940   253   263 D vold    : Remounting 10007 as mode none

> 03-12 10:48:45.946   634   700 D VoldConnector: RCV <- {200 14 Command succeeded}

> 03-12 10:48:45.948   634   634 D VoldConnector: SND -> {15 volume remount_uid 10016 none}

> 03-12 10:48:45.948   253   263 D vold    : Remounting 10016 as mode none

> 03-12 10:48:45.955   634   700 D VoldConnector: RCV <- {200 15 Command succeeded}

> 03-12 10:48:45.956   634   634 D VoldConnector: SND -> {16 volume remount_uid 10016 none}

> 03-12 10:48:45.957   253   263 D vold    : Remounting 10016 as mode none

> 03-12 10:48:45.963   634   700 D VoldConnector: RCV <- {200 16 Command succeeded}

> 03-12 10:48:45.964   634   634 D VoldConnector: SND -> {17 volume remount_uid 10005 none}

> 03-12 10:48:45.965   253   263 D vold    : Remounting 10005 as mode none

> 03-12 10:48:45.971   634   700 D VoldConnector: RCV <- {200 17 Command succeeded}

> 03-12 10:48:45.972   634   634 D VoldConnector: SND -> {18 volume remount_uid 10005 none}

> 03-12 10:48:45.972   253   263 D vold    : Remounting 10005 as mode none

> 03-12 10:48:45.978   634   700 D VoldConnector: RCV <- {200 18 Command succeeded}

> 03-12 10:48:45.980   634   634 D VoldConnector: SND -> {19 volume remount_uid 10003 none}

> 03-12 10:48:45.980   253   263 D vold    : Remounting 10003 as mode none

> 03-12 10:48:45.992   634   700 D VoldConnector: RCV <- {200 19 Command succeeded}

> 03-12 10:48:45.993   634   634 D VoldConnector: SND -> {20 volume remount_uid 10003 none}

> 03-12 10:48:45.994   253   263 D vold    : Remounting 10003 as mode none

> 03-12 10:48:46.000   634   700 D VoldConnector: RCV <- {200 20 Command succeeded}

> 03-12 10:48:46.004   634   634 D VoldConnector: SND -> {21 volume remount_uid 10013 none}

> 03-12 10:48:46.005   253   263 D vold    : Remounting 10013 as mode none

> 03-12 10:48:46.010   634   700 D VoldConnector: RCV <- {200 21 Command succeeded}

> 03-12 10:48:46.011   634   634 D VoldConnector: SND -> {22 volume remount_uid 10013 none}

> 03-12 10:48:46.012   253   263 D vold    : Remounting 10013 as mode none

> 03-12 10:48:46.017   634   700 D VoldConnector: RCV <- {200 22 Command succeeded}

> 03-12 10:48:46.020   634   634 V UserManagerService: Found /data/user_de/0 with serial number -1

> 03-12 10:48:46.021   634   634 D UserManagerService: Serial number missing on /data/user_de/0; assuming current is valid

> 03-12 10:48:46.021   634   634 V UserManagerService: Found /data/user/0 with serial number -1

> 03-12 10:48:46.021   634   634 D UserManagerService: Serial number missing on /data/user/0; assuming current is valid

> 03-12 10:48:46.021   634   634 V UserManagerService: Found /data/system_de/0 with serial number -1

> 03-12 10:48:46.022   634   634 D UserManagerService: Serial number missing on /data/system_de/0; assuming current is valid

> 03-12 10:48:46.024   634   634 D ActivityManager: OK, system ready!

> 03-12 10:48:46.024   634   634 I ActivityManager: System now ready

> 03-12 10:48:46.031   634   648 I SystemServer: Making services ready

> 03-12 10:48:46.031   634   648 I SystemServiceManager: Starting phase 550

> 03-12 10:48:46.032   634   648 D BluetoothManagerService: Bluetooth boot completed

> 03-12 10:48:46.032   634   648 D BluetoothManagerService: Auto-enabling Bluetooth.

> 03-12 10:48:46.033   634   654 D BluetoothManagerService: MESSAGE_ENABLE(0): mBluetooth = null

> 03-12 10:48:46.033   634   648 E InputMethodManagerService: Ignoring updateSystemUiLocked due to an invalid token. uid:1000 token:null

> 03-12 10:48:46.095   634   648 W InputMethodManagerService: Reset the default IME as “Resource” is ready here.

> 03-12 10:48:46.118   634   648 I InputMethodManagerService: Default found, using com.android.inputmethod.latin/.LatinIME

> 03-12 10:48:46.119   634   648 W InputMethodManagerService: Illegal subtype state: old subtype = null, new subtype = android.view.inputmethod.InputMethodSubtype@c9194f98

> 03-12 10:48:46.119   634   654 W ActivityManager: Slow operation: 83ms so far, now at startProcess: returned from zygote!

> 03-12 10:48:46.120   634   654 W ActivityManager: Slow operation: 84ms so far, now at startProcess: done updating battery stats

> 03-12 10:48:46.120   634   654 W ActivityManager: Slow operation: 84ms so far, now at startProcess: building log message

> 03-12 10:48:46.120   634   654 I ActivityManager: Start proc 766:com.android.bluetooth/1002 for service com.android.bluetooth/.btservice.AdapterService

> 03-12 10:48:46.120   634   654 W ActivityManager: Slow operation: 84ms so far, now at startProcess: starting to update pids map

> 03-12 10:48:46.120   634   654 W ActivityManager: Slow operation: 84ms so far, now at startProcess: done updating pids map

> 03-12 10:48:46.121   634   654 W ActivityManager: Slow operation: 85ms so far, now at startProcess: done starting proc!

> 03-12 10:48:46.122   634   634 W VoiceInteractionManagerService: no available voice interaction services found for user 0

> 03-12 10:48:46.122   634   634 W VoiceInteractionManagerService: no available voice recognition services found for user 0

> 03-12 10:48:46.134   634   634 I ActivityManager: Start proc 779:com.android.systemui/u0a14 for added application com.android.systemui

> 03-12 10:48:46.135   634   634 I ActivityManager: START u0 {act=android.intent.action.MAIN cat=[android.intent.category.HOME] flg=0x10000100 cmp=com.android.settings/.CryptKeeper} from uid 0 on display 0

> 03-12 10:48:46.144   634   634 I ActivityManager: Loaded persisted task ids for user 0

> 03-12 10:48:46.162   634   634 I ActivityManager: Start proc 796:com.android.settings:CryptKeeper/1000 for activity com.android.settings/.CryptKeeper

> 03-12 10:48:46.166   634   634 I SystemServer: Enabled StrictMode for system server main thread.

> 03-12 10:48:46.180   634   648 I ActivityManager: Start proc 809:com.android.inputmethod.latin/u0a19 for service com.android.inputmethod.latin/.LatinIME

> 03-12 10:48:46.180   634   648 V InputMethodManagerService: Adding window token: android.os.Binder@43905ea

> 03-12 10:48:46.181   634   699 D MountService: Thinking about init, mSystemReady=true, mDaemonConnected=true

> 03-12 10:48:46.181   634   699 D MountService: Setting up emulation state, initlocked=false

> 03-12 10:48:46.181   634   699 D CryptdConnector: SND -> {1 cryptfs unlock_user_key 0 0 ! !}

> 03-12 10:48:46.182   253   264 D vold    : e4crypt_unlock_user_key 0 serial=0 token_present=0

> 03-12 10:48:46.183   253   264 E vold    : Failed to chmod /data/system_ce/0: No such file or directory

> 03-12 10:48:46.183   253   264 E vold    : Failed to chmod /data/misc_ce/0: No such file or directory

> 03-12 10:48:46.183   253   264 E vold    : Failed to chmod /data/media/0: No such file or directory

> 03-12 10:48:46.183   634   701 D CryptdConnector: RCV <- {200 1 Command succeeded}

> 03-12 10:48:46.186   634   699 D MountService: Thinking about reset, mSystemReady=true, mDaemonConnected=true

> 03-12 10:48:46.189   634   699 D VoldConnector: SND -> {23 volume reset}

> 03-12 10:48:46.190   634   700 D VoldConnector: RCV <- {651 emulated 7}

> 03-12 10:48:46.190   634   700 D VoldConnector: RCV <- {659 emulated}

> 03-12 10:48:46.191   634   700 D VoldConnector: RCV <- {650 emulated 2 “” “”}

> 03-12 10:48:46.191   634   700 D VoldConnector: RCV <- {651 emulated 0}

> 03-12 10:48:46.191   634   700 D VoldConnector: RCV <- {200 23 Command succeeded}

> 03-12 10:48:46.192   634   699 D VoldConnector: SND -> {24 volume user_added 0 0}

> 03-12 10:48:46.193   634   700 D VoldConnector: RCV <- {200 24 Command succeeded}

> 03-12 10:48:46.193   634   653 V MountService: Found primary storage at VolumeInfo{emulated}:

> 03-12 10:48:46.193   634   653 V MountService:     type=EMULATED diskId=null partGuid=null mountFlags=0 mountUserId=-1

> 03-12 10:48:46.193   634   653 V MountService:     state=UNMOUNTED

> 03-12 10:48:46.193   634   653 V MountService:     fsType=null fsUuid=null fsLabel=null

> 03-12 10:48:46.193   634   653 V MountService:     path=null internalPath=null

> 03-12 10:48:46.205   634   699 D VoldConnector: SND -> {25 volume mount emulated 3 -1}

> 03-12 10:48:46.207   253   263 V vold    : Waiting for FUSE to spin up…

> 03-12 10:48:46.207   634   700 D VoldConnector: RCV <- {651 emulated 1}

> 03-12 10:48:46.208   634   700 D VoldConnector: RCV <- {656 emulated /data/media}

> 03-12 10:48:46.208   634   700 D VoldConnector: RCV <- {655 emulated /storage/emulated}

> 03-12 10:48:46.209   634   634 D ConditionProviders.SCP: onConnected

> 03-12 10:48:46.223   634   648 D UsbAlsaManager: Adding ALSA device AlsaDevice: [card: 0, device: 0, type: 1]

> 03-12 10:48:46.224   634   648 D UsbAlsaManager: Adding ALSA device AlsaDevice: [card: 0, device: 0, type: 2]

> 03-12 10:48:46.227   634   731 D BluetoothManagerService: Creating new ProfileServiceConnections object for profile: 1

> 03-12 10:48:46.227   634   731 E BluetoothManagerService: Fail to bind to: Intent { act=android.bluetooth.IBluetoothHeadset }

> 03-12 10:48:46.227   634   731 W BluetoothManagerService: Unable to bind with intent: Intent { act=android.bluetooth.IBluetoothHeadset }

> 03-12 10:48:46.230   634   688 V UsbDeviceManager: USB UEVENT: {SUBSYSTEM=bdi, SEQNUM=1977, ACTION=add, DEVPATH=/devices/virtual/bdi/0:25}

> 03-12 10:48:46.230   634   688 D UsbDeviceManager: subSystem:bdi,devPath:/devices/virtual/bdi/0:25

> 03-12 10:48:46.233   634   688 V UsbDeviceManager: USB UEVENT: {SUBSYSTEM=bdi, SEQNUM=1978, ACTION=add, DEVPATH=/devices/virtual/bdi/0:26}

> 03-12 10:48:46.233   634   688 D UsbDeviceManager: subSystem:bdi,devPath:/devices/virtual/bdi/0:26

> 03-12 10:48:46.234   634   688 V UsbDeviceManager: USB UEVENT: {SUBSYSTEM=bdi, SEQNUM=1979, ACTION=add, DEVPATH=/devices/virtual/bdi/0:27}

> 03-12 10:48:46.234   634   688 D UsbDeviceManager: subSystem:bdi,devPath:/devices/virtual/bdi/0:27

> 03-12 10:48:46.238   634   648 I SystemServer: WebViewFactory preparation

> 03-12 10:48:46.252   634   634 D ZenLog  : set_zen_mode: off,cleanUpZenRules

> 03-12 10:48:46.253   634   634 D ConditionProviders: Subscribing to condition://android/event?userId=-10000&calendar=&reply=1 with ComponentInfo{android/com.android.server.notification.EventConditionProvider}

> 03-12 10:48:46.253   634   634 D ZenLog  : subscribe: condition://android/event?userId=-10000&calendar=&reply=1,ok

> 03-12 10:48:46.253   634   634 D ConditionProviders: Subscribing to condition://android/schedule?days=1.2.3.4.5&start=22.0&end=7.0&exitAtAlarm=false with ComponentInfo{android/com.android.server.notification.ScheduleConditionProvider}

> 03-12 10:48:46.253   634   634 D ZenLog  : subscribe: condition://android/schedule?days=1.2.3.4.5&start=22.0&end=7.0&exitAtAlarm=false,ok

> 03-12 10:48:46.254   634   634 D ConditionProviders: Subscribing to condition://android/schedule?days=6.7&start=23.30&end=10.0&exitAtAlarm=false with ComponentInfo{android/com.android.server.notification.ScheduleConditionProvider}

> 03-12 10:48:46.254   634   634 D ZenLog  : subscribe: condition://android/schedule?days=6.7&start=23.30&end=10.0&exitAtAlarm=false,ok

> 03-12 10:48:46.258   634   700 D VoldConnector: RCV <- {651 emulated 2}

> 03-12 10:48:46.258   634   700 D VoldConnector: RCV <- {200 25 Command succeeded}

> --------- beginning of crash

> 03-12 10:48:46.271   796   796 E AndroidRuntime: FATAL EXCEPTION: main

> 03-12 10:48:46.271   796   796 E AndroidRuntime: Process: com.android.settings:CryptKeeper, PID: 796

> 03-12 10:48:46.271   796   796 E AndroidRuntime: java.lang.RuntimeException: Unable to instantiate application com.android.settings.SettingsApplication: java.lang.ClassNotFoundException: Didn’t find class “com.android.settings.SettingsApplication” on path: DexPathList[[zip file “/system/priv-app/Settings/Settings.apk”],nativeLibraryDirectories=[/system/priv-app/Settings/lib/arm64, /system/priv-app/Settings/Settings.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.app.LoadedApk.makeApplication(LoadedApk.java:802)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.app.ActivityThread.handleBindApplication(ActivityThread.java:5379)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.app.ActivityThread.-wrap2(ActivityThread.java)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.app.ActivityThread
$$
H.handleMessage(ActivityThread.java:1546)
03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.os.Handler.dispatchMessage(Handler.java:102)
03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.os.Looper.loop(Looper.java:154)
03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.app.ActivityThread.main(ActivityThread.java:6121)
03-12 10:48:46.271   796   796 E AndroidRuntime: 	at java.lang.reflect.Method.invoke(Native Method)
03-12 10:48:46.271   796   796 E AndroidRuntime: 	at com.android.internal.os.ZygoteInit
$$
> MethodAndArgsCaller.run(ZygoteInit.java:912)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:802)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: Caused by: java.lang.ClassNotFoundException: Didn’t find class “com.android.settings.SettingsApplication” on path: DexPathList[[zip file “/system/priv-app/Settings/Settings.apk”],nativeLibraryDirectories=[/system/priv-app/Settings/lib/arm64, /system/priv-app/Settings/Settings.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at dalvik.system.BaseDexClassLoader.findClass(BaseDexClassLoader.java:56)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at java.lang.ClassLoader.loadClass(ClassLoader.java:380)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at java.lang.ClassLoader.loadClass(ClassLoader.java:312)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.app.Instrumentation.newApplication(Instrumentation.java:992)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	at android.app.LoadedApk.makeApplication(LoadedApk.java:796)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	… 9 more

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 	Suppressed: java.io.IOException: No original dex files found for dex location /system/priv-app/Settings/Settings.apk

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexFile.openDexFileNative(Native Method)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexFile.openDexFile(DexFile.java:367)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexFile.(DexFile.java:112)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexFile.(DexFile.java:77)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexPathList.loadDexFile(DexPathList.java:359)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexPathList.makeElements(DexPathList.java:323)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexPathList.makeDexElements(DexPathList.java:263)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.DexPathList.(DexPathList.java:126)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.BaseDexClassLoader.(BaseDexClassLoader.java:48)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at dalvik.system.PathClassLoader.(PathClassLoader.java:64)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at com.android.internal.os.PathClassLoaderFactory.createClassLoader(PathClassLoaderFactory.java:43)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.ApplicationLoaders.getClassLoader(ApplicationLoaders.java:58)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.LoadedApk.createOrUpdateClassLoaderLocked(LoadedApk.java:520)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.LoadedApk.getClassLoader(LoadedApk.java:553)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.ActivityThread.getTopLevelResources(ActivityThread.java:1867)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.LoadedApk.getResources(LoadedApk.java:766)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.ContextImpl.(ContextImpl.java:2038)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.ContextImpl.createAppContext(ContextImpl.java:1983)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		at android.app.ActivityThread.handleBindApplication(ActivityThread.java:5296)

> 03-12 10:48:46.271   796   796 E AndroidRuntime: 		… 8 more

> 03-12 10:48:46.278   634   648 I ActivityManager: Start proc 845:WebViewLoader-armeabi-v7a/1037 [android.webkit.WebViewFactory
$$
RelroFileCreator] for 
03-12 10:48:46.297   634   648 I ActivityManager: Start proc 862:WebViewLoader-arm64-v8a/1037 [android.webkit.WebViewFactory
$$
> RelroFileCreator] for

> 03-12 10:48:46.299   634   648 D NetworkManagement: enabling bandwidth control

> 03-12 10:48:46.303   634   795 W ActivityManager:   Force finishing activity com.android.settings/.CryptKeeper

> 03-12 10:48:46.309   634   829 D UsbHostManager: Added device UsbDevice[mName=/dev/bus/usb/005/003,mVendorId=4292,mProductId=60016,mClass=0,mSubclass=0,mProtocol=0,mManufacturerName=Silicon Labs,mProductName=CP2105 Dual USB to UART Bridge Controller,mVersion=2.0,mSerialNumber=00A6B0B4,mConfigurations=[

> 03-12 10:48:46.309   634   829 D UsbHostManager: UsbConfiguration[mId=1,mName=null,mAttributes=128,mMaxPower=50,mInterfaces=[

> 03-12 10:48:46.309   634   829 D UsbHostManager: UsbInterface[mId=0,mAlternateSetting=0,mName=Enhanced Com Port,mClass=255,mSubclass=0,mProtocol=0,mEndpoints=[

> 03-12 10:48:46.309   634   829 D UsbHostManager: UsbEndpoint[mAddress=129,mAttributes=2,mMaxPacketSize=64,mInterval=0]

> 03-12 10:48:46.309   634   829 D UsbHostManager: UsbEndpoint[mAddress=1,mAttributes=2,mMaxPacketSize=64,mInterval=0]]

> 03-12 10:48:46.309   634   829 D UsbHostManager: UsbInterface[mId=1,mAlternateSetting=0,mName=Standard Com Port,mClass=255,mSubclass=0,mProtocol=0,mEndpoints=[

> 03-12 10:48:46.309   634   829 D UsbHostManager: UsbEndpoint[mAddress=130,mAttributes=2,mMaxPacketSize=32,mInterval=0]

> 03-12 10:48:46.309   634   829 D UsbHostManager: UsbEndpoint[mAddress=2,mAttributes=2,mMaxPacketSize=32,mInterval=0]]]]

> 03-12 10:48:46.309   634   829 W ContextImpl: Calling a method in the system process without a qualified user: android.app.ContextImpl.sendBroadcast:877 com.android.server.usb.UsbSettingsManager.deviceAttached:739 com.android.server.usb.UsbHostManager.endUsbDeviceAdded:225 com.android.server.usb.UsbHostManager.monitorUsbHostBus:-2 com.android.server.usb.UsbHostManager.-wrap0:-1

> 03-12 10:48:46.375   634   634 W ContextImpl: Calling a method in the system process without a qualified user: android.app.ContextImpl.startService:1357 com.android.server.content.SyncManager
> KaTeX parse error: Expected 'EOF', got '&' at position 228: …?days=1.2.3.4.5&̲start=22.0&end=…
> Stub
$$
Proxy@a6cd61
03-12 10:48:47.192   634   645 D ConnectivityService: listenForNetwork for uid/pid:10014/779 NetworkRequest [ LISTEN id=4, [] ]
03-12 10:48:47.252   779  1088 E ActivityThread: Failed to find provider info for com.android.contacts
03-12 10:48:47.398   634   645 I ActivityManager: user 0 is still locked. Cannot load recents
03-12 10:48:47.437   634   649 W WindowManager: Keyguard drawn timeout. Setting mKeyguardDrawComplete
03-12 10:48:47.552   634   655 W ActivityManager: Unable to mark boot complete for abi: arm64-v8a (Failed to execute markbootcomplete [arm64]: -1)
03-12 10:48:47.552   634   655 W ActivityManager: Unable to mark boot complete for abi: armeabi-v7a (Failed to execute markbootcomplete [arm]: -1)
03-12 10:48:47.553   634   655 I SystemServiceManager: Starting phase 1000
03-12 10:48:47.568   634   716 D WifiService: New client listening to asynchronous messages
03-12 10:48:47.578   634   688 V UsbDeviceManager: USB UEVENT: {ASYNC=0, SUBSYSTEM=firmware, SEQNUM=1981, TIMEOUT=60, ACTION=add, DEVPATH=/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_pram_data2.bin, FIRMWARE=ak7755_pram_data2.bin}
03-12 10:48:47.578   634   688 D UsbDeviceManager: subSystem:firmware,devPath:/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_pram_data2.bin
03-12 10:48:47.581   634   688 V UsbDeviceManager: USB UEVENT: {SUBSYSTEM=firmware, SEQNUM=1982, ACTION=remove, DEVPATH=/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_pram_data2.bin}
03-12 10:48:47.581   634   688 D UsbDeviceManager: subSystem:firmware,devPath:/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_pram_data2.bin
03-12 10:48:47.581   634   688 V UsbDeviceManager: USB UEVENT: {ASYNC=0, SUBSYSTEM=firmware, SEQNUM=1983, TIMEOUT=60, ACTION=add, DEVPATH=/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_cram_data2.bin, FIRMWARE=ak7755_cram_data2.bin}
03-12 10:48:47.581   634   688 D UsbDeviceManager: subSystem:firmware,devPath:/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_cram_data2.bin
03-12 10:48:47.585   634   688 V UsbDeviceManager: USB UEVENT: {SUBSYSTEM=firmware, SEQNUM=1984, ACTION=remove, DEVPATH=/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_cram_data2.bin}
03-12 10:48:47.585   634   688 D UsbDeviceManager: subSystem:firmware,devPath:/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_cram_data2.bin
03-12 10:48:47.585   634   688 V UsbDeviceManager: USB UEVENT: {ASYNC=0, SUBSYSTEM=firmware, SEQNUM=1985, TIMEOUT=60, ACTION=add, DEVPATH=/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_ofreg_data2.bin, FIRMWARE=ak7755_ofreg_data2.bin}
03-12 10:48:47.586   634   688 D UsbDeviceManager: subSystem:firmware,devPath:/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_ofreg_data2.bin
03-12 10:48:47.590   634   688 V UsbDeviceManager: USB UEVENT: {SUBSYSTEM=firmware, SEQNUM=1986, ACTION=remove, DEVPATH=/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_ofreg_data2.bin}
03-12 10:48:47.590   634   688 D UsbDeviceManager: subSystem:firmware,devPath:/devices/platform/ff120000.i2c/i2c-2/2-0018/firmware/ak7755_ofreg_data2.bin
03-12 10:48:47.593   634   655 I ActivityManager: Start proc 1128:android.ext.services/u0a2 for on-hold
03-12 10:48:47.595   634   655 D ActivityManager: Finishing user boot 0
03-12 10:48:47.598   634   655 D CryptdConnector: SND -&gt; {2 cryptfs prepare_user_storage ! 0 0 2}
03-12 10:48:47.599   253   264 D vold    : e4crypt_prepare_user_storage for volume null, user 0, serial 0, flags 2
03-12 10:48:47.599   253   264 D vold    : Preparing: /data/system_ce/0
03-12 10:48:47.600   253   264 D vold    : Preparing: /data/misc_ce/0
03-12 10:48:47.600   253   264 D vold    : Preparing: /data/media/0
03-12 10:48:47.600   253   264 D vold    : Preparing: /data/data
03-12 10:48:47.600   634   701 D CryptdConnector: RCV &lt;- {200 2 Command succeeded}
03-12 10:48:47.605   634   655 V UserManagerService: Found /data/user/0 with serial number 0
03-12 10:48:47.606   634   655 V UserManagerService: Found /data/system_ce/0 with serial number -1
03-12 10:48:47.606   634   655 D UserManagerService: Serial number missing on /data/system_ce/0; assuming current is valid
03-12 10:48:47.606   634   655 V PackageManager: reconcileAppsData for null u0 0x2
03-12 10:48:47.702   634   655 V PackageManager: reconcileAppsData finished 47 packages
03-12 10:48:47.703   634   648 D BluetoothManagerService: User 0 unlocked
03-12 10:48:47.703   634   648 D MountService: onUnlockUser 0
03-12 10:48:47.703   634   654 D BluetoothManagerService: MESSAGE_USER_UNLOCKED
03-12 10:48:47.703   634   648 D VoldConnector: SND -&gt; {26 volume user_started 0}
03-12 10:48:47.704   634   724 D ConnectivityService: Got NetworkFactory Messenger for WIFI
03-12 10:48:47.704   634   724 D ConnectivityService: Got NetworkFactory Messenger for WIFI_UT
03-12 10:48:47.704   634   724 D ConnectivityService: NetworkFactory connected
03-12 10:48:47.704   634   724 D ConnectivityService: NetworkFactory connected
03-12 10:48:47.704   253   263 D vold    : Linking /storage/emulated/0 to /mnt/user/0/primary
03-12 10:48:47.705   634   700 D VoldConnector: RCV &lt;- {200 26 Command succeeded}
03-12 10:48:47.707   634   699 D MountService: Volume emulated broadcasting mounted to UserHandle{0}
03-12 10:48:47.716   634   634 I InputMethodManagerService: All the enabled IMEs are gone. Reset default enabled IMEs.
03-12 10:48:47.735   634   648 V BackupManagerService: No ancestral data
03-12 10:48:47.746   634   648 V BackupManagerService: Starting with transport com.google.android.gms/.backup.BackupTransportService
03-12 10:48:47.748   634   648 V BackupManagerService: Found transports: 1
03-12 10:48:47.750   634   648 W BackupManagerService: Proposed transport ComponentInfo{android/com.android.internal.backup.LocalTransportService} not whitelisted; ignoring
03-12 10:48:47.751   634   648 I BackupManagerService: Backup enable apparently not migrated
03-12 10:48:47.751   634   648 I BackupManagerService: Migrating enable state true
03-12 10:48:47.753   634   648 I BackupManagerService: Backup enabled =&gt; true
03-12 10:48:47.768   634  1159 V KeyValueBackupJob: Scheduling k/v pass in 244 minutes
03-12 10:48:47.768   634   648 W AppWidgetServiceImpl: Failed to read state: java.io.FileNotFoundException: /data/system/users/0/appwidgets.xml (No such file or directory)
03-12 10:48:47.768   634   648 V AppWidgetServiceImpl: Upgrading widget database from 0 to 1
03-12 10:48:47.769   634   648 W VoiceInteractionManagerService: no available voice interaction services found for user 0
03-12 10:48:47.769   634   648 W VoiceInteractionManagerService: no available voice recognition services found for user 0
03-12 10:48:47.771   634   648 I ActivityManager: Loading recents for user 0 into memory.
03-12 10:48:47.787   634   648 I ActivityManager: Start proc 1168:com.android.nfc/1027 for added application com.android.nfc
03-12 10:48:47.800   634   648 I ActivityManager: Start proc 1180:com.evergrande.idpad.titket.oceanFlower/1000 for added application com.evergrande.idpad.titket.oceanFlower
03-12 10:48:47.813   634   648 I ActivityManager: Start proc 1194:com.cghs.stresstest/1000 for added application com.cghs.stresstest
03-12 10:48:47.815   634   648 I PreBootBroadcaster: Pre-boot of {com.android.managedprovisioning/com.android.managedprovisioning.PreBootListener} for user 0
03-12 10:48:47.821   634   649 I ActivityManager: Config changes=20 {1.0 ?mcc?mnc [en_US] ldltr sw457dp w457dp h659dp 280dpi nrml port finger -keyb/v/h -nav/h s.5}
03-12 10:48:47.855   634   647 I ActivityManager: Start proc 1223:com.android.printspooler/u0a23 for service com.android.printspooler/.model.PrintSpoolerService
03-12 10:48:47.883   634  1230 I ActivityManager: Start proc 1240:com.android.keychain/1000 for service com.android.keychain/.KeyChainService
03-12 10:48:47.908  1180  1180 W ContextImpl: Calling a method in the system process without a qualified user: android.app.ContextImpl.startService:1357 android.content.ContextWrapper.startService:613 com.evergrande.idpad.base.IDApplication.init:84 com.evergrande.idpad.base.IDApplication.onCreate:31 android.app.Instrumentation.callApplicationOnCreate:1024 
03-12 10:48:47.951  1240  1240 E AndroidRuntime: FATAL EXCEPTION: main
03-12 10:48:47.951  1240  1240 E AndroidRuntime: Process: com.android.keychain, PID: 1240
03-12 10:48:47.951  1240  1240 E AndroidRuntime: java.lang.RuntimeException: Unable to instantiate service com.android.keychain.KeyChainService: java.lang.ClassNotFoundException: Didn&\#x27;t find class &quot;com.android.keychain.KeyChainService&quot; on path: DexPathList[[zip file &quot;/system/priv-app/KeyChain/KeyChain.apk&quot;],nativeLibraryDirectories=[/system/priv-app/KeyChain/lib/arm64, /system/priv-app/KeyChain/KeyChain.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at android.app.ActivityThread.handleCreateService(ActivityThread.java:3177)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at android.app.ActivityThread.-wrap5(ActivityThread.java)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at android.app.ActivityThread
$$
> H.handleMessage(ActivityThread.java:1568)

> 03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at android.os.Handler.dispatchMessage(Handler.java:102)

> 03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at android.os.Looper.loop(Looper.java:154)

> 03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at android.app.ActivityThread.main(ActivityThread.java:6121)

> 03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at java.lang.reflect.Method.invoke(Native Method)

> 03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at com.android.internal.os.ZygoteInit
$$
MethodAndArgsCaller.run(ZygoteInit.java:912)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:802)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: Caused by: java.lang.ClassNotFoundException: Didn&\#x27;t find class &quot;com.android.keychain.KeyChainService&quot; on path: DexPathList[[zip file &quot;/system/priv-app/KeyChain/KeyChain.apk&quot;],nativeLibraryDirectories=[/system/priv-app/KeyChain/lib/arm64, /system/priv-app/KeyChain/KeyChain.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at dalvik.system.BaseDexClassLoader.findClass(BaseDexClassLoader.java:56)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at java.lang.ClassLoader.loadClass(ClassLoader.java:380)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at java.lang.ClassLoader.loadClass(ClassLoader.java:312)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	at android.app.ActivityThread.handleCreateService(ActivityThread.java:3174)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	... 8 more
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 	Suppressed: java.io.IOException: No original dex files found for dex location /system/priv-app/KeyChain/KeyChain.apk
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexFile.openDexFileNative(Native Method)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexFile.openDexFile(DexFile.java:367)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexFile.&lt;init&gt;(DexFile.java:112)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexFile.&lt;init&gt;(DexFile.java:77)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexPathList.loadDexFile(DexPathList.java:359)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexPathList.makeElements(DexPathList.java:323)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexPathList.makeDexElements(DexPathList.java:263)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.DexPathList.&lt;init&gt;(DexPathList.java:126)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.BaseDexClassLoader.&lt;init&gt;(BaseDexClassLoader.java:48)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at dalvik.system.PathClassLoader.&lt;init&gt;(PathClassLoader.java:64)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at com.android.internal.os.PathClassLoaderFactory.createClassLoader(PathClassLoaderFactory.java:43)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.ApplicationLoaders.getClassLoader(ApplicationLoaders.java:58)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.LoadedApk.createOrUpdateClassLoaderLocked(LoadedApk.java:520)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.LoadedApk.getClassLoader(LoadedApk.java:553)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.ActivityThread.getTopLevelResources(ActivityThread.java:1867)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.LoadedApk.getResources(LoadedApk.java:766)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.ContextImpl.&lt;init&gt;(ContextImpl.java:2038)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.ContextImpl.createAppContext(ContextImpl.java:1983)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.ActivityThread.handleBindApplication(ActivityThread.java:5296)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.ActivityThread.-wrap2(ActivityThread.java)
03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		at android.app.ActivityThread
$$
> H.handleMessage(ActivityThread.java:1546)

> 03-12 10:48:47.951  1240  1240 E AndroidRuntime: 		… 6 more

> --------- beginning of main

> 03-12 10:48:48.041   272   283 D GRALLOC-ROCKCHIP: enter, w : 871, h : 371, format : 0x1, usage : 0x900.

> 03-12 10:48:48.042   272   283 D GRALLOC-ROCKCHIP: enter, w : 871, h : 371, format : 0x1, usage : 0x900.

> 03-12 10:48:48.044   272   283 D GRALLOC-ROCKCHIP: enter, w : 871, h : 371, format : 0x1, usage : 0x900.

> 03-12 10:48:48.047   272   283 E BufferQueueProducer: [Application Error: com.android.keychain] connect: already connected (cur=1 req=1)

> 03-12 10:48:48.047   634   906 D mali_winsys: EGLint new_window_surface(egl_winsys_display*, void*, EGLSurface, EGLConfig, egl_winsys_surface**, egl_color_buffer_format*, EGLBoolean) returns 0x3000

> 03-12 10:48:48.055   272   281 D GRALLOC-ROCKCHIP: enter, w : 371, h : 871, format : 0x1, usage : 0xb00.

> 03-12 10:48:48.057   634   906 D GRALLOC-ROCKCHIP: enter, w : 371, h : 871, format : 0x1, usage : 0xb00.

> 03-12 10:48:48.063   272   281 D GRALLOC-ROCKCHIP: enter, w : 371, h : 871, format : 0x1, usage : 0xb00.

> 03-12 10:48:48.065   634   906 D GRALLOC-ROCKCHIP: enter, w : 371, h : 871, format : 0x1, usage : 0xb00.

> 03-12 10:48:48.080   272   281 D GRALLOC-ROCKCHIP: enter, w : 371, h : 871, format : 0x1, usage : 0xb00.

> 03-12 10:48:48.082   766   965 I bt_hwcfg: Controller OTP bdaddr 6C:21:A2:18:98:23

> 03-12 10:48:48.082   766   965 I bt_hwcfg: vendor lib fwcfg completed

> 03-12 10:48:48.082   766   965 I bt_vendor: firmware callback

> 03-12 10:48:48.082   766   965 I bt_hci  : firmware_config_callback

> 03-12 10:48:48.082   766   935 I bt_core_module: module_start_up Started module “hci_module”

> 03-12 10:48:48.083   634   906 D GRALLOC-ROCKCHIP: enter, w : 371, h : 871, format : 0x1, usage : 0xb00.

> 03-12 10:48:48.084   766  1290 W bt_osi_thread: run_thread: thread id 1290, thread name bt_workqueue started

> 03-12 10:48:48.084   766  1290 I bt_btu  : btu_task pending for preload complete event

> 03-12 10:48:48.084   766  1290 I bt_btu_task: Bluetooth chip preload is complete

> 03-12 10:48:48.084   766  1290 I bt_btu  : btu_task received preload complete event

> 03-12 10:48:48.085  1168  1292 I chatty  : uid=1027(nfc) AsyncTask \#1 expire 254 lines

> 03-12 10:48:48.085  1168  1291 I chatty  : uid=1027(nfc) AsyncTask \#1 expire 248 lines

> 03-12 10:48:48.086  1168  1294 I chatty  : uid=1027(nfc) AsyncTask \#1 expire 157 lines

> 03-12 10:48:48.089   766  1290 I bt_core_module: module_init Initializing module “bte_logmsg_module”

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_HCI

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_L2CAP

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_RFCOMM

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_AVDT

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_AVRC

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_A2D

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_BNEP

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_BTM

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_GAP

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_PAN

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_SDP

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_GATT

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_SMP

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_BTAPP

> 03-12 10:48:48.089   766  1290 I bt_bte  : BTE_InitTraceLevels – TRC_BTIF

> 03-12 10:48:48.089   766  1290 I bt_core_module: module_init Initialized module “bte_logmsg_module”

> 03-12 10:48:48.090   766  1295 W bt_osi_thread: run_thread: thread id 1295, thread name module_wrapper started

> 03-12 10:48:48.090   766  1295 I bt_core_module: module_start_up Starting module “controller_module”

> 03-12 10:48:48.197   634   655 W WindowManager: App freeze timeout expired.

> 03-12 10:48:48.214   766  1295 I bt_core_module: module_start_up Started module “controller_module”

> 03-12 10:48:48.215   766  1295 W bt_osi_thread: run_thread: thread id 1295, thread name module_wrapper exited

> 03-12 10:48:48.216   766  1290 I bt_btm_sec: BTM_SecRegister p_cb_info->p_le_callback == 0x0xd4a34cb9

> 03-12 10:48:48.216   766  1290 I bt_btm_sec: BTM_SecRegister btm_cb.api.p_le_callback = 0x0xd4a34cb9

> 03-12 10:48:48.232   766   944 D BluetoothAdapterProperties: BT_PROPERTY_LOCAL_LE_FEATURES: update from BT controller mNumOfAdvertisementInstancesSupported = 17 mRpaOffloadSupported = true mNumOfOffloadedIrkSupported = 128 mNumOfOffloadedScanFilterSupported = 16 mOffloadedScanResultStorageBytes= 1024 mIsActivityAndEnergyReporting = true mVersSupported = 96 mTotNumOfTrackableAdv = 20 mIsExtendedScanSupported = true mIsDebugLogSupported = true

> 03-12 10:48:48.233   766   944 I bt_btif_storage: btif_storage_get_adapter_property service_mask:0x20000000

> 03-12 10:48:48.233   766   944 D BluetoothAdapterProperties: Address is:22:22:F0:D6:97:CC

> 03-12 10:48:48.236   634   634 D BluetoothManagerService: Bluetooth Adapter name changed to XL-IDP100

> 03-12 10:48:48.236   634   634 D BluetoothManagerService: Stored Bluetooth name: XL-IDP100

> 03-12 10:48:48.237   766   944 D BluetoothAdapterProperties: Name is: XL-IDP100

> 03-12 10:48:48.239   766   944 D BluetoothAdapterProperties: Scan Mode:20

> 03-12 10:48:48.240   766   944 D BluetoothAdapterProperties: Discoverable Timeout:120

> 03-12 10:48:48.240   766   944 W bt_btif : ***********************************************

> 03-12 10:48:48.240   766   944 W bt_btif : BTIF init BDA was 22:22:f0:d6:97:cc

> 03-12 10:48:48.240   766   944 W bt_btif : Controller BDA is 6c:21:a2:18:98:23

> 03-12 10:48:48.240   766   944 W bt_btif : ***********************************************

> 03-12 10:48:48.240   766   944 D BluetoothAdapterProperties: Address is:6C:21:A2:18:98:23

> 03-12 10:48:48.240   766   944 D bt_hci  : do_postload posting postload work item

> 03-12 10:48:48.241   766   965 I bt_hci  : event_postload

> 03-12 10:48:48.241   766   965 I bt_vendor: sco_config_cb

> 03-12 10:48:48.241   766   965 I bt_hci  : sco_config_callback postload finished.

> 03-12 10:48:48.241   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/lpm) for write failed: Permission denied (13)

> 03-12 10:48:48.242   766  1297 W bt_osi_thread: run_thread: thread id 1297, thread name btif_sock started

> 03-12 10:48:48.242   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.243   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.243   766   944 D bt_bte_conf: Device ID record 1 : primary

> 03-12 10:48:48.243   766   944 D bt_bte_conf:   vendorId            = 000f

> 03-12 10:48:48.243   766   944 D bt_bte_conf:   vendorIdSource      = 0001

> 03-12 10:48:48.243   766   944 D bt_bte_conf:   product             = 1200

> 03-12 10:48:48.243   766   944 D bt_bte_conf:   version             = 1436

> 03-12 10:48:48.243   766   944 D bt_bte_conf:   clientExecutableURL =

> 03-12 10:48:48.243   766   944 D bt_bte_conf:   serviceDescription  =

> 03-12 10:48:48.244   766   944 D bt_bte_conf:   documentationURL    =

> 03-12 10:48:48.244   766   944 D bt_bte_conf: bte_load_did_conf no section named DID2.

> 03-12 10:48:48.244   766   935 I bt_stack_manager: event_start_up_stack finished

> 03-12 10:48:48.244   766   934 D BluetoothAdapterState: Current state: PENDING_COMMAND, message: 3

> 03-12 10:48:48.244   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.244   766   934 D BluetoothAdapterProperties: Setting state to 15

> 03-12 10:48:48.244   766   934 I BluetoothAdapterState: Bluetooth adapter state changed: 14-> 15

> 03-12 10:48:48.245   766   934 D BluetoothAdapterService: updateAdapterState() - Broadcasting state to 1 receivers.

> 03-12 10:48:48.245   634   654 D BluetoothManagerService: MESSAGE_BLUETOOTH_STATE_CHANGE: BLE_TURNING_ON > BLE_ON

> 03-12 10:48:48.245   766   934 I BluetoothAdapterState: Entering BleOnState

> 03-12 10:48:48.245   634   654 D BluetoothManagerService: Bluetooth is in LE only mode

> 03-12 10:48:48.245   634   654 D BluetoothManagerService: Binding Bluetooth GATT service

> 03-12 10:48:48.246   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.246   634   654 D BluetoothManagerService: Sending BLE State Change: BLE_TURNING_ON > BLE_ON

> 03-12 10:48:48.247   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.248   634   634 D BluetoothManagerService: BluetoothServiceConnection: com.android.bluetooth.gatt.GattService

> 03-12 10:48:48.248   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.248   634   654 D BluetoothManagerService: MESSAGE_BLUETOOTH_SERVICE_CONNECTED: 2

> 03-12 10:48:48.249   634   654 D BluetoothManagerService: BluetoothGatt Service is Up

> 03-12 10:48:48.250   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.250   766   934 D BluetoothAdapterState: Current state: BLE ON, message: 1

> 03-12 10:48:48.250   766   934 D BluetoothAdapterProperties: Setting state to 11

> 03-12 10:48:48.250   766   934 I BluetoothAdapterState: Bluetooth adapter state changed: 15-> 11

> 03-12 10:48:48.250   766   934 D BluetoothAdapterService: updateAdapterState() - Broadcasting state to 1 receivers.

> 03-12 10:48:48.250   766   934 D BluetoothAdapterService: startCoreServices()

> 03-12 10:48:48.250   766   934 D BluetoothAdapterService: setProfileServiceState() - Starting service com.android.bluetooth.a2dp.A2dpService

> 03-12 10:48:48.251   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.251   766   934 D BluetoothAdapterService: setProfileServiceState() - Starting service com.android.bluetooth.hid.HidService

> 03-12 10:48:48.252   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.254   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.254   766   934 D BluetoothAdapterService: setProfileServiceState() - Starting service com.android.bluetooth.pan.PanService

> 03-12 10:48:48.254   634   654 D BluetoothManagerService: Persisting Bluetooth Setting: 1

> 03-12 10:48:48.254   634   654 D BluetoothManagerService: MESSAGE_BLUETOOTH_STATE_CHANGE: BLE_ON > TURNING_ON

> 03-12 10:48:48.254   634   654 D BluetoothManagerService: Sending BLE State Change: BLE_ON > TURNING_ON

> 03-12 10:48:48.255   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.256   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.256   634   634 D BluetoothA2dp: Proxy object connected

> 03-12 10:48:48.257   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.258   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.259   766   934 I BluetoothAdapterState: Entering PendingCommandState

> 03-12 10:48:48.259   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.261   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.261   766   766 D BluetoothAdapterService: getAdapterService() - returning com.android.bluetooth.btservice.AdapterService@cdabc0c

> 03-12 10:48:48.262   766   965 I bt_vendor: low_power_mode_cb

> 03-12 10:48:48.262   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.262   766   766 D A2dpService: Received start request. Starting profile…

> 03-12 10:48:48.263   766   766 I BluetoothAvrcpServiceJni: classInitNative: succeeds

> 03-12 10:48:48.265   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.266   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.268   766   766 I bt_btif : get_profile_interface avrcp

> 03-12 10:48:48.270   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.271   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.272   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.274   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.275   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.276   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.277   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.278   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.278   766   766 I BluetoothA2dpServiceJni: classInitNative: succeeds

> 03-12 10:48:48.278   766   766 D A2dpStateMachine: make

> 03-12 10:48:48.279   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.279   766   766 I bt_btif : get_profile_interface a2dp

> 03-12 10:48:48.280   766  1301 W bt_osi_thread: run_thread: thread id 1301, thread name media_worker started

> 03-12 10:48:48.280   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.280   766   944 I bt_btif_storage: btif_storage_get_adapter_property service_mask:0x20000008

> 03-12 10:48:48.290  1168  1303 I chatty  : uid=1027(nfc) Thread-2 expire 18 lines

> 03-12 10:48:48.291  1168  1304 I chatty  : uid=1027(nfc) Thread-3 expire 21 lines

> 03-12 10:48:48.291  1168  1305 I chatty  : uid=1027(nfc) Thread-4 expire 18 lines

> 03-12 10:48:48.295   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.296   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.297   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.298   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.298  1168  1307 I chatty  : uid=1027(nfc) SoundPoolThread expire 2 lines

> 03-12 10:48:48.299   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.303   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.306   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.309  1168  1310 I chatty  : uid=1027(nfc) com.android.nfc expire 3 lines

> 03-12 10:48:48.310   377   919 I OMXMaster: makeComponentInstance(OMX.google.vorbis.decoder) in mediacodec process

> 03-12 10:48:48.311   766   766 D BluetoothAdapterService: getAdapterService() - returning com.android.bluetooth.btservice.AdapterService@cdabc0c

> 03-12 10:48:48.312   766  1300 D A2dpStateMachine: Enter Disconnected: -2

> 03-12 10:48:48.319   766   766 I BluetoothHidServiceJni: classInitNative: succeeds

> 03-12 10:48:48.321   779   779 D BluetoothInputDevice: Proxy object connected

> 03-12 10:48:48.321   779   779 D HidProfile: Bluetooth service connected

> 03-12 10:48:48.321   766   766 D BluetoothAdapterService: getAdapterService() - returning com.android.bluetooth.btservice.AdapterService@cdabc0c

> 03-12 10:48:48.322   766   766 D HidService: Received start request. Starting profile…

> 03-12 10:48:48.323   766   766 I bt_btif : get_profile_interface hidhost

> 03-12 10:48:48.323   766   766 D HidService: setHidService(): set to: null

> 03-12 10:48:48.323   766   944 I bt_bta_hh: BTA_HhEnable sec_mask:0x36 p_cback:0xd4a82b19

> 03-12 10:48:48.323   766   944 I bt_btif_storage: btif_storage_get_adapter_property service_mask:0x20100008

> 03-12 10:48:48.323   766   766 D BluetoothAdapterService: getAdapterService() - returning com.android.bluetooth.btservice.AdapterService@cdabc0c

> 03-12 10:48:48.324   766   766 I BluetoothPanServiceJni: classInitNative(L102): succeeds

> 03-12 10:48:48.327   779   779 D BluetoothPan: BluetoothPAN Proxy object connected

> 03-12 10:48:48.327   779   779 D PanProfile: Bluetooth service connected

> 03-12 10:48:48.327   766   766 D BluetoothAdapterService: getAdapterService() - returning com.android.bluetooth.btservice.AdapterService@cdabc0c

> 03-12 10:48:48.328   766   766 D PanService: Received start request. Starting profile…

> 03-12 10:48:48.328   766   766 D BluetoothPanServiceJni: initializeNative(L107): pan

> 03-12 10:48:48.328   766   766 I bt_btif : get_profile_interface pan

> 03-12 10:48:48.328   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.329   766   766 D BluetoothAdapterService: getAdapterService() - returning com.android.bluetooth.btservice.AdapterService@cdabc0c

> 03-12 10:48:48.329   766   766 D BluetoothAdapterService: handleMessage() - Message: 1

> 03-12 10:48:48.329   766   766 D BluetoothAdapterService: handleMessage() - MESSAGE_PROFILE_SERVICE_STATE_CHANGED

> 03-12 10:48:48.329   766   766 D BluetoothAdapterService: processProfileServiceStateChanged() serviceName=com.android.bluetooth.a2dp.A2dpService, state=12, doUpdate=true

> 03-12 10:48:48.329   766   766 V BluetoothAdapterState: isTurningOff()=false

> 03-12 10:48:48.329   766   766 V BluetoothAdapterState: isTurningOn()=true

> 03-12 10:48:48.329   766   766 V BluetoothAdapterState: isBleTurningOn()=false

> 03-12 10:48:48.329   766   766 V BluetoothAdapterState: isBleTurningOff()=false

> 03-12 10:48:48.329   766   766 D BluetoothAdapterService: processProfileServiceStateChanged() - serviceName=com.android.bluetooth.a2dp.A2dpService isTurningOn=true isTurningOff=false isBleTurningOn=false isBleTurningOff=false

> 03-12 10:48:48.329   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.329   766   944 D BluetoothPanServiceJni: control_state_callback(L61): state:0, local_role:3, ifname:bt-pan

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.gatt.GattService

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: Skip GATT service - already started before

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.hid.HidService

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: onProfileServiceStateChange() - Profile still not running:com.android.bluetooth.hid.HidService

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: handleMessage() - Message: 1

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: handleMessage() - MESSAGE_PROFILE_SERVICE_STATE_CHANGED

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: processProfileServiceStateChanged() serviceName=com.android.bluetooth.hid.HidService, state=12, doUpdate=true

> 03-12 10:48:48.330   766   766 V BluetoothAdapterState: isTurningOff()=false

> 03-12 10:48:48.330   766   766 V BluetoothAdapterState: isTurningOn()=true

> 03-12 10:48:48.330   766   766 V BluetoothAdapterState: isBleTurningOn()=false

> 03-12 10:48:48.330   766   766 V BluetoothAdapterState: isBleTurningOff()=false

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: processProfileServiceStateChanged() - serviceName=com.android.bluetooth.hid.HidService isTurningOn=true isTurningOff=false isBleTurningOn=false isBleTurningOff=false

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.gatt.GattService

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: Skip GATT service - already started before

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.hid.HidService

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.pan.PanService

> 03-12 10:48:48.330   766   766 D BluetoothAdapterService: onProfileServiceStateChange() - Profile still not running:com.android.bluetooth.pan.PanService

> 03-12 10:48:48.330   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.331   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.332   766   766 D BluetoothAdapterService: handleMessage() - Message: 1

> 03-12 10:48:48.332   766   766 D BluetoothAdapterService: handleMessage() - MESSAGE_PROFILE_SERVICE_STATE_CHANGED

> 03-12 10:48:48.332   766   766 D BluetoothAdapterService: processProfileServiceStateChanged() serviceName=com.android.bluetooth.pan.PanService, state=12, doUpdate=true

> 03-12 10:48:48.332   766   766 V BluetoothAdapterState: isTurningOff()=false

> 03-12 10:48:48.332   766   766 V BluetoothAdapterState: isTurningOn()=true

> 03-12 10:48:48.332   766   766 V BluetoothAdapterState: isBleTurningOn()=false

> 03-12 10:48:48.332   766   766 V BluetoothAdapterState: isBleTurningOff()=false

> 03-12 10:48:48.333   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.333   766   766 D BluetoothAdapterService: processProfileServiceStateChanged() - serviceName=com.android.bluetooth.pan.PanService isTurningOn=true isTurningOff=false isBleTurningOn=false isBleTurningOff=false

> 03-12 10:48:48.333   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.gatt.GattService

> 03-12 10:48:48.333   766   766 D BluetoothAdapterService: Skip GATT service - already started before

> 03-12 10:48:48.333   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.hid.HidService

> 03-12 10:48:48.333   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.pan.PanService

> 03-12 10:48:48.333   766   766 D BluetoothAdapterService: Service: com.android.bluetooth.a2dp.A2dpService

> 03-12 10:48:48.333   766   766 D BluetoothAdapterService: onProfileServiceStateChange() - All profile services started.

> 03-12 10:48:48.333   766   934 D BluetoothAdapterState: Current state: PENDING_COMMAND, message: 2

> 03-12 10:48:48.333   766   934 D BluetoothAdapterProperties: ScanMode =  20

> 03-12 10:48:48.333   766   934 D BluetoothAdapterProperties: State =  11

> 03-12 10:48:48.333   766   934 D BluetoothAdapterProperties: Setting state to 12

> 03-12 10:48:48.333   766   934 I BluetoothAdapterState: Bluetooth adapter state changed: 11-> 12

> 03-12 10:48:48.334   766   934 D BluetoothAdapterService: updateAdapterState() - Broadcasting state to 1 receivers.

> 03-12 10:48:48.334   634   654 D BluetoothManagerService: MESSAGE_BLUETOOTH_STATE_CHANGE: TURNING_ON > ON

> 03-12 10:48:48.334   634   654 D BluetoothManagerService: Broadcasting onBluetoothStateChange(true) to 6 receivers.

> 03-12 10:48:48.334   779   793 D BluetoothPan: onBluetoothStateChange on: true

> 03-12 10:48:48.335   634   654 D BluetoothHeadset: onBluetoothStateChange: up=true

> 03-12 10:48:48.335   634   654 D BluetoothManagerService: Creating new ProfileServiceConnections object for profile: 1

> 03-12 10:48:48.335   634   654 E BluetoothManagerService: Fail to bind to: Intent { act=android.bluetooth.IBluetoothHeadset }

> 03-12 10:48:48.335   634   654 W BluetoothManagerService: Unable to bind with intent: Intent { act=android.bluetooth.IBluetoothHeadset }

> 03-12 10:48:48.335   779   793 D BluetoothInputDevice: onBluetoothStateChange: up=true

> 03-12 10:48:48.335   766   944 D BluetoothAdapterProperties: Scan Mode:21

> 03-12 10:48:48.335   766   944 D BluetoothAdapterProperties: Discoverable Timeout:120

> 03-12 10:48:48.335   766   934 I BluetoothAdapterState: Entering OnState

> 03-12 10:48:48.335   766   934 D BluetoothAdapterService: updateUuids() - Updating UUIDs for bonded devices

> 03-12 10:48:48.336   779   793 D BluetoothPbap: onBluetoothStateChange: up=true

> 03-12 10:48:48.336   779   793 E BluetoothPbap: Could not bind to Bluetooth Pbap Service with Intent { act=android.bluetooth.IBluetoothPbap }

> 03-12 10:48:48.336   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.336   634   654 D BluetoothA2dp: onBluetoothStateChange: up=true

> 03-12 10:48:48.337   779   793 D BluetoothMap: onBluetoothStateChange: up=true

> 03-12 10:48:48.337   779   793 E BluetoothMap: Could not bind to Bluetooth MAP Service with Intent { act=android.bluetooth.IBluetoothMap }

> 03-12 10:48:48.337   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.337   766   934 D BluetoothAdapterService: isQuetModeEnabled() - Enabled = false

> 03-12 10:48:48.337   766   934 D BluetoothAdapterService: autoConnect() - Initiate auto connection on BT on…

> 03-12 10:48:48.338   634   654 D BluetoothManagerService: Sending BLE State Change: TURNING_ON > ON

> 03-12 10:48:48.338   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.341   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.341   766   934 D A2dpSinkService: getA2dpSinkService(): service is NULL

> 03-12 10:48:48.344   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.347   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.348  1168  1287 I chatty  : uid=1027(nfc) AsyncTask \#1 expire 65 lines

> 03-12 10:48:48.348  1168  1292 I chatty  : uid=1027(nfc) AsyncTask \#1 expire 12 lines

> 03-12 10:48:48.349  1168  1294 I chatty  : uid=1027(nfc) AsyncTask \#1 expire 49 lines

> 03-12 10:48:48.350   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.351   779  1081 D LocalBluetoothProfileManager: Adding local A2DP SRC profile

> 03-12 10:48:48.353   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.354   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.355   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:48.356   779   779 D BluetoothA2dp: Proxy object connected

> 03-12 10:48:48.356   779  1081 D LocalBluetoothProfileManager: Handsfree Uuid not found.

> 03-12 10:48:48.357  1168  1291 I chatty  : uid=1027(nfc) AsyncTask \#1 expire 114 lines

> 03-12 10:48:48.367  1168  1283 I chatty  : uid=1027(nfc) com.android.nfc expire 5 lines

> 03-12 10:48:48.388   369   901 I chatty  : uid=1013(media) /system/bin/mediaserver expire 1 line

> 03-12 10:48:48.390  1168  1307 I chatty  : uid=1027(nfc) SoundPoolThread expire 2 lines

> 03-12 10:48:48.391  1168  1316 I chatty  : uid=1027(nfc) com.android.nfc expire 3 lines

> 03-12 10:48:48.392   377  1127 I OMXMaster: makeComponentInstance(OMX.google.vorbis.decoder) in mediacodec process

> 03-12 10:48:48.428  1168  1320 I chatty  : uid=1027(nfc) com.android.nfc expire 3 lines

> 03-12 10:48:48.428   377  1095 I OMXMaster: makeComponentInstance(OMX.google.vorbis.decoder) in mediacodec process

> 03-12 10:48:48.711   634   760 D EthernetNetworkFactory: eth0 carrier = 0

> 03-12 10:48:49.070   372   407 E AudioFlinger: no wake lock to update, but system ready!

> 03-12 10:48:49.441  1194  1194 D RecoveryReceiver: RECOVERY_STATE_FILE_TF:/mnt/external_sd/Recovery_state

> 03-12 10:48:49.465   634   645 I ActivityManager: Start proc 1329:android.rockchip.update.service/1000 for broadcast android.rockchip.update.service/.RKUpdateReceiver

> 03-12 10:48:49.470  1329  1329 E art     : setrlimit(RLIMIT_CORE) failed for pid 1329: Operation not permitted

> 03-12 10:48:49.530  1329  1329 D RKUpdateReceiver: action = android.intent.action.MEDIA_MOUNTED

> 03-12 10:48:49.547   634  1098 I ActivityManager: Start proc 1344:android.process.media/u0a5 for broadcast com.android.providers.media/.MediaScannerReceiver

> 03-12 10:48:49.556  1344  1344 I chatty  : uid=10005(com.android.providers.media) android.process.media expire 6 lines

> 03-12 10:48:50.014   634  1098 I ActivityManager: Start proc 1364:com.android.managedprovisioning/u0a4 for broadcast com.android.managedprovisioning/.PreBootListener

> 03-12 10:48:50.015  1364  1364 E art     : setrlimit(RLIMIT_CORE) failed for pid 1364: Operation not permitted

> 03-12 10:48:50.072  1364  1364 W System  : ClassLoader referenced unknown path: /system/priv-app/ManagedProvisioning/lib/arm64

> 03-12 10:48:50.151   634  1098 I PreBootBroadcaster: Pre-boot of {com.android.providers.media/com.android.providers.media.MediaUpgradeReceiver} for user 0

> 03-12 10:48:50.156  1180  1180 D IDPad::broadcast::NfcAdapterReceiver: onReceive:enable adapter = 2(NfcAdapterReceiver.java:24)

> 03-12 10:48:50.178   634  1098 I ActivityManager: Start proc 1381:com.android.settings/1000 for broadcast com.android.settings/.bluetooth.DockEventReceiver

> 03-12 10:48:50.181  1381  1381 E art     : setrlimit(RLIMIT_CORE) failed for pid 1381: Operation not permitted

> 03-12 10:48:50.217  1381  1381 W System  : ClassLoader referenced unknown path: /system/priv-app/Settings/lib/arm64

> 03-12 10:48:50.245  1381  1381 D AndroidRuntime: Shutting down VM

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: FATAL EXCEPTION: main

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: Process: com.android.settings, PID: 1381

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: java.lang.RuntimeException: Unable to instantiate application com.android.settings.SettingsApplication: java.lang.ClassNotFoundException: Didn’t find class “com.android.settings.SettingsApplication” on path: DexPathList[[zip file “/system/priv-app/Settings/Settings.apk”],nativeLibraryDirectories=[/system/priv-app/Settings/lib/arm64, /system/priv-app/Settings/Settings.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.app.LoadedApk.makeApplication(LoadedApk.java:802)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.app.ActivityThread.handleBindApplication(ActivityThread.java:5379)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.app.ActivityThread.-wrap2(ActivityThread.java)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.app.ActivityThread
$$
H.handleMessage(ActivityThread.java:1546)
03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.os.Handler.dispatchMessage(Handler.java:102)
03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.os.Looper.loop(Looper.java:154)
03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.app.ActivityThread.main(ActivityThread.java:6121)
03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at java.lang.reflect.Method.invoke(Native Method)
03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at com.android.internal.os.ZygoteInit
$$
> MethodAndArgsCaller.run(ZygoteInit.java:912)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:802)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: Caused by: java.lang.ClassNotFoundException: Didn’t find class “com.android.settings.SettingsApplication” on path: DexPathList[[zip file “/system/priv-app/Settings/Settings.apk”],nativeLibraryDirectories=[/system/priv-app/Settings/lib/arm64, /system/priv-app/Settings/Settings.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at dalvik.system.BaseDexClassLoader.findClass(BaseDexClassLoader.java:56)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at java.lang.ClassLoader.loadClass(ClassLoader.java:380)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at java.lang.ClassLoader.loadClass(ClassLoader.java:312)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.app.Instrumentation.newApplication(Instrumentation.java:992)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	at android.app.LoadedApk.makeApplication(LoadedApk.java:796)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	… 9 more

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 	Suppressed: java.io.IOException: No original dex files found for dex location /system/priv-app/Settings/Settings.apk

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexFile.openDexFileNative(Native Method)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexFile.openDexFile(DexFile.java:367)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexFile.(DexFile.java:112)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexFile.(DexFile.java:77)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexPathList.loadDexFile(DexPathList.java:359)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexPathList.makeElements(DexPathList.java:323)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexPathList.makeDexElements(DexPathList.java:263)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.DexPathList.(DexPathList.java:126)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.BaseDexClassLoader.(BaseDexClassLoader.java:48)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at dalvik.system.PathClassLoader.(PathClassLoader.java:64)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at com.android.internal.os.PathClassLoaderFactory.createClassLoader(PathClassLoaderFactory.java:43)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.ApplicationLoaders.getClassLoader(ApplicationLoaders.java:58)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.LoadedApk.createOrUpdateClassLoaderLocked(LoadedApk.java:520)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.LoadedApk.getClassLoader(LoadedApk.java:553)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.ActivityThread.getTopLevelResources(ActivityThread.java:1867)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.LoadedApk.getResources(LoadedApk.java:766)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.ContextImpl.(ContextImpl.java:2038)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.ContextImpl.createAppContext(ContextImpl.java:1983)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		at android.app.ActivityThread.handleBindApplication(ActivityThread.java:5296)

> 03-12 10:48:50.247  1381  1381 E AndroidRuntime: 		… 8 more

> 03-12 10:48:50.251   634  1098 W ActivityManager: Process com.android.settings has crashed too many times: killing!

> 03-12 10:48:50.251   634  1098 I ActivityManager: Killing 1381:com.android.settings/1000 (adj 0): crash

> 03-12 10:48:50.252   634  1098 D ActivityManager: cleanUpApplicationRecord – 1381

> 03-12 10:48:50.257  1180  1180 D IDPad::broadcast::NfcAdapterReceiver: onReceive:enable adapter = 3(NfcAdapterReceiver.java:24)

> 03-12 10:48:50.260   634  1098 W BroadcastQueue: Unable to launch app com.android.settings/1000 for broadcast Intent { act=android.bluetooth.adapter.action.STATE_CHANGED flg=0x4000010 (has extras) }: process is bad

> 03-12 10:48:50.299   766  1404 W BluetoothAdapter: getBluetoothService() called with no BluetoothManagerCallback

> 03-12 10:48:50.301   766  1404 I BtOppRfcommListener: Accept thread started.

> 03-12 10:48:50.301   766   965 E bt_upio : upio_set : open(/proc/bluetooth/sleep/btwrite) for write failed: Permission denied (13)

> 03-12 10:48:50.307  1344  1344 I MediaUpgradeReceiver: —> Start upgrade of media database external.db

> 03-12 10:48:50.337  1344  1344 I MediaUpgradeReceiver: <— Finished upgrade of media database external.db in 30ms

> 03-12 10:48:50.338   634  1121 I PreBootBroadcaster: Pre-boot of {com.android.settings/com.android.settings.SettingsInitialize} for user 0

> 03-12 10:48:50.339   634  1121 W BroadcastQueue: Unable to launch app com.android.settings/1000 for broadcast Intent { act=android.intent.action.PRE_BOOT_COMPLETED flg=0x2000110 cmp=com.android.settings/.SettingsInitialize }: process is bad

> 03-12 10:48:50.340   634   648 D ActivityManager: Sending BOOT_COMPLETE user \#0

> 03-12 10:48:50.344   634   634 W TelecomManager: Telecom Service not found.

> 03-12 10:48:50.382  1344  1378 I chatty  : uid=10005(com.android.providers.media) android.process.media expire 2 lines

> 03-12 10:48:50.386   634   634 W BroadcastQueue: Unable to launch app com.android.settings/1000 for broadcast Intent { act=android.intent.action.BOOT_COMPLETED flg=0x9000010 (has extras) }: process is bad

> 03-12 10:48:50.389   634  1408 E BootReceiver: Error reading /data/system/last-header.txt

> 03-12 10:48:50.389   634  1408 E BootReceiver: java.io.FileNotFoundException: /data/system/last-header.txt (No such file or directory)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at java.io.FileInputStream.open(Native Method)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at java.io.FileInputStream.(FileInputStream.java:146)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at android.os.FileUtils.readTextFile(FileUtils.java:263)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at com.android.server.BootReceiver.getPreviousBootHeaders(BootReceiver.java:121)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at com.android.server.BootReceiver.getBootHeadersToLogAndUpdate(BootReceiver.java:143)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at com.android.server.BootReceiver.logBootEvents(BootReceiver.java:163)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at com.android.server.BootReceiver.-wrap2(BootReceiver.java)

> 03-12 10:48:50.389   634  1408 E BootReceiver: 	at com.android.server.BootReceiver$1.run(BootReceiver.java:93)

> 03-12 10:48:50.391   634   958 D xzj     : ----keygurad drawn done,exit bootanim----

> 03-12 10:48:50.395   634  1408 I RecoverySystem: Deleted: /cache/recovery/log

> 03-12 10:48:50.398  1194  1194 D StressTest: =======
> onReceive
> =rebootFlag:0

> 03-12 10:48:50.402  1329  1329 D RKUpdateReceiver: action = android.intent.action.BOOT_COMPLETED

> 03-12 10:48:50.402  1329  1329 D RKUpdateReceiver: RKUpdateReceiver recv ACTION_BOOT_COMPLETED.

> 03-12 10:48:50.403   634  1408 I BootReceiver: No existing last log timestamp file /data/system/log-files.xml; starting empty

> 03-12 10:48:50.405  1329  1329 W ContextImpl: Calling a method in the system process without a qualified user: android.app.ContextImpl.startService:1357 android.content.ContextWrapper.startService:613 android.content.ContextWrapper.startService:613 android.rockchip.update.service.RKUpdateReceiver.onReceive:59 android.app.ActivityThread.handleReceiver:3041

> 03-12 10:48:50.407   634  1408 I BootReceiver: Copying /cache/recovery/last_kmsg to DropBox (SYSTEM_RECOVERY_KMSG)

> 03-12 10:48:50.410  1329  1329 W ContextImpl: Calling a method in the system process without a qualified user: android.app.ContextImpl.startService:1357 android.content.ContextWrapper.startService:613 android.content.ContextWrapper.startService:613 android.rockchip.update.service.RKUpdateReceiver.onReceive:65 android.app.ActivityThread.handleReceiver:3041

> 03-12 10:48:50.428   634   827 I ActivityManager: Start proc 1410:com.android.camera2/u0a16 for broadcast com.android.camera2/com.android.camera.DisableCameraReceiver

> 03-12 10:48:50.428  1329  1329 I android_rockchip_update_UpdateService.cpp: JNI_OnLoad

> 03-12 10:48:50.429   779   779 D PhoneStatusBar: disable: < expand* icons alerts system_info back* home* recent* clock search* quick_settings >

> 03-12 10:48:50.430  1410  1410 E art     : setrlimit(RLIMIT_CORE) failed for pid 1410: Operation not permitted

> 03-12 10:48:50.435  1329  1329 D RKUpdateService: starting RKUpdateService, version is 1.8.0

> 03-12 10:48:50.436  1329  1329 D RKUpdateService: remote uri is
> [http://192.168.1.143:2300/OtaUpdater/android?product=IDP110&version=1.0.0&sn=IDP110&country=US&language=en](http://192.168.1.143:2300/OtaUpdater/android?product=IDP110&version=1.0.0&sn=IDP110&country=US&language=en)

> 03-12 10:48:50.436  1329  1329 D RKUpdateService: remote uri backup is
> [http://192.168.1.143:2300/OtaUpdater/android?product=IDP110&version=1.0.0&sn=IDP110&country=US&language=en](http://192.168.1.143:2300/OtaUpdater/android?product=IDP110&version=1.0.0&sn=IDP110&country=US&language=en)

> 03-12 10:48:50.437   634  1408 I BootReceiver: Copying audit failures to DropBox

> 03-12 10:48:50.437   634  1408 I BootReceiver: Checking for fsck errors

> 03-12 10:48:50.444   634  1408 E ActivityThread: Failed to find provider info for downloads

> 03-12 10:48:50.444   634  1408 E BootReceiver: Can’t remove old update packages

> 03-12 10:48:50.444   634  1408 E BootReceiver: java.lang.IllegalArgumentException: Unknown URL content://downloads/my_downloads

> 03-12 10:48:50.444   634  1408 E BootReceiver: 	at android.content.ContentResolver.delete(ContentResolver.java:1369)

> 03-12 10:48:50.444   634  1408 E BootReceiver: 	at android.provider.Downloads.removeAllDownloadsByPackage(Downloads.java:812)

> 03-12 10:48:50.444   634  1408 E BootReceiver: 	at com.android.server.BootReceiver.removeOldUpdatePackages(BootReceiver.java:116)

> 03-12 10:48:50.444   634  1408 E BootReceiver: 	at com.android.server.BootReceiver.-wrap3(BootReceiver.java)

> 03-12 10:48:50.444   634  1408 E BootReceiver: 	at com.android.server.BootReceiver$1.run(BootReceiver.java:105)

> 03-12 10:48:50.446  1329  1329 D RKUpdateService: first startup!!!

> 03-12 10:48:50.461  1329  1329 D RKUpdateService: onStartCommand…

> 03-12 10:48:50.461  1329  1329 D RKUpdateService: command = 1 delaytime = 20000

> 03-12 10:48:50.462  1329  1329 D RKUpdateService: onStartCommand…

> 03-12 10:48:50.462  1329  1329 D RKUpdateService: command = 2 delaytime = 25000

> 03-12 10:48:50.475   272   272 D GRALLOC-ROCKCHIP: enter, w : 1280, h : 800, format : 0x1, usage : 0x1a00.

> 03-12 10:48:50.533  1410  1410 V CAM_Profiler: [  0.000ms][ui] GUARD: CameraApp onCreate() - START

> 03-12 10:48:50.533  1410  1410 V CAM_Profiler: [  0.000ms][ui] GUARD: FirstRunDetector getTimeOfFirstRun - START

> 03-12 10:48:50.535  1410  1410 V CAM_Profiler: [  1.261ms][ui] GUARD: FirstRunDetector getTimeOfFirstRun - [ 1.261ms] PreferenceManager.getDefaultSharedPreferences

> 03-12 10:48:50.536  1410  1410 V CAM_Profiler: [  2.506ms][ui] GUARD: FirstRunDetector getTimeOfFirstRun - [ 1.245ms] preferences.getLong

> 03-12 10:48:50.537  1410  1410 V CAM_Profiler: [  3.389ms][ui] GUARD: FirstRunDetector getTimeOfFirstRun - [ 0.883ms] getSharedPreferences

> 03-12 10:48:50.539  1410  1410 V CAM_Profiler: [  5.288ms][ui] GUARD: FirstRunDetector getTimeOfFirstRun - [ 1.899ms] preferences.edit()

> 03-12 10:48:50.539  1410  1410 V CAM_Profiler: [  5.698ms][ui] GUARD: FirstRunDetector getTimeOfFirstRun - STOP

> 03-12 10:48:50.539  1410  1410 V CAM_Profiler: [  8.114ms][ui] GUARD: CameraApp onCreate() - [ 8.114ms] initializeTimeOfFirstRun

> 03-12 10:48:50.540  1410  1410 V CAM_Profiler: [  8.560ms][ui] GUARD: CameraApp onCreate() - [ 0.445ms] UsageStatistics.initialize

> 03-12 10:48:50.543  1410  1410 V CAM_Profiler: [ 11.848ms][ui] GUARD: CameraApp onCreate() - [ 3.289ms] clearNotifications

> 03-12 10:48:50.543  1410  1410 V CAM_Profiler: [ 11.848ms][ui] GUARD: CameraApp onCreate() - STOP

> 03-12 10:48:50.557   809   809 I SystemBroadcastReceiver: Boot has been completed

> 03-12 10:48:50.557   809   809 I SystemBroadcastReceiver: toggleAppIcon() : FLAG_SYSTEM = true

> 03-12 10:48:50.595   634  1121 I ActivityManager: Start proc 1430:com.android.onetimeinitializer/u0a6 for broadcast com.android.onetimeinitializer/.OneTimeInitializerReceiver

> 03-12 10:48:50.603  1430  1430 E art     : setrlimit(RLIMIT_CORE) failed for pid 1430: Operation not permitted

> 03-12 10:48:50.648  1430  1430 W System  : ClassLoader referenced unknown path: /system/priv-app/OneTimeInitializer/lib/arm64

> 03-12 10:48:50.676  1430  1430 V OneTimeInitializerRece: OneTimeInitializerReceiver.onReceive

> 03-12 10:48:50.710   634   653 D EthernetNetworkFactory: updateInterface: eth0 link up

> 03-12 10:48:50.712   634   759 D EthernetNetworkFactory: onRequestNetwork: eth0 carrier = 1

> 03-12 10:48:50.712   634   759 D EthernetNetworkFactory: sendEthernetStateChangedBroadcast: curState = CONNECTING

> 03-12 10:48:50.718   634  1448 D EthernetNetworkFactory: starting ipProvisioningThread(eth0): mNetworkInfo=[type: Ethernet[], state: DISCONNECTED/DISCONNECTED, reason: (unspecified), extra: a2:62:c0:2d:b4:5c, failover: false, available: true, roaming: false, metered: false]

> 03-12 10:48:50.719   634  1448 D EthernetNetworkFactory: config DHCP

> 03-12 10:48:50.722  1430  1446 I OneTimeInitializerServ: Updating to version 1.

> 03-12 10:48:50.727   381   633 D CommandListener: Clearing all IP addresses on eth0

> 03-12 10:48:50.727  1430  1446 E ActivityThread: Failed to find provider info for com.android.launcher2.settings

> 03-12 10:48:50.729   634  1449 D IpManager.eth0: newLp{{InterfaceName: eth0 LinkAddresses: []  Routes: [] DnsAddresses: [] Domains: null MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}

> 03-12 10:48:50.732   634  1449 D IpManager.eth0: CMD_UPDATE_TCP_BUFFER_SIZES eth0/2 0 0 524288,1048576,3145728,524288,1048576,2097152 [rcvd_in=StoppedState, proc_in=StoppedState]

> 03-12 10:48:50.733   634  1449 D IpManager.eth0: CMD_START eth0/2 0 0 ProvisioningConfiguration{mEnableIPv4: true, mEnableIPv6: true, mUsingIpReachabilityMonitor: true, mRequestedPreDhcpActionMs: 0, mStaticIpConfig: null, mApfCapabilities: null, mProvisioningTimeoutMs: 0} [rcvd_in=StoppedState, proc_in=StoppedState]

> 03-12 10:48:50.745   634   644 I art     : Background sticky concurrent mark sweep GC freed 10124(1034KB) AllocSpace objects, 24(1712KB) LOS objects, 30% free, 6MB/9MB, paused 5.313ms total 32.787ms

> 03-12 10:48:50.754   634  1449 D IpManager.eth0: newLp{{InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,]  Routes: [fe80::/64 -> :: eth0,] DnsAddresses: [] Domains: null MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}

> 03-12 10:48:50.755   634  1449 D IpManager.eth0: onLinkPropertiesChange()

> 03-12 10:48:50.755   634  1449 D EthernetNetworkFactory: onLinkPropertiesChange: lp = {InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,]  Routes: [fe80::/64 -> :: eth0,] DnsAddresses: [] Domains: null MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}

> 03-12 10:48:50.757   634  1449 D IpManager.eth0: newLp{{InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,]  Routes: [fe80::/64 -> :: eth0,] DnsAddresses: [] Domains: null MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}

> 03-12 10:48:50.771   634  1456 D DhcpClient: Receive thread started

> 03-12 10:48:50.772   634  1449 D IpManager.eth0: CMD_PRE_DHCP_ACTION eth0/2 0 0 null [rcvd_in=RunningState, proc_in=RunningState]

> 03-12 10:48:50.772   634  1449 D IpManager.eth0: EVENT_PRE_DHCP_ACTION_COMPLETE eth0/2 0 0 null [rcvd_in=RunningState, proc_in=RunningState]

> 03-12 10:48:50.775   634  1454 D DhcpClient: Broadcasting DHCPDISCOVER

> 03-12 10:48:50.853   634  1456 D DhcpClient: Received packet: a2:62:c0:2d:b4:5c OFFER, ip /192.168.10.80, mask /255.255.255.0, DNS servers: /192.168.10.1 , gateways [/192.168.10.1] lease time 7200, domain lan

> 03-12 10:48:50.855   634  1454 D DhcpClient: Got pending lease: IP address 192.168.10.80/24 Gateway 192.168.10.1  DNS servers: [ 192.168.10.1 ] Domains lan DHCP server /192.168.10.1 Vendor info null lease 7200 seconds

> 03-12 10:48:50.859   634  1454 D DhcpClient: Broadcasting DHCPREQUEST ciaddr=0.0.0.0 request=192.168.10.80 serverid=192.168.10.1

> 03-12 10:48:50.870   634  1456 D DhcpClient: Received packet: a2:62:c0:2d:b4:5c ACK: your new IP /192.168.10.80, netmask /255.255.255.0, gateways [/192.168.10.1] DNS servers: /192.168.10.1 , lease time 7200

> 03-12 10:48:50.871   634  1454 D DhcpClient: Confirmed lease: IP address 192.168.10.80/24 Gateway 192.168.10.1  DNS servers: [ 192.168.10.1 ] Domains lan DHCP server /192.168.10.1 Vendor info null lease 7200 seconds

> 03-12 10:48:50.874   634  1449 D IpManager.eth0: newLp{{InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,]  Routes: [fe80::/64 -> :: eth0,192.168.10.0/24 -> 0.0.0.0 eth0,0.0.0.0/0 -> 192.168.10.1 eth0,] DnsAddresses: [] Domains: lan MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}

> 03-12 10:48:50.874   634  1449 D IpManager.eth0: onNewDhcpResults(IP address 192.168.10.80/24 Gateway 192.168.10.1  DNS servers: [ 192.168.10.1 ] Domains lan DHCP server /192.168.10.1 Vendor info null lease 7200 seconds)

> 03-12 10:48:50.875   634  1449 D IpManager.eth0: onLinkPropertiesChange()

> 03-12 10:48:50.876   634  1449 D EthernetNetworkFactory: onLinkPropertiesChange: lp = {InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,]  Routes: [fe80::/64 -> :: eth0,192.168.10.0/24 -> 0.0.0.0 eth0,0.0.0.0/0 -> 192.168.10.1 eth0,] DnsAddresses: [] Domains: lan MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}

> 03-12 10:48:50.878   634  1449 D IpManager.eth0: CMD_POST_DHCP_ACTION eth0/2 1 0 IP address 192.168.10.80/24 Gateway 192.168.10.1  DNS servers: [ 192.168.10.1 ] Domains lan DHCP server /192.168.10.1 Vendor info null lease 7200 seconds [rcvd_in=RunningState, proc_in=RunningState]

> 03-12 10:48:50.879   381   633 D CommandListener: Setting iface cfg

> 03-12 10:48:50.884   634  1449 D IpManager.eth0: IPv4 configuration succeeded

> 03-12 10:48:50.890   634  1454 D DhcpClient: Scheduling renewal in 3599s

> 03-12 10:48:50.890   634  1454 D DhcpClient: Scheduling rebind in 6299s

> 03-12 10:48:50.890   634  1454 D DhcpClient: Scheduling expiry in 7199s

> 03-12 10:48:50.891   634  1449 D IpManager.eth0: CMD_CONFIGURE_LINKADDRESS eth0/2 0 0 192.168.10.80/24 [rcvd_in=RunningState, proc_in=RunningState]

> 03-12 10:48:50.892   634  1449 D IpManager.eth0: newLp{{InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,192.168.10.80/24,]  Routes: [fe80::/64 -> :: eth0,192.168.10.0/24 -> 0.0.0.0 eth0,0.0.0.0/0 -> 192.168.10.1 eth0,] DnsAddresses: [192.168.10.1,] Domains: lan MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}

> 03-12 10:48:50.892   634  1449 D IpManager.eth0: onProvisioningSuccess()

> 03-12 10:48:50.895   634  1448 D EthernetNetworkFactory: success get ip: lp = {InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,192.168.10.80/24,]  Routes: [fe80::/64 -> :: eth0,192.168.10.0/24 -> 0.0.0.0 eth0,0.0.0.0/0 -> 192.168.10.1 eth0,] DnsAddresses: [192.168.10.1,] Domains: lan MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}

> 03-12 10:48:50.895   634  1448 D EthernetNetworkFactory: sendEthernetStateChangedBroadcast: curState = CONNECTED

> 03-12 10:48:50.903   634  1448 D ConnectivityService: registerNetworkAgent NetworkAgentInfo{ ni{[type: Ethernet[], state: CONNECTED/CONNECTED, reason: (unspecified), extra: a2:62:c0:2d:b4:5c, failover: false, available: true, roaming: false, metered: false]}  network{100}  nethandle{429513165534}  lp{{InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,192.168.10.80/24,]  Routes: [fe80::/64 -> :: eth0,192.168.10.0/24 -> 0.0.0.0 eth0,0.0.0.0/0 -> 192.168.10.1 eth0,] DnsAddresses: [192.168.10.1,] Domains: lan MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}  nc{[ Transports: ETHERNET Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN LinkUpBandwidth>=100000Kbps LinkDnBandwidth>=100000Kbps]}  Score{30}  everValidated{false}  lastValidated{false}  created{false} lingering{false} explicitlySelected{false} acceptUnvalidated{false} everCaptivePortalDetected{false} lastCaptivePortalDetected{false} }

> 03-12 10:48:50.904   634   724 D ConnectivityService: Got NetworkAgent Messenger

> 03-12 10:48:50.904   634  1448 D EthernetNetworkFactory: exiting ipProvisioningThread(eth0): mNetworkInfo=[type: Ethernet[], state: CONNECTED/CONNECTED, reason: (unspecified), extra: a2:62:c0:2d:b4:5c, failover: false, available: true, roaming: false, metered: false]

> 03-12 10:48:50.904   634   724 D ConnectivityService: NetworkAgentInfo [Ethernet () - 100] EVENT_NETWORK_INFO_CHANGED, going from null to CONNECTED

> 03-12 10:48:50.907   634   724 D ConnectivityService: Adding iface eth0 to network 100

> 03-12 10:48:50.940  1344  1451 I chatty  : uid=10005(com.android.providers.media) android.process.media expire 82 lines

> 03-12 10:48:50.956   634   724 D ConnectivityService: Adding Route [fe80::/64 -> :: eth0] to network 100

> 03-12 10:48:50.958   634   724 D ConnectivityService: Adding Route [192.168.10.0/24 -> 0.0.0.0 eth0] to network 100

> 03-12 10:48:50.960   634   724 D ConnectivityService: Adding Route [0.0.0.0/0 -> 192.168.10.1 eth0] to network 100

> 03-12 10:48:50.961   634   724 D ConnectivityService: Setting DNS servers for network 100 to [/192.168.10.1]

> 03-12 10:48:50.972   634   724 D ConnectivityService: notifyType IP_CHANGED for NetworkAgentInfo [Ethernet () - 100]

> 03-12 10:48:50.977   634   724 D ConnectivityService: scheduleUnvalidatedPrompt 100

> 03-12 10:48:50.977   634   724 D ConnectivityService: updateSignalStrengthThresholds: CONNECT, sending [] to NetworkAgentInfo [Ethernet () - 100]

> 03-12 10:48:50.978   634   724 D ConnectivityService: rematching NetworkAgentInfo [Ethernet () - 100]

> 03-12 10:48:50.978   634   724 D ConnectivityService:  network has: [ Transports: ETHERNET Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN&FOREGROUND LinkUpBandwidth>=100000Kbps LinkDnBandwidth>=100000Kbps]

> 03-12 10:48:50.978   634   724 D ConnectivityService:   checking if request is satisfied: NetworkRequest [ REQUEST id=1, [ Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN] ]

> 03-12 10:48:50.978   634   724 D ConnectivityService: currentScore = 0, newScore = 30

> 03-12 10:48:50.978   634   724 D ConnectivityService: rematch for NetworkAgentInfo [Ethernet () - 100]

> 03-12 10:48:50.978   634   724 D ConnectivityService:    accepting network in place of null

> 03-12 10:48:50.978   634  1457 D NetworkMonitor/NetworkAgentInfo [Ethernet () - 100]: Validation disabled.

> 03-12 10:48:50.979   634   724 D ConnectivityService: sending new Min Network Score(30): NetworkRequest [ REQUEST id=1, [ Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN] ]

> 03-12 10:48:50.979   634   724 D ConnectivityService: Switching to new default network: NetworkAgentInfo{ ni{[type: Ethernet[], state: CONNECTED/CONNECTED, reason: (unspecified), extra: a2:62:c0:2d:b4:5c, failover: false, available: true, roaming: false, metered: false]}  network{100}  nethandle{429513165534}  lp{{InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,192.168.10.80/24,]  Routes: [fe80::/64 -> :: eth0,192.168.10.0/24 -> 0.0.0.0 eth0,0.0.0.0/0 -> 192.168.10.1 eth0,] DnsAddresses: [192.168.10.1,] Domains: lan MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}  nc{[ Transports: ETHERNET Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN&FOREGROUND LinkUpBandwidth>=100000Kbps LinkDnBandwidth>=100000Kbps]}  Score{30}  everValidated{false}  lastValidated{false}  created{true} lingering{false} explicitlySelected{false} acceptUnvalidated{false} everCaptivePortalDetected{false} lastCaptivePortalDetected{false} }

> 03-12 10:48:50.982   634   724 D ConnectivityService: Setting tx/rx TCP buffers to 524288,1048576,3145728,524288,1048576,2097152

> 03-12 10:48:50.988   634   724 D ConnectivityService: sending notification AVAILABLE for NetworkRequest [ LISTEN id=4, [] ]

> 03-12 10:48:50.989   634   724 D ConnectivityService: sending notification AVAILABLE for NetworkRequest [ LISTEN id=3, [] ]

> 03-12 10:48:50.990   634   724 D ConnectivityService: Sending CONNECTED broadcast for type 9 NetworkAgentInfo [Ethernet () - 100] isDefaultNetwork=true

> 03-12 10:48:50.991   634   724 D ConnectivityService: sendStickyBroadcast: action=android.net.conn.CONNECTIVITY_CHANGE

> 03-12 10:48:50.992   634   724 D ConnectivityService: notifyType PRECHECK for NetworkAgentInfo [Ethernet () - 100]

> 03-12 10:48:50.992   634   724 D ConnectivityService:  sending notification for NetworkRequest [ REQUEST id=1, [ Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN] ]

> 03-12 10:48:50.992   634   724 D ConnectivityService:  sending notification for NetworkRequest [ LISTEN id=3, [] ]

> 03-12 10:48:50.992   634   724 D ConnectivityService: sending notification PRECHECK for NetworkRequest [ LISTEN id=3, [] ]

> 03-12 10:48:50.992   634   724 D ConnectivityService:  sending notification for NetworkRequest [ LISTEN id=4, [] ]

> 03-12 10:48:50.992   634   724 D ConnectivityService: sending notification PRECHECK for NetworkRequest [ LISTEN id=4, [] ]

> 03-12 10:48:50.992   634   654 D Tethering: InitialState got CMD_UPSTREAM_CHANGED

> 03-12 10:48:50.993   634   724 D ConnectivityService: NetworkAgent connected

> 03-12 10:48:50.993   634   724 D ConnectivityService: NetworkAgentInfo [Ethernet () - 100] validation passed

> 03-12 10:48:50.993   634   724 D ConnectivityService: rematching NetworkAgentInfo [Ethernet () - 100]

> 03-12 10:48:50.993   634   724 D ConnectivityService:  network has: [ Transports: ETHERNET Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN&VALIDATED&FOREGROUND LinkUpBandwidth>=100000Kbps LinkDnBandwidth>=100000Kbps]

> 03-12 10:48:50.993   634   724 D ConnectivityService: Network NetworkAgentInfo [Ethernet () - 100] was already satisfying request 1. No change.

> 03-12 10:48:50.993   634   724 D ConnectivityService: notifyType CAP_CHANGED for NetworkAgentInfo [Ethernet () - 100]

> 03-12 10:48:50.993   634   724 D ConnectivityService:  sending notification for NetworkRequest [ REQUEST id=1, [ Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN] ]

> 03-12 10:48:50.993   634   724 D ConnectivityService:  sending notification for NetworkRequest [ LISTEN id=3, [] ]

> 03-12 10:48:50.994   634   724 D ConnectivityService: sending notification CAP_CHANGED for NetworkRequest [ LISTEN id=3, [] ]

> 03-12 10:48:50.994   634   724 D ConnectivityService:  sending notification for NetworkRequest [ LISTEN id=4, [] ]

> 03-12 10:48:50.994   634   724 D ConnectivityService: sending notification CAP_CHANGED for NetworkRequest [ LISTEN id=4, [] ]

> 03-12 10:48:50.995   634   724 D ConnectivityService: sending new Min Network Score(70): NetworkRequest [ REQUEST id=1, [ Capabilities: INTERNET&NOT_RESTRICTED&TRUSTED&NOT_VPN] ]

> 03-12 10:48:50.996   634   724 D ConnectivityService: sendStickyBroadcast: action=android.net.conn.INET_CONDITION_ACTION

> 03-12 10:48:51.180   369  1359 I chatty  : uid=1013(media) Binder:369_6 expire 385 lines

> 03-12 10:48:51.751   634  1449 D IpManager.eth0: newLp{{InterfaceName: eth0 LinkAddresses: [fe80::a062:c0ff:fe2d:b45c/64,192.168.10.80/24,]  Routes: [fe80::/64 -> :: eth0,192.168.10.0/24 -> 0.0.0.0 eth0,0.0.0.0/0 -> 192.168.10.1 eth0,] DnsAddresses: [192.168.10.1,] Domains: lan MTU: 0 TcpBufferSizes: 524288,1048576,3145728,524288,1048576,2097152}}

> 03-12 10:48:51.960   372   407 D alsa_route: route_set_controls() set route 24

> 03-12 10:48:51.962   372   407 D AudioHardwareTiny: close device

> 03-12 10:48:52.957  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Proxima.ogg since build fingerprint changed

> 03-12 10:48:52.981  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Radon.ogg since build fingerprint changed

> 03-12 10:48:53.012  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Rubidium.ogg since build fingerprint changed

> 03-12 10:48:53.033  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Selenium.ogg since build fingerprint changed

> 03-12 10:48:53.054  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Shaula.ogg since build fingerprint changed

> 03-12 10:48:53.079  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Sirrah.ogg since build fingerprint changed

> 03-12 10:48:53.106  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/SpaceSeed.ogg since build fingerprint changed

> 03-12 10:48:53.133  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Spica.ogg since build fingerprint changed

> 03-12 10:48:53.159  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Strontium.ogg since build fingerprint changed

> 03-12 10:48:53.185  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Syrma.ogg since build fingerprint changed

> 03-12 10:48:53.211  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/TaDa.ogg since build fingerprint changed

> 03-12 10:48:53.234  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Talitha.ogg since build fingerprint changed

> 03-12 10:48:53.256  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Tejat.ogg since build fingerprint changed

> 03-12 10:48:53.280  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Thallium.ogg since build fingerprint changed

> 03-12 10:48:53.305  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Tinkerbell.ogg since build fingerprint changed

> 03-12 10:48:53.332  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Upsilon.ogg since build fingerprint changed

> 03-12 10:48:53.358  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Vega.ogg since build fingerprint changed

> 03-12 10:48:53.385  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Voila.ogg since build fingerprint changed

> 03-12 10:48:53.411  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Xenon.ogg since build fingerprint changed

> 03-12 10:48:53.438  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/Zirconium.ogg since build fingerprint changed

> 03-12 10:48:53.462  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/arcturus.ogg since build fingerprint changed

> 03-12 10:48:53.487  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/moonbeam.ogg since build fingerprint changed

> 03-12 10:48:53.513  1344  1451 W MediaScanner: forcing rescan of /system/media/audio/notifications/pixiedust.oggsince ringtone setting didn’t finish

> 03-12 10:48:53.538  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications; assuming invalid

> 03-12 10:48:53.558  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Alarm_Rooster_02.ogg; assuming invalid

> 03-12 10:48:53.564  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Argon.ogg; assuming invalid

> 03-12 10:48:53.570  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Barium.ogg; assuming invalid

> 03-12 10:48:53.576  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Carbon.ogg; assuming invalid

> 03-12 10:48:53.582  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Cesium.ogg; assuming invalid

> 03-12 10:48:53.588  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Fermium.ogg; assuming invalid

> 03-12 10:48:53.593  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Hassium.ogg; assuming invalid

> 03-12 10:48:53.598  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Helium.ogg; assuming invalid

> 03-12 10:48:53.603  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Krypton.ogg; assuming invalid

> 03-12 10:48:53.609  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Neon.ogg; assuming invalid

> 03-12 10:48:53.613  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Neptunium.ogg; assuming invalid

> 03-12 10:48:53.618  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Nobelium.ogg; assuming invalid

> 03-12 10:48:53.623  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Osmium.ogg; assuming invalid

> 03-12 10:48:53.628  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Oxygen.ogg; assuming invalid

> 03-12 10:48:53.632  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Platinum.ogg; assuming invalid

> 03-12 10:48:53.637  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Plutonium.ogg; assuming invalid

> 03-12 10:48:53.642  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Promethium.ogg; assuming invalid

> 03-12 10:48:53.647  1344  1451 W MediaProvider: Missing volume for /system/media/audio/alarms/Scandium.ogg; assuming invalid

> 03-12 10:48:53.663  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Adara.ogg; assuming invalid

> 03-12 10:48:53.670  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Aldebaran.ogg; assuming invalid

> 03-12 10:48:53.674  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Altair.ogg; assuming invalid

> 03-12 10:48:53.679  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Alya.ogg; assuming invalid

> 03-12 10:48:53.684  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Antares.ogg; assuming invalid

> 03-12 10:48:53.689  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Antimony.ogg; assuming invalid

> 03-12 10:48:53.694  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Arcturus.ogg; assuming invalid

> 03-12 10:48:53.699  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Argon.ogg; assuming invalid

> 03-12 10:48:53.706  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Beat_Box_Android.ogg; assuming invalid

> 03-12 10:48:53.711  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Bellatrix.ogg; assuming invalid

> 03-12 10:48:53.716  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Beryllium.ogg; assuming invalid

> 03-12 10:48:53.721  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Betelgeuse.ogg; assuming invalid

> 03-12 10:48:53.727  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/CaffeineSnake.ogg; assuming invalid

> 03-12 10:48:53.732  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Canopus.ogg; assuming invalid

> 03-12 10:48:53.738  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Capella.ogg; assuming invalid

> 03-12 10:48:53.743  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Castor.ogg; assuming invalid

> 03-12 10:48:53.748  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/CetiAlpha.ogg; assuming invalid

> 03-12 10:48:53.754  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Cobalt.ogg; assuming invalid

> 03-12 10:48:53.759  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Cricket.ogg; assuming invalid

> 03-12 10:48:53.764  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/DearDeer.ogg; assuming invalid

> 03-12 10:48:53.770  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Deneb.ogg; assuming invalid

> 03-12 10:48:53.775  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Doink.ogg; assuming invalid

> 03-12 10:48:53.780  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/DontPanic.ogg; assuming invalid

> 03-12 10:48:53.786  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Drip.ogg; assuming invalid

> 03-12 10:48:53.791  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Electra.ogg; assuming invalid

> 03-12 10:48:53.796  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/F1_MissedCall.ogg; assuming invalid

> 03-12 10:48:53.801  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/F1_New_MMS.ogg; assuming invalid

> 03-12 10:48:53.807  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/F1_New_SMS.ogg; assuming invalid

> 03-12 10:48:53.812  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Fluorine.ogg; assuming invalid

> 03-12 10:48:53.817  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Fomalhaut.ogg; assuming invalid

> 03-12 10:48:53.822  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Gallium.ogg; assuming invalid

> 03-12 10:48:53.826  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Heaven.ogg; assuming invalid

> 03-12 10:48:53.831  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Helium.ogg; assuming invalid

> 03-12 10:48:53.836  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Highwire.ogg; assuming invalid

> 03-12 10:48:53.842  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Hojus.ogg; assuming invalid

> 03-12 10:48:53.847  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Iridium.ogg; assuming invalid

> 03-12 10:48:53.852  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Krypton.ogg; assuming invalid

> 03-12 10:48:53.857  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/KzurbSonar.ogg; assuming invalid

> 03-12 10:48:53.862  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Lalande.ogg; assuming invalid

> 03-12 10:48:53.867  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Merope.ogg; assuming invalid

> 03-12 10:48:53.872  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Mira.ogg; assuming invalid

> 03-12 10:48:53.877  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/OnTheHunt.ogg; assuming invalid

> 03-12 10:48:53.882  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Palladium.ogg; assuming invalid

> 03-12 10:48:53.887  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Plastic_Pipe.ogg; assuming invalid

> 03-12 10:48:53.892  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Polaris.ogg; assuming invalid

> 03-12 10:48:53.897  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Pollux.ogg; assuming invalid

> 03-12 10:48:53.902  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Procyon.ogg; assuming invalid

> 03-12 10:48:53.907  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Proxima.ogg; assuming invalid

> 03-12 10:48:53.913  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Radon.ogg; assuming invalid

> 03-12 10:48:53.917  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Rubidium.ogg; assuming invalid

> 03-12 10:48:53.922  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Selenium.ogg; assuming invalid

> 03-12 10:48:53.927  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Shaula.ogg; assuming invalid

> 03-12 10:48:53.932  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Sirrah.ogg; assuming invalid

> 03-12 10:48:53.937  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/SpaceSeed.ogg; assuming invalid

> 03-12 10:48:53.942  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Spica.ogg; assuming invalid

> 03-12 10:48:53.947  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Strontium.ogg; assuming invalid

> 03-12 10:48:53.953  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Syrma.ogg; assuming invalid

> 03-12 10:48:53.958  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/TaDa.ogg; assuming invalid

> 03-12 10:48:53.963  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Talitha.ogg; assuming invalid

> 03-12 10:48:53.967  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Tejat.ogg; assuming invalid

> 03-12 10:48:53.972  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Thallium.ogg; assuming invalid

> 03-12 10:48:53.977  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Tinkerbell.ogg; assuming invalid

> 03-12 10:48:53.982  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Upsilon.ogg; assuming invalid

> 03-12 10:48:53.987  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Vega.ogg; assuming invalid

> 03-12 10:48:53.991  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Voila.ogg; assuming invalid

> 03-12 10:48:53.996  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Xenon.ogg; assuming invalid

> 03-12 10:48:54.001  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/Zirconium.ogg; assuming invalid

> 03-12 10:48:54.006  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/arcturus.ogg; assuming invalid

> 03-12 10:48:54.011  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/moonbeam.ogg; assuming invalid

> 03-12 10:48:54.025  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/pixiedust.ogg; assuming invalid

> 03-12 10:48:54.051  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/pizzicato.ogg since build fingerprint changed

> 03-12 10:48:54.072  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/regulus.ogg since build fingerprint changed

> 03-12 10:48:54.092  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/sirius.ogg since build fingerprint changed

> 03-12 10:48:54.114  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/tweeters.ogg since build fingerprint changed

> 03-12 10:48:54.138  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/notifications/vega.ogg since build fingerprint changed

> 03-12 10:48:54.167  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/ANDROMEDA.ogg since build fingerprint changed

> 03-12 10:48:54.188  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Andromeda.ogg since build fingerprint changed

> 03-12 10:48:54.210  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Aquila.ogg since build fingerprint changed

> 03-12 10:48:54.233  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/ArgoNavis.ogg since build fingerprint changed

> 03-12 10:48:54.261  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Atria.ogg since build fingerprint changed

> 03-12 10:48:54.288  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/BOOTES.ogg since build fingerprint changed

> 03-12 10:48:54.309  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Backroad.ogg since build fingerprint changed

> 03-12 10:48:54.328  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/BeatPlucker.ogg since build fingerprint changed

> 03-12 10:48:54.348  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/BentleyDubs.ogg since build fingerprint changed

> 03-12 10:48:54.368  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Big_Easy.ogg since build fingerprint changed

> 03-12 10:48:54.388  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/BirdLoop.ogg since build fingerprint changed

> 03-12 10:48:54.409  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Bollywood.ogg since build fingerprint changed

> 03-12 10:48:54.429  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/BussaMove.ogg since build fingerprint changed

> 03-12 10:48:54.450  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/CANISMAJOR.ogg since build fingerprint changed

> 03-12 10:48:54.470  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/CASSIOPEIA.ogg since build fingerprint changed

> 03-12 10:48:54.492  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Cairo.ogg since build fingerprint changed

> 03-12 10:48:54.515  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Calypso_Steel.ogg since build fingerprint changed

> 03-12 10:48:54.566  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/CanisMajor.ogg since build fingerprint changed

> 03-12 10:48:54.586  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/CaribbeanIce.ogg since build fingerprint changed

> 03-12 10:48:54.606  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Carina.ogg since build fingerprint changed

> 03-12 10:48:54.671  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Centaurus.ogg since build fingerprint changed

> 03-12 10:48:54.695  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Champagne_Edition.ogg since build fingerprint changed

> 03-12 10:48:54.718  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Club_Cubano.ogg since build fingerprint changed

> 03-12 10:48:54.741  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/CrayonRock.ogg since build fingerprint changed

> 03-12 10:48:54.764  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/CrazyDream.ogg since build fingerprint changed

> 03-12 10:48:54.788  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/CurveBall.ogg since build fingerprint changed

> 03-12 10:48:54.813  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Cygnus.ogg since build fingerprint changed

> 03-12 10:48:54.837  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/DancinFool.ogg since build fingerprint changed

> 03-12 10:48:54.860  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Ding.ogg since build fingerprint changed

> 03-12 10:48:54.881  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/DonMessWivIt.ogg since build fingerprint changed

> 03-12 10:48:54.902  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Draco.ogg since build fingerprint changed

> 03-12 10:48:54.922  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/DreamTheme.ogg since build fingerprint changed

> 03-12 10:48:54.944  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Eastern_Sky.ogg since build fingerprint changed

> 03-12 10:48:54.967  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Enter_the_Nexus.ogg since build fingerprint changed

> 03-12 10:48:54.990  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Eridani.ogg since build fingerprint changed

> 03-12 10:48:55.016  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/EtherShake.ogg since build fingerprint changed

> 03-12 10:48:55.043  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/FreeFlight.ogg since build fingerprint changed

> 03-12 10:48:55.071  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/FriendlyGhost.ogg since build fingerprint changed

> 03-12 10:48:55.128  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Funk_Yall.ogg since build fingerprint changed

> 03-12 10:48:55.152  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/GameOverGuitar.ogg since build fingerprint changed

> 03-12 10:48:55.186  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Gimme_Mo_Town.ogg since build fingerprint changed

> 03-12 10:48:55.209  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Girtab.ogg since build fingerprint changed

> 03-12 10:48:55.232  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Glacial_Groove.ogg since build fingerprint changed

> 03-12 10:48:55.257  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Growl.ogg since build fingerprint changed

> 03-12 10:48:55.285  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/HalfwayHome.ogg since build fingerprint changed

> 03-12 10:48:55.313  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Hydra.ogg since build fingerprint changed

> 03-12 10:48:55.316   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.319   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.324   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.324   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.337   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.337   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.344  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/InsertCoin.ogg since build fingerprint changed

> 03-12 10:48:55.347   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.347   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/InsertCoin.ogg

> 03-12 10:48:55.348   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.348   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.350   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.354   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.354   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.363   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.363   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.367  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Kuma.ogg since build fingerprint changed

> 03-12 10:48:55.370   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.370   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Kuma.ogg

> 03-12 10:48:55.370   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.370   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.375   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.379   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.379   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.387   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.387   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.392  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/LoopyLounge.ogg since build fingerprint changed

> 03-12 10:48:55.395   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.395   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/LoopyLounge.ogg

> 03-12 10:48:55.395   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.395   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.398   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.403   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.403   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.412   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.412   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.418  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/LoveFlute.ogg since build fingerprint changed

> 03-12 10:48:55.421   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.421   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/LoveFlute.ogg

> 03-12 10:48:55.421   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.421   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.424   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.430   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.430   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.439   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.440   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.445  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Lyra.ogg since build fingerprint changed

> 03-12 10:48:55.448   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.448   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Lyra.ogg

> 03-12 10:48:55.448   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.448   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.452   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.457   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.457   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.467   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.467   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.472  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Machina.ogg since build fingerprint changed

> 03-12 10:48:55.475   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.476   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Machina.ogg

> 03-12 10:48:55.476   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.476   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.479   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.483   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.483   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.492   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.492   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.497  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/MidEvilJaunt.ogg since build fingerprint changed

> 03-12 10:48:55.513   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.513   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/MidEvilJaunt.ogg

> 03-12 10:48:55.513   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.513   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.516   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.520   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.520   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.528   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.528   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.532  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/MildlyAlarming.ogg since build fingerprint changed

> 03-12 10:48:55.534   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.534   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/MildlyAlarming.ogg

> 03-12 10:48:55.534   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.535   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.537   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.540   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.541   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.547   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.547   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.552  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Nairobi.ogg since build fingerprint changed

> 03-12 10:48:55.554   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.554   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Nairobi.ogg

> 03-12 10:48:55.554   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.554   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.557   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.561   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.561   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.571   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.571   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.576  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Nassau.ogg since build fingerprint changed

> 03-12 10:48:55.578   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.578   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Nassau.ogg

> 03-12 10:48:55.578   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.578   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.581   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.585   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.585   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.593   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.594   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.598  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/NewPlayer.ogg since build fingerprint changed

> 03-12 10:48:55.601   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.601   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/NewPlayer.ogg

> 03-12 10:48:55.601   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.601   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.603   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.608   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.608   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.616   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.616   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.620  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/No_Limits.ogg since build fingerprint changed

> 03-12 10:48:55.623   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.623   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/No_Limits.ogg

> 03-12 10:48:55.623   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.623   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.626   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.630   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.630   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.638   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.638   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.643  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Noises1.ogg since build fingerprint changed

> 03-12 10:48:55.646   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.646   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Noises1.ogg

> 03-12 10:48:55.646   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.646   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.649   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.655   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.655   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.663   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.663   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.667  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Noises2.ogg since build fingerprint changed

> 03-12 10:48:55.686   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.686   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Noises2.ogg

> 03-12 10:48:55.686   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.686   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.689   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.693   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.693   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.700   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.700   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.704  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Noises3.ogg since build fingerprint changed

> 03-12 10:48:55.708   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.708   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Noises3.ogg

> 03-12 10:48:55.708   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.708   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.710   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.714   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.714   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.720   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.720   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.725  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/OrganDub.ogg since build fingerprint changed

> 03-12 10:48:55.728   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.728   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/OrganDub.ogg

> 03-12 10:48:55.728   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.728   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.731   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.734   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.734   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.740   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.740   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.744  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Orion.ogg since build fingerprint changed

> 03-12 10:48:55.746   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.746   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Orion.ogg

> 03-12 10:48:55.746   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.746   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.750   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.753   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.753   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.759   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.759   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.763  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/PERSEUS.ogg since build fingerprint changed

> 03-12 10:48:55.765   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.765   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/PERSEUS.ogg

> 03-12 10:48:55.765   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.765   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.769   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.774   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.774   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.782   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.782   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.787  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Paradise_Island.ogg since build fingerprint changed

> 03-12 10:48:55.790   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.790   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Paradise_Island.ogg

> 03-12 10:48:55.790   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.791   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.794   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.797   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.797   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.805   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.805   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.810  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Pegasus.ogg since build fingerprint changed

> 03-12 10:48:55.813   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.813   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Pegasus.ogg

> 03-12 10:48:55.813   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.813   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.817   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.821   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.821   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.831   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.831   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.836  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Perseus.ogg since build fingerprint changed

> 03-12 10:48:55.839   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.839   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Perseus.ogg

> 03-12 10:48:55.839   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.840   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.845   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.849   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.849   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.859   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.859   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.864  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Playa.ogg since build fingerprint changed

> 03-12 10:48:55.867   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.868   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Playa.ogg

> 03-12 10:48:55.868   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.868   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.871   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.876   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.877   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.886   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.886   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.891  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Pyxis.ogg since build fingerprint changed

> 03-12 10:48:55.894   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.895   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Pyxis.ogg

> 03-12 10:48:55.895   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.895   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.898   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.903   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.903   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.913   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.913   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.918  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Rasalas.ogg since build fingerprint changed

> 03-12 10:48:55.921   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.921   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Rasalas.ogg

> 03-12 10:48:55.921   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.922   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.927   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.931   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.931   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.941   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.941   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.946  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Revelation.ogg since build fingerprint changed

> 03-12 10:48:55.949   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.949   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Revelation.ogg

> 03-12 10:48:55.949   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.950   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.953   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.957   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.958   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.967   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.967   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.972  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Rigel.ogg since build fingerprint changed

> 03-12 10:48:55.975   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.975   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Rigel.ogg

> 03-12 10:48:55.975   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.975   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:55.978   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:55.983   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:55.983   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:55.991   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:55.991   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:55.996  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Ring_Classic_02.ogg since build fingerprint changed

> 03-12 10:48:55.998   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:55.998   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Ring_Classic_02.ogg

> 03-12 10:48:55.998   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:55.998   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:56.002   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:56.005   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:56.005   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:56.015   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:56.015   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:56.021  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Ring_Digital_02.ogg since build fingerprint changed

> 03-12 10:48:56.024   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:56.024   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Ring_Digital_02.ogg

> 03-12 10:48:56.024   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:56.024   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:56.027   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:56.032   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:56.032   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:56.039   634   714 D SntpClient: request time failed: java.net.SocketTimeoutException: Receive timed out

> 03-12 10:48:56.039   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:56.040   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:56.047  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Ring_Synth_02.ogg since build fingerprint changed

> 03-12 10:48:56.050   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:56.050   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Ring_Synth_02.ogg

> 03-12 10:48:56.050   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:56.051   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:56.054   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:56.057   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:56.057   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:56.065   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:56.065   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:56.070  1344  1451 W MediaScanner: forcing rescan of /system/media/audio/ringtones/Ring_Synth_04.oggsince ringtone setting didn’t finish

> 03-12 10:48:56.073   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:56.073   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Ring_Synth_04.ogg

> 03-12 10:48:56.073   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:56.073   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:56.076   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:56.080   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:56.080   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:56.088   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:56.088   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:56.092  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones; assuming invalid

> 03-12 10:48:56.103  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/pizzicato.ogg; assuming invalid

> 03-12 10:48:56.109  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/regulus.ogg; assuming invalid

> 03-12 10:48:56.114  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/sirius.ogg; assuming invalid

> 03-12 10:48:56.120  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/tweeters.ogg; assuming invalid

> 03-12 10:48:56.126  1344  1451 W MediaProvider: Missing volume for /system/media/audio/notifications/vega.ogg; assuming invalid

> 03-12 10:48:56.136   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:56.136   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/ANDROMEDA.ogg

> 03-12 10:48:56.136   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:56.136   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:56.137   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:56.142   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:56.142   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:56.144  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/ANDROMEDA.ogg; assuming invalid

> 03-12 10:48:56.146   369   884 E MetadataRetrieverClient: failed to extract an album art

> 03-12 10:48:56.147   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:56.147   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:56.154  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Andromeda.ogg; assuming invalid

> 03-12 10:48:56.158  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Aquila.ogg; assuming invalid

> 03-12 10:48:56.163  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/ArgoNavis.ogg; assuming invalid

> 03-12 10:48:56.168  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Atria.ogg; assuming invalid

> 03-12 10:48:56.173  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/BOOTES.ogg; assuming invalid

> 03-12 10:48:56.177  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Backroad.ogg; assuming invalid

> 03-12 10:48:56.182  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/BeatPlucker.ogg; assuming invalid

> 03-12 10:48:56.187  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/BentleyDubs.ogg; assuming invalid

> 03-12 10:48:56.191  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Big_Easy.ogg; assuming invalid

> 03-12 10:48:56.196  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/BirdLoop.ogg; assuming invalid

> 03-12 10:48:56.211  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Bollywood.ogg; assuming invalid

> 03-12 10:48:56.216  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/BussaMove.ogg; assuming invalid

> 03-12 10:48:56.221  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/CANISMAJOR.ogg; assuming invalid

> 03-12 10:48:56.226  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/CASSIOPEIA.ogg; assuming invalid

> 03-12 10:48:56.230  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Cairo.ogg; assuming invalid

> 03-12 10:48:56.235  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Calypso_Steel.ogg; assuming invalid

> 03-12 10:48:56.239  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/CanisMajor.ogg; assuming invalid

> 03-12 10:48:56.244  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/CaribbeanIce.ogg; assuming invalid

> 03-12 10:48:56.248  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Carina.ogg; assuming invalid

> 03-12 10:48:56.252  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Centaurus.ogg; assuming invalid

> 03-12 10:48:56.256  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Champagne_Edition.ogg; assuming invalid

> 03-12 10:48:56.260  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Club_Cubano.ogg; assuming invalid

> 03-12 10:48:56.265  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/CrayonRock.ogg; assuming invalid

> 03-12 10:48:56.269  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/CrazyDream.ogg; assuming invalid

> 03-12 10:48:56.274  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/CurveBall.ogg; assuming invalid

> 03-12 10:48:56.279  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Cygnus.ogg; assuming invalid

> 03-12 10:48:56.284  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/DancinFool.ogg; assuming invalid

> 03-12 10:48:56.289  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Ding.ogg; assuming invalid

> 03-12 10:48:56.294  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/DonMessWivIt.ogg; assuming invalid

> 03-12 10:48:56.299  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Draco.ogg; assuming invalid

> 03-12 10:48:56.305  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/DreamTheme.ogg; assuming invalid

> 03-12 10:48:56.306   634   648 W ActivityManager: Launch timeout has expired, giving up wake lock!

> 03-12 10:48:56.310  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Eastern_Sky.ogg; assuming invalid

> 03-12 10:48:56.315  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Enter_the_Nexus.ogg; assuming invalid

> 03-12 10:48:56.319  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Eridani.ogg; assuming invalid

> 03-12 10:48:56.324  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/EtherShake.ogg; assuming invalid

> 03-12 10:48:56.329  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/FreeFlight.ogg; assuming invalid

> 03-12 10:48:56.333  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/FriendlyGhost.ogg; assuming invalid

> 03-12 10:48:56.338  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Funk_Yall.ogg; assuming invalid

> 03-12 10:48:56.343  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/GameOverGuitar.ogg; assuming invalid

> 03-12 10:48:56.348  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Gimme_Mo_Town.ogg; assuming invalid

> 03-12 10:48:56.353  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Girtab.ogg; assuming invalid

> 03-12 10:48:56.357  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Glacial_Groove.ogg; assuming invalid

> 03-12 10:48:56.362  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Growl.ogg; assuming invalid

> 03-12 10:48:56.366  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/HalfwayHome.ogg; assuming invalid

> 03-12 10:48:56.371  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Hydra.ogg; assuming invalid

> 03-12 10:48:56.376  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/InsertCoin.ogg; assuming invalid

> 03-12 10:48:56.380  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Kuma.ogg; assuming invalid

> 03-12 10:48:56.386  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/LoopyLounge.ogg; assuming invalid

> 03-12 10:48:56.391  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/LoveFlute.ogg; assuming invalid

> 03-12 10:48:56.395  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Lyra.ogg; assuming invalid

> 03-12 10:48:56.400  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Machina.ogg; assuming invalid

> 03-12 10:48:56.405  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/MidEvilJaunt.ogg; assuming invalid

> 03-12 10:48:56.410  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/MildlyAlarming.ogg; assuming invalid

> 03-12 10:48:56.414  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Nairobi.ogg; assuming invalid

> 03-12 10:48:56.419  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Nassau.ogg; assuming invalid

> 03-12 10:48:56.424  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/NewPlayer.ogg; assuming invalid

> 03-12 10:48:56.428  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/No_Limits.ogg; assuming invalid

> 03-12 10:48:56.433  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Noises1.ogg; assuming invalid

> 03-12 10:48:56.438  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Noises2.ogg; assuming invalid

> 03-12 10:48:56.444  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Noises3.ogg; assuming invalid

> 03-12 10:48:56.465  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/OrganDub.ogg; assuming invalid

> 03-12 10:48:56.474  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Orion.ogg; assuming invalid

> 03-12 10:48:56.479  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/PERSEUS.ogg; assuming invalid

> 03-12 10:48:56.484  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Paradise_Island.ogg; assuming invalid

> 03-12 10:48:56.489  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Pegasus.ogg; assuming invalid

> 03-12 10:48:56.494  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Perseus.ogg; assuming invalid

> 03-12 10:48:56.499  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Playa.ogg; assuming invalid

> 03-12 10:48:56.504  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Pyxis.ogg; assuming invalid

> 03-12 10:48:56.509  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Rasalas.ogg; assuming invalid

> 03-12 10:48:56.514  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Revelation.ogg; assuming invalid

> 03-12 10:48:56.519  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Rigel.ogg; assuming invalid

> 03-12 10:48:56.524  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Ring_Classic_02.ogg; assuming invalid

> 03-12 10:48:56.529  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Ring_Digital_02.ogg; assuming invalid

> 03-12 10:48:56.533  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Ring_Synth_02.ogg; assuming invalid

> 03-12 10:48:56.548  1344  1451 W MediaProvider: Missing volume for /system/media/audio/ringtones/Ring_Synth_04.ogg; assuming invalid

> 03-12 10:48:56.580  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Road_Trip.ogg since build fingerprint changed

> 03-12 10:48:56.583   369   884 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:56.583   369   884 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Road_Trip.ogg

> 03-12 10:48:56.583   369   884 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:56.583   369   884 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:56.586   369   884 D RockFFPlayer: find av stream info in

> 03-12 10:48:56.588   369   884 D RockFFPlayer: find av stream info ok

> 03-12 10:48:56.589   369   884 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:56.595   369   885 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:56.595   369   885 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:56.599  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/RomancingTheTone.ogg since build fingerprint changed

> 03-12 10:48:56.601   369   885 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:56.601   369   885 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/RomancingTheTone.ogg

> 03-12 10:48:56.601   369   885 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:56.601   369   885 D RkFFMetadataRetriever: setDataSource:filePath = file/fd::8,offset::0

> 03-12 10:48:56.604   369   885 D RockFFPlayer: find av stream info in

> 03-12 10:48:56.607   369   885 D RockFFPlayer: find av stream info ok

> 03-12 10:48:56.607   369   885 D RockFFPlayer: AudioStartTime=0,VideoStartTime=-1,tmpTime=-1

> 03-12 10:48:56.613   369   884 D RockFFPlayer: ~FFMediaSource

> 03-12 10:48:56.613   369   884 D RockFFPlayer: FFMediaSource context null.

> 03-12 10:48:56.617  1344  1451 I MediaScanner: forcing rescan of /system/media/audio/ringtones/Safari.ogg since build fingerprint changed

> 03-12 10:48:56.619   369  1359 D MetadataRetrieverClient: Create Instance of RockMetaDataRetriever

> 03-12 10:48:56.619   369  1359 E FFPlayerHelper: isBDDirectory() ,path = /system/media/audio/ringtones/Safari.ogg

> 03-12 10:48:56.619   369  1359 E FFPlayerHelper: isBDDirectory(): no BDMV

> 03-12 10:48:56.619   369  1359 D RkFFMetadataRetriever: setDataSource:filePath = file


