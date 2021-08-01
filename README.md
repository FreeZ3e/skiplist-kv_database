# skiplist-kv data base

#### 介绍
基于跳表的轻量级键值对数据库

#### 架构
该数据库共有三层模块:
1. data_base : 数据库用户层.对command进行包装及收集指令.用户只需创建data_base对象即可使用数据库.
2. command : 指令层.对指令进行判断及对skip_list进行管理和操作.
3. skip_list : 数据结构底层.

#### 使用说明
如demo.cpp,只要导入data_base.hpp即可创建数据库对象并使用,使用help命令查看完整指令.