/////////////////////////////////////////////////////////////
// 日志：
// 上午 08:54 2016/5/31 星期二 修正二代机视频解码不正常
// 上午 11:00 2016/5/5 星期四    修正实时识别记录的获取bug
// 9:30 2015/9/12  完善用户下载接口和记录下载接口，增加网络配置信息获取接口。dev.cmd_GetNetPro();
/////////////////////////////////////////////////////////////
// 使用说明:
// 本SDK 适用于人脸识别3代机
// 数据下载用的TCP。具体使用见本文件末尾的 test1 函数。
// 视频浏览用的UDP。因此设备和手机需要配置成同一个网段。
// 启动视频例子 dev.cmd_StartVideo("192.168.1.3");
/////////////////////////////////////////////////////////////
package org.coprometro.CHFaceLib;

//import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.*;
import java.nio.charset.Charset;

import javax.imageio.ImageIO;

public class pmdev extends Thread {
	public static final int TCPPORT = 13333; // 服务器端口
	public static final int OFFSET_SIZE = 20; // 包头字节
	public static final int size_NCMD = 20; // 命令头字节
	public static final int PACK_SIZE = (60 * 1024); // 拆包
	public static final int ID_LEN = 24; // id长度

	public static final int M_USER = 0x01; // 用户
	public static final int M_VERIFY = 0x02; // 验证记录
	public static final int M_ALARM = 0x03; // 报警记录
	public static final int M_OPTLOG = 0x04; // 日志记录
	public static final int M_CIMAGE = 0x05; // 变更注册照记录
	public static final int M_RANGE = 0x06; // 区间集合数据下载
	public static final int M_AUTH = 0x07; // 客户端身份验证
	public static final int M_TIME = 0x08; // 系统时间
	public static final int M_HEART = 0x09; // 心跳包
	public static final int M_DEVINFO = 0x0a; // 设备基本信息
	public static final int M_STATIS = 0x0b; // 统计信息
	public static final int M_DEVSET = 0x0c; // 设备基本设置
	public static final int M_FORMAT = 0x0d; // 设备格式化
	public static final int M_DOOR_STATE = 0x0e; // 门状态设置 ,共3种状态：0正常状态 ， 1常开状态，
													// 2常闭状态
	public static final int M_REMOTE_OPEN = 0x0f; // 远程开关门,
	public static final int M_TGROUP = 0x10; // 时间组
	public static final int M_DSPRIGHT = 0x11; // 权限
	public static final int M_UGROUP = 0x12; // 用户组
	public static final int M_MCAST = 0x13; // 多播地址
	public static final int M_APPUPDATE = 0x14; // 程序更新
	public static final int M_VIDEO_OUT = 0x23; // UDP网络视频传输 NET_ADDRESS
	public static final int M_USER_EX = 0x24; // 批量用户上传现在
	public static final int M_REBOOT = 0x25; // 设备重启
	public static final int M_RANGE_EX = 0x26; // 记录批量下载
	public static final int M_SET_IP = 0x27; // 远程修改IP地址
	public static final int M_NOPEN = 0x28; // N+1开门请求
	public static final int M_IOCTRL = 0x29; // IO控制
	public static final int M_NETPRO = 0x30; // 网络设置

	public static final int S_SET = 0x81; // 上传\设置
	public static final int S_DEL = 0x82; // 删除
	public static final int S_GET = 0x83; // 下载\获得
	public static final int S_REALTIME_SEND = 0x84; // 采集实时发送
	public static final int S_REALTIME_LOAD = 0x85; // 实时加载
	public static final int S_REALTIME_CAP = 0x86; // 实时用户采集
	public static final int S_REALTIME_CAP_INFO = 0x88; // 远程采集实是进度信息
	public static final int S_UPDATE_SEND = 0x87; // 更新实时发送
	public static final int S_CLEAR_INOUT = 0x88; // 清空统计计数

	// 客户端与中转服务器主命令
	public static final int C_CNDEV = 0xB0; // client通过中转服务器设备连接
	public static final int C_VERIFY = 0xB1; // 中转服务器验证client身份
	public static final int C_HEART = 0xB2; // 中转服务器与client的心跳包
	// 客户端与中转服务器子命令
	public static final int S_CN = 0xc0; // 建立连接
	public static final int S_DN = 0xc1; // 断开连接

	// 命令执行错误状态
	public static final int SUC_EXECUT = 0x00; // 成功
	public static final int ERR_EXECUT = 0x0A; // 失败
	public static final int ERR_BUSY = 0x01; // 设备忙
	public static final int ERR_LIMIT = 0x02; // 已达上限
	public static final int ERR_NOFIND = 0x03; // 没有找到对应数据
	public static final int ERR_SAVE_FAIL = 0x04; // 数据保存失败
	public static final int ERR_SET_FAIL = 0x05; // 设置失败
	public static final int ERR_VERIFY_FAIL = 0x06; // 验证失败
	public static final int ERR_FROMAT = 0x07; // 格式化失败
	public static final int ERR_PARAM = 0x08; // 参数错误
	public static final int ERR_DISABLE = 0x09; // 要求执行的功能没有使能
	public static final int ERR_SUPPORT = 0x10; // 不支持的命令
	public static final int ERR_INPUTDATA = 0x11; // 网络端传输的数据有异常

	public static final int CMD_STATE_REPLY = 0x40000000; // 标记该命令为状态回复命令
	public static final int CMD_DOWN_ID = 0x80000000; // 下载数据ID集合,优先检测该标记
	public static final int CMD_ID_ALL = 0x00000100; // 下载所有数据的区间集合
	public static final int CMD_ID_TIME_RANGE = 0x00000200; // 下载制定时间段区间集合
	public static final int CMD_ID_RANGE = 0x00000400; // 确定指定区间的区间集合

	// 数据下载
	public static final int DL_IMAGE = 0x00000100; // 需要下载照片
	public static final int DL_FEAT = 0x00000200; // 需要下载特征
	public static final int DL_APPENDID = 0x00000400; // 实时加载指示CID是编号还是卡号
	public static final int LD_ENROLL = 0x00000800; // 需要远程采集
	public static final int USER_TEXT = 0x00000400; // 用户文字信息
	public static final int BATCH_END = 0x00000800; // 批量结束

	// 用户验证
	public static final int AUTH_VERIFY = 0x00000100; // 用户身份验证

	// 门状态: 开，关，不指定为正常状态
	public static final int DOOR_OPEN = 0x00000100; // 开
	public static final int DOOR_CLOSE = 0x00000200; // 关

	// 时间组下载
	public static final int DL_NORMAL_TG = 0x00000100; // 下载正常时间组
	public static final int DL_HOLIDAY_TG = 0x00000200; // 下载节假日

	// 组合有序
	// 用户组合
	// 有序组合开门: 同一组用户必须有序验证，必须先验证完强制用户，在验证普通用户
	// 无序组合开门: 不管以何种顺序验证，只要最后验证总结果打到组合验证的要求，验证通过.
	public static final int G_ORDER = 0x01;

	public static final int CMD_M_ID = 0xA1;
	public static final int CMD_M_AUTH = M_AUTH;
	public static final int CMD_M_HEART = M_HEART;
	public static final int CMD_M_DEVINFO = M_DEVINFO;
	public static final int CMD_M_STATIS = M_STATIS;
	public static final int CMD_M_DEVSET = M_DEVSET;
	public static final int CMD_M_TIME = M_TIME;
	public static final int CMD_M_FORMAT = M_FORMAT;
	public static final int CMD_M_EOPEN = M_DOOR_STATE;
	public static final int CMD_M_REMOTE_OPEN = M_REMOTE_OPEN;
	public static final int CMD_M_NOPEN = M_NOPEN;
	public static final int CMD_M_USER = M_USER;
	public static final int CMD_M_VERIFY = M_VERIFY;
	public static final int CMD_M_ALARM = M_ALARM;
	public static final int CMD_M_OPTLOG = M_OPTLOG;
	public static final int CMD_M_CIMAGE = M_CIMAGE;
	public static final int CMD_M_RANGE = M_RANGE;
	public static final int CMD_M_TGROUP = M_TGROUP;
	public static final int CMD_M_DSPRIGHT = M_DSPRIGHT;
	public static final int CMD_M_UGROUP = M_UGROUP;
	public static final int CMD_M_MCAST = M_MCAST;
	public static final int CMD_M_APPUPDATE = M_APPUPDATE;
	public static final int CMD_M_VIDEO_OUT = M_VIDEO_OUT;
	public static final int CMD_M_USER_EX = M_USER_EX;
	public static final int CMD_M_RECORD_EX = M_RANGE_EX;
	public static final int CMD_M_REBOOT = M_REBOOT;
	public static final int CMD_M_IOCTRL = M_IOCTRL;
	public static final int CMD_M_NETPRO = M_NETPRO;

	public static final int CMD_S_SET = S_SET; // 上传\设置
	public static final int CMD_S_DEL = S_DEL; // 删除
	public static final int CMD_S_GET = S_GET; // 下载\获得
	public static final int CMD_S_REALTIME_SEND = S_REALTIME_SEND; // 采集实时发送
	public static final int CMD_S_REALTIME_LOAD = S_REALTIME_LOAD; // 实时加载
	public static final int CMD_S_REALTIME_CAP = S_REALTIME_CAP; // 实时用户采集
	public static final int CMD_S_REALTIME_CAP_INFO = S_REALTIME_CAP_INFO; // 远程采集实是进度信息
	public static final int CMD_S_UPDATE_SEND = S_UPDATE_SEND; // 更新实时发送
	public static final int CMD_S_CLEAR_INOUT = S_CLEAR_INOUT; // 清空统计计数

	// 状态标记位
	public static final int F_ERR_BUSY = ERR_BUSY;
	public static final int F_ERR_LIMIT = ERR_LIMIT;
	public static final int F_ERR_NOFIND = ERR_NOFIND;
	public static final int F_ERR_SAVE_FAIL = ERR_SAVE_FAIL;
	public static final int F_ERR_SET_FAIL = ERR_SET_FAIL;
	public static final int F_ERR_VERIFY_FAIL = ERR_VERIFY_FAIL;

	// 其他标记位
	public static final int F_DL_ID = CMD_DOWN_ID; // 下载ID集合
	public static final int F_DL_ID_ALL = CMD_ID_ALL; // 下载所有数据的ID集合
	public static final int F_ID_TIME_RANGE = CMD_ID_TIME_RANGE; // 下载制定时间段ID集合
	public static final int F_ID_RANGE = F_ID_TIME_RANGE << 1;
	public static final int F_AUTH_VERIFY = AUTH_VERIFY; // 用户身份验证
	public static final int F_DL_PIC = DL_IMAGE; // 需要下载照片
	public static final int F_DL_FEAT = DL_FEAT; // 需要下载人脸特征
	public static final int F_DL_APPENDID = DL_APPENDID; // 远程加在时，m_ID表示卡号
	public static final int F_DL_NORMAL_TG = DL_NORMAL_TG; // 下载正常时间组
	public static final int F_DL_HOLIDAY_TG = DL_HOLIDAY_TG; // 下载节假日
	public static final int F_DOOR_CLOSE = DOOR_CLOSE; // 紧急关门，否则为紧急开门
	public static final int F_DOOR_OPEN = DOOR_OPEN; // 长开或者长关状态取消
	public static final int F_G_ORDER = G_ORDER; // 组合开门有序
	// public static final int F_DEFAULT_NO_GROUP = DEFAULT_NO_GROUP;

	// 没有指定用户权限,用户默认权限ID为DEFAULT_NO_RIGHT或者DEFAULT_SINGLE_OPEN,DSP上采集的用户，
	// 超级管理员可以统一配置为DSP_DEFAULT_SINGLE_OPEN
	// 默认无权限，默认单一开门权限，组合用户最大组用户数，权限支持最大时间组
	// DEFAULT_NO_RIGHT: DSP,管理端新增用户,默认值之一无权限
	// DEFAULT_SINGLE_OPEN: DSP,管理端新增用户，默认值之二单一开门权限，此权限没有时间组概念
	public static final int DEFAULT_NO_RIGHT = -2;
	public static final int DEFAULT_SINGLE_OPEN = -1;
	public static final int MAX_GROUPUSER = 60;
	public static final int TGROUP_NUM = 8;

	public static final int TIMEGROUP_NUMS = TGROUP_NUM; // 时间组个数
	public static final int USER_COMBINS = MAX_GROUPUSER; // 用户组合个数

	// 日期
	public class NETDATE {
		public int m_Year;
		public byte m_Month;
		public byte m_Day;
		public static final int size = 8;

		public String tostring() {
			return "" + m_Year + "/" + m_Month + "/" + m_Day;
		}
	}

	// 时间
	public class NETTIME {
		public int m_Msec; // 毫秒
		public byte m_Hour;
		public byte m_Minute;
		public byte m_Second;
		public static final int size = 8;

		String tostring() {
			return "" + m_Hour + ":" + m_Minute + "-" + m_Second + "-" + m_Msec;
		}
	}

	public class DATETIME {
		public NETDATE m_Date;
		public NETTIME m_Time;
		public static final int size = NETDATE.size + NETTIME.size;

		String tostring() {
			return "日期:" + m_Date.tostring() + " 时间: " + m_Date.tostring();
		}
	}

	// 获得指定时间段的ID集合
	public class TIMESE {
		public NETDATE m_Start; // 记录起始时间
		public NETDATE m_End; // 记录结束时间
		public static final int size = NETDATE.size * 2;

		TIMESE(NETDATE Start, NETDATE End) {
			m_Start = Start;
			m_End = End;
		}
	};

	public static final int DL_OPENINTERVAL = 0x00000001; // 下载指定记录流水号之后的N条记录，包括指定记录本身

	// 可变数组结构体。数组元素具体什么内容，需要根据NCMD设定的命令来解释
	public class CARRAY {
		public int m_Count; // 数组元素的个数
		public byte[] m_Block; // 数组地址, 4是为了结构体的字节对齐
	};

