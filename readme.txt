
         .,;%!##########################!%; .-----, ;%!#############!%;,.
             .___, .___.______ .____.____  /      |
             /   \/   1  __   ]|    /   / /  ,--. |     
       []   /      .-=  (__)  [|   !.  / /  (___! |  []
          .(___/\___/.________|~      /./____/____|,========-,.
         new operating system ,|_____/.     --------------
                                        develOOpeD bY M@rc!n NoWa((
     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2003-02-26

Kompilacja:

    make core			- obj/novacore (skopiuj do /dev/fd0)
    make exec 			- obj/libs/exec.lib (skopij do /dev/fd0/libs)
	make boot			- obj/floppymbr (MasterBootRecord dla dyskietki)
						  zainstaluj poprzez wywolanie "make installboot"
						  (uwaga, w systemie musi byc obecne polecenie dd)    
