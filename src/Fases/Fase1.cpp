#include "Fases/Fase1.h"

#include "Gerenciadores/Gerenciador_Grafico.h"

#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <cstddef>
#include <sstream>

namespace Fases
{
    namespace
    {
        const float TAMANHO_TILE = 48.0f;
        const unsigned int LARGURA_NIVEL_TILES = 88;
        const unsigned int ALTURA_NIVEL_TILES = 14;
        const unsigned int LINHA_CHAO = 10;
        const float ESCALA_JANELA = 3.0f;
        const float DURACAO_QUADRO_FRUTA = 0.09f;
        const float DURACAO_QUADRO_CAIXA = 0.08f;

        void avancarQuadro(
            sf::Sprite& sprite,
            unsigned int& quadro,
            float& tempo,
            float tempoQuadro,
            float duracaoQuadro,
            unsigned int quantidadeQuadros,
            int larguraQuadro,
            int alturaQuadro
        )
        {
            tempo += tempoQuadro;
            while (tempo >= duracaoQuadro)
            {
                tempo -= duracaoQuadro;
                quadro = (quadro + 1) % quantidadeQuadros;
            }
            sprite.setTextureRect(
                sf::IntRect(
                    static_cast<int>(quadro) * larguraQuadro,
                    0,
                    larguraQuadro,
                    alturaQuadro
                )
            );
        }

        std::string numeroComoTexto(unsigned int valor)
        {
            std::ostringstream stream;
            stream << valor;
            return stream.str();
        }

        std::string numeroComZeros(unsigned int valor, unsigned int digitos)
        {
            std::string texto = numeroComoTexto(valor);
            if (texto.size() < digitos)
            {
                texto.insert(0, digitos - texto.size(), '0');
            }
            return texto;
        }
    }

    Fase1::Fase1(Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico)
        : Fase(gerenciadorGrafico),
          sapoNinja_(0.0f, 0.0f),
          estado_(Introducao),
          larguraNivel_(0.0f),
          cameraX_(0.0f),
          tempoInvulneravel_(0.0f),
          tempoDesdeDisparo_(1.0f),
          tempoInicio_(0.0f),
          tempoCheckpoint_(0.0f),
          tempoSaida_(0.0f),
          quadroInicio_(0),
          quadroCheckpoint_(0),
          quadroSaida_(0),
          frutasColetadas_(0),
          checkpointAtivo_(false)
    {
        lista_ents_.incluir(&sapoNinja_);
        GC_.setJogadores(&sapoNinja_, 0);
        carregarAssets();
        construirFase();
        reiniciarFase();
        atualizarCamera();
    }

    void Fase1::executar()
    {
        atualizar(tempoQuadro_);
    }

