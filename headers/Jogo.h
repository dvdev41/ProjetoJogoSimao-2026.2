#ifndef JOGO_H
#define JOGO_H

#include "Gerenciadores/Gerenciador_Grafico.h"
#include "Entidades/Personagens/Jogador.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Clock.hpp>
#include <array>

#include <string>

class Jogo
{
private:
    enum class Tela
    {
        Menu,
        Fase1,
        Fase2
    };

    Gerenciador_Grafico gerenciadorGrafico_;
    SapoNinja sapoNinja_;
    sf::Texture texturaCabecaPedra_;
    sf::Sprite cabecaPedra_;
    sf::Texture texturaFundo_;
    sf::Sprite fundo_;
    sf::Texture texturaBotaoJogar_;
    sf::Sprite botaoJogar_;
    sf::Texture texturasFases_[2];
    sf::Sprite botoesFases_[2];
    sf::RectangleShape selecaoFase_;
    sf::Texture texturasInimigos_[3];
    sf::Sprite inimigos_[3];
    bool inimigoAtivo_[3];
    sf::Texture texturaShuriken_;
    sf::CircleShape moedas_[8];
    sf::ConvexShape coracoes_[5];
    struct Shuriken
    {
        sf::Sprite sprite;
        float velocidade;
        bool ativo;
    };
    std::array<Shuriken, 8> shurikens_;
    sf::Clock relogioRun_;
    float tempoRun_;
    float tempoDesdeDano_;
    float tempoRecargaShuriken_;
    unsigned int vidas_;
    unsigned int pontuacao_;
    bool runAtiva_;
    unsigned int faseSelecionada_;
    Tela telaAtual_;

    void carregarFundo(const std::string& caminho);
    void desenharMenu();
    void iniciarRun();
    void desenharHud();

public:
    Jogo();
    void executar();
};

#endif
