# Android下NFC驱动初始化过程的一点分析 - xqhrs232的专栏 - CSDN博客
2014年09月03日 15:52:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3964
相关文章
1、JiangBo-Android NFC分析----[http://wenku.baidu.com/link?url=tTD0vVVRFw_5EdcEB5Yycg21ZVuN5nu_ybnVukfU45RFCG-H0exETI5zHiACEFQwgq9Gvq3P9QWgWy_gTHCrMuchS0d8EgG0aNphHnJcnh7](http://wenku.baidu.com/link?url=tTD0vVVRFw_5EdcEB5Yycg21ZVuN5nu_ybnVukfU45RFCG-H0exETI5zHiACEFQwgq9Gvq3P9QWgWy_gTHCrMuchS0d8EgG0aNphHnJcnh7)
2、Android NFC分析----[http://leave001.blog.163.com/blog/static/16269129320122283648327/](http://leave001.blog.163.com/blog/static/16269129320122283648327/)
3、[NFC](http://blog.csdn.net/wangkaiblog/article/details/43085451)----[http://blog.csdn.net/wangkaiblog/article/details/43085451](http://blog.csdn.net/wangkaiblog/article/details/43085451)
4、[深入理解NFC](http://blog.csdn.net/wangkaiblog/article/details/42980281)----[http://blog.csdn.net/wangkaiblog/article/details/42980281](http://blog.csdn.net/wangkaiblog/article/details/42980281)
//基于JNI
//com_android_nfc_NativeNfcManager.cppe:\android\ubetter210\disc1\android4.0\android210-androidfs_v03\android210-androidfs_v03\packages\apps\nfc\Jni855402012-1-31
1、
int register_com_android_nfc_NativeNfcManager(JNIEnv *e)
{
    nfc_jni_native_monitor_t *nfc_jni_native_monitor;
   nfc_jni_native_monitor = nfc_jni_init_monitor();
   if(nfc_jni_native_monitor == NULL)
   {
      LOGE("NFC Manager cannot recover native monitor %x\n", errno);
      return -1;
   }
   return jniRegisterNativeMethods(e,
      "com/android/nfc/nxp/NativeNfcManager",
      gMethods, NELEM(gMethods));
}
2、
/*
 * JNI registration.
 */
static JNINativeMethod gMethods[] =
{
   {"doDownload", "()Z",
        (void *)com_android_nfc_NfcManager_doDownload},
   {"initializeNativeStructure", "()Z",
      (void *)com_android_nfc_NfcManager_init_native_struc},
   {"initialize", "()Z",
      (void *)com_android_nfc_NfcManager_initialize},
   {"deinitialize", "()Z",
      (void *)com_android_nfc_NfcManager_deinitialize},
   {"enableDiscovery", "()V",
      (void *)com_android_nfc_NfcManager_enableDiscovery},
   {"doGetSecureElementList", "()[I",
      (void *)com_android_nfc_NfcManager_doGetSecureElementList},
   {"doSelectSecureElement", "()V",
      (void *)com_android_nfc_NfcManager_doSelectSecureElement},
   {"doDeselectSecureElement", "()V",
      (void *)com_android_nfc_NfcManager_doDeselectSecureElement},
   {"doCheckLlcp", "()Z",
      (void *)com_android_nfc_NfcManager_doCheckLlcp},
   {"doActivateLlcp", "()Z",
      (void *)com_android_nfc_NfcManager_doActivateLlcp},
   {"doCreateLlcpConnectionlessSocket", "(I)Lcom/android/nfc/nxp/NativeLlcpConnectionlessSocket;",
      (void *)com_android_nfc_NfcManager_doCreateLlcpConnectionlessSocket},
   {"doCreateLlcpServiceSocket", "(ILjava/lang/String;III)Lcom/android/nfc/nxp/NativeLlcpServiceSocket;",
      (void *)com_android_nfc_NfcManager_doCreateLlcpServiceSocket},
   {"doCreateLlcpSocket", "(IIII)Lcom/android/nfc/nxp/NativeLlcpSocket;",
      (void *)com_android_nfc_NfcManager_doCreateLlcpSocket},
   {"doGetLastError", "()I",
      (void *)com_android_nfc_NfcManager_doGetLastError},
   {"disableDiscovery", "()V",
      (void *)com_android_nfc_NfcManager_disableDiscovery},
   {"doSetTimeout", "(II)Z",
      (void *)com_android_nfc_NfcManager_doSetTimeout},
   {"doGetTimeout", "(I)I",
      (void *)com_android_nfc_NfcManager_doGetTimeout},
   {"doResetTimeouts", "()V",
      (void *)com_android_nfc_NfcManager_doResetTimeouts},
   {"doAbort", "()V",
      (void *)com_android_nfc_NfcManager_doAbort},
   {"doDump", "()Ljava/lang/String;",
      (void *)com_android_nfc_NfcManager_doDump},
};   
3、
/* Init/Deinit method */
static jboolean com_android_nfc_NfcManager_initialize(JNIEnv *e, jobject o)
{
   struct nfc_jni_native_data *nat = NULL;
   int init_result = JNI_FALSE;
#ifdef TNFC_EMULATOR_ONLY
   char value[PROPERTY_VALUE_MAX];
#endif
   jboolean result;
   CONCURRENCY_LOCK();
#ifdef TNFC_EMULATOR_ONLY
   if (!property_get("ro.kernel.qemu", value, 0))
   {
      LOGE("NFC Initialization failed: not running in an emulator\n");
      goto clean_and_return;
   }
#endif
   /* Retrieve native structure address */
   nat = nfc_jni_get_nat(e, o);
   nat->seId = SMX_SECURE_ELEMENT_ID;
   nat->lto = 150;  // LLCP_LTO
   nat->miu = 128; // LLCP_MIU
   nat->wks = 1;  // LLCP_WKS
   nat->opt = 0;  // LLCP_OPT
   nat->discovery_cfg.PollDevInfo.PollCfgInfo.EnableIso14443A = TRUE;
   nat->discovery_cfg.PollDevInfo.PollCfgInfo.EnableIso14443B = TRUE;
   nat->discovery_cfg.PollDevInfo.PollCfgInfo.EnableFelica212 = TRUE;
   nat->discovery_cfg.PollDevInfo.PollCfgInfo.EnableFelica424 = TRUE;
   nat->discovery_cfg.PollDevInfo.PollCfgInfo.EnableIso15693 = TRUE;
   nat->discovery_cfg.PollDevInfo.PollCfgInfo.EnableNfcActive = TRUE;
   nat->discovery_cfg.PollDevInfo.PollCfgInfo.DisableCardEmulation = FALSE;
   nat->registry_info.MifareUL = TRUE;
   nat->registry_info.MifareStd = TRUE;
   nat->registry_info.ISO14443_4A = TRUE;
   nat->registry_info.ISO14443_4B = TRUE;
   nat->registry_info.Jewel = TRUE;
   nat->registry_info.Felica = TRUE;
   nat->registry_info.NFC = TRUE;
   nat->registry_info.ISO15693 = TRUE;
   exported_nat = nat;
   /* Perform the initialization */
   init_result = nfc_jni_initialize(nat);
clean_and_return:
   CONCURRENCY_UNLOCK();
   /* Convert the result and return */
   return (init_result==TRUE)?JNI_TRUE:JNI_FALSE;
}
4、
/* Initialization function */
static int nfc_jni_initialize(struct nfc_jni_native_data *nat) {
   struct timespec ts;
   uint8_t resp[16];
   NFCSTATUS status;
   phLibNfc_StackCapabilities_t caps;
   phLibNfc_SE_List_t SE_List[PHLIBNFC_MAXNO_OF_SE];
   uint8_t i, No_SE = PHLIBNFC_MAXNO_OF_SE, SmartMX_index = 0, SmartMX_detected = 0;
   phLibNfc_Llcp_sLinkParameters_t LlcpConfigInfo;
   struct nfc_jni_callback_data cb_data;
   uint8_t firmware_status;
   uint8_t update = TRUE;
   int result = JNI_FALSE;
   const hw_module_t* hw_module;
   nfc_pn544_device_t* pn544_dev = NULL;
   int ret = 0;
   LOGD("Start Initialization\n");
   /* Create the local semaphore */
   if (!nfc_cb_data_init(&cb_data, NULL))
   {
      goto clean_and_return;
   }
   /* Get EEPROM values and device port from product-specific settings */
   ret = hw_get_module(NFC_HARDWARE_MODULE_ID, &hw_module);
   if (ret) {
      LOGE("hw_get_module() failed.");
      goto clean_and_return;
   }
   ret = nfc_pn544_open(hw_module, &pn544_dev);
   if (ret) {
      LOGE("Could not open pn544 hw_module.");
      goto clean_and_return;
   }
   if (pn544_dev->num_eeprom_settings == 0 || pn544_dev->eeprom_settings == NULL) {
       LOGE("Could not load EEPROM settings");
       goto clean_and_return;
   }
   /* Reset device connected handle */
   device_connected_flag = 0;
   /* Reset stored handle */
   storedHandle = 0;
   /* Initialize Driver */
   if(!driverConfigured)
   {
       nfc_jni_configure_driver(nat);
   }
   /* ====== INITIALIZE ======= */
   TRACE("phLibNfc_Mgt_Initialize()");
   REENTRANCE_LOCK();
   status = phLibNfc_Mgt_Initialize(gHWRef, nfc_jni_init_callback, (void *)&cb_data);
   REENTRANCE_UNLOCK();
   if(status != NFCSTATUS_PENDING)
   {
      LOGE("phLibNfc_Mgt_Initialize() returned 0x%04x[%s]", status, nfc_jni_get_status_name(status));
      update = FALSE;
      goto force_download;
   }
   TRACE("phLibNfc_Mgt_Initialize returned 0x%04x[%s]", status, nfc_jni_get_status_name(status));
   /* Wait for callback response */
   if(sem_wait(&cb_data.sem))
   {
      LOGE("Failed to wait for semaphore (errno=0x%08x)", errno);
      goto clean_and_return;
   }
   /* Initialization Status */
   if(cb_data.status != NFCSTATUS_SUCCESS)
   {
      update = FALSE;
      goto force_download;
   }
   /* ====== CAPABILITIES ======= */
   REENTRANCE_LOCK();
   status = phLibNfc_Mgt_GetstackCapabilities(&caps, (void*)nat);
   REENTRANCE_UNLOCK();
   if (status != NFCSTATUS_SUCCESS)
   {
      LOGW("phLibNfc_Mgt_GetstackCapabilities returned 0x%04x[%s]", status, nfc_jni_get_status_name(status));
   }
   else
   {
       LOGD("NFC capabilities: HAL = %x, FW = %x, HW = %x, Model = %x, HCI = %x, Full_FW = %d, Rev = %d, FW Update Info = %d",
             caps.psDevCapabilities.hal_version,
             caps.psDevCapabilities.fw_version,
             caps.psDevCapabilities.hw_version,
             caps.psDevCapabilities.model_id,
             caps.psDevCapabilities.hci_version,
             caps.psDevCapabilities.full_version[NXP_FULL_VERSION_LEN-1],
             caps.psDevCapabilities.full_version[NXP_FULL_VERSION_LEN-2],
             caps.psDevCapabilities.firmware_update_info);
   }
   /* ====== FIRMWARE VERSION ======= */
   if(caps.psDevCapabilities.firmware_update_info)
   {
force_download:
       for (i=0; i<3; i++)
       {
           TRACE("Firmware version not UpToDate");
           status = nfc_jni_download(nat, update);
           if(status == NFCSTATUS_SUCCESS)
           {
               LOGI("Firmware update SUCCESS");
               break;
           }
           LOGW("Firmware update FAILED");
           update = FALSE;
       }
       if(i>=3)
       {
           LOGE("Unable to update firmware, giving up");
           goto clean_and_return;
       }
   }
   else
   {
       TRACE("Firmware version UpToDate");
   }
   /* ====== EEPROM SETTINGS ======= */
   // Update EEPROM settings
   TRACE("******  START EEPROM SETTINGS UPDATE ******");
   for (i = 0; i < pn544_dev->num_eeprom_settings; i++)
   {
      gInputParam.buffer = &(pn544_dev->eeprom_settings[i*4]);
      gInputParam.length = 0x04;
      gOutputParam.buffer = resp;
      TRACE("> EEPROM SETTING: %d", i);
      REENTRANCE_LOCK();
      status = phLibNfc_Mgt_IoCtl(gHWRef, NFC_MEM_WRITE, &gInputParam, &gOutputParam, nfc_jni_ioctl_callback, (void *)&cb_data);
      REENTRANCE_UNLOCK();
      if (status != NFCSTATUS_PENDING) {
         LOGE("phLibNfc_Mgt_IoCtl() returned 0x%04x[%s]", status, nfc_jni_get_status_name(status));
         goto clean_and_return;
      }
      /* Wait for callback response */
      if(sem_wait(&cb_data.sem))
      {
         LOGE("Failed to wait for semaphore (errno=0x%08x)", errno);
         goto clean_and_return;
      }
      /* Initialization Status */
      if (cb_data.status != NFCSTATUS_SUCCESS)
      {
         goto clean_and_return;
      }
   }
   TRACE("******  ALL EEPROM SETTINGS UPDATED  ******");
   /* ====== SECURE ELEMENTS ======= */
   REENTRANCE_LOCK();
   LOGD("phLibNfc_SE_GetSecureElementList()");
   status = phLibNfc_SE_GetSecureElementList(SE_List, &No_SE);
   REENTRANCE_UNLOCK();
   if (status != NFCSTATUS_SUCCESS)
   {
      LOGD("phLibNfc_SE_GetSecureElementList(): Error");
      goto clean_and_return;
   }
   LOGD("\n> Number of Secure Element(s) : %d\n", No_SE);
   /* Display Secure Element information */
   for (i = 0; i < No_SE; i++)
   {
      if (SE_List[i].eSE_Type == phLibNfc_SE_Type_SmartMX) {
         LOGD("phLibNfc_SE_GetSecureElementList(): SMX detected, handle=%p", (void*)SE_List[i].hSecureElement);
      } else if (SE_List[i].eSE_Type == phLibNfc_SE_Type_UICC) {
         LOGD("phLibNfc_SE_GetSecureElementList(): UICC detected, handle=%p", (void*)SE_List[i].hSecureElement);
      }
      /* Set SE mode - Off */
      REENTRANCE_LOCK();
      status = phLibNfc_SE_SetMode(SE_List[i].hSecureElement,
            phLibNfc_SE_ActModeOff, nfc_jni_se_set_mode_callback,
            (void *)&cb_data);
      REENTRANCE_UNLOCK();
      if (status != NFCSTATUS_PENDING)
      {
         LOGE("phLibNfc_SE_SetMode() returned 0x%04x[%s]", status,
               nfc_jni_get_status_name(status));
         goto clean_and_return;
      }
      LOGD("phLibNfc_SE_SetMode() returned 0x%04x[%s]", status,
            nfc_jni_get_status_name(status));
      /* Wait for callback response */
      if(sem_wait(&cb_data.sem))
      {
         LOGE("Failed to wait for semaphore (errno=0x%08x)", errno);
         goto clean_and_return;
      }
   }
   /* ====== LLCP ======= */
   /* LLCP Params */
   TRACE("******  NFC Config Mode NFCIP1 - LLCP ******");
   LlcpConfigInfo.miu    = nat->miu;
   LlcpConfigInfo.lto    = nat->lto;
   LlcpConfigInfo.wks    = nat->wks;
   LlcpConfigInfo.option = nat->opt;
   REENTRANCE_LOCK();
   status = phLibNfc_Mgt_SetLlcp_ConfigParams(&LlcpConfigInfo,
                                              nfc_jni_llcpcfg_callback,
                                              (void *)&cb_data);
   REENTRANCE_UNLOCK();
   if(status != NFCSTATUS_PENDING)
   {
      LOGE("phLibNfc_Mgt_SetLlcp_ConfigParams returned 0x%04x[%s]", status,
           nfc_jni_get_status_name(status));
      goto clean_and_return;
   }
   TRACE("phLibNfc_Mgt_SetLlcp_ConfigParams returned 0x%04x[%s]", status,
         nfc_jni_get_status_name(status));
   /* Wait for callback response */
   if(sem_wait(&cb_data.sem))
   {
      LOGE("Failed to wait for semaphore (errno=0x%08x)", errno);
      goto clean_and_return;
   }
   /* ===== DISCOVERY ==== */
   nat->discovery_cfg.NfcIP_Mode = get_p2p_mode();  //initiator
   nat->discovery_cfg.Duration = 300000; /* in ms */
   nat->discovery_cfg.NfcIP_Tgt_Disable = get_p2p_target_disable();
   /* Register for the card emulation mode */
   REENTRANCE_LOCK();
   ret = phLibNfc_SE_NtfRegister(nfc_jni_transaction_callback,(void *)nat);
   REENTRANCE_UNLOCK();
   if(ret != NFCSTATUS_SUCCESS)
   {
        LOGD("phLibNfc_SE_NtfRegister returned 0x%02x",ret);
        goto clean_and_return;
   }
   TRACE("phLibNfc_SE_NtfRegister returned 0x%x\n", ret);
   /* ====== END ======= */
   LOGI("NFC Initialized");
   result = TRUE;
clean_and_return:
   if (result != TRUE)
   {
      if(nat)
      {
         kill_client(nat);
      }
   }
   if (pn544_dev != NULL) {
       nfc_pn544_close(pn544_dev);
   }
   nfc_cb_data_deinit(&cb_data);
   return result;
}
5、
static int nfc_jni_configure_driver(struct nfc_jni_native_data *nat)
{
    char value[PROPERTY_VALUE_MAX];
    int result = FALSE;
    NFCSTATUS status;
    /* ====== CONFIGURE DRIVER ======= */
    /* Configure hardware link */
    gDrvCfg.nClientId = phDal4Nfc_msgget(0, 0600);
    TRACE("phLibNfc_Mgt_ConfigureDriver(0x%08x, 0x%08x)", gDrvCfg.nClientId);
    REENTRANCE_LOCK();
    status = phLibNfc_Mgt_ConfigureDriver(&gDrvCfg, &gHWRef);
    REENTRANCE_UNLOCK();
    if(status == NFCSTATUS_ALREADY_INITIALISED) {
           LOGW("phLibNfc_Mgt_ConfigureDriver() returned 0x%04x[%s]", status, nfc_jni_get_status_name(status));
    }
    else if(status != NFCSTATUS_SUCCESS)
    {
        LOGE("phLibNfc_Mgt_ConfigureDriver() returned 0x%04x[%s]", status, nfc_jni_get_status_name(status));
        goto clean_and_return;
    }
    TRACE("phLibNfc_Mgt_ConfigureDriver() returned 0x%04x[%s]", status, nfc_jni_get_status_name(status));
    if(pthread_create(&(nat->thread), NULL, nfc_jni_client_thread, nat) != 0)
    {
        LOGE("pthread_create failed");
        goto clean_and_return;
    }
    driverConfigured = TRUE;
clean_and_return:
    return result;
}
//phLibNfc.ce:\android\ubetter210\disc1\android4.0\android210-androidfs_v03\android210-androidfs_v03\external\libnfc-nxp\Src352792012-1-31
6、
/*
*************************** Function Definitions ******************************
*/
NFCSTATUS phLibNfc_Mgt_ConfigureDriver (pphLibNfc_sConfig_t     psConfig,
                                        void **                 ppDriverHandle)
{
    if(NULL != gpphLibContext)
    {
        return NFCSTATUS_ALREADY_INITIALISED;
    }   
    return phDal4Nfc_Config(psConfig, ppDriverHandle);
}
//phDal4Nfc.ce:\android\ubetter210\disc1\android4.0\android210-androidfs_v03\android210-androidfs_v03\external\libnfc-nxp\Linux_x86355332012-1-31
7、
/*-----------------------------------------------------------------------------
FUNCTION: phDal4Nfc_Config
PURPOSE: Configure the serial port.
-----------------------------------------------------------------------------*/
NFCSTATUS phDal4Nfc_Config(pphDal4Nfc_sConfig_t config,void **phwref)
{
   NFCSTATUS                       retstatus = NFCSTATUS_SUCCESS;
   const hw_module_t* hw_module;
   nfc_pn544_device_t* pn544_dev;
   uint8_t num_eeprom_settings;
   uint8_t* eeprom_settings;
   int ret;
   /* Retrieve the hw module from the Android NFC HAL */
   ret = hw_get_module(NFC_HARDWARE_MODULE_ID, &hw_module);
   if (ret) {
       LOGE("hw_get_module() failed");
       return NFCSTATUS_FAILED;
   }
   ret = nfc_pn544_open(hw_module, &pn544_dev);
   if (ret) {
       LOGE("Could not open pn544 hw_module");
       return NFCSTATUS_FAILED;
   }
   config->deviceNode = pn544_dev->device_node;
   if (config->deviceNode == NULL) {
       LOGE("deviceNode NULL");
       return NFCSTATUS_FAILED;
   }
   DAL_PRINT("phDal4Nfc_Config");
   if ((config == NULL) || (phwref == NULL))
      return NFCSTATUS_INVALID_PARAMETER;
   /* Register the link callbacks */
   memset(&gLinkFunc, 0, sizeof(phDal4Nfc_link_cbk_interface_t));
   switch(pn544_dev->linktype)
   {
      case PN544_LINK_TYPE_UART:
      case PN544_LINK_TYPE_USB:
      {
DAL_PRINT("UART link Config");
         /* Uart link interface */
         gLinkFunc.init               = phDal4Nfc_uart_initialize;
         gLinkFunc.open_from_handle   = phDal4Nfc_uart_set_open_from_handle;
         gLinkFunc.is_opened          = phDal4Nfc_uart_is_opened;
         gLinkFunc.flush              = phDal4Nfc_uart_flush;
         gLinkFunc.close              = phDal4Nfc_uart_close;
         gLinkFunc.open_and_configure = phDal4Nfc_uart_open_and_configure;
         gLinkFunc.read               = phDal4Nfc_uart_read;
         gLinkFunc.write              = phDal4Nfc_uart_write;
         gLinkFunc.reset              = phDal4Nfc_uart_reset;
      }
      break;
      case PN544_LINK_TYPE_I2C:
      {
DAL_PRINT("I2C link Config");
         /* i2c link interface */
         gLinkFunc.init               = phDal4Nfc_i2c_initialize;
         gLinkFunc.open_from_handle   = phDal4Nfc_i2c_set_open_from_handle;
         gLinkFunc.is_opened          = phDal4Nfc_i2c_is_opened;
         gLinkFunc.flush              = phDal4Nfc_i2c_flush;
         gLinkFunc.close              = phDal4Nfc_i2c_close;
         gLinkFunc.open_and_configure = phDal4Nfc_i2c_open_and_configure;
         gLinkFunc.read               = phDal4Nfc_i2c_read;
         gLinkFunc.write              = phDal4Nfc_i2c_write;
         gLinkFunc.reset              = phDal4Nfc_i2c_reset;
         break;
      }
      default:
      {
         /* Shound not happen : Bad parameter */
         return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_PARAMETER);
      }
   }
   gLinkFunc.init(); /* So that link interface can initialize its internal state */
   retstatus = gLinkFunc.open_and_configure(config, phwref);
   if (retstatus != NFCSTATUS_SUCCESS)
      return retstatus;
   /* Iniatilize the DAL context */
   (void)memset(&gDalContext,0,sizeof(phDal4Nfc_SContext_t));
   pgDalContext = &gDalContext;
   /* Reset the Reader Thread values to NULL */
   memset((void *)&gReadWriteContext,0,sizeof(gReadWriteContext));
   gReadWriteContext.nReadThreadAlive     = TRUE;
   gReadWriteContext.nWriteBusy = FALSE;
   gReadWriteContext.nWaitingOnWrite = FALSE;
   /* Prepare the message queue for the defered calls */
#ifdef USE_MQ_MESSAGE_QUEUE
   nDeferedCallMessageQueueId = mq_open(MQ_NAME_IDENTIFIER, O_CREAT|O_RDWR, 0666, &MQ_QUEUE_ATTRIBUTES);
#else
   nDeferedCallMessageQueueId = config->nClientId;
#endif
   gDalContext.pDev = pn544_dev;
   /* Start Read and Write Threads */
   if(NFCSTATUS_SUCCESS != phDal4Nfc_StartThreads())
   {
      return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
   }
   gDalContext.hw_valid = TRUE;
   phDal4Nfc_Reset(1);
   phDal4Nfc_Reset(0);
   phDal4Nfc_Reset(1);
   return NFCSTATUS_SUCCESS;
}
int hw_get_module(const char *id, const struct hw_module_t **module)
{
    return hw_get_module_by_class(id, NULL, module);
}
static inline int nfc_pn544_open(const struct hw_module_t* module,
        nfc_pn544_device_t** dev) {
    return module->methods->open(module, NFC_PN544_CONTROLLER,
        (struct hw_device_t**) dev);
}
/*
 * Generic device handling
 */
static int nfc_open(const hw_module_t* module, const char* name,
        hw_device_t** device) {
    if (strcmp(name, NFC_PN544_CONTROLLER) == 0) {
        nfc_pn544_device_t *dev = calloc(1, sizeof(nfc_pn544_device_t));
        dev->common.tag = HARDWARE_DEVICE_TAG;
        dev->common.version = 0;
        dev->common.module = (struct hw_module_t*) module;
        dev->common.close = pn544_close;
        dev->num_eeprom_settings = sizeof(pn544_eedata_settings) / 4;
        dev->eeprom_settings = (uint8_t*)pn544_eedata_settings;
        dev->linktype = PN544_LINK_TYPE_UART;
        dev->device_node = "/dev/ttyO3";
        dev->enable_i2c_workaround = 0;
        *device = (hw_device_t*) dev;
        return 0;
    } else {
        return -EINVAL;
    }
}
static struct hw_module_methods_t nfc_module_methods = {
    .open = nfc_open,
};
//phDal4Nfc_messageQueueLib.he:\android\ubetter210\disc1\android4.0\android210-androidfs_v03\android210-androidfs_v03\external\libnfc-nxp\Src14372012-1-31
8、
/*-----------------------------------------------------------------------------
FUNCTION: phDal4Nfc_uart_close
PURPOSE:  Closes the link
-----------------------------------------------------------------------------*/
NFCSTATUS phDal4Nfc_uart_open_and_configure(pphDal4Nfc_sConfig_t pConfig, void ** pLinkHandle)
{
   int          nComStatus;
   NFCSTATUS    nfcret = NFCSTATUS_SUCCESS;
   int          ret;
   DAL_ASSERT_STR(gComPortContext.nOpened==0, "Trying to open but already done!");
   srand(time(NULL));
   /* open communication port handle */
   gComPortContext.nHandle = open(pConfig->deviceNode, O_RDWR | O_NOCTTY);
   if (gComPortContext.nHandle < 0)
   {
      *pLinkHandle = NULL;
      return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
   }
   gComPortContext.nOpened = 1;
   *pLinkHandle = (void*)gComPortContext.nHandle;
   /*
    *  Now configure the com port
    */
   ret = tcgetattr(gComPortContext.nHandle, &gComPortContext.nIoConfigBackup); /* save the old io config */
   if (ret == -1)
   {
      /* tcgetattr failed -- it is likely that the provided port is invalid */
      *pLinkHandle = NULL;
      return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
   }
   ret = fcntl(gComPortContext.nHandle, F_SETFL, 0); /* Makes the read blocking (default).  */
   DAL_ASSERT_STR(ret != -1, "fcntl failed");
   /* Configures the io */
   memset((void *)&gComPortContext.nIoConfig, (int)0, (size_t)sizeof(struct termios));
   /*
    BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
    CRTSCTS : output hardware flow control (only used if the cable has
              all necessary lines. See sect. 7 of Serial-HOWTO)
    CS8     : 8n1 (8bit,no parity,1 stopbit)
    CLOCAL  : local connection, no modem contol
    CREAD   : enable receiving characters
   */
   gComPortContext.nIoConfig.c_cflag = DAL_BAUD_RATE | CS8 | CLOCAL | CREAD;  /* Control mode flags */
   gComPortContext.nIoConfig.c_iflag = IGNPAR;                                          /* Input   mode flags : IGNPAR  Ignore parity errors */
   gComPortContext.nIoConfig.c_oflag = 0;                                               /* Output  mode flags */
   gComPortContext.nIoConfig.c_lflag = 0;                                               /* Local   mode flags. Read mode : non canonical, no echo */
   gComPortContext.nIoConfig.c_cc[VTIME] = 0;                                           /* Control characters. No inter-character timer */
   gComPortContext.nIoConfig.c_cc[VMIN]  = 1;                                           /* Control characters. Read is blocking until X characters are read */
   /*
      TCSANOW  Make changes now without waiting for data to complete
      TCSADRAIN   Wait until everything has been transmitted
      TCSAFLUSH   Flush input and output buffers and make the change
   */
   ret = tcsetattr(gComPortContext.nHandle, TCSANOW, &gComPortContext.nIoConfig);
   DAL_ASSERT_STR(ret != -1, "tcsetattr failed");
   /*
      On linux the DTR signal is set by default. That causes a problem for pn544 chip
      because this signal is connected to "reset". So we clear it. (on windows it is cleared by default).
   */
   ret = ioctl(gComPortContext.nHandle, TIOCMGET, &nComStatus);
   DAL_ASSERT_STR(ret != -1, "ioctl TIOCMGET failed");
   nComStatus &= ~TIOCM_DTR;
   ret = ioctl(gComPortContext.nHandle, TIOCMSET, &nComStatus);
   DAL_ASSERT_STR(ret != -1, "ioctl TIOCMSET failed");
   DAL_DEBUG("Com port status=%d\n", nComStatus);
   usleep(10000); /* Mandatory sleep so that the DTR line is ready before continuing */
   return nfcret;
}
