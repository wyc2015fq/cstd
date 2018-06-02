

enum COMPRESSION_MODE {
  COMPRESS_NONE = 0,
  COMPRESS_RLE,
  COMPRESS_JPEGLOSSY,
  COMPRESS_JPEGLOSSY12BIT,
  COMPRESS_JPEGLOSSLESS,
  COMPRESS_JPEGLOSSLESS2
};
enum DATA_ENDIAN {
  LITTLE_ENDIAN,
  BIG_ENDIAN
};




void SwapWord(char* pArray, int nWords)
{
  char* cc = pArray, c0;
  int i;

  // Swap every two bytes
  for (i = 0; i < nWords; i ++) {
    c0 = *cc;
    *cc = *(cc + 1);
    *(cc + 1) = c0;

    cc += 2;
  }
}




void removeTailingSpace(char* pszStr)
{
  char* cc = pszStr + strlen(pszStr) - 1;

  while (((*cc == ' ') || (*cc == '\t')) && (cc != pszStr)) {
    *cc -- = '\0';
  }

}

void SwapDWord(char* pArray, int nDWords)
{
  char* cc = pArray, c0;
  int i;

  // Rotate every four bytes
  for (i = 0; i < nDWords; i ++) {

    // Swap first and last bytes
    c0 = *cc;
    *cc = *(cc + 3);
    *(cc + 3) = c0;

    // Swap middle two bytes
    c0 = *(cc + 2);
    *(cc + 2) = *(cc + 1);
    *(cc + 1) = c0;

    cc += 4;
  }

}



long int readLength(FILE* fp, BOOL bImplicitVR, DATA_ENDIAN nDataEndian)
{
  long int nValLength = 0;
  short int nsLength;

  if (bImplicitVR) {
    fread(&nValLength, sizeof(long), 1, fp);

    if (nDataEndian == BIG_ENDIAN) {
      SwapDWord((char*) &nValLength, 1);
    }
  }
  else {
    fseek(fp, 2, SEEK_CUR); // Skip 2 VR bytes

    fread(&nsLength, sizeof(short), 1, fp);

    if (nDataEndian == BIG_ENDIAN) {
      SwapWord((char*) &nsLength, 1);
    }

    nValLength = nsLength;
  }

  return nValLength;
}


int readString(FILE* fp, char* pszStr, BOOL bImplicitVR, DATA_ENDIAN nDataEndian)
{
  long int nValLength = 0;

  nValLength = readLength(fp, bImplicitVR, nDataEndian);

  if ((nValLength > 64) || (nValLength < 0)) {
    return -1;
  }

  fread(pszStr, 1, nValLength, fp);
  pszStr[nValLength] = '\0';
  removeTailingSpace(pszStr);

  return 0;
}

int readUS(FILE* fp, DATA_ENDIAN nDataEndian)
{
  unsigned short nVal;

  fseek(fp, 4, SEEK_CUR); // Skip VR and Length bytes (4)
  fread(&nVal, 1, sizeof(short), fp); // read the unsigned short value

  if (nDataEndian == BIG_ENDIAN) {
    SwapWord((char*) &nVal, 1);
  }

  return (int) nVal;


}




int readIS(FILE* fp, BOOL bImplicitVR, DATA_ENDIAN nDataEndian)
{
  char szTemp[64] = "";
  int nVal = 0;

  if (readString(fp, szTemp, bImplicitVR, nDataEndian) == 0) {
    sscanf(szTemp, "%d", &nVal);
  }

  return nVal;

}

float readDS(FILE* fp, BOOL bImplicitVR, DATA_ENDIAN nDataEndian)
{
  char szTemp[64] = "";
  float fVal = 0;

  if (readString(fp, szTemp, bImplicitVR, nDataEndian) == 0) {
    sscanf(szTemp, "%f", &fVal);
  }

  return fVal;
}

