
#ifndef _LBF_FACE_ALIGNMENT_INL_
#define _LBF_FACE_ALIGNMENT_INL_

typedef struct lbf_model_t {
  int npt; //点数68
  int max_stage;
  int NumOfTree;
  int max_depth;
} lbf_model_t;

int ipow(int a, int b) {
  int x=1;
  for (;b-->0;) {
    x*=a;
  }
  return x;
}

lbf_model_t* lbf_model_load(const void* bf, double** pmeanshape, double** pforest, double** pWt) {
  char* data = (char*)bf;
  lbf_model_t* mo=(lbf_model_t*)data;
  if (pmeanshape) {
    *pmeanshape = (double*)(data+sizeof(lbf_model_t));
  }
  if (pforest) {
    *pforest = (double*)(data+sizeof(lbf_model_t) + sizeof(double)*mo->npt*2);
  }
  if (pWt) {
    int nnode = ipow(2, mo->max_depth-1)-1;
    int n = mo->max_stage*mo->npt*mo->NumOfTree*nnode*5;
    *pWt = (double*)(data+sizeof(lbf_model_t) + sizeof(double)*mo->npt*2 + sizeof(double)*n);
  }
  return mo;
}

lbf_model_t* lbf_model_load_fix16(const void* bf, int** pmeanshape, int** pforest, int** pWt) {
  char* data = (char*)bf;
  lbf_model_t* mo=(lbf_model_t*)data;
  if (pmeanshape) {
    *pmeanshape = (int*)(data+sizeof(lbf_model_t));
  }
  if (pforest) {
    *pforest = (int*)(data+sizeof(lbf_model_t) + sizeof(int)*mo->npt*2);
  }
  if (pWt) {
    int nnode = ipow(2, mo->max_depth-1)-1;
    int n = mo->max_stage*mo->npt*mo->NumOfTree*nnode*5;
    *pWt = (int*)(data+sizeof(lbf_model_t) + sizeof(int)*mo->npt*2 + sizeof(int)*n);
  }
  return mo;
}

lbf_model_t* lbf_model_load_fix16_15(const void* bf, int** pmeanshape, int** pforest, short** pWt) {
  char* data = (char*)bf;
  lbf_model_t* mo=(lbf_model_t*)data;
  if (pmeanshape) {
    *pmeanshape = (int*)(data+sizeof(lbf_model_t));
  }
  if (pforest) {
    *pforest = (int*)(data+sizeof(lbf_model_t) + sizeof(int)*mo->npt*2);
  }
  if (pWt) {
    int nnode = ipow(2, mo->max_depth-1)-1;
    int n = mo->max_stage*mo->npt*mo->NumOfTree*nnode*5;
    *pWt = (short*)(data+sizeof(lbf_model_t) + sizeof(int)*mo->npt*2 + sizeof(int)*n);
  }
  return mo;
}

double norm2(double x, double y) {
  return sqrt(x*x+y*y);
}

double lbf_compute_error(int num_points, const DPOINT* ground_truth_points, const DPOINT* detected_points) {
  int i, k=6;
  double interocular_distance = 0;
  double eye_dx=0, eye_dy=0, sum=0, error_of_image;
  for (i=0; i<k; ++i) {
    eye_dx+=ground_truth_points[36+i].x-ground_truth_points[42+i].x;
    eye_dy+=ground_truth_points[36+i].y-ground_truth_points[42+i].y;
  }
  eye_dx/=k, eye_dy/=k;
  interocular_distance = norm2(eye_dx, eye_dy);
  for (i=0; i<num_points; ++i) {
    double x = detected_points[i].x - ground_truth_points[i].x;
    double y = detected_points[i].y - ground_truth_points[i].y;
    sum+= norm2(x, y);
  }
  error_of_image = sum/(num_points*interocular_distance);//单个图片的误差
  return error_of_image;
}

