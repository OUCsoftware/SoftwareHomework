## 任务：  给图像加滤镜

### 1. 任务描述

![](http://ww1.sinaimg.cn/large/6deb72a3ly1fwo23bb045j20740740ty.jpg)

对于上面的图像（lena.jpg），使用GDAL编程，分别使用下面的核进行卷积，并观察效果，说明结果是哪一种滤波。

![](http://ww1.sinaimg.cn/large/6deb72a3ly1fwo2w4e7huj20go0ehabl.jpg)



![](http://ww1.sinaimg.cn/large/6deb72a3ly1fwo2x14c50j20gl05z0tf.jpg)

### 2. 核心代码及实验结果

#### （1）主函数核心代码

```c++
  //打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像宽高和波段数量
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽高分配内存
	inTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	outTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	for (i = 0; i < FILTERNUM; i++)
	{
		poDstDS[i] = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath[i], imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	}
	//波段的输入与输出
	for (j = 0; j < FILTERNUM; j++)
	{
		for (i = 0; i < bandNum; i++)
		{
			poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, inTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
			if (j == 0)
				filter1(inTmp, outTmp, imgXlen, imgYlen);
			else if (j == 1)
			{
				filter2(inTmp, outTmp, imgXlen, imgYlen);
			}
			else if (j == 2)
			{
				filter3(inTmp, outTmp, imgXlen, imgYlen);
			}
			else if (j == 3)
			{
				filter4(inTmp, outTmp, imgXlen, imgYlen);
			}
			else if (j == 4)
			{
				filter5(inTmp, outTmp, imgXlen, imgYlen);
			}
			else
			{
				filter6(inTmp, outTmp, imgXlen, imgYlen);
			}
			poDstDS[j]->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, outTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
			printf("...... band %d processing ......\n", i);
		}
	}
```
#### （2）Box Filter

卷积核：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj6gcpqwwj306a04zdfp.jpg)

核心代码：

```C++
int filter1(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j<imgYlen - 1; j++)
	{
		for (i = 1; i<imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (inTmp[(j - 1)*imgXlen + i] +
				inTmp[j*imgXlen + i - 1] +
				inTmp[j*imgXlen + i] +
				inTmp[j*imgXlen + i + 1] +
				inTmp[(j + 1)*imgXlen + i]) / 5.0f;
		}
	}
	return 0;
}
```

实验结果：

![](http://ww1.sinaimg.cn/large/006zdbpcly1fxj6kjtr5vj308x08xtch.jpg)

#### (2) Motion Filter

卷积核：

![](http://ww1.sinaimg.cn/large/006zdbpcly1fxj6q5o7c9j30bn08udfz.jpg)

核心代码：

```c++
int filter2(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 2; j < imgYlen - 2; j++)
	{
		for (i = 2; i < imgXlen - 2; i++)
		{
			outTmp[j*imgXlen + i] = (inTmp[(j - 2)*imgXlen + i - 2] +
				inTmp[(j - 1)*imgXlen + i - 1] +
				inTmp[j*imgXlen + i] +
				inTmp[(j + 1)*imgXlen + i + 1] +
				inTmp[(j + 2)*imgXlen + i + 2]) / 5.0f;
		}
	}
	return 0;
}
```

实验结果：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj6rpvdohj308y08xaa5.jpg)

#### (4) Edge Filter

卷积核：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj6sz9vncj304904awec.jpg)

核心代码：

```c++
int filter3(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j<imgYlen - 1; j++)
	{
		for (i = 1; i<imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (-inTmp[(j - 1)*imgXlen + i - 1]
				- inTmp[(j - 1)*imgXlen + i]
				- inTmp[(j - 1)*imgXlen + i + 1]
				- inTmp[j*imgXlen + i - 1]
				+ inTmp[j*imgXlen + i] * 8
				- inTmp[j*imgXlen + i + 1]
				- inTmp[(j + 1)*imgXlen + i - 1]
				- inTmp[(j + 1)*imgXlen + i]
				- inTmp[(j + 1)*imgXlen + i + 1]);
		}
	}
	return 0;
}
```

实验结果：

![](http://ww1.sinaimg.cn/large/006zdbpcly1fxj6uj8fmaj308w08vt92.jpg)

#### (4) Sharpen Filter

卷积核：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj6vwdtoxj3048047wec.jpg)

核心代码：

```c++
int filter4(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j < imgYlen - 1; j++)
	{
		for (i = 1; i < imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (-inTmp[(j - 1)*imgXlen + i - 1] -
				inTmp[(j - 1)*imgXlen + i] -
				inTmp[(j - 1)*imgXlen + i + 1] -
				inTmp[j*imgXlen + i - 1] +
				inTmp[j*imgXlen + i] * 8 -
				inTmp[j*imgXlen + i + 1] -
				inTmp[(j + 1)*imgXlen + i - 1] -
				inTmp[(j + 1)*imgXlen + i] -
				inTmp[(j + 1)*imgXlen + i + 1]);
		}
	}
	return 0;
}
```

实验结果：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj6wzbjeqj308x08wdgf.jpg)

#### (5)  Emboss Filter

卷积核：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj6yslaqgj3048048t8k.jpg)

