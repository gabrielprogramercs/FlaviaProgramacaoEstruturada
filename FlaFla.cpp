
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

using namespace std;

struct reg
{
    char paciente[50];
    char cpf[15];
    char telefone[20];
    char medico[50];
    char especialidade[40];
    char data[15];
    char horario[10];
    char status;
};

int tamanho(FILE *arq);
int ativos(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);
void geraarqtxt(FILE *arq);
void excluir(FILE *arq);
void pausa();

int main()
{
    int op;
    FILE *arq;

    if ((arq = fopen("consultas.dat", "rb+")) == NULL)
    {
        if ((arq = fopen("consultas.dat", "wb+")) == NULL)
        {
            printf("Falha ao abrir o arquivo!\n");
            system("pause");
            return 1;
        }
    }

    do
    {
        system("CLS");

        printf("\n======= AGENDAMENTO MEDICO =======\n");
        printf("1. Cadastrar consulta\n");
        printf("2. Consultar por codigo\n");
        printf("3. Exportar arquivo TXT\n");
        printf("4. Excluir consulta\n");
        printf("5. Sair\n");

        printf("\nConsultas ativas: %d", ativos(arq));
        printf("\nTotal armazenado: %d", tamanho(arq));

        printf("\n==================================\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();

        switch (op)
        {
            case 1:
                cadastrar(arq);
                break;

            case 2:
                consultar(arq);
                break;

            case 3:
                geraarqtxt(arq);
                break;

            case 4:
                excluir(arq);
                break;

            case 5:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                pausa();
        }

    } while (op != 5);

    fclose(arq);

    return 0;
}


int tamanho(FILE *arq)
{
    fseek(arq, 0, SEEK_END);

    return ftell(arq) / sizeof(reg);
}


int ativos(FILE *arq)
{
    reg consulta;
    int cont = 0;
    int i;

    for (i = 0; i < tamanho(arq); i++)
    {
        fseek(arq, i * sizeof(reg), SEEK_SET);
        fread(&consulta, sizeof(reg), 1, arq);

        if (consulta.status == ' ')
        {
            cont++;
        }
    }

    return cont;
}


void cadastrar(FILE *arq)
{
    reg consulta;
    int codigo;

    printf("\n======= CADASTRAR CONSULTA =======\n");

    codigo = tamanho(arq) + 1;

    printf("\nCodigo da consulta: %d\n", codigo);

    printf("Nome do paciente: ");
    fgets(consulta.paciente, 50, stdin);
    consulta.paciente[strcspn(consulta.paciente, "\n")] = '\0';

    printf("CPF: ");
    fgets(consulta.cpf, 15, stdin);
    consulta.cpf[strcspn(consulta.cpf, "\n")] = '\0';

    printf("Telefone: ");
    fgets(consulta.telefone, 20, stdin);
    consulta.telefone[strcspn(consulta.telefone, "\n")] = '\0';

    printf("Nome do medico: ");
    fgets(consulta.medico, 50, stdin);
    consulta.medico[strcspn(consulta.medico, "\n")] = '\0';

    printf("Especialidade: ");
    fgets(consulta.especialidade, 40, stdin);
    consulta.especialidade[strcspn(consulta.especialidade, "\n")] = '\0';

    printf("Data (DD/MM/AAAA): ");
    fgets(consulta.data, 15, stdin);
    consulta.data[strcspn(consulta.data, "\n")] = '\0';

    printf("Horario (HH:MM): ");
    fgets(consulta.horario, 10, stdin);
    consulta.horario[strcspn(consulta.horario, "\n")] = '\0';

    // Espaco indica que o registro esta ativo
    consulta.status = ' ';

    printf("\nGravando consulta...\n");

    fseek(arq, 0, SEEK_END);
    fwrite(&consulta, sizeof(reg), 1, arq);

    printf("\nConsulta cadastrada com sucesso!\n");

    pausa();
}


void consultar(FILE *arq)
{
    reg consulta;
    int nr;

    printf("\n======= CONSULTAR CONSULTA =======\n");

    printf("\nInforme o codigo: ");
    scanf("%d", &nr);
    getchar();

    if (nr > 0 && nr <= tamanho(arq))
    {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        fread(&consulta, sizeof(reg), 1, arq);

        if (consulta.status == ' ')
        {
            printf("\nCodigo: %d", nr);
            printf("\nPaciente: %s", consulta.paciente);
            printf("\nCPF: %s", consulta.cpf);
            printf("\nTelefone: %s", consulta.telefone);
            printf("\nMedico: %s", consulta.medico);
            printf("\nEspecialidade: %s", consulta.especialidade);
            printf("\nData: %s", consulta.data);
            printf("\nHorario: %s\n", consulta.horario);
        }
        else
        {
            printf("\nEsta consulta foi excluida!\n");
        }
    }
    else
    {
        printf("\nCodigo de consulta invalido!\n");
    }

    pausa();
}

void geraarqtxt(FILE *arq)
{
    char nomearq[50];

    printf("\n======= EXPORTAR CONSULTAS =======\n");

    printf("Nome do arquivo TXT: ");
    fgets(nomearq, 50, stdin);
    nomearq[strcspn(nomearq, "\n")] = '\0';

    strcat(nomearq, ".txt");

    FILE *arqtxt = fopen(nomearq, "w");

    if (arqtxt == NULL)
    {
        printf("\nNao foi possivel criar o arquivo!\n");
        pausa();
        return;
    }

    reg consulta;
    int nr;

    fprintf(arqtxt, "========== AGENDA MEDICA ==========\n\n");

    for (nr = 0; nr < tamanho(arq); nr++)
    {
        fseek(arq, nr * sizeof(reg), SEEK_SET);
        fread(&consulta, sizeof(reg), 1, arq);

        fprintf(arqtxt, "Codigo: %d\n", nr + 1);
        fprintf(arqtxt, "Paciente: %s\n", consulta.paciente);
        fprintf(arqtxt, "CPF: %s\n", consulta.cpf);
        fprintf(arqtxt, "Telefone: %s\n", consulta.telefone);
        fprintf(arqtxt, "Medico: %s\n", consulta.medico);
        fprintf(arqtxt, "Especialidade: %s\n", consulta.especialidade);
        fprintf(arqtxt, "Data: %s\n", consulta.data);
        fprintf(arqtxt, "Horario: %s\n", consulta.horario);

        if (consulta.status == ' ')
        {
            fprintf(arqtxt, "Status: ATIVO\n");
        }
        else
        {
            fprintf(arqtxt, "Status: EXCLUIDO\n");
        }

        fprintf(arqtxt, "-----------------------------------\n");
    }

    fclose(arqtxt);

    printf("\nArquivo exportado com sucesso!\n");

    pausa();
}

void excluir(FILE *arq)
{
    reg consulta;
    int nr;
    char confirma;

    printf("\n======= EXCLUIR CONSULTA =======\n");

    printf("\nInforme o codigo da consulta: ");
    scanf("%d", &nr);
    getchar();

    if (nr > 0 && nr <= tamanho(arq))
    {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        fread(&consulta, sizeof(reg), 1, arq);

        if (consulta.status == ' ')
        {
            printf("\nPaciente: %s", consulta.paciente);
            printf("\nMedico: %s", consulta.medico);
            printf("\nData: %s", consulta.data);
            printf("\nHorario: %s", consulta.horario);

            printf("\n\nConfirma a exclusao? (S/N): ");
            scanf("%c", &confirma);
            getchar();

            if (toupper(confirma) == 'S')
            {
                consulta.status = '*';

                fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
                fwrite(&consulta, sizeof(reg), 1, arq);

                printf("\nConsulta excluida com sucesso!\n");
            }
            else
            {
                printf("\nExclusao cancelada!\n");
            }
        }
        else
        {
            printf("\nEsta consulta ja foi excluida!\n");
        }
    }
    else
    {
        printf("\nCodigo invalido!\n");
    }

    pausa();
}


void pausa()
{
    printf("\n");
    system("pause");
}