char* convertTo8Bit(char* pData, long nNumPixels, BOOL bIsSigned, short nHighBit, float fRescaleSlope, float fRescaleIntercept, float fWindowCenter, float fWindowWidth)
{
  unsigned char* pNewData = 0;
  long int nCount;
  short* pp;

  // 1. Clip the high bits.
  if (nHighBit < 15) {
    short nMask;
    short nSignBit;

    pp = (short*)pData;
    nCount = nNumPixels;

    if (bIsSigned == 0) { // Unsigned integer
      nMask = 0xffff << (nHighBit + 1);

      while (nCount-- > 0) {
        *(pp ++) &= ~nMask;
      }
    }
    else {
      // 1's complement representation

      nSignBit = 1 << nHighBit;
      nMask = 0xffff << (nHighBit + 1);

      while (nCount -- > 0) {
        if ((*pp & nSignBit) != 0) {
          *(pp ++) |= nMask;
        }
        else {
          *(pp ++) &= ~nMask;
        }
      }
    }
  }

  // 2. Rescale if needed (especially for CT)
  if ((fRescaleSlope != 1.0f) || (fRescaleIntercept != 0.0f)) {
    float fValue;

    pp = (short*)pData;
    nCount = nNumPixels;

    while (nCount-- > 0) {
      fValue = (*pp) * fRescaleSlope + fRescaleIntercept;
      *pp ++ = (short)fValue;
    }

  }

  // 3. Window-level or rescale to 8-bit
  if ((fWindowCenter != 0) || (fWindowWidth != 0)) {
    float fSlope;
    float fShift;
    float fValue;
    unsigned char* np = new unsigned char[nNumPixels + 4];

    pNewData = np;

    // Since we have window level info, we will only map what are
    // within the Window.

    fShift = fWindowCenter - fWindowWidth / 2.0f;
    fSlope = 255.0f / fWindowWidth;

    nCount = nNumPixels;
    pp = (short*)pData;

    while (nCount-- > 0) {
      fValue = ((*pp ++) - fShift) * fSlope;

      if (fValue < 0) {
        fValue = 0;
      }
      else if (fValue > 255) {
        fValue = 255;
      }

      *np ++ = (unsigned char) fValue;
    }

  }
  else {
    // We will map the whole dynamic range.
    float fSlope;
    float fValue;
    int nMin, nMax;
    unsigned char* np = new unsigned char[nNumPixels + 4];

    pNewData = np;

    // First compute the min and max.
    nCount = nNumPixels;
    pp = (short*)pData;
    nMin = nMax = *pp;

    while (nCount-- > 0) {
      if (*pp < nMin) {
        nMin = *pp;
      }

      if (*pp > nMax) {
        nMax = *pp;
      }

      pp ++;
    }

    // Calculate the scaling factor.
    if (nMax != nMin) {
      fSlope = 255.0f / (nMax - nMin);
    }
    else {
      fSlope = 1.0f;
    }

    nCount = nNumPixels;
    pp = (short*)pData;

    while (nCount-- > 0) {
      fValue = ((*pp ++) - nMin) * fSlope;

      if (fValue < 0) {
        fValue = 0;
      }
      else if (fValue > 255) {
        fValue = 255;
      }

      *np ++ = (unsigned char) fValue;
    }
  }

  return (char*)pNewData;

}

