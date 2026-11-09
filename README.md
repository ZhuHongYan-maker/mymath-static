# mymath.dll 动态链接库实验（学号 20262814）

把 `add / sub / mul / div` 四个运算模块编译成 **mymath.dll**，再由 `main.c` 通过导入库
`mymath.lib` 调用，验证动态链接库的「生成」与「使用」。

- 学号：**20262814**（已写进每个源文件的注释，并作为控制台输出 `a` 的值，截图时可直接核对）
- 环境：Visual Studio 2017，Debug | Win32（x86），C 语言
- 交给别人使用时只需要 `mymath.dll` + `mymath.lib` + `mymath.h` 三个文件，**不需要给 .c 源码**

## 1. 运行结果

延续第二周 main.c 的结构（a = 学号，b = 10），运行后控制台输出：

```text
a = 20262814
b = 10
20262814+10=20262824
20262814-10=20262804
20262814*10=202628140
20262814/10=2026281
```

## 2. 目录结构

```text
.
├─ README.md
├─ .gitignore
├─ mymath/               # mymath.dll 工程源码
│  ├─ mymath.h           # 导出/导入声明（MYMATH_API）
│  ├─ add.c
│  ├─ sub.c
│  ├─ mul.c
│  └─ div.c
└─ UseMathDll/           # 调用 mymath.dll 的控制台工程
   └─ main.c
```

## 3. 在 VS2017 中生成 mymath.dll

### 3.1 新建工程

1. 文件 → 新建 → 项目 → **Visual C++ → 空项目**，名称 `mymath`，位置自定（例如 `D:\lab3`）。
   （列表里找不到「空项目」时，用 Visual C++ → Windows 桌面 → Windows 桌面向导，类型选「控制台应用程序」并勾选「空项目」。）
2. 把工具栏的解决方案平台切到 **x86**（也可以选 x64，但第 4 步的调用工程必须一致）。
3. 右键项目 → 属性 → 配置属性 → 常规 → **配置类型**：改成 **动态库(.dll)**。
4. 把本仓库 `mymath` 目录下的 5 个文件复制到工程目录，右键项目 → 添加 → 现有项，全部加入。
5. 右键项目 → 属性 → C/C++ → 预处理器 → **预处理器定义**：追加 `MYMATH_EXPORTS`
   （原有的 `_DEBUG` 等保留，用分号隔开，例如 `_DEBUG;MYMATH_EXPORTS`）。
6. 生成 → 生成解决方案。

> 为什么要 `MYMATH_EXPORTS`：`mymath.h` 用这个宏区分「导出」和「导入」。
> 不定义它，函数不会被导出，`dumpbin /exports mymath.dll` 会是空的，调用方链接时报 LNK2019。

生成成功后「输出」窗口最后几行类似：

```text
mymath.vcxproj -> D:\lab3\Debug\mymath.dll
```

同一个目录里还会生成导入库 **mymath.lib**（调用方链接时用）。

> 如果用的是「动态链接库(DLL)」模板：模板会自带 `dllmain.cpp / pch.cpp / pch.h / framework.h`，
> 保留 `dllmain.cpp`、删掉模板的示例函数文件即可；同时到 C/C++ → 预编译头 选「**不使用预编译头**」，
> 否则新加的 .c 文件会报 C1010。仍然要加 `MYMATH_EXPORTS`。

### 3.2 验证导出（可选）

打开「VS2017 开发者命令提示」，执行：

```bat
dumpbin /exports mymath.dll
```

能看到 `add`、`sub`、`mul`、`divide` 四个导出函数即可。

## 4. main.c 如何使用 mymath.dll

### 4.1 隐式链接（本仓库采用）

1. 右键解决方案 → 添加 → 新建项目 → **Visual C++ → 空项目**，名称 `UseMathDll`，平台同样是 x86。
2. 把 `UseMathDll\main.c` 加入工程；再把 `mymath\mymath.h` 复制到本工程目录
   （或者在 项目属性 → C/C++ → 常规 → 附加包含目录 里填 `mymath.h` 所在目录）。
3. 把生成的 `mymath.lib` 复制到本工程目录；把 `mymath.dll` 复制到 **UseMathDll.exe 所在目录**。
4. 生成并运行（F5）。

`main.c` 里已经写了：

```c
#pragma comment(lib, "mymath.lib")
```

所以不需要再手动填「附加依赖项」；只要 `mymath.lib` 在工程目录，链接器就能找到。
如果提示 LNK1104 找不到 mymath.lib，就在 项目属性 → 链接器 → 常规 → **附加库目录** 里写上它所在的文件夹。

> 两个工程在**同一个解决方案**里时，VS2017 的 Win32 工程默认都输出到 `<解决方案目录>\Debug`，
> `mymath.dll` 天然就在 `UseMathDll.exe` 旁边，通常不用手动复制；只有分开存放时才需要复制。
> 运行时 Windows 会先在 exe 所在目录找 dll，所以 **dll 必须和 exe 在一起**（或位于 PATH 中）。

### 4.2 显式链接（选做）

用 `LoadLibrary` + `GetProcAddress` 在运行时加载，不需要 .lib，示例见文末附录。

## 5. 条件断点（作业要求，截图必须体现）

