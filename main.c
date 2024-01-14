#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct REGISTRO
{
    char codCliente[12];
    char codVeiculo[8];
    char nomeCliente[50];
    char nomeVeiculos[50];
    char dias[4];
} REGISTRO;

typedef struct chaves
{
    char chavePrimaria[19];
    int posicao;
} CHAVE;

typedef struct busca{
    char codCliente[12];
    char codVeiculo[8];

}BUSCA;




FILE *abreArquivo(char *diretorio);
void Insere(FILE *indice, FILE *repositorio, REGISTRO *vet);
void Busca();

int compararChaves(const void *a, const void *b)
{
    const CHAVE *chaveA = (const CHAVE *)a;
    const CHAVE *chaveB = (const CHAVE *)b;
    return strcmp(chaveA->chavePrimaria, chaveB->chavePrimaria);
}

void ordenarPorChavePrimaria(CHAVE vetor[], int tamanho, FILE * arquivo)
{
    // Usar qsort para ordenar o vetor com base nas chaves primárias
    qsort(vetor, tamanho, sizeof(CHAVE), compararChaves);
    
    rewind(arquivo); //volta ao inicio
    for(int i = 0; i < tamanho; i++){ 
        fwrite(&vetor[i], sizeof(CHAVE), 1, arquivo); // reescreve no arquivo, agora de maneira ordenada;
    }
        
}

int main()
{   

    const int tamInsere = 8, tamchave = 7 ;// tamanhos dos vetores
    FILE *repositorio, *indice, *pontInsere, *pontChave; // declara arquivos;
    char diretorio[50];
    REGISTRO vetInsere[tamInsere];
    BUSCA vetChave[tamchave];
    int i, opcao;


    printf("\tO programa foi inicializado...\n\tProcurando arquivo...\n");


    pontInsere = abreArquivo("insere.bin"); // abre insere.bin
    fread(vetInsere, sizeof(REGISTRO), tamInsere, pontInsere); // transfere insere.bin p/ vetInsere
    fclose(pontInsere); // encerra o arquivo

    pontChave = abreArquivo("busca_p.bin"); // abre busca_p.bin
    fread(vetChave, sizeof(BUSCA), tamchave, pontChave); // transfere busca_p.bin p/ vetChave
    for(int k=0; k<tamchave; k++){
        printf("%s|%s\n",vetChave[k].codCliente,vetChave[k].codVeiculo);
    }
    fclose(pontChave); // encerra o arquivo

    /*printf("\nteste");
    printf("\n%s %s %s %s \n", vetChave[0].codVeiculo, vetChave[1].codCliente, vetChave[2].codCliente, vetChave[3].codCliente);*/

    do


    {   
        repositorio = abreArquivo("teste.bin"); // abre teste.bin e coloca no final
        fseek(repositorio , 0, SEEK_END);
        indice = abreArquivo("indice.bin"); // abre indice.bin e coloca no final
        fseek(indice , 0, SEEK_END);

        printf("\n-------------------------------------------------------------------------\n\n");
        printf("\tDigite:\n\t1 - INSERCAO\n\t2 - BUSCA\n\t0 - ENCERRAR O PROGRAMA\n");
        printf("\tOpcao:");
        scanf("%d", &opcao);
        printf("\n-------------------------------------------------------------------------\n\n");

        switch (opcao)
        {
        case 1:
            Insere(indice, repositorio, vetInsere);
            break;
        case 2:
            Busca(vetChave, tamchave, repositorio, indice);
            break;

        case 0:
            break;
        }

        fclose(repositorio);
        fclose(indice);
    } while (opcao != 0);

    printf("\tEncerrando programa...\n");
    return 0;
}
/* FUNÇÕES */

/* Procura com a função "acces()" se o arquivo existe
        se sim ele é aberto com "r+b",
        senão é criado um arquivo com "w+b".
*/
FILE *abreArquivo(char *diretorio)
{
    FILE *pont;
    int header = -1;

    if (access(diretorio, F_OK) == 0)
    {
        printf("\tO arquivo encontrado com sucesso!\n\n");
        pont = fopen(diretorio, "r+b");
    }
    else
    {
        printf("\tO arquivo nao foi encontrado, por isso foi criado um semelhante!\n\n");
        pont = fopen(diretorio, "w+b");
        if (diretorio == "teste.bin")
        {
            fwrite(&header, sizeof(int), 1, pont);
        }
    }
    
    return pont;
}

/*
    A função "Insere" recebe do usuario o dado a ser inserido no arquivo principal e com o auxilio da função "Firstfit",
    insere no primeiro espaço disponível que for encontrado.
    Além disso ela também faz a formatação dos dados para serem alocados no registro.
*/
 
