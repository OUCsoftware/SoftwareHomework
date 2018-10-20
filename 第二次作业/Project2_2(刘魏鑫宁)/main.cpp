#include <iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	//输入图像
	GDALDataset* poSrcDs;
	//输出图像
	GDALDataset* poDstDs;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//截取图像的宽度和高度
	int tmpXlen, tmpYlen;
	//开始的坐标
	int startX, startY;
	//输入图像路径
	const char* srcPath = "wind.jpg";
	//输出图像路径
	const char* dstPath = "res.tif";
	//图像内存
	GByte* buffTmp;
	//图像波段数
	int i, j, k, BandNum;
	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像的宽度,高度,波段
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	BandNum = poSrcDs->GetRasterCount();

	//分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	//创建输出图像
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgXlen, imgYlen, BandNum, GDT_Byte, NULL);

	//输入图像
	for (i = 0; i < BandNum; i++)
	{
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDs->GetRasterBand(i + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//清除内存
	CPLFree(buffTmp);

	//变成白色
	startX = 300, startY = 300;
	tmpXlen = 100, tmpYlen = 50;

	//分配内存
	buffTmp = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));

	//读取RBG通道
	for (k = 1; k <= 3; k++)
	{
		poSrcDs->GetRasterBand(k)->RasterIO(GF_Read, startX, startY,
			tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
		for (j = 0; j < tmpYlen; j++)
		{
			for (i = 0; i < tmpXlen; i++)
			{
				buffTmp[j*tmpXlen + i] = (GByte)255;
			}
		}
		//数据写入poDstDs
		poDstDs->GetRasterBand(k)->RasterIO(GF_Write,
			startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	}
	//清除内存
	CPLFree(buffTmp);

	//黑色的
	startX = 500, startY = 500;
	tmpXlen = 50, tmpYlen = 100;

	//分配内存
	buffTmp = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));

	//读取RBG通道
	for (k = 1; k <= 3; k++)
	{
		poSrcDs->GetRasterBand(k)->RasterIO(GF_Read, startX, startY,
			tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
		for (j = 0; j < tmpYlen; j++)
		{
			for (i = 0; i < tmpXlen; i++)
			{
				buffTmp[j*tmpXlen + i] = (GByte)0;
			}
		}
		//数据写入poDstDs
		poDstDs->GetRasterBand(k)->RasterIO(GF_Write,
			startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	}
	//清除内存
	CPLFree(buffTmp);

	//关闭dataset
	GDALClose(poDstDs);
	GDALClose(poSrcDs);

	system("PAUSE");



	return 0;
}