## nekoHttpd 

一个基于muduo网络库的多线程异步HTTP服务器，致力于提供易用的服务端HTTP层支持

##### 项目结构



##### 依赖

安装命令基于ubuntu-18.04

- C++ boost

  ```bash
  $ sudo apt-get install libboost1.65-all-dev
  ```

- openssl 1.11.1h

    ```bash
  #下载源码并按照INSTALL文件进行安装
  $ ./config
  $ make 
  $ make test
  $ sudo make install
  ```
##### 安装

默认情况下CMAKE_INSTALL_PREFIX=/usr/local/

`libtinyHttpd.so`文件会安装到`${CMAKE_INSTALL_PREFIX}/lib/`

头文件安装到`${CMAKE_INSTALL_PREFIX}/include/tinyHttpd/`

 - 创建一个build文件夹并进入
   ```bash
	$ mkdir build && cd build
   ```
 - 使用cmake命令进行预编译
   ```bash
	$ cmake [本库的根CMakeLists.txt文件的目录]
   ```
 - 编译并安装
   ```bash
	$ make
   $ sudo make install
   ```

##### 使用
暂时见[tests](./tests)
