#include "Entidades/Personagens/Jogador.h"

#include "Gerenciadores/Gerenciador_Grafico.h"

#include <SFML/Window/Keyboard.hpp>

#include <stdexcept>

namespace Entidades
{
    namespace Personagens
    {
        namespace
        {
            const float VELOCIDADE_HORIZONTAL = 260.0f;
            const float ACELERACAO_GRAVIDADE = 1350.0f;
            const float ESCALA_SPRITE = 2.0f;
            const float DURACAO_QUADRO = 0.07f;
            const int TAMANHO_QUADRO = 32;
        }

        Jogador::Jogador(float posicaoX, float posicaoY)
            : Personagem(posicaoX, posicaoY, 3),
              animacaoAtual_(Parado),
              quadroAnimacao_(0),
              tempoAnimacao_(0.0f),
              olhandoParaEsquerda_(false),
              tocouPuloDuplo_(false),
              pontos_(0),
              tempoQuadroExecucao_(0.0f),
              larguraNivelExecucao_(0.0f),
              pSolidosExecucao_(0),
              pPlataformasExecucao_(0)
        {
            const std::string pasta =
                "assets/Pixel Adventure 1/Free/Main Characters/Ninja Frog/";
            pGG_->carregarTextura(texturaParado_, pasta + "Idle (32x32).png");
            pGG_->carregarTextura(texturaCorrendo_, pasta + "Run (32x32).png");
            pGG_->carregarTextura(texturaPulando_, pasta + "Jump (32x32).png");
            pGG_->carregarTextura(
                texturaPuloDuplo_,
                pasta + "Double Jump (32x32).png"
            );
            pGG_->carregarTextura(texturaCaindo_, pasta + "Fall (32x32).png");

            sprite_.setTexture(texturaParado_);
            sprite_.setTextureRect(sf::IntRect(0, 0, TAMANHO_QUADRO, TAMANHO_QUADRO));
            sprite_.setScale(ESCALA_SPRITE, ESCALA_SPRITE);
            sprite_.setPosition(posicaoX, posicaoY);
            setFigura(&sprite_);
        }

        void Jogador::reiniciar(float posicaoX, float posicaoY)
        {
            velocidadeVertical_ = 0.0f;
            estaNoChao_ = true;
            teclaPuloPressionadaAnteriormente_ = false;
            pulosDisponiveis_ = 2;
            olhandoParaEsquerda_ = false;
            tocouPuloDuplo_ = false;
            animacaoAtual_ = Parado;
            quadroAnimacao_ = 0;
            tempoAnimacao_ = 0.0f;
            sprite_.setTexture(texturaParado_, true);
            sprite_.setTextureRect(sf::IntRect(0, 0, TAMANHO_QUADRO, TAMANHO_QUADRO));
            sprite_.setScale(ESCALA_SPRITE, ESCALA_SPRITE);
            sprite_.setPosition(posicaoX, posicaoY);
            x_ = posicaoX;
            y_ = posicaoY;
        }

        void Jogador::configurarExecucao(
            float tempoQuadro,
            float larguraNivel,
            const std::vector<sf::FloatRect>& solidos,
            const std::vector<sf::FloatRect>& plataformas
        )
        {
            tempoQuadroExecucao_ = tempoQuadro;
            larguraNivelExecucao_ = larguraNivel;
            pSolidosExecucao_ = &solidos;
            pPlataformasExecucao_ = &plataformas;
        }

        void Jogador::mover()
        {
            if (pSolidosExecucao_ == 0 || pPlataformasExecucao_ == 0)
            {
                throw std::logic_error(
                    "Contexto de movimento do jogador nao configurado."
                );
            }
            atualizar(
                tempoQuadroExecucao_,
                larguraNivelExecucao_,
                *pSolidosExecucao_,
                *pPlataformasExecucao_
            );
        }

        void Jogador::executar()
        {
            mover();
        }

        void Jogador::salvar()
        {
            salvarDataBuffer();
            buffer_ << id_ << ' ' << x_ << ' ' << y_ << ' '
                    << num_vidas_ << ' ' << pontos_ << ' '
                    << velocidadeVertical_ << ' ' << pulosDisponiveis_;
        }

