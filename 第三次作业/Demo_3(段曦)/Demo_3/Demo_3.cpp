
#include "pch.h"
#include <iostream>
#include "../gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

using namespace std;
int main()

{
	//注册驱动
	GDALAllRegister();

	GDALDataset* pic1;
	GDALDataset* pic2;
	GDALDataset* poDstDS;

	const char *path1 = "superman.jpg";
	const char *path2 = "space.jpg";
	const char *despath = "aim.tif";

	//波段空间
	GByte *bufftmpR, *bufftmpG, *bufftmpB;
	GByte *bufftmpR1, *bufftmpG1, *bufftmpB1;
	GByte *bufftmpR2, *bufftmpG2, *bufftmpB2;

	//参数
	int i, j, bandNum;

	//打开
	pic1 = (GDALDataset*)GDALOpenShared(path1, GA_ReadOnly);
	pic2 = (GDALDataset*)GDALOpenShared(path2, GA_ReadOnly);

	//获取参数,均为640*480不用特殊处理
	int imgXlen, imgYlen;
	imgXlen = pic1->GetRasterXSize();
	imgYlen = pic1->GetRasterYSize();
	bandNum = pic1->GetRasterCount();
	
	//创建输出图像；
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);



	//波段分配内存
	bufftmpR1 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpR2 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpG1 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpG2 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpB1 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpB2 = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpR = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpG = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	bufftmpB = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));

	//superman读入
	pic1->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpR1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	pic1->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpG1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	pic1->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpB1, imgXlen, imgYlen, GDT_Byte, 0, 0);

	//space读入
	pic2->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpR2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	pic2->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpG2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	pic2->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpB2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	
	for (i = 0; i < imgYlen; i++)
		for (j = 0; j < imgXlen; j++)
		{
			//条件1：10<r<160; 条件2：100<g<220; 条件3：10<b<110；同时满足这三个条件的像素为超人区域像素
			if (!(bufftmpR1[i*imgXlen + j] <(GByte)160 && bufftmpR1[i*imgXlen + j] > (GByte)10 && bufftmpG1[i*imgXlen + j] < (GByte)220 && bufftmpG1[i*imgXlen + j] > (GByte)100 && bufftmpB1[i*imgXlen + j] <(GByte)110 && bufftmpB1[i*imgXlen + j] > (GByte)10))
			{
				bufftmpR[i*imgXlen + j] = bufftmpR1[i*imgXlen + j];
				bufftmpG[i*imgXlen + j] = bufftmpG1[i*imgXlen + j];
				bufftmpB[i*imgXlen + j] = bufftmpB1[i*imgXlen + j];
			}
			//填充宇宙
			else
			{
				bufftmpR[i*imgXlen + j] = bufftmpR2[i*imgXlen + j];
				bufftmpG[i*imgXlen + j] = bufftmpG2[i*imgXlen + j];
				bufftmpB[i*imgXlen + j] = bufftmpB2[i*imgXlen + j];
			}
		}

	//写入图像
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpB, imgXlen, imgYlen, GDT_Byte, 0, 0);

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
	GDALClose(pic1);
	GDALClose(pic2);
	GDALClose(poDstDS);
	system("PAUSE");
	return 0;
}