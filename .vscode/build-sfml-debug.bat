@echo off
setlocal

set "WORKSPACE=%~dp0.."
for %%I in ("%WORKSPACE%") do set "WORKSPACE=%%~fI"

set "BUILD_DIR=%TEMP%\ProjetoJogoSimaoBuild"
if not exist "%BUILD_DIR%\." mkdir "%BUILD_DIR%"
if not exist "%BUILD_DIR%\." (
    echo Nao foi possivel criar a pasta de build: "%BUILD_DIR%"
    exit /b 1
)

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo Visual Studio Installer nao encontrado: "%VSWHERE%"
    exit /b 1
)

for /f "usebackq tokens=*" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%I"
if not defined VSINSTALL (
    echo Ferramentas C++ x64 do Visual Studio nao encontradas.
    exit /b 1
)

set "VCVARS=%VSINSTALL%\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" (
    echo Arquivo de ambiente MSVC nao encontrado: "%VCVARS%"
    exit /b 1
)

call "%VCVARS%" >nul
if errorlevel 1 (
    echo Falha ao inicializar o ambiente MSVC x64.
    exit /b 1
)

cd /d "%BUILD_DIR%"
if errorlevel 1 exit /b %errorlevel%

cl /nologo /FS /EHsc /std:c++14 /MDd /Zi /Od ^
    /I"%WORKSPACE%\headers" ^
    /I"%WORKSPACE%\third_party\SFML\include" ^
    "%WORKSPACE%\src\main.cpp" ^
    "%WORKSPACE%\src\Jogo.cpp" ^
    "%WORKSPACE%\src\Fases\Fase1.cpp" ^
    "%WORKSPACE%\src\Entidades\Ente.cpp" ^
    "%WORKSPACE%\src\Entidades\Entidade.cpp" ^
    "%WORKSPACE%\src\Entidades\Personagens\Personagem.cpp" ^
    "%WORKSPACE%\src\Entidades\Personagens\Jogador.cpp" ^
    "%WORKSPACE%\src\Listas\ListaEntidades.cpp" ^
    "%WORKSPACE%\src\Fases\Fase.cpp" ^
    "%WORKSPACE%\src\Gerenciadores\Gerenciador_Colisoes.cpp" ^
    "%WORKSPACE%\src\Gerenciadores\Gerenciador_Grafico.cpp" ^
    /Fe:"%BUILD_DIR%\main.exe" ^
    /Fd:"%BUILD_DIR%\main-compiler.pdb" ^
    /link /DEBUG /PDB:"%BUILD_DIR%\main.pdb" /LIBPATH:"%WORKSPACE%\sfml\bin" ^
    sfml-graphics-d.lib sfml-window-d.lib sfml-system-d.lib
if errorlevel 1 exit /b %errorlevel%

exit /b 0
