#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MV_SIZE 100
#define MR_SIZE 50
#define MS_SIZE (MV_SIZE - MR_SIZE)

typedef struct pagina {
    int modificada;
    int referenciada;
    int presente;
    int molduraPagina;
    int timestamp;
}TPagina;

// Esta variável é incrementada cada vez que uma página é carregada na memória.
int g_timestamp = 0;

int gera_valor_aleatorio(int media, int dp);
int busca_pagina(int pag_virtual, TPagina *memoria_virtual);
void acessa(int pagina_nova, int pagina_antiga, TPagina *memoria_virtual);
int substitui_pagina(TPagina *memoria_virtual);
void encher(TPagina *memoria_virtual);

int main() {
    TPagina memoria_virtual[MV_SIZE];

    // init struct memoria_virtual with null
    for (int i = 0; i < MV_SIZE - 1; i++) {
        memoria_virtual[i].modificada = 0;
        memoria_virtual[i].referenciada = 0;
        memoria_virtual[i].presente = 0;
        memoria_virtual[i].molduraPagina = 0;
    }

    int pagina_nova, page_miss = 0, pagina_antiga, random;

    encher(memoria_virtual);

    int media = 50;
    int desvio_padrao = 10;

    for (int i = 0; i < MV_SIZE - 1; i++) {
        random = gera_valor_aleatorio(media, desvio_padrao);
        pagina_nova = busca_pagina(random, memoria_virtual); // -1 se não encontrou
        pagina_antiga = -1;

        if (pagina_nova == -1) {
            pagina_antiga = substitui_pagina(memoria_virtual); // 33 página para substituir
            page_miss++;
        }

        printf("Página nova: %d\n", pagina_nova);
        printf("Página antiga: %d\n", pagina_antiga);

        acessa(random, pagina_antiga, memoria_virtual);
    }

    // print memoria_virtual
    // for (int i = 0; i < MV_SIZE - 1; i++) {
    //     printf("Página %d\n", i);
    //     printf("Modificada: %d\n", memoria_virtual[i].modificada);
    //     printf("Referenciada: %d\n", memoria_virtual[i].referenciada);
    //     printf("Presente: %d\n", memoria_virtual[i].presente);
    //     printf("Moldura da página: %d\n", memoria_virtual[i].molduraPagina);
    // }

    return 0;
}

void encher(TPagina *memoria_virtual) {
    int moldura, resto;

    for (int i = 0; i < MR_SIZE - 1; i++) {
        resto = i % 2;
        memoria_virtual[i].modificada = resto;
        memoria_virtual[i].referenciada = i > 25 ? resto : 1;
        memoria_virtual[i].presente = 1;
        memoria_virtual[i].molduraPagina = i;
    }
}

// Gera um número aleatório seguindo uma distribuição normal padrão (média 0, desvio padrão 1)
double randn() {
    const double epsilon = 1e-5;  // pequena constante para evitar log(0)
    double u1 = ((double) rand() / (RAND_MAX)) + epsilon;
    double u2 = ((double) rand() / (RAND_MAX)) + epsilon;
    double normal = sqrt(-2*log(u1))*cos(2*M_PI*u2);
    return (normal + 1) / 2;  // transforma o intervalo de [-1,1] para [0,1]
}

int gera_valor_aleatorio(int media, int desvio_padrao) {
    double valor = randn();
    return (int) (valor * desvio_padrao + media);
}

int busca_pagina(int pag_virtual, TPagina *memoria_virtual) {
    if (memoria_virtual[pag_virtual].presente) {
        return pag_virtual;
    }
    return -1;
}

//create a function that replaces a page using FIFO
int fifo(TPagina *memoria_virtual) {
    int i, menor = 0;
    for (i = 0; i < MV_SIZE - 1; i++) {
        if (memoria_virtual[i].timestamp < memoria_virtual[menor].timestamp) {
            menor = i;
        }
    }
    return menor;
}


int substitui_pagina(TPagina *memoria_virtual) {
    int i;
    for (i = 0; i < MV_SIZE - 1; i++) {
        if (memoria_virtual[i].presente && !memoria_virtual[i].referenciada && !memoria_virtual[i].modificada) {
            return i;
        }
    }
    for (i = 0; i < MV_SIZE - 1; i++) {
        if (memoria_virtual[i].presente && !memoria_virtual[i].referenciada && memoria_virtual[i].modificada) {
            return i;
        }
    }
    for (i = 0; i < MV_SIZE - 1; i++) {
        if (memoria_virtual[i].presente && memoria_virtual[i].referenciada && !memoria_virtual[i].modificada) {
            return i;
        }
    }
    for (i = 0; i < MV_SIZE - 1; i++) {
        if (memoria_virtual[i].presente && memoria_virtual[i].referenciada && memoria_virtual[i].modificada) {
            return i;
        }
    }
    return -1; // retorna -1 se não há páginas disponíveis para substituição
}

void acessa(int pagina_nova, int pagina_antiga, TPagina *memoria_virtual) {
    
    if (pagina_antiga == -1) {
        memoria_virtual[pagina_nova].referenciada = 1;
        memoria_virtual[pagina_nova].modificada = 1;
        return;
    }
    
    memoria_virtual[pagina_nova].presente = 1;
    memoria_virtual[pagina_nova].referenciada = 1;
    memoria_virtual[pagina_nova].modificada = 1;
    memoria_virtual[pagina_nova].molduraPagina = memoria_virtual[pagina_antiga].molduraPagina;
    
    memoria_virtual[pagina_antiga].presente = 0;
    memoria_virtual[pagina_antiga].referenciada = 0;
    memoria_virtual[pagina_antiga].modificada = 0;
    memoria_virtual[pagina_antiga].molduraPagina = -1;
}

// int teste() {
//     int page_miss = 0;
//     for (int i = 0; i <= 3; i++) {
//         for (int j = 0; j <= 10; j++) {
//             int k = 0;
//             while (k++ < 10) {
//                 int pag_virtual = gera_valor_aleatorio(k, 0, 9);
//                 int pag_real = busca_pagina(pag_virtual);
//                 if (pag_real == -1) {
//                     pag_real = substitui_pagina(pag_virtual);
//                     page_miss++;
//                 }
//                 acessa_pagina(pag_virtual, pag_real);
//             }
//         }
//     }
// }
// int acessa_pagina(int pag_virtual, int pag_real) {
//     pagina[pag_real].referenciada = pag_virtual;
//     pagina[pag_real].modificada = 1;
// }

// int substitui_pagina(int pag_virtual) {
//     int class;
//     int least_valued_page = -1;
    
//     // Vamos procurar a página de menor classe na memória real
//     for(int i = 0; i < MR_SIZE; i++) {
//         class = 2 * paginas[memoria_real[i]].referenciada + paginas[memoria_real[i]].modificada;

//         // Se a página de menor classe for encontrada (classe 0), ela será substituída imediatamente
//         if(class == 0) {
//             least_valued_page = i;
//             break;
//         }

//         // Se não, mantemos o índice da página de menor classe encontrada até agora
//         if(least_valued_page == -1 || class < (2 * paginas[memoria_real[least_valued_page]].referenciada + paginas[memoria_real[least_valued_page]].modificada)) {
//             least_valued_page = i;
//         }
//     }

//     // Substituímos a página de menor classe pela página virtual
//     int substituido = memoria_real[least_valued_page];
//     memoria_real[least_valued_page] = pag_virtual;
//     paginas[pag_virtual].referenciada = 0;
//     paginas[pag_virtual].modificada = 0;

//     // Retornamos o valor da página que foi substituída
//     return substituido;
// }