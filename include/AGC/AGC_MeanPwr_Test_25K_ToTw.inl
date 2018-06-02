// =========================================================================
//
//             利用平均能量的方式进行数字AGC的实现
//
// =========================================================================
//
//　(C) 2003-2010 广州海格通信集团股份有限公司
//   作  者：   王 云 飞
//   版  本：    V 1.0
//   创建日期： 2008年3月1日
//   完成日期： 2008年3月1日
//--------------------------------------------------------------------------
//  功能说明：
//     1. 利用func_dBToLevel和func_LevelTodB函数实现dB，level之间的转化
//     2. AGC的控制范围[ -54,54]dB, 共108dB, 控制在门限上下各0.1dB的范围内
//
//--------------------------------------------------------------------------

#include "cstd.h"
#include "math/rand.inl"
#include "img/loadmat.inl"
#include "func_dBToLevel.inl"
#include "func_LevelTodB.inl"
int AGC_MeanPwr_Test_25K_ToTw() {
  double *Mag_vec = NULL;
  sys_chdir("E:/code/cstd/AGC");
  
  //rand('state',0);
  //randn('state',0);
  rng_t rng[1];
  rng_mt19937_init(rng, 10);
  //-------------------------------------------------------------------------
  //  AGC仿真参数
  int AGC_SamplNum = 128;                     // 每128个点计算一次
  //   基本仿真参数
  int fs = 25000;                             // 采样率
  int fo_list[] = {1111}; // 信号频率
  int HoldTime = AGC_SamplNum*100;            // 保持时间，
  // // Mag_list = [10, 3, 8, 20, 5] ;      // 幅度向量
  // Mag_list = [20,4,10,3,3];
  //Mag_list = randint(1,10,300) + 1;
  int Mag_list[] = {286,70,183,146,268,229,137,6,247,134};
  int TotalLen = countof(Mag_list)*HoldTime;
  int i, j;
  int Mag_vec_len=HoldTime*countof(Mag_list);
  //Mag_vec = reshape(repmat(Mag_list, HoldTime,1),1,[]);    // 给信号变幅度
  MYREALLOC(Mag_vec, Mag_vec_len);
  for (i=0; i<Mag_vec_len; ++i) {
    Mag_vec[i] = Mag_list[i/HoldTime];
  }
  // 加入噪声的参数
  double Narrow_SNR_dB = 100;                  // 窄带噪声
  double SNR_dB = 100;                        // 宽带噪声
  int noise_len = 0;
  double BandWidth = 3000;                    // 带宽
  double r = 0.9;
  
  int ii=0;
  //*************************************************************************
  // 产生初始信号
  img_t im[20] = {0};
  int fo_list_len = countof(fo_list);
  img_t* sig_src_list = im+ii++;
  img_t* sig_src = im+ii++;
  img_t* sig_chan = im+ii++;
  img_t* tmp = im+ii++;
  img_t* AGC_out_Tmp = im+ii++;
  img_t* AGC_out = im+ii++;
  img_t* AvgMag_tmp_save = im+ii++;
  img_t* Record3 = im+ii++;
  img_t* Record4 = im+ii++;
  img_t* AvgMag_save = im+ii++;
  img_t* agc_delta_save = im+ii++;
  img_t* agc_dB_save = im+ii++;
  img_t* agc_level_save = im+ii++;

  //sig_src_list = zeros(length(fo_list),TotalLen);
  imsetsize_f8(sig_src_list, fo_list_len, TotalLen, 1);
  //for ii = 1:length(fo_list )
  //    sig_src_list(ii,:) = sin(2*pi*fo_list(ii)*(0:TotalLen - 1)/fs + rand(1,1)*2*pi);
  //end
  const double pi = M_PI;
  ii = 0;
  for (ii=0; ii<fo_list_len; ++ii) {
    for (i=0; i<TotalLen; ++i) {
      double t = rng_real3(rng);
      sig_src_list->tt.f8[ii*TotalLen] = sin(2*pi*fo_list[ii]*i/fs + t*2*pi);
    }
  }
  if (1) {
    loadmat_fromfile("sig_src_list.dat", sizeof(double), sig_src_list);
  }
  
  //sig_src = sum(sig_src_list,1)/length( fo_list);
  immean_f8(sig_src_list, OptCol, sig_src);
  // sig_src = 1 + r*cos(2*pi*1000*(0:TotalLen - 1)/16000);
  // sig_src = floor(sig_src.*2000*2.^8);
  // plot( sig_src)
  //sig_src = 420 * cos(2*pi*1000*(1:5000)/fs);//Rf-60dBm
  //sig_chan = [zeros(1,noise_len ),sig_src];
  imsetsize_f8(sig_src, 1, 5000, 1);
  imsetsize_f8(sig_chan, 1, 5000+noise_len, 1);
  for (i=0; i<sig_src->w; ++i) {
    sig_chan->tt.f8[i+noise_len]=sig_src->tt.f8[i] = 420 * cos(2*pi*1000*(i+1)/fs);
  }
  for (i=0; i<noise_len; ++i) {
    sig_chan->tt.f8[i] = 0;
  }
  // 初始信号进行噪声 通过信道加入噪声,AWGN信道，　假设上，下变频端都是理想的，信道的ＡＧＣ控制完全合适
  // imp = sum(abs( sig_src).^2)/length(sig_src );
  // h_narrow = fir1(1024, 2*BandWidth/fs ,'low',chebwin(1025,30));
  // narrow_noise = conv(h_narrow , wgn(1,length( sig_tx) - length(h_narrow )+1, -10*log10(h_narrow*h_narrow')- Narrow_SNR_dB,imp,'complex'));  // 经过滤波器后的噪声功率为之前的m倍，其中m为滤波器能量
  // sig_chan = sig_tx + floor(narrow_noise +  wgn(1,length( sig_tx), -SNR_dB,imp ,'complex'));    // 宽带噪声＋ 窄带噪声
  // sig_chan = sig_tx + wgn(1,length( sig_tx), -SNR_dB,imp ,'complex');   // 仅有宽带噪声
  // sig_chan = [sig_chan(1:31000),111*ones(1,1000),round(0.7*sig_chan(1:30000)),77*ones(1,2000),round(0.25*sig_chan(1:16000))];
  //
  //sig_chan = [1*sig_chan,zeros(1,1000),1*sig_chan,zeros(1,1000),1*sig_chan,zeros(1,1000),1*sig_chan,zeros(1,1000)];
  {
    int old_w = sig_chan->w;
    int old_w_1000 = old_w+1000;
    imclone2(sig_chan, tmp);
    imsetsize_f8(sig_chan, 1, old_w_1000*4, 1);
    for (i=0; i<old_w; ++i) {
      double t = tmp->tt.f8[i];
      sig_chan->tt.f8[old_w_1000*0 + i] = t;
      sig_chan->tt.f8[old_w_1000*1 + i] = t;
      sig_chan->tt.f8[old_w_1000*2 + i] = t;
      sig_chan->tt.f8[old_w_1000*3 + i] = t;
    }
    for (i=old_w; i<old_w_1000; ++i) {
      sig_chan->tt.f8[old_w_1000*0 + i] = 0.;
      sig_chan->tt.f8[old_w_1000*1 + i] = 0.;
      sig_chan->tt.f8[old_w_1000*2 + i] = 0.;
      sig_chan->tt.f8[old_w_1000*3 + i] = 0.;
    }
  }
  //***************AGC 控制**********************************************************
  //　进行ＡＧＣ控制
  // 首先进行平均能量计算， 每128个点计算一次，最少可以对200Hz以内的单音信号采取到一个完整的周期
  // sig_chan = sig_tx;    // 无噪声的情况；
  //AGC_TotalNum = ceil( length( sig_chan)/AGC_SamplNum);
  int AGC_TotalNum = ceil( sig_chan->w/AGC_SamplNum);
  //sig_chan = [sig_chan,zeros(1,AGC_TotalNum*AGC_SamplNum - length(sig_chan ))];
  {
    int old_w = sig_chan->w;
    int len = AGC_TotalNum*AGC_SamplNum;
    imclone2(sig_chan, tmp);
    imsetsize_f8(sig_chan, 1, len, 1);
    for (i=0; i<old_w; ++i) {
      sig_chan->tt.f8[i] = tmp->tt.f8[i];
    }
  }
#define round(x)  (int)(x+0.5)
  //figure;plot(real(sig_chan));
  // AGC控制部分的初始化
  double agc_dB = 0;
  double DB_Threoshold = 64*64;                  // 稳定输出的门限值
  //sig_chan = real(round(sig_chan));              // 只考虑实信号
  for (i=0; i<sig_chan->w; ++i) {
    sig_chan->tt.f8[i] = round(sig_chan->tt.f8[i]);
  }
  // figure;plot(sig_chan);
  double x_d1 = 0, x_d2 = 0, y_d1 = 0, y_d2 = 0;    // iir滤波相关参数的初始化
  //------butter(2,0.1)-------
  int iir_b[] = { 329,   658,   329 };
  int iir_a[] = {16384, -25576, 10507};  // 2.^14放大
  
  //RecordSlotPower = zeros(1,8);
  double RecordSlotPower[8] = {0};
  int g_s08_0dBSnr = -113;                                        // 信噪比0dB时的射频场强
  int g_s16_RxRfAgc = 0;                                          // 射频上报场强dB
  int BaseNoisePwr = 30;                                          // 电台底噪值
  int RfThreshold = -100;                                         // 静音门限，电平门限
  int PwrBackLevdB = 5;                                           // 功率回滞
  int VadState = 0;                                               // 静音开关，0表示输出静音，1表示正常输出

  imsetsize_f8(AGC_out_Tmp, 1, AGC_SamplNum, 1);
  imsetsize_f8(AGC_out, 1, AGC_SamplNum*AGC_TotalNum, 1);
  imsetsize_f8(AvgMag_tmp_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(Record3, 1, AGC_TotalNum, 1);
  imsetsize_f8(Record4, 1, AGC_TotalNum, 1);
  imsetsize_f8(AvgMag_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(agc_delta_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(agc_dB_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(agc_level_save, 1, AGC_TotalNum, 1);
for (ii = 0; ii<AGC_TotalNum; ++ii) {
  //****************************************
  double Agc_Level = 0;
  double agc_dB_abs = 0;
  //func_dBToLevel(abs(agc_dB));     // 调用函数func_dBToLevel计算agc_dB对应的的放大倍数
  agc_dB_abs = fabs(agc_dB);
  func_dBToLevel(&agc_dB, 1, &Agc_Level);
  
  // 根据agc_dB的符号对输入信号进行放大或者衰减
  for (i=0; i<AGC_SamplNum;++i) {
    const double* xx = sig_chan->tt.f8 + ii*AGC_SamplNum;
    if (agc_dB > 0) {      //  agc_dB为正值表示对信号进行放大
      //AGC_out_Tmp->tt.f8[i] = floor( Agc_Level* sig_chan((ii -1)*AGC_SamplNum + 1 : ii*AGC_SamplNum)/2.^6);
      AGC_out_Tmp->tt.f8[i] = floor( pow(Agc_Level * xx[i]/2., 6));
    } else {               //  agc_dB为负值表示对信号进行衰减
      //AGC_out_Tmp->tt.f8[i] = floor( 2.^6* sig_chan((ii -1)*AGC_SamplNum + 1 : ii*AGC_SamplNum)/Agc_Level);
      AGC_out_Tmp->tt.f8[i] = floor( pow(2., 6) * xx[i]/Agc_Level);
    }
  }

  //AGC_out_Tmp = sign(AGC_out_Tmp).*min(abs(AGC_out_Tmp),32767);     // 输出结果以16bit存储，截顶
    for (i=0; i<AGC_out_Tmp->h*AGC_out_Tmp->w;++i) {
      double t = AGC_out_Tmp->tt.f8[i];
      AGC_out_Tmp->tt.f8[i] = CV_SIGN(t) * MIN(fabs(t),32767);
    }
  
    // AGC输出信号AGC_out
    //AGC_out((ii -1)*AGC_SamplNum + 1 : ii*AGC_SamplNum) = AGC_out_Tmp;
    for (j=0; j<AGC_out_Tmp->h; ++j) {
      for (i=0; i<AGC_out_Tmp->w; ++i) {
        AGC_out->tt.f8[j*AGC_out->w + ii*AGC_SamplNum + i] = AGC_out_Tmp->tt.f8[j*AGC_out_Tmp->w + i];
      }
    }
    //*** iir 滤波处理*************************
    // 首先进行平均幅度的计算，
    //AvgMag_tmp = floor(min(sqrt(sum(abs(AGC_out_Tmp ).^2)/AGC_SamplNum) ,32767));  // 计算平均能量，开方得到平均幅度
    double s=0;
    for (j=0; j<AGC_out_Tmp->h; ++j) {
      for (i=0; i<AGC_out_Tmp->w; ++i) {
        double t = AGC_out_Tmp->tt.f8[i];
         s += t*t;
      }
    }
    double AvgMag_tmp = sqrt(s/AGC_SamplNum);
    AvgMag_tmp = floor(MIN(AvgMag_tmp, 32767));
    //AvgMag_tmp_save(ii) = AvgMag_tmp;
    AvgMag_tmp_save->tt.f8[ii] = AvgMag_tmp;
    
    ////////////////////////////%20150611 by chenliang%////////////////////////////////////////////
    //RecordSlotPower = [AvgMag_tmp,RecordSlotPower(1:end-1)];              // 记录最近的信号能量FIFO
    MEMMOVE(RecordSlotPower+1, RecordSlotPower, countof(RecordSlotPower)-1);
    RecordSlotPower[0] = AvgMag_tmp;
    //[MaxFramePower,MaxPos] = max(RecordSlotPower);                          // 采用最大能量,防止爆音
    double MaxFramePower = RecordSlotPower[0];
    int MaxPos = 0;
    for (i=1; i<countof(RecordSlotPower); ++i) {
      if (RecordSlotPower[i]>MaxFramePower) {
        MaxFramePower = RecordSlotPower[i];
        MaxPos = i;
      }
    }

    double NoisePwr=0, SigPwr=0, RfPwr=0;
    // 计算场强,采用MaxFramePower或AvgFramePower
    if(AvgMag_tmp>BaseNoisePwr*2) {
      NoisePwr = log10(BaseNoisePwr);
      SigPwr = log10(MaxFramePower-BaseNoisePwr);//求信噪比
      RfPwr = (SigPwr - NoisePwr)*10 + g_s08_0dBSnr + g_s16_RxRfAgc;
      if(RfPwr<-121) {
        RfPwr = -121;
      }
    } else {
      RfPwr = -121;
    }
    // 输出静音状态
    double VadOnThd = RfThreshold;
    double VadOffThd = RfThreshold-PwrBackLevdB;
    if(VadState==1) {
      if(RfPwr<VadOffThd) {
        VadState = 0;//输出为0
      }
    } else {
      if(RfPwr>VadOnThd) {
        VadState = 1;//正常输出
      }
    }

    Record3->tt.f8[ii] = RfPwr;
    Record4->tt.f8[ii] = MaxFramePower;
    // %20150611 by chenliang%
    
    // 通过IIR滤波器 滤除交流部分
    //AvgMag = round((AvgMag_tmp*iir_b(1) + x_d1*iir_b(2) + x_d2*iir_b(3) - y_d1*iir_a(2) - y_d2*iir_a(3))/2.^14);
    double t = (AvgMag_tmp*iir_b[0] + x_d1*iir_b[1] + x_d2*iir_b[2] - y_d1*iir_a[1] - y_d2*iir_a[2])/2;
    double AvgMag = round(pow(t, 14));
    x_d2 = x_d1; x_d1 = AvgMag_tmp;    // 输入的延迟量更新
    y_d2 = y_d1; y_d1 = AvgMag;        // 输出的延迟量更新
    AvgMag_save->tt.f8[ii] = AvgMag;  // 测试变量，不用保存
    //------------------------------
    if (AvgMag < 0) {
        AvgMag = 100;                    // 如果IIR滤波输出的值为负数或0，强制赋值为100; 防止起伏太大
    }
    AvgMag =  MIN(fabs(AvgMag) ,32767);     // 保证输出为16bit正数
    //****************************
    // 将平均幅度转化为dB值
    AvgMag = MAX(AvgMag,1);                // 防止出现０的情况
    double AvgMagDB=0;
    func_LevelTodB(&AvgMag, 1, &AvgMagDB);    // 调用函数func_LevelTodB计算AvgMag的对数增益
    // 进行AGC_Level的调整
    double agc_delta = AvgMagDB - DB_Threoshold;       // 计算平均幅度dB值与设定门限的差值dB值
    //    agc_delta  = sign(agc_delta)*min(10*64, abs(agc_delta)); // 每次最大的调整不能超过10dB;
    //*********** 对增益差值进行累加，得新的总增益****** 门限范围上下0.1dB**********************
    if (agc_delta  > floor(0.1*64)) {
      agc_dB = agc_dB - ceil(agc_delta/4.);     // 暂定充电常数为 4 ,实现时适当调整，采用向上取整ceil,保证每次至少调整1/64dB
    } else {
      if ( agc_delta < -floor(0.1*64)) {
        agc_dB =  agc_dB - floor( agc_delta/16);   // 暂定放电电常数为 16, 实现时适当调整，采用向下取整floor,保证每次至少调整1/64dB
      } else {
        agc_dB = agc_dB;
      }
    }
    //  输出的最大dB数 20*log10(32767/64) = 54dB;所以总的控制范围为上下54dB,共108dB，上限值为 3456 = 54*64
    agc_dB = CV_SIGN(agc_dB)* MIN(fabs(agc_dB), 3456);
    //****************************************
    //以下是测试变量，实现中不用保存
    agc_delta_save->tt.f8[ii] = agc_delta;
    agc_dB_save->tt.f8[ii] = agc_dB;
    agc_level_save->tt.f8[ii] = Agc_Level;              //　测试用的变量，不用实现
}

#if 0
//****************************************
// fvtool( iir_b,iir_a );    // 测试IIR滤波器响应
// 输出部分比较

figure;plot(Record3,'r');hold on;plot(real(0.2*sig_chan(1:128:end)),'g');
figure;plot(Record3,'g');
figure;plot(Record4,'r');
figure;plot(AGC_out,'b');
figure;
subplot(2,1,1); plot(sig_tx );grid on;title('发送的信号')
subplot(2,1,2); plot(real(sig_chan) );grid on;title('加噪后的信号')
figure;
subplot(2,1,1); plot(real(sig_chan),'r');grid on; hold on;plot(AGC_out);legend('信道信号','AGC输出信号')
subplot(2,1,2); plot(AGC_out);grid on;title('AGC输出信号)')
figure;
subplot(2,1,1); plot(agc_dB_save/64);grid on;title('AGC调整的dB值')
subplot(2,1,2); plot(agc_level_save/64 );grid on;title('AGC调整的电平值')
figure;
subplot(2,1,1);plot(20*log10(AvgMag_tmp_save));grid on;title('初始计算的平均幅度 (dB)')
subplot(2,1,2);plot(20*log10(max(abs(AvgMag_save),1)));grid on;title('经过IIR滤波后的平均幅度 (dB)')
figure;
plot(agc_delta_save);grid on; title( '平均幅度与门限的差距的dB值(Agc-delta，放大了64倍)')
//-----------------------
Mag_list
IIR_Neg_pos = find(AvgMag_save<0)
IIR_Neg_val = AvgMag_save(AvgMag_save<0)

#endif
  imfrees(im, 10);
return 0;
}