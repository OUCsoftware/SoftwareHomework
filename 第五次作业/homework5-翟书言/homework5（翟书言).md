## 任务：  IHS遥感图像融合

### 1. 任务描述

使用IHS变换，对下面图像进行融合。

多光谱图像![](http://ww1.sinaimg.cn/large/006zdbpcly1fyhqyt1oevj30n90n9acp.jpg)

全色图像 ![](http://ww1.sinaimg.cn/large/006zdbpcly1fyhr0rqqnaj30na0n8n0q.jpg)

### 2. 核心代码及实验结果

#### （1）主函数核心代码

```c++
// RGB ==> IHS 正变换
	for (i = 0; i < 3; i++)
	{
		poSrcMul->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, mulXlen, mulYlen, mulTmp[i], mulXlen, mulYlen, GDT_Float32, 0, 0);
		posMatrix(mulTmp, panDstTmp, mulXlen, mulYlen);
	}
	poSrcPan->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, mulXlen, mulYlen, panTmp, mulXlen, mulYlen, GDT_Float32, 0, 0);

	//用 PAN 替换 I 分量
	for(i=0;i<mulXlen;i++)
		for (j = 0; j < mulYlen; j++)
		{
			panDstTmp[0][i*mulXlen + j] = panTmp[i*mulXlen + j];
		}
	
	// IHS ==> RGB 逆变换
	negMatrix(panDstTmp, dstTmp, mulXlen, mulYlen);

	//写入目标地址
	for (i = 0; i < 3; i++)
	{
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, mulXlen, mulYlen, dstTmp[i], mulXlen, mulYlen, GDT_Float32, 0, 0);
	}
```
#### （2）RGB和IHS的正变换和逆变换函数

```C++
//正变换函数
int posMatrix(float*inTmp[3], float*outTmp[3], int imgXlen, int imgYlen)  
{
	int i, j;

	float tran[3][3] = { {1.0f / 3.0f,        1.0f / 3.0f,        1.0f / 3.0f      },
						  {-sqrt(2.0f) / 6.0f, -sqrt(2.0f) / 6.0f, sqrt(2.0f) / 3.0f},
						  {1.0f / sqrt(2.0f),  -1.0f / sqrt(2.0f), 0          } };

	for (i = 1; i < imgXlen - 1; i++)
		for (j = 1; j < imgYlen - 1; j++)
		{
			outTmp[0][i*imgXlen + j] = tran[0][0] * inTmp[0][i*imgXlen + j] + tran[0][1] * inTmp[1][i*imgXlen + j] + tran[0][2] * inTmp[2][i*imgXlen + j];
			outTmp[1][i*imgXlen + j] = tran[1][0] * inTmp[0][i*imgXlen + j] + tran[1][1] * inTmp[1][i*imgXlen + j] + tran[1][2] * inTmp[2][i*imgXlen + j];
			outTmp[2][i*imgXlen + j] = tran[2][0] * inTmp[0][i*imgXlen + j] + tran[2][1] * inTmp[1][i*imgXlen + j] + tran[2][2] * inTmp[2][i*imgXlen + j];
		}
	return 0;
}

//逆变换函数
int negMatrix(float*inTmp[3], float*outTmp[3], int imgXlen, int imgYlen)   
{
	int i, j;
	float tran[3][3] = { {1.0f, -1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f)},
	{1.0f, -1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f) },
	{	1.0f, sqrt(2.0f), 0} };
	for (i = 1; i < imgXlen - 1; i++)
		for (j = 1; j < imgYlen - 1; j++)
		{
			outTmp[0][i*imgXlen + j] = tran[0][0] * inTmp[0][i*imgXlen + j] + tran[0][1] * inTmp[1][i*imgXlen + j] + tran[0][2] * inTmp[2][i*imgXlen + j];
			outTmp[1][i*imgXlen + j] = tran[1][0] * inTmp[0][i*imgXlen + j] + tran[1][1] * inTmp[1][i*imgXlen + j] + tran[1][2] * inTmp[2][i*imgXlen + j];
			outTmp[2][i*imgXlen + j] = tran[2][0] * inTmp[0][i*imgXlen + j] + tran[2][1] * inTmp[1][i*imgXlen + j] + tran[2][2] * inTmp[2][i*imgXlen + j];
		}
	return 0;
}
```



### 3.实验结果

![](http://ww1.sinaimg.cn/large/006zdbpcly1fyhr5iqztaj30or0ore81.jpg)

### 4. 心得体会

​	不难看出，进行融合后的图像比原多光谱图像清晰许多，又比全色图像颜色丰富。图像融合技术得到的结果结合了多光谱图像和全色图像的优点和缺点，呈现出清晰的彩色图像。然而也有一定的不足。融合后的图像相比原多光谱图像清晰度增加，但是颜色也发生改变了，和原图有很大的不同。

​	通过本次实验，我掌握了一种图像增强的方法，提高遥感图像的清晰度。我也了解了除了RGB之外的另一种图像表示模型IHS，并学会了两种模型的转换。