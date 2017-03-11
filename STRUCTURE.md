文件结构
----

SDK_BLE 根目录（目前的repository名字，待修改）
  |
  +- doc 文档
  |
  +- example 示例
  |
  +- export SDK产生的动态链接库
  |  |
  |  +- (linux, windows, ios, osx, android, etc)
  |
  +- external 项目中使用的第三方库（对外发布时只需按其license要求来决定是否提供）
  |
  +- make 各开发环境的项目文件，只针对SDK和test。其他如platform和example的项目文件应与其代码在同一目录。不用“build”是因为它被包含在.gitignore里并且不宜移除。
  |  |
  |  +- (vs2013, g++, etc)
  | 
  +- platform 针对各开发平台做的porting
  |  |
  |  +- (Unity3D, python, js, html5, etc)
  |
  +- src 项目主代码，不包括测试代码
  |  |
  |  +- ble 现在的libSDK
  |  |
  |  +- common 内部使用的公共模块，比如logutil，以及内部使用的types。建议不含cpp文件，如有实现部分用hpp替代。
  |  |
  |  +- gforce C++封装的SDK代码
  |  |
  |  +- include 对外导出的头文件
  |  |  |
  |  |  +- utils 各种types、os abs layer等，不包括内部使用的如logutil
  |  |
  |  +- npi 现在的NPI
  |  |
  |  +- npiutil 现在的SDK_BLE_GUI
  |
  +- test 所有的测试代码
  
 注：
 1、除了根目录（也就是repository的名字）可以有大写之外，所有文件夹名称一律小写。
 2、内部各模块如无特别需求，应该不要再区分子模块。
 3、参考了一些著名的opensource项目，内部各模块如无特殊需求，不要创建目录（包括inc/src），所有的文件都直接放在模块目录下。
 