#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_STRING 100
#define QTD_TERRITORIOS 5

typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

typedef struct {
    int tipo; // 1 = derrotar território, 2 = eliminar cor, 3 = atingir tropas
    char descricao[TAM_STRING];
    int atacante_idx;
    int defensor_idx;
    char alvo_cor[TAM_STRING];
    int qtdObjetivo;
} Missao;

/* ---------- Funções utilitárias ---------- */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validarIndice(int idx, int qtd) {
    return idx >= 0 && idx < qtd;
}

/* ---------- Criação e exibição ---------- */
Territorio *criarMapa(int qtd) {
    Territorio *mapa = (Territorio *)calloc(qtd, sizeof(Territorio));
    if (!mapa) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    printf("===== CRIAÇÃO DO MAPA =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: ");
        fgets(mapa[i].nome, TAM_STRING, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor (dono): ");
        fgets(mapa[i].cor, TAM_STRING, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            limparBufferEntrada();
            printf("Valor inválido. Digite novamente: ");
        }
        limparBufferEntrada();
    }

    printf("\nMapa criado com sucesso!\n");
    return mapa;
}

void exibirMapa(const Territorio *mapa, int qtd) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d) %-15s | Cor: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=================================\n");
}

/* ---------- Mecânica de ataque ---------- */
int rolarDado(void) {
    return (rand() % 6) + 1;
}

void simularAtaque(Territorio *mapa, int qtd) {
    exibirMapa(mapa, qtd);

    int atacante, defensor;
    printf("\n-- FASE DE ATAQUE --\n");
    printf("Escolha o número do território ATACANTE: ");
    if (scanf("%d", &atacante) != 1) { limparBufferEntrada(); return; }
    limparBufferEntrada();

    printf("Escolha o número do território DEFENSOR: ");
    if (scanf("%d", &defensor) != 1) { limparBufferEntrada(); return; }
    limparBufferEntrada();

    atacante--; defensor--;
    if (!validarIndice(atacante, qtd) || !validarIndice(defensor, qtd) || atacante == defensor) {
        printf("Seleção inválida.\n");
        return;
    }

    printf("\n%s (Tropas: %d) ATACA %s (Tropas: %d)\n",
           mapa[atacante].nome, mapa[atacante].tropas,
           mapa[defensor].nome, mapa[defensor].tropas);

    int dadoA = rolarDado();
    int dadoD = rolarDado();

    printf("Dado do ATACANTE: %d\n", dadoA);
    printf("Dado do DEFENSOR: %d\n", dadoD);

    if (dadoA > dadoD) {
        printf("Vitória do atacante! Ganha +1 tropa. Defensor perde 1 tropa.\n");
        mapa[atacante].tropas++;
        mapa[defensor].tropas--;

        if (mapa[defensor].tropas <= 0) {
            printf(">>> %s foi CONQUISTADO por %s!\n", mapa[defensor].nome, mapa[atacante].nome);
            strcpy(mapa[defensor].cor, mapa[atacante].cor);
            mapa[defensor].tropas = 1;
        }
    } else if (dadoA < dadoD) {
        printf("Vitória do defensor! Ganha +1 tropa. Atacante perde 1 tropa.\n");
        mapa[defensor].tropas++;
        mapa[atacante].tropas--;

        if (mapa[atacante].tropas < 0)
            mapa[atacante].tropas = 0;
    } else {
        printf("Empate! Nenhum território sofre alteração.\n");
    }

    exibirMapa(mapa, qtd);
}

/* ---------- Missões ---------- */
Missao sortearMissao(const Territorio *mapa, int qtd) {
    Missao m;
    memset(&m, 0, sizeof(Missao));
    m.tipo = (rand() % 3) + 1;

    if (m.tipo == 1) {
        int a = rand() % qtd, b;
        do { b = rand() % qtd; } while (b == a);
        m.atacante_idx = a;
        m.defensor_idx = b;
        snprintf(m.descricao, TAM_STRING,
                 "Derrote o território '%s' usando o território '%s'.",
                 mapa[b].nome, mapa[a].nome);
    } else if (m.tipo == 2) {
        int i = rand() % qtd;
        strcpy(m.alvo_cor, mapa[i].cor);
        snprintf(m.descricao, TAM_STRING,
                 "Elimine todos os territórios da cor '%s'.", m.alvo_cor);
    } else {
        int alvo = 5 + rand() % 5; // entre 5 e 9
        m.qtdObjetivo = alvo;
        snprintf(m.descricao, TAM_STRING,
                 "Faça algum território atingir pelo menos %d tropas.", alvo);
    }
    return m;
}

void exibirMissao(const Missao *m) {
    printf("\n===== MISSÃO =====\n%s\n==================\n", m->descricao);
}

int verificarVitoria(const Territorio *mapa, int qtd, const Missao *m) {
    if (m->tipo == 1) {
        return mapa[m->defensor_idx].tropas <= 0;
    } else if (m->tipo == 2) {
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, m->alvo_cor) == 0 && mapa[i].tropas > 0)
                return 0;
        }
        return 1;
    } else if (m->tipo == 3) {
        for (int i = 0; i < qtd; i++) {
            if (mapa[i].tropas >= m->qtdObjetivo)
                return 1;
        }
    }
    return 0;
}

/* ---------- Principal ---------- */
int main() {
    srand((unsigned)time(NULL));
    Territorio *mapa = criarMapa(QTD_TERRITORIOS);
    Missao missao = sortearMissao(mapa, QTD_TERRITORIOS);

    printf("\nMissão sorteada automaticamente:\n");
    exibirMissao(&missao);

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1 - Atacar\n");
        printf("2 - Exibir mapa\n");
        printf("3 - Ver missão\n");
        printf("4 - Verificar vitória\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { limparBufferEntrada(); opcao = -1; }
        limparBufferEntrada();

        switch (opcao) {
            case 1: simularAtaque(mapa, QTD_TERRITORIOS); break;
            case 2: exibirMapa(mapa, QTD_TERRITORIOS); break;
            case 3: exibirMissao(&missao); break;
            case 4:
                if (verificarVitoria(mapa, QTD_TERRITORIOS, &missao))
                    printf("\n🎉 MISSÃO CUMPRIDA!\n");
                else
                    printf("\nAinda não cumpriu a missão.\n");
                break;
            case 0: printf("Saindo do jogo...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    free(mapa);
    return 0;
}