int lbf_face_alignment_64f(const void* lbfmodel, const img_t* im, const img_t* imgTe_gray, const double* box, double* outpt, int istrans) {
  double* meanshape=0;
  double* forestmodel=0;
  double* Wt=0;
  lbf_model_t* mo = lbf_model_load(lbfmodel, &meanshape, &forestmodel, &Wt);
  double shape[136] = {0};
  int i, j, i_stage, i_point, i_tree;
  const double Radius[] = {0.4, 0.3, 0.2, 0.15, 0.1};
  int nleaf = ipow(2, mo->max_depth-1);
  int nnode = nleaf-1;
  double bx = box[0], by = box[2];
  double bw = box[1] - box[0], sw;
  double bh = box[3] - box[2], sh;
  double max_x, max_y, min_x, min_y;
  int* Pt_index=0;
  int kPt, index_len = mo->NumOfTree*mo->npt*nleaf;
  Pt_index = MALLOC(int, index_len);
  //根据人脸检测的位置初始化人脸形状
  {
    max_x = meanshape[0], max_y = meanshape[mo->npt];
    min_x = meanshape[0], min_y = meanshape[mo->npt];
    for (i=1; i<mo->npt; ++i) {
      double x = meanshape[mo->npt*0 + i];
      double y = meanshape[mo->npt*1 + i];
      max_x = MAX(max_x, x), max_y = MAX(max_y, y);
      min_x = MIN(min_x, x), min_y = MIN(min_y, y);
    }
    sw = max_x-min_x;
    sh = max_y-min_y;
    for (i=0; i<mo->npt; ++i) {
      double x = meanshape[mo->npt*0 + i];
      double y = meanshape[mo->npt*1 + i];
      if (istrans) {
        shape[mo->npt*1 + i] = (x-min_x)*bh/sw + by;
        shape[mo->npt*0 + i] = (y-min_y)*bw/sh + bx;
      } else {
        shape[mo->npt*0 + i] = (x-min_x)*bw/sw + bx;
        shape[mo->npt*1 + i] = (y-min_y)*bh/sh + by;
      }
    }
  }
  //lbf_show_shape(im, shape, rgb(1,1,1), -1);
  for (i_stage = 0; i_stage<mo->max_stage; ++i_stage) {
    double* shape_curstage = shape;
    double bbox3, bbox4;
    kPt=0;

    max_x = shape_curstage[0], max_y = shape_curstage[mo->npt];
    min_x = shape_curstage[0], min_y = shape_curstage[mo->npt];
    
    for (i=1; i<mo->npt; ++i) {
      double x = shape_curstage[mo->npt*0 + i];
      double y = shape_curstage[mo->npt*1 + i];
      max_x = MAX(max_x, x), max_y = MAX(max_y, y);
      min_x = MIN(min_x, x), min_y = MIN(min_y, y);
    }
    sw = max_x-min_x;
    sh = max_y-min_y;
    bbox3=sw*Radius[i_stage];
    bbox4=sh*Radius[i_stage];
    
    //{utime_start(_start_time);
    for (i_point = 0; i_point<mo->npt; ++i_point) {
      double* forest=forestmodel + (i_stage*mo->npt+ + i_point)*mo->NumOfTree*nnode*5;//当前点森林模型
      double x0=shape_curstage[i_point], y0=shape_curstage[i_point+mo->npt];//当前点坐标
      //[pt pt_new]=getlbf(forest,radius,NumOfTree,imgTe_gray,x0,y0,bbox3,bbox4,max_depth);%LBF
      for (i_tree = 0; i_tree<mo->NumOfTree; ++i_tree) {
        double* tree=forest+i_tree*nnode*5;
        double* thresh=tree + nnode*4;
        //遍历树
        int j=0, pdfeat, id_cnode=0;
        while(id_cnode<nnode) {
          double* feat=tree + id_cnode*4;
          int ax, ay, bx, by;
          if (istrans) {
            ay=ROUND(feat[0]*bbox3+y0)-1;//像素点a的x坐标值
            by=ROUND(feat[1]*bbox3+y0)-1;//像素点b的x坐标值
            ax=ROUND(feat[2]*bbox4+x0)-1;//像素点a的y坐标值
            bx=ROUND(feat[3]*bbox4+x0)-1;//像素点b的y坐标值
          } else {
            ax=ROUND(feat[0]*bbox3+x0)-1;//像素点a的x坐标值
            bx=ROUND(feat[1]*bbox3+x0)-1;//像素点b的x坐标值
            ay=ROUND(feat[2]*bbox4+y0)-1;//像素点a的y坐标值
            by=ROUND(feat[3]*bbox4+y0)-1;//像素点b的y坐标值
          }
          ax = BOUND(ax, 0, imgTe_gray->w-1);
          ay = BOUND(ay, 0, imgTe_gray->h-1);
          bx = BOUND(bx, 0, imgTe_gray->w-1);
          by = BOUND(by, 0, imgTe_gray->h-1);
          pdfeat = imgTe_gray->tt.data[ay*imgTe_gray->s+ax]-imgTe_gray->tt.data[by*imgTe_gray->s+bx];
          id_cnode = id_cnode*2 + 1 + (pdfeat>thresh[id_cnode]);
          j = id_cnode;
        }
        Pt_index[kPt++] = j-nnode;
      }
    }
    //printf("- %3d, %8.5f\n", i_stage, utime_elapsed(_start_time));}
    if (istrans) {
      for (j=0; j<mo->npt*2; ++j) {
        double d=0;
        double* iWt = Wt + (i_stage * mo->npt*2 + j)*index_len;
        for (i=0; i<kPt; ++i, iWt+=nleaf) {
          d += iWt[Pt_index[i]];
        }
        if (j<mo->npt) {
          shape_curstage[mo->npt+j]+=d*sw;
        } else {
          shape_curstage[j-mo->npt]+=d*sh;
        }
      }
    } else {
      for (j=0; j<mo->npt*2; ++j) {
        double d=0;
        double* iWt = Wt + (i_stage * mo->npt*2 + j)*index_len;
        double bb[2];
        bb[0] = sh, bb[1] = sw;
        for (i=0; i<kPt; ++i, iWt+=nleaf) {
          d += iWt[Pt_index[i]];
        }
        shape_curstage[j]+=d*bb[j<mo->npt];
      }
    }
    //lbf_show_shape(im, shape_curstage, rgb(1,1,1), -1);
  }
  for (j=0; j<mo->npt*2; ++j) {
    outpt[j] = shape[j];
  }
  FREE(Pt_index);
  return 0;
}

