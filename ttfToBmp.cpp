#include <ft2build.h>
#include <freetype/freetype.h>  
#include <freetype/ftglyph.h> 
#include <windows.h>
#include <wingdi.h>
#include <iostream>

class FT2Bmp
{
private:
	FT_Library  ftLib; 
	FT_Face     ftFace;
	int m_fontWidth;
	int m_fontHeight;
	int m_outWidth;
	int m_outHeight;
	unsigned char *buf;
public:
	FT2Bmp(const char *szFontPath)
	:m_fontWidth(48)
	,m_fontHeight(48)
	,ftLib(NULL)
	,ftFace(NULL)
	{
		FT_Error ftErr;
		ftErr = FT_Init_FreeType(&ftLib);
		if (ftErr)  
		{  
			ftLib = NULL;
			printf( "[FreeType] FT_Init_FreeType Failed\n");
			return;
		}
		else
		{
			printf( "[FreeType] FT_Init_FreeType Successed\n");
		}
	
		ftErr = FT_New_Face(ftLib,szFontPath,0,&ftFace);
		if (ftErr != 0) 
		{
			ftFace = NULL;
			printf("[FreeType] FT_New_Face Failed\n");
			return;
		} 
		else
		{
			printf("[FreeType] FT_New_Face Successed\n");
			printf("[FreeType] ftFace Face Flags = 0x%x\n",ftFace->face_flags);
			if(ftFace->face_flags & FT_FACE_FLAG_SCALABLE)	        printf("--FT_FACE_FLAG_SCALABLE\n");
			if(ftFace->face_flags & FT_FACE_FLAG_FIXED_SIZES)	    printf("--FT_FACE_FLAG_FIXED_SIZES\n");
			if(ftFace->face_flags & FT_FACE_FLAG_FIXED_WIDTH)	    printf("--FT_FACE_FLAG_FIXED_WIDTH\n");
			if(ftFace->face_flags & FT_FACE_FLAG_SFNT)	            printf("--FT_FACE_FLAG_SFNT\n");
			if(ftFace->face_flags & FT_FACE_FLAG_HORIZONTAL)	    printf("--FT_FACE_FLAG_HORIZONTAL\n");
			if(ftFace->face_flags & FT_FACE_FLAG_KERNING)	        printf("--FT_FACE_FLAG_KERNING\n");
			if(ftFace->face_flags & FT_FACE_FLAG_FAST_GLYPHS)	    printf("--FT_FACE_FLAG_FAST_GLYPHS\n");
			if(ftFace->face_flags & FT_FACE_FLAG_MULTIPLE_MASTERS)	printf("--FT_FACE_FLAG_MULTIPLE_MASTERS\n");
			if(ftFace->face_flags & FT_FACE_FLAG_GLYPH_NAMES)	    printf("--FT_FACE_FLAG_GLYPH_NAMES\n");
			if(ftFace->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM)	printf("--FT_FACE_FLAG_EXTERNAL_STREAM\n");
			if(ftFace->face_flags & FT_FACE_FLAG_HINTER)	        printf("--FT_FACE_FLAG_HINTER\n");
			if(ftFace->face_flags & FT_FACE_FLAG_CID_KEYED)	        printf("--FT_FACE_FLAG_CID_KEYED\n");
			if(ftFace->face_flags & FT_FACE_FLAG_TRICKY)	        printf("--FT_FACE_FLAG_TRICKY\n");
			if(ftFace->face_flags & FT_FACE_FLAG_COLOR)	            printf("--FT_FACE_FLAG_COLOR\n");
			if(ftFace->face_flags & FT_FACE_FLAG_VARIATION)	        printf("--FT_FACE_FLAG_VARIATION\n");
		}
	}
	~FT2Bmp()
	{
		if(ftFace)
		{
			FT_Done_Face(ftFace);
			ftFace = NULL;
		}
		if(ftLib)
		{
			FT_Done_FreeType(ftLib);
			ftLib = NULL;
		}
	}
	void SetFontPixelSize(int width,int height)
	{
		m_fontWidth = width;
		m_fontHeight = height;
	}
	
