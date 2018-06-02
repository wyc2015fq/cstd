
#include "cfile.h"




int main1(const char* fileName)
{
  OFCondition result = EC_Normal;
  /* Load file and get pixel data element */
  DcmFileFormat dfile;
  DJDecoderRegistration::registerCodecs();
  result = dfile.loadFile(fileName);

  if (result.bad()) {
    return 1;
  }

  DcmDataset* data = dfile.getDataset();

  if (data == NULL) {
    return 1;
  }

  DcmElement* element = NULL;
  result = data->findAndGetElement(DCM_PixelData, element);

  if (result.bad() || element == NULL) {
    return 1;
  }

  DcmPixelData* dpix = NULL;
  dpix = OFstatic_cast(DcmPixelData*, element);
  /* Since we have compressed data, we must utilize DcmPixelSequence
     in order to access it in raw format, e. g. for decompressing it
     with an external library.
   */
  DcmPixelSequence* dseq = NULL;
  E_TransferSyntax xferSyntax = EXS_Unknown;
  const DcmRepresentationParameter* rep = NULL;
  // Find the key that is needed to access the right representation of the data within DCMTK
  dpix->getOriginalRepresentationKey(xferSyntax, rep);
  // Access original data representation and get result within pixel sequence
  result = dpix->getEncapsulatedRepresentation(xferSyntax, rep, dseq);

  if (result == EC_Normal) {
    DcmPixelItem* pixitem = NULL;
    // Access first frame (skipping offset table)
    dseq->getItem(pixitem, 1);

    if (pixitem == NULL) {
      return 1;
    }

    Uint8* pixData = NULL;
    // Get the length of this pixel item (i.e. fragment, i.e. most of the time, the lenght of the frame)
    Uint32 length = pixitem->getLength();

    if (length == 0) {
      return 1;
    }

    // Finally, get the compressed data for this pixel item
    result = pixitem->getUint8Array(pixData);
    savedata_11("c:\\aaa.jpg", pixData, length, 0);
    // Do something useful with pixData...
  }

  if (result.bad()) {
    return 1;
  }
  else {
    return 0;
  }
}


int read(const char* fileName)
{
  DcmFileFormat m_dcmFileFormat;
  OFCondition m_dcmCondition = m_dcmFileFormat.loadFile(fileName);

  DJDecoderRegistration::registerCodecs();

  if (m_dcmCondition.good()) {
    DcmDataset* dataset = m_dcmFileFormat.getDataset();
    E_TransferSyntax xfer = dataset->getOriginalXfer();
    xfer = EXS_LittleEndianExplicit;
    dataset->chooseRepresentation(xfer, NULL);

    if (dataset->canWriteXfer(xfer)) {
      DicomImage* m_dcmImage = new DicomImage((DcmObject*)dataset, xfer); //利用dataset生成DicomImage；

      if (m_dcmImage->getStatus() != EIS_Normal) {
        printf("%s\n", DicomImage::getString(m_dcmImage->getStatus()));//这个地方的返回值为 EIS_InvalidValue 不知道什么原因
      }

      m_dcmFileFormat.saveFile("C:\\test_decompressed.dcm", xfer); //输出dcm文件可以，没有问题
      m_dcmImage->writeBMP("C:\\dcm.bmp");//输出bmp文件错误，因为DicomImage对象没有生成
    }
  }

  DJDecoderRegistration::cleanup();
  return 0;
}

int test_mydcmtk()
{
  dirlist_t dl[1] = {0};
  const char* dcmpath = NULL;
  int i;
  dcmpath = "D:/CTdata/CENOVIXupcompress";
  dcmpath = "D:/CTdata/dicomdatabase/ARTERIELLES5";
  dirlist(dl, dcmpath, "*.dcm", 1);

  for (i = 0; i < dl->n; ++i) {
    img_t im[1] = {0};
    DICOMINFO di[1] = {0};
    GetDicomInfo(di, dl->v[0].name);
    //main1(dl->v[0].name);
    read(dl->v[0].name);
    GetDcmLocalImageData(dl->v[0].name, 0, 0, im);
    imshow(im);
    cvWaitKey(-1);
  }

  return 0;
}
