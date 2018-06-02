
#include "lib/libdcmtk.inl"
//#include "lib/dcmtk/lib.inl"

#include "mydcmtk.h"
#include "str.h"

struct  STU_SERIALARRAY {
  int      iSerialNum;
  str_t strSerialUID[1];
  DICOMINFO*  ImageArray;
};
struct STU_STUDYARRAY {
  //  int      iStudyNum;
  str_t strStudyUID[1];
  STU_SERIALARRAY* SerialArray;
};

BOOL GetDicomInfo(DICOMINFO* di, const char* inPath)
{
  DJDecoderRegistration::registerCodecs();
  DcmRLEDecoderRegistration::registerCodecs();

  DcmFileFormat fileformat;
  fileformat.loadFile(inPath);
  DcmDataset* pDataset = NULL;
  pDataset = fileformat.getDataset();

  OFString temp;

  //读取数据
  if (pDataset->findAndGetOFString(DCM_NumberOfFrames, temp, 0).good()) {
    //总帧数
    di->iAllFrame = atoi(temp.data());
    temp.clear();
  }

  //#define DICOMINFO_STR_DEF(name, nameinfo)  if(pDataset->findAndGetOFString(DCM_ ## name, temp, 0).good()) { str_setstr(di->str ## name , temp.data(), temp.length() ); temp.clear(); }
#define DICOMINFO_STR_DEF(name, nameinfo)  if(pDataset->findAndGetOFString(DCM_ ## name, temp, 0).good()) { strncpy(di->str ## name , temp.data(), 32 ); temp.clear(); printf("%s : %s\n", #nameinfo, di->str ## name ); }
  DICOMINFO_STR_DEF_DEF(DICOMINFO_STR_DEF)
#undef DICOMINFO_STR_DEF

  if (pDataset->findAndGetOFString(DCM_PixelSpacing, temp).good()) {
    di->dPixelSpacingX    = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_PixelSpacing, temp, 1).good()) {
    di->dPixelSpacingY    = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImagePositionPatient, temp, 0).good()) {
    di->ImagePosition.x  = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImagePositionPatient, temp, 1).good()) {
    di->ImagePosition.y  = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImagePositionPatient, temp, 2).good()) {
    di->ImagePosition.z  = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImageOrientationPatient, temp, 0).good()) {
    di->ImageOrientationX.x = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImageOrientationPatient, temp, 1).good()) {
    di->ImageOrientationX.y = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImageOrientationPatient, temp, 2).good()) {
    di->ImageOrientationX.z = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImageOrientationPatient, temp, 3).good()) {
    di->ImageOrientationY.x = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImageOrientationPatient, temp, 4).good()) {
    di->ImageOrientationY.y = (double)atof(temp.data());
    temp.clear();
  }

  if (pDataset->findAndGetOFString(DCM_ImageOrientationPatient, temp, 5).good()) {
    di->ImageOrientationY.z = (double)atof(temp.data());
    temp.clear();
  }

  strncpy(di->strFilePath, inPath, countof(di->strFilePath));
  char* strWindowCenter = NULL, *strWindowWidth = NULL;
  pDataset->findAndGetString(DCM_WindowCenter, strWindowCenter);
  pDataset->findAndGetString(DCM_WindowWidth, strWindowWidth);
  di->WinCenter = strWindowCenter ? atoi(strWindowCenter) : 0;
  di->WinWidth = strWindowWidth ? atoi(strWindowWidth) : 0;
  DJDecoderRegistration::cleanup();
  DcmRLEDecoderRegistration::cleanup();
  return TRUE;
}

int GetDcmLocalImageData(const char* inPath, int dWindCenter, int dWindWidth, img_t* im)
{
  DJDecoderRegistration::registerCodecs();
  DcmRLEDecoderRegistration::registerCodecs();

  {
    DicomImage* pDicomImg = NULL;
    pDicomImg = new DicomImage(inPath);

    if (pDicomImg->getStatus() != EIS_Normal) {
      //  AfxMessageBox((LPCTSTR)DicomImage::getString(m_pDicomImg->getStatus()));
      return FALSE;
    }
  }
  DcmFileFormat pDcmFile;
  pDcmFile.loadFile(inPath);
  DcmDataset* pDataset  = NULL;
  pDataset = pDcmFile.getDataset();
  E_TransferSyntax xfer = pDataset->getOriginalXfer();

  OFString temp;

  if (!pDataset->findAndGetOFString(DCM_PixelRepresentation, temp, 0).good()) {
    pDataset->putAndInsertUint16(DCM_PixelRepresentation, 0); //1有符号
  }

  DicomImage* pDicomImg = NULL;
  pDicomImg = new DicomImage(pDataset, xfer, CIF_AcrNemaCompatibility, 0, 0);

  if (pDicomImg->getStatus() != EIS_Normal) {
    //  AfxMessageBox((LPCTSTR)DicomImage::getString(m_pDicomImg->getStatus()));
    return FALSE;
  }

  if (dWindCenter == 0) {
    char* strWindowCenter, *strWindowWidth;
    pDataset->findAndGetString(DCM_WindowCenter, strWindowCenter);
    pDataset->findAndGetString(DCM_WindowWidth, strWindowWidth);
    pDicomImg->setWindow(atoi(strWindowCenter), atoi(strWindowWidth));
  }
  else {
    pDicomImg->setWindow(dWindCenter, dWindWidth);
  }

  EI_Status r = pDicomImg->getStatus();

  if (pDicomImg->getStatus() != EIS_Normal) {
    //  AfxMessageBox((LPCTSTR)DicomImage::getString(m_pDicomImg->getStatus()));
    return FALSE;
  }

  BOOL bReVal = FALSE;
  //(3)读出的图像数据
  BYTE* pDicomDibits = NULL;
  int nSize = 0;
  //得到DICOM文件第frame 的DIB 数据(假设是24 位的)

  if (!pDicomImg->isMonochrome()) {
    nSize = pDicomImg->createWindowsDIB((void*&)pDicomDibits, 0, 0, 24, 1, 1);
  }
  else {
    nSize = pDicomImg->createWindowsDIB((void*&)pDicomDibits, 0, 0, 24, 1, 1);
  }

  if (pDicomDibits != NULL) {
    imsetsize(im, pDicomImg->getHeight(), pDicomImg->getWidth(), 3, 1);
    memcpy(im->tt.data, pDicomDibits, nSize);
    bReVal = TRUE;
  }
  else {
    bReVal = FALSE;
  }

  delete pDicomDibits;
  pDicomDibits = NULL;

  DJDecoderRegistration::cleanup();
  DcmRLEDecoderRegistration::cleanup();
  return bReVal;
}

//创建对象
void* BCreateObject()
{
  return NULL;
}
//销毁对象
BOOL BDestroyObject(void* pImageApi)
{
  return 0;
}
//设置窗口句柄
BOOL BSetWindowHandle(void* pImageApi)
{
  return 0;
}
//添加图像
BOOL BAddImage(void* pImageApi, char* inPath)
{
  return 0;
}
//布局
BOOL BCreateSerialWnd(void* pImageApi, int iRow, int iCol)
{
  return 0;
}
//分格
BOOL BCreateImageWnd(void* pImageApi, int iRow, int iCol)
{
  return 0;
}
BOOL BRefreshImageWnd(void* pImageApi, int iStudy)
{
  return 0;
}

#include "test_mydcmtk.inl"