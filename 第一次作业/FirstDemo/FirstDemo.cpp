// FirstDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
	/// 输入图像
	GDALDataset *poSrcDs;
	/// 输出图像
	GDALDataset *poDstDs;
	/// 图像的高度和宽度
	double imgXlen, imgYlen;
	/// 输入图像路径
	char* srcPath = "./data/Input.jpg";
	/// 输出图像路径
	char* dstPath = "./data/Output.tif";
	/// 图像内存存储
	GByte* buffTmp;
	/// 图像波段数
	int i, bandNum;

	/// 注册驱动
	GDALAllRegister();

	/// 打开图像
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	/// 获取图像高度宽度和波段数
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	bandNum = poSrcDs->GetRasterCount();
	/// 输出获得的结果
	cout << "Image X Length = " << imgXlen << endl;
	cout << "Image Y Length = " << imgYlen << endl;
	cout << "Band Number    = " << bandNum << endl;
	/// 根据图像的宽度和高度分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	/// 创建输出图像
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	/// 一个波段一个波段地输入，然后一个波段一个波段地输出
	for (int i = 0; i < bandNum; i++) {
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDs->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		cout << "... ... Band " << i << " Processing ... ..." << endl;
	}
	/// 清除内存
	CPLFree(buffTmp);
	GDALClose(poSrcDs);
	GDALClose(poDstDs);

	system("pause");
    return 0;
}

