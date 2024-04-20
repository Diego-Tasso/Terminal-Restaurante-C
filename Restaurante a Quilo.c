/* Restaurante a Quilo - Projeto de Extensão - Código Fonte em C
Integrantes de Grupo:
============================================
- Diego Tasso da Cunha Ferreira : 1230202973
============================================
- Maria Luiza França Mendes : 1230112799
============================================
*/
//Includes do Código
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Enumeração para o tipo de bebidas.
typedef enum {
    Agua,
    Guarana,
    Suco_Natural,
    Coca_Cola,
    Sprite,
    Cafe,
    Cappuccino,
    Cerveja
} TipoBebida;

// Estrutura da bebida
typedef struct {
    TipoBebida tipo;
    char nome[50];
    int quantidade;
    float preco;
    float lucro;
} Bebida;

// Declaração de valores para bebidas específicas.
Bebida bebidas[] = {
    {Agua, "Agua", 0, 1.50, 0.0},      
    {Guarana, "Guarana", 0, 4.00, 0.0},     
    {Suco_Natural, "Suco Natural", 0, 3.00, 0.0}, 
    {Coca_Cola, "Coca-Cola", 0, 4.00, 0.0}, 
    {Sprite, "Sprite", 0, 4.00, 0.0},      
    {Cafe, "Cafe", 0, 2.00, 0.0},       
    {Cappuccino, "Cappuccino", 0, 5.00, 0.0},   
    {Cerveja, "Cerveja", 0, 3.00, 0.0}          
};

// Estrutura dos dados para venda gerais.
typedef struct {
    float valorTotal;
    int dia;
    int mes;
    int ano;
} DadosVenda;

// Estrutura para declaração dos dados das vendas mensais.
typedef struct {
    float valorTotalMes;
    int mes;
    int ano;
} DadosVendaMensal;

// Função / Procedimento para o Carregamento de Dados.
void carregarDados(float *valorTotal, int *dia, int *mes, int *ano) {
    char nomeArquivo[30]; 
    
    //Questionamento para abertura da função do relatório baseado na data do arquivo .dat citado.
    printf("Digite a data (dia mes ano): ");
    scanf("%d %d %d", dia, mes, ano);
    sprintf(nomeArquivo, "vendas_%02d_%02d_%04d.dat", *dia, *mes, *ano);
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "rb");

    // Tratamento de erros...
    if (arquivo == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    // Atualização dos valores das vendas e fechamento do arquivo.
    DadosVenda dados;
    fread(&dados, sizeof(DadosVenda), 1, arquivo); 
    *valorTotal = dados.valorTotal; 

    fclose(arquivo);
}

// Função / Procedimento salvar os dados e somar com os dados existentes no arquivo .dat baseado na data.
// Função / Procedimento salvar os dados e somar com os dados existentes no arquivo .dat baseado na data.
void salvarDados(float valorTotal, int dia, int mes, int ano) {
    char nomeArquivo[30];

    // Criação do nome do arquivo
    sprintf(nomeArquivo, "vendas_%02d_%02d_%04d.dat", dia, mes, ano);
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "rb+"); // Modo de abertura para leitura e escrita

    // Tratamento de erros
    if (arquivo == NULL) {
        // Se o arquivo não existir, cria um novo
        arquivo = fopen(nomeArquivo, "wb"); // Modo de abertura para escrita binária

        // Verificação novamente para garantir que o arquivo foi criado
        if (arquivo == NULL) {
            printf("Erro ao criar o arquivo para escrita.\n");
            return;
        }

        // Escreve os dados iniciais no arquivo
        DadosVenda dados;
        dados.valorTotal = valorTotal;
        dados.dia = dia;
        dados.mes = mes;
        dados.ano = ano;
        fwrite(&dados, sizeof(DadosVenda), 1, arquivo);
    } else {
        // Se o arquivo já existir, atualiza os valores das vendas
        DadosVenda dados;

        // Lê os dados existentes
        fread(&dados, sizeof(DadosVenda), 1, arquivo);

        // Soma o valor total ao existente
        dados.valorTotal += valorTotal;

        // Move o ponteiro para o início do arquivo para escrever os dados corretamente
        fseek(arquivo, 0, SEEK_SET);
 
        // Escreve os dados atualizados e fecha o arquivo
        fwrite(&dados, sizeof(DadosVenda), 1, arquivo);
    }

    fclose(arquivo);
}



