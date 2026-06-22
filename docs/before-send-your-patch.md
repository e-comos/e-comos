# Before Send Your Patch - Checklist and Notice
Well, you have a commit want to push, but you are afraid of someone's angry?  
Don't panic. This document will help you.
## Style
So first in our checklist is -- did you use the right style?
> [!NOTE]
> E-comOS Operating System Project(EOSP) is using the 1TBS style + tabs + snack_case.  
> Make sure you followed the [*E-comOS Coding Style Guide*](coding-style.md)

A code that follows the code style specification will give people a bright feeling. This will increase the maintainer's liking for you (especially when he has just finished spraying a person who uses Allman).
## Codes
The code is also very important. If you just have the right style, but the logic is like a beautiful piece of shit, then you will sit ugly and sit in a dump.  
Therefore, it is important to keep your code neat and compilable (no one likes to see code that can't be compiled and sprays people with error messages).  
So, please test your code on your computer. Don't think the CI or CD system can help you.
## Commit
Commit message is not important.  
No matter what others say, the only purpose of your commit message is to let others know what you have done. So you don't need (it doesn't mean hard requirements, see [documents style](documents-style.md)) to add prefixes such as feat.