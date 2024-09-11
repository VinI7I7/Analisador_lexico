#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define DELIMITADOR 1
#define VARIAVEL 2
#define NUMERO 3
#define PALAVRA_CHAVE 4
#define ERRO 5

// Definindo a variável global prog
char *prog;
char token[80];
char token_type;

// Protótipo das funções
int isdelim(char c);
void skip_whitespace_and_comments(void);
void get_token(void);
void print_token(void);
void read_file(const char *filename);

// Função para verificar se um caractere é um delimitador
int isdelim(char c) {
    return strchr("+-*/%=() ,;:.\t\r\n<>", c) != NULL;
}

// Função para ignorar espaços em branco e comentários
void skip_whitespace_and_comments(void) {
    while (*prog) {
        if (isspace(*prog)) {
            // Ignora espaços, tabulações e quebras de linha
            ++prog;
        } else {
            // Se não for espaço, pare
            break;
        }
    }
}

// Função para obter o próximo token
void get_token(void) {
    register char *temp;

    token_type = 0;
    temp = token;
    *temp = '\0';

    if (!*prog) return;
    
    skip_whitespace_and_comments(); // Ignora espaços e comentários antes de processar o token

    if (strchr("+-*/%=() ,;:.<>{}[]", *prog)) {
        token_type = DELIMITADOR;
        *temp++ = *prog++;
    } else if (isalpha(*prog)) {
        while (!isdelim(*prog) && *prog != '\0') {
            *temp++ = *prog++;
        }
        *temp = '\0';
        token_type = (strcmp(token, "var") == 0 || strcmp(token, "begin") == 0 || strcmp(token, "end") == 0 || strcmp(token, "if") == 0 || strcmp(token, "then") == 0 || strcmp(token, "else") == 0 || strcmp(token, "while") == 0 || strcmp(token, "do") == 0) ? PALAVRA_CHAVE : VARIAVEL;
    } else if (isdigit(*prog) || (*prog == '.' && isdigit(*(prog + 1)))) {
        while (!isdelim(*prog) && *prog != '\0') {
            *temp++ = *prog++;
        }
        token_type = NUMERO;
    }
    else{
    	token_type = ERRO;
    	*temp++ = *prog++;
	}
    *temp = '\0';
}

// Função para exibir o token e seu tipo
void print_token(void) {
    switch (token_type) {
        case DELIMITADOR:
            printf("DELIMITADOR: %s\n", token);
            break;
        case VARIAVEL:
            printf("VARIAVEL: %s\n", token);
            break;
        case NUMERO:
            printf("NUMERO: %s\n", token);
            break;
        case PALAVRA_CHAVE:
            printf("PALAVRA CHAVE: %s\n", token);
            break;
        default:
            printf("ERRO: %s\n", token);
            break;
    }
}

// Função para ler o conteúdo de um arquivo
void read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    // Move o ponteiro do arquivo para o final para obter o tamanho do arquivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Aloca buffer para ler o arquivo
    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    // Lê o conteúdo do arquivo para o buffer
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0'; // Null-termina a string

    // Fecha o arquivo
    fclose(file);

    // Define o buffer como o ponteiro de programa
    prog = buffer;
}

int main(void) {
    // Nome do arquivo a ser lido
    const char *filename = "lexico.txt";

    // Lê o conteúdo do arquivo
    read_file(filename);

    // Analisando a expressão
    while (*prog) {
        get_token();
        if (token_type != 0) {
            print_token();
        }
    }

    // Libera a memória alocada
    free(prog);

    return EXIT_SUCCESS;
}
