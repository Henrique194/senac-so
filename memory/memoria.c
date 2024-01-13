#include <stdint.h>
#include "memoria.h"


/**
 * Estrutura de controle dos blocos alocados no
 * buffer. Para facilitar a desalocação de memória,
 * a estrutura possui dois ponteiros para formar
 * uma lista duplamente ligada.
 */
struct BufferPtr {
    size_t size;
    struct BufferPtr *prev;
    struct BufferPtr *next;
} __attribute__((packed));

typedef struct BufferPtr BufferPtr;

/**
 * Buffer com 4096 bytes de tamanho, para simular
 * alocação de memória na heap. Cada vez que for
 * alocado n bytes utilizando a função alocar,
 * estes bytes são gerenciados através de um lista
 * duplamente ligada ("BufferPtr"). Além dos n bytes
 * alocados no buffer, são alocados outros bytes
 * (a depender do tamanho do size_t e ponteiros pelo SO)
 * para controle da lista e posteior liberação de memória.
 */
static uint8_t buffer[4096] = {0};

/**
 * Ponteiro inicial para lista de blocos ocupados no
 * buffer.
 */
static BufferPtr *head_buffer_alloc = NULL;

/**
 * Ponteiro que garante que o espaço inicial do buffer
 * seja sempre utilizado.
 */
static BufferPtr *INITIAL_BUFF_ADDR = (BufferPtr*) &buffer[0];

static const size_t SIZE_BUFFER_PTR = sizeof(BufferPtr);

/**
 * Calcula a quantidade de bytes livres entre buff_ptr e
 * buff_ptr->next.
 */
uint16_t bytes_buff_next(BufferPtr *buff_ptr) {
    void const *ptr = (uint8_t*) buff_ptr + buff_ptr->size;
    void const *next;

    if (buff_ptr->next == NULL) {
        next = &buffer[4095];
    }
    else {
        next = buff_ptr->next;
    }

    return (uint16_t) 1 + next - ptr;
}

/**
 * Insere um novo nó new_buff_ptr na lista de ponteiros ocupados,
 * de maneira que new_buff_ptr se encontra logo após buff_ptr e
 * antes de buff_ptr->prev.
 */
BufferPtr* inserir_buff_ptr(BufferPtr *buff_ptr, size_t size) {
    uint8_t *new_addr_buff = (uint8_t*) buff_ptr + buff_ptr->size;

    BufferPtr *new_buff_ptr = (BufferPtr*) new_addr_buff;
    new_buff_ptr->size = SIZE_BUFFER_PTR + size;
    new_buff_ptr->prev = buff_ptr;
    new_buff_ptr->next = buff_ptr->next;

    if (new_buff_ptr->next != NULL) {
        new_buff_ptr->next->prev = new_buff_ptr;
    }
    buff_ptr->next = new_buff_ptr;

    return new_buff_ptr;
}

/**
 * A alocação do buffer percorre toda lista de blocos alocados,
 * procurando o primeiro espaço livre para alocação de size bytes.
 */
void *aloca(size_t size) {
    const size_t bytes_alloc = SIZE_BUFFER_PTR + size;

    if (!size || bytes_alloc > 4096) {
        return NULL;
    }

    // Caso buffer esteja limpo
    if (head_buffer_alloc == NULL) {
        head_buffer_alloc = INITIAL_BUFF_ADDR;
        head_buffer_alloc->size = SIZE_BUFFER_PTR + size;
        head_buffer_alloc->prev = NULL;
        head_buffer_alloc->next = NULL;
        return (uint8_t*) head_buffer_alloc + SIZE_BUFFER_PTR;
    }

    BufferPtr *curr_ptr = INITIAL_BUFF_ADDR;
    void *ptr = NULL;

    while (curr_ptr != NULL) {
        uint16_t dist_next = bytes_buff_next(curr_ptr);

        if (dist_next >= bytes_alloc) {
            BufferPtr *new_buff_ptr = inserir_buff_ptr(curr_ptr, size);
            ptr = (uint8_t*) new_buff_ptr + SIZE_BUFFER_PTR;
            break;
        }

        curr_ptr = curr_ptr->next;
    }

    return ptr;
}

/**
 * Libera bytes alocados no buffer. Pela utilização
 * de uma lista duplamente ligada, a operação é O(1).
 */
void libera(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    void *temp = (uint8_t*) ptr - sizeof(BufferPtr);
    BufferPtr *buff_ptr = (BufferPtr*) temp;

    if (buff_ptr->prev == NULL) {
        head_buffer_alloc = buff_ptr->next;
        INITIAL_BUFF_ADDR->next = head_buffer_alloc;
    }
    else {
        buff_ptr->prev->next = buff_ptr->next;
    }

    if (buff_ptr->next != NULL) {
        buff_ptr->next->prev = buff_ptr->prev;
    }
}