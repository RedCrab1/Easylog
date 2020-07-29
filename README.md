20200729
# Easylog
version "V0.0.1.1"

实现一种C++多类实例多线程情况下安全写日志的方式．
最初产生写该日志功能考虑的是C工程向C++工程转换时日志不容易改造，所以提供的是C风格的写日志方式．
后续会考虑再提供C++风格的写日志方式.

实现功能：
　１．对输出log进行分级，分为ERROR, WARN, INFO, DEBUG, TRACE　五级，根据设定的log重要等级对log进行输出，
    例如设定等级为INFO, 则重要等级大于等于INFO的ERROR, WARN, INFO log全都输出．　
 2. 采用单例模式，获得唯一的log对象指针，保证C++不同的类实例可以将log写入同一个文件，并保证线程安全．　
 3. 采用工厂模式，可以设定输出日志到不同的log文件，根据logID区分，不同logID输出到不同文件文件夹下，以时间日期为文件名，
 　　　能保证日志文件不会过大，　也不会被覆盖．


 编译方法（测试）：
 　　g++ easylog.cpp main.cpp -lpthread -std=c++11 -o test

　使用时可以直接将easylog.cpp 和easylog.h拷贝到自己工程目录下．

