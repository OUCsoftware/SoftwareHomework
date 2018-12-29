## 任务： 遥感图像的分块处理  

### 1. 任务描述

​	使用上周学习的IHS方法，对宽幅遥感图像分别使用两种分块方式进行融合：

​	第一种方式：使用256 * 256 大小的块；

​	第二种方式：每块的宽度为图像宽度，高度为256像素。

​	比较两种分块方式的处理效率。

### 2. 核心代码

#### （1）第一种方式：使用256 * 256 大小的块

```C++
while (n < imgXlen / 256 + 1)
	{
		while (m < imgYlen / 256 + 1)
		{

			for (i = 0; i < bandNum; i++)
			{
				posrc1->GetRasterBand(i + 1)->RasterIO(GF_Read, 256 * (m - 1), 256 * (n - 1), 256, 256, bufftmp[i], 256, 256, GDT_Float32, 0, 0);
			}

			posrc2->GetRasterBand(1)->RasterIO(GF_Read, 256 * (m - 1), 256 * (n - 1), 256, 256, bufftmp2, 256, 256, GDT_Float32, 0, 0);
			for (i = 0; i < 256 * 256; i++)
			{
				bufftmpH[i] = -sqrt(2.0f) / 6.0f* bufftmp[0][i] - sqrt(2.0f) / 6.0f*bufftmp[1][i] + sqrt(2.0f) / 3.0f*bufftmp[2][i];
				bufftmpS[i] = 1.0f / sqrt(2.0f)* bufftmp[0][i] - 1 / sqrt(2.0f)*bufftmp[1][i];

				bufftmp[0][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] + 1.0f / sqrt(2.0f)*bufftmpS[i];
				bufftmp[1][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] - 1.0f / sqrt(2.0f)*bufftmpS[i];
				bufftmp[2][i] = bufftmp2[i] + sqrt(2.0f)*bufftmpH[i];

			}

			for (i = 0; i < 3; i++)
			{
				podes1->GetRasterBand(i + 1)->RasterIO(GF_Write, 256 * (m - 1), 256 * (n - 1), 256, 256, bufftmp[i], 256, 256, GDT_Float32, 0, 0);
			}
			m++;
		}
		n++;
	}
```



#### （2）第二种方式：每块的宽度为图像宽度，高度为256像素

```c++
while (n < imgXlen / 256 + 1)
	{
		for (i = 0; i < bandNum; i++)
		{
			posrc1->GetRasterBand(i + 1)->RasterIO(GF_Read, 256 * (n - 1), 0, 256, imgYlen, bufftmp[i], 256, imgYlen, GDT_Float32, 0, 0);
		}

		posrc2->GetRasterBand(1)->RasterIO(GF_Read, 256 * (n - 1), 0, 256, imgYlen, bufftmp2, 256, imgYlen, GDT_Float32, 0, 0);
		for (i = 0; i < imgYlen * 256; i++)
		{
			bufftmpH[i] = -sqrt(2.0f) / 6.0f* bufftmp[0][i] - sqrt(2.0f) / 6.0f*bufftmp[1][i] + sqrt(2.0f) / 3.0f*bufftmp[2][i];
			bufftmpS[i] = 1.0f / sqrt(2.0f)* bufftmp[0][i] - 1 / sqrt(2.0f)*bufftmp[1][i];

			bufftmp[0][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] + 1.0f / sqrt(2.0f)*bufftmpS[i];
			bufftmp[1][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] - 1.0f / sqrt(2.0f)*bufftmpS[i];
			bufftmp[2][i] = bufftmp2[i] + sqrt(2.0f)*bufftmpH[i];

		}

		for (i = 0; i < 3; i++)
		{
			podes1->GetRasterBand(i + 1)->RasterIO(GF_Write, 256 * (n - 1), 0, 256, imgYlen, bufftmp[i], 256, imgYlen, GDT_Float32, 0, 0);
		}
		n++;

	}
```


### 3.实验结果

![](http://ww1.sinaimg.cn/large/006zdbpcly1fyi9ytn6nij31380u0tia.jpg)

​	图像明显变清晰了许多。

### 4. 心得体会

​	实验六和实验五很像，方法基本相同。

​	方法一明显比方法二慢很多，分块存储的话还是要用方法二。
