/*
 * div.c  --  除法模块
 * 学号：20262814
 *
 * 函数名用 divide 而不是 div：
 * C 标准库 <stdlib.h> 已经声明了 div_t div(int, int)，
 * 同名会发生冲突（C2371 重定义），所以这里改名为 divide。
 */
#include "mymath.h"

MYMATH_API int divide(int a, int b)
{
    return a / b;   /* 整数除法，调用时保证 b != 0 */
}
