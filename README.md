# ProjetoJogoSimao-2026.2

## SFML e build

O codigo do jogo fica em `src/`, com headers em `headers/` organizados por entidade e gerenciador. Os assets ficam em `assets/`. A copia da SFML 2.6.2 esta separada em `third_party/SFML/`, com headers, DLLs e licenca.

Abra o VS Code a partir do Developer PowerShell x64 do Visual Studio 2022 para que `cl.exe` esteja disponivel e execute a tarefa padrao com `Ctrl+Shift+B`. A tarefa Debug requer as bibliotecas SFML `-d.lib` instaladas em `C:\SFML-2.6.2\lib`; as DLLs locais sao carregadas de `third_party/SFML`.

Os arquivos gerados pela compilacao ficam em `build e obj/`.

## Assets

Os 175 arquivos PNG de Pixel Adventure 1 estao em `assets/Pixel Adventure 1/Free/`.
O jogo carrega as animacoes do Ninja Frog e a cabeca de pedra diretamente desse
caminho relativo ao diretorio raiz do projeto. Os assets Pixel Adventure 1 sao
distribuidos sob a licenca CC0, conforme a pagina do autor no itch.io.

Execute o jogo a partir da raiz do repositorio para que os caminhos dos assets
sejam encontrados. O pacote Pixel Adventure 1 nao inclui as spritesheets dos
inimigos; a imagem de 20 inimigos e apenas uma previa de Pixel Adventure 2.
