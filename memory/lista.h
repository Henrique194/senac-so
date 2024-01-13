/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef LISTA_H
#define LISTA_H

#include <stdint.h>

typedef struct {
    uint32_t chave;
    void *data;
} Registro;

typedef struct No {
    Registro registro;
    struct No *prev;
    struct No *next;
} No;

typedef struct {
    No *head;
}* Lista;

No *criar_no(Registro reg);
Lista criar_lista();
No *buscar_no(Lista lista, uint32_t chave);
void inserir_no(Lista lista, Registro reg);
void remover_no(Lista lista, uint32_t chave_no);
void listar_conteudo(Lista lista);
void libera_lista(Lista lista);

#endif //LISTA_H
