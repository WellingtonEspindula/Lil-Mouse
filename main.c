/*

LIL MOUSE: The mouse trapper

AUTORES: Rafael Trevisan (ravtrevisan@inf.ufrgs.br) e Wellington Espindula (wmespindula@inf.ufrgs.br), também conhecidos como Raffa Moreira e Lil Tom

DESCRIÇÃO: Nesse jogo, o usuário controla um rato, que move-se a partir da entrada do usuário nas setas - ou nas teclas W, A, S e D -, e deve coletar
todo o queijo de um labirinto.
Existem também 4 gatos, que andam em direções aleatórias e tentarão “comer” o gato - isto é, farão o rato perder uma vida caso entrem em contato com ele.
Entretanto, existem ossos no labirinto, que quando coletados pelo rato, o transforma temporariamente em um cachorro; assim, o rato em modo cachorro pode
derrotar os gatos e mandá-los para as suas posições iniciais. Existem também portas móveis nesse labirinto que mudam de posição quando o jogador clica
“B/b”, mudando a configuração do mapa.
Quando o jogador coleta todos os queijos de uma fase, ele passa para a próxima, e esse processo repete-se até que todas as fases acabem.
Cada queijo coletado dá ao jogador 10 pontos, e cada gato derrotado da 50. O objetivo do jogo é conseguir a maior pontuação possível antes de perder as
suas 5 vidas, ou mesmo conseguir passar todas as fases enquanto houverem vidas.

IMPORTANTE:
Para executar o jogo, lembre de incluir a conio2.h e a libconio, bem como a biblioteca libwinmm
disponibilizada nas bibliotecas da MinGW
*/

//Inclui bibliotecas externas
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <conio2.h>
#include <windows.h>

/* CONSTANTES DE LIMITES DO MAPA DO JOGO */
#define LIMITE_X 27
#define LIMITE_Y 11

#define NUM_GATOS 4

/* CONSTANTES QUE INDICAM PARA USO EM DEFINICAO DE DIRECAO */
#define ESQUERDA -1
#define DIREITA 1

#define PARADO 0

#define CIMA -1
#define BAIXO 1

/* CONSTANTES DE LEITURA DO MAPA */
#define RATO 'M'
#define PORTA 'T'
#define GATO 'G'
#define OSSO 'O'
#define PAREDE 'X'
#define QUEIJO 'Q'

/* CONSTANTES DE LETURA DE TECLADO */
#define CTE_SETAS 224
#define SETA_ESQUERDA 75
#define SETA_CIMA 72
#define SETA_DIREITA 77
#define SETA_BAIXO 80

#define KEY_A 65
#define KEY_W 87
#define KEY_D 68

#define KEY_TAB 9
#define KEY_V 86
#define KEY_N 78
#define KEY_C 67
#define KEY_Q 81
#define KEY_B 66
#define KEY_S 83

/* CONSTANTES DE MODO DO RATO */
#define MODO_RATO 0
#define MODO_CACHORRO 1
/* FLAG NÃO IMPLEMENTADA
* #define MODO_COVARDE 2
*/

/* CONSTANTES DE MODO EM QUE A PORTA SE ENCONTRA */
#define MODO_PORTA_INICIAL 0
#define MODO_PORTA_INVISIVEL 1

#define NIVEL_MAX 3
#define WAIT_TIME 10 //tempo de espera do programa

/* CONSTANTES DE DIMENSAO (EM CARACTERES)
    DE OBJETOS NA TELA */
#define DIMENSAO_X_TELA 2
#define DIMENSAO_Y_TELA 2

/* PESO DA PONTUACAO DE QUEIJO COMIDO E DE
GATO COMIDO EM MODO CACHORRO */
#define PESO_PONTO 10
#define PESO_PONTO_CACHORRO 50

#define TEMPO_MODO_CACHORRO 5000    //5000 ms = 5 s
#define TEMPO_MOVIMENTO_GATOS 1000  //1000 ms = 1 s

/*
    Estruturas do jogo
*/
typedef struct {
    int x;
    int y;
} POSICAO;

typedef struct {
    int x;
    int y;
} DIRECAO;

typedef struct {
    POSICAO posicaoInicial;  //posicao na qual rato inicia no mapa
    POSICAO posicaoAtual;    //posicao atual do rato no mapa
    int vidas;
    int modo;
} STR_RATO;

typedef struct {
    POSICAO posicaoInicial; //posicao na qual gato inicia no mapa
    POSICAO posicaoAtual;   //posicao atual do gato no mapa
    DIRECAO direcao;
} STR_GATO;

typedef struct {
    bool parede;
    bool queijo;
    bool osso;
    bool portaInicial;
    bool portaInvisivel;
} MAPA;

typedef struct {
    int contagemPontos;
    int nivel;
    STR_GATO gatos[NUM_GATOS];
    STR_RATO rato;
    MAPA mapa[LIMITE_X][LIMITE_Y];
    bool modoPortas;
} DADOS;

int contagemPontos;
int nivel;

STR_GATO gatos[NUM_GATOS];
STR_RATO lilmouse;
MAPA mapa[LIMITE_X][LIMITE_Y];

/* variável que indica em que modo as portas se encontram:
abertas ou fechadas */
bool modoPortas;

/* variável que indica se o menu encontra-se exibido */
bool exibicaoMenu;

/* variáveis que indicam onde começa as coordanadas dos
mapas centralizados na tela */
int xInicialMapa;
int yInicialMapa;

/* variáveis que indicam o tamanho da tela */
int alturaTela;
int larguraTela;

/* PROTOTIPO DAS FUNCOES */

/**************************************
 *  FUNCOES DO JOGO
 **************************************/


