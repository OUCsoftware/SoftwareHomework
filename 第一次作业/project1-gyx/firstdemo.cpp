// firstdemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc;
	//输出图像
	GDALDataset* podes;

	//图像的宽度和高度
	int imgXlen, imgYlen;
	//图像的路径
	char *srcpath = "trees.jpg";
	//输出图像的路径,只能保存为tif类型的
	char *despath = "res,tif";
	//图像内存存储
	GByte *buffTmp;
	//图像波段数
	int i, bandNum;

	//注册驱动
	GDALAllRegister();
	//打开图像
	posrc = (GDALDataset*)GDALOpenShared(srcpath, GA_ReadOnly);

	//获取图像的宽度，高度和波段数
	imgXlen = posrc->GetRasterXSize();
	imgYlen = posrc->GetRasterYSize();
	bandNum = posrc->GetRasterCount();
	cout << "Image X Length: " << imgXlen << endl;
	cout << "Image Y Length: " << imgYlen << endl;
	cout << "Band number:" << bandNum << endl;

	//根据图像大小进行分配空间
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//创建输出图像
	podes = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//一个个波段的输入，然后一个个波段的输出
	for (i = 0; i < bandNum; i++)
	{
		posrc->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		printf("... ... band %d processing ... ...\n", i);
	}
	//清楚内存
	CPLFree(buffTmp);
	//关闭dataset
	GDALClose(posrc);
	GDALClose(podes);

	system("PAUSE");
	return 0;
}