	public static final int WEB_LOADUSER = 0x0001; // 加载用户
	public static final int WEB_WRITEUSER = 0x0002; // 写用户
	public static final int WEB_WRITEREC = 0x0004; // 写记录
	public static final int WEB_SYNCTIME = 0x0008; // 同步时间

	// 网络属性
	public class NETPRO {
		public byte m_WebPro1;
		public byte m_WebPro2;
		public String m_WebUrl1;// = new byte[127];
		public String m_WebUrl2;// = new byte[127];
		public String m_IPAdr;// = new byte[16];
		public String m_MIPAdr;// = new byte[16];
		public String m_NetMask;// = new byte[16];
		public String m_GateWay;// = new byte[16];
		public String m_Dns;// = new byte[16];
		public static final int size = 128 + 128 + 16 * 5;
	}

	public static final int CHECK_TIME = 0x01; // 检测时间
	public static final int CHECK_WEEK = 0x02; // 检测星期
	public static final int CHECK_DAY = 0x04; // 检测日期
	public static final int CHECK_MONTH = 0x08; // 检测月份
	public static final int CHECK_YEAR = 0x10; // 检测年份

	public static final int WEEK_1 = 0x01; // 星期一有效
	public static final int WEEK_2 = 0x02; // 星期二有效
	public static final int WEEK_3 = 0x04; // 星期三有效
	public static final int WEEK_4 = 0x08; // 星期四有效
	public static final int WEEK_5 = 0x10; // 星期五有效
	public static final int WEEK_6 = 0x20; // 星期六有效
	public static final int WEEK_7 = 0x40; // 星期天有效

	// 任意时间有效 , 正常时间组，节假日时间组
	public static final int ANY_TIME = -1;
	public static final int TG_NORMAL = 1;
	public static final int TG_HOLIDY = 2;

	// 时间组
	public class DSPTIMEG {
		public int m_TGID; // 时间组ID
		public DATETIME m_Start; // 时间组开始时间
		public DATETIME m_End; // 时间组结束时间
		public byte m_TGType; // 时间组类型, TG_NORMAL,TG_HOLIDY
		public byte m_CheckFlag; // 时间检测标记例：m_CheckFlag = CHECK_TIME|CHECK_WEEK
		public byte m_WeekFlag; // 检测星期时，标记那些星期有效。例：m_WeekFlag = WEEK_1|WEEK_5
		public byte m_Reserve; // 保留字节
		public static final int size = 4 + DATETIME.size + 4;
	}

	public static final int CHECK_HOLIDY = 0x01; // 权限检测节假日
	public static final int ACTION_LOCK = 0x02; // 电锁输出
	public static final int ACTION_OUTPUT = 0x04; // 辅助输出
	public static final int ACTION_N_1 = 0x08; // 该权限需要N+1

	// 权限
	public class DSPRIGHT {
		// 权限ID
		public int m_RightID;

		// 时间组ID , (m_TimeGID[0]==ANY_TIME)未指定时间组，开门方式不受时间限制，任意时间段验证成功执行开门方式.
		public int[] m_TimeGID = new int[TGROUP_NUM];

		// BIT(0):1 检测节假日, 不检测节假日
		// BIT(1):1 电锁, 1输出 0不输出
		// BIT(2):1 辅助, 1输出 0不输出
		// BIT(3):1 N＋1, 开启 0关闭
		public int m_Mode;

		public static final int size = (TGROUP_NUM + 2) * 4;
	};

	// 表示用户不参与任何组
	public static final int INVALID_GROUPID = -1;

	public class USERGROUP {
		// 组ID
		public int m_GroupID;
		// BIT(0):1有序，0无序
		public byte m_GroupFlag;
		// 组合成功后，输出参数ACTION_LOCK|ACTION_OUTPUT|ACTION_N_1
		public byte m_Param;
		// 普通用户中有效用户数
		public byte m_NormalValid;
		// 强制用户中有效用户数
		public byte m_ForceValid;
		// 普通用户组合，优先级低
		public String[] m_NormalUsers;// [MAX_GROUPUSER];
		// 强制用户组合，优先级高
		public String[] m_ForceUsers; // [MAX_GROUPUSER];
		public static final int size = 8 + ID_LEN * 2 * MAX_GROUPUSER;
	};

	public class GROUPEX {
		public int m_GroupID;
		public int m_StrSize;
		public byte m_GroupFlag;
		public byte m_MinNum;
		public byte[] m_BLock;//
	};

	public class VUSER {
		public int m_UserType; // 终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
		public String m_UserID; // 用户ID
		public DATETIME m_TypeUpdTime; // 用户类型更改时间
		public DATETIME m_RegistTime; // 人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
		public DATETIME m_LastUpdTime; // 最后更新的时间，识别时特征发生更新或者重新采集要更新此时间

		public String tostring() {
			String[] typestr = { "普通", "管理", "超管" };
			String ss = "";
			ss += "终端设备上的用户类型: " + m_UserType + "(" + typestr[m_UserType] + ")";
			ss += "\n用户ID: " + m_UserID;
			ss += "\n用户类型更改时间" + m_TypeUpdTime.tostring();
			ss += "\n人脸注册时间" + m_UserID;
			ss += "\n最后更新的时间" + m_UserID;
			return ss;
		}
	};

	// 用户信息
	public static final int USER_CARD_INVALID = 0x01; // 卡挂失
	public static final int USER_DLine_VALID = 0x02; // 失效时间有效
	public static final int USER_BLACK = 0x04; // 黑名单
	public static final int USER_MIX1N = 0x08; // 1N人脸认证
	public static final int USER_VIPCARD = 0x10; // VIP用户
	public static final int USER_FINGER_PRINT = 0x20; // 指纹认证
	public static final int USER_SYNC_FINGEER = 0x40; // 指纹同步

	public class USER {
		public int m_RightID; // 用户权限ID,三种可能值(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,用户指定)
		public String m_ImageSID; // 上传注册照，对应的变更ID，全局唯一
		public String m_UserID; // 用户标识
		public String m_Admin; // 人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
		public String m_AppendID; // 卡号或者其他用于1:1的附加身份确认信息
		public String m_UserName; // 用户名
		public String m_Department; // 部门名称
		public DATETIME m_DeadLine; // 失效时间
		public DATETIME m_RegistTime; // 人脸注册时间，即特征采集时间，采集或者重新采集要更新此时间
		public DATETIME m_LastUpdTime; // 最后更新的时间，识别时特征发生更新或者重新采集要更新此时间
		public DATETIME m_TypeUpdTime; // 用户类型变更时间
		public byte m_UserFlag; // 用户状态标记
		public byte m_UserType; // 终端设备上的用户类型(普通，管理，超管 -- 0 , 1 , 2)
		public byte[] m_Feat; // 人脸特征，==0 表示没有人脸特征
		public byte[] m_Pic; // 注册图像 ==null 表示没有图片
		int FeatLen() {
			int m_Feat_length = 0;
			if (m_Feat != null) {
				m_Feat_length = m_Feat.length;
			}
			return m_Feat_length;
		}
		int PicLen() {
			int m_Pic_length = 0;
			if (m_Pic != null) {
				m_Pic_length = m_Pic.length;
			}
			return m_Pic_length;
		}
		// 注册图片,必须JPG,人脸特征是与m_UserID唯一绑定的.m_Block的前m_FeatBytes个字节表示人脸特征,
		// 后m_PicBytes个字节表示图片.
		public int size() {
			return ALIGN(6 * ID_LEN + 4 * size_DATETIME + 2 * 4 + FeatLen()
					+ PicLen()+16, 4);
		}
	}

	// 记录类型:
	// 0x01 - 0x20: 正常事件
	// 0x20 - 0x40: 硬件报警事件
	// 0x40 - 0x50: 辅助输入
	// 0x50 - 0x60: 辅助输出
	// 0xf0 - 0xff: 软件报警事件
	// 其他: 未定义

	public static final int VERIFY_SUCC = 0x01; // 验证成功
	public static final int VERIFY_FAIL = 0x02; // 验证失败
	public static final int ADMIN_SUCC = 0x03; // 管理员验证成功
	public static final int EMER_OPEN = 0x04; // 紧急开门
	public static final int RIGHT_OPEN = 0x05; // 权限开门
	public static final int GROUP_OPEN = 0x06; // 组合开门
	public static final int BUTTON_OPEN = 0x07; // 按钮开门

	public static final int ALARM_HD_MANGET_TIMEOUT = 0x20; // 门磁超时
	public static final int ALARM_HD_MANGET_ILLOPEN = 0x21; // 门磁非法开门
	public static final int ALARM_HD_OFFLINE = 0x22; // 前端掉线报警
	public static final int ALARM_HD_BREAK = 0x30; // 防拆报警
	public static final int ALARM_HD_SHOCK = 0x31; // 震动报警
	public static final int ALARM_HD_FPOWR = 0x36; // 前端供电异常报警
	public static final int ALARM_HD_UPS_ON = 0x37; // UPS备用电池开启
	public static final int ALARM_HD_UPS_OFF = 0x38; // UPS备用电池关闭

	public static final int ALARM_HD_ASSIST = 0x40; // 辅助输入触发
	public static final int ALARM_HD_WL_DOOR_MAGNETIC = 0x41; // 无线
	public static final int ALARM_HD_WL_SMOKE = 0x42;
	public static final int ALARM_HD_WL_GAS = 0x43;
	public static final int ALARM_HD_WL_BUTTON1 = 0x44;
	public static final int ALARM_HD_WL_BUTTON2 = 0x45;
	public static final int ALARM_HD_WL_BUTTON3 = 0x46;
	public static final int ALARM_HD_WL_BUTTON4 = 0x47;
	public static final int ALARM_HD_WL_AREA1 = 0x48;
	public static final int ALARM_HD_WL_AREA2 = 0x49;
	public static final int ALARM_HD_WL_AREA3 = 0x4A;
	public static final int ALARM_HD_WL_AREA4 = 0x4B;
	public static final int ALARM_HD_WL_AREA5 = 0x4C;
	public static final int ALARM_HD_WL_AREA6 = 0x4D;
	public static final int ALARM_HD_WL_AREA7 = 0x4E;

	public static final int ALARM_AB_RULE = 0xE0;

	public static final int ALARM_SF_BACKLIST = 0xF0; // 黑名单验证报警
	public static final int ALARM_SF_ILLCARD = 0xF1; // 无效卡(挂失)
	public static final int ALARM_SF_ILLTIME = 0xF2; // 非法时间识别
	public static final int ALARM_SF_DEADLINE = 0xF3; // 失效时间
	public static final int ALARM_SF_DANGER_OPEN = 0xF4; // 胁迫开门
	public static final int ALARM_SF_SUPER_OPEN = 0xF5; // 超级密码开门
	public static final int LINK_EVENT = 0xF6; // 联动事件

	public class RECORDRANGE {
		// 下载多少条,<=0 表示下载m_SID之后产生的所有记录,此场景下m_SID必须有效
		public int m_Count;
		// DL_OPENINTERVAL打开开区间下载,否则闭区间下载
		public int m_Flag;
		// 从那一条开始，m_SID表示记录精确的流水号,
		// 流水号字符串组成:
		// SN(MAC后3字节) + TYPE(记录类别) + DATETIME(日期) + ID(自定义) = (6BYTE + 1BYTE +
		// 14BYTE + 2BYTE + \0)
		public String m_SID;
		public static final int size = ID_LEN + 8;
	};

	// 记录信息,识别记录，报警记录
	public class RECORD {
		public int m_RecType; // 记录类型
		public int m_Score; // 识别得分 , 联动事件为联动事件的LINKID
		public int m_PicBytes; // 记录图像的字节数，==0 表示没有记录图片
		public String m_ChangID; // 注册照记录号
		public String m_UserID; // 用户ID,如果为空，表示非法记录 , 联动事件为事件描述Text
		public String m_SerialID; // 识别记录流水号ID
		public String m_AppendID; // 卡号或者其他用于1:1的附加身份确认信息
		public String m_UserName; // 用户名
		public String m_Department; // 部门名称
		public DATETIME m_RecTime; // 记录时间
		public byte m_VerifyMode; // 验证模式,例:VERIFY_CARD|VERIFY_FACE_1N,刷卡+人脸
		public byte m_Source; // 事件源设备的485地址
		public byte[] m_Pic; // 记录图像,必须JPG , 2是为了结构体的字节对齐

		public int size() {
			return ALIGN(4 * 4 + ID_LEN * 6 + m_Pic.length, 4);
		}
	}

	// public static final int CGIMG_STEP( obj ) ALIGN4(
	// sizeof(CHANGEIMAGE)+(this).m_PicBytes-4 )
	// public static final int DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )

	// 操作日志类型

	public static final int DSPLOG_ADDUSER = 0x01; // 增加用户
	public static final int DSPLOG_DELUSER = 0x02; // 删除用户
	public static final int DSPLOG_REREGIST = 0x03; // 重新采集
	public static final int DSPLOG_CAHNGETYPE = 0x04; // 改变用户类型
	public static final int DSPLOG_UDISK_ULUSER = 0x05; // U盘上传用户
	public static final int DSPLOG_UDISK_DLUSER = 0x06; // U盘下载用户
	public static final int DSPLOG_UDISK_DLRECORD = 0x07; // U盘下载记录
	public static final int DSPLOG_UDISK_UPDATEAPP = 0x08; // U盘更新程序

	// 设备操作日志
	public class DSPLOG {
		public int m_LogType; // 日志类型
		public String m_SerialID; // 流水号ID
		public String m_Admin; // 操作员
		public String m_BeOptUser; // 被操作员
		public DATETIME m_RecTime; // 记录时间
	}

	// 变更注册照
	public class CHANGEIMAGE {
		public String m_UserID; // 用户ID
		public String m_SerialID; // 流水号ID
		public String m_AppendID; // 卡号或者其他用于1:1的附加身份确认信息
		public String m_UserName; // 用户名
		public String m_Department; // 部门名称
		public String m_Admin; // 人脸注册管理员,标识此用户的人脸特征是哪个管理员采集
		public DATETIME m_RecTime; // 记录时间
		public byte[] m_Pic; // 注册照内存