/** \brief Funcao que inicializa as variaveis de
ambiente para a inicializacao de um novo jogo.
 */
void novoJogo();

/** \brief Funcao que realiza as operacoes para a
mudanca de nivel
 */
void mudaNivel();

/** \brief Funcao chamada quando o usuario ganha para
exibir que o usuario ganhou na tela.
 */
void ganhou();

/** \brief Funcao chamada quando o usuario perde para
informar que perdeu.
 */
void perdeu();


/*********************************************************************
 * FUNÇÕES QUE SALVAM E CARREGAM O ESTADO DO JOGO EM AQUIVO DE TEXTO
 *********************************************************************/


/** \brief Define as variaveis de
 * ambiente utilizando os valores da variavel do tipo DADOS
 * passada como parametro.
 *
 * \param dados - Dados que guardam o estado do jogo que sera carregado
 *
 */
void carregaJogo(DADOS dados);

/** \brief Utiliza as variaveis de
ambiente para popular uma variavel do tipo DADOS
 *
 * \param *dados - Ponteiro do tipo DADOS que
 ira armazenar as variaveis atuais de contexto
 *
 */
void getDados(DADOS *dados);

/** \brief Abre arquivo binario correspondente ao nome passado como
parametro e armazena os dados de um jogo anterior em uma
variavel do tipo DADOS.
 *
 * \param *nomeArquivo - String que recebe o nome do arquivo que sera aberto
 * \param *dados - Ponteiro que ira receber os dados do arquivo.
 * \return Retorna TRUE se foi possivel realizar as operacoes.
 *
 */
bool carregarDados(char *nomeArquivo, DADOS *dados);

/** \brief Salva dados de um jogo em um arquivo binario.
 *
 * \param *nomeArquivo - String do nome do arquivo binario a ser aberto.
 * \param dados - dados de um jogo que sera salvo.
 * \return Retorna TRUE se foi possivel realizar as operacoes.
 *
 */
bool salvarDados(char *nomeArquivo, DADOS dados);


/**************************************
 * FUNCOES DO MAPA
 **************************************/

/** \brief Carrega mapa de arquivo de texto dado o nivel informado.
 *
 * \param nivel - Inteiro que informa o nivel do mapa a ser carregado.
 * \return Retorna TRUE caso a abertura do arquivo de mapa tenha sido bem-sucedido.
 *
 */
bool carregarMapa(int nivel);

/** \brief Exibe o mapa na tela utilizando as variaveis
de ambiente.
 *
 */
void exibeMapa();


/***************************************
 * FUNCOES DE LOCOMOCAO
 ***************************************/

 /** \brief Funcao que percorre os gatos do vetor de
gatos e os move.
  */
void moveGatos();

/** \brief Funcao que move um gato
dada sua posicao e sua direcao. Caso a movimentacao nao
seja possivel, modifica a direcao do deslocamento para uma
que seja possivel.
 *
 * \param *gato - Ponteiro do tipo STR_GATO
 *
 */
void moveGato(STR_GATO *gato);

/** \brief Move o rato - caso esta movimentacao seja possivel
- dada sua posicao e direcao do movimento.
 *
 * \param *rato - Ponteiro do tipo STR_RATO.
 * \param direcao - Direcao na qual o rato ira se mover
 * \return Retorna TRUE se foi possivel mover o rato.
 *
 */
bool moveRato(STR_RATO *rato, DIRECAO direcao);

/** \brief Retorna se existe parede no mapa na posicao entrada.
 *
 * \param posicao - Posicao onde sera verificada se existe parede.
 * \return TRUE se existe parede na posicao. FALSE se nao existe.
 *
 */
bool existeParede(POSICAO posicao);

/** \brief Retorna se existe queijo no mapa na posicao entrada.
 *
 * \param posicao - Posicao onde sera verificada se existe queijo.
 * \return TRUE se existe queijo na posicao. FALSE se nao existe.
 *
 */
bool existeQueijo(POSICAO posicao);

/** \brief Retorna se existe osso no mapa na posicao entrada.
 *
 * \param posicao - Posicao onde sera verificada se existe osso.
 * \return TRUE se existe osso na posicao. FALSE se nao existe.
 *
 */
bool existeOsso(POSICAO posicao);

/** \brief Retorna se existe porta no mapa na posicao entrada.
 *
 * \param posicao - Posicao onde sera verificada se existe porta.
 * \return TRUE se existe porta na posicao. FALSE se nao existe.
 *
 */
bool existePorta(POSICAO posicao);

/** \brief Retorna se existe gato no mapa na posicao entrada.
 *
 * \param posicao - Posicao onde sera verificada se existe gato.
 * \return TRUE se existe gato na posicao. FALSE se nao existe.
 *
 */
bool existeGato(POSICAO posicao);

/** \brief Retorna se existe rato no mapa na posicao entrada.
 *
 * \param posicao - Posicao onde sera verificada se existe rato.
 * \return TRUE se existe rato na posicao. FALSE se nao existe.
 *
 */
bool existeRato(POSICAO posicao);

/** \brief Funcao que percorre os gatos do vetor de
gatos e os move para a posicao inicial.
 */
void resetaGatos();

/** \brief Move gato para sua posicao inicial do mapa.
 *
 * \param *gato - Ponteiro do tipo STR_GATO que ira ser utilizado
 para mudar a posicao do gato para a posicao inicial.
 */
void resetaGato(STR_GATO *gato);

/** \brief Move rato para sua posicao inicial do mapa.
 *
 * \param *rato - Ponteiro do tipo STR_RATO que ira ser utilizado
 para mudar a posicao do rato para a posicao inicial.
 *
 */
void resetaRato(STR_RATO *rato);

