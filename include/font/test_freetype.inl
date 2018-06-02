#include "lib/freetype-2.4.0/src/freetype.inl"
#include "cstd.h"
int test_freetype1()
{
  FT_Library library = 0; /* handle to library */
  FT_Error error = 0;
  FT_Face face = 0; /* handle to face object */
  char* filename = 0;
  //1. Library Initialization
  //To initialize the FreeType library, create a variable of type FT_Library named, and call the function FT_Init_FreeType.
  error = FT_Init_FreeType(&library);
  if (error) {
    printf("an error occurred during library initialization\n");
    return -1;
  }
  //2. Loading a Font Face
  //From a Font File, Create a new face object by calling FT_New_Face. A face describes a given typeface and style.
  filename = "simsun.ttc";
  filename = "YaHei.Consolas.1.11b.ttf";
  error = FT_New_Face(library, filename, 0, &face);
  if (error == FT_Err_Unknown_File_Format) {
    printf("the font file could be opened and read, its font format is unsupported\n");
    return -1;
  }
  else if (error) {
    printf("the font file could not be opened or read, or that it is broken\n");
    return -1;
  }
  //3. Accessing the Face Data
  //A face object models all information that globally describes the face.
  //Usually, this data can be accessed directly by dereferencing a handle, like in face?>num_glyphs.
  {
    FT_Long numGlygphs = face->num_glyphs;
    FT_Long numFaces = face->num_faces;
    FT_String* familyName = face->family_name;
    printf("num_glyphs = %d, num_faces = %d, family_name = %s\n", numGlygphs, numFaces, familyName);
  }
  //4. Setting the Current Pixel Size
  //FreeType 2 uses size objects to model all information related to a given character size for a given face.
  //error = FT_Set_Pixel_Sizes(face, 0, 16);
  error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0);
  if (error) {
    printf("an error occurs when trying to set the pixel size to a value\n");
    return -1;
  }
  //5. Loading a Glyph Image
  //Converting a Character Code Into a Glyph Index
  {
    int i, j;
    FT_Bitmap* bitmap = 0;
    FT_Glyph glyph = {0};
    FT_BitmapGlyph bitmap_glyph;
    static char cs[] = { ' ', '.', ',', '*', '+', '=', '&', '$', '@', '#'};
    const ushort* ss = L"ÎÒ";
    FT_UInt charIndex = FT_Get_Char_Index(face, *(ushort*)ss); //65 => 'A'
    //Once you have a glyph index, you can load the corresponding glyph image
    error = FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
    if (error) {
      printf("an error occurs when trying to load the corresponding glgyh image\n");
      return -1;
    }
    {
      utime_start(_start_time);
      //6. Simple Text Rendering
      error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
      if (error) {
        printf("an error occurs when trying to render glyph\n");
        return -1;
      }
      // ¶ÁÈ¡Í¼Æ¬
      error = FT_Get_Glyph(face->glyph, &glyph);
      if (error) {
        printf("get glyph error\n");
        return -1;
      }
      //convert glyph to bitmap with 256 gray
      FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
      bitmap_glyph = (FT_BitmapGlyph)glyph;
      bitmap = &bitmap_glyph->bitmap;
      printf("%f\n", utime_elapsed(_start_time));
    }
    for (i = 0 ; i < bitmap->rows; ++ i) {
      for (j = 0 ; j < bitmap->width; ++ j) {
        int g = bitmap->buffer[i * bitmap->width + j];
        int index = g * countof(cs) / 256;
        //if it has gray > 0 we set show it as 1, 0 otherwise
        printf("%c", cs[index]);
      }
      printf("\n");
    }
    FT_Done_Glyph(glyph);
    FT_Done_Face(face);
    FT_Done_FreeType(library);
  }
  printf("ok!\n");
  return 0;
}
#include <direct.h>
int test_freetype()
{
  _chdir("E:/pub/bin/draw/freetype");
  test_freetype1();
  return 0;
}

