
#define ThemeColorTable(NAME)  Theme->ColorTable[RibbonColorPart ## NAME]

// #region Pendent for black
// #region Methods

//internal static Color FromHex(string hex)
static Color ColorFromHexStr(const char* hex)
{
  Color clr;
  if ('#'==*hex) {
    ++hex;
  }
  clr = cstr_hex2int(hex, cstr_len(hex));
  clr |= 0xff000000;
  return clr;
}

Color ColorFromHex(const char* hex)
{
  return ColorFromHexStr(hex);
}

static Color ColorToGray(Color c)
{
  int m = (GetRV(c) + (GetGV(c)<<1) + GetBV(c))>>2;
  return _RGB(m, m, m);
}

// #endregion

// #region Colors and Theme

void SetColor(RibbonColorPart ribbonColorPart, Color color)
{
  if (ribbonColorPart<RibbonColorPartMax) {
    Theme->ColorTable[ribbonColorPart] = color;
  }
}
void SetColorRGB(RibbonColorPart ribbonColorPart, int red, int green, int blue)
{
  SetColor(ribbonColorPart, _RGB(red, green, blue));
}

void SetColorString(RibbonColorPart ribbonColorPart, const char* hexColor)
{
  SetColor(ribbonColorPart, ColorFromHex(hexColor));
}

Color GetColor(RibbonColorPart ribbonColorPart)
{
  return ribbonColorPart<RibbonColorPartMax ? Theme->ColorTable[ribbonColorPart] : ColorWhite;
}

const char* GetColorHexStr(RibbonColorPart ribbonColorPart, char* sb, int sblen)
{
  Color c = GetColor(ribbonColorPart);
  int i = cstr_cat(sb, 0, sblen, "#", 1);
  i = cstr_int2hex(sb, i, c, 6);
  return sb;
}


// #endregion

// #region Theme File Read / Write

#if 0
string WriteThemeIniFile()
{
  StringBuilder sb = new StringBuilder();
  sb.AppendLine("[Properties]");
  sb.AppendLine("ThemeName = " + ThemeName);
  sb.AppendLine("Author = " + ThemeAuthor);
  sb.AppendLine("AuthorEmail = " + ThemeAuthorEmail);
  sb.AppendLine("AuthorWebsite = " + ThemeAuthorWebsite);
  sb.AppendLine("DateCreated = " + ThemeDateCreated);
  sb.AppendLine();
  sb.AppendLine("[ColorTable]");
  
  int count = Enum.GetNames(typeof(RibbonColorPart)).Length;
  for (int i = 0; i < count; i++)
  {
    sb.AppendLine(((RibbonColorPart)i).ToString() + " = " + GetColorHexStr((RibbonColorPart)i));
  }
  
  return sb.ToString();
}




void ReadThemeIniFile(string iniFileContent)
{
  string[] sa = null;
  if (iniFileContent.Contains("\r\n"))
  {
    sa = iniFileContent.Split(new string[] { "\r\n" }, StringSplitOptions.None);
  }
  else if (iniFileContent.Contains("\n"))
  {
    sa = iniFileContent.Split(new string[] { "\n" }, StringSplitOptions.None);
  }
  else
  {
    throw new Exception("Unrecognized end line delimeter.");
  }
  
  Dictionary<string, RibbonColorPart> dic1 = new Dictionary<string, RibbonColorPart>();
  foreach (RibbonColorPart e in Enum.GetValues(typeof(RibbonColorPart)))
  {
    dic1[e.ToString().ToLower()] = e;
  }
  
  foreach (string s in sa)
  {
    string a = s.Trim();
    if (a.Length == 0)
    {
      continue;
    }
    else
    {
      string[] sb = a.Split('=');
      if (sb.Length != 2)
        continue;
      string b1 = sb[0].Trim().ToLower();
      string b2 = sb[1].Trim();
      
      if (b1 == "author")
        ThemeAuthor = b2;
      else if (b1 == "authorwebsite")
        ThemeAuthorWebsite = b2;
      else if (b1 == "authoremail")
        ThemeAuthorEmail = b2;
      else if (b1 == "datecreated")
        ThemeDateCreated = b2;
      else if (b1 == "themename")
        ThemeName = b2;
      else
      {
        if (dic1.ContainsKey(b1))
        {
          SetColor(dic1[b1], b2);
        }
      }
    }
  }
}

