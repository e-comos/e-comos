# ELFCOST Linker - It is ECLC and ECC's linker
ELFCOST Linker (EL) is same with GNU ld , it is E-comOS's linker when you do not installed other linker.
## Quick Start
You can use EL in E-comOS 'cause maybe Linux and macOS don't need a new linker - OH SHIT I FORGOT WINDOWS ! - let us don't care about Windows(R)<br>
When you want install EL , please make it from source code.
```bash
git clone https://github.com/E-comOS-Operation-System/el.git && cd el
# Use EMake in E-comOS
emake install
# Use GNU Make in macOS or Linux
sudo make install
# Use some mystical methods to install on Windows.
# Or mystical cannot help you , too.
```
(Yes , this is E-comOS's default linker , and it is not in EPM's official software repository)<br>
Wait ! Did we tell you E-comOS isn't a Unix-Like system? If we didn't tell you , we now tell you , E-comOS is a new system and not a Unix-Like system<br>
(Should we play user in README ? )<br>
OK , this is a true way to install EL<br>
- 1. You can use gfi(Get From Internet) to get source code from release
  2. Use EMake to make from source code.
  3. Well done.

### Unix like user
For Linux and macOS user , you can use make , 'cause maybe some code is not can run in E-comOS only. So , good luck.
### Note
EL in sometimes is do not installed in E-comOS , it depends on which version you are using, so we recommend that users check the list of pre-installed software downloaded with the installation files for reference before installing E-comOS.
## License
We use GNU AGPL Version 3