/** \brief Dada a posicao, retorna um ponteiro para a variavel gato que ocupa
esta posicao.
 *
 * \param posicao - Parametro do tipo POSICAO que informa a posicao a ser verificada.
 * \return retorna um ponteiro do tipo STR_GATO para o gato que ocupa a posicao
 *
 */
STR_GATO *retornaGato(POSICAO posicao);

/** \brief Dado um gato e um rato colidindo, realiza as operacoes de
diminuir a vida de um gato, bem como resetar o gato e o rato no
mapa.
 *
 */
void colisaoRatoGato(STR_GATO *gato, STR_RATO *rato);


/**************************************
 * FUNCOES DE MANIPULAÇÃO DE TELA
 **************************************/

/** \brief Funcao cujo objetivo e, dada posicao do objeto nas
estruturas internas do programa, posicionar o cursor na posicao
que este devera aparecer na tela.
 *
 * \param pObj - Posicao do objeto na estrutura interna do programa
 * \param xObjetoTela - Deslocamento do cursor na coordenada x.
 Valores positivos posicionam o objeto mais a direita. Negativos mais a esquerda.
  * \param yObjetoTela - Deslocamento do cursor na coordenada y.
 Valores positivos posicionam o objeto mais abaixo. Negativos mais acima.
 *
 */
void moveCursorMapa(POSICAO pObj, int xObjTela, int yObjTela);

/** \brief Funcao generica que exibe na tela um objeto do mapa dada sua
posicao. Para tanto, essa funcao admite que o objeto sera representado
na tela por quatro caracteres, dois posteiores e dois inferiores.
 *
 * \param backgroundColor - Cor de fundo na qual o objeto sera exibido na tela.
 * \param textColor - Cor do texto que o objeto sera exibido.
 * \param p - Posicao do objeto na estruta do programa.
 * \param x1 - Caractere que ocupara a posicao posterior esquerda da representacao do objeto.
 * \param x2 - Caractere que ocupara a posicao posterior direita da representacao do objeto.
 * \param y1 - Caractere que ocupara a posicao inferior esquerda da representacao do objeto.
 * \param y2 - Caractere que ocupara a posicao inferior direita da representacao do objeto.
 *
 */
void exibeTela(int backgroundColor, int textColor, POSICAO p, char x1, char x2, char y1, char y2);

/** \brief Dada a posicao, exibe o objeto que a ocupa.
 *
 * \param posicao - Posicao do objeto na estrutura do programa.
 *
 */
void exibeObjeto(POSICAO posicao);

/** \brief Funcao que percorre os gatos do vetor de gatos e os exibe.
 */
void exibeGatos();

/** \brief Exibe gato na tela, dada sua posicao.
 *
 * \param gato - Parametro do tipo STR_GATO que sera exibido na tela.
 *
 */
void exibeGato(STR_GATO gato);

/** \brief Exibe uma porta na tela, dada sua posicao.
 *
 * \param posicao - Posicao da porta na estrutura do programa.
 */
void exibePortaTela(POSICAO p);

/** \brief Exibe menu superior, pausando o andamento do jogo.
 */
void exibeMenu();

/** \brief Remove menu superior da tela, dando continuidade ao jogo.
 */
void escondeMenu();

/** \brief Exibe menu inicial do jogo.
 */
void exibeMenuPrincipal();

/** \brief Exibe informacoes de pontuacao, vida e teclas de atalho na tela.
 */
void exibePontuacao();

/** \brief Exibe um rato na tela, dada sua posicao.
 *
 * \param rato - Parametro do tipo STR_RATO que sera exibido na tela.
 *
 */
void exibeRatoTela(STR_RATO rato);

/** \brief Executa rotina de movimentacao de rato na tela.
Assim, pode-se excluir o que havia na tela na posicao anterior
e modificar a tela para exibir o rato na nova posicao.
 *
 * \param rato - Parametro do tipo STR_RATO que informara as informacoes
 do rato para realizar a sua movimentacao na tela.
 * \param pAnterior - Parametro do tipo POSICAO que informara a posicao
 que o rato ocupava anteriormente ao seu deslocamento.
 *
*/
void moveRatoTela(STR_RATO rato, POSICAO pAnterior);

/** \brief Executa rotina de movimentacao de gato na tela.
Assim, pode-se repor na tela o que havia na tela na posicao
 anterior e modificar a tela para exibir o gato na nova posicao.
 *
 * \param rato - Parametro do tipo STR_GATO que informara as informacoes
 do gato para realizar a sua movimentacao na tela.
 * \param pAnterior - Parametro do tipo POSICAO que informara a posicao
 que o gato ocupava anteriormente ao seu deslocamento.
 *
 */
void moveGatoTela(STR_GATO gato, POSICAO pAnterior);

/** \brief Assim que modificada a posicao das portas,
atualiza a posicao dessas na tela.
 */
void atualizaPortasTela();

/** \brief Funcao generica para remover elemento da tela, dada sua posicao.
 *
 * \param p - Parametro do tipo POSICAO, que informa a posicao do objeto
 que sera removido da tela.
 *
 */
void excluiTela(POSICAO p);


/**************************************
 * FUNCOES AUXILIARES
 **************************************/

/** \brief Altera-se a direcao
de modo que esta esteja no intervalo [-1, 1].
 *
 * \param *direcao - Ponteiro do tipo DIRECAO que sera
 modificado para assumir valores em x e y de no intervalo
 de -1 e 1.
 * \param x - direcao que sera assumida no eixo x.
 * \param y - direcao que sera assumida no eixo y.
 *
 */
void setDirecao(DIRECAO *direcao, int x, int y);

