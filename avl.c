#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node{
    char nomeProd[30];
    int height, indice;
    struct node *esq;
    struct node *dir;
}no;

typedef struct reg{
    char nome[30];
    float preco;
    int qtdEstoque;
}produto;

no *root;
FILE *arquivo;
int myIndex;
produto prod;

//Protótipo das funções
void imprimeMenu(void);
int height(no *N);
int max(int a, int b);
int getBalance(no *N);
no *leftRotate(no *pivo);
no *rightRotate(no *pivo);
no *newNode(char nome[], int index);
no* insert(no* raiz, char nome[], int index);
int incluiProduto();
no *minValueNode(no *raiz);
no *deleteNode(no* raiz, char nome[]);
void exclue(no *aux);
no* searchAndDelete(no* raiz, char nome[]);
void exclueProduto();
void altera(no *aux);
no* searchAndEdit(no *aux, char nome[]);
void alteraProduto(void);
no* searchKey(no *aux, char nome[]);
void buscaProduto(void);
void searchAll(no *raiz);
void buscaTotal(void);
int exibeArvore(no* raiz);
FILE* abrirArquivo(char caminho[30], char modo);
void fecharArquivo(FILE *arquivo);
void gravarEmArquivo(void);
void carregaArquivo(void);
int search(no* aux, char nome[]);


int main(){
    root = NULL;
    carregaArquivo();
    int opcao;
    do{
        imprimeMenu();
        printf("Insira uma opcao: ");
        scanf("%d", &opcao);
        getchar();
        system("cls");
        printf("%d", myIndex);

        switch (opcao){
            case 1:
                incluiProduto();
                break;
            case 2:
                exclueProduto();
                break;
            case 3:
                alteraProduto();
                break;
            case 4:
                buscaProduto();
                break;
            case 5:
                buscaTotal();
                break;
            case 6:
                exibeArvore(root);
                system("pause");
                system("cls");
                break;
            case 0:
                exit(0);
            default:
                printf("Opcao invalida!\n");
        }

    }while (opcao != 0);

    return 0;
}  //FIM FUNCAO MAIN

void imprimeMenu(){
    printf("\t SUPERMERCADO: CONTROLE E REPOSICAO DE ESTOQUE\n");
    printf("1. Incluir produto ao estoque\n");
    printf("2. Excluir produto do estoque\n");
    printf("3. Alterar valor e/ou quantidade em estoque de um produto\n");
    printf("4. Buscar produto especifico no estoque\n");
    printf("5. Busca total\n");
    printf("6. Exibir arvore\n");
    printf("0. Finalizar programa\n");
}

//funcao que devolve a altura de um no
int height(no *N){
    if(N == NULL)
        return 0;
    return N->height;
}

//funcao que retorna o maior dentre dois numeros
int max(int a, int b){
    return (a > b) ? a : b;
}

//funcao que retorna o balanceamento de um no
int getBalance(no *N){
    if(N == NULL)
        return 0;
    return (height(N->esq) - height(N->dir));
}

no *leftRotate(no *pivo){
    no *x = pivo->dir;
    no *y = x->esq;

    x->esq = pivo;
    pivo->dir = y;

    pivo->height = max(height(pivo->esq), height(pivo->dir))+1;
    x->height = max(height(x->esq), height(x->dir))+1;

    return x;
}

no *rightRotate(no *pivo){
    no *x = pivo->esq;
    no *y = x->dir;

    x->dir = pivo;
    pivo->esq = y;

    pivo->height = max(height(pivo->esq), height(pivo->dir))+1;
    x->height = max(height(x->esq), height(x->dir))+1;

    return x;
}

no *newNode(char nome[], int myIndex){
    no* node = (no*) malloc(sizeof(no));
    strcpy(node->nomeProd, nome);
    node->indice = myIndex;
    node->esq = NULL;
    node->dir = NULL;
    node->height = 1;
    return(node);
}

no* insert(no* raiz, char nome[], int myIndex){
    if(raiz == NULL)
        return (newNode(nome, myIndex));

    if(strcmp(nome, raiz->nomeProd) < 0)
        raiz->esq = insert(raiz->esq, nome, myIndex);
    else if(strcmp(nome, raiz->nomeProd) > 0)
        raiz->dir = insert(raiz->dir, nome, myIndex);
    else
        return raiz;

    raiz->height = 1 + max(height(raiz->esq), height(raiz->dir));

    int balance = getBalance(raiz);

    //Left left case
    if(balance > 1 && strcmp(nome, raiz->esq->nomeProd) < 0)
        return rightRotate(raiz);

    // Right Right Case
    if (balance < -1 && strcmp(nome, raiz->dir->nomeProd) > 0)
        return leftRotate(raiz);

    // Left Right Case
    if (balance > 1 && strcmp(nome, raiz->esq->nomeProd) > 0){
        raiz->esq =  leftRotate(raiz->esq);
        return rightRotate(raiz);
    }

    // Right Left Case
    if (balance < -1 && strcmp(nome, raiz->dir->nomeProd) < 0){
        raiz->dir = rightRotate(raiz->dir);
        return leftRotate(raiz);
    }
    return raiz;
}

