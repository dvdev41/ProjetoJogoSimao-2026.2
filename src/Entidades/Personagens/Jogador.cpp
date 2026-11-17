#include "Entidades/Personagens/Jogador.h"

#include "Gerenciadores/Gerenciador_Grafico.h"

#include <SFML/Window/Keyboard.hpp>
#include <stdexcept>
#include <string>

namespace
{
    const float VELOCIDADE_HORIZONTAL = 280.0f;
    const float ESCALA_SPRITE = 2.0f;
    const float DURACAO_QUADRO = 0.05f;
    const int TAMANHO_QUADRO = 32;

    void carregarTextura(sf::Texture& textura, const std::string& caminho)
    {
        if (!textura.loadFromFile(caminho))
        {
            throw std::runtime_error(
                "Nao foi possivel carregar o asset: " + caminho
            );
        }
    }

    float limitarPosicaoHorizontal(
        float posicaoX,
        float larguraSapo,
        float larguraJanela
    )
    {
        const float limiteDireito =
            larguraJanela > larguraSapo
                ? larguraJanela - larguraSapo
                : 0.0f;

        if (posicaoX < 0.0f)
        {
            return 0.0f;
        }
        if (posicaoX > limiteDireito)
        {
            return limiteDireito;
        }
        return posicaoX;
    }
}

SapoNinja::SapoNinja(float posicaoX, float topoChao)
    : animacaoAtual_(Animacao::Parado),
      quadroAnimacao_(0),
      tempoAnimacao_(0.0f),
      olhandoParaEsquerda_(false)
{
    const std::string pasta =
        "assets/Pixel Adventure 1/Free/Main Characters/Ninja Frog/";
    carregarTextura(texturaParado_, pasta + "Idle (32x32).png");
    carregarTextura(texturaCorrendo_, pasta + "Run (32x32).png");
    carregarTextura(texturaPulando_, pasta + "Jump (32x32).png");
    carregarTextura(texturaCaindo_, pasta + "Fall (32x32).png");

    sprite_.setTexture(texturaParado_);
    sprite_.setTextureRect(sf::IntRect(0, 0, TAMANHO_QUADRO, TAMANHO_QUADRO));
    sprite_.setScale(ESCALA_SPRITE, ESCALA_SPRITE);
    sprite_.setPosition(
        posicaoX,
        topoChao - sprite_.getGlobalBounds().height
    );
}

void SapoNinja::atualizar(
    float tempoQuadro,
    float larguraJanela,
    float topoChao
)
{
    float direcaoHorizontal = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        direcaoHorizontal -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        direcaoHorizontal += 1.0f;
    }

    sf::Vector2f posicao = sprite_.getPosition();
    posicao.x += direcaoHorizontal * VELOCIDADE_HORIZONTAL * tempoQuadro;
    if (direcaoHorizontal != 0.0f)
    {
        olhandoParaEsquerda_ = direcaoHorizontal < 0.0f;
    }

    posicao.x = limitarPosicaoHorizontal(
        posicao.x,
        sprite_.getGlobalBounds().width,
        larguraJanela
    );

    const bool teclaPuloPressionada =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    pularSeSolicitado(teclaPuloPressionada);
    atualizarGravidade(
        tempoQuadro,
        posicao.y,
        sprite_.getGlobalBounds().height,
        topoChao
    );

    sprite_.setPosition(posicao);
    atualizarAnimacao(tempoQuadro, direcaoHorizontal != 0.0f);
}

void SapoNinja::atualizarAnimacao(
    float tempoQuadro,
    bool movendoHorizontalmente
)
{
    Animacao proximaAnimacao;
    const sf::Texture* textura;
    unsigned int quantidadeQuadros;

    if (!estaNoChao_)
    {
        proximaAnimacao =
            velocidadeVertical_ < 0.0f ? Animacao::Pulando : Animacao::Caindo;
        textura =
            velocidadeVertical_ < 0.0f ? &texturaPulando_ : &texturaCaindo_;
        quantidadeQuadros = 1;
    }
    else if (movendoHorizontalmente)
    {
        proximaAnimacao = Animacao::Correndo;
        textura = &texturaCorrendo_;
        quantidadeQuadros = 12;
    }
    else
    {
        proximaAnimacao = Animacao::Parado;
        textura = &texturaParado_;
        quantidadeQuadros = 11;
    }

    if (proximaAnimacao != animacaoAtual_)
    {
        animacaoAtual_ = proximaAnimacao;
        quadroAnimacao_ = 0;
        tempoAnimacao_ = 0.0f;
        sprite_.setTexture(*textura, true);
    }
    else if (quantidadeQuadros > 1)
    {
        tempoAnimacao_ += tempoQuadro;
        while (tempoAnimacao_ >= DURACAO_QUADRO)
        {
            quadroAnimacao_ =
                (quadroAnimacao_ + 1) % quantidadeQuadros;
            tempoAnimacao_ -= DURACAO_QUADRO;
        }
    }

    const int esquerda = static_cast<int>(quadroAnimacao_) * TAMANHO_QUADRO;
    sprite_.setTextureRect(
        olhandoParaEsquerda_
            ? sf::IntRect(esquerda + TAMANHO_QUADRO, 0, -TAMANHO_QUADRO,
                          TAMANHO_QUADRO)
            : sf::IntRect(esquerda, 0, TAMANHO_QUADRO, TAMANHO_QUADRO)
    );
}

void SapoNinja::colideCom(const sf::FloatRect& limitesObstaculo)
{
    const sf::FloatRect limitesSapo = sprite_.getGlobalBounds();
    sf::FloatRect intersecao;
    if (!limitesSapo.intersects(limitesObstaculo, intersecao))
    {
        return;
    }

    sf::Vector2f posicao = sprite_.getPosition();
    posicao.x += limitesSapo.left + limitesSapo.width / 2.0f <
                         limitesObstaculo.left + limitesObstaculo.width / 2.0f
                     ? -intersecao.width
                     : intersecao.width;
    sprite_.setPosition(posicao);
}

void SapoNinja::desenhar(Gerenciador_Grafico& gerenciadorGrafico) const
{
    gerenciadorGrafico.desenhar(sprite_);
}
