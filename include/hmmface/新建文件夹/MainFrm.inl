/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "HMMDemo.h"
#include "MainFrm.h"
#include "HMMDemoDoc.h"
//#include "NAME1.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
  ON_WM_CREATE()
  ON_UPDATE_COMMAND_UI(ID_CAPTURE, OnUpdateCapture)
  ON_COMMAND(ID_CAPTURE, OnCapture)
  ON_UPDATE_COMMAND_UI(ID_CAPOPTIONS, OnUpdateCapOptions)
  ON_COMMAND(ID_CAPOPTIONS, OnCapOptions)
  ON_UPDATE_COMMAND_UI(ID_CAPFORMAT, OnUpdateCapFormat)
  ON_COMMAND(ID_CAPFORMAT, OnCapFormat)
  ON_COMMAND(ID_ADD_OBJ, OnAddObj)
  //ON_COMMAND(ID_NAME, OnNAME)
  ON_UPDATE_COMMAND_UI(ID_ADD_OBJ, OnUpdateAddObj)
  //ON_UPDATE_COMMAND_UI(ID_NAME, OnUpdateNAME)
  ON_COMMAND(ID_REMOVE_OBJ, OnRemoveObj)
  ON_UPDATE_COMMAND_UI(ID_REMOVE_OBJ, OnUpdateRemoveObj)
  ON_COMMAND(ID_ZOOMIN, OnZoomIn)
  ON_COMMAND(ID_ZOOMOUT, OnZoomOut)
  ON_COMMAND(ID_SETINFO, OnChangeBaseParams)
  ON_COMMAND(ID_TRAIN, OnTrain)
  ON_UPDATE_COMMAND_UI(ID_TRAIN, OnUpdateTrain)
  ON_COMMAND(ID_RECOG, OnRecognize)
  ON_UPDATE_COMMAND_UI(ID_RECOG, OnUpdateRecog)
  ON_COMMAND(ID_SELECTALL, OnSelectAll)
  ON_COMMAND(ID_DEL_HMM, OnDelHmm)
  ON_COMMAND(ID_ADD_TEST, OnAddTest)
  ON_UPDATE_COMMAND_UI(ID_ADD_TEST, OnUpdateAddTest)
  ON_COMMAND(ID_TEST_FOLDER, OnTestFolder)
  ON_COMMAND(ID_RECOBASE, OnRecobase)
  ON_UPDATE_COMMAND_UI(ID_RECOBASE, OnUpdateRecobase)
  ON_COMMAND(ID_SETTINGS, OnSettings)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()
