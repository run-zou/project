项目描述：
  搭建了一个Web服务器,其主要的功能是管理学生信息的录入,以及增删改查等等；该服务还可以支持简单的GET和POST方法.
  项目主要使用C语言实现，使用了数据库存储学生信息，采用了epoll，libevent等等实现高并发的处理请求.
  使用了dup2，exec，pthread，fork等等函数，使用了简单的html
  
  2016.9.2 添加了线程池支持高并发问题
