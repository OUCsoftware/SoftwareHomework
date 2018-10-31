## ThirdDemo实验报告

姓名：王刚

学号：16020031075

#### **鉴于实验过程简单，不再赘述实验过程。本实验暴露了我自身的较多问题，因此，本着自我批评的原则，着重阐述实验过程中所遇到的问题以及解决方法。**

#### 实验中遇到的问题以及解决方法：

1. CPLFree()函数和GDALClose()函数的混淆。
   曾经把CPLMalloc()的指针用GDALClose()函数清除，导致编译出错。截图如下:

   ![](https://github.com/Histra/ThirdDemo/blob/master/ExperimentReportPic_01.png)

   注意，CPLFree()对应CPLMalloc();

2. 对于图像在计算机里面的行列存储混淆，导致调试程序花了2h。bug自己写的，还得自己debug。

3. 马虎大意，审题不明。
   对于superman.jpg中的RGB(x,y,z),应该同时满足三个条件。

   最初写的版本代码如下：

   ![](https://github.com/Histra/ThirdDemo/blob/master/ExperimentReportPic_03.png)

   导致结果如下：

   ![](https://github.com/Histra/ThirdDemo/blob/master/ExperimentReportPic_02.png)

   更改后结果如下：

   ![](https://github.com/Histra/ThirdDemo/blob/master/ExperimentReportPic_04.png)

#### 实验反思总结：

​	这个实验是第三个实验，暴露了自己的太多问题。调试bug调试到爆炸，总算是在老师的帮助和自己的努力下完成了这个实验。“吾日三省吾身”，一周没敲软件工程的代码，手便生了，这是在让人羞赧。但是，如何化羞赧为动力，去不断地精进自己的代码技能这才是关键。
​	细节决定成败啊，别再大意啦，少年！

​																		2018年10月23日晚21:21

