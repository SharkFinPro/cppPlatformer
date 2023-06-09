#include "SpriteRenderer.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"

SpriteRenderer::SpriteRenderer(sf::Color color)
    : Component{ComponentType::spriteRenderer}, color{color}
{}

void SpriteRenderer::update(float dt)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    sf::RectangleShape shape = sf::RectangleShape({transform->getWidth(), transform->getHeight()});
    shape.move({transform->getX(), transform->getY()});
    shape.setFillColor(color);

    getOwner()->getOwner()->getWindow()->draw(shape);
}

void SpriteRenderer::setColor(sf::Color color)
{
    this->color = color;
}
