#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

/*
==================================================
CONSTANTES
==================================================
*/

#define NO_VALOR_INICIAL 0

#define ERROR_FALHA_ALOCACAO "\n\tERRO: Erro durante alocação de memória!\n"
#define ERROR_INSERIR_REPETIDO "\n\tERRO: Valor já existe na árvore binária!\n"
#define ERROR_VALOR_NAO_ENCONTRADO "\n\tERRO: Valor não encontrado!\n"
#define ERROR_ARVORE_VAZIA "\n\tERRO: Árvore binária vazia!\n"

/*
==================================================
STRUCTS
==================================================
*/

typedef struct No {
    int valor;
    int altura;
    struct No *esquerda;
    struct No *direita;
}No;

typedef struct ArvoreBinaria {
    No *raiz;
    int tamanho;
    int altura;
}ArvoreBinaria;

/*
==================================================
DECLARAÇÃO DE MÉTODOS
==================================================
*/

No* _criarNo();
No* _insere(No *no, int valor);
int _findNo(No *no, int chave);
No* _removerNoComZeroFilhos(No *no);
No* _removerNoComUmFilho(No *no);
No* _removerNoComDoiFilhos(No *no, int chave);
No* _removeNo(No *no, int chave);
void _imprimirNo(No *no);
void _preencharDadosArvore(ArvoreBinaria *arvore);
int _calcularTamanhoArvore(No *no);
int _calcularAlturaArvore(No *no);
void _preencherAlturaNo(No *no, int altura);

int _getAltura(No *no);
int _altura(No *no);
int _maior(int x, int y);
int _fatorBalaceamentoNo(No *no);
No* _rotacaoLL(No *raiz);
No* _rotacaoRR(No *raiz);
No* _rotacaoLR(No *raiz);
No* _rotacaoRL(No *raiz);
No* _balancearArvoreDireita(No *no, int valor);
No* _balancearArvoreEsquerda(No *no, int valor);
No* _balancearArvoreDireitaRemover(No *raiz);
No* _balancearArvoreEsquerdaRemover(No *raiz);

ArvoreBinaria *criarArvore();
void addArvore(ArvoreBinaria *arvore, int valor);
int findArvore(ArvoreBinaria *arvore, int chave);
void removeArvore(ArvoreBinaria *arvore, int chave);
void imprimirArvore(ArvoreBinaria *arvore);

/*
==================================================
CONTROLES DE ALTERAÇÕES NA ÁRVORE
==================================================
*/

No *_criarNo() {
    No* no = (No*)malloc(sizeof(No));

    if (no != NULL) {
        no->valor = NO_VALOR_INICIAL;
        no->altura = 0;
        no->esquerda = NULL;
        no->direita = NULL;
    }

    return no;
}

No* alocarNo(int valor) {
    No* novo = _criarNo();

    novo->valor = valor;

    return novo;
}

No* _insere(No *no, int valor) {
    if(no == NULL) {
        no = alocarNo(valor);
    } else if(valor < no->valor) {
        no->esquerda = _insere(no->esquerda, valor);
        no = _balancearArvoreEsquerda(no, valor);
    } else if(valor > no->valor)   {
        no->direita = _insere(no->direita, valor);
        no = _balancearArvoreDireita(no, valor);
    }

    no->altura = _getAltura(no);
    return no;
}

int _findNo(No *no, int chave) {
    if (no == NULL) {
        printf(ERROR_VALOR_NAO_ENCONTRADO);
        return NULL;
    }
    if (chave > no->valor) {
        return _findNo(no->direita, chave);
    }
    if (chave < no->valor) {
        return _findNo(no->esquerda, chave);
    }

    return no->valor;
}

/*
Remove NÓ que não possui qualquer NÓ filho
*/
No* _removerNoComZeroFilhos(No *no) {
    free(no);
    return NULL;
}

/*
Remove NÓ que possui apenas um NÓ filho
*/
No* _removerNoComUmFilho(No *no) {
    No *suporte;

    if (no->esquerda != NULL) {
        suporte = no->esquerda;
    } else {
        suporte = no->direita;
    }

    free(no);
    return suporte;
}

