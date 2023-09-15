#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void limpa_tela(){
	#ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausa_tela(){
	#ifdef WIN32
	#define PAUSE 1
	#else
	#define PAUSE 0
	#endif
	if(PAUSE==1)
	   system("pause");
	else
	   system("read -p \"Pressione enter para sair\" saindo");
}

enum categoria_enum{
	Alimenticio = 1,
	Higiene = 2,
	Eletronico = 3
}categorias;

struct Produto{
    int codigo;
    char descricao [100];
    categoria_enum categoria;
    int qtd_estoque;
    float preco;
    struct Produto *proximo;
};

struct Carrinho{
	struct Produto *produto;
	int quantidade;
	struct Carrinho *proximo;
};

struct Carrinho *inicio_carrinho = NULL;
struct Carrinho *fim_carrinho = NULL;

struct Produto *inicio = NULL;


categoria_enum int_para_categoria(int categoria){
	switch (categoria) {
        case 1:
            return Alimenticio;
        case 2:
            return Higiene;
        case 3:
            return Eletronico;
    }
}

void cria_estoque(){
	struct Produto arr_produtos[] = {
		{1, "Sabao em po Omo 1Kg", Higiene, 10, 12.89},
		{2, "Sabonete Nivea Unid.", Higiene, 15, 2.69},
		{3, "Escova de dentes Colgate", Higiene, 8, 8.89},
		{4, "Sucrilhos Cx 500g", Alimenticio, 10, 9.99},
		{5, "Pao de Queijo Unid.", Alimenticio, 8, 3.49},
		{6, "Leite Condensado Itambe", Alimenticio, 8, 5.59},
		{7, "Impressora 3D", Eletronico, 3, 2399.99},
		{8, "Super Nintendo Destravado", Eletronico, 2, 449.99},
		{9, "Windows 95", Eletronico, 90, 5499.99},
		{10, "Chip Cerebral com Integracao ChatGPT", Eletronico, 18, 800.00}
	};
    FILE *arquivo_estoque = fopen("estoque.bin", "wb");
	int array_size = sizeof(arr_produtos)/sizeof(struct Produto);
	if(arquivo_estoque != NULL){
			fwrite(&arr_produtos, sizeof(struct Produto), array_size, arquivo_estoque);
			fclose(arquivo_estoque);
	}
}

void salva_arquivo(){
    FILE *arquivo_estoque = fopen("estoque.bin", "wb");
    struct Produto *produtos = inicio;
    if(arquivo_estoque != NULL){
    while(produtos != NULL){
        struct Produto *proximo = produtos->proximo;
        produtos->proximo = NULL;
        fwrite(produtos, sizeof(struct Produto), 1, arquivo_estoque);
        produtos = proximo;
    }
    fclose(arquivo_estoque);
    }
	if(arquivo_estoque != NULL){
			
	}
}

categoria_enum seleciona_categoria(){
	int categoria;

	while(true){

    printf("=========Categoria=============");
    printf("\n1-Alimenticio");
    printf("\n2-Higiene");
    printf("\n3-Eletronico");
    printf("\nEscolha uma opcao:");
    scanf("%d",&categoria);

	limpa_tela();
   	if(categoria > 0 && categoria < 4)
		return int_para_categoria(categoria);
	printf("Opcao invalida...\nTente novamente...\n");
	}
}

void le_estoque(){
	FILE *arquivo_estoque = fopen("estoque.bin", "rb");
	struct Produto produto;
	if(arquivo_estoque != NULL){
	while(fread(&produto, sizeof(struct Produto), 1, arquivo_estoque) == 1){
		struct Produto *pProduto = (struct Produto *)malloc(sizeof(struct Produto));
		pProduto->codigo = produto.codigo;
		strcpy(pProduto->descricao, produto.descricao);
		pProduto->categoria = produto.categoria;
		pProduto->qtd_estoque = produto.qtd_estoque;
		pProduto->preco = produto.preco;
		pProduto->proximo = inicio;
		inicio = pProduto;
	}
	fclose(arquivo_estoque);
	}
}

void listar_produtos(){
	limpa_tela();
	struct Produto *aux = inicio;

	while(aux != NULL){
		printf("Codigo: %d - %s: R$ %.2f\n", aux->codigo, aux->descricao, aux->preco);
		aux = aux->proximo;
	}
	pausa_tela();
}

void listar_com_filtro(categoria_enum cat){
	struct Produto *aux = inicio;
	while(aux != NULL){
		if(aux->categoria == cat){

		printf("Codigo: %d - %s: R$ %.2f\n", aux->codigo, aux->descricao, aux->preco);
	}
		aux = aux->proximo;
	}
}