	void SaveBmp(const char *szFilePath,int width, int height, unsigned char *data)
	{
		BITMAPFILEHEADER bmpheader;  
		BITMAPINFOHEADER bmpinfo;
		int lineByte=(width * 32/8+3)/4*4;

		bmpheader.bfType = 0x4d42;  
		bmpheader.bfReserved1 = 0;  
		bmpheader.bfReserved2 = 0;  
		bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
		bmpheader.bfSize = bmpheader.bfOffBits + lineByte*height;

		bmpinfo.biSize = sizeof(BITMAPINFOHEADER);  
		bmpinfo.biWidth = width;  
		bmpinfo.biHeight = -height;  
		bmpinfo.biPlanes = 1;  
		bmpinfo.biBitCount = 32;  
		bmpinfo.biCompression = BI_RGB;  
		bmpinfo.biSizeImage = lineByte*height;  
		bmpinfo.biXPelsPerMeter = 100;  
		bmpinfo.biYPelsPerMeter = 100;  
		bmpinfo.biClrUsed = 0;  
		bmpinfo.biClrImportant = 0;

		FILE* fp;
		if( (fp = fopen(szFilePath,"wb+")) == NULL ) 
		{  
			printf ("Open BMP File %s Failed!\n",szFilePath);  
			return;  
		}
		fwrite(&bmpheader, 1, sizeof(bmpheader), fp);  
		fwrite(&bmpinfo, 1, sizeof(bmpinfo), fp);
		fwrite(data, 1, height*width*32/8, fp);  
		fclose(fp); 
	}

	void Pix2DCopy(int srcWidth,int srcHeight,unsigned char *srcData, int dstPosX, int dstPoxY, int dstWidth, unsigned char * dtsData)
	{
		int i,j;
		for(i=0;i<srcHeight;i++)
		{
			for(j=0;j<srcWidth;j++)
			{
				dtsData[((dstPosX+j)+(dstPoxY+i)*dstWidth)*4+0]=255-srcData[j+i*srcWidth];
				dtsData[((dstPosX+j)+(dstPoxY+i)*dstWidth)*4+1]=255-srcData[j+i*srcWidth];
				dtsData[((dstPosX+j)+(dstPoxY+i)*dstWidth)*4+2]=255-srcData[j+i*srcWidth];
				dtsData[((dstPosX+j)+(dstPoxY+i)*dstWidth)*4+3];// !!
			}
		}
	}