		public int size() {
			return ALIGN(6 * ID_LEN + 8 + m_Pic.length, 4);
		}
	};

	// DSP相关统计信息
	public class STATIS {
		public int m_TotalUsers; // 当前设备的总用户数
		public int m_NoFeatUser; // 没有采集人脸特征的用户数
		public int m_TotalDays; // 识别记录保存总天数
		public int m_TotalRecords; // 总记录数
		public int m_TotalAlarm; // 总报警记录数
		public int m_TotalDspLog; // 总操作日志数目
		public int m_TotalChangeImage; // 总变更注册照

		public String tostring() {
			String ss = "";
			ss += ("当前设备的总用户数: " + m_TotalUsers);
			ss += ("\n没有采集人脸特征的用户数: " + m_NoFeatUser);
			ss += ("\n识别记录保存总天数: " + m_TotalDays);
			ss += ("\n总记录数: " + m_TotalRecords);
			ss += ("\n总报警记录数: " + m_TotalAlarm);
			ss += ("\n总操作日志数目: " + m_TotalDspLog);
			ss += ("\n总变更注册照: " + m_TotalChangeImage);
			return ss;
		}
	};

	// 验证信息
	public class AUTH {
		public String m_UserName; // 用户名
		public String m_Password; // 密码
	};

	// 设备基本信息
	public class BASEINFO {
		public int m_DevType; // 设备类型
		public int m_LimitUser; // 总人数上限
		public int m_Limit1NUser; // 1N用户上限
		public String m_SN; // 设备编号
		public String m_Ver; // DSP软件版本
		public String m_Space; // 磁盘容量信息

		public String tostring() {
			String ss = "";
			ss += ("设备类型 : " + m_DevType);
			ss += ("\n总使用人数上限 : " + m_LimitUser);
			ss += ("\n1N用户上限 : " + m_Limit1NUser);
			ss += ("\n设备编号 : " + m_SN);
			ss += ("\nDSP软件版本 : " + m_Ver);
			ss += ("\n磁盘容量信息 : " + m_Space);
			return ss;
		}
	}

	// 远程采集进度信息
	public class RCAPINFO {
		public int m_Total; // 采集总数
		public int m_Count; // 当前采集数
		public String m_UserID; // 用户登记编号
	};

	// 2.0版支持的功能
	public static final int RECORD_SAVEFAIL = 0x00000001; // 是否保存识别失败记录
	public static final int SUPER_PASSWORD = 0x00000002; // 超级密码开门有效
	public static final int HDBEEP_OPEN = 0x00000004; // 撤防/布防(硬件报警蜂鸣器开启)
	public static final int REALTIME_RECORD = 0x00000010; // 实时动态记录显示
	public static final int REALTIME_USERLOAD = 0x00000020; // 输入编号或者卡号识别时，如果无法在当前dsp中获得人脸特征,在网络连接状态下，将从管理端实时加载人脸特征
	public static final int REALTIME_USERSEND = 0x00000040; // 采集用户同步
	public static final int DOORMANGET_OPEN = 0x00000080; // 开启门磁检测报警
	public static final int DOORFORCE_OPEN = 0x00000100; // 开启胁迫开门功能
	public static final int REMOTE_CAP_SAVE = 0x00000200; // 远程控制采集的用户是否保存到DSP
	public static final int GPRS_OPEN = 0x00000400; // 开启GPRS模块
	public static final int UPDATE_USERSEND = 0x00000800; // 特征更新时，是否实时发送用户特征
	public static final int REALTIME_USERLOAD_PRIOR = 0x00001000; // 网络连接情况下，优先网络加载
	public static final int AB_LOCK = 0x00002000; // AB互锁
	public static final int DOOR1_NOPEN = 0x00004000; // 门一N+1
	public static final int DOOR2_NOPEN = 0x00008000; // 门二N+1
	public static final int ENABLE_COUNT = 0x00010000; // 在线记录数统计
	public static final int ENCRYP_DISPALY = 0x00020000; // 在线记录数统计

	// 验证模式优先级低-高:UID<CARD<11<MIX<1N
	public static final int VERIFY_USERID = 0x01; // 编号
	public static final int VERIFY_CARD = 0x02; // 刷卡
	public static final int VERIFY_FACE_11 = 0x04; // 11人脸加载
	public static final int VERIFY_FACE_MIX = 0x08; // 混合人脸加载
	public static final int VERIFY_FACE_1N = 0x10; // 1N人脸加载
	public static final int VERIFY_FINGER = 0x20; // 指纹认证

	public static final int OUTPUT_WG_YES = 0x01; // 输出伟根协议
	public static final int OUTPUT_WG_34 = 0x02; // 指定输出伟根34，不指定输出伟根26
	public static final int OUTPUT_WG_CTX = 0x04; // 伟根输出指定内容
	public static final int OUTPUT_WG_UID = 0x08; // 不标记OUTPUT_WG_CTX，输出用户ID，默认输出卡号

	public static final int WG_OLD = 0;
	public static final int WG_26 = 1;
	public static final int WG_34 = 2;
	public static final int WG_35 = 3;
	public static final int WG_36 = 4;
	public static final int WG_37 = 5;
	public static final int WG_44 = 6;
	public static final int WG_45 = 7;

	// 设备工作属性设置
	public class WORKATT {
		// 设备时间组,(m_TimeGID[0]==ANY_TIME)未指定时间组,设备任意时间有效.设备时间组优先级高于用户权限时间组
		public int[] m_DevCtx; // [ TGROUP_NUM ];
		// 设备默认权限: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
		public int m_DefaultRight;
		// 参见2.0版本定义的标记
		public int m_BaseSet;
		// 超级密码, 将产生一条报警记录，并且会抓拍彩色全景图.
		public String m_SuperPassword;
		// 门磁延时时间，单位秒
		public byte m_DoorMangetTime;
		// 电锁持续时间，单位秒
		public byte m_LockTime;
		// 用户身份验证模式,例如:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
		public byte m_VerifyMode;
		// BIT(0) : 0不输出扩展韦根信号,1输出.
		// BIT(1) : 0输出韦根26,1输出韦根34.
		// BIT(2) : 0按照BIT(4)输出 , 1输出m_OutputCtx中指定内容
		// BIT(4) : 0直接输出卡号,1输出编号
		// BIT(4-7): WG输出协议,0-wg26 1-wg34 2-wg35 3-wg36 4-wg37 5-wg44 6-wg45
		public byte m_OutputWG;
		// 韦根协议输出内容
		public int m_OutputCtx;
	};

	// 程序更新结构体
	public class APPUPDATE {
		public String m_FileName; // 文件名
		public byte m_TotalFiles; // 需要更新的文件总数
		public byte m_FileIndex; // 当前更新到第几个
		public byte[] m_File; // 文件数据

		public int size() {
			return ID_LEN + 6 + m_File.length;
		}
	};

	// IO设备状态
	public static final int IO_MODE_NORMAL = 0x00; // 正常状态
	public static final int IO_MODE_OPEN = 0x01; // 常开状态
	public static final int IO_MODE_CLOSE = 0x02; // 常关状态
	// 门磁状态
	public static final int IO_STATE_CLOSE = 0x00; // 门磁关
	public static final int IO_STATE_OPEN = 0x01; // 门磁开
	// 执行动作
	public static final int ACT_IO_OPEN = 0x02; // 执行打开动作
	public static final int ACT_IO_OPENEX = 0x04; // 执行辅助动作
	public static final int ACT_IO_CLOSE = 0x10; // 执行关闭动作
	public static final int ACT_MODE_SET = 0x20; // 设置IO设备工作模式
	public static final int ACT_MODE_GET = 0x40; // 获得IO设备工作模式
	public static final int ACT_STATE_GET = 0x80; // 获得IO设备当前状态

	public class IOCTRL {
		// IO设备485地址
		public byte m_Source;
		// 执行动作
		public byte m_Action;
		// IO设备当前模式
		public byte m_IOMode;
		// IO设备当前状态状态
		public byte m_IOState;
		public static final int size = 4;
	};

	// NOPEN:远程请求管理端开门
	public class NOPEN {
		// m_Users==1权限ID, m_Users>1组合ID
		public int m_CtxID;
		// 验证用户数目： 0密码开门，1权限开门，>1组合开门
		public int m_Users;
		// 控制信息
		public IOCTRL m_XOpen;
		// 验证用户数组
		public String[] m_IDS;// [MAX_GROUPUSER * 2];
	};

	// 关于485地址解析:
	// 1.485地址用一个字节表示，[4-7]表示设备索引号1-15 , [0-3]表示支持16种不同型的设备
	// 已经定义的设备类型 0--人脸前端 , 1--读卡器 ,
	// 例如: 0x11 表示1号门人脸识别前端1， INDEX[4-7] == 1 TYPE[0-3] == 1
	// 0x10 表示1号门人脸识别前端0，INDEX[4-7] == 1 TYPE[0-3] == 0
	// 0x12 表示1号门刷卡器0 INDEX[4-7] == 1 TYPE[0-3] == 2
	// 0x13 表示1号门刷卡器1 INDEX[4-7] == 1 TYPE[0-3] == 3
	// 0x1F 表示门点本身 INDEX[4-7] == 1 TYPE[0-3] == F
	// 0x14 表示辅助输入1 INDEX[4-7] == 1 TYPE[0-3] == 4
	// 0x25 表示辅助输出2 INDEX[4-7] == 2 TYPE[0-3] == 5
	// 2. 所有设备的序号从1开始。此协议可支持15张门，16中不同类型的门点输入设备，15个辅助输入和15个辅助输出
	// 3. [0-7] == 0x00 表示非法地址，[0-7] ＝＝ 0x01表示后端板本身
	// 7_______3______0
	// |_INDEX_|__type__|

	public static final int DEV_TYPE_FACE1 = 0x0; // 人脸前端1
	public static final int DEV_TYPE_FACE2 = 0x1; // 人脸前端2
	public static final int DEV_TYPE_CARD1 = 0x2; // 刷卡器1
	public static final int DEV_TYPE_CARD2 = 0x3; // 刷卡器2
	public static final int DEV_TYPE_IN = 0x4; // 辅助输入
	public static final int DEV_TYPE_OUT = 0x5; // 辅助输出
	public static final int DEV_TYPE_BUTTON = 0x6; // 出门按钮
	public static final int DEV_TYPE_DOOR = 0xF; // 门点本身

	public static final int IMAGE_UYVY = 0x01;
	public static final int IMAGE_YUYV = 0x02;
	public static final int IMAGE_RGB24 = 0x03;

	public class IMAGE {
		public int m_Size; // 图像数据总大小BYTES
		public int m_W; // 图像宽
		public int m_H; // 图像高
		public int m_Step; // 行数据步长
		public byte m_Type; // 图像类型
		public byte[] m_Block; // 图像数据
	};

	Socket sc;
	DataInputStream netInputStream;
	DataOutputStream netOutputStream;
	// int isexit;
	int m_nIndex;

	public static int bytecopy(byte[] src, int srcoff, byte[] dst, int dstoff,
			int maxlen) {
		maxlen = Math.min(maxlen, src.length - srcoff);
		maxlen = Math.min(maxlen, dst.length - dstoff);
		System.arraycopy(src, srcoff, dst, dstoff, maxlen);
		return maxlen;
	}

	public static int bytefill(byte[] src, int srcoff, int maxlen, byte x) {
		maxlen = Math.min(maxlen, src.length - srcoff);
		for (int i = 0; i < maxlen; ++i) {
			src[srcoff + i] = x;
		}
		return maxlen;
	}

	public static byte[] str2bytes(String data, String charsetName) {
		Charset charset = Charset.forName(charsetName);
		if (data==null) {
			return new byte[0];
		} else {
			return data.getBytes(charset);
		}
	}

	public class set_stream {
		int off;
		byte[] bytes;

		set_stream(byte[] bytes1, int off1) {
			bytes = bytes1;
			off = off1;
		}

		public int set_STRING(String data, int maxlen) {
			byte[] s = str2bytes(data, "GBK");
			int len = bytecopy(s, 0, bytes, off, maxlen);
			if (maxlen > len) {
				bytefill(bytes, off + len, maxlen - len, (byte) 0);
			}
			off += maxlen;
			return off;
		}

		public int set_BYTE(byte data) {
			bytes[off + 0] = (byte) (data);
			off += 1;
			return off;
		}

		public int set_BYTES(byte[] data, int n) {
			int i = 0, m = Math.min(data.length, n);
			for (; i < m; ++i) {
				bytes[off + i] = data[i];
			}
			for (; i < n; ++i) {
				bytes[i] = 0;
			}
			off += n;
			return off;
		}

		public int set_SHORT(short data) {
			bytes[off + 0] = (byte) (data & 0xff);
			bytes[off + 1] = (byte) ((data & 0xff00) >> 8);
			off += 2;
			return off;
		}

		public int set_INT(int data) {
			bytes[off + 0] = (byte) (data & 0xff);
			bytes[off + 1] = (byte) ((data & 0xff00) >> 8);
			bytes[off + 2] = (byte) ((data & 0xff0000) >> 16);
			bytes[off + 3] = (byte) ((data & 0xff000000) >> 24);
			off += 4;
			return off;
		}

		public int set_INTS(int[] arr, int n) {
			int m = Math.min(arr.length, n);
			for (int i = 0; i < m; ++i) {
				set_INT(arr[i]);
			}
			for (; m < n; ++m) {
				set_INT((int) 0);
			}
			return off;
		}

		public int set_CID(String str) {
			return set_STRING(str, ID_LEN);
		}

		public int set_NETDATE(NETDATE date) {
			if (date!=null) {
			set_INT(date.m_Year);
			set_BYTE(date.m_Month);
			set_BYTE(date.m_Day);
			}
			off += 8;
			return off;
		}

		public int set_NETTIME(NETTIME time) {
			if (time!=null) {
			set_INT(time.m_Msec);
			set_BYTE(time.m_Hour);
			set_BYTE(time.m_Minute);
			set_BYTE(time.m_Second);
			}
			off += 8;
			return off;
		}

