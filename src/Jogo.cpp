#include "Jogo.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>
#include <stdexcept>
#include <cmath>

namespace
{
    const bool SEGMENTOS[10][7] = {
        {true, true, true, true, true, true, false},
        {false, true, true, false, false, false, false},
        {true, true, false, true, true, false, true},
        {true, true, true, true, false, false, true},
        {false, true, true, false, false, true, true},
        {true, false, true, true, false, true, true},
        {true, false, true, true, true, true, true},
        {true, true, true, false, false, false, false},
        {true, true, true, true, true, true, true},
        {true, true, true, true, false, true, true}
    };

    void desenharNumero(
        Gerenciador_Grafico& grafico,
        unsigned int valor,
        unsigned int digitos,
        float x,
        float y
    )
    {
        const float largura = 8.0f;
        const float altura = 3.0f;
        const float passo = 16.0f;
        for (unsigned int i = 0; i < digitos; ++i)
        {
            const unsigned int digito = valor % 10;
            valor /= 10;
            const float origem = x + (digitos - i - 1) * passo;
            for (unsigned int segmento = 0; segmento < 7; ++segmento)
            {
                if (!SEGMENTOS[digito][segmento])
                {
                    continue;
                }
                sf::RectangleShape forma;
                forma.setSize(
                    segmento == 0 || segmento == 3 || segmento == 6
                        ? sf::Vector2f(largura, altura)
                        : sf::Vector2f(altura, largura)
                );
                const float posicoesX[7] = {3, 11, 11, 3, 0, 0, 3};
                const float posicoesY[7] = {0, 3, 12, 20, 12, 3, 10};
                forma.setPosition(origem + posicoesX[segmento],
                                  y + posicoesY[segmento]);
                forma.setFillColor(sf::Color::White);
                grafico.desenhar(forma);
            }
        }
    }

    sf::Image criarImagemShuriken()
    {
        const char* pixels[16] = {
            ".......##.......", "......####......", ".....##..##.....",
            "....##....##....", "...##......##...", "..##...##...##..",
            ".##...####...##.", "########..######", "######..########",
            ".##...####...##.", "..##...##...##..", "...##......##...",
            "....##....##....", ".....##..##.....", "......####......",
            ".......##......."
        };
        sf::Image imagem;
        imagem.create(16, 16, sf::Color::Transparent);
        for (unsigned int y = 0; y < 16; ++y)
        {
            for (unsigned int x = 0; x < 16; ++x)
            {
                if (pixels[y][x] == '#')
                {
                    imagem.setPixel(
                        x,
                        y,
                        x >= 7 && x <= 8 && y >= 7 && y <= 8
                            ? sf::Color(255, 205, 70)
                            : sf::Color(215, 225, 240)
                    );
                }
            }
        }
        return imagem;
    }
}

