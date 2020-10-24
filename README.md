## nekoHttpd

> based on tinyHttpd(2019.10毕业设计所用依赖)
>
> 轻量的C++微服务框架，用于应付毕业设计里的http服务需求。
>
> 这个项目仅仅是httpserver项目的短期替代版本，在此后本项目会被移除 。

仅仅是一个简单的HTTP服务器，但可以通过重载路由函数实现微服务应用的实现
##### 依赖

（安装命令基于ubuntu-18.04，依赖项的其他版本请自行尝试)

- C++ boost 1.6

  ```bash
  $ apt-get install libboost1.65-all-dev
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
见[example](./example)文件夹。所使用到的配置文件见[conf](./conf)
