// project2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc1;
	GDALDataset* posrc2 ;
	//输出图像
	GDALDataset* podes;

	//图像的宽度和高度
	int imgXlen, imgYlen;
	//图像的路径
	char *srcpath1 = "superman.jpg";
	//输出图像的路径,只能保存为tif类型的
	char *srcpath2 = "space.jpg";
	char *despath = "perfect.tif";
	//图像内存存储
	GByte *bufftmpR,*bufftmpG,*bufftmpB;
	GByte *bufftmpR1, *bufftmpG1, *bufftmpB1;
	GByte *bufftmpR2, *bufftmpG2, *bufftmpB2;
	//图像波段数
	int i, j, bandNum;

	//注册驱动
	GDALAllRegister();
	//打开图像
	posrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	posrc2= (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);
	//获取图像的宽度，高度和波段数
	imgXlen= posrc1->GetRasterXSize();
	imgYlen = posrc1->GetRasterYSize();
	bandNum = posrc1->GetRasterCount();
	//创建输出图像；
	podes = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//根据图像大小进行分配空间
	bufftmpR1= (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpR2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpG1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpG2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpB1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpB2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpR = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpG = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));	
	bufftmpB = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//超人ＲＧＢ通道像素的读入
	posrc1->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpR1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc1->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpG1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc1->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpB1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	//太空RGB通道像素的读入
	posrc2->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpR2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc2->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpG2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc2->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpB2, imgXlen, imgYlen, GDT_Byte, 0, 0);

	for (i = 0; i < imgYlen; i++)
		for (j = 0; j < imgXlen; j++)
		{
			if (!(bufftmpR1[i*imgXlen + j] <(GByte)160 && bufftmpR1[i*imgXlen + j] > (GByte)10 && bufftmpG1[i*imgXlen + j] < (GByte)220 && bufftmpG1[i*imgXlen + j] > (GByte)100  && bufftmpB1[i*imgXlen + j] <(GByte)110 && bufftmpB1[i*imgXlen + j] > (GByte)10))
			{
				bufftmpR[i*imgXlen + j] = bufftmpR1[i*imgXlen + j];
				bufftmpG[i*imgXlen + j] = bufftmpG1[i*imgXlen + j];
				bufftmpB[i*imgXlen + j] = bufftmpB1[i*imgXlen + j];
			}
			else
			{

				bufftmpR[i*imgXlen + j] = bufftmpR2[i*imgXlen + j];
				bufftmpG[i*imgXlen + j] = bufftmpG2[i*imgXlen + j];
				bufftmpB[i*imgXlen + j] = bufftmpB2[i*imgXlen + j];
			}
		}
			

	//写入图像
	podes->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	podes->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	podes->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpB, imgXlen, imgYlen, GDT_Byte, 0, 0);
    //清理内存
	CPLFree(bufftmpR);
	CPLFree(bufftmpG);
	CPLFree(bufftmpB);
	CPLFree(bufftmpG1);
	CPLFree(bufftmpR1);
	CPLFree(bufftmpB1);
	CPLFree(bufftmpR2);
	CPLFree(bufftmpG2);	
	CPLFree(bufftmpB2);
	//关闭dataset
	GDALClose(posrc1);
	GDALClose(posrc2);
	GDALClose(podes);

	system("PAUSE");
	return 0;
}

