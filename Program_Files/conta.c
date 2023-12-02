#include "conta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variáveis globais para a conta atual e o array de contas
Conta *currentAccount = NULL;
Conta *contas = NULL;

// Inicializa o sistema bancário alocando espaço para o array de contas
void inicializar() { contas = malloc(MAX * sizeof(Conta)); }


void menuPrincipal(){

    int opcao = 0;
    printf("Bem-vindos ao nosso sistema bancario. \n");
    do {
        removeLinhasEmBranco("usuarios.txt");

        printf("Menu Principal\n");
        printf("1. Realizar login \n");
        printf("2. Cadastrar nova conta\n");
        printf("3. Listar todas contas\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                opcao = 9;
                // Para sair do loop e chamar a função de login
                break;
            case 2: {
                // Verifica se é possível cadastrar uma nova conta
                int activeAccounts = getActiveAccounts();
                if (activeAccounts >= 0 && activeAccounts < MAX || activeAccounts == 100) {
                    // Insere uma nova conta
                    insert(contas, activeAccounts);
                } else if (activeAccounts < 0) {
                    printf("Sistema em Manutencao. Retorne em outro momento");
                } else {
                    printf("Limite excedido!");
                }
            }
                break;
            case 3: {
                // Lista todas as contas existentes
                int activeAccounts = getActiveAccounts();
                listAccounts(contas, activeAccounts);
            }
                break;
            default:
                printf("Opcao inexistente. \n");
        }

    } while (opcao != 9);

    if (opcao == 9) {
        login();
    }
}