static UINT indicators[] = {
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
  m_busy = FALSE;
  CvHaarClassifierCascade* cascade =
      cvLoadHaarClassifierCascade("<default_face_cascade>",
          cvSize(ORIG_WIN_SIZE, ORIG_WIN_SIZE));
  hid_cascade = cvCreateHidHaarClassifierCascade(cascade, 0, 0, 0, 1);
  cvReleaseHaarClassifierCascade(&cascade);
}
CMainFrame::~CMainFrame()
{
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
    return -1;
  }
  if (!m_wndToolBar.CreateEx(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
    return -1;
  }
  if (!m_wndReBar.Create(this) ||
      !m_wndReBar.AddBar(&m_wndToolBar)) {
    return -1;
  }
  if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
          sizeof(indicators) / sizeof(UINT))) {
    return -1;
  }
  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
      CBRS_TOOLTIPS | CBRS_FLYBY);
  return 0;
}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
    CCreateContext* pContext)
{
  m_wndSplitter.CreateStatic(this, 1, 2);
  m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CImageBaseView), CSize(300, 100), pContext);
  m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CHMMDemoView), CSize(300, 100), pContext);
  //    m_wndSplitter.CreateView(0,2,RUNTIME_CLASS(CTestImageBaseView),CSize(300,100),pContext);
  return TRUE;
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CFrameWnd::PreCreateWindow(cs)) {
    return FALSE;
  }
  return TRUE;
}
CImageBaseView* CMainFrame::GetImageBaseView()
{
  return (CImageBaseView*)m_wndSplitter.GetPane(0, 0);
}
CHMMDemoView* CMainFrame::GetCameraView()
{
  return (CHMMDemoView*)m_wndSplitter.GetPane(0, 1);
}
void CMainFrame::OnUpdateCapture(CCmdUI* pCmdUI)
{
  CHMMDemoView* view = GetCameraView();
  bool enable = false;
  bool press = false;
  if (view) {
    enable = view->Camera().IsInitialized();
    press = view->Camera().IsRunning();
  }
  pCmdUI->Enable(enable);
  pCmdUI->SetCheck(press);
}
void CMainFrame::OnCapture()
{
  CHMMDemoView* view = GetCameraView();//获取摄像头区视口句柄
  if (view && view->Camera().IsInitialized()) {
    if (view->Camera().IsRunning()) {
      view->Camera().Stop();//停止捕捉
    }
    else {
      view->SetImageList(0);//清空记录
      view->Camera().Start();//开始捕捉
    }
  }
}
void CMainFrame::OnUpdateCapOptions(CCmdUI* pCmdUI)
{
  CHMMDemoView* view = GetCameraView();
  pCmdUI->Enable(view != 0);
}
void CMainFrame::OnCapOptions()
{
  CHMMDemoView* view = GetCameraView();
  if (view) {
    view->Camera().VideoSourceDlg();
    view->InvalidateRect(0);
  }
}
void CMainFrame::OnUpdateCapFormat(CCmdUI* pCmdUI)
{
  CHMMDemoView* view = GetCameraView();
  pCmdUI->Enable(view != 0);
}
void CMainFrame::OnCapFormat()
{
  CHMMDemoView* view = GetCameraView();
  if (view) {
    view->Camera().VideoFormatDlg();
    view->InvalidateRect(0);
  }
}
CRect detect_and_draw(IplImage* img, IplImage* temp)
{
  int scale = 2;
  CvPoint pt1, pt2;
  CRect rect;
  int i;
  cvPyrDown(img, temp, CV_GAUSSIAN_5x5);
  cvFlip(temp, temp, 0);
  cvClearMemStorage(storage);
  if (hid_cascade) {
    CvSeq* faces = cvHaarDetectObjects(temp, hid_cascade, storage,
        1.2, 2, CV_HAAR_DO_CANNY_PRUNING);
    for (i = 0; i < (faces ? faces->total : 0); i++) {
      CvRect* r = (CvRect*)cvGetSeqElem(faces, i, 0);
      pt1.x = (r->x + 7) * scale;
      pt2.x = (r->x + r->width - 7) * scale;
      pt1.y = img->height - r->y * scale;
      pt2.y = img->height - (r->y + r->height + 10) * scale;
      cvRectangle(img, pt1, pt2, CV_RGB(0, 255, 0), 3);
      //temp = cvCreateImage( cvSize(frame->width/2,frame->height/2), 8, 3 );
    }
  }
  CPoint p1(pt1.x, img->height - pt1.y);
  CPoint p2(pt2.x, img->height - pt2.y);
  //cvShowImage(WINNAME, img );
  return rect = CRect(p1, p2);
}
void CMainFrame::OnUpdateAddObj(CCmdUI* pCmdUI)
{
  CHMMDemoView* view = GetCameraView();
  // bool enable = false;
  bool enable = true;
  // if( view )
  // {
  //     enable = !view->GetSelection().IsRectEmpty();
  // }
  pCmdUI->Enable(enable);
}
void CMainFrame::OnAddObj()
{
  CHMMDemoView* view = GetCameraView();
  CHMMDemoDoc* doc = GetHMMDoc();
  CRect rect;
  if (view && doc) {
    CCamera& camera = view->Camera();
    camera.Stop();
    //人脸检测部分
    storage = cvCreateMemStorage(0);
    IplImage* image = camera.GetFrame().GetImage();
    IplImage* temp = cvCreateImage(cvSize(image->width / 2, image->height / 2), 8, 3);
    if (image) {
      cvFlip(image, image, 0);
      //  image->origin = IPL_ORIGIN_BL;
      rect = detect_and_draw(image, temp);
      cvWaitKey(0);
      //  cvReleaseImage( &image );
      //  cvReleaseImage( &temp );
      cvFlip(image, image, 0);
      //    CString str;
      //    str.Format("%d,%d,%d,%d",rect.top,rect.bottom,rect.left,rect.right);
      //    AfxMessageBox(str);
    }
    /*    cvNamedWindow( "zhou", 1 );
        cvShowImage("zhou", image );
        cvDestroyWindow("zhou");*/
    doc->AddObj(camera.GetFrame(), /*view->GetSelection()*/rect, view->GetImageList());
    //  camera.GetFrame()返回摄像头当前图片，view->GetSelection返回图片所选中的矩形区域，view->GetImageList()返回记录列表
    camera.Start();
  }
}
//try to write new file to directory
CString GetFreeFilename(CString folder, CString base_name)
{
  int i = 0;
  CString full_name;
  for (;;) {
    FILE* f = 0;
    for (; i < 10000; i++) {
      //GetPersonFullImageName( m_folder, m_folder.GetLength(), filename, path );
      full_name.Format("%s\\%s%04d.bmp", folder, base_name, i);
      f = fopen(full_name, "rb");
      if (!f) {
        break;
      }
      fclose(f);
    }
    if (i == 10000) {
      ASSERT(0); //so many images already exist
      return CString("");
    }
    // try to open for writing. If success, output name
    f = fopen(full_name, "wb");
    if (!f) {
      continue;
    }
    fclose(f);
    remove(full_name);
    break;
  }
  return full_name;
}
void CMainFrame::OnAddTest()
{
  CHMMDemoView* camera_view = GetCameraView();
  CImageBaseView* view = GetImageBaseView();
  CHMMDemoDoc* doc = GetHMMDoc();
  if (view && doc) {
    CRect m_sel = camera_view->GetSelection();
    CCamera& camera = camera_view->Camera();
    camera.Stop();
    //get selected person name
    CPerson* person = doc->GetFaceBase().GetPerson(view->GetPersonIndex());
    CString name = person->GetName();
    //add image which is in view
    CString filename = GetFreeFilename(camera_view->GetTestPath(), name);
    CImage new_img;
    new_img.CopyOf(camera.GetFrame());
    IplImage* iplimage = new_img.GetImage();
    if (!iplimage) {
      MessageBox("No image was selected!");
      return;
    }
    cvSetImageROI(iplimage, RectToCvRect(m_sel));
    CImage tofile;
    tofile.CopyOf(iplimage);
    tofile.Save(filename);
    //add other images
    CStringList* imageList = camera_view->GetImageList();
    if (imageList && (imageList->GetCount() > 1)) {
      //add batch
      POSITION pos = imageList->GetHeadPosition();
      imageList->GetNext(pos);
      while (pos) {
        CImage img;
        img.Load(imageList->GetNext(pos), 1);
        img.Save(GetFreeFilename(camera_view->GetTestPath(), name));
      }
    }
    MessageBox("Images were added to test base");
    camera.Start();
  }
  return;
}
void CMainFrame::OnUpdateRemoveObj(CCmdUI* pCmdUI)
{
  CImageBaseView* view = GetImageBaseView();
  bool enable = false;
  if (view) {
    enable = view->GetActive() >= 0;
  }
  pCmdUI->Enable(enable);
}
void CMainFrame::OnRemoveObj()
{
  CImageBaseView* view = GetImageBaseView();
  CHMMDemoDoc* doc = GetHMMDoc();
  if (doc && view) {
    int active = view->GetActive();
    int person_index = view->GetPersonIndex();
    if (active >= 0) {
      if (doc->RemoveObj(person_index, active)) {
        view->ResetActive();
        view->RefreshView();
      }
    }
  }
}
void CMainFrame::OnZoomIn()
{
  CImageBaseView* view = GetImageBaseView();
  view->Zoom();
}
void CMainFrame::OnZoomOut()
{
  CImageBaseView* view = GetImageBaseView();
  view->Zoom(false);
}
void CMainFrame::OnChangeBaseParams()
{
  CHMMDemoDoc* doc = GetHMMDoc();
  if (doc) {
    doc->ChangeBaseParams();
  }
}
void CMainFrame::OnTrain()
{
  CWaitCursor wait;
  //if 1 person selected - train its HMM
  //if all base in view - train all untrained persons
  CHMMDemoDoc* doc = GetHMMDoc();
  CImageBaseView* base_view = GetImageBaseView();
  if (doc && base_view) {
    //int view_mode = base_view->GetMode();
    CFaceBase& base = doc->GetFaceBase();
    if (base_view->GetPersonIndex() >= 0) {
      //如果是单人模式，训练这个人
      base.TrainPerson(base_view->GetPersonIndex(), true);
    }
    else
      //如果是多人模式，训练所有没有训练过的人
    {
      base.TrainAll(TRAIN_UNTRAINED);
    }
  }
}
void CMainFrame::OnRecognize()
{
  //主要对象声明
  CWaitCursor wait;
  CHMMDemoView* view = GetCameraView();
  CHMMDemoDoc* doc = GetHMMDoc();
  CImageBaseView* baseview = GetImageBaseView();
  CRect rect = CRect(0, 0, 0, 0);
  Cname dlg;
  if (IDOK == dlg.DoModal()) {
    UpdateData();
    m_sName = dlg.m_InputName;
  }
  //AfxMessageBox(m_sName);
  //IplImage* temp;
  if (doc && view && baseview) {
    CFaceBase& base = doc->GetFaceBase();
    CCamera& camera = view->Camera();
    camera.Stop();
    // if (view->GetSelection().IsRectEmpty() )
    // {
    //     MessageBox( "Specify face region", NULL, MB_ICONERROR );
    //    return;
    // }
    //decide if single recognition or batch mode
    CStringList* image_list = view->GetImageList();//获取在HMMDemoView中打开的文件个数
    if (image_list && (image_list->GetCount() > 1)) {
      //perform batch recognition
      int result = base.RecognizeBatch(image_list);
    }
    else {
      int ranged[3];
      //人脸检测部分
      storage = cvCreateMemStorage(0);
      IplImage* image = camera.GetFrame().GetImage();
      IplImage* temp = cvCreateImage(cvSize(image->width / 2, image->height / 2), 8, 3);
      if (image) {
        cvFlip(image, image, 0);
        //  image->origin = IPL_ORIGIN_BL;
        rect = detect_and_draw(image, temp);
        cvWaitKey(0);
        //  cvReleaseImage( &image );
        //    cvReleaseImage( &temp );
        cvFlip(image, image, 0);
        CString str;
        str.Format("%d,%d,%d,%d", rect.top, rect.bottom, rect.left, rect.right);
        AfxMessageBox(str);
      }
      cvNamedWindow("zhou", 1);
      cvShowImage("zhou", image);
      cvDestroyWindow("zhou");
      int result = base.RecognizePerson(camera.GetFrame(), /*view->GetSelection()*/rect,
          ranged); //调用核心代码，进行识别
      if (result == 0) {
        MessageBox("Not all persons are trained", NULL, MB_ICONERROR);
      }
      else {
        CString message = "";
        for (int i = 0 ; i < result; i++) {
          //显示识别出来的人
          CPerson* person = base.GetPerson(ranged[i]);
          message += person->GetName() + "\n";
        }
        /*CPerson* person=base.GetPerson(ranged[0]);
              if(strcmp(m_sName,person->GetName())==0)
                message +="right person";
              else message +="pretend";*/
        baseview->SwitchMode(ranged[0], false);
        MessageBox((LPCTSTR)message, NULL, MB_ICONEXCLAMATION);
      }
    }
    camera.Start();
  }
}
void CMainFrame::OnUpdateRecog(CCmdUI* pCmdUI)
{
  CHMMDemoView* view = GetCameraView();
  // CRect rect = view->GetSelection();
  //pCmdUI->Enable( rect.Width() && rect.Height() );
  pCmdUI->Enable(true);
}
void CMainFrame::OnUpdateTrain(CCmdUI* pCmdUI)
{
  CImageBaseView* view = GetImageBaseView();
  bool enable = false;
  if (view) {
    //int person_index = view->GetPersonIndex();
    CHMMDemoDoc* doc = GetHMMDoc();
    int num_person = doc->GetFaceBase().GetPersonList().GetCount();
    if (doc && num_person >= 0) {
      enable = true;
      //CFaceBase& base = doc->GetFaceBase();
      //CPerson* person = base.GetPerson(person_index);
      //enable = person && person->GetImgList().GetCount() > 0;
    }
  }
  pCmdUI->Enable(enable);
}
void CMainFrame::OnSelectAll() //选中整个图片
{
  CHMMDemoView* view = GetCameraView();
  if (view) {
    view->SetSelection(0);//参数“0”表示选整张图
  }
}
void CMainFrame::OnDelHmm()
{
  CHMMDemoDoc* doc = GetHMMDoc();
  CImageBaseView* view = GetImageBaseView();
  if (doc && view) {
    int person_index = view->GetPersonIndex();
    int result = MessageBox(person_index >= 0 ? "Delete HMM info for current person?" :
        "Delete HMM info for the whole base?", "",
        MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES) {
      doc->DeleteHMMInfo(person_index);
    }
  }
}
void CMainFrame::OnUpdateAddTest(CCmdUI* pCmdUI)
{
  CImageBaseView* view = GetImageBaseView();
  CHMMDemoView* camera_view = GetCameraView();
  if (view) {
    int person_index = view->GetPersonIndex();
    CString path = camera_view->GetTestPath();
    pCmdUI->Enable((person_index >= 0) && (!path.IsEmpty()));
  }
}
void CMainFrame::OnTestFolder()
{
  CHMMDemoView* view = GetCameraView();
  char buffer[1024];
  BROWSEINFO bi;
  bi.hwndOwner = NULL;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = buffer;
  bi.lpszTitle = "Choose directory for test images" ;
  bi.ulFlags = 0;
  bi.lpfn = NULL;
  bi.lParam = 0;
  LPITEMIDLIST il = SHBrowseForFolder(&bi);
  char path[1024];
  if (SHGetPathFromIDList(il, path))
    if (view) {
      view->SetTestPath(path);
    }
}
void CMainFrame::OnRecobase()
{
  CImageBaseView* base_view = GetImageBaseView();
  CFaceBase& original_base = base_view->GetDocument()->GetFaceBase();
  CFileDialog dlg(TRUE, 0, 0, OFN_ENABLESIZING |
      OFN_EXPLORER | OFN_FILEMUSTEXIST,
      "Face Base Files (*.txt)|*.txt|", 0);
  int   buf_size = 1 << 15;
  char* buffer = (char*)malloc(buf_size + 100);
  dlg.m_ofn.lpstrFile = buffer;
  buffer[0] = '\0';
  dlg.m_ofn.nMaxFile = buf_size;
  int result = dlg.DoModal();
  if (result == IDOK) {
    CWaitCursor wait;
    CFaceBase base;
    base.SetFileName(dlg.GetFileName());
    base.Load();
    //recognize
    original_base.RecognizeOtherBase(&base);
  }
}
void CMainFrame::OnUpdateRecobase(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  CImageBaseView* base_view = GetImageBaseView();
  CFaceBase& base = base_view->GetDocument()->GetFaceBase();
  POSITION pos = base.GetPersonList().GetHeadPosition();
  while (pos) {
    CPerson* pers = base.GetPersonList().GetNext(pos);
    if (!pers->IsTrained()) {
      pCmdUI->Enable(FALSE);
      return;
    }
  }
  pCmdUI->Enable(TRUE);
}
void CMainFrame::OnSettings()
{
  CHMMDemoApp* app = (CHMMDemoApp*)AfxGetApp();
  app->OnSettings();// TODO: Add your command handler code here
}

