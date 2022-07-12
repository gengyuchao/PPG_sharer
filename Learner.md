
## 1、实现 max30102 的设备驱动

最近发现了一个 [max30102 的开源设备驱动库](https://github.com/hepingood/max30102),代码风格我很喜欢，就将我开发设备的驱动更换为这个库。

### 移植到 esp32c3

移植 IIC 驱动到 interface 

#### 历史记录
max30102 原本有一个开源驱动 [SparkFun_MAX3010x_Sensor_Library](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library) 是基于 C++ 和 Arduino 的开源驱动。在 Arduino 开发比较友好。但是本次开发的目标是在 ESP-IDF 框架上进行开发，所以最终没有选用此方案。

对这个工程我做了一些探究，在此记录一下：
想要使用 `SparkFun_MAX3010x_Sensor_Library` 驱动需要在 IDF 中实现 Arduino Wire(IIC) 驱动，对此驱动的相关移植我已经在 [Wire库](https://github.com/gengyuchao/GYC_components/tree/master/esp32c3/Wire_test/components/Wire) 中实现。 
对应 `SparkFun_MAX3010x_Sensor_Library` 的移植也在 [max30102](https://github.com/gengyuchao/GYC_components/tree/master/esp32c3/max30102)

## 2、设计研究采样信号的最佳参数
