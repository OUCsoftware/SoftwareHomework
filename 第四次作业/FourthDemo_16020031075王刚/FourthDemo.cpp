// FourthDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstdio>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

const double convolutionKernel01[3][3] = {
	{ 0, 1, 0 },
	{ 1, 1, 1 },
	{ 0, 1, 0 }
};/// multi 0.2
int N1 = 3; double Multi01 = 0.2;///均值模糊
const double convolutionKernel02[5][5] = {
	{ 1, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 }
};/// multi 0.2
int N2 = 5; double Multi02 = 0.2;/// 运动模糊
const double convolutionKernel03[3][3] = {
	{ -1, -1, -1 },
	{ -1,  8, -1 },
	{ -1, -1, -1 }
};
int N3 = 3; double Multi03 = 1;///边缘检测
const double convolutionKernel04[3][3] = {
	{ -1, -1, -1 },
	{ -1,  9, -1 },
	{ -1, -1, -1 }
};
int N4 = 3; double Multi04 = 1;/// 图像锐化
const double convolutionKernel05[3][3] = {
	{ -1, -1,  0 },
	{ -1,  0,  1 },
	{  0,  1,  1 }
};
int N5 = 3; double Multi05 = 1;/// 浮雕
const double convolutionKernel06[5][5] = {
	{ 0.0120, 0.1253, 0.2736, 0.1253, 0.0120 },
	{ 0.1253, 1.3054, 2.8514, 1.3054, 0.1253 },
	{ 0.2736, 2.8514, 6.2279, 2.8514, 0.2736 },
	{ 0.1253, 1.3054, 2.8514, 1.3054, 0.1253 },
	{ 0.0120, 0.1253, 0.2736, 0.1253, 0.0120 }
};
int N6 = 5; double Multi06 = 1.0 / 25;/// 高斯模糊

