#include<iostream>
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main() {
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS1;
	GDALDataset* poDstDS2;
	GDALDataset* poDstDS3;
	GDALDataset* poDstDS4;
	GDALDataset* poDstDS5;
	GDALDataset* poDstDS6;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	const char* srcPath = "linda.jpg";
	//输出图像路径
	const char* dstPath1 = "1.tif";
	const char* dstPath2 = "2.tif";
	const char* dstPath3 = "3.tif";
	const char* dstPath4 = "4.tif";
	const char* dstPath5 = "5.tif";
	const char* dstPath6 = "6.tif";
	//---1atrix1
	int matrix1[3][3] = { {0,1,0},{1,1,1},{0,1,0} };
	int matrix2[5][5] = { {1,0,0,0,0},{0,1,0,0,0},{0,0,1,0,0},{0,0,0,1,0},{0,0,0,0,1} };
	int matrix3[3][3] = { {-1,-1,-1},{-1,8,-1},{-1,-1,-1} };
	int matrix4[3][3] = { {-1,-1,-1},{-1,9,-1},{-1,-1,-1} };
	int matrix5[3][3] = { {-1,-1,0},{-1,0,1},{0,1,1} };
	double matrix6[5][5] = { {0.0120,0.1253,0.2736,0.1253,0.0120},{0.1253,1.3054,2.8514,1.3054,0.1253},
	{0.2736,2.8514,6.2279,2.8514,0.2736},{0.1253,1.3054,2.8514,1.3054,0.1253},{0.0120,0.1253,0.2736,0.1253,0.0120} };

	//图像内存存储
	GByte* buffTmp[3];
	//图像波段数
	int i,j,k, bandNum;
	//卷积核的结果
	int t_i, t_j;
	double result = 0;
	//是否是边缘
	int flag = 0;

	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像宽度,高度,和波段数
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	for (i = 0; i < bandNum; i++)
	{
		buffTmp[i] = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	}

	//创建输出图像
	poDstDS1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath1, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	poDstDS2 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath2, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	poDstDS3 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath3, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	poDstDS4 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath4, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	poDstDS5 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath5, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	poDstDS6 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath6, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//图像1
	cout << "----卷积核一-----" << endl;
	
	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
		//开始处理
		for (i = 0; i < imgXlen; i++)
		{
			for (j = 0; j < imgYlen; j++)
			{
				result = 0;
				flag = 0;
				//矩阵
				for (t_i = 0; t_i < 3&&!flag; t_i++)
				{
					for (t_j = 0; t_j < 3&&!flag; t_j++)
					{
						if (i - 1 + t_i < imgXlen && i - 1 + t_i >= 0 && j - 1 + t_j < imgYlen && j - 1 + t_j >= 0)
						{
							result += buffTmp[k][(i - 1 + t_i)*imgYlen+ j - 1 + t_j] * matrix1[t_i][t_j];
						}
						else
						{
							flag = 1;
							result = 0;
						}
					}
				}
				result *= 0.2;
				buffTmp[k][i*imgYlen + j] = (GByte)result;
			}
			
		}
		poDstDS1->GetRasterBand(k + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//图像2
	cout << "----卷积核二-----" << endl;

	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
		//开始处理
		for (i = 0; i < imgXlen; i++)
		{
			for (j = 0; j < imgYlen; j++)
			{
				result = 0;
				flag = 0;
				//矩阵
				for (t_i = 0; t_i < 5 && !flag; t_i++)
				{
					for (t_j = 0; t_j < 5 && !flag; t_j++)
					{
						if (i - 1 + t_i < imgXlen && i - 1 + t_i >= 0 && j - 1 + t_j < imgYlen && j - 1 + t_j >= 0)
						{
							result += buffTmp[k][(i - 1 + t_i)*imgYlen + j - 1 + t_j] * matrix2[t_i][t_j];
						}
						else
						{
							flag = 1;
							result = 0;
						}
					}
				}
				result *= 0.2;
				buffTmp[k][i*imgYlen + j] = (GByte)result;
			}

		}
		poDstDS2->GetRasterBand(k + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//图像3
	cout << "----卷积核三-----" << endl;

	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
		//开始处理
		for (i = 0; i < imgXlen; i++)
		{
			for (j = 0; j < imgYlen; j++)
			{
				result = 0;
				flag = 0;
				//矩阵
				for (t_i = 0; t_i < 3 && !flag; t_i++)
				{
					for (t_j = 0; t_j < 3 && !flag; t_j++)
					{
						if (i - 1 + t_i < imgXlen && i - 1 + t_i >= 0 && j - 1 + t_j < imgYlen && j - 1 + t_j >= 0)
						{
							result += buffTmp[k][(i - 1 + t_i)*imgYlen + j - 1 + t_j] * matrix3[t_i][t_j];
						}
						else
						{
							flag = 1;
							result = 0;
						}
					}
				}
				result = (unsigned char)result;
				buffTmp[k][i*imgYlen + j] = (GByte)result;
			}

		}
		poDstDS3->GetRasterBand(k + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//图像4
	cout << "----卷积核四-----" << endl;

	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
		//开始处理
		for (i = 0; i < imgXlen; i++)
		{
			for (j = 0; j < imgYlen; j++)
			{
				result = 0;
				flag = 0;
				//矩阵
				for (t_i = 0; t_i < 3 && !flag; t_i++)
				{
					for (t_j = 0; t_j < 3 && !flag; t_j++)
					{
						if (i - 1 + t_i < imgXlen && i - 1 + t_i >= 0 && j - 1 + t_j < imgYlen && j - 1 + t_j >= 0)
						{
							result += buffTmp[k][(i - 1 + t_i)*imgYlen + j - 1 + t_j] * matrix4[t_i][t_j];
						}
						else
						{
							flag = 1;
							result = 0;
						}
					}
				}
				buffTmp[k][i*imgYlen + j] = (GByte)result;
			}

		}
		poDstDS4->GetRasterBand(k + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//图像5
	cout << "----卷积核五-----" << endl;

	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
		//开始处理
		for (i = 0; i < imgXlen; i++)
		{
			for (j = 0; j < imgYlen; j++)
			{
				result = 0;
				flag = 0;
				//矩阵
				for (t_i = 0; t_i < 3 && !flag; t_i++)
				{
					for (t_j = 0; t_j < 3 && !flag; t_j++)
					{
						if (i - 1 + t_i < imgXlen && i - 1 + t_i >= 0 && j - 1 + t_j < imgYlen && j - 1 + t_j >= 0)
						{
							result += buffTmp[k][(i - 1 + t_i)*imgYlen + j - 1 + t_j] * matrix5[t_i][t_j];
						}
						else
						{
							flag = 1;
							result = 0;
						}
					}
				}
				
					result += 128;
				buffTmp[k][i*imgYlen + j] = (GByte)result;
			}

		}
		poDstDS5->GetRasterBand(k + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//图像6
	cout << "----卷积核六-----" << endl;

	for (k = 0; k < bandNum; k++)
	{
		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
		//开始处理
		for (i = 0; i < imgXlen; i++)
		{
			for (j = 0; j < imgYlen; j++)
			{
				result = 0;
				flag = 0;
				//矩阵
				for (t_i = 0; t_i < 5 && !flag; t_i++)
				{
					for (t_j = 0; t_j < 5 && !flag; t_j++)
					{
						if (i - 1 + t_i < imgXlen && i - 1 + t_i >= 0 && j - 1 + t_j < imgYlen && j - 1 + t_j >= 0)
						{
							result += buffTmp[k][(i - 1 + t_i)*imgYlen + j - 1 + t_j] * matrix6[t_i][t_j];
						}
						else
						{
							flag = 1;
							result = 0;
						}
					}
				}
				result /= 25;
				buffTmp[k][i*imgYlen + j] = (GByte)result;
			}

		}
		poDstDS6->GetRasterBand(k + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp[k], imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//释放内存
	CPLFree(buffTmp[0]);
	CPLFree(buffTmp[1]);
	CPLFree(buffTmp[2]);
	GDALClose(poSrcDS);
	GDALClose(poDstDS1);
	GDALClose(poDstDS2);
	GDALClose(poDstDS3);
	GDALClose(poDstDS4);
	GDALClose(poDstDS5);
	GDALClose(poDstDS6);
	system("pause");
	return  0;
}