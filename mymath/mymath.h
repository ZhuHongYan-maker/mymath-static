/*
 * mymath.h  --  mymath.dll 的导出 / 导入声明
 * 学号：20262814
 *
 * 生成 DLL 的工程里必须定义宏 MYMATH_EXPORTS：
 *   DLL 工程   -> MYMATH_API 展开为 __declspec(dllexport)
 *   调用方工程 -> MYMATH_API 展开为 __declspec(dllimport)
 */
#ifndef MYMATH_H
#define MYMATH_H

#ifdef MYMATH_EXPORTS
#define MYMATH_API __declspec(dllexport)
#else
#define MYMATH_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

MYMATH_API int add(int a, int b);
MYMATH_API int sub(int a, int b);
MYMATH_API int mul(int a, int b);
MYMATH_API int divide(int a, int b);

#ifdef __cplusplus
}
#endif

#endif /* MYMATH_H */