int incluiProduto(){
    printf("\t SUPERMERCADO: CONTROLE E REPOSICAO DE ESTOQUE\n");
    printf("\t\t\tINCLUSAO DE PRODUTO:\n");
    printf("Nome do produto: ");
    fgets(prod.nome, 30, stdin);
    setbuf(stdin, NULL);

    no* raiz = root;
    if(search(raiz, prod.nome) == 0){
        printf("Produto ja registrado.\n");
        system("pause");
        system("cls");
        return 0;
    }
    printf("Preco do produto: ");
    scanf("%f", &prod.preco);
    getchar();

    printf("Quantidade em estoque: ");
    scanf("%d", &prod.qtdEstoque);
    getchar();

    gravarEmArquivo();
    root = insert(raiz, prod.nome, myIndex);
    myIndex++;
    printf("Produto inserido com sucesso.\n");
    system("pause");
    system("cls");
}

no *minValueNode(no *raiz){
    no* current = raiz;
    while(current->esq != NULL)
        current = current->esq;
    return current;
}

no *deleteNode(no* raiz, char nome[]){
    if(raiz == NULL)
        return raiz;
    if(strcmp(nome, raiz->nomeProd) < 0)
        raiz->esq = deleteNode(raiz->esq, nome);
    else if(strcmp(nome, raiz->nomeProd) > 0)
        raiz->dir = deleteNode(raiz->dir, nome);
    else{
        if( (raiz->esq == NULL) || (raiz->dir == NULL) ){   //se so tiver 1 ou 0 filho
            no* aux = raiz->esq ? raiz->esq : raiz->dir;

            if(aux == NULL){
                aux = raiz;
                raiz = NULL;
            }
            else
                *raiz = *aux;
            free(aux);
        }
        else{                                            //se tiver 2 filhos
            no *aux = minValueNode(raiz->dir);
            strcpy(raiz->nomeProd, aux->nomeProd);
            raiz->indice = aux->indice;
            raiz->dir = deleteNode(raiz->dir, aux->nomeProd);
        }
    }
    if(raiz == NULL)      // se tinha so um no
        return raiz;

    raiz->height = 1 + max(height(raiz->esq), height(raiz->dir));

    int balance = getBalance(raiz);

    //Left left case
    if(balance > 1 && getBalance(raiz->esq) >= 0)
        return rightRotate(raiz);

    // Right Right Case
    if (balance < -1 && getBalance(raiz->dir) <= 0)
        return leftRotate(raiz);

    // Left Right Case
    if (balance > 1 && getBalance(raiz->esq) < 0){
        raiz->esq =  leftRotate(raiz->esq);
        return rightRotate(raiz);
    }

    // Right Left Case
    if (balance < -1 && getBalance(raiz->dir) > 0){
        raiz->dir = rightRotate(raiz->dir);
        return leftRotate(raiz);
    }
    return raiz;
}

void exclue(no *raiz){
    arquivo = abrirArquivo("estoque.dat", 'l');
    prod.nome[0] = '@';
    prod.nome[1] = '\0';
    fseek(arquivo, raiz->indice*sizeof(produto), 0);
    fwrite(&prod, sizeof(produto), 1, arquivo);
    fecharArquivo(arquivo);
}

no* searchAndDelete(no* raiz, char nome[]){
    if(raiz == NULL)
        return 0;
    if(strcmp(raiz->nomeProd, nome) == 0){
        exclue(raiz);
        return 0;
    }
    else if(strcmp(nome, raiz->nomeProd) < 0)
        searchAndDelete(raiz->esq, nome);
    else if(strcmp(nome, raiz->nomeProd) > 0)
        searchAndDelete(raiz->dir, nome);
}