// Função / Procedimento de Cálculo do valor total obtido em um mes
void calcularTotalMensal(int mes, int ano, float *valorTotalMensal) {
    float total = 0.0;

    // Loop para cálculo do valor mensal com base nas vendas diárias.
    for (int dia = 1; dia <= 31; dia++) {
        char nomeArquivo[30];
        sprintf(nomeArquivo, "vendas_%02d_%02d_%04d.dat", dia, mes, ano);
        FILE *arquivo = fopen(nomeArquivo, "rb");
        if (arquivo != NULL) {
            DadosVenda dados;
            fread(&dados, sizeof(DadosVenda), 1, arquivo);
            total += dados.valorTotal;
            fclose(arquivo);
        }
    }
    // Obtendo o total mensal baseado no Loop
    *valorTotalMensal = total;
}

//Função / Procedimento de geração do relatório diário.
void relatorioDiario() {
    system("CLS");
    int dia, mes, ano;

    // Busca pelo dia específico
    printf("Digite a data (dia mes ano): ");
    scanf("%d %d %d", &dia, &mes, &ano);

    char nomeArquivo[30];
    sprintf(nomeArquivo, "vendas_%02d_%02d_%04d.dat", dia, mes, ano);
    FILE *arquivo = fopen(nomeArquivo, "rb");

    //Tratamento de erros
    if (arquivo == NULL) {
        printf("Arquivo de dados nao encontrado.\n");
        system("PAUSE");
        return;
    }

    float valorTotal;
    fread(&valorTotal, sizeof(float), 1, arquivo);

    int numBebidasVendidas = 0;
    for (size_t i = 0; i < sizeof(bebidas) / sizeof(Bebida); i++) {
        if (bebidas[i].quantidade > 0) {
            numBebidasVendidas += bebidas[i].quantidade;
        }
    }
    // Menu e Relatório
    printf("=======================\n");
    printf("|   Relatorio Diario  |\n");
    printf("=======================\n");
    printf("Data: %02d/%02d/%04d\n", dia, mes, ano);
    printf("Valor total das vendas: %.2f\n\n", valorTotal);

    fclose(arquivo);
    system("PAUSE");
}

//Função / Procedimento para cálculo de valor mensal recebendo mes e ano como parâmetro.
void relatorioMensal(int mes, int ano) {
    float valorTotalMensal;
    calcularTotalMensal(mes, ano, &valorTotalMensal);
    printf("Valor total das vendas no mes %02d/%04d: %.2f\n", mes, ano, valorTotalMensal);
    system ("PAUSE");
}

//Função / Procedimento para cálcular o relatório anual.
void relatorioAnual() {
    system("CLS");
    int ano;

    //Busca pelo ano específico
    printf("Digite o ano para o relatorio anual: ");
    scanf("%d", &ano);

    printf("======================\n");
    printf("|   Relatorio Anual  |\n");
    printf("======================\n");
    printf("Ano: %04d\n", ano);

    DadosVendaMensal dadosMensais[12]; // Array para armazenar os dados mensais
    float valorTotalAnual = 0.0;

    //Loop para cálculo do total anual.
    for (int mes = 1; mes <= 12; mes++) {
        float valorTotalMensal;
        calcularTotalMensal(mes, ano, &valorTotalMensal);
        dadosMensais[mes - 1].valorTotalMes = valorTotalMensal;
        dadosMensais[mes - 1].mes = mes;
        dadosMensais[mes - 1].ano = ano;
        valorTotalAnual += valorTotalMensal;
    }

    // Loop para ordenação em ordem decrescente.
    for (int i = 0; i < 12; i++) {
        for (int j = i + 1; j < 12; j++) {
            if (dadosMensais[i].valorTotalMes < dadosMensais[j].valorTotalMes) {
                DadosVendaMensal temp = dadosMensais[i];
                dadosMensais[i] = dadosMensais[j];
                dadosMensais[j] = temp;
            }
        }
    }

    for (int i = 0; i < 12; i++) {
        printf("Mes %02d/%04d: %.2f\n", dadosMensais[i].mes, dadosMensais[i].ano, dadosMensais[i].valorTotalMes);
    }

    printf("Valor total anual: %.2f\n", valorTotalAnual);
    system("PAUSE");
}

// Cardápio de bebidas baseado na enumeração para facilitar que os dados sejam salvos.
void exibirCardapio() {
    printf("\n============== Cardapio =============\n");
    printf("| 1 - Agua              - R$ 1.50    |\n");
    printf("| 2 - Guarana           - R$ 4.00    |\n");
    printf("| 3 - Suco Natural      - R$ 3.00    |\n");
    printf("| 4 - Coca-Cola         - R$ 4.00    |\n");
    printf("| 5 - Sprite            - R$ 4.00    |\n");
    printf("| 6 - Cafe              - R$ 2.00    |\n");
    printf("| 7 - Cappuccino        - R$ 5.00    |\n");
    printf("| 8 - Cerveja           - R$ 3.00    |\n");
    printf("======================================\n");
}