// Remove linhas em branco de um arquivo
void removeLinhasEmBranco(char *nomeArquivo) {
  FILE *arquivoEntrada, *arquivoSaida;
  char linha[255];

  arquivoEntrada = fopen(nomeArquivo, "r");

  if (arquivoEntrada == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  arquivoSaida = fopen("temp.txt", "w");

  if (arquivoSaida == NULL) {
    printf("Erro ao abrir o arquivo temporário.\n");
    fclose(arquivoEntrada);
    return;
  }

  while (fgets(linha, sizeof(linha), arquivoEntrada)) {
    if (strlen(linha) > 1) {
      fprintf(arquivoSaida, "%s", linha);
    }
  }

  fclose(arquivoEntrada);
  fclose(arquivoSaida);

  remove(nomeArquivo);
  rename("temp.txt", nomeArquivo);
}

// Função do menu que exibe as opções disponíveis
void menu(int numeroConta) {
  int opcao = 0;

  opcoesLogin(numeroConta);
  scanf("%d", &opcao);

  switch (opcao) {
  case 1: {
    float deposito = 0;
    printf("Quanto deseja depositar?");
    scanf("%f", &deposito);
    depositarDinheiro(numeroConta, deposito);
      return menu(numeroConta);
  } break;
  case 2: {
    float saque = 0;
    printf("Quanto deseja sacar?");
    scanf("%f", &saque);
    sacarDinheiro(numeroConta, saque);
      return menu(numeroConta);
  } break;
  case 3:
    consultaInformacao(numeroConta);
          return menu(numeroConta);
    break;
  case 4:
    exit(0);
    break;
  default:
    printf("Opcao invalida.\n");
    break;
  }
}

// Função que realiza um depósito na conta especificada
void depositarDinheiro(int numeroConta, float qtd) {

  FILE *filePointer, *tempFilePointer;
  int bufferLength = 255;
  char buffer[bufferLength];
  char linhaOriginal[bufferLength];

  filePointer = fopen("usuarios.txt", "r");

  if (filePointer == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  tempFilePointer = fopen("temp.txt", "w");

  if (tempFilePointer == NULL) {
    printf("Erro ao abrir o arquivo temporário.\n");
    fclose(filePointer);
    return;
  }

  while (fgets(buffer, bufferLength, filePointer)) {
    strcpy(linhaOriginal, buffer);

    buffer[strcspn(buffer, "\n")] = '\0';

    char *numeroContaStr = strtok(buffer, " ");
    char *senha1 = strtok(NULL, " ");
    char *saldoStr = strtok(NULL, " ");
    char *nome = strtok(NULL, " ");
    char *ativoStr = strtok(NULL, " ");
    char *especialStr = strtok(NULL, " ");

    int accountNumber = atoi(numeroContaStr);
    float saldo = atof(saldoStr);
    int ativo = atoi(ativoStr);
    int especial = atoi(especialStr);

    if (accountNumber == numeroConta) {
      saldo += qtd;

      sprintf(linhaOriginal, "%d %s %.4f %s %s %s", accountNumber, senha1,
              saldo, nome, especialStr, ativoStr);
    }

    if (strcmp(linhaOriginal, buffer) != 0) {
      fprintf(tempFilePointer, "%s\n", linhaOriginal);
    } else {
      fprintf(tempFilePointer, "%s", buffer);
    }
  }

  fclose(filePointer);
  fclose(tempFilePointer);

  remove("usuarios.txt");
  rename("temp.txt", "usuarios.txt");

  printf("Depósito realizado com sucesso!\n");

}

// Função que realiza um saque na conta especificada
void sacarDinheiro(int numeroConta, float qtd) {

  FILE *filePointer, *tempFilePointer;
  int bufferLength = 255;
  char buffer[bufferLength];
  char linhaOriginal[bufferLength];

  filePointer = fopen("usuarios.txt", "r");

  if (filePointer == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  tempFilePointer = fopen("temp.txt", "w");

  if (tempFilePointer == NULL) {
    printf("Erro ao abrir o arquivo temporário.\n");
    fclose(filePointer);
    return;
  }

  while (fgets(buffer, bufferLength, filePointer)) {

    strcpy(linhaOriginal, buffer);

    buffer[strcspn(buffer, "\n")] = '\0';

    char *numeroContaStr = strtok(buffer, " ");
    char *senha1 = strtok(NULL, " ");
    char *saldoStr = strtok(NULL, " ");
    char *nome = strtok(NULL, " ");
    char *ativoStr = strtok(NULL, " ");
    char *especialStr = strtok(NULL, " ");

    int accountNumber = atoi(numeroContaStr);
    float saldo = atof(saldoStr);
    int ativo = atoi(ativoStr);
    int especial = atoi(especialStr);

    if (accountNumber == numeroConta) {

      if (saldo >= qtd) {
        saldo = saldo - qtd;
        printf("Saque realizado com sucesso!\n");
      } else {
        printf("Saldo insuficiente");
      }

      sprintf(linhaOriginal, "%d %s %.4f %s %s %s", accountNumber, senha1,
              saldo, nome, especialStr, ativoStr);
    }

    if (strcmp(linhaOriginal, buffer) != 0) {
      fprintf(tempFilePointer, "%s\n", linhaOriginal);
    } else {
      fprintf(tempFilePointer, "%s", buffer);
    }
  }

  fclose(filePointer);
  fclose(tempFilePointer);

  remove("usuarios.txt");
  rename("temp.txt", "usuarios.txt");
}

void opcoesLogin() {
  printf("O que deseja fazer?\n");
  printf("1. Depositar dinheiro\n");
  printf("2. Sacar dinheiro\n");
  printf("3. Consultar minhas informacoes \n");
  printf("4. Sair\n");
}

// Função que exibe as informações da conta especificada
void consultaInformacao(int numeroConta) {
  FILE *filePointer, *tempFilePointer;
  int bufferLength = 255;
  char buffer[bufferLength];
  char linhaOriginal[bufferLength];

  filePointer = fopen("usuarios.txt", "r");

  if (filePointer == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  tempFilePointer = fopen("temp.txt", "w");

  if (tempFilePointer == NULL) {
    printf("Erro ao abrir o arquivo temporário.\n");
    fclose(filePointer);
    return;
  }

  // Percorre o arquivo para encontrar a conta correspondente
  while (fgets(buffer, bufferLength, filePointer)) {
    strcpy(linhaOriginal, buffer);

    buffer[strcspn(buffer, "\n")] = '\0';

    char *numeroContaStr = strtok(buffer, " ");
    char *senha1 = strtok(NULL, " ");
    senha1 = NULL;
    char *saldoStr = strtok(NULL, " ");
    char *nome = strtok(NULL, " ");
    char *ativoStr = strtok(NULL, " ");
    char *especialStr = strtok(NULL, " ");

    int accountNumber = atoi(numeroContaStr);
    if(accountNumber == numeroConta) {
        printf("Numero da Conta: %s\n", numeroContaStr);
        printf("Saldo: %s\n", saldoStr);
        printf("Nome: %s\n", nome);
        printf("Ativo: %s\n", ativoStr);
        printf("Especial: %s\n", especialStr);
        printf("--------------------\n");
        break;
    }
  }

  fclose(filePointer);
  fclose(tempFilePointer);
}

// Função para registrar uma nova conta no arquivo de registros
float registerDb(int numero, float saldo, int especial, char nome[50],
                 char senha[50], int ativo) {
  FILE *registerdb;
  registerdb = fopen("usuarios.txt", "a+");
  fprintf(registerdb, "%i %s %f %s %i %i\n", numero, senha, saldo, nome,
          especial, ativo);
  fclose(registerdb);
}

// Função de login que autentica o usuário
void login() {
  int conta = 0;

  char password[MAX_PASSWORD];

  printf("Digite sua conta: ");
  scanf("%d", &conta);

  printf("Digite sua senha: ");
  scanf("%s", password);

  if (authenticateRegister(conta, password) == 0) {
    menu(conta);
  } else {
    printf("Login sem sucesso.. Movendo para tela Inicial\n");
    menuPrincipal();
  }
}




int verifyAccountNumber(int conta){

    int verifyAccount = 0;
    char *currentLine;
    FILE* filePointer;
    int bufferLength = 255;
   char buffer[bufferLength];

    filePointer = fopen("usuarios.txt", "r");

   while (fgets(buffer, bufferLength, filePointer)) {
       currentLine = buffer;

        int spaceIndex = strcspn(currentLine, " ");

        char *firstSubstring = currentLine;
       firstSubstring[spaceIndex] = '\0';
        int accountNumber = atoi(firstSubstring);

        if (accountNumber == conta) {
            verifyAccount += 1;
        }

   }


   // Close the file
   fclose(filePointer);
    fflush(stdin);


    if(verifyAccount > 0){
        verifyAccount = 0;
    } else {
        verifyAccount = 1;
    }


    return verifyAccount;
}

int authenticateRegister(int conta, char password[MAX_PASSWORD]){

    int authenticateResult = 0;
    char *currentLine;
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength];

    filePointer = fopen("usuarios.txt", "r");

    while (fgets(buffer, bufferLength, filePointer)) {
        currentLine = buffer;

        int spaceIndex = strcspn(currentLine, " ");

        char *firstSubstring = currentLine;
        firstSubstring[spaceIndex] = '\0';
        char *secondSubstring = currentLine + spaceIndex + 1;
        int spaceIndexSecond = strcspn(secondSubstring, " ");
        secondSubstring[spaceIndexSecond] = '\0';

        int accountNumber = atoi(firstSubstring);


        if (accountNumber == conta && strcmp(secondSubstring, password) == 0) {
            printf("Login bem-sucedido!\n");
            authenticateResult += 1;
            break;
        }
    }


    // Close the file
    fclose(filePointer);
    fflush(stdin);

    if(authenticateResult >= 1){
        authenticateResult = 0;

    } else {
        authenticateResult = 1;
    }

    return authenticateResult;
}

// Função que verifica se duas strings são idênticas e se a senha atende aos
// critérios
int verifyPassword(char *str1, char *str2) {

  if (strcmp(str1, str2) == 0) {
    int pass = strlen(str1);
    if (pass > 4 && pass < MAX_PASSWORD) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

// Função que conta o número de linhas em um arquivo
void countlines() {
  FILE *fp = fopen("usuarios.txt", "r");
  int ch = 0;
  int lines = 0;

  if (fp == NULL) {
    printf("erro");
  }
  lines++;
  while (!feof(fp)) {
    ch = fgetc(fp);
    if (ch == '\n') {
      lines++;
    }
  }
  fclose(fp);
  printf("%d", lines - 1);
}

// Função para inserir uma nova conta no sistema
void insert(Conta *contas, int totalContas) {

  char *p1 = malloc(MAX_PASSWORD * sizeof(char));
  char *p2 = malloc(MAX_PASSWORD * sizeof(char));
  int idx = getCurrentIndex();
  int numeroContaInsert = 0;
  currentAccount = &contas[idx];

  printf("Digite seu nome: ");
  scanf("%s", currentAccount->cliente);
  printf("Digite o numero da conta: ");
  scanf("%i", &numeroContaInsert);
    if(verifyAccountNumber(numeroContaInsert) == 0){
        printf("Conta ja existente! Movendo para tela de login..\n");
       login();
        exit(1);
 } else {
        currentAccount->numero = numeroContaInsert;
    }



  currentAccount->especial = 1;
  currentAccount->saldo = 0;
  currentAccount->ativo = 1;

  printf("Digite a senha: ");
  scanf("%s", p1);
  printf("Digite novamente a senha: ");
  scanf("%s", p2);

  if (verifyPassword(p1, p2)) {
    printf("Cadastrado realizado!\n");
    strcpy(currentAccount->password, p1);
    updateActiveAccounts();
    updateAccountIndex();
    registerDb(currentAccount->numero, currentAccount->saldo,
               currentAccount->especial, currentAccount->cliente,
               currentAccount->password, currentAccount->ativo);
  } else {
    printf("Erro na senha! Verifique seu tamanho ou similaridade.\n");
  }
}

// Função que lista todas as contas existentes
void listAccounts(Conta *contas, int totalContas) {

    int possibleAccounts = MAX - totalContas;
    int currentIndex = getCurrentIndex();
    printf("Lista: \n");
    int contador = 0;
    for (int i = 0; i < currentIndex; i++) {
        currentAccount = &contas[contador];
        if (contador < possibleAccounts) {
            printf("Numero da conta: %i\n", currentAccount->numero);
            printf("Nome do cliente: %s\n", currentAccount->cliente);
            printf("Conta especial: %i\n", currentAccount->especial);
            printf("Saldo: %f\n", currentAccount->saldo);
            printf("Ativa: %i\n", currentAccount->ativo);
            printf("\n");
            contador++;
        }
    }
}

// Função para atualizar o número de contas ativas
int updateActiveAccounts() {
  FILE *accounts;
  accounts = fopen("activeAccounts.txt", "r+");
  int number = 0;

  if (accounts != NULL) {

    fscanf(accounts, "%d", &number);

    int newNumber = number + 1;

    rewind(accounts);

    fprintf(accounts, "%d", newNumber);

    fclose(accounts);

    return newNumber;
  } else {
    printf("Erro ao abrir o arquivo.\n");
    return -1;
  }
}
// Função para atualizar o índice da conta
int updateAccountIndex() {
  FILE *idx;
  idx = fopen("accountsIndex.txt", "r+");
  int number = 0;

  if (idx != NULL) {

    fscanf(idx, "%d", &number);

    int newNumber = number + 1;

    rewind(idx);
    fprintf(idx, "%d", newNumber);
    fclose(idx);

    return newNumber;
  } else {
    printf("Erro ao abrir o arquivo.\n");
    return -1;
  }
}

// Função para obter o índice da conta atual
int getCurrentIndex() {
  FILE *idx;
  idx = fopen("accountsIndex.txt", "r");

  int number;
  int i;

  if (idx == NULL) {
    printf("Erro no Sistema. A conta não pôde ser cadastrada!\n");
    exit(0);
  }

  for (i = 0; i < 1; i++) {
    fscanf(idx, "%d,", &number);
  }
  fclose(idx);
  return number;
}
// Função para obter o número total de contas ativas
int getActiveAccounts() {
  FILE *accounts;
  accounts = fopen("activeAccounts.txt", "r");

  int number;
  int i;

  if (accounts == NULL) {
    printf("Erro no Sistema. A quantidade de contas Nao podem ser exibidas!\n");
    exit(0);
  }

  for (i = 0; i < 1; i++) {
    fscanf(accounts, "%d,", &number);
  }
  fclose(accounts);
  return number;
}