void exclueProduto(){
    printf("\t SUPERMERCADO: CONTROLE E REPOSICAO DE ESTOQUE\n");
    printf("\t\t\tEXCLUSAO DE PRODUTO:\n\n");
    char nome[30] = " ";
    printf("Nome do produto que deseja deletar: ");
    fgets(nome, 30, stdin);
    setbuf(stdin, NULL);

    no* raiz = root;
    if(search(raiz, nome) == 0){
        searchAndDelete(raiz, nome);
        root = deleteNode(raiz, nome);
        printf("Produto deletado com sucesso.\n");
        system("pause");
        system("cls");

    }else{
        printf("Produto nao encontrado.\n");
        system("pause");
        system("cls");
    }
}

void altera(no *aux){
    printf("Nome do produto: %s", aux->nomeProd);
    arquivo = abrirArquivo("estoque.dat", 'l');
    fseek(arquivo, aux->indice*sizeof(produto), 0);
    fread(&prod, sizeof(produto), 1, arquivo);
    printf("Preco: %.2f\n", prod.preco);
    printf("Quantidade em estoque do produto: %d\n\n", prod.qtdEstoque);
    printf("1. Alterar somente o preco.\n");
    printf("2. Alterar somente a quantidade em estoque.\n");
    printf("3. Alterar o valor e a quantidade em estoque.\n");
    printf("Insira uma opcao: ");
    int opcao = 0;
    scanf("%d", &opcao);

    if(opcao == 1){
        printf("Digite o novo preco do produto: ");
        scanf("%f", &prod.preco);
        fseek(arquivo, aux->indice*sizeof(produto), 0);
        fwrite(&prod, sizeof(produto), 1, arquivo);
    }
    else if(opcao == 2){
        printf("Digite a nova quantidade do estoque do produto: ");
        int qtd = 0;
        scanf("%d", &prod.qtdEstoque);
        fseek(arquivo, aux->indice*sizeof(produto), 0);
        fwrite(&prod, sizeof(produto), 1, arquivo);
    }
    else{
        printf("Digite o novo preco do produto: ");
        scanf("%f", &prod.preco);
        printf("Digite a nova quantidade do estoque do produto: ");
        scanf("%d", &prod.qtdEstoque);
        fseek(arquivo, aux->indice*sizeof(produto), 0);
        fwrite(&prod, sizeof(produto), 1, arquivo);
    }
    fecharArquivo(arquivo);
}

no* searchAndEdit(no *aux, char nome[]){
    if(aux == NULL)
        return 0;
    if(strcmp(aux->nomeProd, nome) == 0){
        altera(aux);
        return 0;
    }
    else if(strcmp(nome, aux->nomeProd) < 0)
        searchAndEdit(aux->esq, nome);
    else if(strcmp(nome, aux->nomeProd) > 0)
        searchAndEdit(aux->dir, nome);
}

void alteraProduto(){
    printf("\t SUPERMERCADO: CONTROLE E REPOSICAO DE ESTOQUE\n");
    printf("\t\t\tALTERACAO DE PRODUTO:\n\n");
    printf("Digite o nome do produto que deseja alterar: ");
    char nome[30];
    fgets(nome, 30, stdin);
    no *aux = root;
    if(search(aux, nome) == 0){
        searchAndEdit(aux, nome);
        system("pause");
        system("cls");
    }else{
        printf("Produto nao encontrado.\n");
        system("pause");
        system("cls");
    }
}

no* searchKey(no *aux, char nome[]){
    if(aux == NULL)
        return 0;
    if(strcmp(aux->nomeProd, nome) == 0){
        printf("Nome do produto: %s", aux->nomeProd);
        arquivo = abrirArquivo("estoque.dat", 'l');
        fseek(arquivo, aux->indice*sizeof(produto), 0);
        fread(&prod, sizeof(produto), 1, arquivo);
        printf("Preco: %.2f\n", prod.preco);
        printf("Quantidade em estoque do produto: %d\n\n", prod.qtdEstoque);
        fecharArquivo(arquivo);
        return 0;
    }
    else if(strcmp(nome, aux->nomeProd) < 0)
        searchKey(aux->esq, nome);
    else if(strcmp(nome, aux->nomeProd) > 0)
        searchKey(aux->dir, nome);
}

void buscaProduto(){
    printf("\t SUPERMERCADO: CONTROLE E REPOSICAO DE ESTOQUE\n");
    printf("\t\t\tBUSCA POR PRODUTO ESPECIFICO:\n\n");
    printf("Digite o nome do produto que deseja buscar: ");
    char nome[30];
    fgets(nome, 30, stdin);
    no *aux = root;
    if(search(aux, nome) == 0){
        searchKey(aux, nome);
        system("pause");
        system("cls");

    }else{
        printf("Produto nao encontrado.\n");
        system("pause");
        system("cls");
    }
}

