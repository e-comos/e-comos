# E-comOS Coding Style Guide
## Overview
The codes is most important, if you can, please join up us to develop E-comOS, we're using **1TBS, 4-bits tab and snake_case** **only**.<br>
We'll **reject** use the **Allman, GNU style** and **spaces, camlCase** codes in our codes.<br>
Before reading this article, please print out a copy of the *GNU Coding Standards*, the *Allman Style Guide*, or your company's internal coding style guidelines, and then destroy the document using a shredder that meets Pentagon standards for the destruction of classified materials—an act of great symbolic significance. At the same time, please print out a copy of *The C Programming Language* (K&R) for reference—an act that is also of great symbolic significance.
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
  if (func1() != NULL)
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
for (int cnt=1;cnt<=5;cnt++) {
  // Wrong ❌
  printf("This is wrong");
  // Right ✅
  printf("This is right");
}
```
We are using the 4-spaces tab, because 8-spaces tab make good code looks like a shit, too.<br>
(Notice: If you're using the tab, tab's length can set by your own, so, in your editor is 8-spaces, maybe in my computer it is 4-spaces?)
### Shortly Command
At times, using statements that include curly braces is overly cumbersome.<br>
So, if your code's some statements is in one line, look like this:
```C
if (something) do_something():
```
However, doing this isn't ideal when using multi-branch statements. It will be this :
```C
if (something == true) do_something(); else do_others();
```
Some fools write code like this.
```C
if (some_function() == true) { do_first_thing(); } else if (return_value == A_MACRO) { do_a_thing(); } else {return;}
```
Don't do that. It's ugly.
### Spaces
I said DO NOT use spaces in indentation, but you should put a ' ' after these words:
```C
for, if, case, do, while
```
But you shouldn't put a spaces after the function name, like
```C
/* Wrong ❌ */
scanf ("%d",&var);
```
But. Type should.
```C
// Right ✅
_Noreturn void function(void);
```
You shouldn't add the spaces after "(" in one line.
```C
/* Right ✅ */
for (;;) {
  __builtin_unreachable();
}
```
Before the '.' and the '->' don't put the spaces.
```C
struct_a a.b // Right✅
struct_b c->d // Right✅
```
A good code should look like this:
```C
#include <stdio.h>
#include <dosomething.h>
int main(void) {
  int var;
  scanf("%d",&war);
  for (int tmp=var; tmp >= 0; tmp--) { // Put a spaces before '--''++' is ugly
    printf("%d",&tmp);
  }
  exit(0);
}
```
### Name
When you want to name a variable, you have to distinguish whether it is a global variable or a local one.
#### Global
A The name of a global variable should be simple and easy to remember.
```C
// Good 👍
time_t log_the_user_login_time;
// Bad 👎
time_t ltult;
```
We are using the snack_case to name a variable.
```C
/* Right ✅ */
int nothing_here_return = 0;
// Wrong ❌
int nothingHereReturn = 0;
```
In the global variable name, you can add the subsystem's name, like
```C
int network_count_user_concent = 0;
```
Don't use the `_` in the variable's start. Not include you must to do that.<br>
#### Local
The local name is same with the global name, but if you can, please make it shortly<br>
You can use some alias , e.g.:
```C
int cnt; // A alias for count.
```
> [!WARNING]
> The words like 'master' 'alloc' may be was use in the code, please check your name, are they repeat some variables name?
### Functions
#### Functions Name
The name of a function should be self-explanatory. That is to say, a fifth-grade student in Chinese primary school who knows a little English should be able to know the function of a function by reading your function name. For example:
```C
int output_numbers(int num);
```
You can use some abbreviations that everyone understands. For example, you can name a function like this:
```C
int without_arg_malloc(void);
// Or
int mm_alloc_arg(int arg1);
```
In this example, arg and malloc is a abbreviations.<br>
#### Define a Function
If you want to define a function, a good way is define it in your .h files, like this:
```C
int func(void);
```
When you define a function, and this function has no parameter, please type the 'void' in the '()'. Like this:
```C
// Wrong ❌
int wrong_func();
// Right ✅
int right_func(void);
```
You can define a function and you don't need give your parameters a name, but it's not good:
> [!WARNING]
> The codes like ```int warn_func(int)``` is not good.
> If others see this code, if they don't check the .c file, they cannot know what's means of this parameter.
The good ways are:
```C
int good_func(char* print_words);
```
And, some functions have too more parameters, please define it like:
```C
_Noreturn void a_no_retuen_long_paramters_func (int a, int b, char[] c, void* d, bool f
                                                FILE* g, pid_t h, uint32_t i, int64_t j);