int WriteBMPFile(char* pData, int nFrameSize, short nCols, short nRows, int nBytesP, char* pszPhotometric, int nFrameNum)
{
#if 0
  BITMAPFILEHEADER bf;
  BITMAPINFOHEADER bi;
  int nPaletteSize = sizeof(RGBQUAD) * 256;
  char* cc;
  FILE* fp;
  int i, j, nBytes, nRowPadding, nRowBytes;
  unsigned char oneLutSlot[4];
  char szPadding[4] = {'\0', '\0', '\0', '\0'};

  if (!strncmp(pszPhotometric, "RGB", 3) || !strncmp(pszPhotometric, "YBR", 3)) {
    nPaletteSize = 0;
  }

  // Attention:
  // 1. BMP image rows need to be 4-byte aligned.
  // 2. BMP image is usually bottom up in reverse direction to DICOM images.
  nRowBytes = nCols * nBytesP;
  nRowPadding = nRowBytes % 4;

  //here fill in the two info structure
  bf.bfType = 0x4D42;
  bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nPaletteSize + nFrameSize + nRowPadding * nRows;
  bf.bfReserved1 = 0;
  bf.bfReserved2 = 0;
  bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nPaletteSize;

  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = nCols;
  bi.biHeight = nRows;
  bi.biPlanes = 1;

  if (nPaletteSize == 0) {
    bi.biBitCount = 24;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
  }
  else {
    bi.biBitCount = 8;
    bi.biClrUsed = 256;
    bi.biClrImportant = 256;
  }

  bi.biCompression = 0;
  bi.biSizeImage = nFrameSize;

  //
  // Resolutions are in pixels per meter. Our IMAGES file uses
  // a resolution of 72 pixels per inch, which when multiplied by
  // 39.37 inches per meter, gives us 2835 pixels per meter.
  //
  bi.biXPelsPerMeter = 2835;
  bi.biYPelsPerMeter = 2835;

  // Make file name and give it a .BMP extension.

  // Open the file for wirte.
  char tmp[200];
  sprintf(tmp, "_%04d.BMP", nFrameNum);
  fp = fopen(LPCTSTR(this->srcFileName + tmp), "wb");

  if (fp == NULL) {
    printf("Failed to open file for write.");
  }

  // Write the BMP and DIB file headers.
  nBytes = fwrite((void*)&bf, 1, sizeof(BITMAPFILEHEADER), fp);
  // if (nBytes != sizeof(BITMAPFILEHEADER))
  // post your error message here.

  nBytes = fwrite((void*)&bi, 1, sizeof(BITMAPINFOHEADER), fp);
  // if (nBytes != sizeof(BITMAPINFOHEADER))
  // post your error message here.


  // Write the color palette if 8-bit
  if (nPaletteSize != 0)
    for (i = 0; i < 256; i++) {
      oneLutSlot[0] = oneLutSlot[1] = oneLutSlot[2] = i; // Grayscale LUT.
      oneLutSlot[3] = 0;
      fwrite(oneLutSlot, 1, 4, fp); // Write one color palette slot.
    }

  // Now write the actual pixel data.
  cc = pData + (nRows - 1) * nRowBytes;

  if (nPaletteSize != 0) // Grayscale
    for (i = 0; i < nRows; i ++) {
      nBytes = fwrite(cc, 1, nRowBytes, fp); // Write a row.
      //if (nBytes != nRowBytes)
      // post your error message here.

      if (nRowPadding) { // Pad the row.
        fwrite(szPadding, 1, nRowPadding, fp);
      }

      cc -= nRowBytes;
    }
  else
    for (i = 0; i < nRows; i ++) {
      for (j = 0; j < nCols; j++, cc += 3) {
        fputc(*(cc + 2), fp); // B
        fputc(*(cc + 1), fp); // G
        fputc(*cc, fp); // R
      }

      if (nRowPadding) { // Pad the row.
        fwrite(szPadding, 1, nRowPadding, fp);
      }

      cc -= 2 * nRowBytes;
    }

  fclose(fp);


#endif
  return 0;
}