void searchAll(no *raiz){
    if(raiz != NULL){
        searchAll(raiz->esq);

        printf("Nome do produto: %s", raiz->nomeProd);
        arquivo = abrirArquivo("estoque.dat", 'l');
        printf("Indice: %d\n", raiz->indice);
        fseek(arquivo, raiz->indice*sizeof(produto), 0);
        fread(&prod, sizeof(produto), 1, arquivo);
        printf("Preco: %.2f\n", prod.preco);
        printf("Quantidade em estoque do produto: %d\n\n", prod.qtdEstoque);
        fecharArquivo(arquivo);

        searchAll(raiz->dir);
    }
}

void buscaTotal(void){
    printf("\t SUPERMERCADO: CONTROLE E REPOSICAO DE ESTOQUE\n");
    printf("\t\t\tBUSCA TOTAL:\n\n");
    no *aux = root;
    searchAll(root);
    system("pause");
    system("cls");
}

int exibeArvore(no* raiz){
    if(raiz == NULL)
        return 0;
    if(raiz != NULL){
        printf("\t     %s", raiz->nomeProd);

        if(raiz->esq == NULL && raiz->dir == NULL){
            printf("(null)\t\t\t  (null)");
            printf("\n\n");

        }else if(raiz->esq == NULL && !(raiz->dir == NULL)){
            printf("(null)\t\t\t  ");
            int i;
            printf("(");
            for(i = 0; i < strlen(raiz->dir->nomeProd) && raiz->dir->nomeProd[i] != '\n'; i++)
                printf("%c", raiz->dir->nomeProd[i]);
            printf(")");
            printf("\n\n");

        }else if(!(raiz->esq == NULL) && raiz->dir == NULL){
            int i;
            printf("(");
            for(i = 0; i < strlen(raiz->esq->nomeProd) && raiz->esq->nomeProd[i] != '\n'; i++)
                printf("%c", raiz->esq->nomeProd[i]);
            printf(")");

           if(strlen(raiz->esq->nomeProd)-1 > 5)
                printf("\t\t  ");
            else
                printf("\t\t\t  ");

            printf("(null)");
            printf("\n\n");

        }else{
            int i;
            printf("(");
            for(i = 0; i < strlen(raiz->esq->nomeProd) && raiz->esq->nomeProd[i] != '\n'; i++)
                printf("%c", raiz->esq->nomeProd[i]);

            if(strlen(raiz->esq->nomeProd)-1 > 5)
                printf(")\t\t  (");
            else
                printf(")\t\t\t  (");

            for(i = 0; i < strlen(raiz->dir->nomeProd) && raiz->dir->nomeProd[i] != '\n'; i++)
                printf("%c", raiz->dir->nomeProd[i]);
            printf(")");
            printf("\n\n");
        }
        exibeArvore(raiz->esq);
        exibeArvore(raiz->dir);
    }
}

FILE* abrirArquivo(char caminho[30], char modo){
    FILE *arquivo;
    switch(modo){
        case 'g':
            arquivo = fopen(caminho, "w+b");
            break;
        case 'l':
            arquivo = fopen(caminho, "r+b");
            break;
        case 'a':
            arquivo = fopen(caminho, "a+b");
            break;
    }
    if(arquivo == NULL){
        printf("Nao foi possivel abrir o arquivo");
        exit(0);
    }
    return arquivo;
}

//equivale ao fclose
void fecharArquivo(FILE *arquivo){
    fclose(arquivo);
}

void gravarEmArquivo(){
    arquivo = abrirArquivo("estoque.dat", 'a');
    fwrite(&prod, sizeof(produto), 1, arquivo);
    fecharArquivo(arquivo);
}

void carregaArquivo(){
    arquivo = abrirArquivo("estoque.dat", 'l');
    fseek(arquivo, 0, 0);
    do{
        fread(&prod, sizeof(produto), 1, arquivo);
        if(!feof(arquivo) && prod.nome[0] != '@'){
            root = insert(root, prod.nome, myIndex);

        }
        myIndex++;
    }while(!feof(arquivo));
    fecharArquivo(arquivo);
    myIndex--;
}

int search(no* aux, char nome[]){
    if(aux == NULL)
        return 1;
    if(strcmp(aux->nomeProd, nome) == 0){
        return 0;
    }
    else if(strcmp(nome, aux->nomeProd) < 0)
        search(aux->esq, nome);
    else if(strcmp(nome, aux->nomeProd) > 0)
        search(aux->dir, nome);
}
