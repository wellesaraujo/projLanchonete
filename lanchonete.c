#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

 typedef struct produto {
        char nome[20];
        float preco;
        int id;
        struct produto* prox;
    } produto ;

    typedef struct {
        char categoria[20];
        produto* inicio;
    } Lista;

    Lista* criaLista(char categoria[]){
        Lista* nova = (Lista*) malloc (sizeof(Lista));
        strcpy(nova->categoria, categoria);
        nova -> inicio = NULL;
        return nova;
    }

    Lista* Carrinho;

    void inserirItem (Lista * L, char nomeItem[], float precoItem, int idItem){
        produto* novo = (produto*) malloc (sizeof(produto));
        strcpy(novo->nome, nomeItem);
        novo->preco=precoItem;
        novo->id = idItem;
        novo->prox=NULL;

        if(L->inicio == NULL){
            L -> inicio = novo;
        }
        else {
            produto* aux = L->inicio;
            while(aux->prox != NULL){
                aux=aux->prox;
            }
            aux->prox=novo;
        }
        return;
    }

    produto* getProdutobyID(Lista* L, int id){
        produto* aux = L->inicio;
        while (aux!=NULL){
            if (aux->id == id)
                return aux;
            aux = aux -> prox;
        }
        return NULL;
    }

    void addNoCarrinho (produto* item){
        produto* itemComprado = (produto*) malloc (sizeof(produto));
        strcpy(itemComprado->nome , item->nome);
        itemComprado->preco = item ->preco;
        itemComprado->id=0;
        itemComprado->prox=NULL;

        if(Carrinho->inicio == NULL){
            Carrinho -> inicio = itemComprado;
        }
        else {
            produto* aux = Carrinho->inicio;
            while(aux->prox != NULL){
                aux=aux->prox;
            }
            aux->prox=itemComprado;
        }
        return;
    }

    void exibirLista (Lista* L){

        int op;
        int tamanhoLista;

        do{
            printf ("\t   ## %s ##\n", L->categoria);
            produto* aux;
            aux=L->inicio;
            tamanhoLista=0;
            op=0;
            while (aux!=NULL){
                printf("\t%d - %s [R$%5.2f]\n", aux->id, aux->nome, aux->preco);
                aux = aux -> prox;
                tamanhoLista++;
            }
            free(aux);
            printf("\n\tPressione 0 para Voltar\n");
            scanf("%d", &op);
            //system("clear");
            if(op>0 && op <=tamanhoLista){
                addNoCarrinho(getProdutobyID(L, op));
                printf ("\n\t%s adicionado ao carrinho!\n", getProdutobyID(L, op)->nome);
                sleep(1.5);
                system("clear");
            }
            else if (op!=0)
                printf("\tOpção inválida, tente novamente...\n");

        } while (op!=0);
    system("clear");
    return;
    }

    void imprimirRecibo (float total){
        produto* aux = Carrinho->inicio;
        FILE *nf;
        nf = fopen("nf.txt","w");
        fprintf(nf,"\t## RESUMO DO PEDIDO ##\n\n");
        while (aux!=NULL){
            fprintf(nf, "\t%s - R$%.2f \n", aux->nome, aux->preco);
            aux = aux -> prox;
        }
        fprintf(nf, "\n\t## TOTAL = R$%.2f ##\n", total);
        free(aux);
        fclose(nf);
        return;
    }

    void exibirCarrinho (){
        produto* aux = Carrinho->inicio;
        float total=0;
        printf("\t   ## CARRINHO ##\n\n");
        while (aux!=NULL){
            printf("\t%s - R$%.2f \n", aux->nome, aux->preco);
            total+=aux->preco;
            aux = aux -> prox;
        }
        printf("\n\tTOTAL = R$%.2f\n", total);
        free(aux);

        printf("\n\tPressione 1 para Finalizar a Compra\n");
        printf("\tPressione 0 para Adicionar items\n");
        int op=0;
        do{
            scanf("%d", &op);
            if(op==1){
                printf("\tIMPRIMINDO PEDIDO, AGUARDE...\n\n");
                sleep(1);
                imprimirRecibo(total);
                system("clear");
                free(Carrinho);
                printf("\tObrigadx, volte sempre!\n");
                exit(0);
                break;
            } else if (op!=0){
                printf("\tOpção inválida, tente novamente...\n");
            }
        } while(op!=0);
        system("clear");
        return;
    }

    int main(){

        char nome[20];
        int id;
        float preco;
        Lista* Lanches = criaLista("LANCHES");
        Lista* Bebidas = criaLista("BEBIDAS");
        Lista* Sobremesas = criaLista("SOBREMESAS");
        Carrinho = criaLista("CARRINHO");

        //Ler Arquivo Lanches
        FILE *lanchesTXT;
        lanchesTXT = fopen("InputFiles/lanches.txt", "r");
        if (lanchesTXT){
            do{
                fscanf(lanchesTXT, "%d %s %f",&id, nome, &preco);
                inserirItem(Lanches, nome, preco, id);
            } while (!feof(lanchesTXT));
        }
        fclose(lanchesTXT);

        //Ler Arquivo Bebidas
        FILE* bebidasTXT;
        bebidasTXT = fopen("InputFiles/bebidas.txt", "r");
        if (bebidasTXT){
            do{
                fscanf(bebidasTXT, "%d %s %f",&id, nome, &preco);
                inserirItem(Bebidas, nome, preco, id);
            } while (!feof(bebidasTXT));
        }
        fclose(bebidasTXT);

        //Ler Arquivo Sobremesas
        FILE* sobremesasTXT;
        sobremesasTXT = fopen("InputFiles/sobremesas.txt", "r");
        if (sobremesasTXT){
            do{
                fscanf(sobremesasTXT, "%d %s %f",&id, nome, &preco);
                inserirItem(Sobremesas, nome, preco, id);
            } while (!feof(sobremesasTXT));
        }
        fclose(sobremesasTXT);

        int op=0;
        printf("BEM VINDO A LANCHONETE\n\n");

        do {
            printf ("O QUE DESEJA?\n");
            printf ("1 - Lanches\n2 - Bebidas\n3 - Sobremesas\n4 - Exibir Carrinho\nPressione 0 para Sair\n");
            scanf("%d", &op);

            switch (op){
                case (0) :
                    system("clear");
                    printf("Obrigadx, volte sempre!");
                    free(Carrinho);
                    free(Lanches);
                    free(Bebidas);
                    free(Sobremesas);
                    break;
                case (1) :
                    system("clear");
                    exibirLista(Lanches);
                    break;
                case (2) :
                    system("clear");
                    exibirLista(Bebidas);
                    break;
                case (3) :
                    system("clear");
                    exibirLista(Sobremesas);
                    break;
                case (4) :
                    system("clear");
                    exibirCarrinho();
                    break;
            }
        } while(op!=0);

        return 0;
    }