核心代码：

```c++
int filter5(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j<imgYlen - 1; j++)
	{
		for (i = 1; i < imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (-inTmp[(j - 1)*imgXlen + i - 1] -
				inTmp[(j - 1)*imgXlen + i] -
				inTmp[j*imgXlen + i - 1] +
				inTmp[j*imgXlen + i + 1] +
				inTmp[(j + 1)*imgXlen + i] +
				inTmp[(j + 1)*imgXlen + i + 1]);
		}
	}
	return 0;
}
```

实验结果：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj6zwl9qkj308x08ydg3.jpg)

#### (6) Gauss Filter

卷积核：

![](http://ww1.sinaimg.cn/large/6deb72a3ly1fwo2x14c50j20gl05z0tf.jpg)

核心代码：

```
int filter6(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 2; j<imgYlen - 2; j++)
	{
		for (i = 2; i<imgXlen - 2; i++)
		{
			outTmp[j*imgXlen + i] = (0.0120*inTmp[(j - 2)*imgXlen + i - 2] +
				0.1253*inTmp[(j - 2)*imgXlen + i - 1] +
				0.2736*inTmp[(j - 2)*imgXlen + i] +
				0.1253*inTmp[(j - 2)*imgXlen + i + 1] +
				0.0120*inTmp[(j - 2)*imgXlen + i + 2] +
				0.1253*inTmp[(j - 1)*imgXlen + i - 2] +
				1.3054*inTmp[(j - 1)*imgXlen + i - 1] +
				2.8514*inTmp[(j - 1)*imgXlen + i] +
				1.3054*inTmp[(j - 1)*imgXlen + i + 1] +
				0.1253*inTmp[(j - 1)*imgXlen + i + 2] +
				0.2763*inTmp[j*imgXlen + i - 2] +
				2.8514*inTmp[j*imgXlen + i - 1] +
				6.2279*inTmp[j*imgXlen + i] +
				2.8514*inTmp[j*imgXlen + i + 1] +
				0.2763*inTmp[j*imgXlen + i + 2] +
				0.1253*inTmp[(j + 1)*imgXlen + i - 2] +
				1.3054*inTmp[(j + 1)*imgXlen + i - 1] +
				2.8514*inTmp[(j + 1)*imgXlen + i] +
				1.3054*inTmp[(j + 1)*imgXlen + i + 1] +
				0.1253*inTmp[(j + 1)*imgXlen + i + 2] +
				0.0120*inTmp[(j + 2)*imgXlen + i - 2] +
				0.1253*inTmp[(j + 2)*imgXlen + i - 1] +
				0.2736*inTmp[(j + 2)*imgXlen + i] +
				0.1253*inTmp[(j + 2)*imgXlen + i + 1] +
				0.0120*inTmp[(j + 2)*imgXlen + i + 2]) / 25.0f;
		}
	}
	return 0;
}
```

实验结果：

![](http://ww1.sinaimg.cn/large/006zdbpcgy1fxj8k5wtqtj308x08xwel.jpg)

### 3.结果总览

![](http://ww1.sinaimg.cn/large/006zdbpcly1fxj8ssup0nj30re043gm2.jpg)

### 3. 困难和解决办法

(1）第一次写的时候不能正确输出图片，在老师的讲解之后，把GDT_Byte改成GDT_Float32，转换一下类型就好了

(2）第二次尝试中，输出的图片效果不对。后来发现，我写的卷积函数都只有一个向量既作输入又作输出，就像这样👇

```c++
int filter1(float*Tmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j<imgYlen - 1; j++)
	{
		for (i = 1; i<imgXlen - 1; i++)
		{
			Tmp[j*imgXlen + i] = (Tmp[(j - 1)*imgXlen + i] +
				Tmp[j*imgXlen + i - 1] +
				Tmp[j*imgXlen + i] +
				Tmp[j*imgXlen + i + 1] +
				Tmp[(j + 1)*imgXlen + i]) / 5.0f;
		}
	}
	return 0;
}
```

这样写是有问题的，因为每一个点像素值的计算都是依赖于左边和上边的点的，这样的计算会改变每一个像素点的左边和上边的像素值，使得结果不正确。于是，我改成了输入和输出各一个向量。