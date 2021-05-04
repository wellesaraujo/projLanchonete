#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

 typedef struct produto {
        char nome[20];
        float preco;
        int id;
        int quant;
        struct produto* prox ;
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
        novo->preco = precoItem;
        novo->id = idItem;
        novo->quant=0;
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

    void addNoCarrinho (int quant, produto* item){
        produto* itemComprado = (produto*) malloc (sizeof(produto));
        strcpy(itemComprado->nome , item->nome);
        itemComprado->preco = item ->preco;
        itemComprado->id=0;
        itemComprado->quant=quant;
        itemComprado->prox=NULL;

        produto* aux = Carrinho->inicio;
        int novoID=1;

        if(Carrinho->inicio == NULL){
            itemComprado->id=novoID;
            Carrinho -> inicio = itemComprado;
        }
        else if (strcmp(itemComprado->nome, aux->nome) == 0){
            aux->quant += itemComprado->quant; 
        }
        else {
            novoID++;
            while(aux->prox != NULL){
                aux=aux->prox;
                novoID++;
                if (strcmp(itemComprado->nome, aux->nome) == 0){
                    aux->quant += itemComprado->quant; 
                    return;
                }
            }
            itemComprado->id=novoID;
            aux->prox=itemComprado;
        }
        return;
    }

    void exibirLista (Lista* L){

        int op;
        int tamanhoLista;

        do{
            printf ("\t   ## %s ##\n", L->categoria);
            printf("\t%-3s - %-15s - %-5s\n","Cod","Item","Vl Unit");
            produto* aux;
            aux = L->inicio;
            tamanhoLista=0;
            op=0;
            while (aux!=NULL){
                printf("\t%-3d - %-15s - R$%-5.2f\n", aux->id, aux->nome, aux->preco);
                aux = aux -> prox;
                tamanhoLista++;
            }
            free(aux);
            printf("\n\tPressione 0 para Voltar ");
            scanf("%d", &op);
            if(op>0 && op <=tamanhoLista){
                int quant;
                produto* prod = getProdutobyID(L, op);
                printf("\n\tQuantos %s(s) deseja? ", prod->nome);
                scanf("%d", &quant);
                addNoCarrinho(quant, prod);
                printf ("\n\t%d %s(s) adicionado ao carrinho!\n",quant, prod->nome);
                sleep(1.5);
                system("clear");
            }
            else if (op!=0){
                printf("\tOpção inválida, tente novamente...\n");
                system("clear");
            }
        } while (op!=0);
    system("clear");
    return;
    }

    void imprimirRecibo (){
        produto* aux = Carrinho->inicio;
        float total =0;
        FILE *nf;
        nf = fopen("nf.txt","w");

        fprintf(nf,"\t## RESUMO DO PEDIDO ##\n\n");

        fprintf(nf,"\t%-3s - %-15s - %-5s -> %-5s\n","Qnt","Item","Vl Unit", "Vl Total");
        while (aux!=NULL){
            fprintf(nf,"\t%-3d - %-15s - R$%-5.2f -> R$%-5.2f \n", aux->quant, aux->nome, aux->preco, aux->quant * aux->preco);
            total+= aux->quant * aux->preco;
            aux = aux -> prox;
        }
        fprintf(nf, "\n\t## TOTAL = R$%.2f ##\n", total);
        free(aux);
        fclose(nf);
        return;
    }

    void removerItem(){
        produto* aux = Carrinho->inicio;
        float total=0;
        int cod =0;
        printf("\t   ## REMOVER ITEM ##\n\n");
        printf("\t%-3s - %-15s - %-5s\n","Cod","Item","Quantidade");
        while (aux!=NULL){
            cod++;
            printf("\t%-3d - %-15s - %-5d\n", cod, aux->nome, aux->quant);
            aux = aux -> prox;
        }
        printf("\n\tPressione 0 para voltar ");
        int op=0;
        do{
            scanf("%d", &op);
             if(op>0 && op <=cod){
                 produto* anterior=Carrinho->inicio;
                 produto* excluir=Carrinho->inicio;
                 if(op==1){
                     Carrinho->inicio = excluir->prox;
                     printf("\tItem (%s) removido com sucesso!\n", excluir->nome);
                     free(excluir);
                     sleep(1.5);
                     return;
                 } else {
                     excluir=excluir->prox;
                     for (int i=2; i<op; i++){
                         anterior=anterior->prox;
                         excluir=excluir->prox;
                     }
                     anterior->prox = excluir->prox;
                     printf("\tItem (%s) removido com sucesso!\n", excluir->nome);
                     free(excluir);
                     sleep(1.5);
                     return;
                 }   
            }
            else if (op!=0)
                printf("\tOpção inválida, tente novamente...\n");

        } while (op!=0);
        free(aux);
        return;
        }
    

    void exibirCarrinho (){
        produto* aux = Carrinho->inicio;
        float total=0;
        printf("\t   ## CARRINHO ##\n\n");
        printf("\t%-3s - %-15s - %-5s -> %-5s\n","Qnt","Item","Vl Unit", "Vl Total");
        while (aux!=NULL){
            printf("\t%-3d - %-15s - R$%-5.2f -> R$%-5.2f \n", aux->quant, aux->nome, aux->preco, aux->quant * aux->preco);
            total+= aux->quant * aux->preco;
            aux = aux -> prox;
        }
        printf("\n\tTOTAL = R$%.2f\n", total);
        free(aux);

        printf("\n\tPressione:\n");
        printf("\t0 para Adicionar itens\n");
        printf("\t1 para Remover itens\n");
        printf("\t2 para Finalizar a Compra\n\t");
        int op=0;
        do{
            scanf("%d", &op);
            if(op==2){
                printf("\tIMPRIMINDO PEDIDO, AGUARDE...\n\n");
                sleep(1);
                imprimirRecibo();
                system("clear");
                free(Carrinho);
                printf("\tObrigadx, volte sempre!\n");
                exit(0);
                break;
            } else if (op==1){
                system("clear");
                removerItem();
                system("clear");
                exibirCarrinho();
                return;
            }else if (op!=0){
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
