Equipment test tool
将etc目录里的gui.conf移到/etc/test_gui/下
直接makefile编译生成main执行文件
在配置文件config中修改config.c增加或删除按钮节点。command为可执行程序，由相应模块控制向显示屏返回信息确认状态。
etc目录下提供了重启文件，需交叉编译。