int fix16_to_double(int n, const int* a, double* b) {
  int i;
  for (i=0; i<n; ++i) {
    b[i] = a[i]*(1./(1<<16));
  }
  return 0;
}

int lbf_face_alignment_fix16(const void* lbfmodel, const img_t* im, const img_t* imgTe_gray, const double* box, double* outpt, int istrans) {
  int* meanshape=0;
  int* forestmodel=0;
  short* Wt=0;
  lbf_model_t* mo = lbf_model_load_fix16_15(lbfmodel, &meanshape, &forestmodel, &Wt);
  int shape[136] = {0};
  int i, j, i_stage, i_point, i_tree;
  const double Radius[] = {0.4, 0.3, 0.2, 0.15, 0.1};
  int nleaf = ipow(2, mo->max_depth-1);
  int nnode = nleaf-1;
  int bx = (int)(box[0]*(1<<16)), by = (int)(box[2]*(1<<16));
  int bw = (int)(box[1] - box[0]), sw;
  int bh = (int)(box[3] - box[2]), sh;
  int max_x, max_y, min_x, min_y;
  int Pt_index[512]={0};
  int kPt, index_len = mo->NumOfTree*mo->npt*nleaf;
  //根据人脸检测的位置初始化人脸形状
  {
    max_x = meanshape[0], max_y = meanshape[mo->npt];
    min_x = meanshape[0], min_y = meanshape[mo->npt];
    for (i=1; i<mo->npt; ++i) {
      int x = meanshape[mo->npt*0 + i];
      int y = meanshape[mo->npt*1 + i];
      max_x = MAX(max_x, x), max_y = MAX(max_y, y);
      min_x = MIN(min_x, x), min_y = MIN(min_y, y);
    }
    sw = (max_x-min_x)>>16;
    sh = (max_y-min_y)>>16;
    for (i=0; i<mo->npt; ++i) {
      int x = meanshape[mo->npt*0 + i];
      int y = meanshape[mo->npt*1 + i];
      if (istrans) {
        shape[mo->npt*1 + i] = (int)((double)(x-min_x)*bh/sw + by);
        shape[mo->npt*0 + i] = (int)((double)(y-min_y)*bw/sh + bx);
      } else {
        shape[mo->npt*0 + i] = (int)((double)(x-min_x)*bw/sw + bx);
        shape[mo->npt*1 + i] = (int)((double)(y-min_y)*bh/sh + by);
      }
    }
  }
  //fix16_to_double(136, shape, outpt);lbf_show_shape(im, outpt, rgb(1,1,1), -1);
  for (i_stage = 0; i_stage<mo->max_stage; ++i_stage) {
    int* shape_curstage = shape;
    int bbox3, bbox4;
    kPt=0;

    max_x = shape_curstage[0], max_y = shape_curstage[mo->npt];
    min_x = shape_curstage[0], min_y = shape_curstage[mo->npt];
    
    for (i=1; i<mo->npt; ++i) {
      int x = shape_curstage[mo->npt*0 + i];
      int y = shape_curstage[mo->npt*1 + i];
      max_x = MAX(max_x, x), max_y = MAX(max_y, y);
      min_x = MIN(min_x, x), min_y = MIN(min_y, y);
    }
    sw = (max_x-min_x)>>16;
    sh = (max_y-min_y)>>16;
    bbox3=(int)(sw*Radius[i_stage]);
    bbox4=(int)(sh*Radius[i_stage]);
    
    //{utime_start(_start_time);
    for (i_point = 0; i_point<mo->npt; ++i_point) {
      int* forest=forestmodel + (i_stage*mo->npt+ + i_point)*mo->NumOfTree*nnode*5;//当前点森林模型
      int x0=shape_curstage[i_point], y0=shape_curstage[i_point+mo->npt];//当前点坐标
      //[pt pt_new]=getlbf(forest,radius,NumOfTree,imgTe_gray,x0,y0,bbox3,bbox4,max_depth);%LBF

      for (i_tree = 0; i_tree<mo->NumOfTree; ++i_tree) {
        int* tree=forest+i_tree*nnode*5;
        int* thresh=tree + nnode*4;
        //遍历树
        int j=0, pdfeat, id_cnode=0;

        while(id_cnode<nnode) {
          int* feat=tree + id_cnode*4;
          int ax, ay, bx, by;
          if (istrans) {
            ay=(feat[0]*bbox3+y0)>>16;//像素点a的x坐标值
            by=(feat[1]*bbox3+y0)>>16;//像素点b的x坐标值
            ax=(feat[2]*bbox4+x0)>>16;//像素点a的y坐标值
            bx=(feat[3]*bbox4+x0)>>16;//像素点b的y坐标值
          } else {
            ax=(feat[0]*bbox3+x0)>>16;//像素点a的x坐标值
            bx=(feat[1]*bbox3+x0)>>16;//像素点b的x坐标值
            ay=(feat[2]*bbox4+y0)>>16;//像素点a的y坐标值
            by=(feat[3]*bbox4+y0)>>16;//像素点b的y坐标值
          }

          ax = BOUND(ax, 0, imgTe_gray->w-1);
          ay = BOUND(ay, 0, imgTe_gray->h-1);
          bx = BOUND(bx, 0, imgTe_gray->w-1);
          by = BOUND(by, 0, imgTe_gray->h-1);
          pdfeat = imgTe_gray->tt.data[ay*imgTe_gray->s+ax]-imgTe_gray->tt.data[by*imgTe_gray->s+bx];
          id_cnode = id_cnode*2 + 1 + (pdfeat>thresh[id_cnode]);
          j = id_cnode;
        }
        Pt_index[kPt++] = j-nnode;
      }
    }
    //printf("- %3d, %8.5f\n", i_stage, utime_elapsed(_start_time));}
    //Wt 是 FIX24 格式的
    ASSERT(kPt<countof(Pt_index));
    if (istrans) {
#if 1
      for (i=0; i<kPt; ++i) {
        Pt_index[i] += i*nleaf;
      }
#if 0
      for (j=0; j<mo->npt; ++j) {
        int d0=0, d1=0;
        short* iWt0 = Wt + (i_stage * mo->npt*2 + j)*index_len;
        short* iWt1 = Wt + (i_stage * mo->npt*2 + j + mo->npt)*index_len;
        for (i=0; i<kPt; i+=4) {
          int id0 = Pt_index[i];
          int id1 = Pt_index[i+1];
          int id2 = Pt_index[i+2];
          int id3 = Pt_index[i+3];
          d0 += iWt0[id0]+iWt0[id1]+iWt0[id2]+iWt0[id3];
          d1 += iWt1[id0]+iWt1[id1]+iWt1[id2]+iWt1[id3];
        }
        shape_curstage[mo->npt+j]+=d0*sw;
        shape_curstage[j]+=d1*sh;
      }
#else
      for (j=0; j<mo->npt; ++j) {
        int d0=0, d1=0;
        short* iWt0 = Wt + (i_stage * mo->npt*2 + j)*index_len;
        short* iWt1 = Wt + (i_stage * mo->npt*2 + j + mo->npt)*index_len;
        for (i=0; i<kPt; ++i) {
          int id0 = Pt_index[i];
          d0 += iWt0[id0];
          d1 += iWt1[id0];
        }
        shape_curstage[mo->npt+j]+=d0*sw;
        shape_curstage[j]+=d1*sh;
      }
#endif
#else
      for (j=0; j<mo->npt*2; ++j) {
        int d=0;
        short* iWt = Wt + (i_stage * mo->npt*2 + j)*index_len;
        for (i=0; i<kPt; ++i, iWt+=nleaf) {
          d += iWt[Pt_index[i]];
        }
        if (j<mo->npt) {
          shape_curstage[mo->npt+j]+=d*sw;
        } else {
          shape_curstage[j-mo->npt]+=d*sh;
        }
      }
#endif
    } else {
      for (j=0; j<mo->npt*2; ++j) {
        int d=0;
        short* iWt = Wt + (i_stage * mo->npt*2 + j)*index_len;
        int bb[2];
        bb[0] = sh, bb[1] = sw;
        for (i=0; i<kPt; ++i, iWt+=nleaf) {
          d += iWt[Pt_index[i]];
        }
        d<<=1;
        shape_curstage[j]+=d*bb[j<mo->npt];
      }
    }
    //fix16_to_double(136, shape_curstage, outpt);    lbf_show_shape(im, outpt, rgb(1,1,1), -1);
  }
  fix16_to_double(136, shape, outpt);
  return 0;
}

