#include <stdio.h>
#include <stdint.h>
#include "memoria.h"
#include "lista.h"



No *criar_no(Registro reg) {
    No* no = aloca(sizeof(No));
    no->registro = reg;
    no->prev = NULL;
    no->next = NULL;

    return no;
}

Lista criar_lista() {
    Lista lista = aloca(sizeof(*lista));
    lista->head = NULL;

    return lista;
}

No *buscar_no(Lista lista, uint32_t chave) {
    if (lista == NULL) {
        return NULL;
    }

    No *no_atual = lista->head;
    No *no = NULL;
    while(no_atual != NULL) {
        if (no_atual->registro.chave == chave) {
            no = no_atual;
            break;
        }
        no_atual = no_atual->next;
    }

    return no;
}

void inserir_no(Lista lista, Registro reg) {
    if (lista == NULL) {
        return;
    }
    if (lista->head == NULL) {
        lista->head = criar_no(reg);
        return;
    }

    // Cada nó deve possuir chave única
    if (buscar_no(lista, reg.chave) != NULL) {
        return;
    }

    No *head = lista->head;
    No *no = criar_no(reg);

    no->next = head->next;
    if (head->next != NULL) {
        head->next->prev = no;
    }
    no->prev = head;
    head->next = no;
}

void remover_no(Lista lista, uint32_t chave_no) {
    if (lista == NULL) {
        return;
    }

    No *no_atual = lista->head;

    while (no_atual != NULL) {
        if (no_atual->registro.chave == chave_no) {
            if (no_atual->prev != NULL) {
                no_atual->prev->next = no_atual->next;
            }
            else {
                lista->head = no_atual->next;
            }
            if (no_atual->next != NULL) {
                no_atual->next->prev = no_atual->prev;
            }
            no_atual->prev = NULL;
            no_atual->next = NULL;
            libera(no_atual);
            break;
        }
        no_atual = no_atual->next;
    }
}

void listar_conteudo(Lista lista) {
    if (lista == NULL || lista->head == NULL) {
        printf("Lista esta vazia!\n");
        return;
    }

    No *no_atual = lista->head;
    while (no_atual != NULL) {
        printf("Chave No: %d\n", no_atual->registro.chave);
        no_atual = no_atual->next;
    }
}

void libera_lista(Lista lista) {
    if (lista == NULL) {
        return;
    }

    No *no_atual = lista->head;

    while (no_atual != NULL) {
        No *next = no_atual->next;
        libera(no_atual);
        no_atual = next;
    }

    libera(lista);
}


