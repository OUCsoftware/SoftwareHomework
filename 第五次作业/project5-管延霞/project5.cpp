#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc1, *posrc2;
	//输出图像
	GDALDataset* podes1;
	
	int imgXlen, imgYlen,bandNum;
	//图像的路径
	int i, j;
	//输出图像的路径,只能保存为tif类型的
	char *despath = "1.tif";
    
	//图像内存存储
	float *bufftmp[3],*bufftmp2,*bufftmpH, *bufftmpI, *bufftmpS;
	//图像波段数
	//定义动态的二维数组

	//注册驱动
	GDALAllRegister();
	//打开图像
	char *srcpath1 = "American_MUL.bmp";
	char *srcpath2 = "American_PAN.bmp";
	posrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	posrc2 = (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);
	//获取图像的宽度，高度和波段数
	imgXlen = posrc1->GetRasterXSize();
	imgYlen = posrc1->GetRasterYSize();
	bandNum = posrc1->GetRasterCount();

	//创建输出图像；
	podes1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (i = 0; i <bandNum; i++)
	{
		bufftmp[i] = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	}
	bufftmpH = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	bufftmpI = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	bufftmpS = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	bufftmp2 = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	for ( i = 0; i < bandNum; i++)
	{
		posrc1->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmp[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	posrc2->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmp2, imgXlen, imgYlen, GDT_Float32, 0, 0);
	for ( i = 0; i < imgXlen*imgYlen; i++)
	{
		bufftmpH[i] = -sqrt(2.0f) / 6.0f* bufftmp[0][i] - sqrt(2.0f) / 6.0f*bufftmp[1][i] + sqrt(2.0f) / 3.0f*bufftmp[2][i];
		bufftmpS[i] = 1.0f / sqrt(2.0f)* bufftmp[0][i] - 1 / sqrt(2.0f)*bufftmp[1][i];

		bufftmp[0][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] + 1.0f / sqrt(2.0f)*bufftmpS[i];
		bufftmp[1][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] - 1.0f / sqrt(2.0f)*bufftmpS[i];
		bufftmp[2][i] = bufftmp2[i] + sqrt(2.0f)*bufftmpH[i];

	}

	for (i = 0; i < 3; i++)
	{
		podes1->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmp[i], imgXlen, imgYlen, GDT_Float32, 0, 0);
	}


	//清理内存

	for( j=0;j<3;j++)
		CPLFree(bufftmp[j]);
	CPLFree(bufftmp2);
	CPLFree(bufftmpH);
	CPLFree(bufftmpI);
	CPLFree(bufftmpS);

	//关闭dataset
	GDALClose(posrc1);
	GDALClose(podes1);
	GDALClose(posrc2);
	system("PAUSE");
	return 0;
}