int dicom_transForm(const char* srcFileName)
{
  short nCols = 0, nRows = 0; //初始化行数和列数为0
  short nBitsAllocated, nSamplesPerPixel = 1;
  short nHighBit = 0;
  float fWindowWidth = 0, fWindowCenter = 0, fRescaleSlope = 1, fRescaleIntercept = 0;
  BOOL bIsSigned = FALSE;
  BOOL bGroup2Done = FALSE, bGroup28Done = FALSE, bPixelDataDone = FALSE;
  int nBytesP = 0;
  int nFrameSize = 0;
  int InstanceNumber = 0;
  long int nLength;
  char szPhotometric[32] = "", szTemp[32] = "", szTransferSyntaxUID[80] = "";
  BOOL bImplicitVR = TRUE;
  COMPRESSION_MODE nCompressionMode = COMPRESS_NONE;
  DATA_ENDIAN nDataEndian = LITTLE_ENDIAN;
  int i;
  int nBytes;
  char pszStr[64] = "";
  char str[256];
  char Instance[256];
  char Internal[256];
  int len;
  FILE* fp;
  char* pData = 0;
  short int gTag, eTag;
  int nNumFrames = 1;

  fp = fopen(srcFileName, "rb");//打开二进制文件

  if (!fp) {
    printf("源文件打开失败！.");
    return 0;
  }

  if (fread(Internal, 128 + 4, 1, fp) == 1) {
    if (*(int*)(Internal + 128) != *(int*)"DICM") {
      return 0;
    }
  }

  while (fread(&gTag, sizeof(short), 1, fp) == 1) {
    if (nDataEndian == BIG_ENDIAN) {
      SwapWord((char*) &gTag, 1);
    }

    switch (gTag) {
    case 0x0020:
      fread(&eTag, sizeof(short), 1, fp);

      switch (eTag) {
        //Instance Number
      case 0x0013:
        InstanceNumber = readIS(fp, bImplicitVR, nDataEndian);
        _snprintf(str, countof(str), "%d", InstanceNumber);
        printf(str);
        break;

        //Series Instrance UID
      case 0x000E:
        //str.Format("%d",readLength(fp,bImplicitVR,nDataEndian));
        len = fread(pszStr, 1, readLength(fp, bImplicitVR, nDataEndian), fp);
        ASSERT(len < countof(pszStr));
        strncpy(str, pszStr, len);
        str[len] = 0;
        printf(str);
      }

      break;

    case 0x0002: // Meta header.
      if (1) {
        if (bGroup2Done) {
          break;
        }

        fread(&eTag, sizeof(short), 1, fp);

        // Meta header is always in Little Endian Explicit VR syntax.
        switch (eTag) {
        case 0x0010: // Transfer syntax UID
          if (readString(fp, szTransferSyntaxUID, FALSE, LITTLE_ENDIAN) != 0) {
            break;
          }

          // Check data endian.
          if (!strcmp(szTransferSyntaxUID, "1.2.840.10008.1.2.2")) { // Explicit VR Big Endian
            nDataEndian = BIG_ENDIAN;  // Big Endian
          }
          else {
            nDataEndian = LITTLE_ENDIAN;  // Little Endian
          }

          // Check if it is implicit VR or Explicit VR
          if (!strcmp(szTransferSyntaxUID, "1.2.840.10008.1.2")) { // Implicit VR Little Endian
            bImplicitVR = TRUE;  // Implicit VR
          }
          else {
            bImplicitVR = FALSE;  // Explicit VR
          }

          // Parse the encapsulation/compression
          if (!strcmp(szTransferSyntaxUID, "1.2.840.10008.1.2.4.50")) { // JPEG lossy
            nCompressionMode = COMPRESS_JPEGLOSSY;
          }
          else if (!strcmp(szTransferSyntaxUID, "1.2.840.10008.1.2.4.51")) { // JPEG lossy 12bit
            nCompressionMode = COMPRESS_JPEGLOSSY12BIT;
          }
          else if (!strcmp(szTransferSyntaxUID, "1.2.840.10008.1.2.4.70")) { // JPEG lossless first order prediction
            nCompressionMode = COMPRESS_JPEGLOSSLESS;
          }
          else if (!strcmp(szTransferSyntaxUID, "1.2.840.10008.1.2.4.57")) { // JPEG lossless process 14
            nCompressionMode = COMPRESS_JPEGLOSSLESS2;
          }
          else if (!strcmp(szTransferSyntaxUID, "1.2.840.10008.1.2.5")) { // RLE
            nCompressionMode = COMPRESS_RLE;
          }

          bGroup2Done = TRUE;

          break;
        }
      }

      break;

    case 0x0008: // First non-Meta group
      if (1) {
        fread(&eTag, sizeof(short), 1, fp);

        if (nDataEndian == BIG_ENDIAN) {
          SwapWord((char*) &eTag, 1);
        }

        if ((eTag == 0x0005) || (eTag == 0x0008)) {
          bGroup2Done = TRUE;
        }
      }

      break;

    case 0x0028: // Image pixel data info group
      if (1) {
        fread(&eTag, sizeof(short), 1, fp);

        if (bGroup28Done) {
          break;
        }

        if (nDataEndian == BIG_ENDIAN) {
          SwapWord((char*) &eTag, 1);
        }

        switch (eTag) {
        case 0x0002: // Samples per Pixel
          nSamplesPerPixel = readUS(fp, nDataEndian);
          break;

        case 0x0004: // Photometric Interpolation
          readString(fp, szPhotometric, bImplicitVR, nDataEndian);
          break;

        case 0x0008: // Number of frames
          nNumFrames = readIS(fp, bImplicitVR, nDataEndian);
          break;

        case 0x0010: // Rows
          nRows = readUS(fp, nDataEndian);
          break;

        case 0x0011: // Columns
          nCols = readUS(fp, nDataEndian);
          break;

        case 0x0100: // Bits allocated
          nBitsAllocated = readUS(fp, nDataEndian);
          break;

        case 0x0102: // High bit
          nHighBit = readUS(fp, nDataEndian);
          break;

        case 0x0103: // Is signed?
          bIsSigned = readUS(fp, nDataEndian);
          break;

        case 0x1050: // Window Center
          fWindowCenter = readDS(fp, bImplicitVR, nDataEndian);
          break;

        case 0x1051: // Window Width
          fWindowWidth = readDS(fp, bImplicitVR, nDataEndian);
          break;

        case 0x1052: // Rescale intercept
          fRescaleIntercept = readDS(fp, bImplicitVR, nDataEndian);
          break;

        case 0x1053: // Rescale slope
          fRescaleSlope = readDS(fp, bImplicitVR, nDataEndian);
          bGroup28Done = TRUE;
          break;

        default:
          // do nothing
          break;
        }
      }

      break;

    case 0x7fe0:
      if (1) {
        fread(&eTag, sizeof(short), 1, fp);

        if (bPixelDataDone) {
          break;
        }

        if (nDataEndian == BIG_ENDIAN) {
          SwapWord((char*) &eTag, 1);
        }

        if (eTag == 0x0010) {
          nBytesP = nSamplesPerPixel * nBitsAllocated / 8;
          nFrameSize = nCols * nRows * nBytesP;
          nLength = nNumFrames * nFrameSize;

          // Don't try to parse grup 2 and group 28 any more
          bGroup2Done = TRUE;
          bGroup28Done = TRUE;

          // Parse pixel data
          switch (nCompressionMode) {
          case COMPRESS_NONE:
            pData = new char[nLength + 16];
            fseek(fp, 4, SEEK_CUR); // Skip 4 bytes (length bytes)
            nBytes = fread(pData, 1, nLength, fp);

            if (nBytes != nLength) {
              printf("Failed to read all pixel data.");
            }

            bPixelDataDone = TRUE;
            break;

          case COMPRESS_RLE:
            printf("RLE compression not supported at this moment");
            // To do:
            // 1. Read the offset table.
            // 2. Read and uncompress RLE image frames into either RGB or monochrome format.
            // 3. Put frames in the pData buffer, one frame after another.
            // Public domain RLE decompression source code is in Papyrus and dcmtk.
            break;

          case COMPRESS_JPEGLOSSY:
            printf("JPEG lossy compression not supported at this moment");
            // To do:
            // 1. Read the offset table
            // 2. Read and uncompress JPEG image frames into either RGB or monochrome format.
            // 3. Put frames in the pData buffer, one frame after another.
            // Public domain JPEG decompression source code is in Papyrus and dcmtk.
            break;

          case COMPRESS_JPEGLOSSY12BIT:
            printf("JPEG lossy 12-bit compression not supported at this moment");
            // To do:
            // 1. Read the offset table
            // 2. Read and uncompress JPEG image frames into either RGB or monochrome format.
            // 3. Put frames in the pData buffer, one frame after another.
            // Public domain JPEG decompression source code is in Papyrus and dcmtk.
            break;

          case COMPRESS_JPEGLOSSLESS:
          case COMPRESS_JPEGLOSSLESS2:
            printf("JPEG lossless compression not supported at this moment");
            // To do:
            // 1. Read the offset table
            // 2. Read and uncompress JPEG image frames into either RGB or monochrome format.
            // 3. Put frames in the pData buffer, one frame after another.
            // Public domain JPEG decompression source code is in Papyrus and dcmtk.
            break;
          }
        }
      }

      break;

    default:
      break;
    }

    if (pData) {
      break;  // We are done.
    }
  }

  fclose(fp);
#if 0

  if (pData) { //判断我们是否得到了需要的数据
    // Need to do byte swap?
    if (nDataEndian == BIG_ENDIAN) {
      if (nBitsAllocated > 8) {
        SwapWord(pData, nLength / 2);
      }
    }

    if (nBitsAllocated > 8) {
      // We need to convert it to 8-bit.
      char* pNewData;

      pNewData = convertTo8Bit(pData, nLength / 2, bIsSigned, nHighBit,
          fRescaleSlope, fRescaleIntercept, fWindowCenter, fWindowWidth);

      // Use the new 8-bit data.
      if (pNewData) {
        delete [] pData;
        pData = pNewData;
        nBytesP = 1;
        nFrameSize /= 2;

        nLength /= 2;
      }
    }

    // Write BMP files
    for (i = 0; i < nNumFrames; i++) {
      WriteBMPFile(pData + nFrameSize * i, nFrameSize, nCols, nRows, nBytesP, szPhotometric, i + 1);
    }

    // Free the memory.
    delete [] pData;
  }

#endif
  return 0;
}
