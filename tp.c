#include "automato.h"
#include <stdio.h>
#include <stdlib.h>

// cabeçalhos da Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s arquivo_entrada\n", argv[0]);
        return EXIT_FAILURE;
    }
    int numGeracoes;

    // Inicializa o Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return EXIT_FAILURE;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar o addon de primitivas gráficas.\n");
        return -1;
    }
    // Cria uma janela
    ALLEGRO_DISPLAY *display = al_create_display(400, 200); // Largura e altura da janela

    if (!display) {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return EXIT_FAILURE;
    }

    // Le o autômato celular do arquivo de entrada
    AutomatoCelular* automato = leituraReticulado(argv[1], &numGeracoes);

    // Evolui o reticulado pelo número de gerações especificado
    evoluirReticulado(automato, numGeracoes);

    // Desenha o estado atual do reticulado na janela
    imprimeReticulado(display, automato); // Esta função deve ser implementada para desenhar na janela

    // Aguarda o usuário fechar a janela
    al_rest(100.0); // Aguarda 10 segundos antes de fechar a janela 

    // Desaloca memória antes de encerrar o programa
    desalocarReticulado(automato);

    // Destrui a janela antes de encerrar o programa
    al_destroy_display(display);

    return EXIT_SUCCESS;
}




