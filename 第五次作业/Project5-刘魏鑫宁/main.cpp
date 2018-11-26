#include <iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	/*
	initial
	*/
	//多色调图像
	GDALDataset* poSrcThumDs;
	//清晰图像
	GDALDataset* poSrcPanDs;
	//输出图像
	GDALDataset* poDstDs;

	//图像的宽度和高度
	int imgXlen, imgYlen, bandNum;
	//输入图像的路径
	const char* srcThumPath = "American_MUL.bmp";
	const char* srcPanPath = "American_PAN.bmp";
	//输出图像的路径
	const char* DstPath = "beauty.tif";
	int i, j;

	//图像内存储存
	float* trans_in_R;
	float* trans_in_G;
	float* trans_in_B;
	//tran1的结果
	float* trans_out_I;
	float* trans_out_H;
	float* trans_out_S;
	//tran2的结果
	float* trans_res_R;
	float* trans_res_G;
	float* trans_res_B;
	//单色图像
	float* bufPan;

	//----RGB ==> IHS 正变换矩阵----
	float tran1[3][3] = {
	{1.0f/ 3.0f, 1.0f / 3.0f, 1.0f / 3.0f},
	{-sqrt(2.0f) / 6.0f, -sqrt(2.0f) / 6.0f, sqrt(2.0f) / 3.0f},
	{1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f), 0 } };
	// IHS == > RGB 逆变换矩阵
	float tran2[3][3] = {
	{1.0f, -1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f)},
	{1.0f, -1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f)},
	{1, sqrt(2.0f), 0} };

	/*
	复制图像的像素变成[1:3*xlen*ylen]
	*/
	//驱动
	GDALAllRegister();
	
	//打开不清晰图像
	poSrcThumDs = (GDALDataset*)GDALOpenShared(srcThumPath, GA_ReadOnly);
	poSrcPanDs = (GDALDataset*)GDALOpenShared(srcPanPath, GA_ReadOnly);
	//获取图像宽度，高度和波段数
	imgXlen = poSrcThumDs->GetRasterXSize();
	imgYlen = poSrcThumDs->GetRasterYSize();
	bandNum = poSrcThumDs->GetRasterCount();
	//创建输出图像
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		DstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL
	);

	

	//根据图像的宽度和高度分配内存
	bufPan = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//多色调图像的三个通道
	trans_in_R = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	trans_in_G = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	trans_in_B = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//I,H,S
	trans_out_I = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	trans_out_H = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	trans_out_S = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//变回来的RGB
	trans_res_R = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	trans_res_G = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	trans_res_B = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//读入多色调图像数据

	poSrcThumDs->GetRasterBand(1)->RasterIO(GF_Read,
		0, 0, imgXlen, imgYlen, trans_in_R, imgXlen, imgYlen, GDT_Float32, 0, 0);
	poSrcThumDs->GetRasterBand(2)->RasterIO(GF_Read,
		0, 0, imgXlen, imgYlen, trans_in_G, imgXlen, imgYlen, GDT_Float32, 0, 0);
	poSrcThumDs->GetRasterBand(3)->RasterIO(GF_Read,
		0, 0, imgXlen, imgYlen, trans_in_B, imgXlen, imgYlen, GDT_Float32, 0, 0);


	//进行 RGB ==> IHS 正变换
	for (i = 0; i < imgYlen; i++)
	{
		for (j = 0; j < imgXlen; j++)
		{
			trans_out_I[i*imgXlen + j] = tran1[0][0] * trans_in_R[i*imgXlen + j] +
				tran1[0][1] * trans_in_G[i*imgXlen + j] + tran1[0][2] * trans_in_B[i*imgXlen + j];
			trans_out_H[i*imgXlen + j] = tran1[1][0] * trans_in_R[i*imgXlen + j] +
				tran1[1][1] * trans_in_G[i*imgXlen + j] + tran1[1][2] * trans_in_B[i*imgXlen + j];
			trans_out_S[i*imgXlen + j] = tran1[2][0] * trans_in_R[i*imgXlen + j] +
				tran1[2][1] * trans_in_G[i*imgXlen + j] + tran1[2][2] * trans_in_B[i*imgXlen + j];
		}
	}

	//为bufPan的单色通道赋值
	poSrcPanDs->GetRasterBand(1)->RasterIO(GF_Read,
		0, 0, imgXlen, imgYlen, bufPan, imgXlen, imgYlen, GDT_Float32, 0, 0);
	


	//替换I向量
	for (j = 0; j < imgXlen; j++)
	{
		for (i = 0; i < imgYlen; i++)
		{
			trans_out_I[j*imgYlen+i] = bufPan[j*imgYlen + i];
		}
	}
	

	//进行 IHS ==> RGB 反变换
		for (i = 0; i < imgYlen; i++)
		{
			for (j = 0; j < imgXlen; j++)
			{
				trans_res_R[i*imgXlen + j] = tran2[0][0] * trans_out_I[i*imgXlen + j] +
				tran2[0][1] * trans_out_H[i*imgXlen + j] + tran2[0][2] * trans_out_S[i*imgXlen + j];
				trans_res_G[i*imgXlen + j] = tran2[1][0] * trans_out_I[i*imgXlen + j] +
					tran2[1][1] * trans_out_H[i*imgXlen + j] + tran2[1][2] * trans_out_S[i*imgXlen + j];
				trans_res_B[i*imgXlen + j] = tran2[2][0] * trans_out_I[i*imgXlen + j] +
					tran2[2][1] * trans_out_H[i*imgXlen + j] + tran2[2][2] * trans_out_S[i*imgXlen + j];
			} 
		}

	//把变换结果保存到 im_fus 中
		

	
	//写入
	poDstDs->GetRasterBand(1)->RasterIO(GF_Write,
		0, 0, imgXlen, imgYlen, trans_res_R, imgXlen, imgYlen, GDT_Float32, 0, 0);
	poDstDs->GetRasterBand(2)->RasterIO(GF_Write,
		0, 0, imgXlen, imgYlen, trans_res_G, imgXlen, imgYlen, GDT_Float32, 0, 0);
	poDstDs->GetRasterBand(3)->RasterIO(GF_Write,
		0, 0, imgXlen, imgYlen, trans_res_B, imgXlen, imgYlen, GDT_Float32, 0, 0);

	
	//清除内存
	CPLFree(bufPan);
	CPLFree(trans_in_R);
	CPLFree(trans_in_G);
	CPLFree(trans_in_B);
	CPLFree(trans_out_I);
	CPLFree(trans_out_H);
	CPLFree(trans_out_S);
	CPLFree(trans_res_R);
	CPLFree(trans_res_G);
	CPLFree(trans_res_B);
	
	//清除数据集
	GDALClose(poDstDs);
	GDALClose(poSrcPanDs);
	GDALClose(poSrcThumDs);
	system("PAUSE");

	return 0;
}