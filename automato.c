#include "automato.h"
#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

//Esse arquivo conterá as implementações das funções que operam sobre a estrutura do autômato celular
//não deve haver acúmulo de operações dentro de uma mesma função/procedimento

// Função para alocar um reticulado
AutomatoCelular* alocarReticulado(int dimensao) {
    AutomatoCelular* automato = (AutomatoCelular*)malloc(sizeof(AutomatoCelular));
    if (automato == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o reticulado.\n");
        exit(EXIT_FAILURE);
    }

    automato->celulas = (int**)malloc(dimensao * sizeof(int*));
    if (automato->celulas == NULL) {
        fprintf(stderr, "Erro ao alocar memória para as células.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dimensao; i++) {
        automato->celulas[i] = (int*)malloc(dimensao * sizeof(int));
        if (automato->celulas[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memória para as células.\n");
            exit(EXIT_FAILURE);
        }
    }

    automato->proximaGeracao = (int**)malloc(dimensao * sizeof(int*));
    if (automato->proximaGeracao == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a próxima geração.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dimensao; i++) {
        automato->proximaGeracao[i] = (int*)malloc(dimensao * sizeof(int));
        if (automato->proximaGeracao[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memória para a próxima geração.\n");
            exit(EXIT_FAILURE);
        }
    }

    automato->dimensao = dimensao;

    return automato;
}


// Função para desalocar um reticulado
void desalocarReticulado(AutomatoCelular* automato) {
    // Libera memória das matrizes de células e da estrutura AutomatoCelular
    for (int i = 0; i < automato->dimensao; i++) {
        free(automato->celulas[i]);
        free(automato->proximaGeracao[i]);
    }
    free(automato->celulas);
    free(automato->proximaGeracao);
    free(automato);
}

// Função para ler o reticulado inicial a partir do arquivo
AutomatoCelular* leituraReticulado(const char* nomeArquivo, int *numGeracoes) {
    printf("Abrindo arquivo: %s\n", nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        perror("Erro detalhado");
        exit(EXIT_FAILURE);
    }

    int dimensao;
    fscanf(arquivo, "%d %d", &dimensao, numGeracoes);

    AutomatoCelular* automato = alocarReticulado(dimensao);

    printf("Entrada\n\n");
    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            fscanf(arquivo, "%d", &automato->celulas[i][j]);
            printf("%d ", automato->celulas[i][j]);
        }
        printf("\n");
    }

    fclose(arquivo);

    // Garante que as bordas são células mortas
    for (int i = 0; i < dimensao; i++) {
        automato->celulas[i][0] = 0; // Bordas da esquerda
        automato->celulas[i][dimensao-1] = 0; // Bordas da direita
        automato->celulas[0][i] = 0; // Bordas do topo
        automato->celulas[dimensao-1][i] = 0; // Bordas de baixo
    }

    return automato;
}


// Função auxiliar para contar o número de células vivas vizinhas de uma célula específica
int contarCelulasVivasVizinhas(AutomatoCelular* automato, int linha, int coluna) {
    int contador = 0;
    int dimensao = automato->dimensao;

    // Verifica as 8 células vizinhas ao redor da célula atual
    for (int i = linha - 1; i <= linha + 1; i++) {
        for (int j = coluna - 1; j <= coluna + 1; j++) {
            // Verifica se a célula está dentro dos limites do reticulado
            if (i >= 0 && i < dimensao && j >= 0 && j < dimensao) {
                // Ignora a própria célula
                if (i != linha || j != coluna) {
                    // Se a célula vizinha estiver viva, incrementar o contador
                    if (automato->celulas[i][j] == 1) {
                        contador++;
                    }
                }
            }
        }
    }
    return contador;
}

// Funcao para evoluir o reticulado por um número especificado de gerações
void evoluirReticulado(AutomatoCelular* automato, int geracoes) {
    // Verifica se ainda há gerações a serem evoluídas
    if (geracoes > 0) {
        int dimensao = automato->dimensao;

        // Cria uma cópia temporária do reticulado atual para armazenar a próxima geração
        int** proximaGeracao = (int**)malloc(dimensao * sizeof(int*));
        for (int i = 0; i < dimensao; i++) {
            proximaGeracao[i] = (int*)malloc(dimensao * sizeof(int));
        }

        // Itera sobre cada célula do reticulado
        for (int i = 0; i < dimensao; i++) {
            for (int j = 0; j < dimensao; j++) {
                // Conta o número de células vivas vizinhas
                int celulasVivasVizinhas = contarCelulasVivasVizinhas(automato, i, j);

                // Aplica as regras do jogo da vida para determinar o estado da célula na próxima geração
                if (automato->celulas[i][j] == 1) {
                    if (celulasVivasVizinhas < 2 || celulasVivasVizinhas > 3) {
                        // Morte por solidão ou superpopulação
                        proximaGeracao[i][j] = 0;
                    } else {
                        // Célula viva permanece viva
                        proximaGeracao[i][j] = 1;
                    }
                } else {
                    if (celulasVivasVizinhas == 3) {
                        // Renascimento de célula morta
                        proximaGeracao[i][j] = 1;
                    } else {
                        // Célula morta permanece morta
                        proximaGeracao[i][j] = 0;
                    }
                }
            }
        }

        // Atualiza o estado do reticulado para refletir a próxima geração
        for (int i = 0; i < dimensao; i++) {
            for (int j = 0; j < dimensao; j++) {
                automato->celulas[i][j] = proximaGeracao[i][j];
            }
        }

        // Desaloca a memória da cópia temporária
        for (int i = 0; i < dimensao; i++) {
            free(proximaGeracao[i]);
        }
        free(proximaGeracao);

        // Chama recursivamente a função para evoluir as próximas gerações
        evoluirReticulado(automato, geracoes - 1);
    }
}

// Funcao para imprimir o reticulado atual
void imprimeReticulado(ALLEGRO_DISPLAY *display, AutomatoCelular *automato) {
    ALLEGRO_COLOR lineColor = al_map_rgb(0, 0, 225); 
    ALLEGRO_COLOR aliveColor = al_map_rgb(0, 0, 0); // Cor para células vivas (preto)
    ALLEGRO_COLOR deadColor = al_map_rgb(255, 255, 255); // Cor para células mortas (branco)

    
    int celulaWidth = al_get_display_width(display) / automato->dimensao; // Largura da célula
    int celulaHeight = al_get_display_height(display) / automato->dimensao; // Altura da célula

    // Percorre cada célula do reticulado e desenhe na janela
    for (int i = 0; i < automato->dimensao; i++) {
        for (int j = 0; j < automato->dimensao; j++) {
            // Calcula as coordenadas de desenho da célula
            int x = j * celulaWidth;
            int y = i * celulaHeight;

            // Desenha a célula na janela
            ALLEGRO_COLOR color = (automato->celulas[i][j] == 1) ? aliveColor : deadColor;
            al_draw_filled_rectangle(x, y, x + celulaWidth, y + celulaHeight, color);

            // Desenha as linhas horizontais entre as células
            if (i < automato->dimensao - 1) {
                al_draw_line(x, y + celulaHeight, x + celulaWidth, y + celulaHeight, lineColor, 2.0);
            }
        }
        // Desenha as linhas verticais entre as células
        if (i == automato->dimensao - 1) {
            for (int j = 0; j < automato->dimensao - 1; j++) {
                int x = j * celulaWidth;
                int y = i * celulaHeight;
                al_draw_line(x + celulaWidth, y, x + celulaWidth, y + celulaHeight, lineColor, 2.0);
            }
            // Desenha a última linha vertical para fechar o reticulado
            al_draw_line(al_get_display_width(display), 0, al_get_display_width(display), al_get_display_height(display), lineColor, 1.0);
        }
    }

    // Atualiza a tela para exibir as alterações
    al_flip_display();
}
