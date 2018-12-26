
#include<iostream>
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* input;//输入图像
	GDALDataset* output1, *output2, *output3, *output4, *output5,*output6;//输出图像
	int imgXlen, imgYlen;
	double matirx(double a[3][3], double b[3][3]);
	double matirx(double a[5][5], double b[5][5]);
	const char* inpath = "women.jpg";//输入图像路径
	const char* outpath1 = "1.tif";
	const char* outpath2 = "2.tif";
	const char* outpath3 = "3.tif";
	const char* outpath4 = "4.tif";
	const char* outpath5 = "5.tif";
	const char* outpath6 = "6.tif";//输出图像路径
	GByte* buffTmp[3], *buffTmp1[6][3];//内存存储
	int i,m=0,n=0, bandNum,result[6];//波段数
	GDALAllRegister();
	input = (GDALDataset*)GDALOpenShared(inpath, GA_ReadOnly);
	//获取宽度高度和波段数
	imgXlen = input->GetRasterXSize();
	imgYlen = input->GetRasterYSize();
	bandNum = input->GetRasterCount();
	//创建输出图像
	output1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outpath1, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	output2 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outpath2, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	output3 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outpath3, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	output4 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outpath4, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	output5 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outpath5, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	output6 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outpath6, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//matirx
	double  matrix1[3][3] = { { 0,1,0 },{ 1,1,1 },{ 0,1,0 } };
	double  matrix2[5][5] = { { 1,0,0,0,0 },{ 0,1,0,0,0 },{ 0,0,1,0,0 },{ 0,0,0,1,0 },{ 0,0,0,0,1 } };
	double  matrix3[3][3] = { { -1,-1,-1 },{ -1,8,-1 },{ -1,-1,-1 } };
	double  matrix4[3][3] = { { -1,-1,-1 },{ -1,9,-1 },{ -1,-1,-1 } };
	double  matrix5[3][3] = { { -1,-1,0 },{ -1,0,1 },{ 0,1,1 } };
	double  matrix6[5][5] = { { 0.0120,0.1253,0.2736,0.1253,0.0120 },{ 0.1253,1.3054,2.8514,1.3054,0.1253 },
	{ 0.2736,2.8514,6.2279,2.8514,0.2736 },{ 0.1253,1.3054,2.8514,1.3054,0.1253 },{ 0.0120,0.1253,0.2736,0.1253,0.0120 } };
    //根据宽度和高度分配内存
	for (i = 0; i < bandNum; i++)
	{
		buffTmp[i] = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
		for (int j = 0; j < 6; j++)
			buffTmp1[j][i] = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	}
	double example1[3][3];
	double example2[5][5];

	for (i = 0; i < bandNum; i++)
	{
		input->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp[i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		for (m = 0; m < imgYlen; m++)
		{

			for (n = 0; n < imgXlen; n++)
			{
				if (m >= 1 && m <= imgYlen - 2 && n >= 1 && n <= imgXlen - 2)
				{
					for (int l1 = 0; l1 < 3; l1++)
					{
						for (int l2 = 0; l2 < 3; l2++)
							example1[l1][l2] = buffTmp[i][(m + (l1 - 1))*imgXlen + n + (l2 - 1)];
					}
					result[0] = (int)(0.2*matirx(example1, matrix1));
					result[2] = (int)(matirx(example1, matrix3));
					result[3] = (int)(matirx(example1, matrix4));
					result[4] = (int)(matirx(example1, matrix5));
				}
				else
				{
					result[0] = 0;
					result[2] = 0;
					result[3] = 0;
					result[4] = 0;
				}
				if (m >= 2 && m <= imgYlen - 3 && n >= 2 && n <= imgXlen - 3)
				{
					for (int l1 = 0; l1 < 5; l1++)
					{
						for (int l2 = 0; l2 < 5; l2++)
							example2[l1][l2] = buffTmp[i][(m + (l1 - 2))*imgXlen + n + (l2 - 2)];
					}
					result[1] = (int)(0.2*matirx(example2, matrix2));
					result[5] = (int)(matirx(example2, matrix6) / 25);
				}
				else
				{
					result[1] = 0;
					result[5] = 0;
				}
				for (int l3 = 0; l3 < 6; l3++)
				{
					if (result[l3] < 0)    result[l3] = 0;
					else if (result[l3] > 255) result[l3] = 255;
					buffTmp1[l3][i][m*imgXlen + n] = (GByte)result[l3];
				}
			}
		}
		output1->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp1[0][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		output2->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp1[1][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		output3->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp1[2][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		output4->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp1[3][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		output5->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp1[4][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		output6->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp1[5][i], imgXlen, imgYlen, GDT_Byte, 0, 0);

	}
	//清理内存

	CPLFree(buffTmp[0]);
	CPLFree(buffTmp[1]);
	CPLFree(buffTmp[2]);
	for (i = 0; i < 6; i++)
	{
		CPLFree(buffTmp1[i][0]);
		CPLFree(buffTmp1[i][1]);
		CPLFree(buffTmp1[i][2]);
	}

	//关闭dataset
	GDALClose(input);
	GDALClose(output1);
	GDALClose(output2);
	GDALClose(output3);
	GDALClose(output4);
	GDALClose(output5);
	GDALClose(output6);
	system("PAUSE");
	return 0;

}
double matirx(double a[5][5], double b[5][5])
{
	double result = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			result += a[i][j] * b[i][j];
		}
	}
	return result;
}
double matirx(double a[3][3], double b[3][3])
{
	double result = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result += a[i][j] * b[i][j];
		}
	}
	return result;
}