		public int set_DATETIME(DATETIME datetime) {
			if (datetime!=null) {
			set_NETDATE(datetime.m_Date);
			set_NETTIME(datetime.m_Time);
			} else {
				set_NETDATE(null);
				set_NETTIME(null);
			}
			return off;
		}

		int offset(int add) {
			int i = off;
			off += add;
			for (; i < off; ++i) {
				bytes[i] = 0;
			}
			return off;
		}

		public int set_CIDS(String[] strs, int n) {
			int m = Math.min(strs.length, n);
			for (int i = 0; i < m; ++i) {
				set_CID(strs[i]);
			}
			if (m < n) {
				offset((n - m) * ID_LEN);
			}
			return off;
		}

		public int set_WORKATT(WORKATT workatt) {
			set_INTS(workatt.m_DevCtx, TGROUP_NUM);
			set_INT(workatt.m_DefaultRight);
			set_INT(workatt.m_BaseSet);
			set_CID(workatt.m_SuperPassword);
			set_BYTE((byte) workatt.m_DoorMangetTime);
			set_BYTE((byte) workatt.m_LockTime);
			set_BYTE((byte) workatt.m_VerifyMode);
			set_BYTE((byte) workatt.m_OutputCtx);
			set_INT(workatt.m_OutputCtx);
			return off;
		}

		public int set_USER(USER obj) {
			int off1 = off;
			set_INT(obj.FeatLen());
			set_INT(obj.PicLen());
			set_INT(obj.m_RightID);
			set_CID(obj.m_ImageSID);
			set_CID(obj.m_UserID);
			set_CID(obj.m_Admin);
			set_CID(obj.m_AppendID);
			set_CID(obj.m_UserName);
			set_CID(obj.m_Department);
			set_DATETIME(obj.m_DeadLine);
			set_DATETIME(obj.m_RegistTime);
			set_DATETIME(obj.m_LastUpdTime);
			set_DATETIME(obj.m_TypeUpdTime);
			set_BYTE(obj.m_UserFlag);
			set_BYTE(obj.m_UserType);
			if (obj.m_Feat != null) {
				set_BYTES(obj.m_Feat, obj.m_Feat.length);
			}
			if (obj.m_Pic != null) {
				set_BYTES(obj.m_Pic, obj.m_Pic.length);
			}
			off = off1 + obj.size();
			return off;
		}

		int set_RECORDRANGE(RECORDRANGE obj) {
			int off1 = off;
			set_INT(obj.m_Count);
			set_INT(obj.m_Flag);
			set_CID(obj.m_SID);
			off = off1 + RECORDRANGE.size;
			return off;
		}

		int set_DSPTIMEG(DSPTIMEG obj) {
			int off1 = off;
			set_INT(obj.m_TGID);
			set_DATETIME(obj.m_Start);
			set_DATETIME(obj.m_End);
			set_BYTE(obj.m_TGType);
			set_BYTE(obj.m_CheckFlag);
			set_BYTE(obj.m_WeekFlag);
			set_BYTE(obj.m_Reserve);
			off = off1 + DSPTIMEG.size;
			return off;
		}

		int set_DSPRIGHT(DSPRIGHT obj) {
			int off1 = off;
			set_INT(obj.m_RightID);
			set_INTS(obj.m_TimeGID, TGROUP_NUM);
			set_INT(obj.m_Mode);
			off = off1 + DSPTIMEG.size;
			return off;
		}

		int set_USERGROUP(USERGROUP obj) {
			int off1 = off;
			set_INT(obj.m_GroupID);
			set_BYTE(obj.m_GroupFlag);
			set_BYTE(obj.m_Param);
			set_BYTE(obj.m_NormalValid);
			set_BYTE(obj.m_ForceValid);
			set_CIDS(obj.m_NormalUsers, MAX_GROUPUSER);
			set_CIDS(obj.m_ForceUsers, MAX_GROUPUSER);
			off = off1 + USERGROUP.size;
			return off;
		}

		int set_IOCTRL(IOCTRL obj) {
			int off1 = off;
			set_BYTE(obj.m_Source);
			set_BYTE(obj.m_Action);
			set_BYTE(obj.m_IOMode);
			set_BYTE(obj.m_IOState);
			off = off1 + USERGROUP.size;
			return off;
		}

		int set_APPUPDATE(APPUPDATE obj) {
			int off1 = off;
			set_INT(obj.m_File.length);
			set_CID(obj.m_FileName);
			set_BYTE(obj.m_TotalFiles);
			set_BYTE(obj.m_FileIndex);
			if (obj.m_File != null) {
				set_BYTES(obj.m_File, obj.m_File.length);
			}
			off = off1 + obj.size();
			return off;
		}

		int set_NETPRO(NETPRO obj) {
			int off1 = off;
			set_BYTE(obj.m_WebPro1);
			set_BYTE(obj.m_WebPro2);
			set_STRING(obj.m_WebUrl1, 127);
			set_STRING(obj.m_WebUrl2, 127);
			set_STRING(obj.m_IPAdr, 16);
			set_STRING(obj.m_MIPAdr, 16);
			set_STRING(obj.m_NetMask, 16);
			set_STRING(obj.m_GateWay, 16);
			set_STRING(obj.m_Dns, 16);
			off = off1 + NETPRO.size;
			return off;
		}

	}

	public class get_stream {
		public int off;
		byte[] bytes;

		get_stream(byte[] bytes1, int off1) {
			bytes = bytes1;
			off = off1;
		}

		public byte get_BYTE() {
			byte by = (byte) (0xff & bytes[off]);
			off += 1;
			return by;
		}

		public int get_INT() {
			int ret = (0xff & bytes[off + 0])
					| (0xff00 & (bytes[off + 1] << 8))
					| (0xff0000 & (bytes[off + 2] << 16))
					| (0xff000000 & (bytes[off + 3] << 24));
			off += 4;
			return ret;
		}

		public byte[] get_BYTES(int len) {
			if (len > 0) {
				byte[] by = new byte[len];
				bytecopy(bytes, off, by, 0, len);
				off += len;
				return by;
			}
			return null;
		}

		public String get_STRING(int len) {
			String s = "";
			try {
				s = new String(bytes, off, len, "GBK");
			} catch (Exception e) {
				// li.msg_DEBUGMSG(this, "get_STRING 出错");
				e.printStackTrace();
			}
			off += len;
			return s;
		}

		public String get_CID() {
			return get_STRING(ID_LEN);
		}

		public NETDATE get_NETDATE() {
			NETDATE obj = new NETDATE();
			obj.m_Year = get_INT();
			obj.m_Month = get_BYTE();
			obj.m_Day = get_BYTE();
			off += 2;
			return obj;
		}

		public NETTIME get_NETTIME() {
			NETTIME obj = new NETTIME();
			obj.m_Msec = get_INT();
			obj.m_Hour = get_BYTE();
			obj.m_Minute = get_BYTE();
			obj.m_Second = get_BYTE();
			off += 1;
			return obj;
		}

		public DATETIME get_DATETIME() {
			DATETIME obj = new DATETIME();
			obj.m_Date = get_NETDATE();
			obj.m_Time = get_NETTIME();
			return obj;
		}

		public VUSER get_VUSER() {
			VUSER obj = new VUSER();
			obj.m_UserType = get_INT();
			obj.m_UserID = get_CID();
			obj.m_TypeUpdTime = get_DATETIME();
			obj.m_RegistTime = get_DATETIME();
			obj.m_LastUpdTime = get_DATETIME();
			return obj;
		}

		int ALIGN4(int size) {
			return (((size) + 3) & ~3);
		}

		public USER get_USER() {
			USER obj = new USER();
			int m_FeatLen = get_INT();
			int m_PicLen = get_INT();
			obj.m_RightID = get_INT();
			obj.m_ImageSID = get_CID();
			obj.m_UserID = get_CID();
			obj.m_Admin = get_CID();
			obj.m_AppendID = get_CID();
			obj.m_UserName = get_CID();
			obj.m_Department = get_CID();

			obj.m_DeadLine = get_DATETIME();
			obj.m_RegistTime = get_DATETIME();
			obj.m_LastUpdTime = get_DATETIME();
			obj.m_TypeUpdTime = get_DATETIME();
			obj.m_UserFlag = get_BYTE();
			obj.m_UserType = get_BYTE();

			if (m_FeatLen > 0) {
				obj.m_Feat = get_BYTES(m_FeatLen);
			}
			if (m_PicLen > 0) {
				obj.m_Pic = get_BYTES(m_PicLen);
			}
			off = ALIGN4(off);
			return obj;
		}

		public RECORDRANGE get_RECORDRANGE() {
			RECORDRANGE obj = new RECORDRANGE();
			obj.m_Count = get_INT();
			obj.m_Flag = get_INT();
			obj.m_SID = get_CID();
			return obj;
		}

		public RECORD get_RECORD() {
			RECORD obj = new RECORD();
			obj.m_RecType = get_INT();
			obj.m_Score = get_INT();
			obj.m_PicBytes = get_INT();
			obj.m_ChangID = get_CID();
			obj.m_UserID = get_CID();
			obj.m_SerialID = get_CID();
			obj.m_AppendID = get_CID();
			obj.m_UserName = get_CID();
			obj.m_Department = get_CID();
			obj.m_RecTime = get_DATETIME();
			obj.m_VerifyMode = get_BYTE();
			obj.m_Source = get_BYTE();

			if (obj.m_PicBytes > 0) {
				obj.m_Pic = get_BYTES(obj.m_PicBytes);
			}
			off = ALIGN4(off);
			return obj;
		}

		public NETPRO get_NETPRO() {
			NETPRO obj = new NETPRO();
			obj.m_WebPro1 = get_BYTE();
			obj.m_WebPro2 = get_BYTE();
			obj.m_WebUrl1 = get_STRING(127);
			obj.m_WebUrl2 = get_STRING(127);
			obj.m_IPAdr = get_STRING(16);
			obj.m_MIPAdr = get_STRING(16);
			obj.m_NetMask = get_STRING(16);
			obj.m_GateWay = get_STRING(16);
			obj.m_Dns = get_STRING(16);
			return obj;
		}

	}

	public int close() {
		try {
			if (null != netInputStream) {
				netInputStream.close();
			}
			if (null != netOutputStream) {
				netOutputStream.close();
			}
			if (null != sc) {
				sc.close();
			}
		} catch (Exception e) {
			System.out.println("close 出错");
			e.printStackTrace();
		}
		return 0;
	}

	public static int _inet_addr(String ip) {
		int num = 0;
		if (ip == null) {
			return num;
		}

		try {
			ip = ip.replaceAll("[^0-9\\.]", ""); // 去除字符串前的空字符
			String[] ips = ip.split("\\.");
			if (ips.length == 4) {
				num = Integer.parseInt(ips[3], 10) * 256 * 256 * 256
						+ Integer.parseInt(ips[2], 10) * 256 * 256
						+ Integer.parseInt(ips[1], 10) * 256
						+ Integer.parseInt(ips[0], 10);
				// num = num >>> 0;
			}
		} catch (NullPointerException ex) {
			System.out.println(ip);
		}

		return num;
	}

	public static int ALIGN(int bytes, int alignments) {
		return (((bytes) + ((alignments) - 1)) & ~((alignments) - 1));
	}