    void Fase1::carregarTextura(
        sf::Texture& textura,
        const std::string& caminho
    )
    {
        gerenciadorGrafico_.carregarTextura(textura, caminho);
    }

void Fase1::carregarAssets()
{
    const std::string pasta = "assets/Pixel Adventure 1/Free/";
    carregarTextura(
        texturaFundo_,
        pasta + "Background/Green.png"
    );
    texturaFundo_.setRepeated(true);
    carregarTextura(
        texturaTerreno_,
        pasta + "Terrain/Terrain (16x16).png"
    );
    carregarTextura(
        texturaFonte_,
        pasta + "Menu/Text/Text (White) (8x10).png"
    );
    carregarTextura(
        texturaBotaoJogar_,
        pasta + "Menu/Buttons/Play.png"
    );
    carregarTextura(
        texturaInicio_,
        pasta + "Items/Checkpoints/Start/Start (Moving) (64x64).png"
    );
    carregarTextura(
        texturaCheckpoint_,
        pasta + "Items/Checkpoints/Checkpoint/Checkpoint (Flag Idle)(64x64).png"
    );
    carregarTextura(
        texturaCheckpointAtivo_,
        pasta + "Items/Checkpoints/Checkpoint/Checkpoint (Flag Out) (64x64).png"
    );
    carregarTextura(
        texturaSaida_,
        pasta + "Items/Checkpoints/End/End (Idle).png"
    );
    carregarTextura(
        texturaSaidaAtiva_,
        pasta + "Items/Checkpoints/End/End (Pressed) (64x64).png"
    );
    carregarTextura(
        texturaShuriken_,
        pasta + "Items/Ninja Star/Ninja Star.png"
    );
    carregarTextura(
        texturaFogo_,
        pasta + "Traps/Fire/On (16x32).png"
    );
    carregarTextura(
        texturaSerra_,
        pasta + "Traps/Saw/On (38x38).png"
    );
    carregarTextura(
        texturaEspinhos_,
        pasta + "Traps/Spikes/Idle.png"
    );

    for (unsigned int i = 0; i < 3; ++i)
    {
        const std::string pastaCaixa =
            pasta + "Items/Boxes/Box" + numeroComoTexto(i + 1) + "/";
        carregarTextura(
            texturasCaixas_[i],
            pastaCaixa + "Idle.png"
        );
        carregarTextura(
            texturasQuebraCaixas_[i],
            pastaCaixa + "Break.png"
        );
    }

    const char* arquivosFrutas[8] = {
        "Apple.png",
        "Bananas.png",
        "Cherries.png",
        "Kiwi.png",
        "Melon.png",
        "Orange.png",
        "Pineapple.png",
        "Strawberry.png"
    };
    for (unsigned int i = 0; i < 8; ++i)
    {
        carregarTextura(
            texturasFrutas_[i],
            pasta + "Items/Fruits/" + arquivosFrutas[i]
        );
    }

    fundo_.setTexture(texturaFundo_);
    botaoJogar_.setTexture(texturaBotaoJogar_);
    botaoJogar_.setScale(3.0f, 3.0f);
    inicio_.setTexture(texturaInicio_);
    inicio_.setScale(1.5f, 1.5f);
    checkpoint_.setTexture(texturaCheckpoint_);
    checkpoint_.setScale(1.5f, 1.5f);
    saida_.setTexture(texturaSaida_);
    saida_.setScale(1.5f, 1.5f);

    for (std::size_t rangeIndex = 0; rangeIndex < 6; ++rangeIndex)
    {
        Shuriken& shuriken = shurikens_[rangeIndex];
        shuriken.sprite.setTexture(texturaShuriken_);
        shuriken.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
        shuriken.sprite.setScale(1.5f, 1.5f);
        shuriken.sprite.setOrigin(16.0f, 16.0f);
        shuriken.velocidadeX = 0.0f;
        shuriken.tempoAnimacao = 0.0f;
        shuriken.quadro = 0;
        shuriken.ativo = false;
    }

    barraHud_.setFillColor(sf::Color(19, 34, 39, 225));
}

void Fase1::construirFase()
{
    larguraNivel_ = LARGURA_NIVEL_TILES * TAMANHO_TILE;

    adicionarChao(0, 13, LINHA_CHAO);
    adicionarChao(16, 29, LINHA_CHAO);
    adicionarChao(31, 45, LINHA_CHAO);
    adicionarChao(48, 62, LINHA_CHAO);
    adicionarChao(65, 75, LINHA_CHAO);
    adicionarChao(78, LARGURA_NIVEL_TILES, LINHA_CHAO);

    adicionarPlataforma(13, 16, 8);
    adicionarPlataforma(22, 26, 7);
    adicionarPlataforma(29, 31, 8);
    adicionarPlataforma(41, 45, 7);
    adicionarPlataforma(45, 48, 8);
    adicionarPlataforma(56, 60, 7);
    adicionarPlataforma(62, 65, 8);
    adicionarPlataforma(70, 74, 7);
    adicionarPlataforma(75, 78, 8);

    pontoInicial_ = sf::Vector2f(
        2.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 64.0f
    );
    pontoCheckpoint_ = pontoInicial_;

    inicio_.setPosition(
        0.5f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 96.0f
    );
    checkpoint_.setPosition(
        40.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 96.0f
    );
    saida_.setPosition(
        84.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 96.0f
    );

    botaoJogar_.setPosition(
        (static_cast<float>(gerenciadorGrafico_.getLarguraJanela()) -
         botaoJogar_.getGlobalBounds().width) / 2.0f,
        335.0f
    );

    adicionarCaixa(6, LINHA_CHAO, 0);
    adicionarCaixa(20, LINHA_CHAO, 1);
    adicionarCaixa(34, LINHA_CHAO, 2);
    adicionarCaixa(53, LINHA_CHAO, 0);
    adicionarCaixa(72, LINHA_CHAO, 1);

    adicionarFruta(4, 8, 2);
    adicionarFruta(18, 7, 0);
    adicionarFruta(24, 5, 6);
    adicionarFruta(33, 8, 1);
    adicionarFruta(42, 5, 3);
    adicionarFruta(55, 8, 4);
    adicionarFruta(69, 6, 5);
    adicionarFruta(81, 8, 7);

    adicionarPerigo(
        texturaFogo_,
        9.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 80.0f,
        16,
        32,
        3,
        2.5f,
        0.12f
    );
    adicionarPerigo(
        texturaSerra_,
        25.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 48.0f,
        38,
        38,
        8,
        1.25f,
        0.08f
    );
    adicionarPerigo(
        texturaEspinhos_,
        36.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - TAMANHO_TILE,
        16,
        16,
        1,
        3.0f,
        0.1f
    );
    adicionarPerigo(
        texturaFogo_,
        58.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 80.0f,
        16,
        32,
        3,
        2.5f,
        0.12f
    );
    adicionarPerigo(
        texturaSerra_,
        67.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - 48.0f,
        38,
        38,
        8,
        1.25f,
        0.08f
    );
    adicionarPerigo(
        texturaEspinhos_,
        80.0f * TAMANHO_TILE,
        LINHA_CHAO * TAMANHO_TILE - TAMANHO_TILE,
        16,
        16,
        1,
        3.0f,
        0.1f
    );
}

void Fase1::adicionarTile(
    unsigned int colunaAtlas,
    unsigned int linhaAtlas,
    float x,
    float y
)
{
    sf::Sprite tile;
    tile.setTexture(texturaTerreno_);
    tile.setTextureRect(
        sf::IntRect(
            static_cast<int>(colunaAtlas * 16),
            static_cast<int>(linhaAtlas * 16),
            16,
            16
        )
    );
    tile.setScale(ESCALA_JANELA, ESCALA_JANELA);
    tile.setPosition(x, y);
    tiles_.push_back(tile);
}

void Fase1::adicionarChao(
    unsigned int inicio,
    unsigned int fim,
    unsigned int linha
)
{
    const float x = inicio * TAMANHO_TILE;
    const float y = linha * TAMANHO_TILE;
    const float largura = (fim - inicio) * TAMANHO_TILE;
    const float altura =
        (ALTURA_NIVEL_TILES - linha) * TAMANHO_TILE;
    solidosEstaticos_.push_back(
        sf::FloatRect(x, y, largura, altura)
    );

    for (unsigned int coluna = inicio; coluna < fim; ++coluna)
    {
        const unsigned int topo =
            fim - inicio == 1
                ? 7
                : coluna == inicio
                    ? 6
                    : coluna == fim - 1
                        ? 8
                        : 7;
        adicionarTile(
            topo,
            0,
            coluna * TAMANHO_TILE,
            linha * TAMANHO_TILE
        );

        for (unsigned int camada = 1; camada <= 3; ++camada)
        {
            const unsigned int variante =
                6 + ((coluna + camada) % 3);
            adicionarTile(
                variante,
                camada == 1 ? 1 : 2,
                coluna * TAMANHO_TILE,
                (linha + camada) * TAMANHO_TILE
            );
        }
    }
}

void Fase1::adicionarPlataforma(
    unsigned int inicio,
    unsigned int fim,
    unsigned int linha
)
{
    const float x = inicio * TAMANHO_TILE;
    const float y = linha * TAMANHO_TILE;
    const float largura = (fim - inicio) * TAMANHO_TILE;
    plataformasAereas_.push_back(
        sf::FloatRect(x, y, largura, 2.0f)
    );

    for (unsigned int coluna = inicio; coluna < fim; ++coluna)
    {
        const unsigned int topo =
            fim - inicio == 1
                ? 7
                : coluna == inicio
                    ? 6
                    : coluna == fim - 1
                        ? 8
                        : 7;
        adicionarTile(
            topo,
            0,
            coluna * TAMANHO_TILE,
            linha * TAMANHO_TILE
        );
        adicionarTile(
            7,
            1,
            coluna * TAMANHO_TILE,
            (linha + 1) * TAMANHO_TILE
        );
    }
}

void Fase1::adicionarCaixa(
    unsigned int coluna,
    unsigned int linha,
    unsigned int tipo
)
{
    Caixa caixa;
    caixa.tipo = tipo;
    caixa.quadro = 0;
    caixa.tempoAnimacao = 0.0f;
    caixa.ativa = true;
    caixa.quebrando = false;
    caixa.sprite.setTexture(texturasCaixas_[tipo]);
    caixa.sprite.setTextureRect(sf::IntRect(0, 0, 28, 24));
    caixa.sprite.setScale(2.0f, 2.0f);
    caixa.sprite.setPosition(
        coluna * TAMANHO_TILE,
        linha * TAMANHO_TILE - 48.0f
    );
    caixas_.push_back(caixa);
}

void Fase1::adicionarFruta(
    unsigned int coluna,
    unsigned int linha,
    unsigned int tipo
)
{
    Fruta fruta;
    fruta.tipo = tipo;
    fruta.quadro = 0;
    fruta.tempoAnimacao = 0.0f;
    fruta.ativa = true;
    fruta.sprite.setTexture(texturasFrutas_[tipo]);
    fruta.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    fruta.sprite.setScale(1.5f, 1.5f);
    fruta.sprite.setPosition(
        coluna * TAMANHO_TILE,
        linha * TAMANHO_TILE
    );
    frutas_.push_back(fruta);
}

void Fase1::adicionarPerigo(
    const sf::Texture& textura,
    float x,
    float y,
    int larguraQuadro,
    int alturaQuadro,
    unsigned int quantidadeQuadros,
    float escala,
    float duracaoQuadro
)
{
    Perigo perigo;
    perigo.sprite.setTexture(textura);
    perigo.sprite.setTextureRect(
        sf::IntRect(0, 0, larguraQuadro, alturaQuadro)
    );
    perigo.sprite.setScale(escala, escala);
    perigo.sprite.setPosition(x, y);
    perigo.quadro = 0;
    perigo.quantidadeQuadros = quantidadeQuadros;
    perigo.larguraQuadro = larguraQuadro;
    perigo.alturaQuadro = alturaQuadro;
    perigo.tempoAnimacao = 0.0f;
    perigo.duracaoQuadro = duracaoQuadro;

    const sf::FloatRect limites = perigo.sprite.getGlobalBounds();
    const float margemX = limites.width * 0.16f;
    const float margemY = limites.height * 0.14f;
    perigo.limites = sf::FloatRect(
        limites.left + margemX,
        limites.top + margemY,
        limites.width - 2.0f * margemX,
        limites.height - 2.0f * margemY
    );
    perigos_.push_back(perigo);
}

void Fase1::reiniciarFase()
{
    sapoNinja_.setVidas(3);
    sapoNinja_.setPontos(0);
    frutasColetadas_ = 0;
    checkpointAtivo_ = false;
    tempoInvulneravel_ = 0.0f;
    tempoDesdeDisparo_ = 1.0f;
    pontoCheckpoint_ = pontoInicial_;
    cameraX_ = 0.0f;

    sapoNinja_.reiniciar(pontoInicial_.x, pontoInicial_.y);

    for (std::size_t rangeIndex = 0; rangeIndex < caixas_.size(); ++rangeIndex)
    {
        Caixa& caixa = caixas_[rangeIndex];
        caixa.ativa = true;
        caixa.quebrando = false;
        caixa.quadro = 0;
        caixa.tempoAnimacao = 0.0f;
        caixa.sprite.setTexture(texturasCaixas_[caixa.tipo], true);
        caixa.sprite.setTextureRect(sf::IntRect(0, 0, 28, 24));
        caixa.sprite.setScale(2.0f, 2.0f);
    }
    for (std::size_t rangeIndex = 0; rangeIndex < frutas_.size(); ++rangeIndex)
    {
        Fruta& fruta = frutas_[rangeIndex];
        fruta.ativa = true;
        fruta.quadro = 0;
        fruta.tempoAnimacao = 0.0f;
        fruta.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    }
    for (std::size_t rangeIndex = 0; rangeIndex < 6; ++rangeIndex)
    {
        Shuriken& shuriken = shurikens_[rangeIndex];
        shuriken.ativo = false;
        shuriken.quadro = 0;
        shuriken.tempoAnimacao = 0.0f;
    }

    checkpoint_.setTexture(texturaCheckpoint_, true);
    checkpoint_.setTextureRect(sf::IntRect(0, 0, 64, 64));
    saida_.setTexture(texturaSaida_, true);
    saida_.setTextureRect(sf::IntRect(0, 0, 64, 64));
    quadroInicio_ = 0;
    quadroCheckpoint_ = 0;
    quadroSaida_ = 0;
    tempoInicio_ = 0.0f;
    tempoCheckpoint_ = 0.0f;
    tempoSaida_ = 0.0f;
}

void Fase1::atualizar(float tempoQuadro)
{
    tempoQuadro = std::min(tempoQuadro, 0.1f);
    setTempoQuadro(tempoQuadro);
    atualizarAnimacoes(tempoQuadro);

    const bool enter =
        gerenciadorGrafico_.consumirPressionamentoTecla(sf::Keyboard::Enter);
    const bool espaco =
        gerenciadorGrafico_.consumirPressionamentoTecla(sf::Keyboard::Space);
    const bool escape =
        gerenciadorGrafico_.consumirPressionamentoTecla(sf::Keyboard::Escape);
    const bool reiniciar =
        gerenciadorGrafico_.consumirPressionamentoTecla(sf::Keyboard::R);
    const bool atirar =
        gerenciadorGrafico_.consumirPressionamentoTecla(sf::Keyboard::F);
    sf::Vector2f posicaoClique;
    const bool clicou =
        gerenciadorGrafico_.consumirCliqueMouse(posicaoClique);

    if (estado_ == Introducao)
    {
        if (enter || espaco || clicou)
        {
            estado_ = Jogando;
        }
        atualizarCamera();
        return;
    }

    if (estado_ == Pausada)
    {
        if (reiniciar)
        {
            reiniciarFase();
            estado_ = Jogando;
        }
        else if (escape)
        {
            estado_ = Jogando;
        }
        return;
    }

    if (estado_ == Concluida || estado_ == GameOver)
    {
        if (enter)
        {
            reiniciarFase();
            estado_ = Introducao;
        }
        return;
    }

    if (escape)
    {
        estado_ = Pausada;
        return;
    }
    if (reiniciar)
    {
        reiniciarFase();
        estado_ = Jogando;
        return;
    }

    tempoDesdeDisparo_ += tempoQuadro;
    if (tempoInvulneravel_ > 0.0f)
    {
        tempoInvulneravel_ =
            std::max(0.0f, tempoInvulneravel_ - tempoQuadro);
    }

    std::vector<sf::FloatRect> solidos = solidosEstaticos_;
    for (std::size_t rangeIndex = 0; rangeIndex < caixas_.size(); ++rangeIndex)
    {
        const Caixa& caixa = caixas_[rangeIndex];
        if (caixa.ativa && !caixa.quebrando)
        {
            solidos.push_back(caixa.sprite.getGlobalBounds());
        }
    }

    sapoNinja_.configurarExecucao(
        tempoQuadro,
        larguraNivel_,
        solidos,
        plataformasAereas_
    );
    lista_ents_.percorrer();

    const sf::FloatRect limitesSapo = sapoNinja_.getLimites();
    for (std::size_t rangeIndex = 0; rangeIndex < frutas_.size(); ++rangeIndex)
    {
        Fruta& fruta = frutas_[rangeIndex];
        if (fruta.ativa &&
            GC_.verificarColisao(
                limitesSapo,
                fruta.sprite.getGlobalBounds()
            ))
        {
            fruta.ativa = false;
            ++frutasColetadas_;
            sapoNinja_.adicionarPontos(10);
        }
    }

    if (!checkpointAtivo_ &&
        GC_.verificarColisao(
            limitesSapo,
            checkpoint_.getGlobalBounds()
        ))
    {
        checkpointAtivo_ = true;
        pontoCheckpoint_ = sf::Vector2f(
            41.0f * TAMANHO_TILE,
            LINHA_CHAO * TAMANHO_TILE - 64.0f
        );
        checkpoint_.setTexture(texturaCheckpointAtivo_, true);
        checkpoint_.setTextureRect(sf::IntRect(0, 0, 64, 64));
        quadroCheckpoint_ = 0;
    }

    if (tempoInvulneravel_ <= 0.0f)
    {
        for (std::size_t rangeIndex = 0; rangeIndex < perigos_.size(); ++rangeIndex)
        {
            const Perigo& perigo = perigos_[rangeIndex];
            if (GC_.verificarColisao(limitesSapo, perigo.limites))
            {
                receberDano();
                break;
            }
        }
    }

    if (estado_ == Jogando &&
        sapoNinja_.getPosicao().y >
            ALTURA_NIVEL_TILES * TAMANHO_TILE + 48.0f)
    {
        receberDano();
    }

    if (estado_ == Jogando &&
        (atirar || clicou) &&
        tempoDesdeDisparo_ >= 0.2f)
    {
        dispararShuriken();
    }
    if (estado_ == Jogando)
    {
        atualizarShurikens(tempoQuadro);
    }

    if (estado_ == Jogando &&
        GC_.verificarColisao(
            limitesSapo,
            saida_.getGlobalBounds()
        ))
    {
        estado_ = Concluida;
        saida_.setTexture(texturaSaidaAtiva_, true);
        saida_.setTextureRect(sf::IntRect(0, 0, 64, 64));
        quadroSaida_ = 0;
    }

    atualizarCamera();
}

void Fase1::atualizarAnimacoes(float tempoQuadro)
{
    avancarQuadro(
        inicio_,
        quadroInicio_,
        tempoInicio_,
        tempoQuadro,
        0.08f,
        17,
        64,
        64
    );

    const unsigned int quadrosCheckpoint = checkpointAtivo_ ? 26 : 10;
    avancarQuadro(
        checkpoint_,
        quadroCheckpoint_,
        tempoCheckpoint_,
        tempoQuadro,
        0.1f,
        quadrosCheckpoint,
        64,
        64
    );

    if (estado_ == Concluida)
    {
        avancarQuadro(
            saida_,
            quadroSaida_,
            tempoSaida_,
            tempoQuadro,
            0.1f,
            8,
            64,
            64
        );
    }

    atualizarCaixas(tempoQuadro);
    atualizarFrutas(tempoQuadro);
    atualizarPerigos(tempoQuadro);
}

void Fase1::atualizarCaixas(float tempoQuadro)
{
    for (std::size_t rangeIndex = 0; rangeIndex < caixas_.size(); ++rangeIndex)
    {
        Caixa& caixa = caixas_[rangeIndex];
        if (!caixa.ativa || !caixa.quebrando)
        {
            continue;
        }
        caixa.tempoAnimacao += tempoQuadro;
        while (caixa.tempoAnimacao >= DURACAO_QUADRO_CAIXA)
        {
            caixa.tempoAnimacao -= DURACAO_QUADRO_CAIXA;
            ++caixa.quadro;
            if (caixa.quadro >= 4)
            {
                caixa.ativa = false;
                caixa.quebrando = false;
                break;
            }
            caixa.sprite.setTextureRect(
                sf::IntRect(
                    static_cast<int>(caixa.quadro) * 28,
                    0,
                    28,
                    24
                )
            );
        }
    }
}

void Fase1::atualizarFrutas(float tempoQuadro)
{
    for (std::size_t rangeIndex = 0; rangeIndex < frutas_.size(); ++rangeIndex)
    {
        Fruta& fruta = frutas_[rangeIndex];
        if (!fruta.ativa)
        {
            continue;
        }
        avancarQuadro(
            fruta.sprite,
            fruta.quadro,
            fruta.tempoAnimacao,
            tempoQuadro,
            DURACAO_QUADRO_FRUTA,
            17,
            32,
            32
        );
    }
}

void Fase1::atualizarPerigos(float tempoQuadro)
{
    for (std::size_t rangeIndex = 0; rangeIndex < perigos_.size(); ++rangeIndex)
    {
        Perigo& perigo = perigos_[rangeIndex];
        if (perigo.quantidadeQuadros <= 1)
        {
            continue;
        }
        avancarQuadro(
            perigo.sprite,
            perigo.quadro,
            perigo.tempoAnimacao,
            tempoQuadro,
            perigo.duracaoQuadro,
            perigo.quantidadeQuadros,
            perigo.larguraQuadro,
            perigo.alturaQuadro
        );
    }
}

void Fase1::atualizarShurikens(float tempoQuadro)
{
    for (std::size_t rangeIndex = 0; rangeIndex < 6; ++rangeIndex)
    {
        Shuriken& shuriken = shurikens_[rangeIndex];
        if (!shuriken.ativo)
        {
            continue;
        }

        const sf::FloatRect limitesAnteriores =
            shuriken.sprite.getGlobalBounds();
        sf::Vector2f posicao = shuriken.sprite.getPosition();
        posicao.x += shuriken.velocidadeX * tempoQuadro;
        shuriken.sprite.setPosition(posicao);

        shuriken.tempoAnimacao += tempoQuadro;
        while (shuriken.tempoAnimacao >= 0.06f)
        {
            shuriken.tempoAnimacao -= 0.06f;
            shuriken.quadro = (shuriken.quadro + 1) % 8;
            shuriken.sprite.setTextureRect(
                sf::IntRect(
                    static_cast<int>(shuriken.quadro) * 32,
                    0,
                    32,
                    32
                )
            );
        }

        const sf::FloatRect limitesAtuais =
            shuriken.sprite.getGlobalBounds();
        const float esquerda =
            std::min(limitesAnteriores.left, limitesAtuais.left);
        const float topo =
            std::min(limitesAnteriores.top, limitesAtuais.top);
        const float direita =
            std::max(
                limitesAnteriores.left + limitesAnteriores.width,
                limitesAtuais.left + limitesAtuais.width
            );
        const float base =
            std::max(
                limitesAnteriores.top + limitesAnteriores.height,
                limitesAtuais.top + limitesAtuais.height
            );
        const sf::FloatRect areaPercorrida(
            esquerda,
            topo,
            direita - esquerda,
            base - topo
        );

        if (posicao.x < -32.0f || posicao.x > larguraNivel_ + 32.0f)
        {
            shuriken.ativo = false;
            continue;
        }

        for (std::size_t rangeIndex = 0; rangeIndex < caixas_.size(); ++rangeIndex)
        {
            Caixa& caixa = caixas_[rangeIndex];
            if (caixa.ativa &&
                !caixa.quebrando &&
                GC_.verificarColisao(
                    areaPercorrida,
                    caixa.sprite.getGlobalBounds()
                ))
            {
                quebrarCaixa(caixa);
                shuriken.ativo = false;
                break;
            }
        }
    }
}

void Fase1::atualizarCamera()
{
    const float larguraVista =
        static_cast<float>(gerenciadorGrafico_.getLarguraJanela());
    const float alturaVista =
        static_cast<float>(gerenciadorGrafico_.getAlturaJanela());
    const float centroJogador =
        sapoNinja_.getLimites().left +
        sapoNinja_.getLimites().width * 0.5f;
    cameraX_ = std::max(
        0.0f,
        std::min(
            centroJogador - larguraVista * 0.4f,
            larguraNivel_ - larguraVista
        )
    );
    vistaMundo_.reset(
        sf::FloatRect(cameraX_, 0.0f, larguraVista, alturaVista)
    );
}

void Fase1::dispararShuriken()
{
    for (std::size_t rangeIndex = 0; rangeIndex < 6; ++rangeIndex)
    {
        Shuriken& shuriken = shurikens_[rangeIndex];
        if (shuriken.ativo)
        {
            continue;
        }

        const sf::FloatRect limitesSapo = sapoNinja_.getLimites();
        const bool olhandoParaEsquerda =
            sapoNinja_.olhandoParaEsquerda();
        shuriken.ativo = true;
        shuriken.velocidadeX = olhandoParaEsquerda ? -620.0f : 620.0f;
        shuriken.quadro = 0;
        shuriken.tempoAnimacao = 0.0f;
        shuriken.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
        shuriken.sprite.setRotation(0.0f);
        shuriken.sprite.setPosition(
            limitesSapo.left +
                (olhandoParaEsquerda ? -8.0f : limitesSapo.width + 8.0f),
            limitesSapo.top + limitesSapo.height * 0.48f
        );
        tempoDesdeDisparo_ = 0.0f;
        return;
    }
}

void Fase1::quebrarCaixa(Caixa& caixa)
{
    caixa.quebrando = true;
    caixa.quadro = 0;
    caixa.tempoAnimacao = 0.0f;
    caixa.sprite.setTexture(texturasQuebraCaixas_[caixa.tipo], true);
    caixa.sprite.setTextureRect(sf::IntRect(0, 0, 28, 24));
    sapoNinja_.adicionarPontos(100);
}

void Fase1::receberDano()
{
    if (tempoInvulneravel_ > 0.0f || estado_ != Jogando)
    {
        return;
    }

    sapoNinja_.receberDano();
    if (sapoNinja_.getVidas() == 0)
    {
        estado_ = GameOver;
        return;
    }

    sapoNinja_.reiniciar(pontoCheckpoint_.x, pontoCheckpoint_.y);
    tempoInvulneravel_ = 1.5f;
}

void Fase1::desenharMundo()
{
    gerenciadorGrafico_.aplicarVista(vistaMundo_);

    const int deslocamentoFundo =
        static_cast<int>(cameraX_ * 0.2f);
    fundo_.setTextureRect(
        sf::IntRect(
            deslocamentoFundo,
            0,
            static_cast<int>(gerenciadorGrafico_.getLarguraJanela()),
            static_cast<int>(gerenciadorGrafico_.getAlturaJanela())
        )
    );
    fundo_.setPosition(cameraX_, 0.0f);
    gerenciadorGrafico_.desenhar(fundo_);

    for (std::size_t rangeIndex = 0; rangeIndex < tiles_.size(); ++rangeIndex)
    {
        const sf::Sprite& tile = tiles_[rangeIndex];
        gerenciadorGrafico_.desenhar(tile);
    }
    gerenciadorGrafico_.desenhar(inicio_);
    gerenciadorGrafico_.desenhar(checkpoint_);
    gerenciadorGrafico_.desenhar(saida_);

    for (std::size_t rangeIndex = 0; rangeIndex < perigos_.size(); ++rangeIndex)
    {
        const Perigo& perigo = perigos_[rangeIndex];
        gerenciadorGrafico_.desenhar(perigo.sprite);
    }
    for (std::size_t rangeIndex = 0; rangeIndex < frutas_.size(); ++rangeIndex)
    {
        const Fruta& fruta = frutas_[rangeIndex];
        if (fruta.ativa)
        {
            gerenciadorGrafico_.desenhar(fruta.sprite);
        }
    }
    for (std::size_t rangeIndex = 0; rangeIndex < caixas_.size(); ++rangeIndex)
    {
        const Caixa& caixa = caixas_[rangeIndex];
        if (caixa.ativa)
        {
            gerenciadorGrafico_.desenhar(caixa.sprite);
        }
    }
    for (std::size_t rangeIndex = 0; rangeIndex < 6; ++rangeIndex)
    {
        const Shuriken& shuriken = shurikens_[rangeIndex];
        if (shuriken.ativo)
        {
            gerenciadorGrafico_.desenhar(shuriken.sprite);
        }
    }

    const bool mostrarSapo =
        tempoInvulneravel_ <= 0.0f ||
        static_cast<unsigned int>(tempoInvulneravel_ * 12.0f) % 2 == 0;
    if (mostrarSapo)
    {
        gerenciadorGrafico_.desenharEnte(&sapoNinja_);
    }
    gerenciadorGrafico_.restaurarVistaPadrao();
}

void Fase1::desenharHud()
{
    barraHud_.setSize(
        sf::Vector2f(
            static_cast<float>(gerenciadorGrafico_.getLarguraJanela()),
            44.0f
        )
    );
    barraHud_.setPosition(0.0f, 0.0f);
    gerenciadorGrafico_.desenhar(barraHud_);

    desenharTexto("FASE 1 BOSQUE", 16.0f, 11.0f, 1.5f, sf::Color::White);
    desenharTexto(
        "FRUTAS " + numeroComoTexto(frutasColetadas_) +
            " DE " + numeroComoTexto(static_cast<unsigned int>(frutas_.size())),
        285.0f,
        11.0f,
        1.5f,
        sf::Color::White
    );
    desenharTexto(
        "PONTOS " + numeroComZeros(
            static_cast<unsigned int>(sapoNinja_.getPontos()),
            3
        ),
        590.0f,
        11.0f,
        1.5f,
        sf::Color::White
    );
    desenharTexto(
        "VIDAS " + numeroComoTexto(
            static_cast<unsigned int>(sapoNinja_.getVidas())
        ),
        830.0f,
        11.0f,
        1.5f,
        sf::Color::White
    );
}

void Fase1::desenharOverlay(
    const std::string& titulo,
    const std::string& linha1,
    const std::string& linha2
)
{
    sf::RectangleShape fundoOverlay(
        sf::Vector2f(
            static_cast<float>(gerenciadorGrafico_.getLarguraJanela()),
            static_cast<float>(gerenciadorGrafico_.getAlturaJanela())
        )
    );
    fundoOverlay.setFillColor(sf::Color(11, 19, 23, 205));
    gerenciadorGrafico_.desenhar(fundoOverlay);

    desenharTextoCentralizado(
        titulo,
        142.0f,
        4.0f,
        sf::Color(246, 245, 220)
    );
    desenharTextoCentralizado(
        linha1,
        230.0f,
        2.0f,
        sf::Color(192, 230, 159)
    );
    desenharTextoCentralizado(
        linha2,
        278.0f,
        2.0f,
        sf::Color::White
    );
}

void Fase1::desenharTexto(
    const std::string& texto,
    float x,
    float y,
    float escala,
    const sf::Color& cor
)
{
    float posicaoX = x;
    for (std::size_t rangeIndex = 0; rangeIndex < texto.size(); ++rangeIndex)
    {
        char caractere = texto[rangeIndex];
        if (caractere == ' ')
        {
            posicaoX += 8.0f * escala;
            continue;
        }

        int indice = -1;
        if (caractere >= 'A' && caractere <= 'Z')
        {
            indice = caractere - 'A';
        }
        else if (caractere >= '0' && caractere <= '9')
        {
            indice = 30 + caractere - '0';
        }
        if (indice < 0)
        {
            posicaoX += 8.0f * escala;
            continue;
        }

        sf::Sprite glifo;
        glifo.setTexture(texturaFonte_);
        glifo.setTextureRect(
            sf::IntRect((indice % 10) * 8, (indice / 10) * 10, 8, 10)
        );
        glifo.setScale(escala, escala);
        glifo.setPosition(posicaoX, y);
        glifo.setColor(cor);
        gerenciadorGrafico_.desenhar(glifo);
        posicaoX += 8.0f * escala;
    }
}

void Fase1::desenharTextoCentralizado(
    const std::string& texto,
    float y,
    float escala,
    const sf::Color& cor
)
{
    const float largura =
        static_cast<float>(texto.size()) * 8.0f * escala;
    const float x =
        (static_cast<float>(gerenciadorGrafico_.getLarguraJanela()) -
         largura) / 2.0f;
    desenharTexto(texto, x, y, escala, cor);
}

void Fase1::desenhar()
{
    desenharMundo();
    desenharHud();

    if (estado_ == Introducao)
    {
        desenharOverlay(
            "NINJA FROG",
            "FASE 1 BOSQUE VERDE",
            "ENTER PARA JOGAR"
        );
        gerenciadorGrafico_.desenhar(botaoJogar_);
        desenharTextoCentralizado(
            "AD MOVER  ESPACO PULO DUPLO",
            405.0f,
            1.4f,
            sf::Color::White
        );
        desenharTextoCentralizado(
            "F ATIRA SHURIKEN",
            435.0f,
            1.4f,
            sf::Color(192, 230, 159)
        );
    }
    else if (estado_ == Pausada)
    {
        desenharOverlay(
            "PAUSADO",
            "ESC CONTINUA",
            "R REINICIA A FASE"
        );
    }
    else if (estado_ == Concluida)
    {
        desenharOverlay(
            "FASE COMPLETA",
            "FRUTAS " + numeroComoTexto(frutasColetadas_) +
                " DE " + numeroComoTexto(
                    static_cast<unsigned int>(frutas_.size())
                ),
            "ENTER NOVA PARTIDA"
        );
    }
    else if (estado_ == GameOver)
    {
        desenharOverlay(
            "FIM DE JOGO",
            "PONTOS " + numeroComZeros(
                static_cast<unsigned int>(sapoNinja_.getPontos()),
                3
            ),
            "ENTER TENTAR DE NOVO"
        );
    }
}
}