struct Carrinho *listar_carrinho_pelo_codigo(int codigo){

    struct Carrinho *aux = inicio_carrinho;
    while(aux != NULL){
		if(aux->produto->codigo == codigo){
		return aux;
	}
		aux = aux->proximo;
	}
	return NULL;
};

struct Produto *listar_pelo_codigo(int codigo){
	struct Produto *aux = inicio;
	while(aux != NULL){
		if(aux->codigo == codigo){
		return aux;
	}
		aux = aux->proximo;
	}
	return NULL;
}

void exibir_menu_de_filtro(){
	limpa_tela();
	categoria_enum cat = seleciona_categoria();
	listar_com_filtro(cat);
	pausa_tela();
}

void altera_quantidade_carrinho(int codigo, int quantidade){	
	struct Carrinho *aux = inicio_carrinho;
	bool flag = false;
	if(aux == NULL){
		struct Carrinho *car = (struct Carrinho *)malloc(sizeof(struct Carrinho));
		car->produto = listar_pelo_codigo(codigo);
		car->quantidade = quantidade;
		car->proximo = NULL;
		inicio_carrinho = car;
		fim_carrinho = car;
	}else{
		while(aux != NULL){
			if(aux->produto->codigo == codigo){
				flag = true;
				aux->quantidade = aux->quantidade + quantidade;
				break;
			}
			aux = aux->proximo;
		}
		if(!flag){
			struct Carrinho *car = (struct Carrinho *)malloc(sizeof(struct Carrinho));
			car->produto = listar_pelo_codigo(codigo);
			car->quantidade = quantidade;
			fim_carrinho->proximo = car;
			fim_carrinho = car;
		}
		}
}

bool valida_quantidade_do_produto(int codigo, int quantidade){
    struct Produto *produto = listar_pelo_codigo(codigo);
    struct Carrinho *carrinho = listar_carrinho_pelo_codigo(codigo);
    if (carrinho == NULL) {
	return produto->qtd_estoque >= quantidade;
	}
    return produto->qtd_estoque >= carrinho->quantidade + quantidade;
}

void exibe_carrinho(){
	limpa_tela();
	float total = 0;
	struct Carrinho *aux = inicio_carrinho;
	if(aux == NULL){
		printf("Voce nao possui nenhum item adicionado no seu carrinho...\n");
		pausa_tela();
	}else{
	while(aux != NULL){
		printf("Produto - %s\nQuantidade: %d\nPreco Unitario: %.2f\nTotal do Item: %.2f\n", aux->produto->descricao, aux->quantidade, aux->produto->preco, aux->quantidade * aux->produto->preco);
		printf("---------------------------------------------------\n");
		total = total + (aux->quantidade * aux->produto->preco);
		aux = aux->proximo;
	}
	printf("\n\nTotal do carrinho: %.2f\n\n", total);
	pausa_tela();
	}
}

void adiciona_ao_carrinho(){
	limpa_tela();
	int codigo;
	struct Produto *produto_encontrado = NULL;
	while(true){
    printf("=========Adicionar ao Carrinho=============");
    printf("\nDigite o código do produto:\n");
    scanf("%d",&codigo);
	produto_encontrado = listar_pelo_codigo(codigo);
	limpa_tela();
   	if(produto_encontrado)
		break;
	printf("Opcao invalida...\nTente novamente...\n");
	}
	int quantidade;
	while(true){
		printf("Digite zero para cancelar\n");
		printf("Produto: %s\nQuantidade disponivel - %d\n", produto_encontrado->descricao, produto_encontrado->qtd_estoque);
		printf("Digite a quantidade que deseja adicionar ao carrinho:\n");
    	scanf("%d", &quantidade);
    	if(quantidade == 0){
    		break;
		}
    	if(valida_quantidade_do_produto(codigo, quantidade)){
            break;
		}
		limpa_tela();
		printf("Quantidade invalida...\nTente novamente...\n");
	}
	if(quantidade != 0){
	    limpa_tela();
		altera_quantidade_carrinho(codigo, quantidade);
		printf("Produto adicionado com sucesso!\n\n");
		pausa_tela();
	}
}

struct Carrinho *encontra_item_carrinho_anterior(int codigo){
	struct Carrinho *aux = inicio_carrinho;
    while(aux != NULL){
        if(aux->produto->codigo == codigo) return aux;
		if(aux->proximo->produto->codigo == codigo) return aux;
		aux = aux->proximo;
	}
	return NULL;
}

int exibe_menu_alteracao(){
	int opcao;
	while(true){
		printf("============MENU DE ALTERACAO============\n");
		printf("1 - Remover Item\n");
		printf("2 - Alterar quantidade do item\n");
		printf("3 - Cancelar\n\n");
		printf("Digite a opcao desejada: \n");
		scanf("%d", &opcao);
		if(opcao > 0 && opcao < 4) return opcao;
		limpa_tela();
		printf("OPCAO INVALIDA...\n\n");
	}
}

