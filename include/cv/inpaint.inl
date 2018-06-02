

/* ////////////////////////////////////////////////////////////////////
//
//  Geometrical transforms on images and matrices: rotation, zoom etc.
//
// */

#undef CC_MAT_ELEM_PTR_FAST
#define CC_MAT_ELEM_PTR_FAST(mat, row, col, pix_size)  \
     ((mat)->tt.data + (size_t)(mat)->step*(row) + (pix_size)*(col))

inline float
min4(float a, float b, float c, float d)
{
    a = MIN(a,b);
    c = MIN(c,d);
    return MIN(a,c);
}

#define CC_MAT_3COLOR_ELEM(img,type,y,x,c) CC_MAT_ELEM(img,type,y,(x)*3+(c))
#define KNOWN  0  //known outside narrow band
#define BAND   1  //narrow band (known)
#define INSIDE 2  //unknown
#define CHANGE 3  //servise

typedef struct CvHeapElem
{
    float T;
    int i,j;
    struct CvHeapElem* prev;
    struct CvHeapElem* next;
}
CvHeapElem;


class CvPriorityQueueFloat
{
protected:
    CvHeapElem *mem,*empty,*head,*tail;
    int num,in;

public:
    bool Init(const img_t* f)
    {
        int i,j;
        for(i = num = 0; i < f->rows; i++)
        {
            for(j = 0; j < f->cols; j++)
                num += CC_MAT_ELEM(*f,uchar,i,j)!=0;
        }
        if (num<=0) return false;
        mem = (CvHeapElem*)cAlloc((num+2)*sizeof(CvHeapElem));
        if (mem==NULL) return false;

        head       = mem;
        head->i    = head->j = -1;
        head->prev = NULL;
        head->next = mem+1;
        head->T    = -FLT_MAX;
        empty      = mem+1;
        for (i=1; i<=num; i++) {
            mem[i].prev   = mem+i-1;
            mem[i].next   = mem+i+1;
            mem[i].i      = mem[i].i = -1;
            mem[i].T      = FLT_MAX;
        }
        tail       = mem+i;
        tail->i    = tail->j = -1;
        tail->prev = mem+i-1;
        tail->next = NULL;
        tail->T    = FLT_MAX;
        return true;
    }

    bool Add(const img_t* f) {
        int i,j;
        for (i=0; i<f->rows; i++) {
            for (j=0; j<f->cols; j++) {
                if (CC_MAT_ELEM(*f,uchar,i,j)!=0) {
                    if (!Push(i,j,0)) return false;
                }
            }
        }
        return true;
    }

    bool Push(int i, int j, float T) {
        CvHeapElem *tmp=empty,*add=empty;
        if (empty==tail) return false;
        while (tmp->prev->T>T) tmp = tmp->prev;
        if (tmp!=empty) {
            add->prev->next = add->next;
            add->next->prev = add->prev;
            empty = add->next;
            add->prev = tmp->prev;
            add->next = tmp;
            add->prev->next = add;
            add->next->prev = add;
        } else {
            empty = empty->next;
        }
        add->i = i;
        add->j = j;
        add->T = T;
        in++;
        //      printf("push i %3d  j %3d  T %12.4e  in %4d\n",i,j,T,in);
        return true;
    }

    bool Pop(int *i, int *j) {
        CvHeapElem *tmp=head->next;
        if (empty==tmp) return false;
        *i = tmp->i;
        *j = tmp->j;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        tmp->prev = empty->prev;
        tmp->next = empty;
        tmp->prev->next = tmp;
        tmp->next->prev = tmp;
        empty = tmp;
        in--;
        //      printf("pop  i %3d  j %3d  T %12.4e  in %4d\n",tmp->i,tmp->j,tmp->T,in);
        return true;
    }

    bool Pop(int *i, int *j, float *T) {
        CvHeapElem *tmp=head->next;
        if (empty==tmp) return false;
        *i = tmp->i;
        *j = tmp->j;
        *T = tmp->T;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        tmp->prev = empty->prev;
        tmp->next = empty;
        tmp->prev->next = tmp;
        tmp->next->prev = tmp;
        empty = tmp;
        in--;
        //      printf("pop  i %3d  j %3d  T %12.4e  in %4d\n",tmp->i,tmp->j,tmp->T,in);
        return true;
    }

