# 游戏服务器之基于文件数据库和mysql数据库的db服务器设计 - gauss的专栏 - CSDN博客
2014年10月19日 14:58:48[gauss](https://me.csdn.net/mathlmx)阅读数：558
设计上：
（1）文件数据库是用于处理游戏逻辑业务请求。
（2）mysql数据库是用于处理运营后台查询请求。
（3）备份业务是用于定时备份文件数据库文件。
总体设计图如 [http://blog.csdn.net/chenjiayi_yun/article/details/11913207](http://blog.csdn.net/chenjiayi_yun/article/details/11913207)
文件数据库业务设计 可查看 [http://blog.csdn.net/chenjiayi_yun/article/details/17880275](http://blog.csdn.net/chenjiayi_yun/article/details/17880275)
本文目录：
1、角色数据更新请求
2、mysql业务线程循环处理更新请求
3、文件数据库业务线程循环处理更新请求
4、备份业务线程循环处理备份业务
本文内容：
1、角色数据更新请求
保存角色数据请求,提交到业务线程队列
保存角色描述数据，提交到文件数据库角色描述数据更新列表和mysql角色描述数据更新列表。
保存角色具体数据，提交到文件数据库角色数据更新列表和mysql角色数据更新列表。
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/398245/fork)
- VOID CDBDataClient::CatchSaveCharData(CDataPacketReader &inPacket)  
- {  
- INT64 nCharId;  
- DWORD dwDataSize;  
- 
- inPacket >> nCharId;  
- inPacket >> dwDataSize;  
- 
- CGameDataBuffer *pBuffer = m_pDataServer->AllocGameDataBuffer();  
- pBuffer->AddRef();  
- pBuffer->SetData(nCharId, inPacket.getOffsetPtr(), dwDataSize);  
- 
- CharDesc *pCharDesc = m_pLocalDB->GetCharDesc(nCharId);  
- if (pCharDesc)  
- {  
- //跳过角色基本数据头结构
- common::DBType::PlayerBasicData *pBaseData =(common::DBType::PlayerBasicData *)\  
- pBuffer->getPositionPtr(sizeof(common::DBType::DataSectionHeader));  
- 
- //发起对角色描述数据的更新
- pCharDesc->wLevel = pBaseData->wLevel;  
- pCharDesc->btGender = pBaseData->btGender;  
- pCharDesc->nUpdateTime = CMiniDateTime::now();  
- 
- m_pLocalDB->PostUpdateCharDesc(pCharDesc);  
- m_pSQLDB->PostUpdateCharDesc(pCharDesc);  
- }  
- else logError( _T("试图更新在本地角色描述数据中找不到的角色数据(%lld)"), nCharId);  
- 
- m_pLocalDB->PostUpdateCharData(pBuffer);  
- m_pSQLDB->PostUpdateCharData(pBuffer);  
- 
- pBuffer->Release();  
- 
- CDataPacket &pack = AllocProtoPacket(DBType::dsSaveCharData);  
- pack << (int)0;//SUCCESS
- pack << nCharId;  
- FlushProtoPacket(pack);  
- }  
2、mysql业务线程循环处理更新请求
mysql线程处理循环，处理数据写入mysql请求
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/398245/fork)
- VOID CSQLDB::OnRountine()  
- {  
- TICKCOUNT dwStartTick;  
- while (!terminated())  
- {  
- if (!m_boStarted)  
- {  
- moon::OS::osSleep(16);  
- continue;  
- }  
- //连接SQL数据库
- if (!ConnectSQLDB())  
- {  
- moon::OS::osSleep(16);  
- continue;  
- }  
- dwStartTick = _getTickCount();  
- //处理角色描述更新请求
- m_NewCharList.flush();  
- if (m_NewCharList.count() > 0)  
- {  
- ProcessAddNewChars(4);  
- }  
- m_UpdateCharDescList.flush();  
- if (m_UpdateCharDescList.count() > 0)  
- {  
- ProcessUpdateCharDescs(4);  
- }  
- //处理角色数据更新请求
- m_CharDataUpdateList.flush();  
- if (m_CharDataUpdateList.count() > 0)  
- {  
- ProcessCharDataUpdate(4);  
- }  
- //处理帮会数据更新请求
- m_GuildDataUpdateList.flush();  
- if (m_GuildDataUpdateList.count() > 0)  
- {  
- ProcessGuildDataUpdate(4);  
- }  
- moon::OS::osSleep(4);  
- }  
- }  
3、文件数据库业务线程循环处理更新请求
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/398245/fork)
- VOID CLocalDB::OnRountine()  
- {  
-     TICKCOUNT dwStartTick;  
-     TICKCOUNT dwBackupTick = 0;  
- while (!terminated())  
-     {  
- if (!m_boOpened)  
-         {  
-             osSleep(16);  
- continue;  
-         }  
- 
-         dwStartTick = _getTickCount();  
- if ((m_nBackupInterval) && (0 == dwBackupTick))  
-             dwBackupTick = _getTickCount();  
- //处理角色描述更新请求
-         m_CharDescUpdateList.flush();  
- if (m_CharDescUpdateList.count() > 0)  
-         {  
-             ProcessCharDescUpdate(4);  
-         }  
- //处理角色数据更新请求
-         m_CharDataUpdateList.flush();  
- if (m_CharDataUpdateList.count() > 0)  
-         {  
-             ProcessCharDataUpdate(4);  
-         }  
- //处理帮会数据更新请求
-         m_GuildDataUpdateList.flush();  
- if (m_GuildDataUpdateList.count() > 0)  
-         {  
-             ProcessGuildDataUpdate(4);  
-         }  
- 
- if ((m_nBackupInterval) && ((INT)(_getTickCount() - dwBackupTick) >= m_nBackupInterval))//每隔一段时间（目前48小时）
-         {  
-             BackupFDBFiles();  
-             m_ZipBackup.StartZipBackup();//发送信号通知备份业务线程处理备份业务
-             dwBackupTick = 0;  
-         }  
- 
-         osSleep(4);  
-     }  
- }  
4、备份业务线程循环处理备份业务
备份业务线程阻塞等待通知来处理文件数据库的文件的备份业务。
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/32133623#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/398245/fork)
- VOID CZipBackup::OnRountine()  
- {  
- #ifdef WINDOWS
-     m_hZipBackupEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);  
- #else
-     pthread_cond_init(&m_hZipBackupEvent, NULL);  
- #endif
- while (!terminated())  
-     {  
- #ifdef WINDOWS
-         ::WaitForSingleObject(m_hZipBackupEvent, INFINITE);  
- #else
-         pthread_mutex_lock( &m_tx );  
-         pthread_cond_wait( &m_hZipBackupEvent , &m_tx );  
-         pthread_mutex_unlock( &m_tx );  
- #endif
- if (terminated())  
- break;  
-         DoZipBackup();  
-         CleanBackupFiles();  
-         osSleep(10);  
-     }  
- #ifdef WINDOWS
-     ::CloseHandle(m_hZipBackupEvent);  
-     m_hZipBackupEvent = NULL;  
- #else
-     pthread_cond_destroy(&m_hZipBackupEvent);  
- #endif
- }  
