#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
using namespace sf;
int main()
{
    RenderWindow window(VideoMode(200, 200), "SFML works!");
    CircleShape shape(10.f);
    shape.setFillColor(Color::White);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(100.f, 160.f);
    RectangleShape ground(Vector2f(200.f, 20.f));
    ground.setPosition(0.f, 180.f);
    ground.setFillColor(Color(90, 65, 40));
    RectangleShape obstacle(Vector2f(24.f, 24.f));
    obstacle.setPosition(130.f, 156.f);
    obstacle.setFillColor(Color(180, 90, 45));
    Clock clock;

    while (window.isOpen())
    {
        const float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        Vector2f movement(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::W))
            movement.y -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::S))
            movement.y += 1.f;
        if (Keyboard::isKeyPressed(Keyboard::A))
            movement.x -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::D))
            movement.x += 1.f;

        if (movement.x != 0.f || movement.y != 0.f)
        {
            const float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= length;
            const Vector2f previousPosition = shape.getPosition();
            shape.move(movement * 150.f * deltaTime);

            const float radius = shape.getRadius();
            const FloatRect obstacleBounds = obstacle.getGlobalBounds();
            const Vector2f center = shape.getPosition();
            const float closestX = std::max(obstacleBounds.left, std::min(center.x, obstacleBounds.left + obstacleBounds.width));
            const float closestY = std::max(obstacleBounds.top, std::min(center.y, obstacleBounds.top + obstacleBounds.height));
            const float dx = center.x - closestX;
            const float dy = center.y - closestY;

            if (dx * dx + dy * dy < radius * radius)
            {
                if (movement.x != 0.f)
                {
                    obstacle.move(movement.x * 30.f, 0.f);
                    Vector2f obstaclePosition = obstacle.getPosition();
                    obstaclePosition.x = std::max(0.f, std::min(200.f - obstacle.getSize().x, obstaclePosition.x));
                    obstacle.setPosition(obstaclePosition);
                }

                const FloatRect movedBounds = obstacle.getGlobalBounds();
                const Vector2f movedCenter = shape.getPosition();
                const float movedClosestX = std::max(movedBounds.left, std::min(movedCenter.x, movedBounds.left + movedBounds.width));
                const float movedClosestY = std::max(movedBounds.top, std::min(movedCenter.y, movedBounds.top + movedBounds.height));
                const float movedDx = movedCenter.x - movedClosestX;
                const float movedDy = movedCenter.y - movedClosestY;
                if (movedDx * movedDx + movedDy * movedDy < radius * radius)
                    shape.setPosition(previousPosition);
            }
        }

        Vector2f position = shape.getPosition();
        position.x = std::max(shape.getRadius(), std::min(200.f - shape.getRadius(), position.x));
        position.y = std::max(shape.getRadius(), std::min(180.f - shape.getRadius(), position.y));
        shape.setPosition(position);

        window.clear(Color(35, 40, 55));
        window.draw(ground);
        window.draw(obstacle);
        window.draw(shape);
        window.display();
    }

    return 0;
}