    CvPriorityQueueFloat(void) {
        num=in=0;
        mem=empty=head=tail=NULL;
    }

    ~CvPriorityQueueFloat(void)
    {
        cFree(&mem);
    }
};

inline float VectorScalMult(CPoint2D32f v1,CPoint2D32f v2) {
   return v1.x*v2.x+v1.y*v2.y;
}

inline float VectorLength(CPoint2D32f v1) {
   return v1.x*v1.x+v1.y*v1.y;
}

///////////////////////////////////////////////////////////////////////////////////////////
//HEAP::iterator Heap_Iterator;
//HEAP Heap;

float FastMarching_solve(int i1,int j1,int i2,int j2, const img_t* f, const img_t* t)
{
    double sol, a11, a22, m12;
    a11=CC_MAT_ELEM(*t,float,i1,j1);
    a22=CC_MAT_ELEM(*t,float,i2,j2);
    m12=MIN(a11,a22);
    
    if(CC_MAT_ELEM(*f,uchar,i1,j1) != INSIDE)
        if(CC_MAT_ELEM(*f,uchar,i2,j2) != INSIDE)
            if(fabs(a11-a22) >= 1.0)
                sol = 1+m12;
            else
                sol = (a11+a22+sqrt((double)(2-(a11-a22)*(a11-a22))))*0.5;
        else
            sol = 1+a11;
    else if(CC_MAT_ELEM(*f,uchar,i2,j2) != INSIDE)
        sol = 1+a22;
    else
        sol = 1+m12;
            
    return (float)sol;
}

/////////////////////////////////////////////////////////////////////////////////////


static void
icvCalcFMM(const img_t *f, img_t *t, CvPriorityQueueFloat *Heap, bool negate) {
   int i, j, ii = 0, jj = 0, q;
   float dist;

   while (Heap->Pop(&ii,&jj)) {

      unsigned known=(negate)?CHANGE:KNOWN;
      CC_MAT_ELEM(*f,uchar,ii,jj) = (uchar)known;

      for (q=0; q<4; q++) {
         i=0; j=0;
         if     (q==0) {i=ii-1; j=jj;}
         else if(q==1) {i=ii;   j=jj-1;}
         else if(q==2) {i=ii+1; j=jj;}
         else {i=ii;   j=jj+1;}
         if ((i<=0)||(j<=0)||(i>f->rows)||(j>f->cols)) continue;

         if (CC_MAT_ELEM(*f,uchar,i,j)==INSIDE) {
            dist = min4(FastMarching_solve(i-1,j,i,j-1,f,t),
                        FastMarching_solve(i+1,j,i,j-1,f,t),
                        FastMarching_solve(i-1,j,i,j+1,f,t),
                        FastMarching_solve(i+1,j,i,j+1,f,t));
            CC_MAT_ELEM(*t,float,i,j) = dist;
            CC_MAT_ELEM(*f,uchar,i,j) = BAND;
            Heap->Push(i,j,dist);
         }
      }
   }

   if (negate) {
      for (i=0; i<f->rows; i++) {
         for(j=0; j<f->cols; j++) {
            if (CC_MAT_ELEM(*f,uchar,i,j) == CHANGE) {
               CC_MAT_ELEM(*f,uchar,i,j) = KNOWN;
               CC_MAT_ELEM(*t,float,i,j) = -CC_MAT_ELEM(*t,float,i,j);
            }
         }
      }
   }
}


