#include <font_manager.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Face g_tface;
static int g_iFontSize = 12;
static int FreetypeGetStringRegionCar(char *str, pRegionCartesian ptRegionCartesian)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = g_tface->glyph;

    /* 初始化 */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* 指定原点为(0, 0) */
    pen.x = 0;
    pen.y = 0;

    /* 计算每个字符的bounding box */
    /* 先translate, 再load char, 就可以得到它的外框了 */
    for (i = 0; i < strlen(str); i++)
    {
        /* 转换：transformation */
        FT_Set_Transform(g_tface, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(g_tface, str[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(g_tface->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }
        
        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* 更新外框 */
        if ( glyph_bbox.xMin < bbox.xMin )
            bbox.xMin = glyph_bbox.xMin;

        if ( glyph_bbox.yMin < bbox.yMin )
            bbox.yMin = glyph_bbox.yMin;

        if ( glyph_bbox.xMax > bbox.xMax )
            bbox.xMax = glyph_bbox.xMax;

        if ( glyph_bbox.yMax > bbox.yMax )
            bbox.yMax = glyph_bbox.yMax;
        
        /* 计算下一个字符的原点: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    /* return string bbox */
    ptRegionCartesian->iLeftUpx = bbox.xMin;
    ptRegionCartesian->iLeftUpy = bbox.yMax;
    ptRegionCartesian->iWidth = bbox.xMax - bbox.xMin;
    ptRegionCartesian->iHeight = bbox.yMax - bbox.yMin;

    return 0;
}
static int FreeTypeFontInit(char *aFileName){
    FT_Library library;
    int error;
    error = FT_Init_FreeType( &library );              /* initialize library */
    if(error){
        printf("Error: Could not initialize FreeType library\n");
    }
    error = FT_New_Face( library, aFileName, 0, &g_tface ); /* create face object */
    if(error){
        printf("Error: Could not open font file\n");
    }
    FT_Set_Pixel_Sizes( g_tface, g_iFontSize, 0 );        
    return 0;   /* set font size */
}
static int FreeTypeSetFontSize(int iFontSize){
    FT_Set_Pixel_Sizes( g_tface, iFontSize, 0 );
    return 0;
}
static int FreeTypeGetFontBitMap(unsigned int dwCode, pFontBitMap pBitMap){
    FT_Vector pen;
 //   FT_Glyph  glyph;
    FT_GlyphSlot slot = g_tface->glyph;

    pen.x = pBitMap->iCurOriginX*64;
    pen.y = pBitMap->iCurOriginY*64;
    int error;
    FT_Set_Transform( g_tface, NULL, &pen );
    error = FT_Load_Char( g_tface, dwCode, FT_LOAD_RENDER );
    if(error){
        printf("Error: Could not load character\n");
        return -1;
    }
    pBitMap->pucBuffer = slot->bitmap.buffer;

    pBitMap->tregion.iLeftUpx=slot->bitmap_left;
	pBitMap->tregion.iLeftUpy=pBitMap->iCurOriginY*2 - slot->bitmap_top;
	pBitMap->tregion.iWidth = slot->bitmap.width;
	pBitMap->tregion.iHeight = slot->bitmap.rows;
	pBitMap->iNextOriginX = pBitMap->iCurOriginX + slot->advance.x/64;
	pBitMap->iNextOriginY = pBitMap->iCurOriginY;

    return 0;
}
static FontOpr g_tFreetypeOpr ={
    .name = "freetype",
    .FontInit = FreeTypeFontInit,
    .SetFontSize = FreeTypeSetFontSize,
    .GetFontBitMap = FreeTypeGetFontBitMap,
    .GetStringRegionCar = FreetypeGetStringRegionCar
};
void FreetypeRegister(){
    RegisterFont(&g_tFreetypeOpr);
}
