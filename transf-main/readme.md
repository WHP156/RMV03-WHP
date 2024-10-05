# 任务概要

本任务要求大家在本程序的main函数中的“代码区"中书写代码（可以额外添加其他程序文件），识别Mat src图像中的：

![img](./image/target.png)

![img](./image/R.png)

并使用cv::circle在长方形与R的中间绘制一个圆形，同时计算单次循环识别的时间(这一点并不十分关键，毕竟和设备有关)

其中图像中的风车（能量机关）将以正弦速度旋转m，同时为了考虑现实中的观测噪声，我们为其添加了一定的噪声，其公式大致为：

$$
v=A\sin{(\omega t + \alpha)}+b
$$

为大家提供三档要求：

- 识别能量机关标靶
- **拟合**能量机关旋转各参数，且已知参数范围（查看源码获得参数）
- **拟合**能量机关旋转各参数（请勿直接查看源码获得参数）

我们会检查大家的代码以确定大家的到底处于哪一档






## Self Presentation
1. I tired to install ceres and eigen3 by myself.
Though I had some problems,but thanks to [CSDN](https://blog.csdn.net/weixin_47421410/article/details/134958162?ops_request_misc=%257B%2522request%255Fid%2522%253A%252274844729-8339-4910-8BBA-BAC82BC75E25%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=74844729-8339-4910-8BBA-BAC82BC75E25&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-134958162-null-null.142^v100^control&utm_term=ceres&spm=1018.2226.3001.4187).I have installed it.
2. I had a little trouble to identify R and Hammer.
I didn't know *matchTemplet* this function before,so I can't identify R correctly,but I searched it online,find [ZHIHU](https://zhuanlan.zhihu.com/p/545654329?utm_psn=1826104517382500352) this page,which helped me most.
3. I also have no idea how to calculate the speed.
But I saw a photo in our QQ.like![alt text](1.jpg),which made me understand that I can use mathmatic to simplify my computers calculation.And it turned out to be right.