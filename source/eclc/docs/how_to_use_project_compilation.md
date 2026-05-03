# HELP DOC (1) - How to use project compilation
E-comOS is not support make so compiler must can compilation a project.<br>
You must to learn how to use it.
## Command(s)
First , want to use project compilation you must add option ```-f``` , or eclc will report a error :
```
ERROR : In case `NO_OPTION_-f` , you cannot comilation a project in eclc
Note : type `eclc --help` or `eclc -h` to get more info.
```
But if you add option `-f` , you want to compilation **a** file , it will report:
```
ERROR : In case `OPTION_-f` , you cannot comilation file(s) in eclc
Note : Type `eclc --help` or `eclc -h` to get more info.
```
More option you only need add option after 'f':
```
eclc -f[s] <example> --cpp-code
```
([s] is a option here)<br>
**END**<br>
                            -> NEXT PAGE