/** \brief Retorna uma direcao aleatoria
entre -1 e 1.
 *
 * \return Retorna valor inteiro aleatorio entre -1 e 1.
 *
 */
int retornaDirecaoAleatoria();

/** \brief Retorna se todos os queijos do mapa foram comidos.
 *
 * \return TRUE se todos os queijos presentes no mapa foram comidos.
 Caso contrario, retorna FALSE.
 *
 */
bool foramTodosQuiejosComidos();


int main()
{
    srand(time(NULL));
    _setcursortype(_NOCURSOR);
    /* Definicao de tamanho da tela do jogo */
    system("mode con:cols=110 lines=30");

    /* Variaveis locais para controle de entrada do teclado */
    int isKeyPressed;
    int keypressed;

    /* pega dados da tela */
    struct text_info tela;
    gettextinfo(&tela);

    /* salva altura da tela em caracteres */
    alturaTela = (int) tela.screenheight;
    /* salva largura da tela em caracteres */
    larguraTela = (int) tela.screenwidth;

    /* armazena a altura do jogo em caracteres */
    int alturaJogo = LIMITE_Y * DIMENSAO_X_TELA;
    /* armazena a largura do jogo em caracteres */
    int larguraJogo = LIMITE_X * DIMENSAO_X_TELA;

    /*
        Essa parte utiliza as informacoes de largura e altura para centralizar
        o jogo na tela
        Assim:

        <-----larguraTela/2---->
        ------------------------|------------------------|  -
        |         <-largura/2-->|                | y     |  |
        |<---x-->|--------------|--------------| -       |  |
        |        |xxxxxxxxxxxxxx|xxxxxxxxxxxxxx| |       |  |
        |        |xxxxxxxxxxxxxx|xxxxxxxxxxxxxx| altura/2|  |
        |        |xxxxxxxxxxxxxx|xxxxxxxxxxxxxx| |       |  |
        |--------|--------------|--------------|---------| alturaJogo/2
        |        |xxxxxxxxxxxxxx|xxxxxxxxxxxxxx|         |  |
        |        |xxxxxxxxxxxxxx|xxxxxxxxxxxxxx|         |  |
        |        |xxxxxxxxxxxxxx|xxxxxxxxxxxxxx|         |  |
        |        |--------------|--------------|         |  |
        |                       |                        |  |
        |-----------------------|------------------------|  -

        Portanto, sabe-se que, para centralizar o mapa, terá que diminuir
        a metade da largura da tela pela metade da largura do jogo e
        utilizar esse valor como referência para deslocar o jogo para
        a direita. O mesmo vale para a altura, que com a diferença da
        metade do valor da altura da tela pela metado do valor da altura jogo,
        desloca-se o mapa para abaixo utiizando este valor de referência

    */
    xInicialMapa = (round(larguraTela/2) - round(larguraJogo/2)); //valor referência para deslocamento na horizontal
    yInicialMapa = (round(alturaTela/2) - round(alturaJogo/2));   //valor referência para deslocamento na vertical

    exibeMenuPrincipal();
    PlaySound(TEXT("raff.wav"), NULL, SND_ASYNC);
    while ( (keypressed != KEY_N) && (keypressed != KEY_C) && (keypressed != KEY_Q) ){
        keypressed = toupper(getch());
        switch (keypressed){
            case KEY_N:
                novoJogo();
                PlaySound(NULL, 0, 0);
                break;
            case KEY_C:
                PlaySound(NULL, 0, 0);
                insline();
                char nomeArquivo[20];
                DADOS dados;
                cprintf("\nNome: ");
                gets(nomeArquivo);
                if (carregarDados(nomeArquivo, &dados)){
                    carregaJogo(dados);
                } else {
                    printf("Achou arquivo errado, otario!!");
                    system("exit");
                    return 0;
                }
                break;
            case KEY_Q:
                PlaySound(NULL, 0, 0);
                system("exit");
                return 0;
        }
    }

    int contadorDelayGatos = 0;
    int contadorTempoModoCachorro = 0;

    while((nivel <= NIVEL_MAX) && (lilmouse.vidas > 0)) {
        if (foramTodosQuiejosComidos(mapa)){
            mudaNivel();
        }

        if (lilmouse.modo == MODO_CACHORRO){
            if (contadorTempoModoCachorro == TEMPO_MODO_CACHORRO/WAIT_TIME){
                lilmouse.modo = MODO_RATO;
                contadorTempoModoCachorro = 0;
                exibeRatoTela(lilmouse);
            } else{
                contadorTempoModoCachorro++;
            }
        }

        /* Conta o tempo de movimentacao dos gatos */
        if (contadorDelayGatos > (TEMPO_MOVIMENTO_GATOS/WAIT_TIME)){
            moveGatos();
            exibeRatoTela(lilmouse);
            contadorDelayGatos = 0;
        } else{
            contadorDelayGatos++;
        }

        isKeyPressed = kbhit();
        if (isKeyPressed){
            keypressed = toupper(getch());
            if (keypressed == KEY_TAB){
                exibeMenu();
                keypressed = toupper(getch());
                char nomeArquivo[20];
                DADOS dados;
                switch (keypressed){
                case KEY_V:
                    escondeMenu();
                    break;
                case KEY_N:
                    novoJogo();
                    break;
                case KEY_S:
                    cprintf("\nNome: ");
                    gets(nomeArquivo);
                    getDados(&dados);
                    escondeMenu();
                    salvarDados(nomeArquivo, dados);
                    break;
                case KEY_C:
                    cprintf("\nNome: ");
                    gets(nomeArquivo);
                    carregarDados(nomeArquivo, &dados);
                    carregaJogo(dados);
                    break;
                case KEY_Q:
                    system("exit");
                    return 0;
                    break;
                default:
                    escondeMenu();
                    break;
                }
            } else if (keypressed == KEY_B){
                modoPortas = !modoPortas;
                atualizaPortasTela();
            } else if ((keypressed == CTE_SETAS) || (keypressed == 0)){
                keypressed = getch();
                DIRECAO direcaoRato;

                switch (keypressed){
                case SETA_BAIXO:
                    direcaoRato.x = PARADO;
                    direcaoRato.y = BAIXO;
                    break;
                case SETA_ESQUERDA:
                    direcaoRato.x = ESQUERDA;
                    direcaoRato.y = PARADO;
                    break;
                case SETA_DIREITA:
                    direcaoRato.x = DIREITA;
                    direcaoRato.y = PARADO;
                    break;
                case SETA_CIMA:
                    direcaoRato.x = PARADO;
                    direcaoRato.y = CIMA;
                    break;
                }
                moveRato(&lilmouse, direcaoRato);
            } else {
                DIRECAO direcaoRato;
                switch (keypressed){
                case KEY_S:
                    direcaoRato.x = PARADO;
                    direcaoRato.y = BAIXO;
                    break;
                case KEY_A:
                    direcaoRato.x = ESQUERDA;
                    direcaoRato.y = PARADO;
                    break;
                case KEY_D:
                    direcaoRato.x = DIREITA;
                    direcaoRato.y = PARADO;
                    break;
                case KEY_W:
                    direcaoRato.x = PARADO;
                    direcaoRato.y = CIMA;
                    break;
                }
                moveRato(&lilmouse, direcaoRato);
            }
        }

        Sleep(WAIT_TIME);
        isKeyPressed = kbhit();
    }

    if (lilmouse.vidas == 0){
        perdeu();
    }

    system("exit");
    return 0;
}