```
> [!NOTE]
> If your function name is too long, use the abbreviations if you can.
#### Implement the Function
> [!NOTE]
> The implement should put in the '.c' file, or, some functions don't need one line to define, so just put it in your '.c' file
> If you want to include a function but this function don't have a special statement(e.g., `int a_func(void);`), please use the
> `extern` to include
If your function's define don't give parameter a name, in your implement, you should give parameter a name. Like this:
```C
/* In your .h file, the functions define: */
_Noreturn void kernel_panic(char*);
/* Wrong ❌ */
_Noreturn void kernel_panic(char*) { // Compile will report the error
  /* Codes here */;
}
/* Right ✅ */
_Noreturn void kernel_panic(char* errmsg) {
  /* Codes here */
}
```
A defined function should have a implement
> [!NOTE]
> If you found out some functions was define but don't have implement, and this functions is unuseful, please remove it.
#### Quit the Function
> [!WARNING]
> Not any functions need quit or return, don't add the `return` in these functions
##### Plan A: Return
If your code have a caller, you can use the 'return' to come back. Like:
```C
int main(void) {
  return 0;
}
```
It's not useful in the kernel. Some functions don't have caller.
##### Plan B: Goto
Although it is claimed to be outdated by some people, the equivalent of goto statements is still often used by compilers, and the specific form is unconditional jump instructions.<br>
When a function exits from multiple positions and needs to do some common operations similar to cleaning, the goto statement is very convenient. If you don't need to clean up, just return directly.<br>
Choose a tag name that can explain the goto behavior or why it exists. If goto wants to release buffer, a good name can be out_free_buffer:. Don't use GW_BASIC names like err1: and err2:, because once you add or delete the exit path (of the function), you have to renumber them, which will be difficult to check the correctness.<br>
Example:
```C
int foo(int* a) {
  do {
    something(a);
    a++;
    if (a <= a_number) goto a_number_more_than_a;
  }
  while (something);
  goto exit;
a_number_more_than_a:
  free(a_number_more_than_a);
exit:
  free(a);
  exit(0);
```
Pay attention for this error:
```C
err:
  mm_free(tmp->foo);
  mm_free(tmp);
  return ret;
```
This code is copied from *Linux Coding Guide*.<br>
The error in this code is that foo is NULL on some exit paths. Usually, this error is fixed by separating it into two error tags err_free_bar: and err_free_foo
### Comments
After the C11, C support the single-line comments.<br>
#### Single-Line Comments
The single-line comments like this:
```C
// This is a comment
```
Single-line comments are used to describe the purpose of the code on the current line, the preceding line, or the following line. For Example:
```C
int tmp = 0 // Init tmp is 0
```
You shouldn't use too many single-line comments, for example, this:
```C
// This comments have too many signgle-line comments
// Don't Do that.
// Your comments (single-line) should <3 lines
// But this ... is 5 lines
// So, it's wrong.

// 👆 This is WRONG ❌
// 👆 But this is right ✅.
```
Comments are good, but there is a risk of over-commenting. Never use comments to explain how your code works; it is better to write code that is self-explanatory, as explaining poorly written code is a waste of time.<br>
When you are writing a comments document, a good way is use multi-line comments. But you can use the single-line comemnts, too. For example:
```C
// ----- THIS IS A SUBJECT -----
// Chapter A.
//   ...
// ------- END OF TEXT ---------
```
You should try to ensure that the number of `=` or `-` characters at the beginning and end of the comment matches, and that the closing text (such as `END OF TEXT`) aligns roughly with the position of the opening text.
#### Multi-Line Comments
The multi-line comments is a good way to write comments document.<br>
For examples:
```C
/**
 * This is the preferred style for multi-line
 * comments in the Linux kernel source code.
 * Please use it consistently.
 *
 * Description:  A column of asterisks on the left side,
 * with beginning and ending almost-blank lines.
 */
```
See [document style](documents-style.md) to learn details.<br>
In any case, annotating data—whether of basic or derived types—is important. To facilitate this, declare only one data item per line (avoid declaring multiple items at once using commas). This leaves you space to write a brief comment explaining the purpose of each item.
### Macro & Constant
#### Define a Macro or Define a Constant
> [!WARNING]
> If possible, avoid redefining macros.<br>
> Macros are not type-safe;<br>
> you should not use them indiscriminately. If a constant is required to ensure type safety, use a constant instead of a macro.<br>
Defining a macro or a constant is a very simple task.
##### Define a Macro
Define a macro; use uppercase letters for the macro name, although function-like macros may use lowercase letters.<br>
Like this:
```C
#define THIS_IS_A_MACRO
```
Macros containing multiple statements should be enclosed in a do-while block:
```C
#define macrofun(a, b, c)                       \
        do {                                    \
                if (a == 5)                     \
                        do_this(b, c);          \
        } while (0)
```
Things to avoid when using macros:
 1. Macros that affect control flow:
```C
#define FOO(x)                                  \
        do {                                    \
                if (blah(x) < 0)                \
                        return -EBUGGERED;      \
        } while (0)
``}
```
It's not good, It looks like a function, yet it causes the function that **called** it to exit; don't confuse the reader's mental parser.
2. A macro that relies on a local variable with a fixed name.
3. Parameterized macros used as lvalues—such as `FOO(x) = y`—will cause errors if `FOO` is converted into an inline function.
4. Overlooking operator precedence: Macros that define constants using expressions must enclose the expression in parentheses. Macros with parameters also require attention to this issue.
```C
#define CONSTANT 0x4000
#define CONSTEXP (CONSTANT | 3)
```
5. Naming conflicts when defining function-like local variables within macros.
#### Constant
Constants are defined and follow the same conventions as macros; however, it is important to note that, unlike macros, constants incur a runtime overhead.<br>
However, when a feature requires strong type safety and performance is not critical, use constants (although such cases are rare).
### Print the Message
#### In the Kernel
When you are coding for kernel. Please use the `print_str` to print the kernel message.<br>
Kernel's print_* functions unsupport format output now.
#### In the User Space
Use the `printf` `spintf` and other functions to do it.
#### Note
> [!NOTE]
> Kernel developers should come across as articulate. Pay close attention to the spelling of kernel messages to ensure a good impression. Avoid non-standard forms like "dont"; instead, use "do not" or "don't." Ensure that these messages are simple, clear, and unambiguous.<br>
> Kernel messages do not need to end with a period.<br>
### Editor
Some editors can interpret configuration information embedded in source files and marked with special tags. For example, Emacs can parse lines marked like this:
```paint text
-*- mode: c -*-
```
Or this
```C
/*
Local Variables:
compile-command: "gcc -DMAGIC_DEBUG_FLAG foo.c"
End:
*/
```
Vim can parse markers like this:
```C
/* vim:set sw=8 noet */
```
Do not include any such content in the source code. Everyone has their own editor configuration, and your source files should not override others' settings. This includes markers related to indentation and mode configuration. People can use their own custom modes or employ other clever methods to achieve the correct indentation.
## Compile Codes and Assembly Language
### Inline Assembly
Inline assembly is not good for cross-platform. However, in code targeting specific architectures, you may need inline assembly to interface with CPU- and platform-specific features. Do not hesitate to do so when necessary. Yet, avoid using inline assembly unnecessarily when C can get the job done; whenever possible, you can—and should—communicate with the hardware using C.<br>
Consider writing simple helper functions to encapsulate common inline assembly sequences, rather than repeatedly writing inline assembly code that differs only slightly. Remember that inline assembly can utilize C parameters.<br>
Large, reasonably complex assembly functions should be placed in .S files, with their corresponding C prototypes defined in C header files.<br>
You might need to mark assembly statements as `volatile` to prevent GCC from removing them after failing to detect any side effects. However, you do not always need to do this, as unnecessary use of `volatile` can limit optimization.<br>
When writing a single inline assembly statement containing multiple instructions, enclose each instruction in quotes and place it on a separate line; append `\n\t` to the end of every instruction except the last one to ensure correct indentation for the next instruction in the assembly output:
```C
asm ("magic %reg1, #42\n\t"
     "more_magic %reg2, %reg3"
     : /* outputs */ : /* inputs */ : /* clobbers */);
```
### Assembly Syntax
Assembly syntax is highly fragmented, with different syntaxes for each architecture; x86 assembly, in particular, suffers from the most severe fragmentation. We use GAS/AT&T syntax for x86 assembly, while for other architectures, we follow the officially recommended conventions.
### Compile
Please enable -Werror or an equivalent flag when compiling.

---
Last Update: Sun Jun 21 8:43 PM(UTC+0800), 2026
