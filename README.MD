# 电路故障波形生成终端

这个软件是一个项目中的服务器部分，负责处理硬件部分的网络请求、故障方案编辑、分发，故障方案存储以及故障波形编辑等工作。在技术上主要使用了Qt中的`数据库`、`网络`、`绘图`、`QSS`等技术。界面设计从简，主要颜色搭配不是很符合审美观，但是展示了Qt界面自定义的灵活性和可能性。

## 重点技术

- Qt中使用`SQLite`。外键关联在设计Model的时候可以从`QSqlRelationalTableModel`继承，这样外键所在表的数据更新时，View上面的数据能够自动更新。
- `Socket`网络编程。软件部分需要实现网络服务器的功能，所以本质上需要利用socket实现服务器，以完成硬件部分的数据请求。
- `多线程`编程。服务器部分的设计是这样的：每当收到一个网络连接，即启动一个后台线程处理该对应的请求。由于在同一时刻发请求只可能有一个，并发度并不高。因此这样的设计可以较大的提高程序的计算性能。
- `QSS`的使用。在QSS的使用上还是有所有欠缺，有些地方的设计感觉山寨风较浓，还需继续沉淀。

## 软件截图

- 登录界面

![login.png](https://raw.githubusercontent.com/csuft/QCircuitFaultEditor/master/screenshots/login.png "使用前先登录")

- 主窗体

![login.png](https://raw.githubusercontent.com/csuft/QCircuitFaultEditor/master/screenshots/windowa.png "主窗体")

- 创建新的故障方案

![login.png](https://raw.githubusercontent.com/csuft/QCircuitFaultEditor/master/screenshots/windowb.png "使用默认配置创建新故障方案")

- 编辑方案

![login.png](https://raw.githubusercontent.com/csuft/QCircuitFaultEditor/master/screenshots/windowc.png "编辑方案中的结点参数")

- 设置波形参数

![login.png](https://raw.githubusercontent.com/csuft/QCircuitFaultEditor/master/screenshots/windowd.png "设置波形参数")

- 波形设置和添加新方案

![login.png](https://raw.githubusercontent.com/csuft/QCircuitFaultEditor/master/screenshots/wave_settings.png "波形参数设置")
![login.png](https://raw.githubusercontent.com/csuft/QCircuitFaultEditor/master/screenshots/new_solution.png "添加故障方案")

## 更多资料

请参考[http://www.cnblogs.com/csuftzzk](http://www.cnblogs.com/csuftzzk "去博客园看看")