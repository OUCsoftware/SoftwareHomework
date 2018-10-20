// SecondDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
	/// 输入图像
	GDALDataset* poSrcDs;
	/// 输出图像1
	GDALDataset* poDstDs1;
	/// 输出图像2
	GDALDataset* poDstDs2;
	/// 输入图像路径
	char* srcPath = "./data/Input.jpg";
	/// 输出图像1(任务1)
	char* dstPath1 = "./data/Output1.tif";
	/// 输出图像2(任务2)
	char* dstPath2 = "./data/Output2.tif";
	/// 图像的高度和宽度
	double imgXlen, imgYlen;
	/// 图像波段数
	int bandNum;

	/// 注册驱动
	GDALAllRegister();

	/// 打开图像
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	/// 获取图像高度宽度和波段数
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	bandNum = poSrcDs->GetRasterCount();

	/// 创建输出图像1
	poDstDs1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath1, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	/// 创建输出图像2
	poDstDs2 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath2, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	/// 临时变量暂存图像内存
	GByte* Temp;
	/// 为临时变量Temp分配内存
	Temp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	/// 复制输入图像到输出图像1和输出图像2
	for (int i = 0; i < bandNum; i++) {
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, Temp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDs1->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, Temp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDs2->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, Temp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	/// 清除Temp内存
	CPLFree(Temp);

	cout << "Running Task one ..." << endl;
	/// 起始位置坐标
	int startX = 100;
	int startY = 100;
	/// 区域宽度高度
	int tmpXlen = 200;
	int tmpYlen = 150;
	/// 截取图像内存存储
	GByte* buffTmp;
	/// 分配内存
	buffTmp = (GByte*)CPLMalloc(tmpXlen * tmpYlen * sizeof(GByte));
	/// 读取红色通道到buffTmp中
	poSrcDs->GetRasterBand(1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	/// 遍历区域，逐像素置为255
	for (int i = 0; i < tmpYlen; i++) {
		for (int j = 0; j < tmpXlen; j++) {
			buffTmp[tmpXlen * i + j] = (GByte)255;
		}
	}
	/// 数据写入poDstDs1
	poDstDs1->GetRasterBand(1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	cout << "Ended Task one !" << endl;

	cout << "Running Task two ..." << endl;
	/// 重置初始位置坐标
	startX = 300;
	startY = 300;
	/// 重置区域宽度高度
	tmpXlen = 100;
	tmpYlen = 50;
	/// 区域变白
	for (int k = 0; k < bandNum; k++) {
		/// 读取颜色通道到buffTmp中
		poSrcDs->GetRasterBand(k + 1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
		/// 遍历区域，逐像素置为255
		for (int i = 0; i < tmpYlen; i++) {
			for (int j = 0; j < tmpXlen; j++) {
				buffTmp[tmpXlen * i + j] = (GByte)255;
			}
		}
		/// 数据写入poDstDs2
		poDstDs2->GetRasterBand(k + 1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	}
	/// 重置初始位置坐标
	startX = 500;
	startY = 500;
	/// 重置区域宽度高度
	tmpXlen = 50;
	tmpYlen = 100;
	/// 区域变黑
	for (int k = 0; k < bandNum; k++) {
		/// 读取颜色通道到buffTmp中
		poSrcDs->GetRasterBand(k + 1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
		/// 遍历区域，逐像素置为0
		for (int i = 0; i < tmpYlen; i++) {
			for (int j = 0; j < tmpXlen; j++) {
				buffTmp[tmpXlen * i + j] = (GByte)0;
			}
		}
		/// 数据写入poDstDs2
		poDstDs2->GetRasterBand(k + 1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	}
	cout << "Ended Task two !" << endl;

	/// 清除内存
	CPLFree(buffTmp);
	GDALClose(poSrcDs);
	GDALClose(poDstDs1);
	GDALClose(poDstDs2);
	system("pause");
	return 0;
}

