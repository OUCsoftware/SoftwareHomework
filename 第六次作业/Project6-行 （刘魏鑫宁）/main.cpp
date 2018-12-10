#include <iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main()
{
	/*
	initial
	*/
	//��ɫ��ͼ��
	GDALDataset* poSrcThumDs;
	//����ͼ��
	GDALDataset* poSrcPanDs;
	//���ͼ��
	GDALDataset* poDstDs;

	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen, bandNum;
	//����ͼ���·��
	const char* srcThumPath = "Mul_large.tif";
	const char* srcPanPath = "Pan_large.tif";
	//���ͼ���·��
	const char* DstPath = "beauty.tif";
	int i, j, k;
	const int CutYlen = 256;//Ҫ�ضϵĸ߶�
	int LeftYlen;//����Ҫ�ضϵĸ߶ȣ�����ʣ�µ�

	//ͼ���ڴ洢��
	float* trans_in_R;
	float* trans_in_G;
	float* trans_in_B;
	//tran1�Ľ��
	float* trans_out_I;
	float* trans_out_H;
	float* trans_out_S;
	//tran2�Ľ��
	float* trans_res_R;
	float* trans_res_G;
	float* trans_res_B;
	//��ɫͼ��
	float* bufPan;

	//----RGB ==> IHS ���任����----
	float tran1[3][3] = {
	{1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f},
	{-sqrt(2.0f) / 6.0f, -sqrt(2.0f) / 6.0f, sqrt(2.0f) / 3.0f},
	{1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f), 0 } };
	// IHS == > RGB ��任����
	float tran2[3][3] = {
	{1.0f, -1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f)},
	{1.0f, -1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f)},
	{1, sqrt(2.0f), 0} };

	/*
	����ͼ������ر��[1:3*xlen*ylen]
	*/
	//����
	GDALAllRegister();

	//�򿪲�����ͼ��
	poSrcThumDs = (GDALDataset*)GDALOpenShared(srcThumPath, GA_ReadOnly);
	poSrcPanDs = (GDALDataset*)GDALOpenShared(srcPanPath, GA_ReadOnly);
	//��ȡͼ���ȣ��߶ȺͲ�����
	imgXlen = poSrcThumDs->GetRasterXSize();
	imgYlen = poSrcThumDs->GetRasterYSize();
	bandNum = poSrcThumDs->GetRasterCount();
	cout << imgXlen << " " << imgYlen << " " << endl;
	//�������ͼ��
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		DstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL
	);

	//����ͼ��Ŀ�Ⱥ͸߶ȷ����ڴ�
	bufPan = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	//��ɫ��ͼ�������ͨ��
	trans_in_R = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	trans_in_G = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	trans_in_B = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	//I,H,S
	trans_out_I = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	trans_out_H = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	trans_out_S = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	//�������RGB
	trans_res_R = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	trans_res_G = (float*)CPLMalloc(imgXlen*CutYlen * sizeof(float));
	trans_res_B = (float*)CPLMalloc(imgXlen*CutYlen* sizeof(float));
	//�����ɫ��ͼ������

	for (k = 0; k < (imgYlen / CutYlen) +1; k++)
	{
		if ((k + 1) * CutYlen < imgYlen)
		{
			LeftYlen = CutYlen;
		}
		else
		{
			LeftYlen = imgYlen - k * CutYlen;
		}
			poSrcThumDs->GetRasterBand(1)->RasterIO(GF_Read,
				0, k * CutYlen,  imgXlen, LeftYlen, trans_in_R, imgXlen, LeftYlen, GDT_Float32, 0, 0);
			poSrcThumDs->GetRasterBand(2)->RasterIO(GF_Read,
				 0, k * CutYlen, imgXlen, LeftYlen, trans_in_G, imgXlen, LeftYlen, GDT_Float32, 0, 0);
			poSrcThumDs->GetRasterBand(3)->RasterIO(GF_Read,
				0, k * CutYlen,  imgXlen, LeftYlen, trans_in_B, imgXlen, LeftYlen, GDT_Float32, 0, 0);


		//���� RGB ==> IHS ���任
		for (i = 0; i < LeftYlen; i++)
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

		//ΪbufPan�ĵ�ɫͨ����ֵ
		poSrcPanDs->GetRasterBand(1)->RasterIO(GF_Read,
			0, k*CutYlen, imgXlen, LeftYlen, bufPan, imgXlen, LeftYlen, GDT_Float32, 0, 0);



		//�滻I����
		for (j = 0; j < LeftYlen; j++)
		{
			for (i = 0; i < imgXlen; i++)
			{
				trans_out_I[j*imgXlen + i] = bufPan[j*imgXlen + i];
			}
		}


		//���� IHS ==> RGB ���任
		for (i = 0; i < LeftYlen; i++)
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

		//�ѱ任������浽 im_fus ��



		//д��
		poDstDs->GetRasterBand(1)->RasterIO(GF_Write,
			0, k*CutYlen, imgXlen, LeftYlen, trans_res_R, imgXlen, LeftYlen, GDT_Float32, 0, 0);
		poDstDs->GetRasterBand(2)->RasterIO(GF_Write,
			0, k*CutYlen, imgXlen, LeftYlen, trans_res_G, imgXlen, LeftYlen, GDT_Float32, 0, 0);
		poDstDs->GetRasterBand(3)->RasterIO(GF_Write,
			0, k*CutYlen, imgXlen, LeftYlen, trans_res_B, imgXlen, LeftYlen, GDT_Float32, 0, 0);

	}


	

	




	//����ڴ�
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

	//������ݼ�
	GDALClose(poDstDs);
	GDALClose(poSrcPanDs);
	GDALClose(poSrcThumDs);
	system("PAUSE");

	return 0;
}