string WriteThemeXmlFile()
{
  string a = "";
  using (StringWriter str = new StringWriter())
  {
    using (XmlTextWriter xml = new XmlTextWriter(str))
    {
      xml.WriteStartDocument();
      xml.WriteWhitespace("\r\n");
      xml.WriteStartElement("RibbonColorTheme");
      xml.WriteWhitespace("\r\n\t");
      xml.WriteStartElement("Properties");
      xml.WriteWhitespace("\r\n\t\t");
      xml.WriteElementString("ThemeName", ThemeName);
      xml.WriteWhitespace("\r\n\t\t");
      xml.WriteElementString("Author", ThemeAuthor);
      xml.WriteWhitespace("\r\n\t\t");
      xml.WriteElementString("AuthorEmail", ThemeAuthorEmail);
      xml.WriteWhitespace("\r\n\t\t");
      xml.WriteElementString("AuthorWebsite", ThemeAuthorWebsite);
      xml.WriteWhitespace("\r\n\t\t");
      xml.WriteElementString("DateCreated", ThemeDateCreated);
      xml.WriteWhitespace("\r\n\t");
      xml.WriteEndElement();
      xml.WriteWhitespace("\r\n\t");
      xml.WriteStartElement("ColorTable");
      int count = Enum.GetNames(typeof(RibbonColorPart)).Length;
      for (int i = 0; i < count; i++)
      {
        xml.WriteWhitespace("\r\n\t\t");
        xml.WriteElementString(((RibbonColorPart)i).ToString(), GetColorHexStr((RibbonColorPart)i));
      }
      xml.WriteWhitespace("\r\n\t");
      xml.WriteEndElement(); xml.WriteWhitespace("\r\n");
      xml.WriteEndElement(); xml.WriteWhitespace("\r\n");
      xml.WriteEndDocument();
    }
    a = str.ToString();
  }
  return a;
}

void ReadThemeXmlFile(string xmlFileContent)
{
  Dictionary<string, RibbonColorPart> dic1 = new Dictionary<string, RibbonColorPart>();
  foreach (RibbonColorPart e in Enum.GetValues(typeof(RibbonColorPart)))
  {
    dic1[e.ToString().ToLower()] = e;
  }
  
  using (StringReader stringReader = new StringReader(xmlFileContent))
  {
    using (XmlTextReader reader = new XmlTextReader(stringReader))
    {
      while (reader.Read())
      {
        switch (reader.Name)
        {
        case "ThemeName":
          ThemeName = reader.ReadString();
          break;
        case "Author":
          ThemeAuthor = reader.ReadString();
          break;
        case "AuthorEmail":
          ThemeAuthorEmail = reader.ReadString();
          break;
        case "AuthorWebsite":
          ThemeAuthorWebsite = reader.ReadString();
          break;
        case "DateCreated":
          ThemeDateCreated = reader.ReadString();
          break;
        default:
          {
            if (dic1.ContainsKey(reader.Name.ToLower()))
            {
              SetColor(dic1[reader.Name.ToLower()], reader.ReadString());
            }
            break;
          }
        }
      }
    }
  }
}
// #endregion

#endif


