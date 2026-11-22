# ProjetoJogoSimao-2026.2

Jogo de plataforma 2D em C++ e SFML, baseado nos assets Pixel Adventure 1. A
Fase 1 e um percurso lateral de grama e terra com o Ninja Frog, pulo duplo,
shurikens, caixas quebraveis, frutas, armadilhas, checkpoint e chegada.

## Controles

- Menu inicial: clique em Jogar ou pressione Enter; escolha uma das duas fases
  com clique, 1 ou 2. Esc retorna ao menu inicial.
- A/D ou setas: mover
- Espaco ou W: pular; aperte novamente no ar para o pulo duplo
- F ou clique esquerdo: atirar shuriken
- Esc: pausar ou continuar
- R: reiniciar a fase
- Enter: confirmar ou voltar a jogar

## Arquitetura

Os arquivos de `headers/` e `src/` estao organizados por entidades, fases,
gerenciadores e listas. `Controle::Jogo` coordena a execucao; `Fases::Fase1`
deriva da classe base `Fases::Fase`; e o jogador segue a hierarquia
`Entidades::Ente` -> `Entidades::Entidade` -> `Entidades::Personagem` ->
`Entidades::Personagens::Jogador`. A fase utiliza os gerenciadores grafico e de
colisoes, alem de uma lista de entidades cujos nos nao sao proprietarios dos
objetos apontados.

O jogo inicia em um menu com acesso a uma tela de selecao entre duas fases.
A Fase 2 usa uma paleta noturna; ambas reutilizam o percurso e a jogabilidade
atuais.

## Build SFML

O codigo fica em `src/`, com headers em `headers/`. Instale o Visual Studio 2022
Build Tools com a carga de trabalho de C++ para Windows. `Ctrl+Shift+B` compila
e F5 compila e inicia a depuracao; a tarefa localiza o MSVC e configura o
ambiente automaticamente. Ela usa os headers em `third_party/SFML/include`, as
bibliotecas de importacao em `sfml/bin` e as DLLs em `third_party/SFML`.

O executavel e os simbolos de depuracao ficam em
`%TEMP%/ProjetoJogoSimaoBuild`, fora do OneDrive para evitar bloqueios dos
arquivos intermediarios. O jogo e iniciado com a raiz do repositorio como
diretorio de trabalho para localizar os assets.

## Assets

Os assets do Pixel Adventure 1 estao em `assets/Pixel Adventure 1/Free/` e sao
distribuidos sob a licenca CC0. A imagem `20 Enemies.png` e somente uma previa
do pacote de inimigos; a fase usa as caixas e armadilhas disponibilizadas no
pacote gratuito como alvos e perigos.
