// FifthDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main()
{
	/// 输入输出图像路径
	char* mulPath = "./data/American_Mul.bmp";
	char* panPath = "./data/American_Pan.bmp";
	char* resPath = "./data/American_Res.tif";
	/// 注册驱动
	GDALAllRegister();

	GDALDataset *poMulDs, *poPanDs, *poResDs;/// 定义指针
	int Xlen, Ylen;/// 图像长宽
	/// 定义R,G,B,I,H,S,P数组
	double *bandR;
	double *bandG;
	double *bandB;
	double *bandI;
	double *bandH;
	double *bandS;
	double *bandP;
	/// 打开输入图像
	poMulDs = (GDALDataset*)GDALOpenShared(mulPath, GA_ReadOnly);
	poPanDs = (GDALDataset*)GDALOpenShared(panPath, GA_ReadOnly);
	/// 获取图像长宽
	Xlen = poMulDs->GetRasterXSize();
	Ylen = poMulDs->GetRasterYSize();
	/// 创建输出图像
	poResDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(resPath, Xlen, Ylen, 3, GDT_Byte, NULL);
	/// 输出相应信息
	cout << " ... ... image Xlen : " << Xlen << " pixels ... ...\n";
	cout << " ... ... image Ylen : " << Ylen << " pixels ... ...\n";
	cout << " ... ... Mul image band number : " << poMulDs->GetRasterCount() << endl;
	cout << " ... ... Pan image band number : " << poPanDs->GetRasterCount() << endl;
	/// 为数组们开辟空间
	bandR = (double*)CPLMalloc(Xlen * Ylen * sizeof(double));
	bandG = (double*)CPLMalloc(Xlen * Ylen * sizeof(double));
	bandB = (double*)CPLMalloc(Xlen * Ylen * sizeof(double));
	bandI = (double*)CPLMalloc(Xlen * Ylen * sizeof(double));
	bandH = (double*)CPLMalloc(Xlen * Ylen * sizeof(double));
	bandS = (double*)CPLMalloc(Xlen * Ylen * sizeof(double));
	bandP = (double*)CPLMalloc(Xlen * Ylen * sizeof(double));
	/// 把波段写入相应数组中
	poMulDs->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, Xlen, Ylen, bandR, Xlen, Ylen, GDT_Float64, 0, 0);
	poMulDs->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, Xlen, Ylen, bandG, Xlen, Ylen, GDT_Float64, 0, 0);
	poMulDs->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, Xlen, Ylen, bandB, Xlen, Ylen, GDT_Float64, 0, 0);
	poPanDs->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, Xlen, Ylen, bandP, Xlen, Ylen, GDT_Float64, 0, 0);
	/// 循环赋值
	for (int i = 0; i < Xlen* Ylen; i++) {
		/// RGB => IHS
		bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
		bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];
		/// IHS => RGB
		bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
		bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
		bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
	}
	/// 将RGB波段写入输出图像中
	poResDs->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, Xlen, Ylen, bandR, Xlen, Ylen, GDT_Float64, 0, 0);
	poResDs->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, Xlen, Ylen, bandG, Xlen, Ylen, GDT_Float64, 0, 0);
	poResDs->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, Xlen, Ylen, bandB, Xlen, Ylen, GDT_Float64, 0, 0);
	/// 关闭指针
	GDALClose(poMulDs);
	GDALClose(poPanDs);
	GDALClose(poResDs);
	/// 释放空间
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);
	/// 控制台暂停
	system("pause");
	return 0;
}