/*
Remove NÓ que possui dois NÓs filhos
*/
No* _removerNoComDoiFilhos(No *no, int chave) {
    No *suporte = no->esquerda;

    while (suporte->direita != NULL) {
        suporte = suporte->direita;
    }
    no->valor = suporte->valor;
    suporte->valor = chave;
    no->esquerda = _removeNo(no->esquerda, chave);

    return no;
}

No* _removeNo(No *no, int chave) {
    if (no == NULL) {
        printf(ERROR_VALOR_NAO_ENCONTRADO);
        return NULL;
    }

    if (chave == no->valor) {
        if (no->esquerda == NULL && no->direita == NULL) {
            return _removerNoComZeroFilhos(no);
        }
        if (no->esquerda == NULL || no->direita == NULL) {
            return _removerNoComUmFilho(no);
        }
        no = _removerNoComDoiFilhos(no, chave);
        return _balancearArvoreEsquerdaRemover(no);
    } else {
        if (chave > no->valor) {
            no->direita = _removeNo(no->direita, chave);
            no = _balancearArvoreDireitaRemover(no);
        } else {
            no->esquerda = _removeNo(no->esquerda, chave);
            no = _balancearArvoreEsquerdaRemover(no);
        }
        return no;
    }
}

void _imprimirNo(No *no) {
    if (no != NULL) {
        _imprimirNo(no->esquerda);
        printf("[%d | %d] ", no->valor, no->altura);
        _imprimirNo(no->direita);
    }
}

void _preencharDadosArvore(ArvoreBinaria *arvore) {
    arvore->tamanho = _calcularTamanhoArvore(arvore->raiz);
    arvore->altura = _calcularAlturaArvore(arvore->raiz);
    _preencherAlturaNo(arvore->raiz, arvore->altura);
}

/*
Calcular o número de elementos salvos numa árvore
*/
int _calcularTamanhoArvore(No *no) {
    if (no == NULL) {
        return 0;
    }

    int size = 1;
    
    size += _calcularTamanhoArvore(no->esquerda);
    size += _calcularTamanhoArvore(no->direita);

    return size;
}

int _calcularAlturaArvore(No *no) {
    if (no == NULL) {
        return -1;
    }

    return _maior(_calcularAlturaArvore(no->esquerda), _calcularAlturaArvore(no->direita)) + 1;
}

void _preencherAlturaNo(No *no, int altura) {
    if (no == NULL) {
        return;
    }

    no->altura = altura;
    _preencherAlturaNo(no->esquerda, altura - 1);
    _preencherAlturaNo(no->direita, altura - 1);
}

/*
==================================================
BALANCEAMENTO
==================================================
*/

int _getAltura(No *no) {
    return _maior(_altura(no->esquerda), _altura(no->direita)) + 1;
}

int _altura(No *no) {
    if (no == NULL) {
        return -1;
    }
    return no->altura;
}

int _maior(int x, int y) {
    if (x > y) {
        return x;
    }
    return y;
}

int _fatorBalaceamentoNo(No *no) {
    return labs(_altura(no->esquerda) - _altura(no->direita));
}

No* _rotacaoLL(No *raiz) {
    No *suporte = raiz->esquerda;

    raiz->esquerda = suporte->direita;
    suporte->direita = raiz;

    raiz->altura = _getAltura(raiz);
    suporte->altura = _maior(_altura(suporte->esquerda), raiz->altura) + 1;
    
    return suporte;
}

No* _rotacaoRR(No *raiz) {
    No *suporte = raiz->direita;

    raiz->direita = suporte->esquerda;
    suporte->esquerda = raiz;

    raiz->altura = _getAltura(raiz);
    suporte->altura = _maior(_altura(suporte->direita), raiz->altura) + 1;

    return suporte;
}

No* _rotacaoLR(No *raiz) {
    raiz->esquerda = _rotacaoRR(raiz->esquerda);
    return _rotacaoLL(raiz);
}

No* _rotacaoRL(No *raiz) {
    raiz->direita = _rotacaoLL(raiz->direita);
    return _rotacaoRR(raiz);
}

