#include <iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

/*
�����ʼ��
*/
const double matrix1[3][3] =
{
	{0,1,0},
	{1,1,1},
	{0,1,0}
};

const double matrix2[5][5] =
{
	{1,0,0,0,0},
	{0,1,0,0,0},
	{0,0,1,0,0},
	{0,0,0,1,0},
	{0,0,0,0,1}
};


const double matrix3[3][3] =
{
	{-1,-1,-1},
	{-1,8,-1},
	{-1,-1,-1}
};

const double matrix4[3][3] =
{
	{-1,-1,-1},
	{-1,9,-1},
	{-1,-1,-1}
};

const double matrix5[3][3] =
{
	{-1,-1,0},
	{-1,0,1},
	{0,1,1}
};

const double matrix6[5][5] =
{
	{ 0.0120, 0.1253, 0.2736, 0.1253, 0.0120 },
	{ 0.1253, 1.3054, 2.8514, 1.3054, 0.1253 },
	{ 0.2736, 2.8514, 6.2279, 2.8514, 0.2736 },
	{ 0.1253, 1.3054, 2.8514, 1.3054, 0.1253 },
	{ 0.0120, 0.1253, 0.2736, 0.1253, 0.0120 }
};

//����
int N[6] = { 3,5,3,3,3,5 };
//Ҫ�˵Ĵ���
double mul[6] = { 0.2,0.2,1,1,1,1.0 / 25 };
//ͼ��ĸ߿�ȺͲ�����
int imgXlen, imgYlen, bandNum;

//��ʱ����
double temp3[3][3];
double temp5[5][5];

void calculate(int order, GDALDataset* input, GDALDataset* output, int N)
{
	
	for (int band = 0; band < bandNum; band++)
	{
		//��ʱ�ڴ�
		GByte* buffTmp;
		GByte* buffTmpw;
		//�����ڴ�
		buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
		buffTmpw = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
		input->GetRasterBand(band + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		for (int i = 0; i < imgYlen; i++)
		{
			for (int j = 0; j < imgXlen; j++ )
			{
				//����ڱ߽���Ϊ0
				if (N==3&&!(i > 0 && i < imgYlen - 1 && j>0 && j < imgXlen-1))
					buffTmpw[i*imgXlen + j] = (GByte)0;
				else if(N == 5 && !(i >= 2 && i <= imgYlen - 3 && j>=2 && j <= imgXlen-3))
					buffTmpw[i*imgXlen + j] = (GByte)0;
				else
				{
					double ans = 0;
					//�������Ϊ3
					if (N == 3)
					{
						for (int m = 0; m < 3; m++)
						{
							for (int n = 0; n < 3; n++)
							{
								temp3[m][n] = (double)buffTmp[(i-1+m)*imgXlen + j-1+n];
								//ȫ��������
								if (order == 1)
									ans += temp3[m][n] * matrix1[m][n];
								if (order == 3)
									ans += temp3[m][n] * matrix3[m][n];
								if (order == 4)
									ans += temp3[m][n] * matrix4[m][n];
								if (order == 5)
									ans += temp3[m][n] * matrix5[m][n];
							}
						}
					}
					//�������Ϊ5
					else if (N == 5)
					{
						for (int m = 0; m < 5; m++)
						{
							for (int n = 0; n < 5; n++)
							{
								temp5[m][n] = (double)buffTmp[(i - 2 + m)*imgXlen + j - 2 + n];
								//ȫ��������
								if (order == 2)
									ans += temp5[m][n] * matrix2[m][n];
								if (order == 6)
									ans += temp5[m][n] * matrix6[m][n];
							}
						}
					}
					if(order==5)
					ans += 128;
					ans *= mul[order - 1];
					if (ans < 0) ans = 0;
					else if (ans > 255) ans = 255;
					buffTmpw[i*imgXlen+j] = (GByte)ans;
				}//end
			}
		}
		output->GetRasterBand(band + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmpw, imgXlen, imgYlen, GDT_Byte, 0, 0);
		//����ڴ�
		CPLFree(buffTmp);
		CPLFree(buffTmpw);
	}
	
}


int main()
{
	//����ͼ��
	GDALDataset* input;
	//���ͼ��1-6
	GDALDataset* output1;
	GDALDataset* output2;
	GDALDataset* output3;
	GDALDataset* output4;
	GDALDataset* output5;
	GDALDataset* output6;
	//ͼ���·��
	const char* inputPath = "linda.jpg";
	const char* outputPath1 = "1.tif";
	const char* outputPath2 = "2.tif";
	const char* outputPath3 = "3.tif";
	const char* outputPath4 = "4.tif";
	const char* outputPath5 = "5.tif";
	const char* outputPath6 = "6.tif";

	/*------
	��ʼ����
	-----*/

	GDALAllRegister();//ע������
	input = (GDALDataset*)GDALOpenShared(inputPath, GA_ReadOnly);//�õ�ͼƬ
	imgXlen = input->GetRasterXSize();
	imgYlen = input->GetRasterYSize();
	bandNum = input->GetRasterCount();

	//����ͼ��
	output1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputPath1, imgXlen,
		imgYlen, bandNum, GDT_Byte, NULL);
	output2 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputPath2, imgXlen,
		imgYlen, bandNum, GDT_Byte, NULL);
	output3 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputPath3, imgXlen,
		imgYlen, bandNum, GDT_Byte, NULL);
	output4 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputPath4, imgXlen,
		imgYlen, bandNum, GDT_Byte, NULL);
	output5 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputPath5, imgXlen,
		imgYlen, bandNum, GDT_Byte, NULL);
	output6 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputPath6, imgXlen,
		imgYlen, bandNum, GDT_Byte, NULL);
	
	//��ʼ����
	calculate(1, input, output1, 3);
	calculate(2, input, output2, 5);
	calculate(3, input, output3, 3);
	calculate(4, input, output4, 3);
	calculate(5, input, output5, 3);
	calculate(6, input, output6, 5);
	
	//�ر�dataset
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
