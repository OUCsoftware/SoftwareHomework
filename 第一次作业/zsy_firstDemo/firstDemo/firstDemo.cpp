#include"stdafx.h"
#include <iostream>
using namespace std;
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
	GDALDataset* poSrcDS;           //输入图像
	GDALDataset* poDstDS;			//输出图像
	int imgXlen, imgYlen;			//图像的宽度和高度
	char* srcPath = "trees.jpg";	//输入图像路径
	char* dstPath = "res.tif";		//输出图像路径
	GByte* buffTmp;					//图像内存存储
	int i, bandNum;					//图像波段数
									//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像宽高和波段数量
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//输出获取结果
	cout << "Image X Length: " << imgXlen << endl;
	cout << "Image Y Length: " << imgYlen << endl;
	cout << "Band Number: " << bandNum << endl;

	//根据图像的宽高分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//波段的输入与输出
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		printf("...... band %d processing ......\n", i);
	}

	//清除内存
	CPLFree(buffTmp);
	//关闭dataset，先打开的后关
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
	return 0;
}