1. 右键 **UseMathDll** 项目 → **设为启动项目**（否则 F5 启动的是 DLL 工程，会提示找不到可执行文件）。
2. 打开 mymath 工程的 `add.c`，在 `return a + b;` 这一行按 **F9** 下断点。
3. 右键这个断点 → **条件...** → 勾选「条件」→ 输入 `a == 20262814`（或 `b == 10`）→ 确定。
4. 按 **F5** 调试运行，程序会在条件成立时停在这一行（两个工程在同一解决方案，断点在 DLL 里也命中）。
5. 打开 调试 → 窗口 → **局部变量**、**调用堆栈**，确认 `a = 20262814`、`b = 10`，
   调用堆栈是 `add ← main`（mymath.dll!add ← UseMathDll.exe!main）。
6. 此时截图（这张就是「条件断点」那张）。

## 6. 提交截图清单（全屏、含学号、至少 3 张）

| 序号 | 截图内容 | 画面里要能看到 |
| --- | --- | --- |
| 1 | 生成 mymath.dll | 解决方案资源管理器里的 mymath 工程（add.c/sub.c/mul.c/div.c）+「输出」窗口的 `mymath.vcxproj -> ...\mymath.dll` |
| 2 | 调用运行结果 | 控制台的 `a = 20262814` 和四行运算结果 |
| 3 | 条件断点 | 断点条件（`a == 20262814`）、局部变量、调用堆栈 |
| 4（可选） | 导出设置 | 项目属性里的 `MYMATH_EXPORTS`，或 `dumpbin /exports` 的结果 |

注意：

- **全屏截图**（不要只截一个窗口）：把 VS 最大化后截整个桌面，保证能看到 VS 标题栏和任务栏。
- 学号出现在两处最稳妥：`main.c` 顶部的注释、控制台输出的 `a = 20262814`。
- 三张图里都应能看出是 **mymath.dll**（不是第二周的 mymath2 静态库），否则和题目对不上。

## 7. 上传到代码托管平台（Gitee / GitHub）

本目录已经 `git init` 并完成首次提交（分支 `main`），仓库地址：

```text
https://github.com/ZhuHongYan-maker/mymath-dll
```

以后改完代码再提交：

```bash
git add .
git commit -m "mymath.dll 动态链接库实验（学号 20262814）"
git push
```

> 如果是在 U 盘 / exFAT 盘上使用 git，可能报 `detected dubious ownership`，先执行一次：
> `git config --global --add safe.directory <本仓库的绝对路径>`

## 8. 常见错误

| 现象 | 原因 | 解决 |
| --- | --- | --- |
| C1010：在查找预编译头时遇到意外的文件结尾 | 工程开着预编译头，.c 里没有 `#include "pch.h"` | 项目属性 → C/C++ → 预编译头 → 不使用预编译头 |
| LNK2019：无法解析的外部符号 `_add` 等 | DLL 没有导出（漏了 `MYMATH_EXPORTS`），或没链上 `mymath.lib` | 补宏重新生成；确认 `mymath.lib` 在工程目录 / 附加库目录正确 |
| LNK1104：无法打开文件 mymath.lib | 库目录不对 | 链接器 → 常规 → 附加库目录 指向 lib 所在文件夹 |
| 运行时报 0xc000007b 或「找不到 mymath.dll」 | dll 不在 exe 目录；或 x86 与 x64 混用 | 复制 dll 到 exe 旁；两个工程平台保持一致 |
| C2371：`div` 重定义；不同的基类型 | 函数名 `div` 与 `<stdlib.h>` 冲突 | 本仓库已改名为 `divide`，见 `div.c` 注释 |
| 中文注释乱码 | 源文件编码问题 | 本仓库 .c/.h 为 UTF-8 带 BOM；若仍乱码，C/C++ → 命令行 加 `/utf-8` |
| 断点不命中 | 条件写错，或不是 Debug 配置 | 条件写成 `a == 20262814`，确认工具栏是 Debug / x86 |

## 附录：显式调用（LoadLibrary）示例

把下面代码存成另一个控制台工程的 `main.c`，不需要 `mymath.lib`，但 `mymath.dll` 仍要放在 exe 旁边：

```c
#include <windows.h>
#include <stdio.h>

typedef int (*BINOP)(int, int);

int main(void)
{
    HMODULE h = LoadLibraryA("mymath.dll");
    if (h == NULL)
    {
        printf("LoadLibrary 失败，错误码 %lu\n", GetLastError());
        return 1;
    }

    BINOP add    = (BINOP)GetProcAddress(h, "add");
    BINOP sub    = (BINOP)GetProcAddress(h, "sub");
    BINOP mul    = (BINOP)GetProcAddress(h, "mul");
    BINOP divide = (BINOP)GetProcAddress(h, "divide");
    if (add == NULL || sub == NULL || mul == NULL || divide == NULL)
    {
        printf("GetProcAddress 失败\n");
        FreeLibrary(h);
        return 1;
    }

    int a = 20262814, b = 10;
    printf("%d+%d=%d\n", a, b, add(a, b));
    printf("%d-%d=%d\n", a, b, sub(a, b));
    printf("%d*%d=%d\n", a, b, mul(a, b));
    printf("%d/%d=%d\n", a, b, divide(a, b));

    FreeLibrary(h);
    return 0;
}
```