Jogo::Jogo()
    : gerenciadorGrafico_(800, 600, "Aventura do Sapo Ninja"),
      sapoNinja_(80.0f, gerenciadorGrafico_.getPosicaoChao()),
      tempoRun_(0.0f),
      tempoDesdeDano_(1.0f),
      tempoRecargaShuriken_(2.0f),
      vidas_(5),
      pontuacao_(0),
      runAtiva_(false),
      faseSelecionada_(0),
      telaAtual_(Tela::Menu)
{
    carregarFundo("assets/Pixel Adventure 1/Free/Background/Blue.png");

    const std::string caminhoTextura =
        "assets/Pixel Adventure 1/Free/Traps/Rock Head/Idle.png";
    if (!texturaCabecaPedra_.loadFromFile(caminhoTextura))
    {
        throw std::runtime_error(
            "Nao foi possivel carregar o asset: " + caminhoTextura
        );
    }
    cabecaPedra_.setTexture(texturaCabecaPedra_);
    cabecaPedra_.setScale(2.0f, 2.0f);
    cabecaPedra_.setPosition(
        400.0f,
        gerenciadorGrafico_.getPosicaoChao() -
            cabecaPedra_.getGlobalBounds().height
    );

    const std::string caminhoBotao =
        "assets/Pixel Adventure 1/Free/Menu/Buttons/Play.png";
    if (!texturaBotaoJogar_.loadFromFile(caminhoBotao))
    {
        throw std::runtime_error(
            "Nao foi possivel carregar o asset: " + caminhoBotao
        );
    }
    botaoJogar_.setTexture(texturaBotaoJogar_);
    botaoJogar_.setScale(3.0f, 3.0f);
    botaoJogar_.setPosition(
        (static_cast<float>(gerenciadorGrafico_.getLarguraJanela()) -
         botaoJogar_.getGlobalBounds().width) / 2.0f,
        350.0f
    );

    const std::string pastaFases =
        "assets/Pixel Adventure 1/Free/Menu/Levels/";
    for (unsigned int i = 0; i < 2; ++i)
    {
        const std::string caminho = pastaFases + (i == 0 ? "01.png" : "02.png");
        if (!texturasFases_[i].loadFromFile(caminho))
        {
            throw std::runtime_error(
                "Nao foi possivel carregar o asset: " + caminho
            );
        }
        botoesFases_[i].setTexture(texturasFases_[i]);
        botoesFases_[i].setScale(3.0f, 3.0f);
        botoesFases_[i].setPosition(290.0f + i * 120.0f, 225.0f);
    }

    selecaoFase_.setFillColor(sf::Color::Transparent);
    selecaoFase_.setOutlineColor(sf::Color(255, 220, 80));
    selecaoFase_.setOutlineThickness(4.0f);

    if (!texturaShuriken_.loadFromImage(criarImagemShuriken()))
    {
        throw std::runtime_error("Nao foi possivel criar o sprite da shuriken");
    }
    texturaShuriken_.setSmooth(false);

    const std::string pastaPersonagens =
        "assets/Pixel Adventure 1/Free/Main Characters/";
    const std::string caminhosInimigos[3] = {
        pastaPersonagens + "Mask Dude/Idle (32x32).png",
        pastaPersonagens + "Pink Man/Idle (32x32).png",
        pastaPersonagens + "Virtual Guy/Idle (32x32).png"
    };
    const float posicoesInimigos[3] = {260.0f, 485.0f, 670.0f};
    for (unsigned int i = 0; i < 3; ++i)
    {
        if (!texturasInimigos_[i].loadFromFile(caminhosInimigos[i]))
        {
            throw std::runtime_error(
                "Nao foi possivel carregar o asset: " + caminhosInimigos[i]
            );
        }
        inimigos_[i].setTexture(texturasInimigos_[i]);
        inimigos_[i].setTextureRect(sf::IntRect(0, 0, 32, 32));
        inimigos_[i].setScale(2.0f, 2.0f);
        inimigos_[i].setPosition(
            posicoesInimigos[i],
            gerenciadorGrafico_.getPosicaoChao() - 64.0f
        );
        inimigoAtivo_[i] = true;
    }

    const float posicoesMoedas[8] = {
        180.0f, 320.0f, 400.0f, 560.0f, 740.0f, 100.0f, 620.0f, 350.0f
    };
    for (unsigned int i = 0; i < 8; ++i)
    {
        moedas_[i].setRadius(9.0f);
        moedas_[i].setFillColor(sf::Color(255, 205, 40));
        moedas_[i].setOutlineThickness(2.0f);
        moedas_[i].setOutlineColor(sf::Color(255, 245, 130));
        moedas_[i].setPosition(
            posicoesMoedas[i],
            i < 5 ? gerenciadorGrafico_.getPosicaoChao() - 95.0f
                  : gerenciadorGrafico_.getPosicaoChao() - 55.0f
        );
    }

    for (unsigned int i = 0; i < 5; ++i)
    {
        coracoes_[i].setPointCount(8);
        coracoes_[i].setPoint(0, sf::Vector2f(10.0f, 20.0f));
        coracoes_[i].setPoint(1, sf::Vector2f(0.0f, 10.0f));
        coracoes_[i].setPoint(2, sf::Vector2f(0.0f, 5.0f));
        coracoes_[i].setPoint(3, sf::Vector2f(5.0f, 0.0f));
        coracoes_[i].setPoint(4, sf::Vector2f(10.0f, 5.0f));
        coracoes_[i].setPoint(5, sf::Vector2f(15.0f, 0.0f));
        coracoes_[i].setPoint(6, sf::Vector2f(20.0f, 5.0f));
        coracoes_[i].setPoint(7, sf::Vector2f(20.0f, 10.0f));
        coracoes_[i].setPosition(646.0f + i * 27.0f, 18.0f);
    }

    for (std::size_t i = 0; i < shurikens_.size(); ++i)
    {
        shurikens_[i].sprite.setTexture(texturaShuriken_);
        shurikens_[i].sprite.setScale(2.0f, 2.0f);
        shurikens_[i].sprite.setOrigin(8.0f, 8.0f);
        shurikens_[i].velocidade = 0.0f;
        shurikens_[i].ativo = false;
    }
}

