
#ifdef __cplusplus
extern "C" {
#endif

  extern unsigned char *stbi_load_from_memory( unsigned char const * buffer, int len, int * x, int * y, int * comp, int req_comp );
  
#ifdef __cplusplus
};
#endif

static TImageInfo* UILoadImage( STRINGorID bitmap, STRINGorID type, DWORD mask ) {
  LPBYTE pData = NULL;
  DWORD dwRead, dwSize = 0;
  LPBYTE pImage;
  int i, x, y, n;
  BITMAPINFO bmi;
  bool bAlphaChannel;
  LPBYTE pDest;
  HBITMAP hBitmap;
  TImageInfo* data;
  if ( HIWORD( bitmap.s ) != 0 ) {
    HANDLE hFile = CreateFile( bitmap.s, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
      FILE_ATTRIBUTE_NORMAL, NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
      return NULL;
    dwSize = GetFileSize( hFile, NULL );
    if ( dwSize == 0 )
      return NULL;
    dwRead = 0;
    pData = malloc( dwSize );
    ReadFile( hFile, pData, dwSize, &dwRead, NULL );
    CloseHandle( hFile );
    if ( dwRead != dwSize ) {
      free(pData);
      return NULL;
    }
  } else {
    HGLOBAL hGlobal;
    HRSRC hResource = FindResource( GetResourceInstance(), bitmap.s, type.s );
    if ( hResource == NULL )
      return NULL;
    hGlobal = LoadResource( GetResourceInstance(), hResource );
    if ( hGlobal == NULL ) {
      FreeResource( hResource );
      return NULL;
    }
    dwSize = SizeofResource( GetResourceInstance(), hResource );
    if ( dwSize == 0 )
      return NULL;
    pData = malloc( dwSize );
    CopyMemory( pData, ( LPBYTE ) LockResource( hGlobal ), dwSize );
    FreeResource( hResource );
  }
  pImage = NULL;
  pImage = stbi_load_from_memory( pData, dwSize, &x, &y, &n, 4 );
  free(pData) ;
  if ( !pImage )
    return NULL;
  ZeroMemory( &bmi, sizeof( BITMAPINFO ) );
  bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
  bmi.bmiHeader.biWidth = x;
  bmi.bmiHeader.biHeight = -y;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = x * y * 4;
  bAlphaChannel = false;
  pDest = NULL;
  hBitmap = CreateDIBSection( NULL, &bmi, DIB_RGB_COLORS, ( void** ) & pDest, NULL, 0 );
  if ( !hBitmap )
    return NULL;
  for ( i = 0; i < x * y; i++ ) {
    pDest[ i * 4 + 3 ] = pImage[ i * 4 + 3 ];
    if ( pDest[ i * 4 + 3 ] < 255 ) {
      pDest[ i * 4 ] = ( BYTE ) ( (DWORD)( pImage[ i * 4 + 2 ] ) * pImage[ i * 4 + 3 ] / 255 );
      pDest[ i * 4 + 1 ] = ( BYTE ) ( (DWORD)( pImage[ i * 4 + 1 ] ) * pImage[ i * 4 + 3 ] / 255 );
      pDest[ i * 4 + 2 ] = ( BYTE ) ( (DWORD)( pImage[ i * 4 ] ) * pImage[ i * 4 + 3 ] / 255 );
      bAlphaChannel = true;
    } else {
      pDest[ i * 4 ] = pImage[ i * 4 + 2 ];
      pDest[ i * 4 + 1 ] = pImage[ i * 4 + 1 ];
      pDest[ i * 4 + 2 ] = pImage[ i * 4 ];
    }
    if ( *( DWORD* ) ( &pDest[ i * 4 ] ) == mask ) {
      pDest[ i * 4 ] = ( BYTE ) 0;
      pDest[ i * 4 + 1 ] = ( BYTE ) 0;
      pDest[ i * 4 + 2 ] = ( BYTE ) 0;
      pDest[ i * 4 + 3 ] = ( BYTE ) 0;
      bAlphaChannel = true;
    }
  }
  data = malloc(sizeof(TImageInfo));
  data->hBitmap = hBitmap;
  data->nX = x;
  data->nY = y;
  data->alphaChannel = bAlphaChannel;
  return data;
}