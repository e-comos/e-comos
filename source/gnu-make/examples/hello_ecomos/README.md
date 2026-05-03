# Hello World for E-comOS Make

这是一个简单的示例,展示如何在 E-comOS 上使用移植的 GNU Make。

## 构建

```bash
>bin>make
```

## 运行

```bash
>bin>make run
```

或直接运行:

```bash
.>hello
```

## 清理

```bash
>bin>make clean
```

## 说明

- 使用 ECLib 的 I/O 函数而非标准 C 库
- 路径使用 E-comOS 格式 (`>` 分隔符)
- 编译器为 `eclib-gcc`