/* FUNÇÕES DO JOGO */
void novoJogo(){
    /*  Inicializa as variaveis basicas do jogo  */
    contagemPontos = 0;
    nivel = 1;
    lilmouse.vidas = 3;
    modoPortas = MODO_PORTA_INICIAL;

    clrscr();
    carregarMapa(nivel);
    exibeMapa();
}

void mudaNivel(){
    nivel++;
    modoPortas = MODO_PORTA_INICIAL;

    textbackground(BLACK);
    clrscr();
    if (nivel == (NIVEL_MAX+1)){
        ganhou();
    } else{
        carregarMapa(nivel);
        exibeMapa();
        exibePontuacao();
    }
}

void ganhou(){
    clrscr();
    textbackground(BLACK);
    textcolor(WHITE);

    PlaySound(TEXT("raff.wav"), NULL, SND_ASYNC);

    printf(" ____  _____   ____   ____   ____   ____  _ _ _ \n");
    printf("|  _ \\|  __ \\ / __ \\ / __ \\ / __ \\ / __ \\| | | |\n");
    printf("| |_) | |__) | |  | | |  | | |  | | |  | | | | |\n");
    printf("|  _ <|  _  /| |  | | |  | | |  | | |  | | | | |\n");
    printf("| |_) | | \\ \\| |__| | |__| | |__| | |__| |_|_|_|\n");
    printf("|____/|_|  \\_\\\\____/ \\____/ \\____/ \\____/(_|_|_)\n");

    printf(" _______ _____            _____  _____  ______ _____    __  __ ______ _____  __  __  ____  \n");
    printf("|__   __|  __ \\     /\\   |  __ \\|  __ \\|  ____|  __ \\  |  \\/  |  ____|  __ \\|  \\/  |/ __ \\ \n");
    printf("   | |  | |__) |   /  \\  | |__) | |__) | |__  | |__) | | \\  / | |__  | |__) | \\  / | |  | | \n");
    printf("   | |  |  _  /   / /\\ \\ |  ___/|  ___/|  __| |  _  /  | |\\/| |  __| |  _  /| |\\/| | |  | | \n");
    printf("   | |  | | \\ \\  / ____ \\| |    | |    | |____| | \\ \\  | |  | | |____| | \\ \\| |  | | |__| | \n");
    printf("   |_|  |_|  \\_\\/_/    \\_\\_|    |_|    |______|_|  \\_\\ |_|  |_|______|_|  \\_\\_|  |_|\\____/ \n");

    printf("  _____ _  _______    ______ ______ ______ \n");
    printf(" / ____| |/ /  __ \\  |____  |____  |____  | \n");
    printf("| (___ | ' /| |__) |     / /    / /    / / \n");
    printf(" \\___ \\|  < |  _  /     / /    / /    / /  \n");
    printf(" ____) | . \\| | \\ \\    / /    / /    / /   \n");
    printf("|_____/|_|\\_\\_|  \\_\\  /_/    /_/    /_/   \n");

    getch();
}

void perdeu(){
    textbackground(BLACK);
    textcolor(WHITE);
    clrscr();

    PlaySound(TEXT("raff.wav"), NULL, SND_ASYNC);

    printf(" _____             _             \n");
    printf("|  __ \\           | |            \n");
    printf("| |__) |__ _ __ __| | ___ _   _  \n");
    printf("|  ___/ _ \\ '__/ _` |/ _ \\ | | | \n");
    printf("| |  |  __/ | | (_| |  __/ |_| |_ \n");
    printf("|_|   \\___|_|  \\__,_|\\___|\\__,_( ) \n");
    printf("                               |/ \n");

    printf(" ____  _____   ____   ____   ____   ____  _ _ _ \n");
    printf("|  _ \\|  __ \\ / __ \\ / __ \\ / __ \\ / __ \\| | | |\n");
    printf("| |_) | |__) | |  | | |  | | |  | | |  | | | | |\n");
    printf("|  _ <|  _  /| |  | | |  | | |  | | |  | | | | |\n");
    printf("| |_) | | \\ \\| |__| | |__| | |__| | |__| |_|_|_|\n");
    printf("|____/|_|  \\_\\\\____/ \\____/ \\____/ \\____/(_|_|_)\n");

    getch();
}