int exibe_menu_alteracao_codigo(){
	int codigo;
	struct Carrinho *carrinho_encontrado;
	limpa_tela();
	while(true){
		printf("Digite zero para cancelar.\n");
		printf("=============Alterrar Carrinho=============\n");
		printf("Digite o codigo do produto: \n");
		scanf("%d", &codigo);
		carrinho_encontrado = listar_carrinho_pelo_codigo(codigo);
		if(codigo == 0) break;
		if(carrinho_encontrado) break;
		limpa_tela();
		printf("Produto não encontrado...\n\n");
	}
	return codigo;
}

void deleta_item(int codigo){
    limpa_tela();
    struct Carrinho *carrinho_anterior = encontra_item_carrinho_anterior(codigo);
	if(carrinho_anterior->produto->codigo == codigo) inicio_carrinho = carrinho_anterior->proximo;
	else carrinho_anterior->proximo = carrinho_anterior->proximo->proximo;
	printf("PRODUTO RREMOVIDO DO CARRINHO...\n\n");
	pausa_tela();
}

void remove_item_do_carrinho(){
	limpa_tela();
	int codigo;
	codigo = exibe_menu_alteracao_codigo();
	if(codigo == 0) return;
	deleta_item(codigo);
}

void altera_item_do_carrinho(){
    limpa_tela();
	int codigo;
	int quantidade;
	codigo = exibe_menu_alteracao_codigo();
	limpa_tela();
	if(codigo == 0) return;
	struct Carrinho *carrinho_encontrado = listar_carrinho_pelo_codigo(codigo);
	while(true){
		printf("=============Alterrar Carrinho=============\n");
		printf("Produto: %s\nQuantidade atual: %d\n\n", carrinho_encontrado->produto->descricao, carrinho_encontrado->quantidade);
		printf("Digite a nova quantidade: \n");
		scanf("%d", &quantidade);
		if(valida_quantidade_do_produto(codigo, quantidade) && quantidade >= 0){
		    if(quantidade == 0){
		        deleta_item(codigo);
		        break;
		    }
			carrinho_encontrado->quantidade = quantidade;
			break;
		}
		limpa_tela();
		printf("Quantidade inválida\n\n");
	}
	
}

void altera_carrinho(){
	limpa_tela();
	int codigo;
	int quantidade;
	int opcao;
	struct Carrinho *carrinho_encontrado;
	
	opcao = exibe_menu_alteracao();
	limpa_tela();
	
	switch(opcao){
		case 1:
			remove_item_do_carrinho();
			return ;
			break;
		case 2:
		    altera_item_do_carrinho();
	    	return ;
			break;
		case 3:
			limpa_tela();
			return;
	}
	
}

void efetua_compra(){
    struct Carrinho *carrinho = inicio_carrinho;
    while(carrinho != NULL){
        carrinho->produto->qtd_estoque = carrinho->produto->qtd_estoque - carrinho->quantidade;
        carrinho = carrinho->proximo;
    }
    inicio_carrinho = NULL;
    fim_carrinho = NULL;
    limpa_tela();
}

void sair(){
    salva_arquivo();
    limpa_tela();
    printf("Obrigado por utilizar nossos servicos!\n\n");
}

void menu(){
	limpa_tela();
    int resp;

    printf("=========CARRINHO DE COMPRAS=============");
    printf("\n1-Listar todos os produtos.");
    printf("\n2-Buscar produto de acordo com a categoria.");
    printf("\n3-Adicionar produtos em carrinho de compra.");
    printf("\n4-Visualizar carrinho de compra");
    printf("\n5-Editar carrinho de compra");
    printf("\n6-Efetuar a compra.");
    printf("\n7-Sair.");
    printf("\nEscolha uma opÃ§Ã£o:");
    scanf("%d",&resp);

    switch (resp)
    {
    case 1:
        listar_produtos();
        menu();
        break;
    case 2:
        exibir_menu_de_filtro();
        menu();
        break;
    case 3:
        adiciona_ao_carrinho();
        menu();
        break;
    case 4:
        exibe_carrinho();
        menu();
        break;
    case 5:
        altera_carrinho();
        menu();
        break;
    case 6:
        exibe_carrinho();
        efetua_compra();
        menu();
        break;
    case 7:
        sair();
     default:
        break;
    }
}

main(){
	FILE *estoque = fopen("estoque.bin", "r");
	if(estoque == NULL) cria_estoque();
	le_estoque();
	menu();
	pausa_tela();	

}
