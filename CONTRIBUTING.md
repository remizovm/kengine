# Contribution guide

## Issues and pull requests

See [issue template](ISSUE_TEMPLATE.md) and [pr template](PULL_REQUEST_TEMPLATE.md)

## Branches and commits

Each new branch should contain only one issue to be solved. Fox example, 
don't mix addition of some improvements with refactoring in the same branch. 
Each branch should contain your github name and a descriptive heading of the 
branch contents. Example:

`remizovm-invalid-timer-bug`

Use all lowercase and dashes.

Squash your branch to a single commit before making a pull request. Use 
[these](https://chris.beams.io/posts/git-commit/) rules as a starting point.

Example commit message:

```
Add xm audio support

It's to boring to live without music so here is a XM music playback support.
- Add xm.inc 
- Add xm_play and xm_stop functions

This resolves #25
```

## Code style

 - Use 8 spaces for indentation before the instruction and before it's operands:
 
 BAD:
 ```
 proc my_proc
 mov eax,ebx
 ret
 endp
 ```
 
 GOOD:
 ```
 proc my_proc
         mov     eax, ebx
         ret
 endp
 ```
 
 - Split each argument with a single space after a comma:
  
 BAD:
 
 `stdcall foo,0x10,eax`
 
 GOOD:
 
 `stdcall foo, 0x10, eax`
 
 - Don't use camelCase or PascalCase. Instead, use the underscore and lower case:
 
 BAD: 
 
 `myCoolVariable`
 
 GOOD:
 
 `my_cool_variable`
 
 - Use a filename prefix inside a particular file for you function names:
 
 BAD:
 
 ```
 somewhere inside gui.inc
 
 proc do_stuff
 endp
 ```
 
 GOOD:
  ```
 somewhere inside gui.inc
 
 proc gui_do_stuff
 endp
 ```
 
 - Prefer self-explanatory names for everything and don't use shortening:
 
 BAD:
 
 ```
 rct_cntr dd 0
 
 proc gui_drw_rct
 endp
 ```
 
 GOOD:
 
 ```
 rect_counter dd 0
 
 proc gui_draw_rectangle
 endp
 ```
 
 - Each function and global variable should be commented:
 
 ```
 .data
 ; used for doing some great things
 my_new_variable dd 0
 
 ; parses the virtual memory and returns it's size in bytes in
 ; the edx register
 ; uses eax and ebx registers
 ; arguments:
 ; - address: the starting address of the parser
 proc mem_parse uses eax ebx, address
 endp
 ```
 
