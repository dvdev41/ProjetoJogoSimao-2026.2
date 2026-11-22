#ifndef JOGADOR_H
#define JOGADOR_H

#include "Entidades/Personagens/Personagem.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>

namespace Gerenciadores
{
    class Gerenciador_Grafico;
}

namespace Entidades
{
    namespace Personagens
    {
        class Jogador : public Personagem
        {
        private:
            enum Animacao
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
            int pontos_;
            float tempoQuadroExecucao_;
            float larguraNivelExecucao_;
            const std::vector<sf::FloatRect>* pSolidosExecucao_;
            const std::vector<sf::FloatRect>* pPlataformasExecucao_;

            void atualizarAnimacao(float tempoQuadro, bool movendoHorizontalmente);
            void atualizar(
                float tempoQuadro,
                float larguraNivel,
                const std::vector<sf::FloatRect>& solidos,
                const std::vector<sf::FloatRect>& plataformas
            );

        public:
            Jogador(float posicaoX, float posicaoY);
            void reiniciar(float posicaoX, float posicaoY);
            void configurarExecucao(
                float tempoQuadro,
                float larguraNivel,
                const std::vector<sf::FloatRect>& solidos,
                const std::vector<sf::FloatRect>& plataformas
            );
            virtual void mover();
            virtual void executar();
            virtual void salvar();
            virtual sf::FloatRect getLimites() const;
            sf::Vector2f getPosicao() const;
            bool olhandoParaEsquerda() const;
            int getPontos() const;
            void adicionarPontos(int pontos);
            void setPontos(int pontos);
            void receberDano();
        };
    }
}

#endif