        void Jogador::atualizar(
            float tempoQuadro,
            float larguraNivel,
            const std::vector<sf::FloatRect>& solidos,
            const std::vector<sf::FloatRect>& plataformas
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
            const sf::FloatRect limitesAntes = sprite_.getGlobalBounds();
            const float larguraJogador = limitesAntes.width;
            const float alturaJogador = limitesAntes.height;
            posicao.x += direcaoHorizontal * VELOCIDADE_HORIZONTAL * tempoQuadro;
            if (direcaoHorizontal != 0.0f)
            {
                olhandoParaEsquerda_ = direcaoHorizontal < 0.0f;
            }

            std::size_t i;
            for (i = 0; i < solidos.size(); ++i)
            {
                const sf::FloatRect limitesMovidos(
                    posicao.x,
                    posicao.y,
                    larguraJogador,
                    alturaJogador
                );
                if (!limitesMovidos.intersects(solidos[i]))
                {
                    continue;
                }
                if (direcaoHorizontal > 0.0f)
                {
                    posicao.x = solidos[i].left - larguraJogador;
                }
                else if (direcaoHorizontal < 0.0f)
                {
                    posicao.x = solidos[i].left + solidos[i].width;
                }
            }

            const float limiteDireito =
                larguraNivel > larguraJogador ? larguraNivel - larguraJogador : 0.0f;
            if (posicao.x < 0.0f)
            {
                posicao.x = 0.0f;
            }
            else if (posicao.x > limiteDireito)
            {
                posicao.x = limiteDireito;
            }

            const bool teclaPuloPressionada =
                sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            if (pularSeSolicitado(teclaPuloPressionada))
            {
                tocouPuloDuplo_ = true;
            }

            const float posicaoYAnterior = posicao.y;
            const float baseAnterior = posicaoYAnterior + alturaJogador;
            velocidadeVertical_ += ACELERACAO_GRAVIDADE * tempoQuadro;
            float proximaPosicaoY = posicao.y + velocidadeVertical_ * tempoQuadro;
            estaNoChao_ = false;

            if (velocidadeVertical_ >= 0.0f)
            {
                float topoColisao = 0.0f;
                bool encontrouPiso = false;
                for (i = 0; i < solidos.size(); ++i)
                {
                    const bool sobrepoeHorizontalmente =
                        posicao.x + larguraJogador > solidos[i].left &&
                        posicao.x < solidos[i].left + solidos[i].width;
                    if (sobrepoeHorizontalmente &&
                        baseAnterior <= solidos[i].top + 2.0f &&
                        proximaPosicaoY + alturaJogador >= solidos[i].top &&
                        (!encontrouPiso || solidos[i].top < topoColisao))
                    {
                        topoColisao = solidos[i].top;
                        encontrouPiso = true;
                    }
                }
                for (i = 0; i < plataformas.size(); ++i)
                {
                    const bool sobrepoeHorizontalmente =
                        posicao.x + larguraJogador > plataformas[i].left &&
                        posicao.x < plataformas[i].left + plataformas[i].width;
                    if (sobrepoeHorizontalmente &&
                        baseAnterior <= plataformas[i].top + 2.0f &&
                        proximaPosicaoY + alturaJogador >= plataformas[i].top &&
                        (!encontrouPiso || plataformas[i].top < topoColisao))
                    {
                        topoColisao = plataformas[i].top;
                        encontrouPiso = true;
                    }
                }
                if (encontrouPiso)
                {
                    proximaPosicaoY = topoColisao - alturaJogador;
                    velocidadeVertical_ = 0.0f;
                    estaNoChao_ = true;
                    pulosDisponiveis_ = 2;
                    tocouPuloDuplo_ = false;
                }
            }
            else
            {
                for (i = 0; i < solidos.size(); ++i)
                {
                    const float baseSolido = solidos[i].top + solidos[i].height;
                    const bool sobrepoeHorizontalmente =
                        posicao.x + larguraJogador > solidos[i].left &&
                        posicao.x < solidos[i].left + solidos[i].width;
                    if (sobrepoeHorizontalmente &&
                        posicaoYAnterior >= baseSolido - 2.0f &&
                        proximaPosicaoY <= baseSolido)
                    {
                        proximaPosicaoY = baseSolido;
                        velocidadeVertical_ = 0.0f;
                        break;
                    }
                }
            }

            if (proximaPosicaoY < 0.0f)
            {
                proximaPosicaoY = 0.0f;
                if (velocidadeVertical_ < 0.0f)
                {
                    velocidadeVertical_ = 0.0f;
                }
            }
            posicao.y = proximaPosicaoY;
            sprite_.setPosition(posicao);
            x_ = posicao.x;
            y_ = posicao.y;
            atualizarAnimacao(tempoQuadro, direcaoHorizontal != 0.0f);
        }

        void Jogador::atualizarAnimacao(
            float tempoQuadro,
            bool movendoHorizontalmente
        )
        {
            if (estaNoChao_)
            {
                tocouPuloDuplo_ = false;
            }

            Animacao proximaAnimacao;
            const sf::Texture* textura;
            unsigned int quantidadeQuadros;

            if (!estaNoChao_ && tocouPuloDuplo_)
            {
                proximaAnimacao = PuloDuplo;
                textura = &texturaPuloDuplo_;
                quantidadeQuadros = 6;
            }
            else if (!estaNoChao_)
            {
                proximaAnimacao =
                    velocidadeVertical_ < 0.0f ? Pulando : Caindo;
                textura =
                    velocidadeVertical_ < 0.0f ? &texturaPulando_ : &texturaCaindo_;
                quantidadeQuadros = 1;
            }
            else if (movendoHorizontalmente)
            {
                proximaAnimacao = Correndo;
                textura = &texturaCorrendo_;
                quantidadeQuadros = 12;
            }
            else
            {
                proximaAnimacao = Parado;
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

            const int esquerda =
                static_cast<int>(quadroAnimacao_) * TAMANHO_QUADRO;
            sprite_.setTextureRect(
                olhandoParaEsquerda_
                    ? sf::IntRect(
                          esquerda + TAMANHO_QUADRO,
                          0,
                          -TAMANHO_QUADRO,
                          TAMANHO_QUADRO
                      )
                    : sf::IntRect(esquerda, 0, TAMANHO_QUADRO, TAMANHO_QUADRO)
            );
        }

        sf::FloatRect Jogador::getLimites() const
        {
            return sprite_.getGlobalBounds();
        }

        sf::Vector2f Jogador::getPosicao() const
        {
            return sprite_.getPosition();
        }

        bool Jogador::olhandoParaEsquerda() const
        {
            return olhandoParaEsquerda_;
        }

        int Jogador::getPontos() const
        {
            return pontos_;
        }

        void Jogador::adicionarPontos(int pontos)
        {
            pontos_ += pontos;
        }

        void Jogador::setPontos(int pontos)
        {
            pontos_ = pontos;
        }

        void Jogador::receberDano()
        {
            if (num_vidas_ > 0)
            {
                --num_vidas_;
            }
        }
    }
}
