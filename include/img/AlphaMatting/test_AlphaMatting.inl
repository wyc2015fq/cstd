

#include "SharedMatting.h"
#include "SharedMatting.inl"

int compareResult(const char * lResultName, const char * rResultName)
{
  img_t  l[1] = {0};
  img_t  r[1] = {0};
  imread(lResultName, 3, 1, l);
  imread(rResultName, 3, 1, r);
  
  if (l->h != r->h ) return 1;
  if (l->w != r->w ) return 1;
  int i, j;
  unsigned char * l_ptr, * r_ptr;
  
  for ( j=0;j<r->h;j++ ){
    l_ptr = l->tt.data + l->s*j;
    r_ptr = r->tt.data + r->s*j;
    for( i=0;i<r->w * r->c;i++ ){
      if( l_ptr[i] != r_ptr[i] )
      {
        cout << "Inconsist found: j="<<j<<",i="<<i<<std::endl;
        return 1;
      }
    }
  }
  
  imfree(l);
  imfree(r);
  
    return 0;
}


int test_AlphaMatting() {
  SharedMatting sm;
    sys_chdir("E:/code/c/Image/cutphoto/AlphaMatting-master/Testset");
    const char* inMap = "input.png";
    const char* triMap = "trimap.png";
    const char* resultMap = "resultMap.png";
    const char* banchMark = "result.png";
    
    sm.loadImage(inMap);
    
    sm.loadTrimap(triMap);
    
    sm.solveAlpha();
    
    sm.save(resultMap);
    
    
    const char* error;
    if( compareResult(banchMark, resultMap) != 0 ){
        error = "Test Fail";
    } else {
        error = "Test Pass";
    }
    
    std::cout << error <<std::endl;
    
    return 0;
}