void Jogo::iniciarRun()
{
    sapoNinja_.reiniciar(80.0f, gerenciadorGrafico_.getPosicaoChao());
    vidas_ = 5;
    pontuacao_ = 0;
    tempoRun_ = 0.0f;
    tempoDesdeDano_ = 1.0f;
    tempoRecargaShuriken_ = 2.0f;
    relogioRun_.restart();
    runAtiva_ = true;
    for (unsigned int i = 0; i < 8; ++i)
    {
        moedas_[i].setRadius(9.0f);
    }
    for (unsigned int i = 0; i < 3; ++i)
    {
        inimigoAtivo_[i] = true;
        inimigos_[i].setColor(sf::Color::White);
    }
    for (std::size_t i = 0; i < shurikens_.size(); ++i)
    {
        shurikens_[i].ativo = false;
    }
}

void Jogo::desenharHud()
{
    for (unsigned int i = 0; i < 5; ++i)
    {
        coracoes_[i].setFillColor(
            i < vidas_ ? sf::Color(235, 55, 75) : sf::Color(75, 75, 85)
        );
        gerenciadorGrafico_.desenhar(coracoes_[i]);
    }
    const sf::Vector2f moedaPosicao(18.0f, 19.0f);
    sf::CircleShape iconeMoeda(8.0f);
    iconeMoeda.setPosition(moedaPosicao);
    iconeMoeda.setFillColor(sf::Color(255, 205, 40));
    gerenciadorGrafico_.desenhar(iconeMoeda);
    desenharNumero(gerenciadorGrafico_, pontuacao_, 3, 42.0f, 16.0f);
    const unsigned int segundos = static_cast<unsigned int>(tempoRun_);
    sf::CircleShape relogio(9.0f);
    relogio.setPosition(145.0f, 15.0f);
    relogio.setFillColor(sf::Color::Transparent);
    relogio.setOutlineThickness(2.0f);
    relogio.setOutlineColor(sf::Color::White);
    gerenciadorGrafico_.desenhar(relogio);
    sf::RectangleShape ponteiro(sf::Vector2f(2.0f, 6.0f));
    ponteiro.setPosition(152.0f, 19.0f);
    ponteiro.setFillColor(sf::Color::White);
    gerenciadorGrafico_.desenhar(ponteiro);
    desenharNumero(gerenciadorGrafico_, segundos, 3, 170.0f, 16.0f);
}

void Jogo::carregarFundo(const std::string& caminho)
{
    if (!texturaFundo_.loadFromFile(caminho))
    {
        throw std::runtime_error("Nao foi possivel carregar o asset: " + caminho);
    }

    fundo_.setTexture(texturaFundo_);
    const sf::Vector2u tamanhoTextura = texturaFundo_.getSize();
    fundo_.setScale(
        static_cast<float>(gerenciadorGrafico_.getLarguraJanela()) /
            static_cast<float>(tamanhoTextura.x),
        gerenciadorGrafico_.getPosicaoChao() /
            static_cast<float>(tamanhoTextura.y)
    );
}

