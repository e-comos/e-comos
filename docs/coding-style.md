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
After the C11, C support the one line comments.<br>
