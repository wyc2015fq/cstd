
// Fallout FON-file viewer
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int width;		//字符宽度
  int offset;		//字符在数据块中的映射位置
} FontInfo;

typedef struct {
	int num; 			//文件包含的字符数目
	int height; 			//字体高度
	int spacing; 			//字体相邻字符间距
	FontInfo* info; 		//指向结构数组的指针，描述字体
	unsigned char* data;		//指向结构数组的指针，描述符号
} Font;


int main(int argc, char* argv[])
{
  const char* fn = "../Fonts/8514fix.fon";
    FILE* fp;
    Font font;
    int last;
    int size;
    int i;

    // Read file
    fp = fopen(fn, "rb");

    if (!fp) {
        printf("Error: Unable open %s\n", argv[1]);
        return -1;
    }


    if (fread(&font, sizeof(font), 1, fp) != 1) {
        printf("Error: Unable read FON-file header\n");
        fclose(fp);
        return -1;
    }

    font.info = malloc(font.num * sizeof(FontInfo));

    if (font.info == NULL) {
        printf("Error: Unable allocate memory for glyphs info\n");
        fclose(fp);
        return -1;
    }

    if (fread(font.info, sizeof(FontInfo), font.num, fp) != font.num) {
        printf("Error: Unable read info about glyphs\n");
        free(font.info);
        fclose(fp);
        return -1;
    }

    last = font.num - 1;
    size = font.info[last].offset + (font.info[last].width + 7) / 8 * font.height;

    font.data = malloc(size);

    if (font.data == NULL) {
        printf("Error: Unable allocate memory for glyphs\n");
        free(font.info);
        fclose(fp);
        return -1;
    }

    if (fread(font.data, 1, size, fp) != size) {
        printf("Error: Unable read glyphs\n");
        free(font.data);
        free(font.info);
        fclose(fp);
        return -1;
    }

    fclose(fp);

    // Font info
    printf("Number of glyphs: %d\n", font.num);
    printf("Height: %d\n", font.height);
    printf("Spacing: %d\n", font.spacing);
    printf("\n");

    // Glyphs
    for(i = 0; i < font.num; i++) {
        printf("Glyph %d (0x%02X)\n", i, i);
        printf("================\n");

        printf("Width: %d\n", font.info[i].width);
        printf("Offset: 0x%08X (%d)\n", font.info[i].offset, font.info[i].offset);
        printf("Offset from begin of file: 0x%08X (%d)\n",
               sizeof(font) + sizeof(FontInfo) * font.num + font.info[i].offset,
               sizeof(font) + sizeof(FontInfo) * font.num + font.info[i].offset);
        printf("\n");

        if (font.height * font.info[i].width != 0) {
            int offset = font.info[i].offset;
            int bytesPerLine = (font.info[i].width + 7) / 8;
            int j;
            int h;

            for(h = 0; h < font.height; h++) {
                for(j = 0; j < font.info[i].width; j++) {
                    if (font.data[offset + h * bytesPerLine + (j / 8)] & (1 << (7 - (j % 8)))) {
                        printf("#");
                    }
                    else {
                        printf(".");
                    }
                }

                printf("\n");
            }

            printf("\n");
            printf("\n");
        }
        else {
            printf("Empty\n");
            printf("\n");
            printf("\n");
        }
    }

    // Free font data
    free(font.data);
    free(font.info);

    return 0;
}


