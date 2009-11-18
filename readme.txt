
         .,;%!##########################!%; .-----, ;%!#############!%;,.
             .___, .___.______ .____.____  /      |
             /   \/   1  __   ]|    /   / /  ,--. |     
       []   /      .-=  (__)  [|   !.  / /  (___! |  []
          .(___/\___/.________|~      /./____/____|,========-,.
         new operating system ,|_____/.     --------------
                                        develOOpeD bY M@rc!n NoWa((
     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Important!
==========

1. USE AT OWN RISK - I'm not responsible for any damage caused by this software
2. currently libraries compilation fails (gcc version incompatibility) 



Requirements:
=============
  - nasm (tested with 2.05.01)
  - gcc
  - floppy disk in FAT12

Compilation:
============

    make core
    make exec
    make boot


Instalation:
============

  1. copy obj/novacore to floppy (assume as /dev/fd0)
  2. makedir /dev/fd0/libs
  3. copy obj/libs/exec.lib to /def/fd0/libs/
  4. install MBR on floppy (try make installboot - installs on /dev/fd0)
     or use dd utility:
       dd count=1 seek=0 if=obj/floppy-mbr of=/dev/fd0
  5. boot from floppy and... (boom!)
