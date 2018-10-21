// homework2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;
	int imgXlen, imgYlen;
	int startX, startY;             //起始位置坐标
	int tmpXlen, tmpYlen;           //区域宽高
	GByte* buffTmp;					//图像内存存储
	GByte* tmp;                     //区域图像内存存储
	int i, j, bandNum;
	char* srcPath = "square.jpg";	//输入图像路径
	char* dstPath = "a.tif";		//输出图像路径
	
	startX = 100;
	startY = 100;
	tmpXlen = 200;
	tmpYlen = 150;
	GDALAllRegister();
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte)); //分配内存
	tmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i+1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, tmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	for (j = 0; j < tmpYlen; j++)
	{
		for (i = 0; i < tmpXlen; i++)
		{
			tmp[j*tmpXlen + i] = (GByte)255;
		}
	}
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, tmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	CPLFree(tmp);
	CPLFree(buffTmp);
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	int k;
	int startX1, startY1, startX2, startY2;
	int tmpXlen1, tmpYlen1, tmpXlen2, tmpYlen2;
	GByte* tmp1;
	GByte* tmp2;
	char* dstPath2 = "b.tif";

	startX1 = 300;
	startY1 = 300;
	tmpXlen1 = 100;
	tmpYlen1 = 50;

	startX2 = 500;
	startY2 = 500;
	tmpXlen2 = 50;
	tmpYlen2 = 100;

	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte)); //分配内存
	tmp1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	tmp2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath2, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read, startX1, startY1, tmpXlen1, tmpYlen1, tmp1, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
		for (j = 0; j < tmpYlen1; j++)
		{
			for (i = 0; i < tmpXlen1; i++)
			{
				tmp1[j*tmpXlen1 + i] = (GByte)255;
			}
		}
		poDstDS->GetRasterBand(k + 1)->RasterIO(GF_Write, startX1, startY1, tmpXlen1, tmpYlen1, tmp1, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
	}

	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read, startX2, startY2, tmpXlen2, tmpYlen2, tmp2, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);
		for (j = 0; j < tmpYlen2; j++)
		{
			for (i = 0; i < tmpXlen2; i++)
			{
				tmp2[j*tmpXlen2 + i] = (GByte)0;
			}
		}
		poDstDS->GetRasterBand(k + 1)->RasterIO(GF_Write, startX2, startY2, tmpXlen2, tmpYlen2, tmp2, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);
	}
	
	CPLFree(tmp2);
	CPLFree(tmp1);
	CPLFree(buffTmp);
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
	return 0;
}

