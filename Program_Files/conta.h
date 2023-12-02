// Definindo o tamanho máximo para o número de contas e senhas
#define MAX 100
#define MAX_PASSWORD 15

// Estrutura que representa uma conta bancária
typedef struct {
    int numero;                  // Número da conta
    char cliente[51];            // Nome do cliente (máximo de 50 caracteres)
    int especial;                // Indica se a conta é especial
    float saldo;                 // Saldo da conta
    int ativo;                   // Indica se a conta está ativa
    char password[MAX_PASSWORD]; // Senha da conta (máximo de 14 caracteres)
} Conta;

// Declaração de variáveis globais
extern Conta *contas;           // Array de contas
extern Conta *currentAccount;    // Conta atualmente selecionada

// Declaração de funções

// Função para realizar um saque em uma conta
void sacarDinheiro(int numeroConta, float qtd);

// Função para remover linhas em branco de algum arquivo
void removeLinhasEmBranco();

// Função para realizar um depósito em uma conta
void depositarDinheiro(int numeroConta, float qtd);

// Função que exibe opções de menu
void opcoesLogin();

// Função para realizar o processo de login
void login();

// Função para autenticar um registro durante o login
int authenticateRegister();

// Função para atualizar o índice da conta
int updateAccountIndex();

// Função para obter o índice da conta atual
int getCurrentIndex();

// Função para listar todas as contas
void listAccounts();

// Função para inicializar o sistema (pode envolver a alocação de memória)
void inicializar();

// Função para obter o número total de contas ativas
int getActiveAccounts();

// Função para atualizar o número de contas ativas
int updateActiveAccounts();

// Função para inserir uma nova conta no sistema
void insert(Conta *contas, int activeAccounts);

// Função para contar o número de linhas em algum arquivo
void countlines();

// Função para consultar informações de uma conta
void consultaInformacao();

int authenticateRegister();

void menuPrincipal();