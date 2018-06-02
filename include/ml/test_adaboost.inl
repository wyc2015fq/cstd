
#include "inifile.h"
#include "ml/feathaar.inl"
//weaktype
#define updateweight LUT_update_weight
#define geterror LUT_get_error
#define findsamp HAAR_find_samp
#define getfeat HAAR_feat
#define uninit_feat HAAR_uninit


#include "ml/adaboost.inl"

int boosttrain_init(BOOST* hh);

int boosttrain_main()
{
#include "ml/adaboost_train_cascade_impl.inl"
  return 0;
}

int boosttrain_init(BOOST* hh)
{
  int maxstagelen, maxweaklen;
  ininame(INIFILENAME);
  iniappname(ADABOOSTSECT);
  hh->ha = inigetfloat("ha");
  hh->fa = inigetfloat("fa");
  hh->min_neg_rate = inigetfloat("最小负样本比例");
  maxstagelen = inigetint("最大强分类器数");
  maxweaklen = inigetint("最大弱分类器数");
  //inigetfloat( "特征挑选比例", hh->select_rate );
  hh->flag_unreselect = inigetint("不重选特征");
  //hh->curr_pos_num = inigetint( "基本正样本数" );
  //hh->pos_num_step = inigetint( "增加正样本数" );

  // 创建样本容器对象
  hh->pos_num = inigetint("正样本数目");
  hh->samp_num = inigetint("负样本数目") + hh->pos_num;
  cascade_new(hh->ca, "cat.dat", maxstagelen, maxweaklen, sizeof(weaktype));
  hh->ca->is_nesting = inigetint("使用nesting结构");
  HAAR_init(hh);

  logprintf("<< boost 类型: lut, 特征类型: haar, 特征总数: %6d >>\n", hh->feat_num);

  return 0;
}

int test_adaboost()
{
  boosttrain_main();
  return 0;
}