/* FUNÇÕES QUE SALVAM E CARREGAM O ESTADO DO JOGO EM AQUIVO
DE TEXTO*/
void carregaJogo(DADOS dados){
    memset(mapa, 0, sizeof(mapa));
    memset(gatos, 0, sizeof(gatos));

    contagemPontos = dados.contagemPontos;
    nivel = dados.nivel;
    memcpy(gatos, dados.gatos, sizeof(dados.gatos));
    lilmouse = dados.rato;
    memcpy(mapa, dados.mapa, sizeof(dados.mapa));
    modoPortas = dados.modoPortas;

    clrscr();
    exibeMapa();
}

void getDados(DADOS *dados){
    dados->contagemPontos = contagemPontos;
    dados->nivel = nivel;
    memcpy(dados->gatos, gatos, sizeof(gatos));
    dados->rato = lilmouse;
    memcpy(dados->mapa, mapa, sizeof(mapa));
    dados->modoPortas = modoPortas;
}

bool carregarDados(char *nomeArquivo, DADOS *dados){
    char nomeArquivoFinal[50];
    sprintf(nomeArquivoFinal, "saves/%s.bin", nomeArquivo);

    FILE *arquivo;
    arquivo = fopen(nomeArquivoFinal, "rb");

    if (arquivo){
        int resultado = fread(dados, sizeof(DADOS), 1, arquivo);
        fclose(arquivo);
        return resultado == 1;

    } else {
        return FALSE;
    }
}

bool salvarDados(char *nomeArquivo, DADOS dados){
    char nomeArquivoFinal[50];
    sprintf(nomeArquivoFinal, "saves/%s.bin", nomeArquivo);

    FILE *arquivo;
    arquivo = fopen(nomeArquivoFinal, "wb");

    if (arquivo){
        int resultado = fwrite(&dados, sizeof(DADOS), 1, arquivo);
        fclose(arquivo);
        return resultado == 1;

    } else {
        return FALSE;
    }
}


/* FUNÇÕES DE MAPA */
bool carregarMapa(int nivel){
    int idGato = 0;

    /* Carrega nome do arquivo de mapa conforme o nivel */
    char nomeArquivo[12];
    sprintf(nomeArquivo, "mapas/nivel%02d.txt", nivel);

    /* Carrega o nome do arquivo */
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL){
        printf("Ocorreu um erro ao inicializar o mapa do programa");
        return FALSE;
    } else {
        /* limpa mapa da memória*/
        memset(mapa, 0, sizeof(mapa));
        /* preenche o mapa e os objetos da memória */
        for (int j = 0; j < LIMITE_Y; j++){
            for (int i = 0; i < (LIMITE_X+1); i++){
                char caractere = (char) fgetc(arquivo);
                switch (caractere) {
                    case PAREDE:
                        mapa[i][j].parede = TRUE;
                        break;
                    case QUEIJO:
                        mapa[i][j].queijo = TRUE;
                        break;
                    case OSSO:
                        mapa[i][j].osso = TRUE;
                        break;
                    case GATO:
                        gatos[idGato].posicaoInicial.x = i;
                        gatos[idGato].posicaoInicial.y = j;
                        gatos[idGato].posicaoAtual.x = i;
                        gatos[idGato].posicaoAtual.y = j;
                        setDirecao(&(gatos[idGato].direcao),
                                   retornaDirecaoAleatoria(),
                                   retornaDirecaoAleatoria());
                        idGato++;
                        break;
                    case RATO:
                        lilmouse.posicaoInicial.x = i;
                        lilmouse.posicaoInicial.y = j;
                        lilmouse.posicaoAtual.x = i;
                        lilmouse.posicaoAtual.y = j;
                        break;
                    case PORTA:
                        mapa[i][j].portaInicial = TRUE;
                        mapa[i+DIREITA][j+BAIXO].portaInvisivel = TRUE;
                        break;
                }
            }
        }
        fclose(arquivo);
        return TRUE;
    }
}

void exibeMapa(){
    exibicaoMenu = FALSE;
    for (int j = 0; j < LIMITE_Y; j++){
        for (int i = 0; i < LIMITE_X; i++){
            POSICAO posicaoAtual = {i, j};
            exibeObjeto(posicaoAtual);
        }
        printf("\n");
    }
    exibeGatos();
    exibePontuacao();
    exibeRatoTela(lilmouse);
}


/* FUNÇÕES DE LOCOMOÇÃO */
void moveGatos(){
    for (int i = 0; i < NUM_GATOS; i++){
        moveGato(&(gatos[i]));
    }
}

void moveGato(STR_GATO *gato){
    POSICAO pInicial = gato->posicaoAtual;

    POSICAO nPosicao;
    nPosicao.x = pInicial.x + gato->direcao.x;
    nPosicao.y = pInicial.y + gato->direcao.y;


    while ( ((gato->direcao.x == PARADO) && (gato->direcao.y == PARADO)) ||
        existeParede(nPosicao) ||
        existePorta(nPosicao) ){
            setDirecao(&(gato->direcao),
                        retornaDirecaoAleatoria(),
                        retornaDirecaoAleatoria());

            nPosicao.x = pInicial.x + gato->direcao.x;
            nPosicao.y = pInicial.y + gato->direcao.y;
    }

    if (existeRato(nPosicao)){
        colisaoRatoGato(gato, &lilmouse);
    } else {
        gato->posicaoAtual = nPosicao;
        moveGatoTela(*gato, pInicial);
    }
}

