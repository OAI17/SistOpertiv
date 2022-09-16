#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "builtin.h"
#include "command.h"
#include "tests/syscall_mock.h"


// Preguntar si esta forma de checkear los comandos internos es correcta
#define INTERNO_SIZE 3
const char* INTERNO[] = {"cd", "help", "exit"};
/* 
 * cd = 0
 * help = 1
 * exit = 2
 */ 

/* Indica si el comando alojado en `cmd` es un comando interno
 */
bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    bool result = false;
    char *aux;
    int i = 0;
    aux = strdup(scommand_front(cmd));
    while(i < INTERNO_SIZE && !result){
        if (!scommand_is_empty(cmd)){
            if(strcmp(aux, INTERNO[i]) == 0){
             result = true;
            }
            i++;
        }
        else{
            result = true;
        }   
    }
    free(aux);  
    return result;
}


/* Indica si el pipeline tiene solo un elemento y si este se corresponde a un
 * comando interno.
 * REQUIRES: p != NULL
 * ENSURES:
 * builtin_alone(p) == pipeline_length(p) == 1 &&
 *                     builtin_is_internal(pipeline_front(p))
 */
bool builtin_alone(pipeline p) {
    assert(p != NULL);
    bool result = false;
    if (pipeline_front(p) != NULL) {
        if((pipeline_length(p) == 1 ) && builtin_is_internal(pipeline_front(p))) {
            result = true;
        }
    }
    return result;
}


static void dump_help_commands(void){

    char *help_command = 
                            "\n"
                            "------------------------------------\n"
                            "||||||||||||||||||||||||||||||||||||\n"
                            "------------------------------------\n"
                            "\n"
                            "-Mybash version 2022 (FaMAF)\n"
                            "-These shell commands are defined internally.  Type `help' to see this list.\n"
                            "-Designed by Ignacio Ramirez, Bruno Volpini, Kevin PREGUNTAR APELLIDO and Tomas Marmay\n"  
                            "\n"
                            "(internal) COMMANDS:\n"
                            "\n"
                            "° help: COMPLETAR\n"
                            "° cd : COMPLETAR DESCRIPCION\n"
                            "° exit: COMPLETAR\n"
                            "\n"
                            "------------------------------------\n"
                            "||||||||||||||||||||||||||||||||||||\n"
                            "------------------------------------ \n";
   printf("%s",help_command);
}


/* Ejecuta un comando interno
 * REQUIRES: {builtin_is_internal(cmd)}
 */
void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));

    char* front;
    front = strdup(scommand_front(cmd));

    if(strcmp(front,INTERNO[0]) == 0) {
        scommand_pop_front(cmd);
        free(front);
        front = strdup(scommand_front(cmd));
        chdir(front);
        //scommand_destroy(cmd);
    } else if(strcmp(front,INTERNO[1]) == 0) {
        //scommand_destroy(cmd);
        dump_help_commands();
    } else if(strcmp(front,INTERNO[2]) == 0) {
        close(STDIN_FILENO);
    }
    free(front);
}