void ColorTableDefault() {
#define RIBBONCOLOR_SET(NAME, color)  Theme->ColorTable[ RibbonColorPart ## NAME ] = color
  RIBBONCOLOR_SET(FormBorder, ColorFromHexStr("#3B5A82"));
  
  RIBBONCOLOR_SET(OrbDropDownDarkBorder, _RGB(0x9b, 0xaf, 0xca));
  RIBBONCOLOR_SET(OrbDropDownLightBorder, _RGB(0xff, 0xff, 0xff));
  RIBBONCOLOR_SET(OrbDropDownBack, _RGB(0xbf, 0xd3, 0xeb));
  RIBBONCOLOR_SET(OrbDropDownNorthA, _RGB(0xd7, 0xe5, 0xf7));
  RIBBONCOLOR_SET(OrbDropDownNorthB, _RGB(0xd4, 0xe1, 0xf3));
  RIBBONCOLOR_SET(OrbDropDownNorthC, _RGB(0xc6, 0xd8, 0xee));
  RIBBONCOLOR_SET(OrbDropDownNorthD, _RGB(0xb7, 0xca, 0xe6));
  RIBBONCOLOR_SET(OrbDropDownSouthC, _RGB(0xb0, 0xc9, 0xea));
  RIBBONCOLOR_SET(OrbDropDownSouthD, _RGB(0xcf, 0xe0, 0xf5));
  RIBBONCOLOR_SET(OrbDropDownContentbg, _RGB(0xE9, 0xEA, 0xEE));
  RIBBONCOLOR_SET(OrbDropDownContentbglight, _RGB(0xFA, 0xFA, 0xFA));
  RIBBONCOLOR_SET(OrbDropDownSeparatorlight, _RGB(0xF5, 0xF5, 0xF5));
  RIBBONCOLOR_SET(OrbDropDownSeparatordark, _RGB(0xC5, 0xC5, 0xC5));
  
  /// <summary>
  /// Caption bar is made of 4 rectangles height of each is indicated below
  /// </summary>
  RIBBONCOLOR_SET(Caption1, ColorFromHexStr("#E3EBF6")); //4
  RIBBONCOLOR_SET(Caption2, ColorFromHexStr("#DAE9FD"));
  RIBBONCOLOR_SET(Caption3, ColorFromHexStr("#D5E5FA")); //4
  RIBBONCOLOR_SET(Caption4, ColorFromHexStr("#D9E7F9"));
  RIBBONCOLOR_SET(Caption5, ColorFromHexStr("#CADEF7")); //23
  RIBBONCOLOR_SET(Caption6, ColorFromHexStr("#E4EFFD"));
  RIBBONCOLOR_SET(Caption7, ColorFromHexStr("#B0CFF7")); //1
  
  RIBBONCOLOR_SET(QuickAccessBorderDark, ColorFromHexStr("#B6CAE2"));
  RIBBONCOLOR_SET(QuickAccessBorderLight, ColorFromHexStr("#F2F6FB"));
  RIBBONCOLOR_SET(QuickAccessUpper, ColorFromHexStr("#E0EBF9"));
  RIBBONCOLOR_SET(QuickAccessLower, ColorFromHexStr("#C9D9EE"));
  
  RIBBONCOLOR_SET(OrbOptionBorder, ColorFromHexStr("#7793B9"));
  RIBBONCOLOR_SET(OrbOptionBackground, ColorFromHexStr("#E8F1FC"));
  RIBBONCOLOR_SET(OrbOptionShine, ColorFromHexStr("#D2E1F4"));
  
  // #endregion
  
  // #region Fields
  
  RIBBONCOLOR_SET(Arrow, ColorFromHexStr("#678CBD"));
  RIBBONCOLOR_SET(ArrowLight, _A_RGB(200, ColorWhite));
  RIBBONCOLOR_SET(ArrowDisabled, ColorFromHexStr("#B7B7B7"));
  RIBBONCOLOR_SET(Text, ColorFromHexStr("#15428B"));
  
  /// <summary>
  /// Orb colors in normal state
  /// </summary>
  RIBBONCOLOR_SET(OrbBackgroundDark, ColorFromHexStr("#7C8CA4"));
  RIBBONCOLOR_SET(OrbBackgroundMedium, ColorFromHexStr("#99ABC6"));
  RIBBONCOLOR_SET(OrbBackgroundLight, ColorWhite);
  RIBBONCOLOR_SET(OrbLight, ColorWhite);
  
  /// <summary>
  /// Orb colors in selected state
  /// </summary>
  RIBBONCOLOR_SET(OrbSelectedBackgroundDark, ColorFromHexStr("#DFAA1A"));
  RIBBONCOLOR_SET(OrbSelectedBackgroundMedium, ColorFromHexStr("#F9D12E"));
  RIBBONCOLOR_SET(OrbSelectedBackgroundLight, ColorFromHexStr("#FFEF36"));
  RIBBONCOLOR_SET(OrbSelectedLight, ColorFromHexStr("#FFF52B"));
  
  /// <summary>
  /// Orb colors in pressed state
  /// </summary>
  RIBBONCOLOR_SET(OrbPressedBackgroundDark, ColorFromHexStr("#CE8410"));
  RIBBONCOLOR_SET(OrbPressedBackgroundMedium, ColorFromHexStr("#CE8410"));
  RIBBONCOLOR_SET(OrbPressedBackgroundLight, ColorFromHexStr("#F57603"));
  RIBBONCOLOR_SET(OrbPressedLight, ColorFromHexStr("#F08500"));
  RIBBONCOLOR_SET(OrbBorderAero, ColorFromHexStr("#99A1AD"));
  
  /// <summary>
  /// 2010 style Orb colors
  /// </summary>
  RIBBONCOLOR_SET(OrbButtonText, ColorWhite);
  RIBBONCOLOR_SET(OrbButtonBackground, _RGB(60, 120, 187));
  RIBBONCOLOR_SET(OrbButtonDark, _RGB(25, 65, 135));
  RIBBONCOLOR_SET(OrbButtonMedium, _RGB(56, 135, 191));
  RIBBONCOLOR_SET(OrbButtonLight, _RGB(64, 154, 207));
  RIBBONCOLOR_SET(OrbButtonPressedCenter, _RGB(25, 64, 136));
  RIBBONCOLOR_SET(OrbButtonPressedNorth, _RGB(71, 132, 194));
  RIBBONCOLOR_SET(OrbButtonPressedSouth, _RGB(56, 135, 191));
  RIBBONCOLOR_SET(OrbButtonGlossyNorth, _RGB(71, 132, 194));
  RIBBONCOLOR_SET(OrbButtonGlossySouth, _RGB(46, 104, 178));
  RIBBONCOLOR_SET(OrbButtonBorderDark, _RGB(68, 135, 213));
  RIBBONCOLOR_SET(OrbButtonBorderLight, _RGB(160, 204, 243));
  
  // RIBBONCOLOR_SET(RibbonBackground, ColorFromHexStr("#BFDBFF"));
  RIBBONCOLOR_SET(RibbonBackground, ColorFromHexStr("#BED0E8"));
  
  RIBBONCOLOR_SET(TabBorder, ColorFromHexStr("#8DB2E3"));
  RIBBONCOLOR_SET(TabNorth, ColorFromHexStr("#EBF3FE"));
  RIBBONCOLOR_SET(TabSouth, ColorFromHexStr("#E1EAF6"));
  RIBBONCOLOR_SET(TabGlow, ColorFromHexStr("#D1FBFF"));
  RIBBONCOLOR_SET(TabText, ColorFromHexStr("#15428B"));
  RIBBONCOLOR_SET(TabActiveText, ColorFromHexStr("#15428B"));
  RIBBONCOLOR_SET(TabContentNorth, ColorFromHexStr("#C8D9ED"));
  RIBBONCOLOR_SET(TabContentSouth, ColorFromHexStr("#E7F2FF"));
  RIBBONCOLOR_SET(TabSelectedGlow, ColorFromHexStr("#E1D2A5"));
  RIBBONCOLOR_SET(PanelDarkBorder, _A_RGB(51, ColorFromHexStr("#15428B")));
  RIBBONCOLOR_SET(PanelLightBorder, _A_RGB(102, ColorWhite));
  RIBBONCOLOR_SET(PanelTextBackground, ColorFromHexStr("#C2D9F0"));
  RIBBONCOLOR_SET(PanelTextBackgroundSelected, ColorFromHexStr("#C2D9F0"));
  RIBBONCOLOR_SET(PanelText, ColorFromHexStr("#15428B"));
  RIBBONCOLOR_SET(PanelBackgroundSelected, _A_RGB(102, ColorFromHexStr("#E8FFFD")));
  RIBBONCOLOR_SET(PanelOverflowBackground, ColorFromHexStr("#B9D1F0"));
  RIBBONCOLOR_SET(PanelOverflowBackgroundPressed, ColorFromHexStr("#7699C8"));
  RIBBONCOLOR_SET(PanelOverflowBackgroundSelectedNorth, _A_RGB(100, ColorWhite));
  RIBBONCOLOR_SET(PanelOverflowBackgroundSelectedSouth, _A_RGB(102, ColorFromHexStr("#B8D7FD")));
  
  RIBBONCOLOR_SET(ButtonBgOut, ColorFromHexStr("#C1D5F1"));
  RIBBONCOLOR_SET(ButtonBgCenter, ColorFromHexStr("#CFE0F7"));
  RIBBONCOLOR_SET(ButtonBorderOut, ColorFromHexStr("#B9D0ED"));
  RIBBONCOLOR_SET(ButtonBorderIn, ColorFromHexStr("#E3EDFB"));
  RIBBONCOLOR_SET(ButtonGlossyNorth, ColorFromHexStr("#DEEBFE"));
  RIBBONCOLOR_SET(ButtonGlossySouth, ColorFromHexStr("#CBDEF6"));
  
  RIBBONCOLOR_SET(ButtonDisabledBgOut, ColorFromHexStr("#E0E4E8"));
  RIBBONCOLOR_SET(ButtonDisabledBgCenter, ColorFromHexStr("#E8EBEF"));
  RIBBONCOLOR_SET(ButtonDisabledBorderOut, ColorFromHexStr("#C5D1DE"));
  RIBBONCOLOR_SET(ButtonDisabledBorderIn, ColorFromHexStr("#F1F3F5"));
  RIBBONCOLOR_SET(ButtonDisabledGlossyNorth, ColorFromHexStr("#F0F3F6"));
  RIBBONCOLOR_SET(ButtonDisabledGlossySouth, ColorFromHexStr("#EAEDF1"));
  
  RIBBONCOLOR_SET(ButtonSelectedBgOut, ColorFromHexStr("#FFD646"));
  RIBBONCOLOR_SET(ButtonSelectedBgCenter, ColorFromHexStr("#FFEAAC"));
  RIBBONCOLOR_SET(ButtonSelectedBorderOut, ColorFromHexStr("#C2A978"));
  RIBBONCOLOR_SET(ButtonSelectedBorderIn, ColorFromHexStr("#FFF2C7"));
  RIBBONCOLOR_SET(ButtonSelectedGlossyNorth, ColorFromHexStr("#FFFDDB"));
  RIBBONCOLOR_SET(ButtonSelectedGlossySouth, ColorFromHexStr("#FFE793"));
  
  RIBBONCOLOR_SET(ButtonPressedBgOut, ColorFromHexStr("#F88F2C"));
  RIBBONCOLOR_SET(ButtonPressedBgCenter, ColorFromHexStr("#FDF1B0"));
  RIBBONCOLOR_SET(ButtonPressedBorderOut, ColorFromHexStr("#8E8165"));
  RIBBONCOLOR_SET(ButtonPressedBorderIn, ColorFromHexStr("#F9C65A"));
  RIBBONCOLOR_SET(ButtonPressedGlossyNorth, ColorFromHexStr("#FDD5A8"));
  RIBBONCOLOR_SET(ButtonPressedGlossySouth, ColorFromHexStr("#FBB062"));
  
  RIBBONCOLOR_SET(ButtonCheckedBgOut, ColorFromHexStr("#F9AA45"));
  RIBBONCOLOR_SET(ButtonCheckedBgCenter, ColorFromHexStr("#FDEA9D"));
  RIBBONCOLOR_SET(ButtonCheckedBorderOut, ColorFromHexStr("#8E8165"));
  RIBBONCOLOR_SET(ButtonCheckedBorderIn, ColorFromHexStr("#F9C65A"));
  RIBBONCOLOR_SET(ButtonCheckedGlossyNorth, ColorFromHexStr("#F8DBB7"));
  RIBBONCOLOR_SET(ButtonCheckedGlossySouth, ColorFromHexStr("#FED18E"));
  
  RIBBONCOLOR_SET(ItemGroupOuterBorder, ColorFromHexStr("#9EBAE1"));
  RIBBONCOLOR_SET(ItemGroupInnerBorder, _A_RGB(51, ColorWhite));
  RIBBONCOLOR_SET(ItemGroupSeparatorLight, _A_RGB(64, ColorWhite));
  RIBBONCOLOR_SET(ItemGroupSeparatorDark, _A_RGB(38, ColorFromHexStr("#9EBAE1")));
  RIBBONCOLOR_SET(ItemGroupBgNorth, ColorFromHexStr("#CADCF0"));
  RIBBONCOLOR_SET(ItemGroupBgSouth, ColorFromHexStr("#D0E1F7"));
  RIBBONCOLOR_SET(ItemGroupBgGlossy, ColorFromHexStr("#BCD0E9"));
  
  RIBBONCOLOR_SET(ButtonListBorder, ColorFromHexStr("#B9D0ED"));
  RIBBONCOLOR_SET(ButtonListBg, ColorFromHexStr("#D4E6F8"));
  RIBBONCOLOR_SET(ButtonListBgSelected, ColorFromHexStr("#ECF3FB"));
  
  RIBBONCOLOR_SET(DropDownBg, ColorFromHexStr("#FAFAFA"));
  RIBBONCOLOR_SET(DropDownImageBg, ColorFromHexStr("#E9EEEE"));
  RIBBONCOLOR_SET(DropDownImageSeparator, ColorFromHexStr("#C5C5C5"));
  RIBBONCOLOR_SET(DropDownBorder, ColorFromHexStr("#868686"));
  RIBBONCOLOR_SET(DropDownGripNorth, ColorFromHexStr("#FFFFFF"));
  RIBBONCOLOR_SET(DropDownGripSouth, ColorFromHexStr("#DFE9EF"));
  RIBBONCOLOR_SET(DropDownGripBorder, ColorFromHexStr("#DDE7EE"));
  RIBBONCOLOR_SET(DropDownGripDark, ColorFromHexStr("#5574A7"));
  RIBBONCOLOR_SET(DropDownGripLight, ColorFromHexStr("#FFFFFF"));
  
  RIBBONCOLOR_SET(SeparatorLight, ColorFromHexStr("#FAFBFD"));
  RIBBONCOLOR_SET(SeparatorDark, ColorFromHexStr("#96B4DA"));
  RIBBONCOLOR_SET(SeparatorBg, ColorFromHexStr("#DAE6EE"));
  RIBBONCOLOR_SET(SeparatorLine, ColorFromHexStr("#C5C5C5"));
  
  RIBBONCOLOR_SET(TextBoxUnselectedBg, ColorFromHexStr("#EAF2FB"));
  RIBBONCOLOR_SET(TextBoxBorder, ColorFromHexStr("#ABC1DE"));
  
  RIBBONCOLOR_SET(ToolTipContentNorth, _RGB(250, 252, 254));// SystemColors.MenuBar);// FromHex("#C8D9ED"));
  RIBBONCOLOR_SET(ToolTipContentSouth, _RGB(206, 220, 241));// SystemColors.MenuBar);// FromHex("#E7F2FF"));
  RIBBONCOLOR_SET(ToolTipDarkBorder, ColorDarkGray);// _RGB(51, FromHex("#15428B")));
  RIBBONCOLOR_SET(ToolTipLightBorder, _A_RGB(102, ColorWhite));
  RIBBONCOLOR_SET(ToolTipText, ColorFromHexStr("#15428B"));  // in XP SystemColors.InactiveCaptionText is hardly readable
  
  RIBBONCOLOR_SET(ToolStripItemTextPressed, ColorFromHexStr("#444444"));
  RIBBONCOLOR_SET(ToolStripItemTextSelected, ColorFromHexStr("#444444"));
  RIBBONCOLOR_SET(ToolStripItemText, ColorFromHexStr("#444444"));
  
  RIBBONCOLOR_SET(clrVerBG_Shadow, _RGBA(255, 181, 190, 206));
  
  /// <summary>
  /// 2013 Colors
  /// Office 2013 White Theme
  /// </summary>
  RIBBONCOLOR_SET(ButtonPressed_2013, ColorFromHexStr("#92C0E0"));
  RIBBONCOLOR_SET(ButtonSelected_2013, ColorFromHexStr("#CDE6F7"));
  RIBBONCOLOR_SET(OrbButton_2013, ColorFromHexStr("#0072C6"));
  RIBBONCOLOR_SET(OrbButtonSelected_2013, ColorFromHexStr("#2A8AD4"));
  RIBBONCOLOR_SET(OrbButtonPressed_2013, ColorFromHexStr("#2A8AD4"));
  
  RIBBONCOLOR_SET(TabText_2013, ColorFromHexStr("#0072C6"));
  RIBBONCOLOR_SET(TabTextSelected_2013, ColorFromHexStr("#444444"));
  RIBBONCOLOR_SET(PanelBorder_2013, ColorFromHexStr("#15428B"));
  
  RIBBONCOLOR_SET(RibbonBackground_2013, ColorFromHexStr("#FFFFFF"));
  RIBBONCOLOR_SET(TabCompleteBackground_2013, ColorFromHexStr("#FFFFFF"));
  RIBBONCOLOR_SET(TabNormalBackground_2013, ColorFromHexStr("#FFFFFF"));
  RIBBONCOLOR_SET(TabActiveBackbround_2013, ColorFromHexStr("#FFFFFF"));
  
  RIBBONCOLOR_SET(TabBorder_2013, ColorFromHexStr("#D4D4D4"));
  RIBBONCOLOR_SET(TabCompleteBorder_2013, ColorFromHexStr("#D4D4D4"));
  
  RIBBONCOLOR_SET(TabActiveBorder_2013, ColorFromHexStr("#D4D4D4"));
  
  RIBBONCOLOR_SET(OrbButtonText_2013, ColorFromHexStr("#FFFFFF"));
  RIBBONCOLOR_SET(PanelText_2013, ColorFromHexStr("#666666"));
  RIBBONCOLOR_SET(RibbonItemText_2013, ColorFromHexStr("#444444"));
  RIBBONCOLOR_SET(ToolTipText_2013, ColorFromHexStr("#262626"));
  
  RIBBONCOLOR_SET(ToolStripItemTextPressed_2013, ColorFromHexStr("#444444"));
  RIBBONCOLOR_SET(ToolStripItemTextSelected_2013, ColorFromHexStr("#444444"));
  RIBBONCOLOR_SET(ToolStripItemText_2013, ColorFromHexStr("#444444"));
  // #endregion
}

#include "RibbonProfesionalRendererColorTableBlack.inl"
#include "RibbonProfesionalRendererColorTableBlue.inl"
#undef RIBBONCOLOR_SET