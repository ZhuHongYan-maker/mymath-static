/*
 * main.c  --  调用 mymath.dll 的控制台程序
 * 学号：20262814
 *
 * 链接方式：隐式链接（导入库 mymath.lib + 运行时 mymath.dll）
 *   1. mymath.h、mymath.lib 放在本工程目录（或设置好包含目录 / 库目录）
 *   2. mymath.dll 放在 UseMathDll.exe 同一目录
 *   3. 下面的 #pragma comment 自动让链接器链接 mymath.lib
 */
#include <stdio.h>
#include "mymath.h"

#pragma comment(lib, "mymath.lib")

int main(void)
{
    int a = 20262814;   /* 学号，输出后便于截图核对 */
    int b = 10;

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    printf("%d+%d=%d\n", a, b, add(a, b));
    printf("%d-%d=%d\n", a, b, sub(a, b));
    printf("%d*%d=%d\n", a, b, mul(a, b));
    printf("%d/%d=%d\n", a, b, divide(a, b));

    return 0;
}
