# PPG_sharer
基于蓝牙的 PPG 信号分享工程

## 开发计划

- [x] 1、实现 max30102 的设备驱动
- [x] 2、设计研究采样信号的最佳参数
- [ ] 3、实现蓝牙的数据分享
- [ ] 4、PPG 信号的分析
- [x] 5、上位机的设计

## 使用方法

1、 下载并安装 IDF 开发环境，[安装教程](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32c3/get-started/index.html)

2、 克隆本项目

```
git clone https://github.com/gengyuchao/PPG_sharer.git
```

3、编译和下载
```
idf.py set-target esp32c3
idf.py flash monitor
```

4、输入命令：

```
max30102 -r fifo 100
```
这部分内容还没有完成，只是设置了测试命令。可以通过 `-h` 查看帮助信息。
```
esp32c3> max30102 -h
max30102 -i
        show max30102 chip and driver information.
max30102 -h
        show max30102 help.
max30102 -p
        show max30102 pin connections of the current board.
max30102 -t reg
        run max30102 register test.
max30102 -t fifo <times>
        run max30102 fifo test.times means test times.
max30102 -r fifo <times>
        run max30102 fifo mytest.times means read times.

```


## 学习资源
[开发记录](./Learner.md)
## 资源
1、本项目基于 [ESP32-C3](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32c3/get-started/index.html) 开发, [datasheet](https://www.espressif.com.cn/sites/default/files/documentation/)esp32-c3_datasheet_cn.pdf)

2、本项目使用的光学传感器为 MAX30102 [datasheet](https://pdf1.alldatasheetcn.com/datasheet-pdf/view/859400/MAXIM/MAX30102.html)

3、本项目的光学传感器驱动基于 [LibDriver MAX30102](https://github.com/hepingood/max30102) 修改 感谢原作者开源了非常棒的硬件驱动库。
