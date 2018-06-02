
HICON GetAssociatedFileIcon(LPCTSTR lpctszFilePath)
{
  SHFILEINFO shFileInfo = { 0 };
  // For retrieving icon
  SHGetFileInfo(lpctszFilePath, FILE_ATTRIBUTE_NORMAL, &shFileInfo, sizeof(shFileInfo), SHGFI_SMALLICON | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
  // Icon to return
  return shFileInfo.hIcon;
}