No* _balancearArvoreDireita(No *raiz, int valor) {
    if (_fatorBalaceamentoNo(raiz) > 1) {
        if (valor > raiz->direita->valor) {
            raiz = _rotacaoRR(raiz);
        } else {
            raiz = _rotacaoRL(raiz);
        }
    }
    return raiz;
}

No* _balancearArvoreEsquerda(No *raiz, int valor) {
    if (_fatorBalaceamentoNo(raiz) > 1) {
        if (valor < raiz->esquerda->valor) {
            raiz = _rotacaoLL(raiz);
        } else {
            raiz = _rotacaoLR(raiz);
        }
    }
    return raiz;
}

No* _balancearArvoreDireitaRemover(No *raiz) {
    if (_fatorBalaceamentoNo(raiz) > 1) {
        if (_altura(raiz->esquerda->direita) > _altura(raiz->esquerda->esquerda)) {
            raiz = _rotacaoLR(raiz);
        } else {
            raiz = _rotacaoLL(raiz);
        }
    }
    return raiz;
}

No* _balancearArvoreEsquerdaRemover(No *raiz) {
    if (_fatorBalaceamentoNo(raiz) > 1) {
        if (_altura(raiz->direita->direita) < _altura(raiz->direita->esquerda)) {
            raiz = _rotacaoRL(raiz);
        } else {
            raiz = _rotacaoRR(raiz);
        }
    }
    return raiz;
}

/*
==================================================
MÉTODOS PÚBLICOS
==================================================
*/

// Cria e retorna uma nova instância de 'ArvoreBinaria'
ArvoreBinaria *criarArvore() {
    ArvoreBinaria *arvore = (ArvoreBinaria*)malloc(sizeof(ArvoreBinaria));

    arvore->raiz = NULL;
    arvore->tamanho = 0;
    arvore->altura = 0;

    return arvore;
}

void addArvore(ArvoreBinaria *arvore, int valor) {
    arvore->raiz = _insere(arvore->raiz, valor);
    _preencharDadosArvore(arvore);
}

int findArvore(ArvoreBinaria *arvore, int chave) {
    if (arvore->raiz != NULL) {
        return _findNo(arvore->raiz, chave);
    }
    printf(ERROR_ARVORE_VAZIA);
    return NULL;
}

void removeArvore(ArvoreBinaria *arvore, int chave) {
    if (arvore->raiz != NULL) {
        arvore->raiz = _removeNo(arvore->raiz, chave);
        _preencharDadosArvore(arvore);
    } else {
        printf(ERROR_ARVORE_VAZIA);
    }
}

void imprimirArvore(ArvoreBinaria *arvore) {
    printf("\n===== Árvore Binária =====\n");
    printf("\nTamanho: %d", arvore->tamanho);
    printf("\nAltura: %d", arvore->altura);
    printf("\n\n");
    if (arvore->raiz != NULL) {
        _imprimirNo(arvore->raiz);
    }
    printf("\n\n==========================\n");
}

/*
==================================================
MAIN
==================================================
*/

int selecionarOpcoes() {
    int select;

    printf("\n");
    printf("\n1 - Inserir");
    printf("\n2 - Imprimir");
    printf("\n3 - Buscar");
    printf("\n4 - Remover");
    printf("\n0 - Sair");
    printf("\nSelect: ");

    scanf("%d", &select);

    return select;
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL, "Portuguese");

    ArvoreBinaria *arvore = criarArvore();
    int select, loop = 1;
    int entrada;

    while (loop) {
        select = selecionarOpcoes();
        printf("\n");
        switch (select) {
            case 1:
                printf("Novo valor: ");
                scanf("%d", &entrada);
                addArvore(arvore, entrada);
                break;
            case 2:
                imprimirArvore(arvore);
                break;
            case 3:
                printf("Buscar: ");
                scanf("%d", &entrada);
                entrada = findArvore(arvore, entrada);
                printf("\nResultado: %d", entrada);
                break;
            case 4:
                printf("Remover: ");
                scanf("%d", &entrada);
                removeArvore(arvore, entrada);
                break;
            default:
                printf("\n\tSaindo...\n\n");
                loop = 0;
                break;
        }
    }

    free(arvore);
    
    return EXIT_SUCCESS;
}