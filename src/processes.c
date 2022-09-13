#include <stdio.h>
#include "processes.h"
#include "utils.h"

/* Função que lê arquivo TXT de processos e retorna uma lista de 
 * processos com ID, prioridade e tempo de CPU. */
p_list_t * parse_processes(char *filename) {
    p_list_t * list = p_list_init();
    process_t * p = NULL;

    // Abre arquivo de processos
    FILE *p_file = NULL; 
    p_file = fopen(filename, "r");

    // Variáveis para armazenar valores lidos por linha
    int id, priority, cpu_time;

    if (p_file != NULL) {
        // Lê arquivo até encontrar um EOF
        while (fscanf(p_file, "%d, %d, %d", &id, &priority, &cpu_time) != EOF) {
            p = (process_t *) alloc_or_panic(sizeof(process_t));
            p->id = id;
            p->priority = priority;
            p->cpu_time = cpu_time;
            p_list_append(list, p);
        }
        fclose(p_file);
    } else {
        free(list);
        fprintf(stderr, "[ERRO] Nao foi possivel abrir arquivo %s.\n", filename);
        exit(EXIT_FAILURE);
    }
    return list;
}

void dump_processes(p_list_t * list) {
    for (size_t i = 0; i < list->size; i++) {
        process_t p = *list->processes[i];
        printf("[DEBUG] ID do processo: %d\n", p.id);
        printf("        Prioridade:     %d\n", p.priority);
        printf("        Tempo de CPU:   %d\n\n", p.cpu_time);
    }
}

p_list_t * p_list_init(void) {
    p_list_t *list = NULL;
    list = (p_list_t *) alloc_or_panic(sizeof(p_list_t));
    list->size = 0;
    list->processes = NULL;
    return list;
}

void p_list_append(p_list_t *list, process_t *process) {
    list->size++;
    list->processes = (process_t**) realloc_or_panic(list->processes, list->size * sizeof(process_t*));
    list->processes[list->size - 1] = process;
}

void p_list_destroy(p_list_t *list) {
    // Libera memória de cada processo da lista.
    for (size_t i = 0; i < list->size; i++) {
        free(list->processes[i]);
    }
    // Libera array de processos e a lista em si.
    free(list->processes);
    free(list);
}