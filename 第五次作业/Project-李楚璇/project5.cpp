#include "stdafx.h"
#include<iostream>
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* poSrc1, *poSrc2;

	GDALDataset* poDst;

	int imgXlen, imgYlen, bandNum;
	int i, j;
	const char *despath = "1.tif";

	float *buff[3], *buffT, *buffH, *buffI, *buffS;
	GDALAllRegister();
	const char *srcpath1 = "American_MUL.bmp";
	const char *srcpath2 = "American_PAN.bmp";
	poSrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	poSrc2 = (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);

	imgXlen = poSrc1->GetRasterXSize();
	imgYlen = poSrc1->GetRasterXSize();
	bandNum = poSrc1->GetRasterCount();

	poDst = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (i = 0; i < bandNum; i++)
	{
		buff[i] = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	}
	buffH = (float *)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffI = (float *)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffS = (float *)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffT = (float *)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	for (i = 0; i < bandNum; i++)
	{
		poSrc1->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buff[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	poSrc2->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffT, imgXlen, imgYlen, GDT_Float32, 0, 0);
	for (i = 0; i < imgXlen*imgYlen; i++)
	{
		//RGB=>IHS
		buffH[i] = -sqrt(2.0f) / 6.0f* buff[0][i] - sqrt(2.0f) / 6.0f*buff[1][i] + sqrt(2.0f) / 3.0f*buff[2][i];
		buffS[i] = 1.0f / sqrt(2.0f)* buff[0][i] - 1 / sqrt(2.0f)*buff[1][i];
		//IHS=>RGB
		buff[0][i] = buffT[i] - 1.0f / sqrt(2.0f)*buffH[i] + 1.0f / sqrt(2.0f)*buffS[i];
		buff[1][i] = buffT[i] - 1.0f / sqrt(2.0f)*buffH[i] - 1.0f / sqrt(2.0f)*buffS[i];
		buff[2][i] = buffT[i] + sqrt(2.0f)*buffH[i];
	}
	for (i = 0; i < 3; i++)
	{
		poDst->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buff[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	//清理内存

	for (j = 0; j < 3; j++)
		CPLFree(buff[j]);
	CPLFree(buffT);
	CPLFree(buffH);
	CPLFree(buffI);
	CPLFree(buffS);

	//关闭dataset
	GDALClose(poSrc1);
	GDALClose(poDst);
	GDALClose(poSrc2);
	system("PAUSE");
	return 0;
}

