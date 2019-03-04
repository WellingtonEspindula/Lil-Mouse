# Lil-Mouse
Projeto final da disciplina de Algoritmos e Programação (UFRGS 2018/2) cujo objetivo é desenvolver um jogo baseado no clássico Mouse Trap do Atari.

## Autores:
Wellington Espindula e Rafael Trevisan

## Descrição: 
Nesse jogo, o usuário controla um rato, que move-se a partir da entrada do usuário nas setas - ou nas teclas W, A, S e D -, e deve coletar
todo o queijo de um labirinto. Existem também 4 gatos, que andam em direções aleatórias e tentarão “comer” o gato - isto é, farão o rato perder uma vida caso entrem em contato com ele.
Entretanto, existem ossos no labirinto, que quando coletados pelo rato, o transforma temporariamente em um cachorro; assim, o rato em modo cachorro pode derrotar os gatos e mandá-los para as suas posições iniciais. Existem também portas móveis nesse labirinto que mudam de posição quando o jogador clica “B/b”, mudando a configuração do mapa.
Quando o jogador coleta todos os queijos de uma fase, ele passa para a próxima, e esse processo repete-se até que todas as fases acabem.
Cada queijo coletado dá ao jogador 10 pontos, e cada gato derrotado da 50. O objetivo do jogo é conseguir a maior pontuação possível antes de perder as suas 5 vidas, ou mesmo conseguir passar todas as fases enquanto houverem vidas.

## Para começar:
Antes de começar, esse jogo foi desenvolvido em C com bibliotecas que só irão funcionar em Windows. Sabendo isso, certifique-se de instalar a Conio2 no seu computador e na sua IDE de preferência. Durante o desenvolvimento, foi utilizado o CodeBlocks. Além disso, é necessário checar se a libwinmm do MinGW está incluída nas configurações de compilação - biblioteca que fará possível rodar a música no início e fim do jogo.

## Documentação:
A documentação do código encontra-se disponível no endereço: https://wellingtonespindula.github.io/Lil-Mouse/.
Se essa não for suficiente, baixe o Relatório.pdf - disponível no diretório principal desse repositório -, onde encontrará mais informações a respeito da documentação do software.
