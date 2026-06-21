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
for (int cnt=1, cnt<=5, cnt++) {
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
if (some_function() == true) { do_first_thing() } else if (return_value == A_MACRO) { do_a_thing(); } else {return;}
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
```
Before the '.' and the '->' don't put the spaces.
