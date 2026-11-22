#ifndef JOGADOR_H
#define JOGADOR_H

#include "Entidades/Ente.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Gerenciador_Grafico;

class SapoNinja : public Ente
{
private:
    enum class Animacao
    {
        Parado,
        Correndo,
        Pulando,
        PuloDuplo,
        Caindo
    };

    sf::Texture texturaParado_;
    sf::Texture texturaCorrendo_;
    sf::Texture texturaPulando_;
    sf::Texture texturaPuloDuplo_;
    sf::Texture texturaCaindo_;
    sf::Sprite sprite_;
    Animacao animacaoAtual_;
    unsigned int quadroAnimacao_;
    float tempoAnimacao_;
    bool olhandoParaEsquerda_;
    bool tocouPuloDuplo_;

    void atualizarAnimacao(float tempoQuadro, bool movendoHorizontalmente);

public:
    SapoNinja(float posicaoX, float topoChao);
    void reiniciar(float posicaoX, float topoChao);
    void atualizar(float tempoQuadro, float larguraJanela, float topoChao);
    bool colideCom(const sf::FloatRect& limitesObstaculo);
    bool intersecta(const sf::FloatRect& limites) const;
    sf::FloatRect getLimites() const;
    bool olhandoParaEsquerda() const;
    void desenhar(Gerenciador_Grafico& gerenciadorGrafico) const;
};

#endif