static void
icvTeleaInpaintFMM(const img_t *f, img_t *t, img_t *out, int range, CvPriorityQueueFloat *Heap) {
   int i = 0, j = 0, ii = 0, jj = 0, k, l, q, color = 0;
   float dist;

   if (CC_MAT_CN(out->tid)==3) {

      while (Heap->Pop(&ii,&jj)) {

         CC_MAT_ELEM(*f,uchar,ii,jj) = KNOWN;
         for(q=0; q<4; q++) {
            if     (q==0) {i=ii-1; j=jj;}
            else if(q==1) {i=ii;   j=jj-1;}
            else if(q==2) {i=ii+1; j=jj;}
            else if(q==3) {i=ii;   j=jj+1;}
            if ((i<=1)||(j<=1)||(i>t->rows-1)||(j>t->cols-1)) continue;

            if (CC_MAT_ELEM(*f,uchar,i,j)==INSIDE) {
               dist = min4(FastMarching_solve(i-1,j,i,j-1,f,t),
                           FastMarching_solve(i+1,j,i,j-1,f,t),
                           FastMarching_solve(i-1,j,i,j+1,f,t),
                           FastMarching_solve(i+1,j,i,j+1,f,t));
               CC_MAT_ELEM(*t,float,i,j) = dist;

               for (color=0; color<=2; color++) {
                  CPoint2D32f gradI,gradT,r;
                  float Ia=0,Jx=0,Jy=0,s=1.0e-20f,w,dst,lev,dir,sat;

                  if (CC_MAT_ELEM(*f,uchar,i,j+1)!=INSIDE) {
                     if (CC_MAT_ELEM(*f,uchar,i,j-1)!=INSIDE) {
                        gradT.x=(float)((CC_MAT_ELEM(*t,float,i,j+1)-CC_MAT_ELEM(*t,float,i,j-1)))*0.5f;
                     } else {
                        gradT.x=(float)((CC_MAT_ELEM(*t,float,i,j+1)-CC_MAT_ELEM(*t,float,i,j)));
                     }
                  } else {
                     if (CC_MAT_ELEM(*f,uchar,i,j-1)!=INSIDE) {
                        gradT.x=(float)((CC_MAT_ELEM(*t,float,i,j)-CC_MAT_ELEM(*t,float,i,j-1)));
                     } else {
                        gradT.x=0;
                     }
                  }
                  if (CC_MAT_ELEM(*f,uchar,i+1,j)!=INSIDE) {
                     if (CC_MAT_ELEM(*f,uchar,i-1,j)!=INSIDE) {
                        gradT.y=(float)((CC_MAT_ELEM(*t,float,i+1,j)-CC_MAT_ELEM(*t,float,i-1,j)))*0.5f;
                     } else {
                        gradT.y=(float)((CC_MAT_ELEM(*t,float,i+1,j)-CC_MAT_ELEM(*t,float,i,j)));
                     }
                  } else {
                     if (CC_MAT_ELEM(*f,uchar,i-1,j)!=INSIDE) {
                        gradT.y=(float)((CC_MAT_ELEM(*t,float,i,j)-CC_MAT_ELEM(*t,float,i-1,j)));
                     } else {
                        gradT.y=0;
                     }
                  }
                  for (k=i-range; k<=i+range; k++) {
                     int km=k-1+(k==1),kp=k-1-(k==t->rows-2);
                     for (l=j-range; l<=j+range; l++) {
                        int lm=l-1+(l==1),lp=l-1-(l==t->cols-2);
                        if (k>0&&l>0&&k<t->rows-1&&l<t->cols-1) {
                           if ((CC_MAT_ELEM(*f,uchar,k,l)!=INSIDE)&&
                               ((l-j)*(l-j)+(k-i)*(k-i)<=range*range)) {
                              r.y     = (float)(i-k);
                              r.x     = (float)(j-l);

                              dst = (float)(1./(VectorLength(r)*sqrt((double)VectorLength(r))));
                              lev = (float)(1./(1+fabs(CC_MAT_ELEM(*t,float,k,l)-CC_MAT_ELEM(*t,float,i,j))));

                              dir=VectorScalMult(r,gradT);
                              if (fabs(dir)<=0.01) dir=0.000001f;
                              w = (float)fabs(dst*lev*dir);

                              if (CC_MAT_ELEM(*f,uchar,k,l+1)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.x=(float)((CC_MAT_3COLOR_ELEM(*out,uchar,km,lp+1,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm-1,color)))*2.0f;
                                 } else {
                                    gradI.x=(float)((CC_MAT_3COLOR_ELEM(*out,uchar,km,lp+1,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color)));
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.x=(float)((CC_MAT_3COLOR_ELEM(*out,uchar,km,lp,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm-1,color)));
                                 } else {
                                    gradI.x=0;
                                 }
                              }
                              if (CC_MAT_ELEM(*f,uchar,k+1,l)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.y=(float)((CC_MAT_3COLOR_ELEM(*out,uchar,kp+1,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km-1,lm,color)))*2.0f;
                                 } else {
                                    gradI.y=(float)((CC_MAT_3COLOR_ELEM(*out,uchar,kp+1,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color)));
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.y=(float)((CC_MAT_3COLOR_ELEM(*out,uchar,kp,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km-1,lm,color)));
                                 } else {
                                    gradI.y=0;
                                 }
                              }
                              Ia += (float)w * (float)(CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color));
                              Jx -= (float)w * (float)(gradI.x*r.x);
                              Jy -= (float)w * (float)(gradI.y*r.y);
                              s  += w;
                           }
                        }
                     }
                  }
                  sat = (float)((Ia/s+(Jx+Jy)/(sqrt(Jx*Jx+Jy*Jy)+1.0e-20f)+0.5f));
                  {
                  int isat = cRound(sat);
                  CC_MAT_3COLOR_ELEM(*out,uchar,i-1,j-1,color) = CC_CAST_8U(isat);
                  }
               }

               CC_MAT_ELEM(*f,uchar,i,j) = BAND;
               Heap->Push(i,j,dist);
            }
         }
      }

   } else if (CC_MAT_CN(out->tid)==1) {

      while (Heap->Pop(&ii,&jj)) {

         CC_MAT_ELEM(*f,uchar,ii,jj) = KNOWN;
         for(q=0; q<4; q++) {
            if     (q==0) {i=ii-1; j=jj;}
            else if(q==1) {i=ii;   j=jj-1;}
            else if(q==2) {i=ii+1; j=jj;}
            else if(q==3) {i=ii;   j=jj+1;}
            if ((i<=1)||(j<=1)||(i>t->rows-1)||(j>t->cols-1)) continue;

            if (CC_MAT_ELEM(*f,uchar,i,j)==INSIDE) {
               dist = min4(FastMarching_solve(i-1,j,i,j-1,f,t),
                           FastMarching_solve(i+1,j,i,j-1,f,t),
                           FastMarching_solve(i-1,j,i,j+1,f,t),
                           FastMarching_solve(i+1,j,i,j+1,f,t));
               CC_MAT_ELEM(*t,float,i,j) = dist;

               for (color=0; color<=0; color++) {
                  CPoint2D32f gradI,gradT,r;
                  float Ia=0,Jx=0,Jy=0,s=1.0e-20f,w,dst,lev,dir,sat;

                  if (CC_MAT_ELEM(*f,uchar,i,j+1)!=INSIDE) {
                     if (CC_MAT_ELEM(*f,uchar,i,j-1)!=INSIDE) {
                        gradT.x=(float)((CC_MAT_ELEM(*t,float,i,j+1)-CC_MAT_ELEM(*t,float,i,j-1)))*0.5f;
                     } else {
                        gradT.x=(float)((CC_MAT_ELEM(*t,float,i,j+1)-CC_MAT_ELEM(*t,float,i,j)));
                     }
                  } else {
                     if (CC_MAT_ELEM(*f,uchar,i,j-1)!=INSIDE) {
                        gradT.x=(float)((CC_MAT_ELEM(*t,float,i,j)-CC_MAT_ELEM(*t,float,i,j-1)));
                     } else {
                        gradT.x=0;
                     }
                  }
                  if (CC_MAT_ELEM(*f,uchar,i+1,j)!=INSIDE) {
                     if (CC_MAT_ELEM(*f,uchar,i-1,j)!=INSIDE) {
                        gradT.y=(float)((CC_MAT_ELEM(*t,float,i+1,j)-CC_MAT_ELEM(*t,float,i-1,j)))*0.5f;
                     } else {
                        gradT.y=(float)((CC_MAT_ELEM(*t,float,i+1,j)-CC_MAT_ELEM(*t,float,i,j)));
                     }
                  } else {
                     if (CC_MAT_ELEM(*f,uchar,i-1,j)!=INSIDE) {
                        gradT.y=(float)((CC_MAT_ELEM(*t,float,i,j)-CC_MAT_ELEM(*t,float,i-1,j)));
                     } else {
                        gradT.y=0;
                     }
                  }
                  for (k=i-range; k<=i+range; k++) {
                     int km=k-1+(k==1),kp=k-1-(k==t->rows-2);
                     for (l=j-range; l<=j+range; l++) {
                        int lm=l-1+(l==1),lp=l-1-(l==t->cols-2);
                        if (k>0&&l>0&&k<t->rows-1&&l<t->cols-1) {
                           if ((CC_MAT_ELEM(*f,uchar,k,l)!=INSIDE)&&
                               ((l-j)*(l-j)+(k-i)*(k-i)<=range*range)) {
                              r.y     = (float)(i-k);
                              r.x     = (float)(j-l);

                              dst = (float)(1./(VectorLength(r)*sqrt(VectorLength(r))));
                              lev = (float)(1./(1+fabs(CC_MAT_ELEM(*t,float,k,l)-CC_MAT_ELEM(*t,float,i,j))));

                              dir=VectorScalMult(r,gradT);
                              if (fabs(dir)<=0.01) dir=0.000001f;
                              w = (float)fabs(dst*lev*dir);

                              if (CC_MAT_ELEM(*f,uchar,k,l+1)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.x=(float)((CC_MAT_ELEM(*out,uchar,km,lp+1)-CC_MAT_ELEM(*out,uchar,km,lm-1)))*2.0f;
                                 } else {
                                    gradI.x=(float)((CC_MAT_ELEM(*out,uchar,km,lp+1)-CC_MAT_ELEM(*out,uchar,km,lm)));
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.x=(float)((CC_MAT_ELEM(*out,uchar,km,lp)-CC_MAT_ELEM(*out,uchar,km,lm-1)));
                                 } else {
                                    gradI.x=0;
                                 }
                              }
                              if (CC_MAT_ELEM(*f,uchar,k+1,l)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.y=(float)((CC_MAT_ELEM(*out,uchar,kp+1,lm)-CC_MAT_ELEM(*out,uchar,km-1,lm)))*2.0f;
                                 } else {
                                    gradI.y=(float)((CC_MAT_ELEM(*out,uchar,kp+1,lm)-CC_MAT_ELEM(*out,uchar,km,lm)));
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.y=(float)((CC_MAT_ELEM(*out,uchar,kp,lm)-CC_MAT_ELEM(*out,uchar,km-1,lm)));
                                 } else {
                                    gradI.y=0;
                                 }
                              }
                              Ia += (float)w * (float)(CC_MAT_ELEM(*out,uchar,km,lm));
                              Jx -= (float)w * (float)(gradI.x*r.x);
                              Jy -= (float)w * (float)(gradI.y*r.y);
                              s  += w;
                           }
                        }
                     }
                  }
                  sat = (float)((Ia/s+(Jx+Jy)/(sqrt(Jx*Jx+Jy*Jy)+1.0e-20f)+0.5f));
                  {
                  int isat = cRound(sat);
                  CC_MAT_ELEM(*out,uchar,i-1,j-1) = CC_CAST_8U(isat);
                  }
               }

               CC_MAT_ELEM(*f,uchar,i,j) = BAND;
               Heap->Push(i,j,dist);
            }
         }
      }
   }
}


