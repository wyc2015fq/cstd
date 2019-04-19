# Android VOIP拨打电话机制分析 - 三少GG - CSDN博客
2013年03月20日 23:48:55[三少GG](https://me.csdn.net/scut1135)阅读数：2780
【转载自】http://blog.csdn.net/jindegegesun/article/details/8673514
### [CSipSimple拨打电话机制分析](http://blog.csdn.net/jindegegesun/article/details/8673514)
CSipSimple是运行在android设备上的一个开源的sip协议应用程序，本文其中的拨打电话机制进行大致分析。
项目中，拨打电话利用了AIDL方法来实现。aidl是 Android Interface definition language的缩写，它是一种android内部进程通信接口的描述语言，通过它来定义进程间的通信接口，完成IPC（Inter-Process Communication，进程间通信）。
创建.aidl文件
![](http://pic002.cnblogs.com/images/2012/349223/2012071008561074.png)
ISipService.aidl内容如下：
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- /**
-  * Copyright (C) 2010-2012 Regis Montoya (aka r3gis - www.r3gis.fr)
-  * This file is part of CSipSimple.
-  *
-  *  CSipSimple is free software: you can redistribute it and/or modify
-  *  it under the terms of the GNU General Public License as published by
-  *  the Free Software Foundation, either version 3 of the License, or
-  *  (at your option) any later version.
-  *  If you own a pjsip commercial license you can also redistribute it
-  *  and/or modify it under the terms of the GNU Lesser General Public License
-  *  as an android library.
-  *
-  *  CSipSimple is distributed in the hope that it will be useful,
-  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
-  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-  *  GNU General Public License for more details.
-  *
-  *  You should have received a copy of the GNU General Public License
-  *  along with CSipSimple.  If not, see <http://www.gnu.org/licenses/>.
-  *  
-  *  This file and this file only is also released under Apache license as an API file
-  */
- package com.csipsimple.api;  
- import com.csipsimple.api.SipProfileState;  
- import com.csipsimple.api.SipCallSession;  
- import com.csipsimple.api.MediaState;  
- 
- interface ISipService{  
- /**
-     * Get the current API version
-     * @return version number. 1000 x major version + minor version
-     * Each major version must be compatible with all versions of the same major version
-     */
- 
- .........  
- void makeCallWithOptions(in String callee, int accountId, in Bundle options);  
- }  
ISipService.aidl中定义了包含makeCallWithOptions
方法的接口ISipService。
自动编译生成java文件
eclipse中的ADT插件会自动在aidl文件中声明的包名目录下生成java文件，如下图所示：
![](http://pic002.cnblogs.com/images/2012/349223/2012071009025992.png)
ISipService.java
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- package com.csipsimple.api;  
- publicinterface ISipService extends android.os.IInterface  
- {  
- ……  
- //Place a call
- 
- ublic void makeCallWithOptions(java.lang.String callee, int accountId, android.os.Bundle options) throws android.os.RemoteException;  
- }  
接下来就是实现ISipService.aidl中定义的接口，提供接口的实例供客户端调用
### IPC实现
项目中拨打电话  
void com.csipsimple.api.ISipService.makeCallWithOptions(String msg, String toNumber, long accountId)
结合代码一层层看调用
目录：src\com\csipsimple\ui\dialpad
DialerFragment.java
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- private ISipService service;  
- private ServiceConnection connection = new ServiceConnection() {  
- 
- @Override
- publicvoid onServiceConnected(ComponentName arg0, IBinder arg1) {  
-         service = ISipService.Stub.asInterface(arg1);  
-      ........  
-     }  
- 
- @Override
- publicvoid onServiceDisconnected(ComponentName arg0) {  
-         service = null;  
-     }  
- };  
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- <span style="color:#333333;">   @Override
- publicvoid placeCall() {  
-         placeCallWithOption(null);  
-     }  
- 
- privatevoid placeCallWithOption(Bundle b) {  
- if (service == null) {  
- return;  
-         }  
-         String toCall = "";  
-         Long accountToUse = SipProfile.INVALID_ID;  
- // Find account to use
-         SipProfile acc = accountChooserButton.getSelectedAccount();  
- if (acc != null) {  
-             accountToUse = acc.id;  
-         }  
- // Find number to dial
- if(isDigit) {  
-             toCall = PhoneNumberUtils.stripSeparators(digits.getText().toString());  
-         }else {  
-             toCall = digits.getText().toString();  
-         }  
- 
- if (TextUtils.isEmpty(toCall)) {  
- return;  
-         }  
- 
- // Well we have now the fields, clear theses fields
-         digits.getText().clear();  
- 
- // -- MAKE THE CALL --//
- if (accountToUse >= 0) {  
- // It is a SIP account, try to call service for that
- try {  
-                </span><span style="color:#ff0000;"> service.makeCallWithOptions(toCall, accountToUse.intValue(), b);</span><span style="color:#333333;">  
-             } catch (RemoteException e) {  
-                 Log.e(THIS_FILE, "Service can't be called to make the call");  
-             }  
-         } elseif (accountToUse != SipProfile.INVALID_ID) {  
- // It's an external account, find correct external account
-             CallHandlerPlugin ch = new CallHandlerPlugin(getActivity());  
-             ch.loadFrom(accountToUse, toCall, new OnLoadListener() {  
- @Override
- publicvoid onLoad(CallHandlerPlugin ch) {  
-                     placePluginCall(ch);  
-                 }  
-             });  
-         }  
-     }  
-     </span>  
这里的调用需要先了解Service的机制
service.makeCallWithOptions(toCall, accountToUse.intValue(), b)
方法调用了ISipService的方法，找到它的代码如下：
目录：src\com\csipsimple\service
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- 2.服务端  
- SipService.java  
- /**
-   * 继承 Service发布服务
-   */
- publicclass SipService extends Service {  
-      ...  
- 
- // 为服务实现公共接口， Stub类继承了Binder
- privatefinal ISipService.Stub binder = new ISipService.Stub() {  
-         ...  
- @Override
- publicvoid makeCallWithOptions(final String callee, finalint accountId, final Bundle options)  
- throws RemoteException {  
-             SipService.this.enforceCallingOrSelfPermission(SipManager.PERMISSION_USE_SIP, null);  
- //We have to ensure service is properly started and not just binded
-             SipService.this.startService(new Intent(SipService.this, SipService.class));  
- 
- if(pjService == null) {  
-                 Log.e(THIS_FILE, "Can't place call if service not started");  
- // TODO - we should return a failing status here
- return;  
-             }  
- 
- if(!supportMultipleCalls) {  
- // Check if there is no ongoing calls if so drop this request by alerting user
-                 SipCallSession activeCall = pjService.getActiveCallInProgress();  
- if(activeCall != null) {  
- if(!CustomDistribution.forceNoMultipleCalls()) {  
-                         notifyUserOfMessage(R.string.not_configured_multiple_calls);  
-                     }  
- return;  
-                 }  
-             }  
-             getExecutor().execute(new SipRunnable() {  
- @Override
- protectedvoid doRun() throws SameThreadException {  
-                   <span style="color:#ff0000;">  pjService.makeCall(callee, accountId, options);  
-                 }</span>  
-             });  
-         }  
- 
/**
      * 返回一个实现了接口的类对象，给客户端接收
      */
     @Override
     public IBinder onBind(Intent intent) {
        String serviceName = intent.getAction();
        Log.d(THIS_FILE, "Action is " + serviceName );
        if (serviceName == null || serviceName.equalsIgnoreCase(SipManager.INTENT_SIP_SERVICE )) {
            Log.d(THIS_FILE, "Service returned");
            return binder ;
        } else if (serviceName. equalsIgnoreCase(SipManager.INTENT_SIP_CONFIGURATION )) {
            Log.d(THIS_FILE, "Conf returned");
            return binderConfiguration ;
        }
        Log.d(THIS_FILE, "Default service (SipService) returned");
        return binder;
     }
     ...
 }
上文说过，需要实现ISipService.aidl中定义的接口，来提供接口的实例供客户端调用。要实现自己的接口，就从ISipService.Stub类继承，然后实现相关的方法。
Stub类继承了Binder，因此它的对象就可以被远程的进程调用了。如果Service中有对象继承了Stub类，那么这个对象中的方法就可以在Activity等地方中使用，也就是说此时makeCallWithOptions
就可以被其他Activity访问调用了。
现在我们通过onBind(Intent intent)方法得到了可供客户端接收的IBinder对象，就可以回头看看刚才DialerFragment.java文件中的调用情况了。
在客户端（此处也就是调用远程服务的Activity）实现ServiceConnection,在ServiceConnection.onServiceConnected()方法中会接收到IBinder对象，调用ISipService.Stub.asInterface((IBinder)service)将返回值转换为ISipService类型。
语句
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- <span style="color:#ff0000;">service.makeCallWithOptions(toCall, accountToUse.intValue(), b);</span><span style="font-family: Verdana, Arial, Helvetica, sans-serif;">调用接口中的方法，完成IPC方法。</span>  
回到刚才的服务端实现，在继承Service发布服务的代码中，调用了 pjService.makeCall(callee, accountId, options)方法。
先看看这部分代码：
目录：src\com\csipsimple\pjsip
PjSipService.java
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- <span style="color:#333333;">publicint makeCall(String callee, int accountId, Bundle b) throws SameThreadException {  
- if (!created) {  
- return -1;  
-         }  
- 
- final ToCall toCall = sanitizeSipUri(callee, accountId);  
- if (toCall != null) {  
-             pj_str_t uri = pjsua.pj_str_copy(toCall.getCallee());  
- 
- // Nothing to do with this values
- byte[] userData = newbyte[1];  
- int[] callId = newint[1];  
-             pjsua_call_setting cs = new pjsua_call_setting();  
-             pjsua_msg_data msgData = new pjsua_msg_data();  
- int pjsuaAccId = toCall.getPjsipAccountId();  
- 
- // Call settings to add video
-             pjsua.call_setting_default(cs);  
-             cs.setAud_cnt(1);  
-             cs.setVid_cnt(0);  
- if(b != null && b.getBoolean(SipCallSession.OPT_CALL_VIDEO, false)) {  
-                 cs.setVid_cnt(1);  
-             }  
-             cs.setFlag(0);  
- 
-             pj_pool_t pool = pjsua.pool_create("call_tmp", 512, 512);  
- 
- // Msg data to add headers
-             pjsua.msg_data_init(msgData);  
-             pjsua.csipsimple_init_acc_msg_data(pool, pjsuaAccId, msgData);  
- if(b != null) {  
-                 Bundle extraHeaders = b.getBundle(SipCallSession.OPT_CALL_EXTRA_HEADERS);  
- if(extraHeaders != null) {  
- for(String key : extraHeaders.keySet()) {  
- try {  
-                             String value = extraHeaders.getString(key);  
- if(!TextUtils.isEmpty(value)) {  
- int res = pjsua.csipsimple_msg_data_add_string_hdr(pool, msgData, pjsua.pj_str_copy(key), pjsua.pj_str_copy(value));  
- if(res == pjsuaConstants.PJ_SUCCESS) {  
-                                     Log.e(THIS_FILE, "Failed to add Xtra hdr (" + key + " : " + value + ") probably not X- header");  
-                                 }  
-                             }  
-                         }catch(Exception e) {  
-                             Log.e(THIS_FILE, "Invalid header value for key : " + key);  
-                         }  
-                     }  
-                 }  
-             }  
- 
-             </span><span style="color:#ff0000;">int status = pjsua.call_make_call(pjsuaAccId, uri, cs, userData, msgData, callId);</span><span style="color:#333333;">  
- if(status == pjsuaConstants.PJ_SUCCESS) {  
-                 dtmfToAutoSend.put(callId[0], toCall.getDtmf());  
-                 Log.d(THIS_FILE, "DTMF - Store for " + callId[0] + " - "+toCall.getDtmf());  
-             }  
-             pjsua.pj_pool_release(pool);  
- return status;  
-         } else {  
-             service.notifyUserOfMessage(service.getString(R.string.invalid_sip_uri) + " : "
-                     + callee);  
-         }  
- return -1;  
-     }</span>  
由红色部分的语句，我们找到pjsua类。
目录：src\org\pjsip\pjsua
pjsua.java
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- package org.pjsip.pjsua;  
- 
- publicclass pjsua implements pjsuaConstants {  
- publicsynchronizedstaticint call_make_call(int acc_id, pj_str_t dst_uri, pjsua_call_setting opt, byte[] user_data, pjsua_msg_data msg_data, int[] p_call_id) {  
-    <span style="color:#ff0000;"> return pjsuaJNI.call_make_call</span>(acc_id, pj_str_t.getCPtr(dst_uri), dst_uri, pjsua_call_setting.getCPtr(opt), opt, user_data, pjsua_msg_data.getCPtr(msg_data), msg_data, p_call_id);  
-   }  
- ..........  
- }  
继续看调用，找到pjsuaJNI文件。
目录：src\org\pjsip\pjsua
pjsuaJNI.java
/* ----------------------------------------------------------------------------
  * This file was automatically generated by SWIG (http://www.swig.org).
  * Version 2.0.4
  *
  * Do not make changes to this file unless you know what you are doing--modify
  * the SWIG interface file instead.
  * ----------------------------------------------------------------------------- */
 package org.pjsip.pjsua;
 public class pjsuaJNI {
     ...
   public final static native int call_make_call(int jarg1, long jarg2, pj_str_t jarg2_, long jarg3, pjsua_call_setting jarg3_, byte[] jarg4, long jarg5, pjsua_msg_data jarg5_, int[] jarg6);
     ...
 }
我们看到了native方法call_make_call，它调用的是封装在库libpjsipjni.so中的函数pjsua_call_make_call，进一步可以在jni目录下找到C代码。
目录：jni\pjsip\sources\pjsip\src\pjsua-lib
pjsua_call.c
**[java]**[view
 plain](http://blog.csdn.net/jindegegesun/article/details/8673514#)[copy](http://blog.csdn.net/jindegegesun/article/details/8673514#)[print](http://blog.csdn.net/jindegegesun/article/details/8673514#)[?](http://blog.csdn.net/jindegegesun/article/details/8673514#)
- PJ_DEF(pj_status_t) pjsua_call_make_call(pjsua_acc_id acc_id,  
- const pj_str_t *dest_uri,  
- const pjsua_call_setting *opt,  
- void *user_data,  
- const pjsua_msg_data *msg_data,  
-                      pjsua_call_id *p_call_id)  
- {  
-     pj_pool_t *tmp_pool = NULL;  
-     pjsip_dialog *dlg = NULL;  
-     pjsua_acc *acc;  
-     pjsua_call *call;  
- int call_id = -1;  
-     pj_str_t contact;  
-     pj_status_t status;  
- 
- 
- /* Check that account is valid */
-     PJ_ASSERT_RETURN(acc_id>=0 || acc_id<(int)PJ_ARRAY_SIZE(pjsua_var.acc),   
-              PJ_EINVAL);  
- 
- /* Check arguments */
-     PJ_ASSERT_RETURN(dest_uri, PJ_EINVAL);  
- 
-     PJ_LOG(4,(THIS_FILE, "Making call with acc #%d to %.*s", acc_id,  
-           (int)dest_uri->slen, dest_uri->ptr));  
- 
-     pj_log_push_indent();  
- 
-     PJSUA_LOCK();  
- 
- /* Create sound port if none is instantiated, to check if sound device
-      * can be used. But only do this with the conference bridge, as with 
-      * audio switchboard (i.e. APS-Direct), we can only open the sound 
-      * device once the correct format has been known
-      */
- if (!pjsua_var.is_mswitch && pjsua_var.snd_port==NULL &&   
-     pjsua_var.null_snd==NULL && !pjsua_var.no_snd)   
-     {  
-     status = pjsua_set_snd_dev(pjsua_var.cap_dev, pjsua_var.play_dev);  
- if (status != PJ_SUCCESS)  
- goto on_error;  
-     }  
- 
-     acc = &pjsua_var.acc[acc_id];  
- if (!acc->valid) {  
-     pjsua_perror(THIS_FILE, "Unable to make call because account "
- "is not valid", PJ_EINVALIDOP);  
-     status = PJ_EINVALIDOP;  
- goto on_error;  
-     }  
- 
- /* Find free call slot. */
-     call_id = alloc_call_id();  
- 
- if (call_id == PJSUA_INVALID_ID) {  
-     pjsua_perror(THIS_FILE, "Error making call", PJ_ETOOMANY);  
-     status = PJ_ETOOMANY;  
- goto on_error;  
-     }  
- 
-     call = &pjsua_var.calls[call_id];  
- 
- /* Associate session with account */
-     call->acc_id = acc_id;  
-     call->call_hold_type = acc->cfg.call_hold_type;  
- 
- /* Apply call setting */
-     status = apply_call_setting(call, opt, NULL);  
- if (status != PJ_SUCCESS) {  
-     pjsua_perror(THIS_FILE, "Failed to apply call setting", status);  
- goto on_error;  
-     }  
- 
- /* Create temporary pool */
-     tmp_pool = pjsua_pool_create("tmpcall10", 512, 256);  
- 
- /* Verify that destination URI is valid before calling 
-      * pjsua_acc_create_uac_contact, or otherwise there  
-      * a misleading "Invalid Contact URI" error will be printed
-      * when pjsua_acc_create_uac_contact() fails.
-      */
- if (1) {  
-     pjsip_uri *uri;  
-     pj_str_t dup;  
- 
-     pj_strdup_with_null(tmp_pool, &dup, dest_uri);  
-     uri = pjsip_parse_uri(tmp_pool, dup.ptr, dup.slen, 0);  
- 
- if (uri == NULL) {  
-         pjsua_perror(THIS_FILE, "Unable to make call",   
-              PJSIP_EINVALIDREQURI);  
-         status = PJSIP_EINVALIDREQURI;  
- goto on_error;  
-     }  
-     }  
- 
- /* Mark call start time. */
-     pj_gettimeofday(&call->start_time);  
- 
- /* Reset first response time */
-     call->res_time.sec = 0;  
- 
- /* Create suitable Contact header unless a Contact header has been
-      * set in the account.
-      */
- if (acc->contact.slen) {  
-     contact = acc->contact;  
-     } else {  
-     status = pjsua_acc_create_uac_contact(tmp_pool, &contact,  
-                           acc_id, dest_uri);  
- if (status != PJ_SUCCESS) {  
-         pjsua_perror(THIS_FILE, "Unable to generate Contact header",   
-              status);  
- goto on_error;  
-     }  
-     }  
- 
- /* Create outgoing dialog: */
-     status = pjsip_dlg_create_uac( pjsip_ua_instance(),   
-                    &acc->cfg.id, &contact,  
-                    dest_uri, dest_uri, &dlg);  
- if (status != PJ_SUCCESS) {  
-     pjsua_perror(THIS_FILE, "Dialog creation failed", status);  
- goto on_error;  
-     }  
- 
- /* Increment the dialog's lock otherwise when invite session creation
-      * fails the dialog will be destroyed prematurely.
-      <span style="white-space:pre"> </span>*/
-     pjsip_dlg_inc_lock(dlg);  
- 
- if (acc->cfg.allow_via_rewrite && acc->via_addr.host.slen > 0)  
-         pjsip_dlg_set_via_sent_by(dlg, &acc->via_addr, acc->via_tp);  
- 
- /* Calculate call's secure level */
-     call->secure_level = get_secure_level(acc_id, dest_uri);  
- 
- /* Attach user data */
-     call->user_data = user_data;  
- 
- /* Store variables required for the callback after the async
-      * media transport creation is completed.
-      */
- if (msg_data) {  
-     call->async_call.call_var.out_call.msg_data = pjsua_msg_data_clone(  
-                                                           dlg->pool, msg_data);  
-     }  
-     call->async_call.dlg = dlg;  
- 
- /* Temporarily increment dialog session. Without this, dialog will be
-      * prematurely destroyed if dec_lock() is called on the dialog before
-      * the invite session is created.
-      */
-     pjsip_dlg_inc_session(dlg, &pjsua_var.mod);  
- 
- /* Init media channel */
-     status = pjsua_media_channel_init(call->index, PJSIP_ROLE_UAC,   
-                       call->secure_level, dlg->pool,  
-                       NULL, NULL, PJ_TRUE,  
-                                       &on_make_call_med_tp_complete);  
- if (status == PJ_SUCCESS) {  
-         status = on_make_call_med_tp_complete(call->index, NULL);  
- if (status != PJ_SUCCESS)  
- goto on_error;  
-     } elseif (status != PJ_EPENDING) {  
-     pjsua_perror(THIS_FILE, "Error initializing media channel", status);  
-         pjsip_dlg_dec_session(dlg, &pjsua_var.mod);  
- goto on_error;  
-     }  
- 
- /* Done. */
- 
- if (p_call_id)  
-     *p_call_id = call_id;  
- 
-     pjsip_dlg_dec_lock(dlg);  
-     pj_pool_release(tmp_pool);  
-     PJSUA_UNLOCK();  
- 
-     pj_log_pop_indent();  
- 
- return PJ_SUCCESS;  
- 
- 
- on_error:  
- if (dlg) {  
- /* This may destroy the dialog */
-     pjsip_dlg_dec_lock(dlg);  
-     }  
- 
- if (call_id != -1) {  
-     reset_call(call_id);  
-     pjsua_media_channel_deinit(call_id);  
-     }  
- 
- if (tmp_pool)  
-     pj_pool_release(tmp_pool);  
-     PJSUA_UNLOCK();  
- 
-     pj_log_pop_indent();  
- return status;  
- }  
- 通过本文的研究分析，我们了解到CSipSimple通过aidl方法实现进程间通信，从而实现了拨打电话功能。