void Insere(FILE *indice, FILE *repositorio, REGISTRO *vet)
{   
    int aux, tamAqChaves; // auxiliar pra achar no vetor de registros;
    REGISTRO reg;
    char concat[124], concatIndice[25];

    printf("Digite o número a ser inserido (1-8): ");
    scanf("%d", &aux);
    reg = vet[aux - 1]; // definindo quem reg vale;
    CHAVE chave;
    if (aux >= 1 && aux <= 8)
    {
        //escreve no teste.bin
        sprintf(concat, "%s|%s|%s|%s|%s", reg.codCliente, reg.codVeiculo, reg.nomeCliente, reg.nomeVeiculos, reg.dias); // concatena todas as infos em concat;
        int size = strlen(concat); // conta quantos bytes;
        fwrite(&size, sizeof(int), 1, repositorio); // escreve o tamanho em bytes
        fwrite(concat, sizeof(char), size, repositorio); // escreve o concat
        
       // fwrite(&size, sizeof(int), 1, indice); // escreve o tamanho em bites
        fseek(repositorio, -size,SEEK_CUR); 
        int posiIndiceRegistro = ftell(repositorio);

        sprintf(concatIndice, "%s|%s", reg.codCliente, reg.codVeiculo); // concatena todas as infos em concatIndice;
        size = strlen(concatIndice);
        char sla = ' '; 
        fwrite(concatIndice, sizeof(char), size, indice); // escreve o concatIndice
        fwrite(&sla, sizeof(char),1,indice); 
        fwrite(&posiIndiceRegistro,sizeof(int),1,indice); // escreve a quantidade de bytes que precisa pular para achar o equivalente no arq principal
        // //rewind(indice); 
        // //fwrite(&tamAqChaves,sizeof(int),1, indice); // atualiza header
        
        printf("Registro inserido com sucesso!\n");
        //fseek(indice, 1, SEEK_CUR); 
        int contadorNumeroRegistros =0; 
        rewind(indice); 
        char aux[20]; 
        int aux2; 
        char leitura; 
        while(fgetc(indice)!= EOF)
        {
        fseek(indice,-1,1);
        fread(&aux,sizeof(char),size,indice);
        fread(&leitura,sizeof(char),1,indice);
        fread(&aux2,sizeof(int),1,indice);
        contadorNumeroRegistros++; 
        }
        CHAVE vetChave[contadorNumeroRegistros]; 
        rewind(indice); 
        // int i =0; 

        for(int i =0; i< contadorNumeroRegistros; i++ )
        {
            //fread(&vetChave[i].chavePrimaria, sizeof(char), sizeof(vetChave[i].chavePrimaria), indice);
            fread(&vetChave[i].chavePrimaria, sizeof(char) , size, indice); 
            printf("Chave Primária: %s\n", vetChave[i].chavePrimaria);  
            fread(&leitura, sizeof(char) , 1, indice); 
            fread(&vetChave[i].posicao, sizeof(int) , 1, indice); 
            printf("Chave Posicao: %d\n", vetChave[i].posicao);  

        }
       
        ordenarPorChavePrimaria(vetChave, contadorNumeroRegistros, indice);
        }
    else
    {
        printf("Número fora do intervalo válido (1-8).\n");
    }
    return;
}


void Busca(BUSCA *vet, int tam, FILE *repositorio, FILE *indice)
{
    int aux;
    BUSCA rem;
    CHAVE repo;
    REGISTRO *reg;
    char chaveBuscada[20] = "";
    //char concat[20] = "";
    printf("Digite o índice a ser buscado: ");
    scanf("%d", &aux);

    if (aux >= 1 && aux <= tam)
    {
        rem = vet[aux - 1]; // Definindo rem como o índice escolhido
        sprintf(chaveBuscada, "%s|%s", rem.codCliente, rem.codVeiculo);
        bool encontrou = false;
        rewind(indice); // Volta ao início do arquivo de índice
        fread(&repo, sizeof(CHAVE), 1, indice);
        int size = sizeof(repo);
        rewind(indice);
        while (fread(&repo, size, 1, indice) == 1)
        {     
            char chavePrimariaIndice[20]; // Extrair a chave primária do índice, ignorando o último byte (posição)
            memcpy(chavePrimariaIndice, repo.chavePrimaria, 19); // Copia os primeiros 19 bytes
            chavePrimariaIndice[19] = '\0'; // Adicionar o terminador nulo para garantir que a string seja corretamente terminada

            if (strcmp(chavePrimariaIndice, chaveBuscada) == 0)
            {
                encontrou = true;
                break; // Encontrou a chave, sai do loop
            }
        }
        //repo.posicao += 4;
        printf("\nnova pos = %d\n", repo.posicao);

        if (encontrou)
        {   
            int tamanho = 0;


            rewind(repositorio); 
            fseek(repositorio, (repo.posicao -4), SEEK_CUR); 
            fread(&tamanho,sizeof(int),1,repositorio);
            char aux3[tamanho];
             fread(aux3, 1, tamanho, repositorio); // Lê o registro correspondente
            printf("\n%s", aux3);

            char * codCli = strtok(aux3, "|");   
            char * codVei = strtok(NULL, "|");
            char * nomeCli = strtok(NULL, "|");
            char * nomeVei = strtok(NULL, "|");
            char * dias = strtok(NULL, "|");
            printf("\nRegistro encontrado:\n");
            printf("informações: %s\n", codCli);
            printf("CodVeiculo: %s\n", codVei);
            printf("NomeCliente: %s\n", nomeCli);
            printf("NomeVeiculos: %s\n", nomeVei);
            printf("Dias: %s\n", dias);


    
        }
        else
        {
            printf("Chave não encontrada.\n");
        }
    }
    else
    {
        printf("Número de índice fora do intervalo válido (1-%d).\n", tam);
    }
}