// Função / Procedimento para calcular o lucro com um prato a quilo.
void aQuilo(float *vt) {
    int qVendas = 10;
    system("CLS");
    printf("============================================\n");
    printf("|   Quantas vendas voce deseja registrar?  |\n");
    printf("============================================\n");  
    printf("---> ");
    scanf("%d", &qVendas);

    // Tratamento de erros
    if (qVendas <= 0) {
        printf("Numero invalido de vendas.\n");
        return; 
    }

    // Alocando memória para armazenar o peso dos pratos vendidos, do valor e da bebida.
    float *pesoPratos = malloc(qVendas * sizeof(float));
    float *valorObtido = malloc(qVendas * sizeof(float));
    int bebidaEscolhida[qVendas];
    char Resp;

    //  Tratamento de Erros
    if (pesoPratos == NULL || valorObtido == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    for (int i = 0; i < qVendas; i++) {
        bebidaEscolhida[i] = -1;
    }

    // Loop para que o usuário armazene o peso dos pratos e o valor lucrado com as bebidas.
    for (int i = 0; i < qVendas; i++) {
        printf("Digite o peso em gramas do prato %d: ", i + 1);
        scanf("%f", &pesoPratos[i]);

        printf("O cliente %d pediu alguma bebida? [S] ou [N]: ", i + 1);
        scanf(" %c", &Resp);

        // Condicional para o pedido de bebida.
        if (Resp == 'S' || Resp == 's') {
            exibirCardapio();
            printf("Escolha a bebida (1 a 8): ");
            scanf("%d", &bebidaEscolhida[i]);
            bebidaEscolhida[i]--;

            if (bebidaEscolhida[i] < 0 || bebidaEscolhida[i] >= 8) {
                printf("Opção inválida.\n");
                free(pesoPratos);
                free(valorObtido);
                system("PAUSE");
                return;
            }

            printf("Quantas unidades de %s foram pedidas? ", bebidas[bebidaEscolhida[i]].nome);
            scanf("%d", &bebidas[bebidaEscolhida[i]].quantidade);
            printf("O cliente pediu %d unidades de %s.\n", bebidas[bebidaEscolhida[i]].quantidade, bebidas[bebidaEscolhida[i]].nome);
        }  

        // Condicional para o pedido de bebida.
        else {
            printf("O cliente nao pediu nenhuma bebida.\n");

            if (bebidaEscolhida[i] != -1) {
                bebidas[bebidaEscolhida[i]].quantidade = 0;
            }
        }

        // Cálculo do lucro obtido armazenando na variável valor total com condicional para tratamento de erros.
        int quantidadeBebida = bebidas[bebidaEscolhida[i]].quantidade;
        if (bebidaEscolhida[i] != -1) {
            bebidas[bebidaEscolhida[i]].lucro = bebidas[bebidaEscolhida[i]].preco * quantidadeBebida;
            valorObtido[i] = pesoPratos[i] * 0.07 + bebidas[bebidaEscolhida[i]].preco;
            printf("O valor lucrado com as bebidas foi --> %.2f\n", bebidas[bebidaEscolhida[i]].lucro);
            printf("O valor do prato %d -->  %.2f\n", i + 1, valorObtido[i]);
            *vt += valorObtido[i] + bebidas[bebidaEscolhida[i]].lucro;
        }
        else {
            bebidas[bebidaEscolhida[i]].lucro = 0;
            valorObtido[i] = pesoPratos[i] * 0.07; 
            printf("O valor do prato %d -->  %.2f\n", i + 1, valorObtido[i]);
            *vt += valorObtido[i];
        }
    }

    //Armazenamento do .dat do valor total obtido no dia.
    int dia, mes, ano;
    printf("Digite a data (dia mes ano): ");
    scanf("%d %d %d", &dia, &mes, &ano);
    salvarDados(*vt, dia, mes, ano);
    free(pesoPratos);
    free(valorObtido);
    system ("PAUSE");
}

//Função / Procedimento para calcular o lucro obtido com as quentinhas.
void Quentinhas(float *vt) {
    int qVendas;
    system("CLS");
    printf("===========================================================\n");
    printf("|   De quantas quentinhas voce deseja registrar a venda?  |\n");
    printf("===========================================================\n");
    printf("---> ");
    scanf("%d", &qVendas);

    // Tratamento de erro.
    if (qVendas <= 0) {
        printf("Numero invalido de quentinhas.\n");
        return;
    }

    // Alocando memória para o valor obtido e a bebida.
    float *valorObtido = malloc(qVendas * sizeof(float));
    int bebidaEscolhida[qVendas];
    char Resp;

    // Condicional para tratamento de erros.
    if (valorObtido == NULL) {
        printf("Erro!\n");
        return;
    }

    // Tratamento de erros.
    for (int i = 0; i < qVendas; i++) {
        bebidaEscolhida[i] = -1;
    }

    // Loop com condicional para a escolha de bebidas e armazenamento do valor total obtido com a quentinha considerando o valor de 20 com a embalagem de 0.5
    for (int i = 0; i < qVendas; i++) {
        printf("O cliente %d pediu alguma bebida? [S] ou [N]: ", i + 1);
        scanf(" %c", &Resp);

        if (Resp == 'S' || Resp == 's') {
            exibirCardapio();
            printf("Escolha a bebida (1 a 8): ");
            scanf("%d", &bebidaEscolhida[i]);
            bebidaEscolhida[i]--;

            if (bebidaEscolhida[i] < 0 || bebidaEscolhida[i] >= 8) {
                printf("Opção inválida.\n");
                free(valorObtido);
                system("PAUSE");
                return;
            }

            printf("Quantas unidades de %s foram pedidas? ", bebidas[bebidaEscolhida[i]].nome);
            scanf("%d", &bebidas[bebidaEscolhida[i]].quantidade);
            printf("O cliente pediu %d unidades de %s.\n", bebidas[bebidaEscolhida[i]].quantidade, bebidas[bebidaEscolhida[i]].nome);
        }  
        else {
            printf("O cliente nao pediu nenhuma bebida.\n");

            if (bebidaEscolhida[i] != -1) {
                bebidas[bebidaEscolhida[i]].quantidade = 0;
            }
        }

        if (bebidaEscolhida[i] != -1) {
            bebidas[bebidaEscolhida[i]].lucro = bebidas[bebidaEscolhida[i]].preco * bebidas[bebidaEscolhida[i]].quantidade;
            valorObtido[i] = 20.50 + bebidas[bebidaEscolhida[i]].lucro;
            printf("O valor lucrado com as bebidas foi --> %.2f\n", bebidas[bebidaEscolhida[i]].lucro);
            printf("O valor da quentinha %d -->  %.2f\n", i + 1, valorObtido[i]);
            *vt += valorObtido[i];
        }
        else {
            valorObtido[i] = 20.50;
            printf("O valor da quentinha %d -->  %.2f\n", i + 1, valorObtido[i]);
            *vt += valorObtido[i];
        }
    }

    // Colocando no dia .dat o valor obtido.
    int dia, mes, ano;
    printf("Digite a data (dia mes ano): ");
    scanf("%d %d %d", &dia, &mes, &ano);
    salvarDados(*vt, dia, mes, ano);
    free(valorObtido);
    system ("PAUSE");
}

// Função principal com menu de opções
int main() {
    float valorTotal = 0.0;
    int opcao;
    do {
        system("CLS");
        printf("\n============== Menu ===============\n");
        printf("| 1 - A Quilo                     |\n");
        printf("| 2 - Quentinhas                  |\n");
        printf("| 3 - Relatorio de Vendas Diario  |\n");
        printf("| 4 - Relatorio de Vendas Mensal  |\n");
        printf("| 5 - Relatorio Anual | Ordenado  |\n");
        printf("| 6 - Sair                        |\n");
        printf("===================================\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);
        switch(opcao) {
            case 1:
                // Puxando Função aQuilo
                aQuilo(&valorTotal);
                break;
            case 2:
                // Puxando Função Quentinhas
                Quentinhas(&valorTotal);
                break;
            case 3:
                // Puxando Procedimento do Relatório Diário
                relatorioDiario();
                break;
            case 4:
                {
                    // Enviando os parametros e abrindo o menu da função relatório mensal.
                    int mes, ano;
                    system ("CLS");
                    printf("=======================\n");
                    printf("|   Relatorio Mensal  |\n");
                    printf("=======================\n");
                    printf("Digite o mes e o ano para o relatorio mensal (mes ano): ");
                    scanf("%d %d", &mes, &ano);
                    relatorioMensal(mes, ano);
                    break;
                }
            case 5:
                // Puxando Procedimento do Relatório Anual
                relatorioAnual();
                break;
            case 6:
                // Saíndo do Programa e finalizando o while.
                printf("Saindo...\n");
                break;
            default:
                // Tratamento de erros.
                printf("Opcao invalida!\n");
                system("PAUSE");
        }
    } while (opcao != 6);

    return 0;
}
