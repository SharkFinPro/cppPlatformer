#include "RigidBody.h"
#include "../GameObject.h"
#include "Transform.h"

RigidBody::RigidBody()
    : Component{ComponentType::rigidBody}, velocity{0, 0}, doGravity{true}, gravity{0.01f}, falling{true}, transform{nullptr}
{}

void RigidBody::fixedUpdate(float dt)
{
    if (!transform)
        transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    falling = true;

    if (doGravity)
        velocity.setY(velocity.getY() + gravity);

    limitMovement();

    transform->move(velocity.getX() * dt, velocity.getY() * dt);
}

void RigidBody::applyForce(float x, float y)
{
    velocity.setX(velocity.getX() + x);
    velocity.setY(velocity.getY() + y);
}

void RigidBody::limitMovement()
{
    velocity.setX(velocity.getX() / 1.5f);
}

bool RigidBody::isFalling() const
{
    return falling;
}

void RigidBody::handleCollision(Vec2<float> penetrationVector)
{
    if (!transform)
        transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    if (penetrationVector.getX() != 0)
        handleXCollision();

    if (penetrationVector.getY() != 0)
        handleYCollision();

    transform->move(-penetrationVector.getX(), -penetrationVector.getY());
}

void RigidBody::handleXCollision()
{
    velocity.setX(0);
}

void RigidBody::handleYCollision()
{
    if (velocity.getY() > 0)
        falling = false;

    velocity.setY(0);
}
