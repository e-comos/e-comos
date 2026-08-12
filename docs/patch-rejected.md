# If Your Patch Is Rejected
If your patch was rejected, don't worry. After all, anyone might write baffling code for any reason.<br>
This article explains what to do if your patch is rejected.
## Coding Style
Code that adheres to a [style guide](https://github.com/e-comos/e-comos/tree/main/docs/coding-style.md) leaves a good impression on maintainers; conversely, code that ignores it leaves them with a "delightful" (read: terrible) experience.
So, if your patch is rejected, please check your code and make sure they're following the coding style guide.
## Cannot Run
A important reason will make your patch be rejected. That's **your code cannot compile or run**. If your patch cannot work, that is useless.<br>
Make sure you tested your code in the QEMU or your device.
## Lost the Copyright Head
You should add this part in your code's head:
```
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
```
If you didn't add it, your patch will be rejected; also, please get your employer (if you work as a programmer) or school, if any, to sign a "copyright disclaimer" for the program, if necessary.<br>
For more information on this, and how to apply and follow the GNU AGPL, see <https://www.gnu.org/licenses/>.
## Lost Signs
You should add this in your patch
```
	Signed-off-by: <Your Name> your-email@your-email-service-provider.com
```
If you didn't add this line, your patch may be rejected.
