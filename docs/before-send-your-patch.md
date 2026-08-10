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
So, please test your code on your computer. Don't think the CI or CD system can help you. A good way is enable -Werror or an equivalent flag when compiling.
## Commit
Commit message is not important.  
No matter what others say, the only purpose of your commit message is to let others know what you have done. So you don't need (it doesn't mean hard requirements, see [documents style](documents-style.md)) to add prefixes such as feat.  
You can write the commit message like this:
```paint text
Fixed an issue with incorrect memory barrier usage in src/mm/mm.c.
```
Or, more simple:
```paint text
Fixed an issue with incorrect mb() usage
```
That's enough. If you are really angry, you can write it like this.
```paint text
Fixed an fuck issue with the fuck mb() usage
```
That’s fine. If your code is completely self-explanatory, you could even just write "fix a problem," since the patch file itself says it all—provided you pray the maintainer isn't viewing your patch on a mobile phone.  
So, if you don't want your patch review process to hit a snag simply because a maintainer is using a Pixel or an iPhone—well, reviewing patch files on a phone is admittedly a bit of a masochistic exercise—you'd be better off writing a proper commit message.
## Patch(es)
### Cover Letter
I think you're making a patch collection.  
If you made a patch collection, don't panic. Who hasn't cobbled together a patch set containing 42 patches—an answer to the universe, life, and everything else?  
A good way is write a cover letter. If you made a patch collection, don't panic. Who hasn't cobbled together a patch set containing 42 patches—an answer to the universe, life, and everything else?  
Simply put, a cover letter is an email containing only the description of the patch set; it is generally assigned index number 0 within the set—meaning the subject line should start with `[PATCH 0/x] <Your Subject>`.  
A good cover letter can help you to get the trust from maintainer, in the open-source commuity, the **trust** is very important.
### Patch - How to Make a Patch
> [!WARNING]
> Use Outlook's mail sevice is not a good choice, my email is a outlook.com's email service. Because Microsoft's some ... "safe" auth system, I cannot use `git send-email`.
> If you want to send your patch, please use `git send-email` first.
> If you can't use the `git send-email`, please use your mail software with paint text to send you patch.

If you did a good jobs -- for eaxmple, fixed a bug or typo -- you should push your changes into the main branch.  
But we don't want to use the pull request, so you've to send a patch.
#### How to Send Your Patch
If you want to send your patch, make sure:
1. Your patch is tested in your computer;
2. You were add the copyright notice in your code; and
3. You're a human. We don't want a robot answering during code reviews.

If you’ve done the above, then please:
1. Commit your changes;
2. Use `git format-patch` to make a patch(or make patches).
3. Use `git send-email` or your mail software to send you patches.
Well done.
