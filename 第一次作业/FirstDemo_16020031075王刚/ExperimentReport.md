### FirstDemo实验报告

姓名：王刚

学号：16020031075

#### 实验过程：

1. 打开Visual Studio2017，文件->新建->项目，选择

   ![1538733519755](https://github.com/Histra/FirstDemo/blob/master/1538733519755.png)

2. 把老师提供的代码敲一遍

3. 写注释并把程序读懂

4. 在源程序目录下新建data文件夹用于存放数据图片的结果图片。

   ![1538733734711](https://github.com/Histra/FirstDemo/blob/master/1538733734711.png)

5. 修改代码中的图片路径及名称：

   ![1538734051190](https://github.com/Histra/FirstDemo/blob/master/1538734051190.png)

6. 下载老师给的gdal文件夹、gdal_i.lib和gdal18.dll。把gdal文件夹和gdal_i.lib拷贝到项目目录下，把gdal18.dll拷贝到Release文件夹下。

   然后在代码快中添加如下代码：

   ```c++
   #include "./gdal/gdal_priv.h"
   #pragma comment(lib, "gdal_i.lib")
   ```

7. 调试后运行程序，结果如下：

   ![1538733887353](https://github.com/Histra/FirstDemo/blob/master/1538733887353.png)

   打开data文件夹：

   ![1538733963670](https://github.com/Histra/FirstDemo/blob/master/1538733963670.png)

8. 打开Input.jpg的属性界面，检查输出结果是否正确：

   ![1538734366523](https://github.com/Histra/FirstDemo/blob/master/1538734366523.png) 

   显然，结果正确。

#### 实验中遇到的问题以及解决方法：

1. 新建FirstDemo工程的时候，Visual Studio2017在FirstDemo.cpp文件中自动加入了stdafx.h这个头文件。注释掉后运行会提示错误。在网上百度后得知，这个头文件是标准应用程序框架的扩展，应该在程序中存在。
2. 在我看来，这个实验的难点在于学会使用GitHub以及读懂其界面上的英语单词。
3. 对于写实验报告来讲，实验报告中的图片应该是一个网址，询问老师后成功解决。

#### 实验反思总结：

​	本实验代码老师提供了，但是阅读的过程中还是感到了模棱两可，比如GDAL中的好多函数的使用都不熟悉，深深地感知到自己知识的浅薄。此外，对于GitHubDesktop的使用还是”大姑娘上花轿“，其实对于GitHub我用的并不多，这也是我涉足的盲区，不过还好现在还为时未晚。第一次使用Typora编辑.md文件写实验报告，感觉Typora这个编辑器很不错，希望以后多多练习。此外，GitHub界面上的英语是硬伤，因此，要加强英语的学习。希望有一天自己能够写出来像老师给出的例程一样的代码。