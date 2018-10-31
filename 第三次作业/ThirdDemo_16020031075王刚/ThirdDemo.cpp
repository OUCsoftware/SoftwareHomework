// ThirdDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main()
{
	DWORD dwStart = GetTickCount();///记录开始运行时间

	/// 输入图像
	GDALDataset* inputSpace;
	GDALDataset* inputSuperman;
	/// 输出图像
	GDALDataset* output;
	/// 输入输出图像路径
	const char* spacePath = "./data/space.jpg";
	const char* supermanPath = "./data/superman.jpg";
	const char* outputPath = "./data/result.tif";

	GDALAllRegister();/// 注册驱动
	/// 打开图像
	inputSpace = (GDALDataset*)GDALOpenShared(spacePath, GA_ReadOnly);
	inputSuperman = (GDALDataset*)GDALOpenShared(supermanPath, GA_ReadOnly);
	/// 获取superman的参数
	int supermanXlen = inputSuperman->GetRasterXSize();
	int supermanYlen = inputSuperman->GetRasterYSize();
	int supermanBandNum = inputSuperman->GetRasterCount();
	/// 创建输出图像
	output = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputPath, supermanXlen, supermanYlen, supermanBandNum, GDT_Byte, NULL);
	/// 获取space的参数
	int spaceXlen = inputSpace->GetRasterXSize();
	int spaceYlen = inputSpace->GetRasterYSize();
	/// 定义spaceBuffTmp指针数组并为之开辟空间
	GByte* spaceBuffTmp[3] = { (GByte*)CPLMalloc(spaceXlen * spaceYlen * sizeof(GByte)),
							   (GByte*)CPLMalloc(spaceXlen * spaceYlen * sizeof(GByte)),
							   (GByte*)CPLMalloc(spaceXlen * spaceYlen * sizeof(GByte)) };
	/// 定义supermanBuffTmp指针数组并为之开辟空间
	GByte* supermanBuffTmp[3] = { (GByte*)CPLMalloc(supermanXlen * supermanYlen * sizeof(GByte)),
							      (GByte*)CPLMalloc(supermanXlen * supermanYlen * sizeof(GByte)),
		                          (GByte*)CPLMalloc(supermanXlen * supermanYlen * sizeof(GByte)) };
	/// 将superman的像素信息读入supermanBuffTmp，将space的像素信息读入spaceBuffTmp
	for (int i = 0; i < supermanBandNum; i++) {
		inputSuperman->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, supermanXlen, supermanYlen, supermanBuffTmp[i], supermanXlen, supermanYlen, GDT_Byte, 0, 0);
		inputSpace->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, spaceXlen, spaceYlen, spaceBuffTmp[i], spaceXlen, spaceYlen, GDT_Byte, 0, 0);
	}
	/// 定义限制区域上界和下界
	const int rgb[3][2] = { { 10, 160 },{ 100, 220 },{ 10, 110 } };
	/// 遍历，对于符合条件的supermanBuffTmp，将其赋值给spaceBuffTmp
	for (int i = 0; i < spaceYlen; i++) {
		for (int j = 0; j < spaceXlen; j++) {
			bool flag1 = false;
			bool flag2 = false;
			bool flag3 = false;
			if (int(supermanBuffTmp[0][i*spaceXlen + j]) > rgb[0][0] && int(supermanBuffTmp[0][i*spaceXlen + j]) < rgb[0][1])
				flag1 = true;
			if (int(supermanBuffTmp[1][i*spaceXlen + j]) > rgb[1][0] && int(supermanBuffTmp[1][i*spaceXlen + j]) < rgb[1][1])
				flag2 = true;
			if (int(supermanBuffTmp[2][i*spaceXlen + j]) > rgb[2][0] && int(supermanBuffTmp[2][i*spaceXlen + j]) < rgb[2][1])
				flag3 = true;
			if (!(flag1&&flag2&&flag3)) {
				for (int p = 0; p < supermanBandNum; p ++)
					spaceBuffTmp[p][i*spaceXlen + j] = supermanBuffTmp[p][i*spaceXlen + j];
			}
		}
	}
	/// 将spaceBuffTmp写入output，并清除开辟的空间
	for (int i = 0; i < supermanBandNum; i++) {
		output->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, spaceXlen, spaceYlen, spaceBuffTmp[i], spaceXlen, spaceYlen, GDT_Byte, 0, 0);
		CPLFree(spaceBuffTmp[i]);
		CPLFree(supermanBuffTmp[i]);
	}
	/// 关闭文件
	GDALClose(inputSpace);
	GDALClose(inputSuperman);
	GDALClose(output);

	DWORD dwStop = GetTickCount();/// 获取结束时间
	double time = (double)(dwStop - dwStart);/// 获得运行时间
	cout << "\n\tTime Consuming : " << time << "ms\n\n";/// 输出运行时间
	
	system("pause");
    return 0;
}

