
#include "cstd.h"

#if 0
#define LINE  printf("line %d\n", __LINE__);
#else
#define LINE 0
#endif
//#include "net/sock.inl"
//#include "nms_face.inl"

//#include "lib/curl/curl.inl"

#ifdef RYW_ANDROID
#define CFGDIR      "/storage/emulated/0/ryw/"
#define LOGFILE      CFGDIR"log.txt"
#define NETDEVICE   "wlan0"
#define PICPATH	     "/sdcard/DCIM/facepic/"
#else
#define CFGDIR      "./nms/"
#define LOGFILE      CFGDIR"log.txt"
#define NETDEVICE   "enp2s0"
#define PICPATH	     CFGDIR"DCIM/facepic/"
#endif

#define FILE_CFG	CFGDIR"/facecfg.json"
#define FILE_BFDB	CFGDIR"/bfdb.dat"
#define ZIPPATH	  CFGDIR
#define FILE_VER	20170701

#ifndef _RYCOMMON_H__
typedef enum __DEBUGLEVEL
{
  DEMERG = 0,      // 紧急
    DALERT,          // 警报
    DCRIT,
    DERR,            // 错误
    DWARNING,        // 警告
    DNOTICE,
    DINFO,           // 信息
    DDEBUG,          // 调试
}DEBUGLEVEL;
#endif // _RYCOMMON_H__

// LOGFILE

#ifdef RYW_ANDROID
#include <android/log.h>
CC_INLINE int andprintf(const char* format, ...) {
  int gsAndroidDebugType[] = 
  {
    ANDROID_LOG_SILENT,
      ANDROID_LOG_FATAL,
      ANDROID_LOG_VERBOSE,
      ANDROID_LOG_ERROR,  
      ANDROID_LOG_WARN,  
      ANDROID_LOG_DEFAULT,
      ANDROID_LOG_INFO,  
      ANDROID_LOG_DEBUG,  
      ANDROID_LOG_UNKNOWN
  };
  char buf[1024];
  //int debug = 0;
  LOGPRINTFTOBUF(buf, format, 1);
  __android_log_print(ANDROID_LOG_DEBUG, "FACEDETECT", "%s", buf);
  //puts(buf);
  return 0;
}
#else
CC_INLINE int andprintf(const char* format, ...) {
  char buf[1024] = {0};
  LOGPRINTFTOBUF(buf, format, 1);
  puts(buf);
  return 0;
}
#endif


#include "facelib/facedetect/test_facetrack.inl"
//#include "test_http.inl"
#include "facecfg.inl"
#include "rongyihttp.inl"
//#include "rongyitcp.inl"
#include "net/mac.inl"

#ifdef _FACETRACK_INL_
int test_buf_push() {
  int i;
  buf_t bf[1] = {0};
  for (i=0; i<100; ++i) {
    buf_push(bf, &i, sizeof(int));
  }
  for (i=0; i<100; ++i) {
    int* aa = (int*)bf->data;
    ASSERT(aa[i]==i);
  }
  bffree(bf);
  return 0;
}

//#include "img/test_color.inl"
#include "cap/capimgseq.inl"
#include "datetime.inl"

#ifdef _WIN32
#if _MSC_VER > 1300
#define USE_PROTOBUF
#endif
#else
#define USE_PROTOBUF
#endif
#ifdef USE_PROTOBUF
#include "protobuf/ServiceFaceRecognition.inl"
#endif // USE_PROTOBUF

#define USE_DB_SQLITE
#ifdef USE_DB_SQLITE
#include "db/CppSQLite3.cpp"
#include "db/FaceSqlite.cpp"
#endif // USE_DB_SQLITE


int test_rongyihttp_report_face_data() {
  const char* host;
  host = "http://dev.pack.hdp.rongyi.com";
  host = "http://node.rongyi.com";
  //"/api/v1/report/faceres";
  int ret=0;
  char url[256];
  char* mac_address = get_local_mac(NULL);
  _snprintf(url, 256, "%s/base_face/faceData/statistics", host);
  strcpy(url, "http://10.1.100.217:15031/headimg/upload");
  strcpy(url, "http://10.1.100.223:80");
  enum {nn = 2};
  faceobj_t fo[nn] = {0};
  int* p = (int*)fo;
  int i, j, n = sizeof(fo)/sizeof(int);
  for (i=0; i<n; ++i) {
    p[i] = i;
  }
  img_t im[1] = {0};
  imread("D:\\imgad.jpg", 3, 1, im);
  for (i=0; i<nn; ++i) {
    memset(fo[i].image, 0, sizeof(buf_t));
    fo[i].face.rect = cRectf(10, 10, 200, 200);
    save_face_bf(fo[i].image, im, fo[i].face.rect, fo[i].face.rect.w*0.5);
  }
  rongyihttp_report_face_data(url, mac_address, fo, nn, true);
  imfree(im);
  return 0;
}