	void RenderTest(const wchar_t* str,int &topToOrigin,int &downToOrigin,int &totalAdvance)
	{
		FT_Error ftErr;
		topToOrigin = 0;
		downToOrigin = 0;
		totalAdvance = 0;
		if(!ftFace)
		{
			return;
		}
		if(FT_Set_Char_Size(ftFace,m_fontWidth*64,m_fontHeight*64,0,0)!=0)
		{
			printf("[FreeType] FT_Set_Char_Size Failed\n");
			return;
		}

		int i;
		int len = wcslen(str);
		for(i=0;i<len;i++)
		{
			FT_UInt index = FT_Get_Char_Index(ftFace,str[i]);
			if(index==0)
			{
				printf("[FreeType] Undefined Character Code: 0x%x\n", str[i]);
			}
			ftErr=FT_Load_Glyph(ftFace,index,FT_LOAD_DEFAULT);
			if(ftErr!=0)
			{
				printf("[FreeType] FT_Load_Glyph Failed\n");
				return;
			}
			else
			{
				printf("[FreeType] FT_Load_Glyph Successed\n");
			}
			if (ftFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)  
			{  
				FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);  
			}
			
			int up = ftFace->glyph->metrics.horiBearingY>>6;
			int down = (ftFace->glyph->metrics.height>>6) - up;
			
			if(up>topToOrigin)
			{
				topToOrigin = up;
			}
			if(down>downToOrigin)
			{
				downToOrigin = down;
			}
			
			if(i<len-1)
			{
				totalAdvance += ftFace->glyph->metrics.horiAdvance>>6;
			}
			else
			{
				int horiAdvance = ftFace->glyph->metrics.horiAdvance>>6;
				int horiBearingX = ftFace->glyph->metrics.horiBearingX>>6;
				int width = ftFace->glyph->metrics.width>>6;
				if(horiBearingX + width > horiAdvance)
				{
					totalAdvance+=horiBearingX+width+horiBearingX;
				}
				else
				{
					totalAdvance+=horiAdvance;
				}
			}
		}
	}

	void RenderOneCharacter(FT_ULong charcode,unsigned char *&picdata, int &Bearing_x,int &Bearing_y,int &width, int &height, int &Advance_x)
	{
		FT_Error ftErr;
		if(!ftFace)
		{
			return;
		}
		if(FT_Set_Char_Size(ftFace,m_fontWidth*64,m_fontHeight*64,0,0)!=0)
		{
			printf("[FreeType] FT_Set_Char_Size Failed\n");
			return;
		}
		FT_UInt index = FT_Get_Char_Index(ftFace,charcode);
		if(index==0)
		{
			printf("[FreeType] Undefined Character Code: 0x%x\n", charcode);
			charcode = ' ';
			index = FT_Get_Char_Index(ftFace,charcode);
		}
		ftErr=FT_Load_Glyph(ftFace,index,FT_LOAD_DEFAULT);
		if(ftErr!=0)
		{
			printf("[FreeType] FT_Load_Glyph Failed\n");
			return;
		}

	    if (ftFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)  
		{  
			FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);  
		}  

		Bearing_x = ftFace->glyph->metrics.horiBearingX>>6;
		Bearing_y = ftFace->glyph->metrics.horiBearingY>>6;
		width     = ftFace->glyph->metrics.width>>6;
		height    = ftFace->glyph->metrics.height>>6;
		Advance_x = ftFace->glyph->metrics.horiAdvance>>6; 
		picdata   = &ftFace->glyph->bitmap.buffer[0];

	}
};

void DoStr2Bmp(const wchar_t *str,
			   const char *ttfFilePath, 
			   int fontWidthPix,
			   int fontHeightPix,
			   const char *bmpFilePath)
{
	int strLength = wcslen(str);
	unsigned char *BMPDATA;
	FT2Bmp *Ft2Bmp;

	Ft2Bmp = new FT2Bmp(ttfFilePath);
	Ft2Bmp->SetFontPixelSize(fontWidthPix,fontHeightPix);

	int topToOrigin = 0;
	int downToOrigin = 0;
	int totalAdvance = 0;
	Ft2Bmp->RenderTest(str,topToOrigin,downToOrigin,totalAdvance);

	printf("BMP Size: width = %d, height = %d\n", totalAdvance,topToOrigin+downToOrigin );
	BMPDATA = (unsigned char *)new unsigned int[(topToOrigin+downToOrigin)*totalAdvance];
	memset(BMPDATA,0xff,(topToOrigin+downToOrigin)*totalAdvance*4);
	
	int i;
	int offset_x=0;
	int offset_y=0;
	for(i=0;i<strLength;i++)
	{
		unsigned char *fontpic;
		int Bearing_x,Bearing_y,width,height,advance_x;
		Ft2Bmp->RenderOneCharacter(str[i],fontpic,Bearing_x,Bearing_y,width,height,advance_x);
		Ft2Bmp->Pix2DCopy(width,height,fontpic,offset_x+Bearing_x,topToOrigin-Bearing_y,totalAdvance,BMPDATA);
		offset_x+=advance_x;
	}
	

	Ft2Bmp->SaveBmp(bmpFilePath,totalAdvance,(topToOrigin+downToOrigin),BMPDATA);

	delete(BMPDATA);
	delete(Ft2Bmp);
}

int main(int argc, char* argv[])
{
	DoStr2Bmp(L"落霞与孤鹜齐飞，秋水共长天一色。","./STLITI.ttf",64,80,"./out1.bmp");
	printf("Finished!\n");
	DoStr2Bmp(L"π = 3.14159265358979323846264338327950288419716939937510...","./STLITI.ttf",32,32,"./out2.bmp");
	printf("Finished!\n");
	printf("Press ANY Key To Exit.\n");
	getchar();
	return 0;
}