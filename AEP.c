#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

int validador(const char* senha);
void gerarSenha(char* texto, int tamanho);
void cifraVigenereAscii(char* texto, const char* chave, int criptografar);
void abrirClientes(const char* arquivo);
void incluirClientes(const char* nomeArquivo, const char nome[],const char chave[],const char senha[]);
void excluirClientes(const char* nomeArquivo, const char* nome);
void alterarClientes(const char* nomeArquivo);
void clear(void);

int i;

#define MAX_LINHAS 1000
#define MAX_TAMANHO 256
//
//  
//
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

int main() {
    char texto[100];
    char chave[100];
    char user[100];
    int tamanhoSenha, escolha;
    FILE *arq;
	setlocale(LC_ALL,"Portuguese");

    srand(time(NULL));

	arq = fopen("Cliente.txt", "r");
	if (arq == NULL)
    {
        arq = fopen("Cliente.txt", "w");
        if (arq == NULL)
        {
        	perror("Não foi possivel criar o arquivo");
        	exit(EXIT_FAILURE);
		}
    }

    do
    {
    printf("Digite o seu usuário (caracteres ASCII visíveis/Não é possível adicionar espaços no nome): ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = '\0';
    } while (validador(user) == 0);

    do
    {
    printf("Digite a sua senha (caracteres ASCII visíveis/Não é possível adicionar espaços na senha): ");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = '\0';  
    } while (validador(texto) == 0);

    tamanhoSenha = strlen(texto);
    char senha[tamanhoSenha + 1];

    gerarSenha(senha, tamanhoSenha);
    //printf("Chave gerada: %s \n", senha);

    cifraVigenereAscii(texto, senha, 1);
    //printf("Texto Criptografado: %s\n", texto);

    cifraVigenereAscii(texto, senha, 0);
    //printf("Texto Descriptografado: %s\n", texto);

do
{ 
    printf("1 - Lista dos Clientes \n");
    printf("2 - Incluir um novo Cliente \n");
    printf("3 - Exclusão de um Cliente \n");
    printf("4 - Alteração de um Cliente \n");

    printf("\n");

    scanf("%d", &escolha);
    clear();

    switch(escolha)
    {
        case 1:
            printf("\n");
            printf("Lista dos clientes: \n");
            abrirClientes("Cliente.txt");
            break;
        case 2:
            printf("\n");
            incluirClientes("Cliente.txt", user, senha, texto);
            break;
        case 3:
            printf("\n");
            printf("Escreva o nome do usuário: ");
            fgets(user, sizeof(user), stdin);
            user [strcspn(user, "\n")] = '\0';
            excluirClientes("Cliente.txt", user);
            break;
        case 4:
            alterarClientes("Cliente.txt");
            break;
        default:
            escolha = 100;
            break;

    }
} while (escolha == 100);
    return 0;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
// Função para criptografar ou descriptografar o texto usando a cifra de Vigenère (baseada na tabela ASCII)
void cifraVigenereAscii(char* texto, const char* chave, int criptografar) {
    int texto_len = strlen(texto);
    int chave_len = strlen(chave);

    for (i = 0; i < texto_len; i++) 
	{
        int j = 0;
        char c = texto[i];
        if (c >= 32 && c <= 126) 
		{ //32 = Primeiro número de caractere visível na tabela ASCII 
            int chave_char = chave[j % chave_len]; //Número de caracteres visíveis na tabela ASCII
            if (criptografar) 
			{
                texto[i] = 32 + ((c - 32 + chave_char - 32) % 95);
            } else texto[i] = 32 + ((c - 32 - (chave_char - 32) + 95) % 95);
        }
        j++;
    }
}
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
// Função para gerar uma chave aleatória na criptografia
void gerarSenha(char* texto, int tamanho) 
{
    const char alfanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int numCar = strlen(alfanum);

    for (i = 0; i < tamanho; i++) 
	{
        int indice = rand() % numCar;
        texto[i] = alfanum[indice];
    }

    texto[tamanho] = '\0';
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
// Função que verifica se a senha é válida (sem espaços) ou inválida (com espaços);
int validador(const char* texto)
{
    for (i = 0;texto[i] != '\0';i++)
    {
        if (isspace(texto[i])) return 0;
    }
    return 1;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
// Função responsável por abrir a lista com os nomes dos clientes
void abrirClientes(const char* nomeArquivo)
{   
    FILE *arq = fopen(nomeArquivo, "r");

    if (arq == NULL)
    {
        perror("Não foi possível abrir o arquivo!"); // a função perror coloca a mensagem escrita no terminal, é usado quando pode acontecer algum possível erro
        exit (EXIT_FAILURE); // termina o processo
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), arq)) // Enquanto o arquivo não acabar
    {
        linha[strcspn(linha, "\n")] = '\0'; // o caracter de quebra de linha vira nulo

        char *espaco = strchr(linha, ','); // nos diz onde está a primeira vírgula
        if (espaco != NULL) *espaco = '\0'; // Substitui a vírgula por nulo, terminando a string

        printf("%s \n", linha); // Escreve o nome
    }
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
// Função responsável por incluir clientes na lista dos nomes, chaves e senhas
void incluirClientes(const char* nomeArquivo, const char nome[], const char chave[], const char senha[])
{
    FILE *arq = fopen(nomeArquivo, "r"); // Abrir no modo leitura para verificar a existência
    if (arq == NULL)
    {
        perror("Não foi possível abrir o arquivo para leitura!");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    int usuarioExistente = 0;

    while (fgets(linha, sizeof(linha), arq)) 
    {
        linha[strcspn(linha, "\n")] = '\0'; 

        //char *nomeArquivoAtual = linha; 
        char *virgula = strchr(linha, ',');
        if (virgula != NULL)
        {   
            *virgula = '\0';

            char nomeAtualSemAspas[256];
            char *nomeArquivoAtual = linha + 1; //aponta no segundo caractere da string, já que o primeiro é uma aspas ex: "Joao","Senha","Chave"
            strncpy(nomeAtualSemAspas, nomeArquivoAtual, strlen(nomeArquivoAtual) - 2);
            nomeAtualSemAspas[strlen(nomeArquivoAtual) - 1] = '\0';
            nomeArquivoAtual[strlen(nomeArquivoAtual) - 1] = '\0'; // Remove a segunda aspas da variável para compará-la mais tarde

            if (strcmp(nomeArquivoAtual, nome) == 0) // Se algum nome do arquivo for igual ao input
            {
                usuarioExistente = 1;
                break;
            }
        }
    }


    fclose(arq);

    if (usuarioExistente)
    {
        printf("Usuário já existe.\n");
        return;
    }

    // Reabre o arquivo no modo "a"(append/anexação) para adicionar um novo cliente
    arq = fopen(nomeArquivo, "a");
    if (arq == NULL)
    {
        perror("Não foi possível abrir o arquivo para escrita!");
        exit(EXIT_FAILURE);
    }

    fprintf(arq, "\"%s\",\"%s\",\"%s\"\n", nome, senha, chave); // saída: "nome","senha","chave"
    printf("Você foi adicionado ao sistema com sucesso!");

    fclose(arq);
}


//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

void excluirClientes(const char* nomeArquivo, const char* nome) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) 
	{
        perror("Não foi possível abrir o arquivo original para leitura!");
        exit(EXIT_FAILURE);
    }
    char nomes[MAX_LINHAS][MAX_TAMANHO];
    char chaves[MAX_LINHAS][MAX_TAMANHO];
    char senhas[MAX_LINHAS][MAX_TAMANHO];
    int numLinhas = 0;

    char linha[MAX_TAMANHO];
    while (fgets(linha, sizeof(linha), arq)) 
	{
        linha[strcspn(linha, "\n")] = '\0'; 

        char *nomeArquivoAtual = linha;
        char *virgula = strchr(linha, ',');
        if (virgula != NULL) 
		{
            *virgula = '\0'; 
            char nomeAtualSemAspas[MAX_TAMANHO];

            // Remove as aspas do nome
            int comprimento = strlen(nomeArquivoAtual) - 2;
            if (comprimento > 0) 
			{
                strncpy(nomeAtualSemAspas, nomeArquivoAtual + 1, comprimento);
                nomeAtualSemAspas[comprimento] = '\0';
            } else nomeAtualSemAspas[0] = '\0';

            strcpy(nomes[numLinhas], nomeAtualSemAspas);
            strcpy(chaves[numLinhas], virgula + 1); 

            char *senhaInicio = strchr(chaves[numLinhas], ',') + 1;
            if (senhaInicio) 
			{
                senhaInicio[strcspn(senhaInicio, "\"")] = '\0'; // Remove as aspas da senha
                strcpy(senhas[numLinhas], senhaInicio);
            }

            numLinhas++;
        }
    }
    fclose(arq);

    // Reescreve o arquivo, excluindo a linha que tinha o nome digitado
    arq = fopen(nomeArquivo, "w");
    if (arq == NULL) 
	{
        perror("Não foi possível abrir o arquivo para escrita!");
        exit(EXIT_FAILURE);
    }

    int excluida = 0;
    for (i = 0; i < numLinhas; i++) 
	{
        if (strcmp(nomes[i], nome) != 0) 
		{
            fprintf(arq, "\"%s\",\"%s\",\"%s\"\n", nomes[i], chaves[i], senhas[i]);
        } else excluida = 1;
    }

    fclose(arq);

    if (excluida) printf("Cliente excluído com sucesso!\n");
    else printf("Cliente não encontrado.\n");
}



//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

void alterarClientes(const char* nomeArquivo) 
{
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) 
	{
        perror("Não foi possível abrir o arquivo para leitura!");
        exit(EXIT_FAILURE);
    }

    char nomes[MAX_LINHAS][MAX_TAMANHO];
    char senhas[MAX_LINHAS][MAX_TAMANHO];
    char chaves[MAX_LINHAS][MAX_TAMANHO];
    int numLinhas = 0;

    char linha[MAX_TAMANHO];

    while (fgets(linha, sizeof(linha), arq)) 
	{
        linha[strcspn(linha, "\n")] = '\0'; // Remove nova linha

        // Verifica se a linha tá organizada no formato esperado ("nome","senha","chave")
        if (sscanf(linha, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"",    nomes[numLinhas], senhas[numLinhas], chaves[numLinhas]) == 3) {
            //printf("Linha lida: %s\n", linha); // Para debug
            //printf("Nome: %s, Senha: %s, Chave: %s\n", nomes[numLinhas], senhas[numLinhas], chaves[numLinhas]);
            numLinhas++;
        } else {
            //printf("Formato inválido: %s\n", linha); // Para debug
        }
    }
    fclose(arq);

    char nomeAtual[MAX_TAMANHO];
    char senhaAtual[MAX_TAMANHO];
    char novoNome[MAX_TAMANHO];

    printf("Digite o nome do usuário para alteração: ");
    fgets(nomeAtual, sizeof(nomeAtual), stdin);
    nomeAtual[strcspn(nomeAtual, "\n")] = '\0';

    printf("Digite a senha do usuário para alteração: ");
    fgets(senhaAtual, sizeof(senhaAtual), stdin);
    senhaAtual[strcspn(senhaAtual, "\n")] = '\0';

    int encontrado = 0;
    int nomeDisponivel = 1;

    for (i = 0; i < numLinhas; i++) 
	{
        if (strcmp(nomes[i], nomeAtual) == 0 && strcmp(senhas[i], senhaAtual) == 0) 
		{
            encontrado = 1;
            printf("Digite o novo nome de usuário: ");
            fgets(novoNome, sizeof(novoNome), stdin);
            novoNome[strcspn(novoNome, "\n")] = '\0';
		int j;
            for (j = 0; j < numLinhas; j++) 
			{
                if (strcmp(nomes[j], novoNome) == 0) 
				{
                    nomeDisponivel = 0;
                    printf("Novo nome de usuário já está em uso.\n");
                    break;
                }
            }

            if (nomeDisponivel) 
			{
                strcpy(nomes[i], novoNome);
            }
            break;
        }
    }

    if (!encontrado) 
	{
        printf("Usuário não encontrado ou senha incorreta.\n");
        return;
    }

    // essa parte da função vai reescrever o nome desejado
    arq = fopen(nomeArquivo, "w");
    if (arq == NULL) 
	{
        perror("Não foi possível abrir o arquivo para escrita!");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < numLinhas; i++) 
	{
        fprintf(arq, "\"%s\",\"%s\",\"%s\"\n", nomes[i], senhas[i], chaves[i]);
    }

    fclose(arq);

    if (nomeDisponivel) 
	{
        printf("Nome de usuário alterado com sucesso!\n");
    }
}



//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

void clear(void)
{
    while (getchar() != '\n');
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
