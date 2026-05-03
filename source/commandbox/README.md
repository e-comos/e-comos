# commandbox
E-comOS Command Box , All system commands is in here<br>
Any developers can use it to develop yourself command tools , and its very important for E-comOS Basic Terminal Shell , 'cause when you input the command to ebts , ebts will send request to commandbox , commandbox build command send back to ebts.
## How to use it
### Developers
The simplest way is to directly enter the command in the terminal, and the commandbox will run.<br>
However, if you are a developer and want to create your own derivative versions, then use
```bash
make config
```
to select what you need and don't need in your derivative versions.
### System Users
You only need do a thing , type your command in ebts or other shell for E-comOS.
Such as `switch user1` , you will switch to user "user1".
```
[computer:user] root > switch user1
```
### How do we understand what this actually means?
It's very simple; it's just a library that simply allows ebts to call commands.
## How exactly does it work?
Let's take the operation of "ebts" as an example.<br>
ebts ------->(send a request)commandbox --------->(Find and load command)---------------> Run it , and show result to user<br>
Very simple!
## How we license
In light of the new circumstances of the 21st century, and in order to fully embody the principles of open source and freedom, we have chosen GNU AGPL Version 3 as our open source license.