static void
icvNSInpaintFMM(const img_t *f, img_t *t, img_t *out, int range, CvPriorityQueueFloat *Heap) {
   int i = 0, j = 0, ii = 0, jj = 0, k, l, q, color = 0;
   float dist;

   if (CC_MAT_CN(out->tid)==3) {

      while (Heap->Pop(&ii,&jj)) {

         CC_MAT_ELEM(*f,uchar,ii,jj) = KNOWN;
         for(q=0; q<4; q++) {
            if     (q==0) {i=ii-1; j=jj;}
            else if(q==1) {i=ii;   j=jj-1;}
            else if(q==2) {i=ii+1; j=jj;}
            else if(q==3) {i=ii;   j=jj+1;}
            if ((i<=1)||(j<=1)||(i>t->rows-1)||(j>t->cols-1)) continue;

            if (CC_MAT_ELEM(*f,uchar,i,j)==INSIDE) {
               dist = min4(FastMarching_solve(i-1,j,i,j-1,f,t),
                           FastMarching_solve(i+1,j,i,j-1,f,t),
                           FastMarching_solve(i-1,j,i,j+1,f,t),
                           FastMarching_solve(i+1,j,i,j+1,f,t));
               CC_MAT_ELEM(*t,float,i,j) = dist;

               for (color=0; color<=2; color++) {
                  CPoint2D32f gradI,r;
                  float Ia=0,s=1.0e-20f,w,dst,dir;

                  for (k=i-range; k<=i+range; k++) {
                     int km=k-1+(k==1),kp=k-1-(k==f->rows-2);
                     for (l=j-range; l<=j+range; l++) {
                        int lm=l-1+(l==1),lp=l-1-(l==f->cols-2);
                        if (k>0&&l>0&&k<f->rows-1&&l<f->cols-1) {
                           if ((CC_MAT_ELEM(*f,uchar,k,l)!=INSIDE)&&
                               ((l-j)*(l-j)+(k-i)*(k-i)<=range*range)) {
                              r.y=(float)(k-i);
                              r.x=(float)(l-j);

                              dst = 1/(VectorLength(r)*VectorLength(r)+1);

                              if (CC_MAT_ELEM(*f,uchar,k+1,l)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.x=(float)(abs(CC_MAT_3COLOR_ELEM(*out,uchar,kp+1,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,kp,lm,color))+
                                                    abs(CC_MAT_3COLOR_ELEM(*out,uchar,kp,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km-1,lm,color)));
                                 } else {
                                    gradI.x=(float)(abs(CC_MAT_3COLOR_ELEM(*out,uchar,kp+1,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,kp,lm,color)))*2.0f;
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.x=(float)(abs(CC_MAT_3COLOR_ELEM(*out,uchar,kp,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km-1,lm,color)))*2.0f;
                                 } else {
                                    gradI.x=0;
                                 }
                              }
                              if (CC_MAT_ELEM(*f,uchar,k,l+1)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.y=(float)(abs(CC_MAT_3COLOR_ELEM(*out,uchar,km,lp+1,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color))+
                                                    abs(CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm-1,color)));
                                 } else {
                                    gradI.y=(float)(abs(CC_MAT_3COLOR_ELEM(*out,uchar,km,lp+1,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color)))*2.0f;
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.y=(float)(abs(CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color)-CC_MAT_3COLOR_ELEM(*out,uchar,km,lm-1,color)))*2.0f;
                                 } else {
                                    gradI.y=0;
                                 }
                              }

                              gradI.x=-gradI.x;
                              dir=VectorScalMult(r,gradI);

                              if (fabs(dir)<=0.01) {
                                 dir=0.000001f;
                              } else {
                                 dir = (float)fabs(VectorScalMult(r,gradI)/sqrt(VectorLength(r)*VectorLength(gradI)));
                              }
                              w = dst*dir;
                              Ia += (float)w * (float)(CC_MAT_3COLOR_ELEM(*out,uchar,km,lm,color));
                              s  += w;
                           }
                        }
                     }
                  }
                  {
                  int out_val = cRound((double)Ia/s);
                  CC_MAT_3COLOR_ELEM(*out,uchar,i-1,j-1,color) = CC_CAST_8U(out_val);
                  }
               }

               CC_MAT_ELEM(*f,uchar,i,j) = BAND;
               Heap->Push(i,j,dist);
            }
         }
      }

   } else if (CC_MAT_CN(out->tid)==1) {

      while (Heap->Pop(&ii,&jj)) {

         CC_MAT_ELEM(*f,uchar,ii,jj) = KNOWN;
         for(q=0; q<4; q++) {
            if     (q==0) {i=ii-1; j=jj;}
            else if(q==1) {i=ii;   j=jj-1;}
            else if(q==2) {i=ii+1; j=jj;}
            else if(q==3) {i=ii;   j=jj+1;}
            if ((i<=1)||(j<=1)||(i>t->rows-1)||(j>t->cols-1)) continue;

            if (CC_MAT_ELEM(*f,uchar,i,j)==INSIDE) {
               dist = min4(FastMarching_solve(i-1,j,i,j-1,f,t),
                           FastMarching_solve(i+1,j,i,j-1,f,t),
                           FastMarching_solve(i-1,j,i,j+1,f,t),
                           FastMarching_solve(i+1,j,i,j+1,f,t));
               CC_MAT_ELEM(*t,float,i,j) = dist;

               {
                  CPoint2D32f gradI,r;
                  float Ia=0,s=1.0e-20f,w,dst,dir;

                  for (k=i-range; k<=i+range; k++) {
                     int km=k-1+(k==1),kp=k-1-(k==t->rows-2);
                     for (l=j-range; l<=j+range; l++) {
                        int lm=l-1+(l==1),lp=l-1-(l==t->cols-2);
                        if (k>0&&l>0&&k<t->rows-1&&l<t->cols-1) {
                           if ((CC_MAT_ELEM(*f,uchar,k,l)!=INSIDE)&&
                               ((l-j)*(l-j)+(k-i)*(k-i)<=range*range)) {
                              r.y=(float)(i-k);
                              r.x=(float)(j-l);

                              dst = 1/(VectorLength(r)*VectorLength(r)+1);

                              if (CC_MAT_ELEM(*f,uchar,k+1,l)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.x=(float)(abs(CC_MAT_ELEM(*out,uchar,kp+1,lm)-CC_MAT_ELEM(*out,uchar,kp,lm))+
                                                    abs(CC_MAT_ELEM(*out,uchar,kp,lm)-CC_MAT_ELEM(*out,uchar,km-1,lm)));
                                 } else {
                                    gradI.x=(float)(abs(CC_MAT_ELEM(*out,uchar,kp+1,lm)-CC_MAT_ELEM(*out,uchar,kp,lm)))*2.0f;
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k-1,l)!=INSIDE) {
                                    gradI.x=(float)(abs(CC_MAT_ELEM(*out,uchar,kp,lm)-CC_MAT_ELEM(*out,uchar,km-1,lm)))*2.0f;
                                 } else {
                                    gradI.x=0;
                                 }
                              }
                              if (CC_MAT_ELEM(*f,uchar,k,l+1)!=INSIDE) {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.y=(float)(abs(CC_MAT_ELEM(*out,uchar,km,lp+1)-CC_MAT_ELEM(*out,uchar,km,lm))+
                                                    abs(CC_MAT_ELEM(*out,uchar,km,lm)-CC_MAT_ELEM(*out,uchar,km,lm-1)));
                                 } else {
                                    gradI.y=(float)(abs(CC_MAT_ELEM(*out,uchar,km,lp+1)-CC_MAT_ELEM(*out,uchar,km,lm)))*2.0f;
                                 }
                              } else {
                                 if (CC_MAT_ELEM(*f,uchar,k,l-1)!=INSIDE) {
                                    gradI.y=(float)(abs(CC_MAT_ELEM(*out,uchar,km,lm)-CC_MAT_ELEM(*out,uchar,km,lm-1)))*2.0f;
                                 } else {
                                    gradI.y=0;
                                 }
                              }

                              gradI.x=-gradI.x;
                              dir=VectorScalMult(r,gradI);

                              if (fabs(dir)<=0.01) {
                                 dir=0.000001f;
                              } else {
                                 dir = (float)fabs(VectorScalMult(r,gradI)/sqrt(VectorLength(r)*VectorLength(gradI)));
                              }
                              w = dst*dir;
                              Ia += (float)w * (float)(CC_MAT_ELEM(*out,uchar,km,lm));
                              s  += w;
                           }
                        }
                     }
                  }
                  {
                  int out_val = cRound((double)Ia/s);
                  CC_MAT_ELEM(*out,uchar,i-1,j-1) = CC_CAST_8U(out_val);
                  }
               }

               CC_MAT_ELEM(*f,uchar,i,j) = BAND;
               Heap->Push(i,j,dist);
            }
         }
      }

   }
}

