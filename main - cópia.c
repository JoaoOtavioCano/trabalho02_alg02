#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct registro
{
    int id;
    char *titulo;
    char *autor;
    int byte_offset;
}registro;

void lerRegistros(registro* registro);
void escreverArqDados(registro *registro, FILE *arquivo);
registro lerArqDados(FILE *arquivo, int byte_offset);
void exibirRegistros(registro registro);

int main(void)
{
  // abrindo arquivos de dados e de indices para escrita e leitura em binário
  FILE *arq_dados, *ind_primario, *ind_secundario;
  arq_dados = fopen("livros.bin", "wb+");
  ind_primario = fopen("id.bin", "wb+");
  ind_secundario = fopen("autor.bin", "wb+");

  // armazena operacao a ser executada
  char operacao[6];

  // buffer para salvar temporariamente  um registro
  registro registro_buffer;
  registro_buffer.titulo = (char *)malloc(100 * sizeof(char));
  registro_buffer.autor = (char *)malloc(100 * sizeof(char));

  while( 1 > 0 ){  
    scanf("%s", operacao);
    if (strcmp(operacao, "ADD") == 0)
    {
      lerRegistros(&registro_buffer);
      escreverArqDados(&registro_buffer, arq_dados);
    }else if(strcmp(operacao, "SEARCH") == 0) 
    {
      
    }else if (strcmp(operacao, "REMOVE") == 0) 
    {
      
    }else if (strcmp(operacao, "EXIT") == 0) 
    {
      break;
    }
  }  


  fclose(arq_dados);
  fclose(ind_primario);
  fclose(ind_secundario);
  return 0;
}

// FUNCIONA
void lerRegistros(registro *registro)
{
  char entrada[300];
  scanf(" %[^(\r|\n)]", entrada);
  sscanf( entrada, "id='%i' titulo='%[^(\r|\n)']' autor='%[^(\r|\n)']'", &registro->id, registro->titulo, registro->autor);
}

//FUNCIONA
void escreverArqDados(registro *registro, FILE *arquivo)
{
    int tam_string_autor = strlen(registro->autor);
    char delimitador_titulo = 124;
    char fim_registro = -1;

    fwrite(&registro->id, 1, sizeof(int), arquivo);
    fwrite(registro->titulo, 1, strlen(registro->titulo), arquivo);
    fwrite(&delimitador_titulo, 1, 1, arquivo);
    fwrite(&tam_string_autor, 1, sizeof(int), arquivo);
    fwrite(registro->autor, 1, tam_string_autor, arquivo);
    fwrite(&fim_registro, 1, 1, arquivo);
}

// FUNCIONA
// busca no arquivo de dados o registro 
registro lerArqDados(FILE *arquivo, int byte_offset)
{
    char seek_buffer[1] = {0};
    int contador = 0;
    char delimitador_titulo = 124;
    int tam_string_autor;


    registro registro;
    registro.titulo = (char *)malloc(100 * sizeof(char));
    registro.autor = (char *)malloc(100 * sizeof(char));

    fseek(arquivo, byte_offset, SEEK_SET);
    fread(&registro.id, 1, 4, arquivo);
    contador = 0;
    while(seek_buffer[0] != delimitador_titulo)
    {
        fread(seek_buffer, 1, 1, arquivo);
        contador++;
    }
    fseek(arquivo, byte_offset+4, SEEK_SET);
    fread(registro.titulo, 1, contador - 1, arquivo);
    fseek(arquivo, byte_offset+4+contador, SEEK_SET);
    fread(&tam_string_autor, 1, sizeof(int), arquivo);
    fread(registro.autor, 1, tam_string_autor, arquivo);

    return registro;
}

// imprime o registro
void exibirRegistros(registro registro)
{
    printf("%i - ", registro.id);
    printf("%s - ", registro.titulo);
    printf("%s\n", registro.autor);
}