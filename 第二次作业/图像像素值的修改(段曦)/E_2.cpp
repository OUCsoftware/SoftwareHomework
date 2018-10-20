#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//输入图像
	GDALDataset* poSrcDS;

	//输出图像
	GDALDataset* poDstDS;

	//起始位置的坐标
	int StartX = 100, StartY = 100;

	//截取图像的宽度和高度
	int tmpXlen = 200, tmpYlen = 150;

	//图像的宽度和高度
	int imgXlen, imgYlen;

	//内存
	GByte* buffTmp;

	//输入图像的路径
	const char* srcPath = "qingdao.jpg";

	//输出图像的路径
	const char* dstPath = "qingdao_1.tif";

	//参数
	int i, j, k, bandNum;

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像宽度,高度和波段数量
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//一个个波段的输入,然后一个个波段的输出
	for (i = 0; i < bandNum; i++)
	{
		//写入缓冲区
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//读取红色通道在buffTmp中
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	//遍历区域,逐像素置为255
	for (j = 0; j < tmpYlen; j++)
	{
		for (i = 0; i < tmpXlen; i++)
		{
			buffTmp[j*tmpXlen + i] = (GByte)255;
		}
	}
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	for (k = 1; k < 4; k++)
	{
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 300, 300, 100, 50, buffTmp, 100, 50, GDT_Byte, 0, 0);

		//遍历区域,逐像素置为0
		for (j = 0; j < tmpYlen; j++)
		{
			for (i = 0; i < tmpXlen; i++)
			{
				buffTmp[j*tmpXlen + i] = (GByte)255;
			}
		}
		//写入缓冲区
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 300, 300, 100, 50, buffTmp, 100, 50, GDT_Byte, 0, 0);
	}
	
	for (k = 1; k < 4; k++)
	{
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 500, 500, 100, 50, buffTmp, 100, 50, GDT_Byte, 0, 0);

		//遍历区域,逐像素置为0
		for (j = 0; j < tmpYlen; j++)
		{
			for (i = 0; i < tmpXlen; i++)
			{
				buffTmp[j*tmpXlen + i] = (GByte)0;
			}
		}
		//写入缓冲区
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 500, 500, 50, 100, buffTmp, 50, 100, GDT_Byte, 0, 0);
	}

	//清除内存
	CPLFree(buffTmp);

	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
	system("PAUSE");
	return 0;

}