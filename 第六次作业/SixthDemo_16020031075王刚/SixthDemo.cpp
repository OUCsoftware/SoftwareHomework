// SixthDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
#include "windows.h"
using namespace std;


int main()
{
	/// 输入输出图像路径
	char* mulPath = "./data/Mul_large.tif";
	char* panPath = "./data/Pan_large.tif";
	char* resPath = "./data/Res_large.tif";
	char* ansPath = "./data/Ans_large.tif";
	/// 注册驱动
	GDALAllRegister();

	GDALDataset *poMulDs, *poPanDs, *poResDs, *poAnsDs;/// 定义指针
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
	poAnsDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(ansPath, Xlen, Ylen, 3, GDT_Byte, NULL);
	/// 输出相应信息
	cout << " ... ... image Xlen : " << Xlen << " pixels ... ...\n";
	cout << " ... ... image Ylen : " << Ylen << " pixels ... ...\n";
	cout << " ... ... Mul image band number : " << poMulDs->GetRasterCount() << endl;
	cout << " ... ... Pan image band number : " << poPanDs->GetRasterCount() << endl;


	DWORD dwStart = GetTickCount();/// 获取当前时间

	cout << "\t....Running The First.... Please wait patiently ...\n";
	/// 块大小
	const int BlockXSize = 256;
	const int BlockYSize = 256;
	/// 分配存储空间
	bandR = (double*)CPLMalloc(BlockXSize * BlockYSize * sizeof(double));
	bandG = (double*)CPLMalloc(BlockXSize * BlockYSize * sizeof(double));
	bandB = (double*)CPLMalloc(BlockXSize * BlockYSize * sizeof(double));
	bandI = (double*)CPLMalloc(BlockXSize * BlockYSize * sizeof(double));
	bandH = (double*)CPLMalloc(BlockXSize * BlockYSize * sizeof(double));
	bandS = (double*)CPLMalloc(BlockXSize * BlockYSize * sizeof(double));
	bandP = (double*)CPLMalloc(BlockXSize * BlockYSize * sizeof(double));
	/// 获得长宽块个数以及余下的长宽长度
	int XBlockCount = Xlen / BlockXSize;
	int XBlockLeft = Xlen % BlockXSize;
	int YBlockCount = Ylen / BlockYSize;
	int YBlockLeft = Ylen % BlockYSize; 
	for (int y = 0; y < YBlockCount; y++) {/// 遍历块
		for (int x = 0; x < XBlockCount; x++) {/// 执行RGB和IHS之间的变换，和实验五的核心代码大同小异
			poMulDs->GetRasterBand(1)->RasterIO(GF_Read, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandR, BlockXSize, BlockYSize, GDT_Float64, 0, 0);
			poMulDs->GetRasterBand(2)->RasterIO(GF_Read, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandG, BlockXSize, BlockYSize, GDT_Float64, 0, 0);
			poMulDs->GetRasterBand(3)->RasterIO(GF_Read, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandB, BlockXSize, BlockYSize, GDT_Float64, 0, 0);
			poPanDs->GetRasterBand(1)->RasterIO(GF_Read, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandP, BlockXSize, BlockYSize, GDT_Float64, 0, 0);
			for (int i = 0; i < BlockXSize* BlockYSize; i++) {
				/// RGB => IHS
				bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
				bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];
				/// IHS => RGB
				bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
				bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
				bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
			}
			poResDs->GetRasterBand(1)->RasterIO(GF_Write, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandR, BlockXSize, BlockYSize, GDT_Float64, 0, 0);
			poResDs->GetRasterBand(2)->RasterIO(GF_Write, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandG, BlockXSize, BlockYSize, GDT_Float64, 0, 0);
			poResDs->GetRasterBand(3)->RasterIO(GF_Write, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandB, BlockXSize, BlockYSize, GDT_Float64, 0, 0);
		}
	}
	/// 释放空间
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);
	/// 分配存储空间
	bandR = (double*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(double));
	bandG = (double*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(double));
	bandB = (double*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(double));
	bandI = (double*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(double));
	bandH = (double*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(double));
	bandS = (double*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(double));
	bandP = (double*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(double));
	for (int y = 0; y < YBlockCount; y++) {/// 遍历y方向上的不足的一块
		poMulDs->GetRasterBand(1)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandR, XBlockLeft, BlockYSize, GDT_Float64, 0, 0);
		poMulDs->GetRasterBand(2)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandG, XBlockLeft, BlockYSize, GDT_Float64, 0, 0);
		poMulDs->GetRasterBand(3)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandB, XBlockLeft, BlockYSize, GDT_Float64, 0, 0);
		poPanDs->GetRasterBand(1)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandP, XBlockLeft, BlockYSize, GDT_Float64, 0, 0);
		for (int i = 0; i < XBlockLeft* BlockYSize; i++) {
			/// RGB => IHS
			bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
			bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];
			/// IHS => RGB
			bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
			bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
			bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
		}
		poResDs->GetRasterBand(1)->RasterIO(GF_Write, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandR, XBlockLeft, BlockYSize, GDT_Float64, 0, 0);
		poResDs->GetRasterBand(2)->RasterIO(GF_Write, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandG, XBlockLeft, BlockYSize, GDT_Float64, 0, 0);
		poResDs->GetRasterBand(3)->RasterIO(GF_Write, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandB, XBlockLeft, BlockYSize, GDT_Float64, 0, 0);
	}
	/// 释放空间
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);
	/// 分配空间
	bandR = (double*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(double));
	bandG = (double*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(double));
	bandB = (double*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(double));
	bandI = (double*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(double));
	bandH = (double*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(double));
	bandS = (double*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(double));
	bandP = (double*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(double)); 
	for (int x = 0; x < XBlockCount; x++) {/// 遍历x方向上的不足的一块
		poMulDs->GetRasterBand(1)->RasterIO(GF_Read, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandR, BlockXSize, YBlockLeft, GDT_Float64, 0, 0);
		poMulDs->GetRasterBand(2)->RasterIO(GF_Read, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandG, BlockXSize, YBlockLeft, GDT_Float64, 0, 0);
		poMulDs->GetRasterBand(3)->RasterIO(GF_Read, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandB, BlockXSize, YBlockLeft, GDT_Float64, 0, 0);
		poPanDs->GetRasterBand(1)->RasterIO(GF_Read, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandP, BlockXSize, YBlockLeft, GDT_Float64, 0, 0);
		for (int i = 0; i < BlockXSize* YBlockLeft; i++) {
			/// RGB => IHS
			bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
			bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];
			/// IHS => RGB
			bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
			bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
			bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
		}
		poResDs->GetRasterBand(1)->RasterIO(GF_Write, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandR, BlockXSize, YBlockLeft, GDT_Float64, 0, 0);
		poResDs->GetRasterBand(2)->RasterIO(GF_Write, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandG, BlockXSize, YBlockLeft, GDT_Float64, 0, 0);
		poResDs->GetRasterBand(3)->RasterIO(GF_Write, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandB, BlockXSize, YBlockLeft, GDT_Float64, 0, 0);
	}
	/// 释放内存
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);
	/// 分配内存
	bandR = (double*)CPLMalloc(XBlockLeft * YBlockLeft * sizeof(double));
	bandG = (double*)CPLMalloc(XBlockLeft * YBlockLeft * sizeof(double));
	bandB = (double*)CPLMalloc(XBlockLeft * YBlockLeft * sizeof(double));
	bandI = (double*)CPLMalloc(XBlockLeft * YBlockLeft * sizeof(double));
	bandH = (double*)CPLMalloc(XBlockLeft * YBlockLeft * sizeof(double));
	bandS = (double*)CPLMalloc(XBlockLeft * YBlockLeft * sizeof(double));
	bandP = (double*)CPLMalloc(XBlockLeft * YBlockLeft * sizeof(double));
	/// x和y方向上的均不足的一块
	poMulDs->GetRasterBand(1)->RasterIO(GF_Read, XBlockCount*BlockXSize, YBlockCount*BlockYSize, XBlockLeft, YBlockLeft, bandR, XBlockLeft, YBlockLeft, GDT_Float64, 0, 0);
	poMulDs->GetRasterBand(2)->RasterIO(GF_Read, XBlockCount*BlockXSize, YBlockCount*BlockYSize, XBlockLeft, YBlockLeft, bandG, XBlockLeft, YBlockLeft, GDT_Float64, 0, 0);
	poMulDs->GetRasterBand(3)->RasterIO(GF_Read, XBlockCount*BlockXSize, YBlockCount*BlockYSize, XBlockLeft, YBlockLeft, bandB, XBlockLeft, YBlockLeft, GDT_Float64, 0, 0);
	poPanDs->GetRasterBand(1)->RasterIO(GF_Read, XBlockCount*BlockXSize, YBlockCount*BlockYSize, XBlockLeft, YBlockLeft, bandP, XBlockLeft, YBlockLeft, GDT_Float64, 0, 0);
	for (int i = 0; i < XBlockLeft* YBlockLeft; i++) {
		/// RGB => IHS
		bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
		bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];
		/// IHS => RGB
		bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
		bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
		bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
	}
	poResDs->GetRasterBand(1)->RasterIO(GF_Write, XBlockCount*BlockXSize, YBlockCount*BlockYSize, XBlockLeft, YBlockLeft, bandR, XBlockLeft, YBlockLeft, GDT_Float64, 0, 0);
	poResDs->GetRasterBand(2)->RasterIO(GF_Write, XBlockCount*BlockXSize, YBlockCount*BlockYSize, XBlockLeft, YBlockLeft, bandG, XBlockLeft, YBlockLeft, GDT_Float64, 0, 0);
	poResDs->GetRasterBand(3)->RasterIO(GF_Write, XBlockCount*BlockXSize, YBlockCount*BlockYSize, XBlockLeft, YBlockLeft, bandB, XBlockLeft, YBlockLeft, GDT_Float64, 0, 0);
	/// 释放内存
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP); 

	DWORD dwStop = GetTickCount();/// 获得当前时间
	double dwInterval = (double)(dwStop - dwStart);/// 得到运行时间
	cout << "...\tThe First 256*256 for one Block Running Time is " << dwInterval << " ms...\n" << endl;/// 输出之


	dwStart = GetTickCount();/// 获得当前时间

	cout << "\t....Running The Second.... Please wait patiently ...\n";
	/// 定义第二种块的大小
	const int BlockXSize_ = Xlen;
	const int BlockYSize_ = 256;
	/// 定义Y方向上上的块个数和不足一块的长度
	int YBlockCount_ = Ylen / BlockYSize_;
	int YBlockLeft_ = Ylen % BlockYSize_;
	/// 分配空间
	bandR = (double*)CPLMalloc(BlockYSize_ * BlockXSize_ * sizeof(double));
	bandG = (double*)CPLMalloc(BlockYSize_ * BlockXSize_ * sizeof(double));
	bandB = (double*)CPLMalloc(BlockYSize_ * BlockXSize_ * sizeof(double));
	bandI = (double*)CPLMalloc(BlockYSize_ * BlockXSize_ * sizeof(double));
	bandH = (double*)CPLMalloc(BlockYSize_ * BlockXSize_ * sizeof(double));
	bandS = (double*)CPLMalloc(BlockYSize_ * BlockXSize_ * sizeof(double));
	bandP = (double*)CPLMalloc(BlockYSize_ * BlockXSize_ * sizeof(double));
	for (int y = 0; y < YBlockCount; y++) {/// 遍历整块
		poMulDs->GetRasterBand(1)->RasterIO(GF_Read, 0, y*BlockYSize_, BlockXSize_, BlockYSize_, bandR, BlockXSize_, BlockYSize_, GDT_Float64, 0, 0);
		poMulDs->GetRasterBand(2)->RasterIO(GF_Read, 0, y*BlockYSize_, BlockXSize_, BlockYSize_, bandG, BlockXSize_, BlockYSize_, GDT_Float64, 0, 0);
		poMulDs->GetRasterBand(3)->RasterIO(GF_Read, 0, y*BlockYSize_, BlockXSize_, BlockYSize_, bandB, BlockXSize_, BlockYSize_, GDT_Float64, 0, 0);
		poPanDs->GetRasterBand(1)->RasterIO(GF_Read, 0, y*BlockYSize_, BlockXSize_, BlockYSize_, bandP, BlockXSize_, BlockYSize_, GDT_Float64, 0, 0);
		for (int i = 0; i < BlockYSize_* BlockXSize_; i++) {
			/// RGB => IHS
			bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
			bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];
			/// IHS => RGB
			bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
			bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
			bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
		}
		poAnsDs->GetRasterBand(1)->RasterIO(GF_Write, 0, y*BlockYSize_, BlockXSize_, BlockYSize_, bandR, BlockXSize_, BlockYSize_, GDT_Float64, 0, 0);
		poAnsDs->GetRasterBand(2)->RasterIO(GF_Write, 0, y*BlockYSize_, BlockXSize_, BlockYSize_, bandG, BlockXSize_, BlockYSize_, GDT_Float64, 0, 0);
		poAnsDs->GetRasterBand(3)->RasterIO(GF_Write, 0, y*BlockYSize_, BlockXSize_, BlockYSize_, bandB, BlockXSize_, BlockYSize_, GDT_Float64, 0, 0);
	}
	/// 释放内存
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);
	/// 分配内存
	bandR = (double*)CPLMalloc(YBlockLeft_ * BlockXSize_ * sizeof(double));
	bandG = (double*)CPLMalloc(YBlockLeft_ * BlockXSize_ * sizeof(double));
	bandB = (double*)CPLMalloc(YBlockLeft_ * BlockXSize_ * sizeof(double));
	bandI = (double*)CPLMalloc(YBlockLeft_ * BlockXSize_ * sizeof(double));
	bandH = (double*)CPLMalloc(YBlockLeft_ * BlockXSize_ * sizeof(double));
	bandS = (double*)CPLMalloc(YBlockLeft_ * BlockXSize_ * sizeof(double));
	bandP = (double*)CPLMalloc(YBlockLeft_ * BlockXSize_ * sizeof(double));
	/// 不足的一块
	poMulDs->GetRasterBand(1)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize_, BlockXSize_, YBlockLeft_, bandR, BlockXSize_, YBlockLeft_, GDT_Float64, 0, 0);
	poMulDs->GetRasterBand(2)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize_, BlockXSize_, YBlockLeft_, bandG, BlockXSize_, YBlockLeft_, GDT_Float64, 0, 0);
	poMulDs->GetRasterBand(3)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize_, BlockXSize_, YBlockLeft_, bandB, BlockXSize_, YBlockLeft_, GDT_Float64, 0, 0);
	poPanDs->GetRasterBand(1)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize_, BlockXSize_, YBlockLeft_, bandP, BlockXSize_, YBlockLeft_, GDT_Float64, 0, 0);
	for (int i = 0; i < YBlockLeft_* BlockXSize_; i++) {
		/// RGB => IHS
		bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
		bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];
		/// IHS => RGB
		bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
		bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
		bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
	}
	poAnsDs->GetRasterBand(1)->RasterIO(GF_Write, 0, YBlockCount*BlockYSize_, BlockXSize_, YBlockLeft_, bandR, BlockXSize_, YBlockLeft_, GDT_Float64, 0, 0);
	poAnsDs->GetRasterBand(2)->RasterIO(GF_Write, 0, YBlockCount*BlockYSize_, BlockXSize_, YBlockLeft_, bandG, BlockXSize_, YBlockLeft_, GDT_Float64, 0, 0);
	poAnsDs->GetRasterBand(3)->RasterIO(GF_Write, 0, YBlockCount*BlockYSize_, BlockXSize_, YBlockLeft_, bandB, BlockXSize_, YBlockLeft_, GDT_Float64, 0, 0);
	/// 释放空间
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);
	dwStop = GetTickCount();/// 获得当前时间
	dwInterval = (double)(dwStop - dwStart);/// 得到运行时间
	cout << "...\tThe Second Xlen*256 for one Block Running Time is " << dwInterval << " ms...\n" << endl;/// 输出之

	/// 关闭指针
	GDALClose(poMulDs);
	GDALClose(poPanDs);
	GDALClose(poResDs);
	GDALClose(poAnsDs);
	/// 控制台暂停
	system("pause");
    return 0;
}

