#ifndef FASE1_H
#define FASE1_H

#include "Entidades/Personagens/Jogador.h"
#include "Fases/Fase.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

namespace Fases
{
    class Fase1 : public Fase
    {
    private:
        enum Estado
        {
            Introducao,
            Jogando,
            Pausada,
            Concluida,
            GameOver
        };

        struct Caixa
        {
            sf::Sprite sprite;
            unsigned int tipo;
            unsigned int quadro;
            float tempoAnimacao;
            bool ativa;
            bool quebrando;
        };

        struct Fruta
        {
            sf::Sprite sprite;
            unsigned int tipo;
            unsigned int quadro;
            float tempoAnimacao;
            bool ativa;
        };

        struct Perigo
        {
            sf::Sprite sprite;
            sf::FloatRect limites;
            unsigned int quadro;
            unsigned int quantidadeQuadros;
            int larguraQuadro;
            int alturaQuadro;
            float tempoAnimacao;
            float duracaoQuadro;
        };

        struct Shuriken
        {
            sf::Sprite sprite;
            float velocidadeX;
            float tempoAnimacao;
            unsigned int quadro;
            bool ativo;
        };

        Entidades::Personagens::Jogador sapoNinja_;
        Estado estado_;

    sf::Texture texturaFundo_;
    sf::Texture texturaTerreno_;
    sf::Texture texturaFonte_;
    sf::Texture texturaBotaoJogar_;
    sf::Texture texturaInicio_;
    sf::Texture texturaCheckpoint_;
    sf::Texture texturaCheckpointAtivo_;
    sf::Texture texturaSaida_;
    sf::Texture texturaSaidaAtiva_;
    sf::Texture texturaShuriken_;
    sf::Texture texturaFogo_;
    sf::Texture texturaSerra_;
    sf::Texture texturaEspinhos_;
    sf::Texture texturasCaixas_[3];
    sf::Texture texturasQuebraCaixas_[3];
    sf::Texture texturasFrutas_[8];

    sf::Sprite fundo_;
    sf::Sprite botaoJogar_;
    sf::Sprite inicio_;
    sf::Sprite checkpoint_;
    sf::Sprite saida_;
    sf::View vistaMundo_;
    sf::RectangleShape barraHud_;

    std::vector<sf::Sprite> tiles_;
    std::vector<sf::FloatRect> solidosEstaticos_;
    std::vector<sf::FloatRect> plataformasAereas_;
    std::vector<Caixa> caixas_;
    std::vector<Fruta> frutas_;
    std::vector<Perigo> perigos_;
    Shuriken shurikens_[6];

    sf::Vector2f pontoInicial_;
    sf::Vector2f pontoCheckpoint_;
    float larguraNivel_;
    float cameraX_;
    float tempoInvulneravel_;
    float tempoDesdeDisparo_;
    float tempoInicio_;
    float tempoCheckpoint_;
    float tempoSaida_;
    unsigned int quadroInicio_;
    unsigned int quadroCheckpoint_;
    unsigned int quadroSaida_;
    unsigned int frutasColetadas_;
    bool checkpointAtivo_;

    void carregarTextura(sf::Texture& textura, const std::string& caminho);
    void carregarAssets();
    void construirFase();
    void adicionarTile(
        unsigned int colunaAtlas,
        unsigned int linhaAtlas,
        float x,
        float y
    );
    void adicionarChao(
        unsigned int inicio,
        unsigned int fim,
        unsigned int linha
    );
    void adicionarPlataforma(
        unsigned int inicio,
        unsigned int fim,
        unsigned int linha
    );
    void adicionarCaixa(unsigned int coluna, unsigned int linha, unsigned int tipo);
    void adicionarFruta(
        unsigned int coluna,
        unsigned int linha,
        unsigned int tipo
    );
    void adicionarPerigo(
        const sf::Texture& textura,
        float x,
        float y,
        int larguraQuadro,
        int alturaQuadro,
        unsigned int quantidadeQuadros,
        float escala,
        float duracaoQuadro
    );

    void reiniciarFase();
    void atualizarAnimacoes(float tempoQuadro);
    void atualizarCaixas(float tempoQuadro);
    void atualizarFrutas(float tempoQuadro);
    void atualizarPerigos(float tempoQuadro);
    void atualizarShurikens(float tempoQuadro);
    void atualizarCamera();
    void dispararShuriken();
    void quebrarCaixa(Caixa& caixa);
    void receberDano();

    void desenharMundo();
    void desenharHud();
    void desenharOverlay(
        const std::string& titulo,
        const std::string& linha1,
        const std::string& linha2
    );
    void desenharTexto(
        const std::string& texto,
        float x,
        float y,
        float escala,
        const sf::Color& cor
    );
    void desenharTextoCentralizado(
        const std::string& texto,
        float y,
        float escala,
        const sf::Color& cor
    );

    public:
        explicit Fase1(Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico);
        virtual void executar();
        void atualizar(float tempoQuadro);
        virtual void desenhar();
    };
}

#endif
