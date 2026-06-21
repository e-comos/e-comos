# E-comOS Coding Style Guide
## Overview
The codes is most important, if you can, please join up us to develop E-comOS, we're using **1TBS, 4-bits tab and snake_case** **only**.<br>
We'll **reject** use the **Allman, GNU style** and **spaces, camlCase** codes in our codes.<br>

## Basic Code Style
### Curly Brackets
E-comOS Operating System Project use the **1TBS** style. The 1TBS means in **any case**, you shouldn't type the `{` in a new line.
```C
#include <stdio.h>

/* Right ✅ */
void* func1(void) {
  printf("This is right!");
  return NULL;
}

/* Wrong ❌ */
int main(void)
{
  func1();
  if (func1 != NULL)
  {
    abort();
  }
  else
  {
    exit(0);
  }
}
```
### Tab
You have to use the Tab key to indentation. Here is a Example
```C
