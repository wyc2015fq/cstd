
# Linux-2.6.25 TCPIP函数调用大致流程 - 高科的专栏 - CSDN博客

2011年12月02日 17:01:59[高科](https://me.csdn.net/pbymw8iwm)阅读数：834标签：[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[tcp																](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[dst																](https://so.csdn.net/so/search/s.do?q=dst&t=blog)[hook																](https://so.csdn.net/so/search/s.do?q=hook&t=blog)[action																](https://so.csdn.net/so/search/s.do?q=action&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=hook&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=dst&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=dst&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=input&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=input&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=output&t=blog)
Linux-2.6.25 TCPIP函数调用大致流程
学习目的，随手笔记。函数和文字说明会不断补充更新。
Changelog
2008.10.08    最近找工作忙。暂时缓缓
插口层
系统调用
send
sys_send
sys_sendto
sendto
sys_sendto
sock_sendmsg
sendmsg
sys_sendmsg
sock_sendmsg
write
sys_write
vfs_write
file->f_op->write = do_sync_write
filp->f_op->aio_write = sock_aio_write
do_sock_write
__sock_sendmsg
writev
sys_writev
vfs_writev
do_readv_writev
do_sync_readv_writev
sock_aio_write
do_sock_write
__sock_sendmsg
recv
sys_recv
sys_recvfrom
recvfrom
sys_recvfrom
sock_recvmsg
recvmsg
sys_recvmsg
sock_recvmsg
read
sys_read
vfs_read
file->f_op->read= do_sync_read
filp->f_op->aio_read= sock_aio_read
do_sock_read
__sock_recvmsg
readv
sys_readv
vfs_readv
do_readv_readv
do_sync_readv_readv
sock_aio_read
do_sock_read
__sock_recvmsg
socket
listen
connect
bind
select
close
shutdown
ioctl
getsockname
getpeername
setsockopt
getsockopt
内部实现函数
sock_sendmsg
__sock_sendmsg
__sock_sendmsg
sock->ops->sendmsg
对于TCP就是tcp_sendmsg，否则就是inet_sendmsg。后者调用sk->sk_prot->sendmsg，会继续分用为udp_sendmsg或raw_sendmsg函数
sock_recvmsg
__sock_recvmsg
__sock_recvmsg
sock->ops->recvmsg = sock_common_recvmsg
sock_common_recvmsg对于不同协议，是tcp_recvmsg，udp_sendmsg或raw_sendmsg函数。
运输层
TCP
系统调用sys_connect间接调用了tcp_v4_connect
tcp_v4_connect
ip_route_connect(寻找路由)
__ip_route_output_key
ip_route_output_flow★
tcp_connect(构造一个SYN并发送)
tcp_transmit_skb
inet_csk_reset_xmit_timer(启动一个超时定时器,等待SYN+ACK)

TCP的写函数最终都调用了tcp_sendmsg
tcp_sendmsg★
__tcp_push_appending_frames
tcp_write_xmit
tcp_transmit_skb
tcp_push_one
tcp_transmit_skb
tcp_push
__tcp_push_pending_frames
TCP发送数据共有三种途径__tcp_push_appending_frames,tcp_push_one,tcp_push，其中tcp_push调用了__tcp_push_pending_frames。到底调用哪个或哪些函数取决于是否有PUSH标志、NAGLE是否开启、和一些其他情况。
__tcp_push_appending_frames是试图一次发送完缓存队列中所有的skb。tcp_push_one先计算拥塞窗口，然后只发送窗口大小的数据，如果窗口大小为0，则不发送任何数据。

TCP实际的发送函数，tcp_transmit_skb
/* This routine actually transmits TCP packets queued in by
* tcp_do_sendmsg().  This is used by both the initial
* transmission and possible later retransmissions.
* All SKB's seen here are completely headerless.  It is our
* job to build the TCP header, and pass the packet down to
* IP so it can do the same plus pass the packet off to the
* device.
*
* We are working here with either a clone of the original
* SKB, or a fresh unique copy made by the retransmit engine.
*/
tcp_transmit_skb
build包头
icsk->icsk_af_ops->queue_xmit = ip_queue_xmit★

硬件->IP层->运输层收到数据，添加到对应的SOCKET缓冲区中，回复ACK
由ip_rcv间接调用
tcp_v4_rcv
__inet_lookup(根据一些参数，查找sock结构)
__inet_lookup_established(在已经建立的连接中找，通过inet_lhashfn在哈希表中查找)
__inet_lookup_listener(在监听中的Socket中找，通过inet_lhashfn在哈希表中查找)
tcp_v4_do_rcv
tcp_rcv_established(ESTABLISHED)★
tcp_child_process
tcp_rcv_state_process
tcp_rcv_state_process(除ESTABLISHED和TIME_WAIT之外)★
tcp_prequeue(见后面详细解释)
sk->sk_backlog_rcv = tcp_v4_do_rcv(又回到开头)
sk_add_backlog(见后面详细解释)
tcp_timewait_state_process(TIME_WAIT)
tcp_v4_timewait_ack(TIME_WAIT)
tcp_v4_send_ack(发送ACK)
sock结构被初始化的时候，发送和接收数据的缓冲队列也被初始化完成，接收数据用到以下三个队列:
sk->receive_queue
sk->prequeue
sk->sk_backlog
sk->prequeue:如果sk没有被用户态程序锁定,则先进入prequeue
sk->receive_queue:接收到数据包的sk_buff链表队列，如果数据包过多，造成receive_queue满，或者sock被用户程序锁定，将转入sk_backlog
sk->sk_backlog:当sock_owned_by_user函数返回真时候，(sk)->sk_lock.owner被锁定，使用sk_add_backlog()函数(该函数实现非常简单，只是一个为链表添加节点的动作)将SKB加入这个后备队列。

tcp_rcv_established
TCP接受里面最主要的就是tcp_rcv_established和tcp_rcv_state_process了
tcp_rcv_established★
if(fast path)
检查包头各字段
tcp_ack(处理CK)
tcp_data_snd_check(发送ACK)
__skb_pull(腾出空间)
__skb_queue_tail(把数据追加到接受缓冲区)
else(slow path)
tcp_data_queue
对滑动窗口、序号做出处理
__skb_pull
__skb_queue_tail
tcp_event_data_recv(更新状态)

tcp_rcv_state_process
TCP协议的状态机，状态转移函数。ESTABLISHED和TIME_WAIT状态之外的其他状态都会调用此函数
tcp_rcv_state_process★
icsk->icsk_af_ops->conn_request(是tcp_v4_conn_request，LISTEN状态)
tcp_v4_send_synack(发送SYN+ACK)
ip_build_and_send_pkt
ip_local_out
__ip_local_out
nf_hook(dst_output)
dst_output
tcp_rcv_synsent_state_process(SYN_SENT)
tcp_reset
tcp_ack(收到ACK)
tcp_set_state(SYN_RECV->ESTABLISHED或者FIN_WAIT1->FIN_WAIT2)
tcp_time_wait(CLOSING->TIME_WAIT)
tcp_update_metrics(LAST_ACK)
...(都是和TCP协议状态转移相关的东西，这里目的是打通上下，以后慢慢分析)

还有两个出镜率较高的函数tcp_v4_send_reset和tcp_v4_send_ack
tcp_v4_send_reset(发送RST)
ip_send_reply
ip_route_output_key
ip_push_pending_frames

tcp_v4_send_ack(发送ACK)
ip_send_reply
ip_route_output_key
ip_push_pending_frames

用户子上而下的读函数都间接的调用了tcp_recvmsg
tcp_recvmsg★
skb_copy_datagram_iovec
tcp_recv_urg(接受一个字节的URG数据)
UDP
UDP的写函数都调用了udp_sendmsg
udp_sendmsg★
ip_route_output_flow
ip_append_data
udp_flush_pending_frames
ip_flush_pending_frames
udp_push_pending_frames
ip_push_pending_frames
硬件->IP层->运输层收到数据，添加到对应的SOCKET缓冲区中
由ip_rcv间接调用
udp_rcv
__udp4_lib_rcv
if(是多播或广播)
__udp4_lib_mcast_deliver
udp_queue_rcv_skb(对每个需要接受的UDP SOCKET缓冲调用)
__udp4_lib_lookup
udp_queue_rcv_skb

把数据块sk_buff放到一个sock结构的接受缓存的末尾中
udp_queue_rcv_skb
sock_queue_rcv_skb
skb_queue_tail
用户子上而下的读函数都间接的调用了udp_recvmsg
udp_recvmsg★
__skb_recv_datagram
skb_copy_datagram_iovec
skb_copy_and_csum_datagram_iovec
原始套接字
RAW Socket的写函数都调用了raw_sendmsg
raw_sendmsg★
ip_route_output_flow
if(设置了IP_HDRINCL选项，即自己构造ip头部)
raw_send_hdrinc★
else
ip_append_data
ip_flush_pending_frames或
ip_push_pending_frames

自底向上的收包
raw_rcv
由ip_forward调用ip_call_ra_chain，然后再调用的raw_rcv
raw_rcv
sock_queue_rcv_skb
skb_queue_tail
sk->sk_data_ready = sock_def_readable
waitqueue_active
sk_wake_async

用户子上而下的读函数都间接的调用了raw_recvmsg
raw_recvmsg★
skb_recv_datagram
__skb_recv_datagram
wait_for_packet(如果没有数据，则调用此函数等待数据)

ICMP
在任何需要发送ICMP报文的时候都会调用此函数
icmp_send
__ip_route_output_key
ip_route_output_slow
ip_route_output_key
ip_route_output_flow
icmp_push_reply
ip_append_data
ip_flush_pending_frames或
ip_push_pending_frames

硬件->IP层->运输层收到ICMP数据，作出处理逻辑
由ip_rcv间接调用
icmp_rcv
完全就是icmp协议的处理逻辑，通过函数指针icmp_pointers[icmph->type].handler调用了一下函数中的某一个
icmp_discard
icmp_unreach
icmp_redirect
icmp_timestamp
icmp_address
icmp_address_reply
icmp_echo

网络层
IP发送
网络层中主要的发送函数有以下三个：ip_push_pending_frames，ip_queue_xmit，raw_send_hdrinc
ip_push_pending_frames★
将所有pending状态的IP分组组合成一个IP分组，并发送
ip_local_out

ip_queue_xmit★
ip_route_output_flow(找路由)
ip_local_out

raw_send_hdrinc★
NF_HOOK(dst_output)

ip_local_out★
__ip_local_out
nf_hook(dst_output)
dst_output
路由选择
ip_route_output_flow★
__ip_route_output_key
ip_route_output_slow
路由选择
ip_route_output_slow★
fib_lookup
ip_mkroute_output
__mkroute_output
rt_hash
rt_intern_hash
arp_bind_neighbour
__neigh_lookup_errno
neigh_lookup
neigh_create

dst_output★
dst->output = ip_output
NF_HOOK_COND(ip_finish_output)
dst_output
ip_fragment
ip_finish_output2
neigh_hh_output
hh->hh_output = dev_queue_xmit★
dst->neighbour->output = neigh_resolve_output
neigh->ops->queue_xmit = dev_queue_xmit★
IP接受
接收IPv4包，由netif_rx间接调用
ip_rcv★
NF_HOOK
ip_rcv_finish
ip_route_input
dst_input
dst->input(可能是ip_local_deliver或ip_forward)
if(是发给本地的包)
dst->input是ip_local_deliver
NF_HOOK
ip_local_deliver_finish
ipprot->handler(可能是tcp_v4_rcv,udp_rcv,icmp_rcv,igmp_rcv)
else
dst->input是ip_forward
更新路由
ip_route_input★
ip_route_input_mc(多播)
rt_hash
rt_intern_hash
ip_route_input_slow(其它)
ip_mkroute_input
__mkroute_input
rt_hash
rt_intern_hash
每收到一个IP报文都会调用此函数更新路由表。ip_route_input函数的上半部分是在hash table寻找路由项，如果找到就返回。找不到才会调用后面的ip_route_input_mc或ip_route_input_slow来更新路由表。

转发
ip_forward★
ip_call_ra_chain
raw_rcv★
xfrm4_route_forward(处理路由)
xfrm_route_forward
__xfrm_route_forward
xfrm_lookup
__xfrm_lookup
xfrm_find_bundle
afinfo->find_bundle = __xfrm4_find_bundle
xfrm_bundle_create
xfrm_dst_lookup
afinfo->dst_lookup = xfrm4_dst_lookup
__ip_route_output_key
ip_route_output_slow★
处理各个参数(在一定条件下发送ICMP)
ip_decrease_ttl(减少TTL)
NF_HOOK(ip_forward_finish)
dst_output

链路层
接收帧
由硬件驱动在中断处理程序中直接调用netif_rx
netif_rx★
if(netpoll_rx函数与把数据拿走)
return
__skb_queue_tail(把所有收到的数据保存起来)
napi_schedule
__napi_schedule
__raise_softirq_irqoff(NET_RX_SOFTIRQ);
在net_dev_init函数中初始化了软中断：
open_softirq(NET_TX_SOFTIRQ, net_tx_action, NULL);
open_softirq(NET_RX_SOFTIRQ, net_rx_action, NULL);
所以NET_RX_SOFTIRQ中断的处理函数是net_rx_action，NET_TX_SOFTIRQ中断的处理函数是net_tx_action。需要让上层接收数据时，只要触发相应的软中断，如__raise_softirq_irqoff(NET_RX_SOFTIRQ)。内核就会在适当时机执行do_softirq来处理pending的软中断。

net_rx_action★
n->poll = process_backlog
netif_receive_skb
pt_prev->func = ip_rcv★(在这里完成了交接)
__raise_softirq_irqoff(NET_RX_SOFTIRQ)

发送帧
dev_queue_xmit★
rcu_read_lock_bh
if(设备有发送队列)
q->enqueue(将数据追加到发送队列，软中断处理函数net_tx_action会执行真正的发送工作)
else
dev_hard_start_xmit
dev->hard_start_xmit = el_start_xmit★
调用outw汇编指令发送数据，够底层了
rcu_read_unlock_bh

net_tx_action★
__kfree_skb(释放已发送的，此时中断由dev_kfree_skb_irq函数发起)
qdisc_run
__qdisc_run
qdisc_restart
dev_hard_start_xmit★
netif_schedule
netif_schedule

netif_schedule★
__netif_schedule
raise_softirq_irqoff(NET_TX_SOFTIRQ)

