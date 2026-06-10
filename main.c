#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int numeroConta;
    char nome[50];
    float saldo;
    int ativo;
} Cliente;

void cadastrar(FILE *arq) {
    Cliente c;
    int pos;

    printf("Posicao: ");
    scanf("%d", &pos);

    printf("Numero da conta: ");
    scanf("%d", &c.numeroConta);

    printf("Nome: ");
    scanf(" %[^\n]", c.nome);

    printf("Saldo: ");
    scanf("%f", &c.saldo);

    c.ativo = 1;

    fseek(arq, pos * sizeof(Cliente), SEEK_SET);
    fwrite(&c, sizeof(Cliente), 1, arq);

    printf("Cliente cadastrado!\n");
}

void consultar(FILE *arq) {
    Cliente c;
    int conta;
    int achou = 0;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numeroConta == conta) {
            printf("\nConta: %d\n", c.numeroConta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: %.2f\n", c.saldo);
            achou = 1;
            break;
        }
    }

    if (!achou)
        printf("Cliente nao encontrado!\n");
}

void atualizar(FILE *arq) {
    Cliente c;
    int conta;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numeroConta == conta) {
            printf("Novo saldo: ");
            scanf("%f", &c.saldo);

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);

            printf("Saldo atualizado!\n");
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void removerCliente(FILE *arq) {
    Cliente c;
    int conta;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numeroConta == conta) {
            c.ativo = 0;

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);

            printf("Conta encerrada!\n");
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void listar(FILE *arq) {
    Cliente c;

    rewind(arq);

    printf("\n--- CLIENTES ---\n");

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo) {
            printf("Conta: %d\n", c.numeroConta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: %.2f\n\n", c.saldo);
        }
    }
}

int main() {
    FILE *arq;
    int opcao;

    arq = fopen("contas.dat", "r+b");

    if (arq == NULL) {
        arq = fopen("contas.dat", "w+b");
    }

    do {
        printf("\n1 - Cadastrar cliente\n");
        printf("2 - Consultar cliente\n");
        printf("3 - Atualizar saldo\n");
        printf("4 - Encerrar conta\n");
        printf("5 - Listar clientes\n");
        printf("6 - Rewind e listar novamente\n");
        printf("7 - Encerrar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar(arq);
                break;

            case 2:
                consultar(arq);
                break;

            case 3:
                atualizar(arq);
                break;

            case 4:
                removerCliente(arq);
                break;

            case 5:
                listar(arq);
                break;

            case 6:
                rewind(arq);
                listar(arq);
                break;

            case 7:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 7);

    fclose(arq);

    return 0;
}