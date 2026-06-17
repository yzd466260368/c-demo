#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    /*
    常用类型
    time_t      // 保存时间戳
    struct tm   // 保存年月日时分秒等结构化时间
    clock_t     // 程序运行时间计时
    常用函数
    time()        // 获取当前时间戳
    localtime()   // 转成本地时间
    gmtime()      // 转成 UTC 时间
    mktime()      // struct tm 转 time_t
    strftime()    // 格式化时间字符串
    difftime()    // 计算两个时间差
    clock()       // 获取程序运行 CPU 时间
    */

    // time_t now;
    // struct tm *local;

    // time(&now);
    // local = localtime(&now);

    // printf("当前时间：%04d-%02d-%02d %02d:%02d:%02d\n",
    //        local->tm_year + 1900,
    //        local->tm_mon + 1,
    //        local->tm_mday,
    //        local->tm_hour,
    //        local->tm_min,
    //        local->tm_sec);

    char buf[64];
    time_t now = time(NULL);            // 获取系统当前时间戳
    struct tm *local = localtime(&now); // 转为本地时间结构体

    // 格式：年-月-日 时:分:秒
    // strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    strftime(buf, sizeof(buf), "%Y-%m-%d", local);
    printf("当前时间：%s\n", buf);

    return 0;
}