bool moveRato(STR_RATO *rato, DIRECAO direcao){
    POSICAO pInicial = rato->posicaoAtual;

    POSICAO nPosicao;
    nPosicao.x = pInicial.x + direcao.x;
    nPosicao.y = pInicial.y + direcao.y;

    if ((existeParede(nPosicao)) || (existePorta(nPosicao)) ){
         return FALSE;
     } else {
        rato->posicaoAtual = nPosicao;
        if (existeGato(nPosicao)){
            STR_GATO *gato = retornaGato(nPosicao);
            colisaoRatoGato(gato, rato);        }
        if (existeOsso(nPosicao)){
            /* COLISÃO DO RATO COM OSSO */
            lilmouse.modo = MODO_CACHORRO;
            mapa[nPosicao.x][nPosicao.y].osso = FALSE;
            excluiTela(nPosicao);
            Beep(500, 250);
        } else if (existeQueijo(nPosicao)){
            /* COLISÃO DO RATO COM QUEIJO */
            contagemPontos += PESO_PONTO;
            mapa[nPosicao.x][nPosicao.y].queijo = FALSE;
            excluiTela(nPosicao);
            exibePontuacao();
        }
        moveRatoTela(*rato, pInicial);
        return TRUE;
     }
}

bool existeParede(POSICAO posicao){
    return mapa[posicao.x][posicao.y].parede;
}

bool existeQueijo(POSICAO posicao){
    return mapa[posicao.x][posicao.y].queijo;
}

bool existeOsso(POSICAO posicao){
    return mapa[posicao.x][posicao.y].osso;
}

bool existePorta(POSICAO posicao){
    if (modoPortas == MODO_PORTA_INICIAL){
        return mapa[posicao.x][posicao.y].portaInicial;
    } else {
        return mapa[posicao.x][posicao.y].portaInvisivel;
    }

}

bool existeGato(POSICAO posicao){
    bool aux = FALSE;
    for (int i = 0; i < NUM_GATOS; i++){
        POSICAO pGato = gatos[i].posicaoAtual;
        if ((pGato.x == posicao.x) && (pGato.y == posicao.y)){
            aux = TRUE;
        }
    }
    return aux;
}

bool existeRato(POSICAO posicao){
    if ((lilmouse.posicaoAtual.x == posicao.x)
        && (lilmouse.posicaoAtual.y == posicao.y)){
        return TRUE;
    } else {
        return FALSE;
    }
}

void resetaGatos(){
    for (int i = 0; i < NUM_GATOS; i++){
        resetaGato(&gatos[i]);
    }
}

void resetaGato(STR_GATO *gato){
    excluiTela(gato->posicaoAtual);
    gato->posicaoAtual = gato->posicaoInicial;
    exibeGato(*gato);
}

void resetaRato(STR_RATO *rato){
    excluiTela(rato->posicaoAtual);
    rato->posicaoAtual = rato->posicaoInicial;
    exibeRatoTela(*rato);
}

STR_GATO *retornaGato(POSICAO posicao){
    for (int i = 0; i < NUM_GATOS; i++){
        POSICAO pGato = gatos[i].posicaoAtual;
        if ((pGato.x == posicao.x) && (pGato.y == posicao.y)){
            return &gatos[i];
        }
    }

    return NULL;
}

void colisaoRatoGato(STR_GATO *gato, STR_RATO *rato){
    if (rato->modo == MODO_RATO){
        (rato->vidas)--;
        resetaGatos();
        resetaRato(rato);
        exibePontuacao();
        Beep(900, 300);
    } else if (rato->modo == MODO_CACHORRO){
        contagemPontos += PESO_PONTO_CACHORRO;
        resetaGato(gato);
        exibePontuacao();
    }
}


/* FUNÇÕES DE MANIPULAÇÃO DA TELA*/
void moveCursorMapa(POSICAO pObj, int xObjTela, int yObjTela){
    /* soma a posição no qual o mapa inicia na horizontal
    com as posicões dos objetos anteriores - dado pela posição deles
    no mapa mulplicado pela dimensão padrão dos objetos em x -
    e com a posicao de deslocamento nos caracteres de determinacao
    do objeto
      */
    int xTela = xInicialMapa + (DIMENSAO_X_TELA * pObj.x) + (1+xObjTela);
    /* Faz o mesmo em vertical */
    int yTela = yInicialMapa + (DIMENSAO_Y_TELA * pObj.y) + (1+yObjTela);
    /*põe o cursor nessa posicao */
    gotoxy(xTela, yTela);
}

void exibeTela(int backgroundColor, int textColor, POSICAO p, char x1, char x2, char y1, char y2){
    textbackground(backgroundColor);
    textcolor(textColor);
    moveCursorMapa(p, 0, 0);
    putch(x1);

    moveCursorMapa(p, 1, 0);
    putch(x2);

    moveCursorMapa(p, 0, 1);
    putch(y1);

    moveCursorMapa(p, 1, 1);
    putch(y2);
}

void exibeObjeto(POSICAO posicao){
    if (existeParede(posicao)){
        exibeTela(GREEN, WHITE, posicao, ' ', ' ', ' ', ' ');
    } else if (existePorta(posicao)){
        exibePortaTela(posicao);
    } else if(existeQueijo(posicao)){
        exibeTela(BLACK, YELLOW, posicao, ' ', ' ', ' ', '.');
    } else if (existeOsso(posicao)){
        exibeTela(BLACK, WHITE, posicao, '\\', '/', '/', '\\');
    } else {
        exibeTela(BLACK, WHITE, posicao, ' ', ' ', ' ', ' ');
    }
}