//buf_t g_lbfmodel[1] = {0};

int detect_eyes1(const void* model, int h, int w, const unsigned char* data, int datastep, const XRECT* face, XRECT* deye, XRECT* leye, XRECT* reye, int is_trans) {
  img_t imgTe_gray[1] _INIT0;
  double box[4] = {0};
  enum {npt=68};
  double pt[2*npt] = {0};
  int i, k = 6;
  double eye[4] = {0};
  int cx, cy;
  IMINIT(imgTe_gray, h, w, data, datastep, 1, 1);
#if 0
  if (NULL==g_lbfmodel->data) {
    buf_load("D:/pub/bin/lbf_face_alignment/train_model_new.dat", g_lbfmodel);
  }
#endif
  box[0] = face->x;
  box[1] = face->x + face->w;
  box[2] = face->y;
  box[3] = face->y + face->h;
  lbf_face_alignment_fix16(model, 0, imgTe_gray, box, pt, is_trans);
  for (i=0; i<k; ++i) {
    eye[0] += pt[npt*0 + 36+i];
    eye[1] += pt[npt*1 + 36+i];
    eye[2] += pt[npt*0 + 42+i];
    eye[3] += pt[npt*1 + 42+i];
  }
  memset(deye, 0, sizeof(XRECT));
  cx = is_trans ? face->w/8 : face->w/4;
  cy = is_trans ? face->w/4 : face->w/8;
  reye->h = leye->h = cx;
  reye->w = leye->w = cy;
  leye->x = (int)(eye[0]/k)-leye->w/2;
  leye->y = (int)(eye[1]/k)-leye->h/2;
  reye->x = (int)(eye[2]/k)-reye->w/2;
  reye->y = (int)(eye[3]/k)-reye->h/2;
  return 0;
}

#endif // _LBF_FACE_ALIGNMENT_INL_
