# TP2-SO
Construcción del Núcleo de un Sistema Operativo y estructuras de administración de recursos.

### Instalacion

Asegurarse previamente antes de poder ejecutar el programa tener instalado minisat. Para instalarlo:
    
    nasm qemu gcc make

    apt-get install minisat

### Compilacion

Para poder compilar los archivos primero compilar Toolchain

    user@linux:$ cd Toolchain
    user@linux:$ make all

Luego compilar el Kernel desde el directorio TP2-SO. En este caso se compila con el memory manager basado en Kernighan y Ritchie

    user@linux:$ make all

Para compilar con el buddy manager se debe compilar con el comando:

    user@linux:$ make MM=BUDDY_MM all


### Ejecucion:

Para la ejecucion del kernel correr desde el directorio TP2-SO

    user@linux:$ ./run.sh

En la shell de Userland cuenta con una serie de comandos que puede visualizar con el comando:

    >myshell:$ help

Todas los comandos se presentan en la lista a continuación


* help: Lista la informacion de los comandos disponibles
    	 
         >myshell:$ help    
    
* inforeg: Informacion sobre el valor de todos los registros. Para guardarlos presionar CTRL + r lo cual le devuelve una captura de los registros en ese momento

        >myshell:$ inforeg     

* printmem: Volcado de memoria de 32 bytes de a partir del puntero dado como argumento

        >myshell:$ printmem  <puntero>  

* time: Indica la hora actual

        >myshell:$ time     

* temperature: Informacion sobre la temperatura del procesador

        >myshell:$ temperature   

* information Informacion sobre marca y modelo del procesador

        >myshell:$ information     

* testException0: Realiza un testeo de la exception dividir por cero

        >myshell:$ testException0     

* testException6: Realiza un testeo de la exception de Invalid Opcode

        >myshell:$ testException6  

* ps: Imprime la informacion de los procesos corriendo actualmente

        >myshell:$ ps

* block Bloquea el proceso dado su id

        >myshell:$ block <pid>

* unblock: Desbloquea el proceso dado su id. Modo de uso: unblock <PID>

        >myshell:$ unblock <pid>

* loop: Imprime su ID con un saludo cada una determinada cantidad de segundos

        >myshell:$ loop <seconds>

* nice: Cambia la prioridad de un proceso

        >myshell:$ nide <pid> <priority>

* kill:  Mata el proceso dado un id

        >myshell:$ kill <pid>

* sem: Imprime informacion de los semaforos

        >myshell:$ sem


* mem: Imprime informacion de la memoria

        >myshell:$ mem

* test: Realiza un testeo. test mm realiza un testeo del memory manager. test prio hace un testeo cambiando prioridades de los procesos
test processes se encarga de crear, bloquear y desbloquear y matar procesos. test sync realiza un testeo de los semaforos. test no sync
realiza las mismas operaciones que test sync pero sin semaforos

        >myshell:$ test <mm|prio|processes|sync|nosync>

* filter: Filtra las vocales del input. Pruebe usarlo pipeando con otro commando. Ejemplo help | filter

        >myshell:$ filter 

* wc: Cuenta la cantidad de lineas de input. Pruebe usarlo pipeando con otro commando. Ejemplo help | wc

        >myshell:$ wc

* cat: Imprime el stdin tal como la recibe. Pruebe usarlo pipeando con otro commando. Ejemplo help | cat

        >myshell:$ cat

* pipe: Imprime informacion de los pipes

        >myshell:$ pipe

* phylo: Simula el problema de la cena de los filosofos. E significa que el filosofo esta comiendo

        >myshell:$ phylo

Nota: en caso de utilizar filter, wc y cat que esperan una entrada. En caso de no mandarle EOF no van a terminar su ejecucion
Puede utilizar CTRL + C para enviarle EOF. Ademas los procesos que corran en foreground puede realizar un kill sobre ellos con CTRL + Z
Basta para correr un proceso en background utilizar un & en el comando. Por default corren en foreground.
    
Para eliminar los archivos ejecutables basta con correr el comando desde el directorio TP2-SO :

    make clean

### Analisis estatico con pvs-studio y cppcheck

Para realizar los checkeos con pvs-studio y cppcheck, primero debe instalarlos con los comandos:

    apt-get install pvs-studio
    apt-get install cppcheck

A continuacion puede realizar los chequeos con el comando:

    make check

Podra ver el analisis de cpp check y de pvs-studio en los archivos out.cppOut y report.tasks respectivamente.

Para eliminar dichos archivos una vez leidos utilice el comando:

        make cleanCheck