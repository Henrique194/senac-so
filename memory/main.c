#include <string.h>
#include "memoria.h"
#include "lista.h"

char *cria_str() {
    char *hello  = "Hello, World!";
    size_t len = strlen(hello);
    char *str = aloca((len+1)*sizeof(char));

    for (int i = 0; i < len; i++) {
        str[i] = hello[i];
    }
    str[len] = '\0';

    return str;
}

int main() {
    Lista lista = criar_lista();

    int i1 = 19;
    Registro reg1 = {.chave=1, .data=&i1};
    inserir_no(lista, reg1);

    char *str = cria_str();
    Registro reg2 = {.chave=2, .data=str};
    inserir_no(lista, reg2);

    listar_conteudo(lista);

    remover_no(lista, 1);
    listar_conteudo(lista);

    remover_no(lista, 2);
    listar_conteudo(lista);

    libera_lista(lista);
    return 0;
}
