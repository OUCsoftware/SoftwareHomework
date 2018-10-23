#include <iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	//输入图像,太空
	GDALDataset* poSrcSpaceDs;
	//输入图像,超人
	GDALDataset* poSrcSuperDs;
	//输出图像
	GDALDataset* poDstDs;
	//起始位置的坐标
	int startX = 0, startY = 0;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//内存 
	GByte* buffTmp;
	//SUPER内存
	GByte* buffTmpSpaceR;
	GByte* buffTmpSpaceB;
	GByte* buffTmpSpaceG;
	//space内存
	GByte* buffTmpSuperR;
	GByte* buffTmpSuperG;
	GByte* buffTmpSuperB;
	//输入图像space的路径
	const char* srcSpacePath = "space.jpg";
	//输入图像superman的路径
	const char* srcSuperPath = "superman.jpg";
	//参数
	const char* dstPath = "ok.tif";
	int i, j, bandNum;

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcSpaceDs = (GDALDataset*)GDALOpenShared(srcSpacePath, GA_ReadOnly);
	poSrcSuperDs = (GDALDataset*)GDALOpenShared(srcSuperPath, GA_ReadOnly);
	
	/*
	先创建一个只有superman的图像
	*/
	
	//获取super图像的宽度,高度和波段数量
	imgXlen = poSrcSuperDs->GetRasterXSize();
	imgYlen = poSrcSuperDs->GetRasterYSize();
	bandNum = poSrcSuperDs->GetRasterCount();

	//创建输出图像
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//开始分配内存,超人的
	buffTmpSuperR = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmpSuperG = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmpSuperB = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	//分配space内存
	buffTmpSpaceR = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmpSpaceG = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmpSpaceB = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	//读取背景在buffTmpSpace里
	poSrcSpaceDs->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen,
		buffTmpSpaceR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcSpaceDs->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen,
		buffTmpSpaceG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcSpaceDs->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen,
		buffTmpSpaceB, imgXlen, imgYlen, GDT_Byte, 0, 0);

	//读取背景在buffTmpSuper
	poSrcSuperDs->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen,
		buffTmpSuperR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcSuperDs->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen,
		buffTmpSuperG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcSuperDs->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen,
		buffTmpSuperB, imgXlen, imgYlen, GDT_Byte, 0, 0);



	//遍历
	for (j = 0; j < imgYlen; j++)
	{
		for (i = 0; i < imgXlen; i++)
		{
			if ( !(buffTmpSuperR[j*imgXlen+i] > (GByte)10 && buffTmpSuperR[j*imgXlen+i] < (GByte)160 && buffTmpSuperG[j*imgXlen+i]>(GByte)100 && buffTmpSuperG[j*imgXlen
			+i] < (GByte)220 && buffTmpSuperB[j*imgXlen+i]>(GByte)10 && buffTmpSuperB[j*imgXlen+i] < (GByte)110))
			{
				buffTmpSpaceR[j*imgXlen + i] = buffTmpSuperR[j*imgXlen + i];
				buffTmpSpaceG[j*imgXlen + i] = buffTmpSuperG[j*imgXlen + i];
				buffTmpSpaceB[j*imgXlen + i] = buffTmpSuperB[j*imgXlen + i];
			}
		}
	}
	//开始填充
	poDstDs->GetRasterBand(1)->RasterIO(GF_Write,
		0, 0, imgXlen, imgYlen, buffTmpSpaceR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDs->GetRasterBand(2)->RasterIO(GF_Write,
		0, 0, imgXlen, imgYlen, buffTmpSpaceG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDs->GetRasterBand(3)->RasterIO(GF_Write,
		0, 0, imgXlen, imgYlen, buffTmpSpaceB, imgXlen, imgYlen, GDT_Byte, 0, 0);

	//清除内存
	CPLFree(buffTmpSpaceR);
	CPLFree(buffTmpSpaceG);
	CPLFree(buffTmpSpaceB);
	CPLFree(buffTmpSuperR);
	CPLFree(buffTmpSuperG);
	CPLFree(buffTmpSuperB);
	//关闭datatset
	GDALClose(poDstDs);
	GDALClose(poSrcSpaceDs);
	GDALClose(poSrcSuperDs);

	system("pause");
	return 0;

	

	return 0;
}