void exibePortaTela(POSICAO p){
    exibeTela(LIGHTGRAY, WHITE, p, ' ', ' ', ' ', ' ');
}

void exibeRatoTela(STR_RATO rato){
    if (rato.modo == MODO_RATO){
        exibeTela(DARKGRAY, WHITE, rato.posicaoAtual, '*', '*', '-', '-');
    } else {
        exibeTela(BROWN, WHITE, rato.posicaoAtual, '*', '*', '-', '-');
    }
}

void exibeGatos(){
    for (int i = 0; i < NUM_GATOS; i++){
        exibeGato(gatos[i]);
    }
}

void exibeGato(STR_GATO gato){
    POSICAO pGato = gato.posicaoAtual;
    exibeTela(WHITE, BLACK, pGato, '*', '*', '-', '-');
}

void moveRatoTela(STR_RATO rato, POSICAO pAnterior) {
    exibeObjeto(pAnterior);
    exibeRatoTela(rato);
}

void moveGatoTela(STR_GATO gato, POSICAO pAnterior){
    exibeObjeto(pAnterior);
    exibeGato(gato);
}

void exibeMenu(){
    if (exibicaoMenu == FALSE){
        exibicaoMenu = TRUE;
        textbackground(BLACK);
        textcolor(WHITE);
        gotoxy(1, 1);
        for (int i = 0; i < 4; i++){
            insline();
        }

        char menu[] = "(N) Novo  (C) Carregar  (S) Salvar  (Q) Sair  (V) Voltar";
        int larguraMenu = strlen(menu);
        int xPont = round(larguraTela/2) - round(larguraMenu/2);
        gotoxy(xPont, 2);
        cprintf("Menu:\n");
        gotoxy(xPont, 3);
        cprintf(menu);
    }
}

void escondeMenu(){
    exibicaoMenu = FALSE;
    gotoxy(1, 1);
    for (int i = 0; i < 4; i++){
        delline();
    }
    exibeMapa();
}

void exibeMenuPrincipal(){
    clrscr();

    int tamanhoX = 0;

    char *x[6] = {" _       ___   _         __  __    ___    _   _   ____    _____  ",
            "| |     |_ _| | |       |  \\/  |  / _ \\  | | | | / ___|  | ____| ",
            "| |      | |  | |       | |\\/| | | | | | | | | | \\___ \\  |  _|   ",
            "| |___   | |  | |___    | |  | | | |_| | | |_| |  ___) | | |___  ",
            "|_____| |___| |_____|   |_|  |_|  \\___/   \\___/  |____/  |_____| ",
            "THE MOUSE TRAPPER"};


    for (int i = 0; i < 6; i++){
        if (strlen(x[i]) > tamanhoX){
            tamanhoX = strlen(x[i]);
        }
    }

    int deslocamento = round(larguraTela/2) - round(tamanhoX/2);

    for (int i = 0; i < 6; i++){
        gotoxy(deslocamento, i+2);
        cprintf("%s", x[i]);
    }

    gotoxy(deslocamento, 10);
    cprintf("(N) Novo jogo");
    gotoxy(deslocamento, 11);
    cprintf("(C) Carregar jogo");
    gotoxy(deslocamento, 12);
//    cprintf("(L) Lista maiores pontuacoes");
    cprintf("(Q) Sair");

}

void exibePontuacao(){
    struct text_info tela;
    gettextinfo(&tela);
    int largura = (int) tela.screenwidth;

    char texto[100];
    sprintf(texto, "Pontuacao: %d  Vidas: %d  Nivel: %d  [TAB]: Menu  B/b: Portas", contagemPontos, lilmouse.vidas, nivel);
    int larguraTexto = strlen(texto);

    int xPont = round(largura/2) - round(larguraTexto/2);
    int yPont = yInicialMapa + 2*LIMITE_Y + 2;

    textbackground(BLACK);
    textcolor(WHITE);
    gotoxy(xPont, yPont);
    cprintf(texto);
}

void atualizaPortasTela(){
    for (int j = 0; j < LIMITE_Y; j++){
        for (int i = 0; i < LIMITE_X; i++){
            POSICAO p = {i, j};
            if (modoPortas == MODO_PORTA_INICIAL){
                if (mapa[i][j].portaInicial){
                    exibePortaTela(p);
                }
                if (mapa[i][j].portaInvisivel){
                    excluiTela(p);
                }
            } else if (modoPortas == MODO_PORTA_INVISIVEL){
                if (mapa[i][j].portaInvisivel){
                    exibePortaTela(p);
                }
                if (mapa[i][j].portaInicial){
                    excluiTela(p);
                }
            }
        }
    }
}

void excluiTela(POSICAO p){
    exibeTela(BLACK, WHITE, p, ' ', ' ', ' ', ' ');
}


/* FUNÇÕES AUXILIARES */
void setDirecao(DIRECAO *direcao, int x, int y){
    if ( (x >= -1) && (x <= 1) ){
        direcao->x = x;
    } else {
        direcao->x = 0;
    }
    if ( (y >= -1) && (y <= 1) ){
        direcao->y = y;
    } else {
        direcao->y = 0;
    }
}

int retornaDirecaoAleatoria(){
    return (rand()%3) - 1;
}

bool foramTodosQuiejosComidos(MAPA mapaL[LIMITE_X][LIMITE_Y]){
    /* Retorna se o número de queijos do mapa é zero */

    int numQueijos = 0;
    for (int j = 0; j < LIMITE_Y; j++){
        for (int i = 0; i < LIMITE_X; i++){
            if (mapaL[i][j].queijo){
                numQueijos++;
            }
        }
    }

    return numQueijos == 0;
}