int test_rongyinet() {CPCODETO("D:/code/end_code/rywfacedetect");
  //char* ss = strtimefmt(buf, 256, "%04y-%02m-%02M-%02s");
  //andprintf(0, "adfaf %s", "123");
  //test_rongyihttp_report_face_data(); return 0;
  cap_t cap[1] = {0};
  img_t im[10] = {0};
  //test_buf_push();
  faceobj_t fo[10];
  int nfo = 0, frame=0, ch=0;
  int i, imgid=0;
  bool drawshape = false, drawid = false;
  int ret;
  facecfg_t cfg[1] = {0};
  buf_t bfdb[1] = {0};
  int bfdb_len;
  buf_t bfs[1000] = {0};
  int nfile = 0;
  facecfg_init(cfg);
  andprintf("CFGDIR=%s\n", CFGDIR);
  mkdirs(CFGDIR);
  buf_load(FILE_BFDB, bfdb);
  bfdb_len = bfdb->len/sizeof(faceobj_t);
  bfdb->len = bfdb_len * sizeof(faceobj_t);
  facecfg_load(cfg, FILE_CFG);
  facecfg_save(cfg, FILE_CFG);
  facecfg_print(cfg);
#ifdef _RONGYITCP_INL_
  {
    const char* ip;
    int port = 8888;
    ip = "127.0.0.1";
    ip = "10.1.60.239";
    ip = "10.1.107.128";
    port = 60000;
    port = 8333;
    ret = rongyitcp_open(r, ip, port);
    if (!ret) {
      andprintf("rongyitcp_open fail ip=%s port=%d\n", ip, port);
      return 0;
    }
  }
#endif //  _RONGYITCP_INL_
  
  http_init();
  //rmdirs(PICPATH);
  {
    time_t timep;
    struct tm *p;
    time(&timep);
    p=localtime(&timep); /*取得当地时间*/
    int64 now_ticks = DateToTicks(1900+p->tm_year, 1+p->tm_mon, p->tm_mday);
    dirlist_t dl[1] = {0};
    dirlist(dl, PICPATH, "", FF_FULLNAME);
    for (i=0; i<dl->n; ++i) {
      if (dl->v[i].attrib & AS_SUBDIR) {
        const char* name = dl->v[i].name;
        const char* s = GetFileNameExt(name);
        int y = cstr_atoi(s, 0, 4);
        int m = cstr_atoi(s, 4, 6);
        int d = cstr_atoi(s, 6, 8);
        int64 ticks = DateToTicks(y, m, d);
        if ((now_ticks-ticks)>3*TicksPerDay) {
          rmdirs(name);
          andprintf("rmdirs(%s)", name);
        }
      }
    }
    dirlist_free(dl);
  }
  mkdirs(PICPATH);
  //cap_open( cap, 0, 640, 480 );
  //cap_open( cap, 0, 1092, 1080);
  //capvfwavi_open(cap, "D:/dt/dt.avi");
  //ret = cap_open(cap, 0, 1280, 960);
  
  const char* sdate = _strdate1();
  char buf[256];
  _snprintf(buf, 256, "%s/%s/", PICPATH, sdate);
  mkdirs(buf);
//#if defined _WIN32 && _DEBUG
#if 0
  ret = capimgseq_open(cap, "D:/dt/dt_1/list.txt", 0);
#else
  ret = cap_open(cap, 0, 1280, 720);
#endif
  //ret = cap_open(cap, 0, 640, 480);
  if (!cap->x) {
    andprintf("can not open cam!\n");
    sys_sleep(5*1000);
    return 0;
  }
  //FILELINE;
  for ( ; 'q' != ch && 'Q' != ch; ++frame ) {
    img_t* rgb = im+1;
    //FILELINE;
    if (!cap_getframe(cap, im, CAP_RGB)) {
      break;
    }
    //MEM_CHECK_BEGIN;
    if (0) {
      if ('p'==ch || (frame%100)==0) {
        if (0) {
          str_t s1[1] = {0};
          img2str(im, 80, s1);
          andprintf(s1->s);
          str_free(s1);
        }
        _snprintf(buf, 256, "%s/%s/%s_full.jpg", PICPATH, sdate, _strtime2());
        imwrite(buf, im);
      }
    }
    if (frame>128) {
      int asdf = 0;
    }
    //FILELINE;
    //andprintf("h=%d w=%d\n", im->h, im->w);
    {utime_start(_start_time);
    nfo = facetrack(im, fo, nfo, countof(fo), &cfg->face_code);
#ifdef _NMS_FACE_INL_
    nms_face_findhuman_run(nfo);
#endif //  _RONGYITCP_INL_
    printf("time = %f nfo=%d\n", utime_elapsed(_start_time), nfo);}
#ifdef _RONGYITCP_INL_
    select_run(r->s, 10);
#endif // _RONGYITCP_INL_
#ifdef USE_FACEPLUSPLUS
    for (i=0; i<nfo; ++i) {
      if (fo[i].inited==0) {
        if (0) {
          faceplusplus_image(im, fo+i);
        }
      }
    }
#endif // USE_FACEPLUSPLUS
    
    if (1) {
      for (i=0; i<nfo; ++i) {
        if (fo[i].event==1) {
          if (1) {
            _snprintf(buf, 256, "D:/dt/out/%05d_%05d_%d_%02d.jpg", frame, fo[i].id, fo[i].gender, fo[i].age);
            save_face(buf, im, fo[i].face.rect, fo[i].face.rect.w);
          }
        }
      }
    }
    const char* host;
    host = "http://dev.pack.hdp.rongyi.com";
    host = "http://node.rongyi.com";
    //"/api/v1/report/faceres";
    host = cfg->host;
    int ret=0;
    char url[256];
    char* mac_address = get_local_mac(NULL);
    _snprintf(url, 256, "%s/base_face/faceData/statistics", host);

    if (1) {
      int cnt_new=0;
      for (i=0; i<nfo; ++i) {
        if (fo[i].event==1) {
          cnt_new++;
          andprintf("%d in[%d,%d,%d,%d]\n", fo[i].id, (int)fo[i].face.rect.x, (int)fo[i].face.rect.y, (int)fo[i].face.rect.w, (int)fo[i].face.rect.h);
          fo[i].enter_time = time(NULL);
#ifdef _RONGYITCP_INL_
          rongyitcp_facedetectinfo(r, fo[i].id);
#endif // _RONGYITCP_INL_
          nfile = nfile%countof(bfs);
          andprintf("in %d\n", fo[i].id);
          if (1) {
            rongyihttp_report_face_data(url, mac_address, fo+i, 1, true);
          }
          save_face_bf(fo[i].image, im, fo[i].face.rect, fo[i].face.rect.w*0.5);
          if (1) {
            save_face_bf(bfs+nfile, im, fo[i].face.rect, fo[i].face.rect.w*0.5);
            _snprintf(buf, 256, "%s/%s/%s.jpg", PICPATH, sdate, _strtime2());
            //_snprintf(buf, 256, "%s/%s.jpg", PICPATH, stime);
            buf_save(buf, bfs+nfile);
            andprintf("save pic %s", buf);
#ifdef USE_DB_SQLITE
            DbSQLite_AddFaceData(fo+i, buf);
#endif
            //PICPATH
          }
          ++nfile;
        } else if (fo[i].event==-1) {
          fo[i].left_time = time(NULL);
          andprintf("out %d\n", fo[i].id);
          //rongyihttp_report_face_data("http://dev.state.hdp.rongyi.com/api/v1/report/faceres", );
          buf_push(bfdb, fo+i, sizeof(faceobj_t));
        }
      }
      if (cnt_new>0) {
        facecfg_save(cfg, FILE_CFG);
      }
    }
    bfdb_len = bfdb->len/sizeof(faceobj_t);
    printf("bfdb_len=%d\n", bfdb_len);
    if (1) {
#ifdef USE_PROTOBUF
      if (bfdb_len>0) {
        buf_t bf[1] = {0};
        const char* url = "http://192.168.10.194:28082/screen_detector_log?";
        str_t strResponse[1] = { 0 };
        HttpParam cValueParam[1] = {"Content-Type", "application/octet-stream"  };
        char* mac_address = get_local_mac(NULL);
        int len = ServiceFaceRecognition(mac_address, "", bfdb_len, (faceobj_t*)(bfdb->data), bf);
        ret = http_post_mem(url, cValueParam, 1, bf->data, bf->len, strResponse);
        printf("http_post_mem ret = %d\n", ret);
        if (ret) {
          bfdb->len = 0;
        }
        bffree(bf);
      }
#endif // _SERVICEFACERECOGNITION_INL_
#if 0
      if (bfdb_len>0) {
        ret = rongyihttp_report_face_data(url, mac_address, (faceobj_t*)(bfdb->data), bfdb_len);
        if (ret) {
          bfdb->len = 0;
        }
        andprintf("rongyihttp_report_face_data %s! mac=%s  url=%s\n", ret ? "ok" : "fail", mac_address, url);
      }
      if (0 && (frame-1)%1000==0) {
        const char* host;
        host = "http://dev.pack.hdp.rongyi.com";
        host = "http://dev.state.hdp.rongyi.com";
        host = cfg->pichost;
        if (nfile>0) {
          imgid = rongyihttp_upload_mem_files(host, cfg->mall_id, cfg->terminal_id, cfg->machine_no, imgid, bfs, nfile, ZIPPATH, ZIPPATH);
          nfile = 0;
        }
      }
#endif
    }
    nfo = faceobj_clear(fo, nfo);
    //MEM_CHECK_END;
#ifdef USE_GUI
    for (i = 0; i < nfo; ++i) {
      draw_faceobj(im, NULL, fo + i, drawshape, true);
    }
    imshow_(im);
    ch = WaitKey(10);
#else
    sys_sleep(1);
#endif // USE_GUI
    //printf("frame %d w=%d h=%d\n", frame, im->w, im->h);
    }
    cap_close( cap );
    imfrees(im, 10);
    return 0;
}

#include "lib.inl"
#endif // _FACETRACK_INL_

