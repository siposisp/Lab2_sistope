<h1>Sistemas Operativos: Laboratorio 2</h1>

Este repositorio contiene el segundo laboratorio del curso de Sistemas Operativos, principalmente enfocado en diferentes aspectos del manejo de procesos en un sistema operativo Linux y el uso de pipes.

<h2>Laboratorio 2: Funciones Unix - Fork - Pipes</h2>

Este laboratorio tiene como objetivo aplicar técnicas de programación imperativa mediante lenguaje C, como la recepción de parámetros mediante getopt, compilación mediante Makefile sobre sistema operativo Linux y el manejo de la creacion de procesos y uso de pipes mediante linea de comando.

<h3>Autor: Nicolas Gajardo & Cristian Romero</h3>

<h3>Fecha: 05-12-2024</h3>

Cómo ejecutar este proyecto

0.Se requiere un sistema operativo linux-based.

1.Compilar mediante el comando <code>make</code>.

2.Para ejecutar debes estar conciente de las siguientes definiciones para los flags para los diferentes archivos:

<h3>Primer Programa</h3>

       -i: es el archivo de entrada 
       
       -o: es el archivo de salida
       
       -d: es el separador
       
       -c: es el indicador de columnas
      
Ejemplo de comando para ejecutar:

              ./cout -i input.txt -o ouput.txt -d : -c 2,4      

<h3>Segundo Programa</h3>

       -i: es el archivo de entrada 
       
       -o: es el archivo de salida
       
       -s: es el String original del archivo a modificar
       
       -S: es el String nuevo que se escribira

Ejemplo de comando para ejecutar:

              ./srep -i input.txt -o ouput.txt -s root -S hola
      
<h3>Tercer Programa</h3>

       -i: es el archivo de entrada 
       
       -C: es el que cuenta el numero de caracteres
       
       -L: es el que cuenta el numero de lineas
       
Ejemplo de comando para ejecutar:

              ./count -i input.txt -C -L
      
              ./count -i input.txt -L -C
   
              ./count -i input.txt -L
       
              ./count -i input.txt -C

Obs: El programa está preparado para entregar los valores solicitados de ejemplo en la rubrica.

<h2>Archivo MakeFile</h2>

Una vez abierta la consola/terminal en linux algunos comandos utiles para el archivo MakeFile son los siguientes:
      
              ./make              
   
              ./make clear 
       

<h2>Excepciones</h2>

<h3>Primer Programa</h3>

Hay algunos casos en las cuales puede dar error, al colocar el separador indicado:

Caso de fallo:

              ./cut -i input.txt -o ouput.txt -d ; -c 2,4,3

                          
<h3>Segundo Programa</h3>


Los comandos separados por ejemplo "hola mundo", deben ser escritos entre comillas para realizar el correcto reemplazo.

Ejemplo de compilado correcto:

              ./srep -i input.txt -o ouput.txt -s root -S "hola mundo"  

              ./srep -i input.txt -o ouput.txt -s root -S hola_mundo 
              
Lo anterior debido a que el programa reemplaza hasta que lea un espacio en blanco.

Caso erroneo:

              ./srep -i input.txt -o ouput.txt -s root -S hola mundo
       
            
Se cambiara los root solo por "hola" y no la palabra completa.

Existen comandos que deben ser escritos doblemente debido a diversos factores en el sistema operativo Linux. Por ejemplo:
<code>"/".</code>

Ejemplo de compilado correcto:

              ./srep -i input.txt -o ouput.txt -s root -S //

<h2>Archivo lab2.c</h2>

Comandos para ejecutar el archivo lab2.c implementando pipes segun los requerimientos solicitados:

               ./lab2 ./cut -i input.txt -d : -c 2,4 | srep -s / -S \ | count -o output.txt -C -L

               ./lab2 ./srep -i input.txt -s / -S \ | cut -d : -c 2,4 -o output.txt

               ./lab2 cut -i input.txt -d : -c 2,4 | ./srep -s / -S \ | count -C

Estos son algunos ejemplos de comandos implementados.
               
<h2>Descripción</h2>

En este laboratorio se implementara algunas funciones simplificadas de las que provienen UNIX, en la que se realizaran distintas manipulaciones de texto sobre los archivos csv entregados. Cada una de estas funciones sera un archivo .c distinto, el cual realizara el simil de la funcion UNIX respectiva. Finalmente, estos archivos se ejecutaran por linea de comando.

<h3>Funcion Cut</h3>
El programa cut se usa para extraer informacion sobre nuestro archivo de entrada .csv o, en caso de no especificar un archivo de entrada, sobre un stdin, con el objetivo de obtener ciertas columnas
de interes.

<h3>Funcion Srep</h3>
El programa srep (String Replace) realiza un reemplazo del “string objetivo” (el que se encuentra dentro del archivo) por el “string nuevo” (el que ocupara el nuevo lugar dentro del archivo). Este cambio debe realizarse para todas las instancias en el que se encuentre el “string objetivo”. Finalmente se debera guardar la informacion reemplazada en un archivo de salida.
<h3>Funcion Count</h3>
El programa count se usa para obtener el total de lineas, palabras o bytes contenidos en el archivo csv, siendo en el caso de indicar un archivo de entrada; en caso contrario, la entrada sera por stdin.

<h3>Uso de pipes</h3>

Anteriormente, se describieron los programas cut, count y srep, los que debieron ser construidos e implementados en el laboratorio 1. Ahora se debera desarrollar un cuarto programa, el que tendra la mision de llamar y coordinar a los programas mencionados anteriormente (cut, count y srep). Este cuarto programa debe ser llamado lab2.c el cual recibira como argumento de linea de comando un string que describe la ejecucion de uno, dos o tres de los programas (cut, count y srep) conectados
por pipes.

La idea del lab2 es simular las operaciones que realiza la bash, cuando se ejecuta este comando (los comandos fueron meencionados anteriormente como ejemplo).

Luego, cada programa debe tener la capacidad de determinar si su operacion de I/O, ya sea de lectura o escritura, es por archivo o por stdin, y stdout. Tambien, el proceso lab2 debe saber cuando duplicar las entradas y salidas estandar con los pipes.