#define SET_BORDER1_C1(image,type,value) {\
      int i,j;\
      for(j=0; j<image->cols; j++) {\
         CC_MAT_ELEM(*image,type,0,j) = value;\
      }\
      for (i=1; i<image->rows-1; i++) {\
         CC_MAT_ELEM(*image,type,i,0) = CC_MAT_ELEM(*image,type,i,image->cols-1) = value;\
      }\
      for(j=0; j<image->cols; j++) {\
         CC_MAT_ELEM(*image,type,erows-1,j) = value;\
      }\
   }

#define COPY_MASK_BORDER1_C1(src,dst,type) {\
      int i,j;\
      for (i=0; i<src->rows; i++) {\
         for(j=0; j<src->cols; j++) {\
            if (CC_MAT_ELEM(*src,type,i,j)!=0)\
               CC_MAT_ELEM(*dst,type,i+1,j+1) = INSIDE;\
         }\
      }\
   }


CC_IMPL void
cvInpaint(const img_t* _input_img, const img_t* _inpaint_mask, img_t* _output_img,
           double inpaintRange, int flags)
{
    img_t *mask = 0, *band = 0, *f = 0, *t = 0, *out = 0;
    CvPriorityQueueFloat *Heap = 0, *Out = 0;
    IplConvKernel *el_cross = 0, *el_range = 0;
    
    CC_FUNCNAME("cvInpaint");
    
    __BEGIN__;

    img_t input_hdr, mask_hdr, output_hdr;
    img_t* input_img, *inpaint_mask, *output_img;
    int range=cRound(inpaintRange);    
    int erows, ecols;

    CC_CALL(input_img = cvGetMat(_input_img, &input_hdr));
    CC_CALL(inpaint_mask = cvGetMat(_inpaint_mask, &mask_hdr));
    CC_CALL(output_img = cvGetMat(_output_img, &output_hdr));
    
    if(!CC_ARE_SIZES_EQ(input_img,output_img) || !CC_ARE_SIZES_EQ(input_img,inpaint_mask))
        CC_ERROR(CC_StsUnmatchedSizes, "All the input and output images must have the same size");
    
    if(CC_MAT_TYPE(input_img->tid) != CC_8UC1 &&
        CC_MAT_TYPE(input_img->tid) != CC_8UC3 ||
        !CC_ARE_TYPES_EQ(input_img,output_img))
        CC_ERROR(CC_StsUnsupportedFormat,
        "Only 8-bit 1-channel and 3-channel input/output images are supported");
    
    if(CC_MAT_TYPE(inpaint_mask->tid) != CC_8UC1)
        CC_ERROR(CC_StsUnsupportedFormat, "The mask must be 8-bit 1-channel image");
    
    range = MAX(range,1);
    range = MIN(range,100);

    ecols = input_img->cols + 2;
    erows = input_img->rows + 2;

    CC_CALL(f = cvCreateMat(erows, ecols, CC_8UC1));
    CC_CALL(t = cvCreateMat(erows, ecols, CC_32FC1));
    CC_CALL(band = cvCreateMat(erows, ecols, CC_8UC1));
    CC_CALL(mask = cvCreateMat(erows, ecols, CC_8UC1));
    CC_CALL(el_cross = cvCreateStructuringElementEx(3,3,1,1,CC_SHAPE_CROSS,NULL));
    
    cvCopy(input_img, output_img);
    cvSet(mask,cScalar(KNOWN,0,0,0));
    COPY_MASK_BORDER1_C1(inpaint_mask,mask,uchar);
    SET_BORDER1_C1(mask,uchar,0);
    cvSet(f,cScalar(KNOWN,0,0,0));
    cvSet(t,cScalar(1.0e6f,0,0,0));
    cvDilate(mask,band,el_cross,1);   // image with narrow band
    Heap=new CvPriorityQueueFloat;
    if (!Heap->Init(band))
        EXIT;
    cvSub(band,mask,band,NULL);
    SET_BORDER1_C1(band,uchar,0);
    if (!Heap->Add(band))
        EXIT;
    cvSet(f,cScalar(BAND,0,0,0),band);
    cvSet(f,cScalar(INSIDE,0,0,0),mask);
    cvSet(t,cScalar(0,0,0,0),band);
    
    if(flags == CC_INPAINT_TELEA)
    {
        CC_CALL(out = cvCreateMat(erows, ecols, CC_8UC1));
        CC_CALL(el_range = cvCreateStructuringElementEx(2*range+1,2*range+1,
            range,range,CC_SHAPE_RECT,NULL));
        cvDilate(mask,out,el_range,1);
        cvSub(out,mask,out,NULL);
        Out=new CvPriorityQueueFloat;
        if (!Out->Init(out))
            EXIT;
        if (!Out->Add(band))
            EXIT;
        cvSub(out,band,out,NULL);
        SET_BORDER1_C1(out,uchar,0);
        icvCalcFMM(out,t,Out,true);
        icvTeleaInpaintFMM(mask,t,output_img,range,Heap);
    }
    else
        icvNSInpaintFMM(mask,t,output_img,range,Heap);
    
    __END__;
    
    delete Out;
    delete Heap;
    cvReleaseStructuringElement(&el_cross);
    cvReleaseStructuringElement(&el_range);
    cvReleaseMat(&out);
    cvReleaseMat(&mask);
    cvReleaseMat(&band);
    cvReleaseMat(&t);
    cvReleaseMat(&f);
}
