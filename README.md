# 上海交通大学 RoboMaster2020赛季 视觉代码

本代码为上海交通大学RoboMaster2020赛季步兵视觉代码（机器学习版本），整体代码框架由[上海交通大学RoboMaster2019赛季视觉代码](https://github.com/xinyang-go/SJTU-RM-CV-2019)改进而来。通过**模块解耦**，**统一错误处理**，**统一接口**等手段，使得代码结构更加清晰。可以直接删除能量机关算法代码以及main函数能量机关注册相关代码，移植到哨兵或英雄上。

相较于19赛季代码，本代码**支持多种后端识别算法**（OpenCV图形学方法，Torch机器学习方法，TensorRT机器学习方法）。只需要修改cmakelist.txt中定义的编译目标为对应的算法方式即可完成切换。

---
