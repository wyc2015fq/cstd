# 一个高性能RPC框架的连接管理 - gauss的专栏 - CSDN博客
2015年04月14日 15:08:35[gauss](https://me.csdn.net/mathlmx)阅读数：358
个人分类：[分布式与缓存设计](https://blog.csdn.net/mathlmx/article/category/3168687)
既然说连接，先对EpollServer的连接管理做个介绍吧。客户端与服务器一次conn，被封装成为Connection类在服务器进行管理。
服务器连接有三种类型，分别为：
**enum** EnumConnectionType
        {
*EM_TCP* =
 0,
*EM_UDP* =
 1,
*EM_BUS* =
 2
        };
**Connection成员属性**
time_t_iLastRefreshTime;//最后刷新时间，初始化为初始化的当前时间
BindAdapter        
 *_pBindAdapter;//适配器
TC_Socket_sock;
 //TC_Socket
uint32_t_uid;
 //连接的唯一编号，默认为0
**int**_lfd;
 //监听的socket，默认为-1
**int**_timeout;//超时时间
string_ip;
 //ip
uint16_t_port;
 //端口，默认为0
string_recvbuffer;
 //接收数据buffer
string_sendbuffer;
 //发送数据buffer
size_t_sendPos;//默认为0
**int**_iHeaderLen;
 //需要过滤的头部字节数，默认为0
**bool**_bClose;
 //发送完当前数据就关闭连接，默认为false
**int**_iMaxTemQueueSize;//临时队列的最大长度，默认为100
        EnumConnectionType _enType;
 //连接类型，默认为TCP
**bool**_bEmptyConn;
 //是否为空连接，默认为false，UDP一直为false
**char**                 *_pRecvBuffer;
 //接收数据的临时buffer,加这个目的是对udp接收数据包大小进行设置,默认为null
size_t_nRecvBufferSize;//默认为64K，每次默认读取数据块大小
Connection提供了一系列的getter/setter方法修改上述属性，并且将那个EpollServer列为friend class,提供接收数据的功能操作接口。
**注意!一大波数据正在袭来**
当连接需要接收数据的时候（边缘触发，所以这里关键是一次性要将所有数据接收完）,我们需要一次性把数据接收完以后，把数据解析成一个一个完整的数据包，放入到指定的队列o中，首先Connection的buf是string的，本身不存在长度限制（可优化点），每次read的buf是8192个字节，不断的把客户端管道中的数据读取到Connection的Buf中，直到客户端EAGINE（没有数据了），或者接收到的数据小于每次读取的8192个字节。则表示已经没有数据了。所以这里如果长时间不处理，将会使得Connection的buf
 string堆积很多数据，如果没有数据可读了，则调用解析协议。
**int****TC_EpollServer::Connection::recv**(recv_queue::queue_type &o)
{
    o.clear();
**char** buffer[8192]
 = "\0";
**while**(**true**)
    {
**int** iBytesReceived
 = 0;
**if**(_lfd ==
 -1)
        {
**if**(_pRecvBuffer)
      //如果该connection分配了自己的数据接受buffer，则用自己的，否则每次接收每次new出来一块buffer。
                {
                     iBytesReceived = _sock.recvfrom((**void***)_pRecvBuffer,_nRecvBufferSize, _ip, _port,
 0);
                }
**else**
                {
                     iBytesReceived = _sock.recvfrom((**void***)buffer,**sizeof**(buffer), _ip, _port,
 0);
                }
        }
**else**
        {
            iBytesReceived = ::read(_sock.getfd(),
 (**void***)buffer, **sizeof**(buffer));
        }
**if** (iBytesReceived
 < 0)
        {
**if**(errno == EAGAIN)
            {
//没有数据了
**break**;
            }
**else**
            {
//客户端主动关闭
_pBindAdapter->getEpollServer()->debug("recv ["+ _ip + ":" + TC_Common::tostr(_port)
 + "] close connection");
**return** -1;
            }
        }
**else****if**(
 iBytesReceived == 0)
        {
//客户端主动关闭
_pBindAdapter->getEpollServer()->debug("recv ["+ _ip + ":" + TC_Common::tostr(_port)
 + "] close connection");
**return** -1;
        }
//保存接收到数据
**if**(_lfd ==
 -1)
        {
**if**(_pRecvBuffer)
                {
_recvbuffer.append(_pRecvBuffer,
 iBytesReceived);
                }
**else**
                {
_recvbuffer.append(buffer,
 iBytesReceived);
                }
        }
**else**
           {
_recvbuffer.append(buffer,
 iBytesReceived);
           }
//UDP协议
**if**(_lfd ==
 -1)
        {
**if**(_pBindAdapter->isIpAllow(_ip)
 == **true**)
            {
parseProtocol(o);
            }
**else**
            {
//udpip无权限
_pBindAdapter->getEpollServer()->debug("accept
 [" + _ip+ ":"+ TC_Common::tostr(_port)
 + "] [" + TC_Common::tostr(_lfd)
 + "] not allowed");
            }
_recvbuffer = "";
        }
**else**
        {
//接收到数据不超过buffer,没有数据了(如果有数据,内核会再通知你)
**if**((size_t)iBytesReceived
 < **sizeof**(buffer))
            {
**break**;
            }
//字符串太长时substr性能会急剧下降
**if**(_recvbuffer.length()
 > 8192)
            {
parseProtocol(o);
            }
        }
    }
**if**(_lfd !=
 -1)
    {
**return**parseProtocol(o);
    }
**return** o.size();
}
解析方法将把Connection从客户端管道中获取的所有的数据（_recvbuffer）调用协议解析器进行协议解析，直到剩下的数据不构成一个完整的接受包为止。把这些请求包放入方法参数的队列中。
**int****TC_EpollServer::Connection::****parseProtocol**(recv_queue::queue_type&o)
{
**try**
    {
**while**(**true**)
        {
//需要过滤首包包头
**if**(_iHeaderLen>
 0)
            {
**if**(_recvbuffer.length()
 >= (**unsigned**)_iHeaderLen)
                {
stringheader =_recvbuffer.substr(0,_iHeaderLen);
_pBindAdapter->getHeaderFilterFunctor()((**int**)(TC_EpollServer::*PACKET_FULL*),
 header);
_recvbuffer=_recvbuffer.substr(_iHeaderLen);
_iHeaderLen=
 0;
                }
**else**
                {
_pBindAdapter->getHeaderFilterFunctor()((**int**)(TC_EpollServer::*PACKET_LESS*),_recvbuffer);
_iHeaderLen-=_recvbuffer.length();
_recvbuffer="";
**break**;
                }
            }
stringro;
**int**b
 =_pBindAdapter->getProtocol()(_recvbuffer,
 ro);    //此处调用了Adapter关联的编码解码器，taf默认是AppProtocol::parse()
**if**(b
 ==TC_EpollServer::*PACKET_LESS*)
            {
//包不完全
**break**;
   
            }
**else****if**(b
 ==TC_EpollServer::*PACKET_FULL*) 
            {
tagRecvData*
 recv =**new**tagRecvData();
                recv->buffer          
 = ro;
                recv->ip              
 =_ip;
                recv->port            
 =_port;
                recv->recvTimeStamp   
 =TC_TimeProvider::*getInstance*()->getNow();
                recv->uid             
 = getId();
                recv->isOverload      
 =**false**;
                recv->isClosed        
 =**false**;
//收到完整的包才算
**this**->_bEmptyConn=**false**;
//收到完整包
                o.push_back(recv);
**if**((**int**)
 o.size() >_iMaxTemQueueSize)
                {
insertRecvQueue(o);
                    o.clear();
                }
**if**(_recvbuffer.empty())
                {
**break**;
                }
            }
**else**
            {
_pBindAdapter->getEpollServer()->error("recv["+_ip+":"+TC_Common::tostr(_port)
 +"],packet error.");
**return**-1;                     //协议解析错误
            }
        }
    }
**catch**(exception&ex)
    {
_pBindAdapter->getEpollServer()->error("recvprotocol
 error:"+string(ex.what()));
**return**-1;
    }
**catch**(...)
    {
_pBindAdapter->getEpollServer()->error("recvprotocol
 error");
**return**-1;
    }
**return**o.size();
}
AppProtocol解析如下：
/**
     * 解析协议
     * @paramin, 目前的buffer
     * @paramout, 一个完整的包
     *
     * @returnint, 0表示没有接收完全, 1表示收到一个完整包
     */
**static****int****parse**(string&in,string&out)
    {
**return**parseLen<10000000>(in,out);
    }
**template**<taf::Int32**iMaxLength**>
**static****int****parseLen**(string&in,string&out)
    {
**if**(in.length()
 <**sizeof**(taf::Int32))
  //首先需要一个int32来标志包的长度，所以我们的接受buff的长度起码得>int32,否则标识为收包不完整
        {
**return**TC_EpollServer::*PACKET_LESS*;
        }
        taf::Int32iHeaderLen;
 //包的长度头
memcpy(&iHeaderLen, in.c_str(),**sizeof**(taf::Int32));
  //获取包的长度
        iHeaderLen =ntohl(iHeaderLen); //将网络顺序变为主机顺序，非常重要
**if**(iHeaderLen
 < taf::Int32(**sizeof**(taf::Int32))||
 iHeaderLen >**iMaxLength**) //如果当前的包的长度超过了最大长度或者小于一个int32的长度（包头本身）则认为协议错误
        {
**return**TC_EpollServer::*PACKET_ERR*;
        }
**if**((**int**)in.length()
 < iHeaderLen)  //缓存区里面字段长度不够包头长度
        {
**return**TC_EpollServer::*PACKET_LESS*;
        }
        out = in.substr(**sizeof**(taf::Int32),
 iHeaderLen -**sizeof**(taf::Int32));
        in  = in.substr(iHeaderLen);//将刚才已经解析的完整的包部分从输入中去掉
**return**TC_EpollServer::*PACKET_FULL*;
    }
利用adapter中注册的protocol进行编解码，如果包不完整，则跳出，返回状态码，如果收包完整，则构建新的tagRecvData，设置完整的包buff，并且赋予Connection的ID（这个Connection锁接收到的数据包都是这个Connection的ID）以及将接收数据tagRecvData的状态设置为非空，将这个包放入临时接收数据队列。如果当前这个接受数据队列超出了临时队列的最大长度（否则则在返回后再插入，避免大量的请求阻塞在这里而不能得到及时处理导致的各种超时，所以每到一定数目，则先放入adapter去进行处理），则将该接收队列中的数据包放到adapter的rbuffer中(这个rbuffer是所有Connection一起共享的，所以必须是线程安全的)，如果当前adapter已经过载，将数据包依次设置为过载状态（很重要）放前面，这样有利于该Adapter接口紧急处理，避免雪崩和滚雪球效应，否则放后面。
**void****TC_EpollServer::Connection::****insertRecvQueue**(recv_queue::queue_type &vRecvData)
{
**if**(!vRecvData.empty())
    {
//服务队列已超载  数据放在队列头部
**if**(_pBindAdapter->isOverload())
        {
recv_queue::queue_type::iterator it
 = vRecvData.begin();
recv_queue::queue_type::iterator itEnd
 = vRecvData.end();
**while**(it
 != itEnd)
            {
               (*it)->isOverload = **true**;
               ++it;
            }
_pBindAdapter->insertRecvQueue(vRecvData,**false**);
        }
**else**
        {
_pBindAdapter->insertRecvQueue(vRecvData);
        }
    }
}

Connection需要集中管理起来
所有Connection都会被ConnectionList一个连接队列管理起来。统一进行超时检测等，空连接超时检测等，连接队列存储的数据为
pair<Connection*, multimap<time_t, uint32_t>::iterator> list_data;
ConnectionList成员属性如下：
TC_EpollServer                 
 *_pEpollServer; //服务
uint32_t_total;
 //总计连接数
        list<uint32_t>                  _free;
 //空闲链表
size_t_free_size;
 //空闲链元素个数
list_data                      
 *_vConn; //链接
multimap<time_t, uint32_t>      _tl;
 //超时链表
time_t_lastTimeoutTime;
 //上次检查超时时间
uint32_t_iConnectionMagic;
 //连接ID的魔数

ConnectionList初始化，指定ConnectionList可以管理的最大连接数目，建立最大连接数目需要的空间，并标识为可用。
**void****TC_EpollServer::ConnectionList::init**(uint32_t size)
{
_lastTimeoutTime = TC_TimeProvider::*getInstance*()->getNow();
  //初始化检查超时时间
_total =
 size;
_free_size 
 = 0;  //空闲连接数目初始化为0
//初始化链接链表
**if**(_vConn) **delete**[] _vConn;
  //如果空间存在，则删除先。
//分配total+1个空间(多分配一个空间, 第一个空间其实无效)
_vConn = **new**list_data[_total+1];
  //分配最大连接数目需要的空间
_iConnectionMagic  
 = ((uint32_t)_lastTimeoutTime)
 << 20;  //连接ID的魔数
//free从1开始分配, 这个值为uid,
 0保留为管道用, epollwait根据0判断是否是管道消息 ，表示空间的连接数目
**for**(uint32_t i
 = 1; i <= _total; i++)
    {
_vConn[i].first = NULL;
_free.push_back(i);
        ++_free_size;
    }
}
获取唯一ID，从当前free的空闲连接资源中取得一个可用连接的ID（也就是链表下标），并将之从free中去掉，并且与魔数或运算生成唯一ID。分配ID，相当于分配资源了。这里的魔数是ConnectionList建立的时候的时间<<20的值(剩下的12位留出来给该ConnectionList的下标，表明这个Connection存储在data_list的哪个位置)，主要是用于校验当前的链接是否是该ConnectionList管理的资源，Connection的ID是否是这个ConnectionList分配出去的。
uint32_t**TC_EpollServer::ConnectionList::****getUniqId**()
{
TC_ThreadLock::Lock lock(***this**);
uint32_t uid
 = _free.front();
assert(uid > 0 && uid <= _total);
_free.pop_front();
    --_free_size;
**return**_iConnectionMagic |
 uid;
}
新增连接，首先锁住整个ConnectionList，获取连接的ID(该ID之前由该LIST生成)，获得当时分配资源的下标，并将至加载到指定链表数据的指定位置。并且将该connection放入到指定链表位置，同时将该connection的下标放入到超时管理的map中。
**void****TC_EpollServer::****ConnectionList****::add**(Connection *cPtr, time_t iTimeOutStamp)
{
TC_ThreadLock::Lock lock(***this**);
uint32_t muid
 = cPtr->getId();
uint32_t magi
 = muid & (0xFFFFFFFF << 20);
uint32_t uid 
 = muid & (0x7FFFFFFF >> 11);
assert(magi == _iConnectionMagic&&
 uid > 0 && uid <= _total && !_vConn[uid].first);
_vConn[uid]
 = make_pair(cPtr, _tl.insert(make_pair(iTimeOutStamp,
 uid)));
}

删除某个连接资源，根据指定的连接的ID,获得其在链表中的下标，从超时检测的map中移除他，从链表中找到该pair，删除connection资源，并且重新将他放到free可用中，同时更新freesize
**void****TC_EpollServer::ConnectionList::****del**(uint32_tuid)
{
TC_ThreadLock::Lock lock(***this**);
uint32_t magi
 = uid & (0xFFFFFFFF << 20);
    uid           = uid & (0x7FFFFFFF >> 11);
assert(magi == _iConnectionMagic&&
 uid > 0 && uid <= _total && _vConn[uid].first);
    _del(uid);
}
**void****TC_EpollServer::ConnectionList::_del**(uint32_t uid)
{
assert(uid > 0 && uid <= _total&& _vConn[uid].first);
_tl.erase(_vConn[uid].second);
**delete**_vConn[uid].first;
_vConn[uid].first = NULL;
_free.push_back(uid);
    ++_free_size;
}
超时检测，没秒检查一次，每次把超时最后检测时间更新一下，超时检测的时候需要锁住整个链表，
**void****TC_EpollServer::ConnectionList::checkTimeout**(time_t iCurTime)
{
//至少1s才能检查一次
**if**(iCurTime
 - _lastTimeoutTime < 1)
    {
**return**;
    }
_lastTimeoutTime =
 iCurTime;
TC_ThreadLock::Lock lock(***this**);
multimap<time_t, uint32_t>::iterator it
 = _tl.begin();
**while**(it
 != _tl.end())
    {
//已经检查到当前时间点了, 后续不用在检查了
**if**(it->first >
 iCurTime)
        {
**break**;
        }
uint32_t uid
 = it->second;
        ++it;
//udp的监听端口,
 不做处理
**if**(_vConn[uid].first->getListenfd()
 == -1 || _vConn[uid].first->getType()
 == Connection::*EM_BUS*)
        {
**continue**;
        }
//超时关闭
_pEpollServer->delConnection(_vConn[uid].first, **false**);
//从链表中删除
        _del(uid);
    }
**if**(_pEpollServer->IsEmptyConnCheck())
     {
           it = _tl.begin();
**while**(it
 != _tl.end())
           {
uint32_t uid
 = it->second;
//遍历所有的空连接
**if**(_vConn[uid].first->IsEmptyConn())
                {
//获取空连接的超时时间点
time_t iEmptyTimeout = (it->first - _vConn[uid].first->getTimeout())
 + (_pEpollServer->getEmptyConnTimeout()/1000);
//已经检查到当前时间点了, 后续不用在检查了
**if**(iEmptyTimeout
 > iCurTime)
                     {
**break**;
                     }
//udp的监听端口,
 不做处理
**if**(_vConn[uid].first->getListenfd()
 == -1 || _vConn[uid].first->getType()
 == Connection::*EM_BUS*)
                     {
                           ++it;
**continue**;
                     }
//超时关闭
_pEpollServer->delConnection(_vConn[uid].first, **false**);
//从链表中删除
                     _del(uid);
                }
                ++it;
           }
     }
}
