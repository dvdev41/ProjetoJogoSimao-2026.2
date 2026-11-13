# ProjetoJogoSimao-2026.2

## SFML e build

O projeto usa SFML 2.6.2 para MSVC x64. Os headers estao em `sfml/include`; as DLLs e bibliotecas dinamicas `.lib` usadas pela tarefa Debug estao em `sfml/bin`. A licenca da SFML esta em `sfml/license.md`.

Abra o VS Code a partir do Developer PowerShell x64 do Visual Studio 2022 para que `cl.exe` esteja disponivel e execute a tarefa padrao com `Ctrl+Shift+B`. A configuracao de Debug usa as bibliotecas `-d.lib` e carrega DLLs de `sfml/bin`.

Para habilitar audio com SFML, copie tambem `openal32.dll` do pacote SFML para `sfml/bin`; esse arquivo ainda nao esta presente.