	// 校验和计算
	public static int sock_checksum(byte buffer[], int off, int size) {
		int cksum = 0;
		while ((size - 1) > off) {
			int low = buffer[off];
			int high = buffer[off + 1];
			int z = (int) (((high & 0xff) << 8) | (low & 0xff));
			cksum += z;
			// li.msg_DEBUGMSG(this, "cksum = " + cksum + " z = " + z);
			off += 2;
		}

		if (size > off) {
			cksum += buffer[off];
		}

		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >> 16);
		return (~cksum) & 0xffff;
	}

	int send_cmd_packet(int m_MCmd, int m_SCmd, int m_CFlag, byte[] data,
			int len) {
		byte[] pBuffer = new byte[PACK_SIZE + OFFSET_SIZE];// 最大60K大小一个包
		int nTotalSend = 0;
		// int bOK = 1;
		int i = 0;
		int nSeq = ++m_nIndex;
		int m_Param1 = -1;
		int m_Param2;
		int cmdlen = len + size_NCMD;
		set_stream set = new set_stream(pBuffer, OFFSET_SIZE);

		set.set_INT((int) m_MCmd);
		set.set_INT((int) m_SCmd);
		set.set_INT((int) m_CFlag);
		set.set_INT((int) m_Param1);
		m_Param2 = _inet_addr(get_ip());
		set.set_INT((int) m_Param2);

		for (i = 0; i < cmdlen;) {
			int nSend;
			// 组装网络命令包 加包头
			int msg_identify = nSeq; // 标识
			int msg_offset = i * PACK_SIZE; // 偏移
			int msg_total = cmdlen; // 数据总长度
			int msg_checksum = 0; // 检验和
			int msg_len = Math.min(PACK_SIZE, (cmdlen - i)); // 数据长度

			if (len > 0 && data != null) {
				if (0 == i) {
					bytecopy(data, 0, pBuffer, OFFSET_SIZE + size_NCMD, msg_len
							- size_NCMD);
				} else {
					bytecopy(data, i, pBuffer, size_NCMD, msg_len);
				}
			}

			nSend = ALIGN(msg_len, 4) + OFFSET_SIZE;

			set = new set_stream(pBuffer, 0);
			set.set_INT((int) msg_identify);
			set.set_INT((int) msg_offset);
			set.set_INT((int) msg_len);
			set.set_INT((int) msg_total);
			msg_checksum = sock_checksum(pBuffer, 0, OFFSET_SIZE);
			set.set_INT((int) msg_checksum);

			// println("send" + nSend);

			try {
				netOutputStream.write(pBuffer, 0, nSend);
			} catch (Exception e) {
				e.printStackTrace();
				li.msg_CONNECT_CUT(this);
			}

			nTotalSend += msg_len;
			i += msg_len;
		}
		// li.msg_DEBUGMSG(this, "发送 成功:" + nTotalSend + "bytes");
		return nTotalSend;
	}

	byte[] msg_buf;
	int msg_identify = -1; // 标识
	int msg_len; // 数据长度

	int safe_read(byte[] bytes, int off, int len) throws Exception {
		int i = 0;
		for (; i < len;) {
			int readed_len = netInputStream.read(bytes, off + i, len - i);
			if (readed_len <= 0) {
				return -1;
			}
			i += readed_len;
		}
		return i;
	}

	String bytes2string(byte[] bytes, int off, int len) {
		String ss = "";
		for (int j = 0; j < len; ++j) {
			ss += " " + bytes[j];
		}
		return ss;
	}

	int recv_pack_data() throws Exception {
		byte[] pbuf = new byte[OFFSET_SIZE + 40];
		int readed_len = 0;
		get_stream get = new get_stream(pbuf, 0);

		try {
			// 获取包头
			readed_len = safe_read(pbuf, 0, OFFSET_SIZE);
			// li.msg_DEBUGMSG(this, "read " + readed_len + "bytes");
		} catch (SocketTimeoutException e) {
			// li.msg_DEBUGMSG(this, "read 包头 超时");
			return 0;
		} catch (Exception e) {
			li.msg_DEBUGMSG(this, "read 包头 出错");
			e.printStackTrace();
			// close();
			return -1;
		}

		// li.msg_VIDEO_OUT(this, bytes2string(pbuf, 0, 20));

		if (readed_len != 20) {
			li.msg_DEBUGMSG(this, "read 包头 出错 长度不对 len=" + readed_len);
			return 0;
		}

		int checksum = sock_checksum(pbuf, 0, OFFSET_SIZE);
		// 校验
		if (0 != checksum) {
			li.msg_DEBUGMSG(this, "校验出错，丢弃");
			return -3; // 校验出错，丢弃
		}

		// 查找本次标识的数据接收缓冲
		int identify = get.get_INT();
		int offset = get.get_INT();
		int len = get.get_INT();
		int total = get.get_INT();

		if (identify != msg_identify || null == msg_buf) { // 没找到，则为新视频数据包。
			if (offset != 0) { // 检验是否丢包头，否则为无命令头的非法视频数据。
				li.msg_DEBUGMSG(this, "无命令头，丢弃");
				return -1;
			}

			// 为接收本次视频数据开辟一个快缓存，并插入到MAP映射表中
			msg_buf = new byte[total];
			msg_identify = identify;
			msg_len = 0;
		}

		// 收数据
		try {
			readed_len = safe_read(msg_buf, offset, len);
			// li.msg_DEBUGMSG(this, "len " + readed_len + " " + len);
			// li.msg_DEBUGMSG(this, "read " + readed_len + "bytes");
		} catch (Exception e) {
			li.msg_DEBUGMSG(this, "safe_read 出错" + readed_len + " " + len);
			return -1;
		}

		msg_len += len;

		int ret = 0;
		if (msg_len == msg_buf.length) {
			ret = msg_len;
			msg_identify = -1;
			// li.msg_DEBUGMSG(this, "recv_pack_data " + ret + "bytes");
		}
		return ret;
	}

	public int GET_ERR(int state) {
		int errcode = ((state) & 0xff);
		switch (errcode) {
		case SUC_EXECUT:
			li.msg_DEBUGMSG(this, "命令成功");
			break;
		case ERR_EXECUT:
			li.msg_DEBUGMSG(this, "命令失败");
			break;
		case ERR_BUSY:
			li.msg_DEBUGMSG(this, "错误: 设备忙");
			break;
		case ERR_LIMIT:
			li.msg_DEBUGMSG(this, "错误: 已达上限");
			break;
		case ERR_NOFIND:
			li.msg_DEBUGMSG(this, "错误: 没有找到对应数据");
			break;
		case ERR_SAVE_FAIL:
			li.msg_DEBUGMSG(this, "错误: 数据保存失败");
			break;
		case ERR_SET_FAIL:
			li.msg_DEBUGMSG(this, "错误: 设置失败");
			break;
		case ERR_VERIFY_FAIL:
			li.msg_DEBUGMSG(this, "错误: 验证失败");
			break;
		case ERR_FROMAT:
			li.msg_DEBUGMSG(this, "错误: 格式化失败");
			break;
		case ERR_PARAM:
			li.msg_DEBUGMSG(this, "错误: 参数错误");
			break;
		case ERR_DISABLE:
			li.msg_DEBUGMSG(this, "错误: 要求执行的功能没有使能");
			break;
		case ERR_SUPPORT:
			li.msg_DEBUGMSG(this, "错误: 不支持的命令");
			break;
		case ERR_INPUTDATA:
			li.msg_DEBUGMSG(this, "错误: 网络端传输的数据有异常");
			break;
		}
		return errcode;
	}

	int m_Author;

	int msg_proc(int m_MCmd, int m_SCmd, int m_CFlag, int m_Param1, int m_Param2) {
		int MCmd = m_MCmd;
		int off = size_NCMD;
		// int eResult = GET_ERR(m_CFlag);
		get_stream get = new get_stream(msg_buf, off);
		li.msg_DEBUGMSG(this, "收到消息 m_MCmd=" + m_MCmd + "长度: " + msg_buf.length);
		switch (MCmd) {
		case CMD_M_AUTH:
			m_Author = (m_CFlag & F_AUTH_VERIFY);
			// if ( (pCMD.m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER :
			// DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pCMD.m_Param1,
			// GET_ERR(pCMD.m_CFlag), pvClientData);
			li.msg_AUTH(this, m_CFlag);
			break;
		case CMD_M_DEVINFO:
			BASEINFO baseinfo = new BASEINFO();
			baseinfo.m_DevType = get.get_INT();
			baseinfo.m_LimitUser = get.get_INT();
			baseinfo.m_Limit1NUser = get.get_INT();
			baseinfo.m_SN = get.get_CID();
			baseinfo.m_Ver = get.get_CID();
			baseinfo.m_Space = get.get_CID();
			li.msg_DEVINFO(this, baseinfo);
			break;
		case CMD_M_STATIS:
			STATIS statis = new STATIS();
			statis.m_TotalUsers = get.get_INT();
			statis.m_NoFeatUser = get.get_INT();
			statis.m_TotalDays = get.get_INT();
			statis.m_TotalRecords = get.get_INT();
			statis.m_TotalAlarm = get.get_INT();
			statis.m_TotalDspLog = get.get_INT();
			statis.m_TotalChangeImage = get.get_INT();
			li.msg_STATIS(this, statis);
			break;
		case CMD_M_NETPRO:
			NETPRO netpro = get.get_NETPRO();
			li.msg_NETPRO(this, netpro);
			break;

		case CMD_M_USER:
			if ((m_CFlag & F_DL_ID) != 0) {
				if (0 == GET_ERR(m_CFlag)) {
					int m_Count = get.get_INT();
					VUSER[] vuser = new VUSER[m_Count];
					for (int i = 0; i < m_Count; ++i) {
						vuser[i] = get.get_VUSER();
					}
					li.msg_VUSER(this, vuser);
				}
			}
			break;
		case CMD_M_USER_EX:
			if (0 == GET_ERR(m_CFlag)) {
				switch (m_SCmd) {
				case CMD_S_GET:
					int m_Count = get.get_INT();
					li.msg_DEBUGMSG(this, "CMD_M_USER_EX m_Count: " + m_Count
							+ " msg_buf.len: " + msg_buf.length);
					if (m_Count > 0) {
						USER[] user = new USER[m_Count];
						for (int i = 0; i < m_Count; ++i) {
							user[i] = get.get_USER();
						}
						li.msg_USER(this, user);
					}
					break;
				default:
					li.msg_DEBUGMSG(this, "暂不支持的子命令  m_SCmd=" + m_SCmd);
					break;
				}
			}
			// dev_insert_users(s->user, pUserBatch->m_pUserArray,
			// pUserBatch->m_nCount, 1);
			// listbox_set_item_count(s->lb_reg, s->user->m_nCount);
			break;

		case CMD_M_VERIFY:
		case CMD_M_ALARM:
			if (0 == GET_ERR(m_CFlag)) {
				RECORD[] record = new RECORD[1];
				record[0] = get.get_RECORD();
				li.msg_RECORD(this, m_MCmd, record);
			}
			break;
		case CMD_M_OPTLOG:
		case CMD_M_CIMAGE:
		case CMD_M_RANGE:
			if (0 == GET_ERR(m_CFlag)) {
				int m_Count = get.get_INT();
				li.msg_DEBUGMSG(this, "" + m_Count);
				RECORDRANGE[] range = new RECORDRANGE[m_Count];
				for (int i = 0; i < m_Count; ++i) {
					range[i] = get.get_RECORDRANGE();
				}
				li.msg_RECORDRANGE(this, m_MCmd, range);
			}
			break;

		case CMD_M_RECORD_EX:
			if (0 == GET_ERR(m_CFlag)) {
				switch (m_SCmd) {
				case CMD_M_VERIFY:
				case CMD_M_ALARM:
					int m_Count = get.get_INT();
					RECORD[] record = new RECORD[m_Count];
					for (int i = 0; i < m_Count; ++i) {
						record[i] = get.get_RECORD();
					}
					li.msg_RECORDEX(this, m_MCmd, record);
					break;
				}
			}
			break;
		default:
			break;
		}
		return 0;
	}

	public void run() {
		msg_len = 0;
		msg_identify = -1;
		while (!sc.isClosed()) {
			try {
				int ret = recv_pack_data();
				if (ret > 0) {
					get_stream get = new get_stream(msg_buf, 0);
					int m_MCmd = get.get_INT();
					int m_SCmd = get.get_INT();
					int m_CFlag = get.get_INT();
					int m_Param1 = get.get_INT();
					int m_Param2 = get.get_INT();
					if (CMD_M_HEART == m_MCmd) {
						if (SUC_EXECUT == GET_ERR(m_CFlag)) {
							// 发送心跳包
							send_cmd_packet(CMD_M_HEART, CMD_S_SET, 0, null, 0);
						}
					} else {
						msg_proc(m_MCmd, m_SCmd, m_CFlag, m_Param1, m_Param2);
					}
				} else if (ret < 0) {
					close();
				}
			} catch (Exception e) {
				close();
				li.msg_DEBUGMSG(this, "recv_pack_data 出错, 主动断开");
				return;
			}
		}
	}

	// 功 能 设备验证或设置验证
	// 参 数 cszDevAddr设备地址
	// cName 用户名
	// cPws 密码
	// nFlag 1=验证 2=设置
	// 说 明
	public int cmd_SetDevAuth(String sLoginName, String sPassword, int nFlag) {
		byte pAuth[] = new byte[2 * ID_LEN];
		set_stream set = new set_stream(pAuth, 0);
		set.set_CID(sLoginName);
		set.set_CID(sPassword);
		// 组命令数据
		return send_cmd_packet(CMD_M_AUTH, CMD_S_SET, ((nFlag == 2) ? 0
				: F_AUTH_VERIFY), pAuth, pAuth.length);
	}

	// 功 能 设置设备所在区域地址
	// 参 数
	// cszDevAddr 设备地址
	// cszRgnAddr 设备区域地址
	// 说 明 当设备建立连接后可将设备设置到某一个区域，以后及可对这个区域进行搜索设备
	public int cmd_SetDevRegionAddr(String cszGroupIP) {
		byte cid[] = new byte[ID_LEN];
		set_stream set = new set_stream(cid, 0);
		set.set_CID(cszGroupIP);
		return send_cmd_packet(CMD_M_MCAST, CMD_S_SET, 0, cid, cid.length);
	}

	public int cmd_GetDevRgn() {
		return send_cmd_packet(CMD_M_MCAST, CMD_S_GET, 0, null, 0);
	}

	public static int TRUE = 1;
	public static int FALSE = 0;
	public static int size_DATETIME = 16;

	// 功 能 设置设备系统时间
	// 参 数
	// cszDevAddr 设备地址
	// cSysTime 时间信息
	// 说 明 DevOperResultNotify回调参数对应类型：
	// eType = DEV_SYS_TIME_OPER
	// eFlag = DEV_OPER_UPLOAD ，
	// pvContent = NULL
	public int cmd_SetDevTime(DATETIME cTime) {
		byte[] dt = new byte[size_DATETIME];
		set_stream set = new set_stream(dt, 0);
		set.set_DATETIME(cTime);
		return send_cmd_packet(CMD_M_TIME, CMD_S_SET, 0, dt, size_DATETIME);
	}

	// 功 能 获取设备系统时间
	// 参 数 cszDevAddr 设备地址
	// 说 明 DevOperResultNotify回调参数对应类型：
	// eType = DEV_SYS_TIME_OPER
	// eFlag = DEV_OPER_DOWNLOAD，
	// pvContent = DEV_DATETIME*
	public int cmd_GetDevTime() {
		return send_cmd_packet(CMD_M_TIME, CMD_S_GET, 0, null, 0);
	}

	// 功 能 获取设备基本信息
	// 参 数 cszDevAddr 设备地址
	// 说 明
	public int cmd_GetDevBaseInfo() {
		return send_cmd_packet(CMD_M_DEVINFO, CMD_S_GET, 0, null, 0);
	}

	// 功 能 获取设备统计信息
	// 参 数 cszDevAddr 设备地址
	// 说 明
	public int cmd_GetDevStatisInfo() {
		return send_cmd_packet(CMD_M_STATIS, CMD_S_GET, 0, null, 0);
	}

	// 功 能 获取人脸机工作属性
	// 参 数
	// cszDevAddr 人脸机地址
	// nSeq 操作序列号
	// 说 明 工作属性DEV_WORKATT通过回调函数给出。
	public int cmd_GetDevWorkAttInfo() {
		return send_cmd_packet(CMD_M_DEVSET, CMD_S_GET, 0, null, 0);
	}

	// 功 能 设置设备工作属性信息
	// 参 数
	// cszDevAddr 设备地址
	// cWorkAtt 工作属性结构体
	// nSeq 操作序列号

	public int cmd_SetDevWorkAttInfo(WORKATT workatt) {
		int pSet_len = 4 * (TGROUP_NUM + 3) + 4 + ID_LEN + 20;
		byte[] pSet = new byte[pSet_len];
		set_stream set = new set_stream(pSet, 0);
		set.set_WORKATT(workatt);
		return send_cmd_packet(CMD_M_DEVSET, CMD_S_SET, 0, pSet, pSet_len);
	}

	// public static int USER_JPEG_SIZE = (64 * 1024);
	// public static int REC_JPEG_SIZE = (18 * 1024);
	// public static int FEAT_SIZE = (90 * 1024);

	// 功 能 上传指定用户
	// 参 数
	// cszDevAddr 设备地址
	// cUser 用户信息结构体
	// nSeq 操作序列号
	// 说 明
	// 用户无人脸库时，DEV_USER中的 m_FeatLen = 0， m_FeatData = NULL
	// 用户无注册照时，DEV_USER中的 m_PicLen= 0， m_PicData = NULL
	// DevOperResultNotify回调参数对应类型：eType = DEV_USER_OPER
	// eFlag = DEV_OPER_UPLOAD，pvContent= NULL
	public int cmd_SetUser(USER cUser) {
		byte[] pUser = new byte[cUser.size()];
		set_stream set = new set_stream(pUser, 0);
		int len = set.off;
		return send_cmd_packet(CMD_M_USER, CMD_S_SET, 0, pUser, len);
	}

	// 功 能 上传实时加载用户
	// 参 数
	// cszDevAddr 设备地址
	// cUser 用户信息结构体
	// m_CFlag 实时加载信息 [F_DL_APPENDID|F_DL_PIC|F_DL_FEAT]
	// nSeq 操作序列号
	// 说 明 设备端请求客户端上传某用户时，通过此接口上传所请求用户信息，
	// cRtLoad为设备端请求时传过来的信息，客户端禁止修改。此函数实现远程用户加载功能。
	public int cmd_SetRealTimeUser(USER cUser, int m_CFlag) {
		byte[] pUser = new byte[cUser.size()];
		set_stream set = new set_stream(pUser, 0);
		int len = set.off;
		return send_cmd_packet(CMD_M_USER, CMD_S_REALTIME_LOAD, m_CFlag, pUser,
				len);
	}

	// 功 能 远程实时采集用户特征
	// 参 数
	// cszDevAddr 设备地址
	// cUser 用户信息
	// 说 明 管理端录入用户资料时，可通过该函数将用户资料发送的人脸机，控制人脸机采集该用户的人脸。
	// 远程用户采集功能，使得直接在管理端就可以采集用户，在配合远程视频的情况下采集的可视化程度和直接在机器上采集一样。
	// 如果使能了(用户采集实时同步)。远程采集的用户也会实时的发回管理端.
	public int cmd_SetRealTimeUserCap(String cUserID) {
		USER cUser = new USER();
		cUser.m_UserID = cUserID;
		byte[] pUser = new byte[cUser.size()];
		set_stream set = new set_stream(pUser, 0);
		set.set_USER(cUser);
		int len = set.off;
		li.msg_DEBUGMSG(this, "send_cmd_packet len=" + len + " cUserID = " + cUserID + "cUser.size()=" + cUser.size());
		return send_cmd_packet(CMD_M_USER, CMD_S_REALTIME_CAP, 0, pUser, len);
	}

	// 功 能 删除指定用户
	// 参 数
	// cszDevAddr 设备地址
	// cID 用户编号
	// nSeq 操作序列号
	// 说 明 删除成功与否，通过操作回调函数给出结果
	// DevOperResultNotify回调参数对应类型：
	// eType = DEV_USER_OPER
	// eFlag = DEV_OPER_DELETE，
	// pvContent = NULL
	public int cmd_DelUser(String cID) {
		byte[] cid = new byte[ID_LEN];
		set_stream set = new set_stream(cid, 0);
		set.set_CID(cID);
		return send_cmd_packet(CMD_M_USER, CMD_S_DEL, 0, cid, ID_LEN);
	}

	// 功 能 下载指定用户
	// 参 数
	// cszDevAddr 设备地址
	// cID 用户ID
	// nFlag F_DL_PIC 需下载用户照片
	// F_DL_FEAT 需要下载人脸特征
	// F_DL_PIC|F_DL_FEAT=两者
	// 为0默认下载文字信息
	// nSeq 操作序列号
	// 说 明 DevOperResultNotify回调参数对应类型：
	// eType = DEV_USER_OPER
	// eFlag = DEV_OPER_DOWNLOAD，
	// pvContent = USER*
	public int cmd_GetSingleUser(String cID, int m_CFlag) {
		byte[] cid = new byte[ID_LEN];
		set_stream set = new set_stream(cid, 0);
		set.set_CID(cID);
		return send_cmd_packet(CMD_M_USER, CMD_S_GET, 0, cid, ID_LEN);
	}

	// 功 能 下载所有用户
	// 参 数 cszDevAddr 设备地址
	// 说 明
	public int cmd_GetAllUser() {
		return send_cmd_packet(CMD_M_USER, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// 功 能 下载某一时间段内的注册用户
	// 参 数
	// cszDevAddr 设备地址
	// cStart 起始日期
	// cEnd 终止日期
	// 说 明
	public int cmd_GetSegTimeUser(NETDATE cStart, NETDATE cEnd) {
		byte[] seg = new byte[16];
		set_stream set = new set_stream(seg, 0);
		set.set_NETDATE(cStart);
		set.set_NETDATE(cEnd);
		return send_cmd_packet(CMD_M_USER, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				seg, 16);
	}

	// 功 能 下载识别记录区间
	// 参 数
	// cszDevAddr 设备地址
	// nSeq 操作序列
	// 说 明 DevOperResultNotify回调参数对应类型：
	// eType = DEV_RECORD_OPER
	// eFlag = DEV_OPER_DOWNLOAD
	// pvContent = DEV_DL_RECORD_ARRAY*
	// 参考DEV_DL_RECORD_ARRAY结构，一天为一个区间，该函数返回识别记录的所有区间。
	public int cmd_GetAllIdentifyRecord() {
		return send_cmd_packet(CMD_M_VERIFY, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// 功 能 下载指定时间段识别记录
	// 参 数 cszDevAddr 设备地址
	// cStart 起始日期
	// cEnd 终止日期
	// 说 明
	public int cmd_GetSegTimeIdentifyRecord(NETDATE cStart, NETDATE cEnd) {
		byte[] seg = new byte[NETDATE.size * 2];
		set_stream set = new set_stream(seg, 0);
		set.set_NETDATE(cStart);
		set.set_NETDATE(cEnd);
		return send_cmd_packet(CMD_M_USER, CMD_S_GET,
				F_DL_ID | F_ID_TIME_RANGE, seg, NETDATE.size * 2);
	}

	// 功 能 获取区间识别记录/报警记录指定ID的后续区间
	// 参 数
	// cszDevAddr 设备地址
	// cRange ID 区间
	// m_CFlag F_DL_PIC=需要下载记录照片
	// bKnowRange true=具体数据， false=区间集合
	// nSeq 操作序列
	// 说 明 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
	// DevOperResultNotify回调参数对应类型：
	// 下载区间时
	// pvContent = DEV_DL_RECORD_ARRAY*
	// cRange.m_SID = 指定SN
	// cRange.m_Count = -1;
	// cRange.m_bOpenRange = TRUE;
	// bKnowRange = FALSE，flag = 1（无效参数）
	// 下载具体记录时
	// DevOperResultNotify回调参数对应类型：
	// pvContent = DEV_RECORD*
	public int cmd_GetRangeRec(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	// 功 能 下载报警记录区间
	// 参 数
	// cszDevAddr 设备地址
	// nSeq 操作序列
	// 说 明 DevOperResultNotify回调参数对应类型：
	// eType = DEV_ALARM_OPER
	// eFlag = DEV_OPER_DOWNLOAD，
	// pvContent = DEV_DL_RECORD_ARRAY*
	// 参考DEV_DL_RECORD_ARRAY结构，报警记录只有一个区间
	public int cmd_GetAllAlarmRecord() {
		return send_cmd_packet(CMD_M_ALARM, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// 功 能 下载指定时间段报警记录
	// 参 数 cszDevAddr 设备地址
	// cStart 起始日期
	// cEnd 终止日期
	// 说 明
	public int cmd_GetSegTimeAlarmRecord(NETDATE cStart, NETDATE cEnd) {
		byte[] seg = new byte[NETDATE.size * 2];
		set_stream set = new set_stream(seg, 0);
		set.set_NETDATE(cStart);
		set.set_NETDATE(cEnd);
		return send_cmd_packet(CMD_M_ALARM, CMD_S_GET, F_DL_ID
				| F_ID_TIME_RANGE, seg, NETDATE.size * 2);
	}

	// 功 能 下载区间报警记录
	// 参 数 cszDevAddr 设备地址
	// cRange ID区间
	// flag DL_DEV_RECORD_PIC=需要下载记录照片 0x00=不下载照片, 0x01下载照片
	// bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
	// 说 明 此接口用于从某一条精确的记录ID开始下载nRang条数据
	public int cmd_GetRangeAlarm(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	// 功 能 下载所有日志
	// 参 数 cszDevAddr 设备地址
	// 说 明
	public int cmd_GetLog() {
		return send_cmd_packet(CMD_M_OPTLOG, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// 功 能 下载区间日志
	// 参 数 cszDevAddr 设备地址
	// cRange ID区间
	// bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
	// 说 明 此接口用于从某一条精确的记录ID开始下载nRang条数据
	public int cmd_GetRangeLog(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	// 功 能 获取变更注册照记录区间
	// 参 数
	// cszDevAddr 设备地址
	// nSeq 操作序列
	// 说 明
	// DevOperResultNotify回调参数对应类型：
	// eType = DEV_CHANGEIMAGE_OPER
	// eFlag = DEV_OPER_DOWNLOAD，
	// pvContent = DEV_DL_RECORD_ARRAY*
	// 参考DEV_DL_RECORD_ARRAY结构，变更注册照只有一个区间
	public int cmd_GetChangeImage() {
		return send_cmd_packet(CMD_M_CIMAGE, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// 功 能 下载指定区间的变更注册照记录或者查询区间数据的具体大小
	// 参 数
	// cszDevAddr 设备地址
	// cRange 区间, 参考区间定义
	// flag DL_DEV_RECORD_PIC=需要下载记录照片
	// bKnowRange true=具体数据， false=区间集合
	// nSeq 操作序列号
	// 说 明 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
	// 下载区间时
	// DevOperResultNotify回调参数对应类型：
	// pvContent = DEV_DL_RECORD_ARRAY*
	// cRange.m_SID = 指定SN
	// cRange.m_Count = -1;
	// cRange.m_bOpenRange = TRUE;
	// bKnowRange = FALSE，flag = 1（无效参数）
	// 下载具体记录时
	// DevOperResultNotify回调参数对应类型：
	// pvContent=DEV_RECORD*
	public int cmd_GetRangeCImage(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	public int cmd_TimeGroup_impl(DSPTIMEG[] cArray, int m_SCmd, int m_CFlag) {
		byte[] nData = new byte[cArray.length * DSPTIMEG.size + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cArray.length);

		for (int i = 0; i < cArray.length; ++i) {
			set.set_DSPTIMEG(cArray[i]);
		}

		return send_cmd_packet(CMD_M_TGROUP, m_SCmd, m_CFlag, nData,
				nData.length);
	}

	// 功 能 设置时间组
	// 参 数 cszDevAddr 设备地址
	// cTGArray 时间组
	// 说 明
	public int cmd_SetTimeGroup(DSPTIMEG[] cTGArray) {
		return cmd_TimeGroup_impl(cTGArray, CMD_S_SET, F_DL_NORMAL_TG
				| F_DL_HOLIDAY_TG);
	}

	// 功 能 获取时间组
	// 参 数 dev_addr 设备地址
	// cTGArray 时间组
	// m_CFlag 时间组类型
	// 说 明 cTGArray.m_nCount=0时表示下载eType类型的所有时间组
	public int cmd_GetTimeGroup(DSPTIMEG[] cTGArray, int m_CFlag) {
		return cmd_TimeGroup_impl(cTGArray, CMD_S_GET, m_CFlag);
	}

	// 功 能 删除时间组
	// 参 数 cszDevAddr 设备地址
	// cTGArray 时间组
	// eType 时间组类型
	// 说 明 eType.m_nCount=0时表示删除tg_type类型所有时间组
	public int cmd_DelTimeGroup(DSPTIMEG[] cTGArray, int m_CFlag) {
		return cmd_TimeGroup_impl(cTGArray, CMD_S_DEL, m_CFlag);
	}

	public int cmd_Right_impl(DSPRIGHT[] cArray, int m_SCmd) {
		byte[] nData = new byte[cArray.length * DSPRIGHT.size + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cArray.length);

		for (int i = 0; i < cArray.length; ++i) {
			set.set_DSPRIGHT(cArray[i]);
		}

		return send_cmd_packet(CMD_M_DSPRIGHT, m_SCmd, 0, nData, nData.length);
	}

	// 功 能 设置权限
	// 参 数 cszDevAddr 设备地址
	// cRTArray 权限
	// 说 明
	public int cmd_SetRight(DSPRIGHT[] cRTArray) {
		return cmd_Right_impl(cRTArray, CMD_S_SET);
	}

	// 功 能 发送获取权限
	// 参 数 cszDevAddr 设备地址
	// cRTArray 权限
	// 说 明 当rt_array.m_nCount=0时表示下载所有权限
	public int cmd_GetRight(DSPRIGHT[] cRTArray) {
		return cmd_Right_impl(cRTArray, CMD_S_GET);
	}

	// 功 能 删除权限
	// 参 数 cszDevAddr 设备地址
	// cRTArray 权限
	// 说 明 当rt_array.m_nCount=0时表示删除所有权限
	public int cmd_DelRight(DSPRIGHT[] cRTArray) {
		return cmd_Right_impl(cRTArray, CMD_S_DEL);
	}

	public int cmd_UserGroup_impl(USERGROUP[] cArray, int m_SCmd) {
		byte[] nData = new byte[cArray.length * DSPRIGHT.size + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cArray.length);

		for (int i = 0; i < cArray.length; ++i) {
			set.set_USERGROUP(cArray[i]);
		}

		return send_cmd_packet(CMD_M_UGROUP, m_SCmd, 0, nData, nData.length);
	}

	// 功 能 设置用户组
	// 参 数 cszDevAddr 设备地址
	// cUGrray 用户组
	// 说 明
	public int cmd_SetUserGroup(USERGROUP[] cUGrray) {
		return cmd_UserGroup_impl(cUGrray, CMD_S_SET);
	}

	// 功 能 发送获取用户组
	// 参 数 cszDevAddr 设备地址
	// ug_array 用户组
	// 说 明 当rt_array.m_nCount=0时表示下载所有用户组
	public int cmd_GetUserGroup(USERGROUP[] cUGrray) {
		return cmd_UserGroup_impl(cUGrray, CMD_S_GET);
	}

	// 功 能 删除用户组
	// 参 数 cszDevAddr 设备地址
	// ug_array 用户组
	// 说 明 当rt_array.m_nCount=0时表示删除所有用户组
	public int cmd_DelUserGroup(USERGROUP[] cUGrray) {
		return cmd_UserGroup_impl(cUGrray, CMD_S_DEL);
	}

	// 功 能 设置开关门状态
	// 参 数
	// cszDevAddr 设备地址
	// bEOpen true=紧急开门， false=紧急关门
	// 说 明
	public int cmd_SetOpenDoorState(int eState) {
		return send_cmd_packet(CMD_M_EOPEN, CMD_S_SET, eState, null, 0);
	}

	// 功 能 获取开关门状态
	// 参 数 cszDevAddr 设备地址
	// bEOpen true=紧急开门， false=紧急关门
	// 说 明
	public int cmd_GetOpenDoorState() {
		return send_cmd_packet(CMD_M_EOPEN, CMD_S_GET, 0, null, 0);
	}

	// 功 能 远程开门
	// 参 数 cszDevAddr 设备地址
	// bEOpen true=开门， false=关门
	// pDoorArray 门点数组
	// size 数组大小
	// 说 明
	public int cmd_SetRemoteOpenDoor(int bOpen) {
		int nFlag = 0;
		if (0 != bOpen) {
			nFlag = F_DOOR_OPEN;
		} else {
			nFlag = F_DOOR_CLOSE;
		}
		return send_cmd_packet(CMD_M_REMOTE_OPEN, CMD_S_SET, nFlag, null, 0);
	}

	// 功 能 IO设备控制
	// 参 数
	// cszDevAddr 设备地址
	// DEV_IOCTRL 控制信息
	// nSeq 命令执行序列号
	// 说 明 DEV_IOCTRL:m_Source指定要做操作的端口，DEV_IOCTRL:m_Action要执行的动作
	// 打开门1，m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
	// 打辅助1，m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
	// 0x1f和015具体怎么解析的，请参考485地址解析
	public int cmd_IOCtrl(IOCTRL ioctrl) {
		byte[] nData = new byte[IOCTRL.size];
		set_stream set = new set_stream(nData, 0);
		set.set_IOCTRL(ioctrl);
		return send_cmd_packet(CMD_M_IOCTRL, CMD_S_SET, 0, nData, IOCTRL.size);
	}

	// 功 能 格式化设备
	// 参 数 cszDevAddr 设备地址
	// 说 明
	public int cmd_SetFormat() {
		return send_cmd_packet(CMD_M_FORMAT, CMD_S_SET, 0, null, 0);
	}

	// 功 能 设备程序更新
	// 参 数 cszDevAddr 设备地址
	// cAppData 程序更新结构体
	// 说 明
	public int cmd_SetUpdate(APPUPDATE cAppData) {
		byte[] nData = new byte[cAppData.size()];
		set_stream set = new set_stream(nData, 0);
		set.set_APPUPDATE(cAppData);
		return send_cmd_packet(CMD_M_APPUPDATE, CMD_S_SET, 0, nData,
				nData.length);
	}

	// 功 能 批量上传用户
	// 参 数
	// cszDevAddr 设备地址
	// cUsers 批量用户数据
	// nFlag F_DL_PIC //照片
	// DL_DEV_USER_FEAT //人脸特征
	// DL_DEV_USER_TEXT //用户文字信息
	// 说 明: 批量上传用户必须包含用户文字信息，组合如下
	// nFlag = DL_DEV_USER_TEXT 上传文字信息
	// nFlag = DL_DEV_USER_TEXT|F_DL_PIC 文字+注册照
	// nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT 文字+人脸库
	// nFlag = DL_DEV_USER_TEXT|F_DL_PIC|DL_DEV_USER_FEAT文字+注册照+人脸库
	// DevOperResultNotify回调参数对应类型：
	// eType = DEV_USER_EX_OPER
	// eFlag = DEV_OPER_UPLOAD ，
	// pvContent = DEV_BATCH_USER*。
	// 在OPER_SUCCESS
	// 情况下，批量上传的用户个数与返回的用户个数一致。否则设备保存失败。批量上传用户数设定在5个以下(和网络带宽，主机处理速度有关)。
	public int cmd_SetUserEx(USER[] cUsers, int nFlag) {
		int sz = 0;
		for (int i = 0; i < cUsers.length; ++i) {
			sz += cUsers[i].size();
		}
		byte[] nData = new byte[sz + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cUsers.length);

		for (int i = 0; i < cUsers.length; ++i) {
			set.set_USER(cUsers[i]);
		}

		return send_cmd_packet(CMD_M_USER_EX, CMD_S_SET, USER_TEXT, nData,
				nData.length);
	}

	public int cmd_UserEx_impl(String[] pUserIDArray, int m_SCmd, int m_CFlag,
			int max_n) {
		int ret = 0;

		for (int j = 0; j < pUserIDArray.length;) {
			int m_Count = Math.min(max_n, pUserIDArray.length - j);
			byte[] nData = new byte[ID_LEN * m_Count + 8];
			set_stream set = new set_stream(nData, 0);
			set.set_INT(m_Count);
			for (int i = 0; i < m_Count; ++i) {
				set.set_CID(pUserIDArray[j + i]);
			}

			ret = send_cmd_packet(CMD_M_USER_EX, m_SCmd, m_CFlag, nData,
					nData.length);
			j += m_Count;
		}

		return ret;
	}

	// 功 能 批量下载用户
	// 参 数
	// cszDevAddr 设备地址
	// pUserIDArray 用户ID数组首地址
	// nIDCount 数组元素个数
	// nFlag 同cmd_SetUserEx
	// 说 明 DevOperResultNotify回调参数对应类型：
	// eType = USER_TEXT DL_FEAT DL_IMAGE
	// eFlag = DEV_OPER_DOWNLOAD ，
	// pvContent= DEV_BATCH_USER*。
	// OPER_SUCCESS 情况下，如果批量下载的用户数与要求下载的用户数不一致，可以通过
	// cmd_GetAllUser() 来确认用户是否真的存在于设备上。 批量下载用户数设定在5个以下(和网络带宽，主机处理速度有关)。
	public int cmd_GetUserEx(String[] pUserIDArray, int m_CFlag) {
		return cmd_UserEx_impl(pUserIDArray, CMD_S_GET, m_CFlag, 5);
	}

	// 功 能 批量删除用户
	// 参 数
	// cszDevAddr 设备地址
	// pUserIDArray 待删除用户的编号数组
	// nIDCount 数组元素个数
	// 说 明 DevOperResultNotify回调参数对应类型：
	// eType = DEV_USER_EX_OPER
	// eFlag = DEV_OPER_DELETE，
	// pvContent = DEV_BATCH_USER*。
	// 在OPER_SUCCESS 情况下，批量删除的用户个数与返回的用户个数不一致，可以通过cmd_GetAllUser()
	// 来确认用户是否真的存在于设备上。单次批量删除用户数设定在10个以下为好。
	public int cmd_DelUserEx(String[] pUserIDArray) {
		return cmd_UserEx_impl(pUserIDArray, CMD_S_DEL, 0, 1000);
	}

	// 功 能 批量下载记录
	// 参 数 cszDevAddr 设备地址
	// cRange ID区间
	// 说 明 包括验证记录、报警记录，操作日志、变更注册照
	public int cmd_GetRecEx(RECORDRANGE cRange) {
		byte[] nData = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(nData, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RECORD_EX, CMD_S_GET, F_DL_PIC, nData,
				nData.length);
	}

	// 功 能 重启设备
	// 参 数 cszDevAddr 设备地址
	// 说 明
	public int cmd_RebootDev() {
		return send_cmd_packet(CMD_M_REBOOT, CMD_S_SET, 0, null, 0);
	}

	// 功 能 获得/设置 设备网络信息
	// 参 数 cszDevAddr 设备地址
	// pNetPro NULL表示获得，不为空表示设置
	// 说 明
	public int cmd_SetNetPro(NETPRO pNetPro) {
		byte[] nData = new byte[NETPRO.size];
		set_stream set = new set_stream(nData, 0);
		set.set_NETPRO(pNetPro);
		return send_cmd_packet(CMD_M_NETPRO, CMD_S_SET, 0, nData, nData.length);
	}

	public int cmd_GetNetPro() {
		return send_cmd_packet(CMD_M_NETPRO, CMD_S_GET, 0, null, 0);
	}

	String devip;

	public String get_ip() {
		return devip;
	}

	public interface listener {
		public int msg_DEBUGMSG(pmdev dev, String s);

		public int msg_CONNECT_SUCCESS(pmdev dev);

		public int msg_CONNECT_CUT(pmdev dev);

		public int msg_AUTH(pmdev dev, int m_CFlag);

		public int msg_DEVINFO(pmdev dev, pmdev.BASEINFO baseinfo);

		public int msg_STATIS(pmdev dev, pmdev.STATIS statis);

		public int msg_NETPRO(pmdev dev, pmdev.NETPRO netpro);

		public int msg_RECORDRANGE(pmdev dev, int type,
				pmdev.RECORDRANGE[] range);

		public int msg_RECORD(pmdev dev, int type, pmdev.RECORD[] range);
		
		public int msg_RECORDEX(pmdev dev, int type, pmdev.RECORD[] range);

		public int msg_VUSER(pmdev dev, pmdev.VUSER[] vuser);

		public int msg_USER(pmdev dev, pmdev.USER[] vuser);

		public int msg_VIDEO_OUT(pmdev dev, int[] data, int rgb_offset, int w,
				int h, int step);

	}

	listener li;

	public int connect(String ip, String user, String pass, listener listener) {
		try {
			devip = ip;
			li = listener;
			InetSocketAddress address = null;
			address = new InetSocketAddress(ip, TCPPORT);
			sc = new Socket();
			sc.connect(address, 1000);
			sc.setSoTimeout(1000);
			netInputStream = new DataInputStream(sc.getInputStream());
			netOutputStream = new DataOutputStream(sc.getOutputStream());
			li.msg_CONNECT_SUCCESS(this);
			cmd_SetDevAuth(user, pass, 1);
			msg_identify = -1;
			start();
			// 1.send file length
		} catch (SocketTimeoutException e) {
			li.msg_DEBUGMSG(this, "connetc 超时 :" + ip + " 异常");
		} catch (UnknownHostException e) {
			li.msg_DEBUGMSG(this, "-.未知的主机名:" + ip + " 异常");
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
			li.msg_DEBUGMSG(this, "-hostName=" + ip + " portNum=" + TCPPORT
					+ "---.IO异常错误" + e.getMessage());
		}
		return 0;
	}

	public class video_th extends Thread {
		DatagramSocket ds = null;
		int m_nIndex;
		int msg_len;
		int msg_identify;
		int port;
		public volatile boolean exit = false;
		pmdev dev;

		video_th() {
		}

		int startUDP() {
			int port = 0;
			exit = true;
			try {
				ds = new DatagramSocket(0);
				ds.close();
				ds = new DatagramSocket(0);
				port = ds.getLocalPort();
				ds.setSoTimeout(1000);
			} catch (Exception e) {
				e.printStackTrace();
				throw new RuntimeException(e);
			} finally {
				start();
			}
			return port;
		}

		int clip(int x) {
			if (x < 0)
				return 0;
			if (x > 255)
				return 255;
			return x;
		}

		public static final int IMAGE_UYVY = 0x01;
		public static final int IMAGE_YUYV = 0x02;
		public static final int IMAGE_RGB24 = 0x03;

		public int[] YUYV_RGB_int(byte[] src, int off, int step, int width,
				int height) {
			int r, g, b;
			int R, G, B;
			int Y00, Y01, U, V;
			int numOfPixel = width * height;
			int[] rgb = new int[numOfPixel];
			for (int i = 0; i < height; i++) {
				int startY = off + i * step;
				int index = i * width;
				for (int j = 0; j < width; j += 2) {
					Y00 = src[startY + 0];
					U = src[startY + 1];
					Y01 = src[startY + 2];
					V = src[startY + 3];

					startY += 4;
					r = (int) (1.4075 * ((V & 0xff) - 128));
					g = (int) (0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
					b = (int) (1.779 * ((U & 0xff) - 128));
					R = ((Y00 & 0xff) + r);
					G = ((Y00 & 0xff) - g);
					B = ((Y00 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

					R = (int) ((Y01 & 0xff) + r);
					G = (int) ((Y01 & 0xff) - g);
					B = (int) ((Y01 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

				}
			}
			return rgb;
		}

		public int[] UYVY_RGB_int(byte[] src, int off, int step, int width,
				int height) {
			int r, g, b;
			int R, G, B;
			int Y00, Y01, U, V;
			int numOfPixel = width * height;
			int[] rgb = new int[numOfPixel];
			for (int i = 0; i < height; i++) {
				int startY = off + i * step;
				int index = i * width;
				for (int j = 0; j < width; j += 2) {
					U = src[startY + 0];
					Y00 = src[startY + 1];
					V = src[startY + 2];
					Y01 = src[startY + 3];

					startY += 4;
					r = (int) (1.4075 * ((V & 0xff) - 128));
					g = (int) (0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
					b = (int) (1.779 * ((U & 0xff) - 128));
					R = ((Y00 & 0xff) + r);
					G = ((Y00 & 0xff) - g);
					B = ((Y00 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

					R = (int) ((Y01 & 0xff) + r);
					G = (int) ((Y01 & 0xff) - g);
					B = (int) ((Y01 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

				}
			}
			return rgb;
		}

		public int[] RGB24_RGB_int(byte[] src, int off, int step, int width,
				int height) {
			int R, G, B;
			int numOfPixel = width * height;
			int[] rgb = new int[numOfPixel];
			for (int i = 0; i < height; i++) {
				int startY = off + i * step;
				int index = i * width;
				for (int j = 0; j < width; j += 1) {
					R = src[startY + 0];
					G = src[startY + 1];
					B = src[startY + 2];
					startY += 3;
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;
				}
			}
			return rgb;
		}

		public int close() {
			try {
				exit = true;
				// sleep(1000); // 主线程延迟1秒
				// interrupt();
				join();
			} catch (Exception e) {
				e.printStackTrace();
			}
			return 0;
		};

		public void run() {
			msg_len = 0;
			msg_identify = -1;
			exit = false;
			dev.li.msg_DEBUGMSG(dev, "video Thread run");
			int PACK_SIZEEX = PACK_SIZE + 20;
			byte[] buf = new byte[PACK_SIZEEX];
			byte[] data = null;
			int pre_identify = -1, len = 0;
			while (!ds.isClosed() && !exit) {
				try {
					DatagramPacket dp = null;
					dp = new DatagramPacket(buf, PACK_SIZEEX); // 用于接收长度为1024的数据包，并将数据包的数据部分存在buf中
					ds.receive(dp); // 从此套接字接收数据报包
					// li.msg_DEBUGMSG(this, "ip" + dp.getAddress().toString() +
					// " len=" + dp.getLength());
					int recelen = dp.getLength();

					if (recelen < 0) {
						continue;
					}
					if (sock_checksum(buf, 0, 20) != 0) {
						dev.li.msg_DEBUGMSG(dev, "校验错误");
						continue;
					}
					get_stream get = new get_stream(buf, 0);
					int msg_identify = get.get_INT(); // 标识
					int msg_offset = get.get_INT(); // 偏移
					int msg_len = get.get_INT();// 当已经传送过来的命令数据长度，包括命名头的长度，不包括网络包头长度
					int msg_total = get.get_INT();// 总的命令数据长度，包括命令头长度和命令的参数长度
					// int msg_checksum = ;
					get.get_INT();// 检验和
					// dev.li.msg_DEBUGMSG(dev, "msg_identify=" + msg_identify +
					// " msg_offset=" + msg_offset + " msg_len=" + msg_len +
					// " msg_total=" + msg_total);
					if (pre_identify != msg_identify && 0 == msg_offset) {
						if (null == data || data.length < msg_total) {
							data = new byte[msg_total];
						}
						len = 0;
					}
					bytecopy(buf, 20, data, msg_offset, msg_len);
					len += msg_len;
					if (len >= msg_total) {
						get = new get_stream(data, 20);
						int m_Size = get.get_INT(); // 图像数据总大小BYTES
						int m_W = get.get_INT(); // 图像宽
						int m_H = get.get_INT(); // 图像高
						int m_Step = get.get_INT(); // 行数据步长
						int m_Type = get.get_BYTE(); // 数据类型 IMAGE_UYVY
						int off = 4 * 4 + 1;
						//dev.li.msg_DEBUGMSG(dev, "m_Size=" + m_Size + " m_W="
						//		+ m_W + " m_H=" + m_H + " m_Step=" + m_Step);

						int[] rgb = null;
						if (IMAGE_YUYV==m_Type) {
						rgb = YUYV_RGB_int(data, 20 + off, m_Step, m_W,
								m_H);
						} else if (IMAGE_UYVY==m_Type) {
						rgb = UYVY_RGB_int(data, 20 + off, m_Step, m_W,
								m_H);
						} else if (IMAGE_RGB24==m_Type) {
							rgb = RGB24_RGB_int(data, 20 + off, m_Step, m_W,
									m_H);
						} else {
							rgb = null;
						}
						// BufferedImage image = new BufferedImage(WIDTH,
						// HEIGHT, BufferedImage.TYPE_3BYTE_BGR);
						// Image aa;
						// Bitmap bitmap = Bitmap.createBitmap(data, off,
						// m_Step, m_W, m_H,
						// Bitmap.Config.ARGB_8888);
						dev.li.msg_VIDEO_OUT(dev, rgb, 0, m_W, m_H, m_W);
					}
				} catch (SocketTimeoutException e) {
					ds.close();
					// e.printStackTrace();
					dev.li.msg_DEBUGMSG(dev, "udp 超时 :" + devip + " 异常");
				} catch (Exception e) {
					ds.close();
					// e.printStackTrace();
					dev.li.msg_DEBUGMSG(dev,
							"udp receive DatagramPacket 出错, 主动断开");
					return;
				}
			}
			ds.close();
			ds = null;
			dev.li.msg_DEBUGMSG(dev, "线程退出!");
		}
	};

	int ntohs(int x) {
		return (short) (((x & 0xff) << 8) | ((x >> 8) & 0xff));
	}

	video_th vth;
	int m_port = 0;

	public int cmd_StartVideo(String myIP) {
		int ret = 0;
		if (null == vth) {
			vth = new video_th();
			vth.dev = this;
			m_port = vth.startUDP();
			byte[] nData = new byte[18];
			set_stream set = new set_stream(nData, 0);
			// int port1 = ntohs(port);
			set.set_SHORT((short) m_port);
			set.set_STRING(myIP, 16);
			li.msg_DEBUGMSG(this, "cmd_StartVideo ip = " + myIP + " port="
					+ m_port);
			ret = send_cmd_packet(CMD_M_VIDEO_OUT, CMD_S_SET, 0, nData,
					nData.length);
		} else {
			li.msg_DEBUGMSG(this, "视频已经打开，请先关闭!");
		}
		return ret;
	}

	// 停止视频
	public int cmd_StopVideo() {
		int retVal = 0;
		byte[] nData = new byte[18];
		set_stream set = new set_stream(nData, 0);
		// 关闭视频

		// 组命令数据
		set.set_SHORT((short) m_port);
		set.set_STRING(devip, 16);
		retVal = send_cmd_packet(M_VIDEO_OUT, S_GET, 0, nData, nData.length);
		if (vth != null) {
			vth.close();
			vth = null;
		} else {
			li.msg_DEBUGMSG(this, "视频已关闭!无需再关闭!");
		}
		return retVal;
	}

	public static int test1() {
		pmdev client = new pmdev();
		//155
		client.connect("192.168.1.46", "admin", "201031",
				new pmdev.listener() {
					public int println(String s) {
						System.out.println(s);
						return 0;
					}

					public int msg_DEBUGMSG(pmdev dev, String s) {
						println("DEBUG: " + s);
						return 0;
					}

					public int msg_CONNECT_SUCCESS(pmdev dev) {
						println("主机名:" + dev.get_ip() + "成功");
						return 0;
					}

					public int msg_CONNECT_CUT(pmdev dev) {
						println("主机名:" + dev.get_ip() + "断开链接");
						return 0;
					}

					public int msg_AUTH(pmdev dev, int m_CFlag) {
						println("msg_AUTH");
						if ((m_CFlag & F_AUTH_VERIFY) != 0) {
							println("用户身份验证 成功!");
						} else {
							println("设备验证用户设置操作 成功!");
						}
						// dev.cmd_GetAllUser();
						// dev.cmd_GetNetPro(); // 获取网络配置信息
						// dev.cmd_GetAllIdentifyRecord();
						dev.cmd_StopVideo();
						dev.cmd_StartVideo("192.168.1.3");
						try {
							sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						} // 延迟1秒
						//dev.cmd_StopVideo();
						//dev.cmd_StartVideo("192.168.1.3");
						dev.cmd_GetDevBaseInfo();
						//println("cmd_SetRealTimeUserCap!");
						//dev.cmd_SetRealTimeUserCap("0000");
						// dev.cmd_GetDevStatisInfo();
						return 0;
					}

					public int msg_NETPRO(pmdev dev, pmdev.NETPRO netpro) {
						println("msg_NETPRO");
						println("设备%s:获取网络信息成功");
						println("<< 设备基本信息 ");
						println("   m_WebPro1: " + netpro.m_WebPro1);
						println("   m_WebPro2: " + netpro.m_WebPro2);
						println("   m_WebUrl1: " + netpro.m_WebUrl1);
						println("   m_WebUrl2: " + netpro.m_WebUrl2);
						println("   m_IPAdr: " + netpro.m_IPAdr);
						println("   m_MIPAdr: " + netpro.m_MIPAdr);
						println("   m_NetMask: " + netpro.m_NetMask);
						println("   m_GateWay: " + netpro.m_GateWay);
						println("   m_Dns: " + netpro.m_Dns);
						println(">>");
						return 0;
					}

					public int msg_DEVINFO(pmdev dev, pmdev.BASEINFO baseinfo) {
						println("msg_DEVINFO");
						println("设备%s:获取基本信息成功");
						println("<< 设备基本信息 ");
						println("   设备基本信息 " + baseinfo.tostring());
						println(">>");
						return 0;
					}

					public int msg_STATIS(pmdev dev, pmdev.STATIS statis) {
						println("msg_STATIS");
						println("设备%s:获取统计信息成功: " + dev.get_ip());
						println("   设备统计信息 " + statis.tostring());
						return 0;
					}

					public int msg_RECORDRANGE(pmdev dev, int type,
							pmdev.RECORDRANGE[] range) {
						println("msg_RECORDRANGE");
						println("记录类型: " + type);
						println("记录区间数目: " + range.length);
						for (int i = 0; i < range.length; ++i) {
							println("m_Count: " + range[i].m_Count
									+ " m_Flag: " + range[i].m_Flag
									+ " m_SID: " + range[i].m_SID);
						}
						if (range.length > 0) {
							dev.cmd_GetRecEx(range[0]);
						}
						return 0;
					}

					public int msg_RECORD(pmdev dev, int type,
							pmdev.RECORD[] range) {
						println("msg_RECORD");
						for (int i = 0; i < range.length; ++i) {
							println(i + " ID: " + range[i].m_UserID + "时间"
									+ range[i].m_RecTime.tostring());
						}
						return 0;
					}

					public int msg_RECORDEX(pmdev dev, int type,
							pmdev.RECORD[] range) {
						println("msg_RECORDEX");
						for (int i = 0; i < range.length; ++i) {
							println(i + " ID: " + range[i].m_UserID + "时间"
									+ range[i].m_RecTime.tostring());
						}
						return 0;
					}

					public int msg_VUSER(pmdev dev, pmdev.VUSER[] vuser) {
						println("msg_VUSER");
						println("获取用户列表成功!");
						for (int i = 0; i < vuser.length; ++i) {
							// println("用户: " + vuser[i].m_UserID);
						}
						if (vuser.length > 2) {
							// 下载两个人的数据
							String[] strlist = new String[2];
							strlist[0] = vuser[0].m_UserID;
							strlist[1] = vuser[1].m_UserID;
							dev.cmd_GetUserEx(strlist, USER_TEXT | DL_FEAT
									| DL_IMAGE);
						}
						println("用户总数: " + vuser.length);
						return 0;
					}

					public int msg_USER(pmdev dev, pmdev.USER[] user) {
						println("msg_USER");
						println("用户总数: " + user.length);
						for (int i = 0; i < user.length; ++i) {
							println("用户: " + user[i].m_UserID + "特征长度 "
									+ user[i].m_Feat.length);
						}
						return 0;
					}
					
					public int msg_VIDEO_OUT(pmdev dev, int[] data,
							int yuvdataoff, int w, int h, int step) {
						println("视频: w:" + w + " h:" + h + " datalen:" + data.length);

						++aa;
						if (aa<10) {
							println("asdfadsf");
							FileOutputStream fos = null;
							try {
								fos = new FileOutputStream("C:\\aaa.dat");
							} catch (FileNotFoundException e) {
								e.printStackTrace();
							}
							// 用FileOutputStream 的write方法写入字节数组
							try {
								for (int i=0; i<data.length; ++i) {
									fos.write(intToByteArray(data[i]));
								}
							} catch (IOException e) {
								e.printStackTrace();
							}
							System.out.println("写入成功");
							// 为了节省IO流的开销，需要关闭
							try {
								fos.close();
							} catch (IOException e) {
								e.printStackTrace();
							}
						}
						return 0;
					}
				});
		try {
			client.join();
		} catch (Exception e) {
			e.printStackTrace();
		}
		client.close();
		System.out.println("退出");
		return 0;
	}

	static int aa=0;
	public static byte[] intToByteArray(final int integer) {
		int byteNum = (40 - Integer.numberOfLeadingZeros(integer < 0 ? ~integer
				: integer)) / 8;
		byte[] byteArray = null;
		if (integer < 0) {
			byteArray = new byte[] { -1, -1, -1, -1 };
		} else {
			byteArray = new byte[4];
		}

		for (int n = 0; n < byteNum; n++)
			byteArray[3 - n] = (byte) (integer >>> (n * 8));

		return (byteArray);
	}
	public static int test2() {
		String imgPath = "E://58.jpg";
		BufferedImage image;

		try {
			image = ImageIO.read(new FileInputStream(imgPath));
			System.out.println("getWidth " + image.getWidth());
			System.out.println("getHeight " + image.getHeight());
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.out.println("test2 退出");
		return 0;
	}

	public static void main(String[] args) throws Exception {
		test1();
		return;
	}
}