int bandNum;/// 波段数
int inputXlen;/// 图像的宽度
int inputYlen;/// 图像的高度
void CalculateConvolution(int order, GDALDataset* input, GDALDataset* output, int N, double Multi)
{/// order:确定使用哪一个convolutionKernel;input:输入图像;output:输出图像;N:convolutionKernel的大小;Multi:最终和的倍数
	GByte* buffTmp;/// 临时变量
	buffTmp = (GByte*)CPLMalloc(inputXlen * inputYlen * sizeof(GByte));/// 为buffTmp开辟空间
	for (int band = 0; band < bandNum; band++) {/// 遍历RGB波段
		input->GetRasterBand(band + 1)->RasterIO(GF_Read, 0, 0, inputXlen, inputYlen, buffTmp, inputXlen, inputYlen, GDT_Byte, 0, 0);/// buffTmp读入input的数据
		for (int i = 0; i < inputYlen; i++) {/// 遍历buffTmp
			for (int j = 0; j < inputXlen; j++) {
				if (i < N / 2 || j < N / 2 || i >= inputYlen - N / 2 || j >= inputXlen - N / 2) {/// **这里边界设置有点小问题**
					buffTmp[i * inputXlen + j] = (GByte)0;/// 对于边界上没有像素值的位置，直接置为0
				}
				else {
					double ans = 0;/// 临时和
					for (int m = 0; m < N; m++) {/// 根据order的值遍历convolutionKernel
						for (int n = 0; n < N; n++) {
							if (order == 1) {/// 这里存疑，对于convolutionKernel和当前（i，j）对应位置相乘的问题，注释掉的是有问题的
								ans += convolutionKernel01[m][n] * (double)buffTmp[(i + m)*inputXlen + (j + n)];
								//ans += convolutionKernel01[m][n] * (double)buffTmp[(i - N / 2 + m)*inputXlen + (j - N / 2 + n)];
							}
							if (order == 2) {
								ans += convolutionKernel02[m][n] * (double)buffTmp[(i + m)*inputXlen + (j + n)];
								//ans += convolutionKernel02[m][n] * (double)buffTmp[(i - N / 2 + m)*inputXlen + (j - N / 2 + n)];
							}
							if (order == 3) {
								ans += convolutionKernel03[m][n] * (double)buffTmp[(i + m)*inputXlen + (j + n)];
								//ans += convolutionKernel03[m][n] * (double)buffTmp[(i - N / 2 + m)*inputXlen + (j - N / 2 + n)];
							}
							if (order == 4) {
								ans += convolutionKernel04[m][n] * (double)buffTmp[(i + m)*inputXlen + (j + n)];
								//ans += convolutionKernel04[m][n] * (double)buffTmp[(i - N / 2 + m)*inputXlen + (j - N / 2 + n)];
							}
							if (order == 5) {
								ans += convolutionKernel05[m][n] * (double)buffTmp[(i + m)*inputXlen + (j + n)];
								//ans += convolutionKernel05[m][n] * (double)buffTmp[(i - N / 2 + m)*inputXlen + (j - N / 2 + n)];
							}
							if (order == 6) {
								ans += convolutionKernel06[m][n] * (double)buffTmp[(i + m)*inputXlen + (j + n)];
								//ans += convolutionKernel06[m][n] * (double)buffTmp[(i - N / 2 + m)*inputXlen + (j - N / 2 + n)];
							}
						}
					}
					ans = ans * Multi;/// ans扩大Multi倍
					if (ans < 0)/// ans为负，直接置零
						ans = 0;
					if (ans > 255)/// ans大于255，直接置为255
						ans = 255;
					buffTmp[i * inputXlen + j] = (GByte)ans;/// 将buffTmp当前位置像素值置为ans
				}
			}
		}
		output->GetRasterBand(band + 1)->RasterIO(GF_Write, 0, 0, inputXlen, inputYlen, buffTmp, inputXlen, inputYlen, GDT_Byte, 0, 0);/// 将buffTmp写入output
	}
	CPLFree(buffTmp);/// 清除开辟的零食空间buffTmp，防止内存泄漏
}
int main()
{
	GDALDataset* input;/// 输入图像
	GDALDataset* output01;/// 输出图像01
	GDALDataset* output02;/// 输出图像02
	GDALDataset* output03;/// 输出图像03
	GDALDataset* output04;/// 输出图像04
	GDALDataset* output05;/// 输出图像05
	GDALDataset* output06;/// 输出图像06
	const char* inputPath = "./data/lena.jpg";/// 输入图像路径
	const char* output01Path = "./data/result01.tif";/// 输出图像01路径
	const char* output02Path = "./data/result02.tif";/// 输出图像02路径
	const char* output03Path = "./data/result03.tif";/// 输出图像03路径
	const char* output04Path = "./data/result04.tif";/// 输出图像04路径
	const char* output05Path = "./data/result05.tif";/// 输出图像05路径
	const char* output06Path = "./data/result06.tif";/// 输出图像06路径

	GDALAllRegister();/// 注册驱动

	input = (GDALDataset*)GDALOpenShared(inputPath, GA_ReadOnly);/// 读入图像
    bandNum = input->GetRasterCount();/// 获取波段数
	inputXlen = input->GetRasterXSize();/// 得到图像的宽度
	inputYlen = input->GetRasterYSize();/// 得到图像的高度
	/// 创建输出图像们
	output01 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(output01Path, inputXlen, inputYlen, bandNum, GDT_Byte, NULL);
	output02 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(output02Path, inputXlen, inputYlen, bandNum, GDT_Byte, NULL);
	output03 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(output03Path, inputXlen, inputYlen, bandNum, GDT_Byte, NULL);
	output04 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(output04Path, inputXlen, inputYlen, bandNum, GDT_Byte, NULL);
	output05 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(output05Path, inputXlen, inputYlen, bandNum, GDT_Byte, NULL);
	output06 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(output06Path, inputXlen, inputYlen, bandNum, GDT_Byte, NULL);
	/// 执行图像过滤函数
	CalculateConvolution(1, input, output01, N1, Multi01);
	CalculateConvolution(2, input, output02, N2, Multi02);
	CalculateConvolution(3, input, output03, N3, Multi03);
	CalculateConvolution(4, input, output04, N4, Multi04);
	CalculateConvolution(5, input, output05, N5, Multi05);
	CalculateConvolution(6, input, output06, N6, Multi06);
	/// 关闭指针
	GDALClose(input);
	GDALClose(output01);
	GDALClose(output02);
	GDALClose(output03);
	GDALClose(output04);
	GDALClose(output05);
	GDALClose(output06);

	system("pause");
	return 0;
}