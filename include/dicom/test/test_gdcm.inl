
//#include "gdcm/itkGDCMImageIO.h"
#pragma comment(lib,"gdcm.lib")
#pragma comment(lib,"gdcmjpeg8.lib")
#pragma comment(lib,"gdcmjpeg12.lib")
#pragma comment(lib,"gdcmjpeg16.lib")
#pragma comment(lib,"gdcmopenjpeg.lib")

#include "gdcm/include/libgdcm.h"

//#include "gdcm/itkGDCMImageIO.h"
//#include "gdcm/itkGDCMImageIO.cxx"


//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")

#include "cstd.h"
#include "str.h"
#include "cfile.h"

int didom_unpack(const char* infile, const char* oufile)
{
  gdcm::ImageReader reader;
  reader.SetFileName(infile);

  if (!reader.Read()) {
    printf("Cannot read requested file");
  }

  gdcm::Image& image = reader.GetImage();
  int len = image.GetBufferLength();
  const gdcm::PixelFormat& pixeltype = image.GetPixelFormat();
  int pxsz = pixeltype.GetPixelSize();
  {
    gdcm::ImageWriter   writer;
    gdcm::FileMetaInformation&      fmi = writer.GetFile().GetHeader();
    gdcm::DataSet&      header = writer.GetFile().GetDataSet();
    gdcm::Global&       g = gdcm::Global::GetInstance();
    const gdcm::Dicts& dicts = g.GetDicts();
    const gdcm::Dict&   pubdict = dicts.GetPublicDict();
    gdcm::File& f = reader.GetFile();
    writer.SetImage(image);
    writer.SetFile(reader.GetFile());
    writer.SetFileName(oufile);

    if (!writer.Write()) {
      printf("DICOM does not support this component type");
    }
  }
  return 0;
}

int test_gdcm()
{
  dirlist_t sd[1] = {0};
  const char* path = "D:/CTdata/heater";
  const char* outpath = "D:/CTdata/heater/out";
  int i;
  char outfilename[256];
  sys_chdir(path);
  dirlist(sd, path, "*.dcm", 0);

  sys_mkdir(outpath);

  for (i = 0; i < sd->n; ++i) {
    char* pfnameext = NULL;
    _snprintf(outfilename, countof(outfilename), "%s/%s", outpath, pfnameext = GetFileNameExt(sd->v[i].name));
    didom_unpack(sd->v[i].name, outfilename);
    printf("%s\n", pfnameext);
    //dicom_transForm(sd->v[i].name);
  }

  dirlist_free(sd);
  return 0;
}