void Jogo::desenharMenu()
{
    gerenciadorGrafico_.desenhar(fundo_);
    for (unsigned int i = 0; i < 2; ++i)
    {
        gerenciadorGrafico_.desenhar(botoesFases_[i]);
    }
    const sf::FloatRect limites =
        botoesFases_[faseSelecionada_].getGlobalBounds();
    selecaoFase_.setPosition(limites.left - 4.0f, limites.top - 4.0f);
    selecaoFase_.setSize(
        sf::Vector2f(limites.width + 8.0f, limites.height + 8.0f)
    );
    gerenciadorGrafico_.desenhar(selecaoFase_);
    gerenciadorGrafico_.desenhar(botaoJogar_);
}

void Jogo::executar()
{
    sf::Clock relogio;

    while (gerenciadorGrafico_.estaAberto())
    {
        gerenciadorGrafico_.processarEventos();
        if (!gerenciadorGrafico_.estaAberto())
        {
            break;
        }

        if (telaAtual_ == Tela::Menu)
        {
            gerenciadorGrafico_.consumirPressionamentoF();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                faseSelecionada_ = 0;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                faseSelecionada_ = 1;
            }

            sf::Vector2f mouse;
            if (gerenciadorGrafico_.consumirCliqueMouse(mouse))
            {
                for (unsigned int i = 0; i < 2; ++i)
                {
                    sf::FloatRect areaFase =
                        botoesFases_[i].getGlobalBounds();
                    areaFase.left -= 18.0f;
                    areaFase.top -= 18.0f;
                    areaFase.width += 36.0f;
                    areaFase.height += 36.0f;
                    if (areaFase.contains(mouse.x, mouse.y))
                    {
                        faseSelecionada_ = i;
                    }
                }
                sf::FloatRect areaJogar = botaoJogar_.getGlobalBounds();
                areaJogar.left -= 18.0f;
                areaJogar.top -= 18.0f;
                areaJogar.width += 36.0f;
                areaJogar.height += 36.0f;
                if (areaJogar.contains(mouse.x, mouse.y))
                {
                    telaAtual_ =
                        faseSelecionada_ == 0 ? Tela::Fase1 : Tela::Fase2;
                    iniciarRun();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                telaAtual_ =
                    faseSelecionada_ == 0 ? Tela::Fase1 : Tela::Fase2;
                iniciarRun();
            }

            if (telaAtual_ != Tela::Menu)
            {
                carregarFundo(
                    faseSelecionada_ == 0
                        ? "assets/Pixel Adventure 1/Free/Background/Green.png"
                        : "assets/Pixel Adventure 1/Free/Background/Purple.png"
                );
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            telaAtual_ = Tela::Menu;
            runAtiva_ = false;
            carregarFundo(
                "assets/Pixel Adventure 1/Free/Background/Blue.png"
            );
        }

        if (telaAtual_ == Tela::Menu)
        {
            gerenciadorGrafico_.limpar();
            desenharMenu();
            gerenciadorGrafico_.exibir();
            continue;
        }

        float tempoQuadro = relogio.restart().asSeconds();
        if (tempoQuadro > 0.1f)
        {
            tempoQuadro = 0.1f;
        }

        if (runAtiva_ && vidas_ > 0)
        {
            tempoRun_ += tempoQuadro;
            tempoDesdeDano_ += tempoQuadro;
            tempoRecargaShuriken_ += tempoQuadro;
            sapoNinja_.atualizar(
                tempoQuadro,
                static_cast<float>(gerenciadorGrafico_.getLarguraJanela()),
                gerenciadorGrafico_.getPosicaoChao()
            );

            const bool tocouArmadilha =
                sapoNinja_.colideCom(cabecaPedra_.getGlobalBounds());
            if (tocouArmadilha && tempoDesdeDano_ >= 1.0f)
            {
                --vidas_;
                tempoDesdeDano_ = 0.0f;
            }

            for (unsigned int i = 0; i < 8; ++i)
            {
                if (moedas_[i].getRadius() > 0.0f &&
                    sapoNinja_.intersecta(moedas_[i].getGlobalBounds()))
                {
                    moedas_[i].setRadius(0.0f);
                    ++pontuacao_;
                }
            }

            for (unsigned int i = 0; i < 3; ++i)
            {
                if (inimigoAtivo_[i] &&
                    sapoNinja_.intersecta(inimigos_[i].getGlobalBounds()) &&
                    tempoDesdeDano_ >= 1.0f)
                {
                    --vidas_;
                    tempoDesdeDano_ = 0.0f;
                }
            }

            const bool teclaF =
                gerenciadorGrafico_.consumirPressionamentoF();
            if (teclaF && tempoRecargaShuriken_ >= 2.0f)
            {
                for (std::size_t i = 0; i < shurikens_.size(); ++i)
                {
                    if (!shurikens_[i].ativo)
                    {
                        shurikens_[i].ativo = true;
                        shurikens_[i].velocidade =
                            sapoNinja_.olhandoParaEsquerda() ? -480.0f : 480.0f;
                        const sf::FloatRect limitesSapo = sapoNinja_.getLimites();
                        shurikens_[i].sprite.setPosition(
                            limitesSapo.left +
                                (shurikens_[i].velocidade < 0.0f
                                     ? -8.0f
                                     : limitesSapo.width + 8.0f),
                            limitesSapo.top + limitesSapo.height / 2.0f
                        );
                        shurikens_[i].sprite.setRotation(0.0f);
                        tempoRecargaShuriken_ = 0.0f;
                        break;
                    }
                }
            }

            for (std::size_t i = 0; i < shurikens_.size(); ++i)
            {
                if (!shurikens_[i].ativo)
                {
                    continue;
                }
                sf::Vector2f posicao = shurikens_[i].sprite.getPosition();
                posicao.x += shurikens_[i].velocidade * tempoQuadro;
                shurikens_[i].sprite.setPosition(posicao);
                shurikens_[i].sprite.rotate(720.0f * tempoQuadro);
                if (posicao.x < -24.0f || posicao.x > 800.0f)
                {
                    shurikens_[i].ativo = false;
                    continue;
                }
                for (unsigned int j = 0; j < 3; ++j)
                {
                    if (inimigoAtivo_[j] &&
                        shurikens_[i].sprite.getGlobalBounds().intersects(
                            inimigos_[j].getGlobalBounds()))
                    {
                        inimigoAtivo_[j] = false;
                        inimigos_[j].setColor(sf::Color::Transparent);
                        shurikens_[i].ativo = false;
                        break;
                    }
                }
            }
        }

        gerenciadorGrafico_.limpar();
        gerenciadorGrafico_.desenhar(fundo_);
        gerenciadorGrafico_.desenharChao();
        gerenciadorGrafico_.desenhar(cabecaPedra_);
        for (unsigned int i = 0; i < 8; ++i)
        {
            if (moedas_[i].getRadius() > 0.0f)
            {
                gerenciadorGrafico_.desenhar(moedas_[i]);
            }
        }
        for (unsigned int i = 0; i < 3; ++i)
        {
            if (inimigoAtivo_[i])
            {
                gerenciadorGrafico_.desenhar(inimigos_[i]);
            }
        }
        for (std::size_t i = 0; i < shurikens_.size(); ++i)
        {
            if (shurikens_[i].ativo)
            {
                gerenciadorGrafico_.desenhar(shurikens_[i].sprite);
            }
        }
        sapoNinja_.desenhar(gerenciadorGrafico_);
        desenharHud();
        gerenciadorGrafico_.exibir();
    }
}
