#include <string.h>
#include "filesystem.h"
#include "utils.h"

fs_t * fs_init(alloc_type type, size_t size) {
    fs_t *filesystem = alloc_or_panic(sizeof(fs_t));
    filesystem->type = type;
    filesystem->size = size;
    filesystem->blocks = alloc_or_panic(size * sizeof(block));
    // Inicializa disco com caracteres 0 (zero)
    memset(filesystem->blocks, '0', size);

    return filesystem;
}

/* Função responsável por criar sistema de arquivos usando dados providos
 * no arquivo de operações. */
fs_t * create_filesystem(FILE *operations_file) {
    alloc_type type;
    size_t disk_size;
    unsigned int num_file;

    // Lê informações sobre o sistema de arquivos no .txt de operações
    fscanf(operations_file, "%d\n%zu\n%u", &type, &disk_size, &num_file);

    fs_t *filesystem = fs_init(type, disk_size);
    fs_populate_blocks(operations_file, filesystem, num_file);

    return filesystem;
}

/* Função que popula disco com arquivos armazenados inicialmente, providos
 * pelo arquivo de operações. */
void fs_populate_blocks(FILE *operations_file, fs_t *filesystem, unsigned int num_files) {
    // Variáveis para armazenar valores lidos do arquivo
    char filename;
    size_t idx, size;

    // Lê num_files linhas do arquivo de operações: arquivos inicialmente no disco
    for (unsigned int i = 0; i < num_files; i++) {
        fscanf(operations_file, "%c, %zu, %zu", &filename, &idx, &size);

        // Checa se o arquivo sendo inserido respeita os limites do disco
        if (idx <= filesystem->size && idx+size <= filesystem->size) {
            memset(&filesystem->blocks[idx], filename, size);
        } else {
            fprintf(stderr, COLOR_RED"[ERRO]"COLOR_RST" Nao pode popular disco, "
                    "arquivo %c ultrapassa limite de armazenamento.\n", filename);
        }
    }
}

void fs_add_file(fs_t *filesystem, size_t idx, char filename, size_t size) {
    // Checa se o arquivo sendo inserido respeita os limites do disco
    if (idx <= filesystem->size && idx+size <= filesystem->size) {
        memset(&filesystem->blocks[idx], filename, size);
    } else {
        fprintf(stderr, COLOR_RED"[ERRO]"COLOR_RST" Nao pode inserir arquivo no disco, "
                "arquivo %c ultrapassa limite de armazenamento.\n", filename);
    }
}

/* Função que recebe arquivo para criado e decide aonde no
 * disco o arquivo será inserido baseado no tipo de alocação
 * e os blocos livres e ocupa esses blocos. */
void fs_create_file(fs_t *fs, char filename, size_t size) {
    switch (fs->type) {
        case CONTIGUOUS:
            /* Codigo para alocação contígua. */
            break;
        case LINKED:
            /* Codigo para alocação encadeada. */
            break;
        case INDEXED:
            /* Codigo para alocação indexada. */
            break;
    }
}

/* Função que recebe arquivo para ser deleteado, sobrescreve
 * os blocos e os marca como blocos livres. */
void fs_delete_file(fs_t *fs, char filename) {

}

void fs_destroy(fs_t *filesystem) {
    free(filesystem->blocks);
    free(filesystem);
}

void dump_blocks(fs_t *filesystem) {
    size_t size = filesystem->size;

    // Imprime linha separadora de cima
    printf("-");
    for (size_t i = 0; i < 4*size; i++)
        printf("-");
    printf("\n");

    // Imprime valores no disco com separador '|'
    printf("|");
    for (size_t i = 0; i < size; i++)
        printf(" %c |", filesystem->blocks[i]);
    printf("\n");

    // Imprime linha separadora de baixo
    printf("-");
    for (size_t i = 0; i < 4*size; i++)
        printf("-");
    printf("\n");
}
