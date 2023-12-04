#include <iostream>
#include "Collider.h"
#include "../GameObject.h"
#include "Transform.h"

#include "../GameObjectManager.h"

Collider::Collider(bool debug)
  : Component{ComponentType::collider}, debug{debug}
{}

bool Collider::collidesWith(GameObject* other)
{
  auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));
  if (!transform)
    return false;

  auto otherTransform = dynamic_cast<Transform*>(other->getComponent(ComponentType::transform));
  if (!otherTransform)
    return false;

  auto otherCollider = dynamic_cast<Collider*>(other->getComponent(ComponentType::collider));
  if (!otherCollider)
    return false;

  std::vector<Vec3<float>> simplex;
  auto d_ = transform->getPosition() - otherTransform->getPosition();
  Vec3<float> direction = { d_.getX(), d_.getY(), 0 };

  Vec3<float> support = getSupport(this, otherCollider, direction);

  simplex.insert(simplex.begin(), support);

  direction = support * -1.0f;

  int iterations = 0;
  while (true)
  {
    iterations++;
    if (iterations > 50)
      return false;

    support = getSupport(this, otherCollider, direction);


    if (support.dot(direction) < 0)
      return false;

    simplex.insert(simplex.begin(), support);

    if (debug)
    {
      sf::ConvexShape shape;
      shape.setPointCount(simplex.size());
      for (int i = 0; i < simplex.size(); i++)
      {
        shape.setPoint(i, sf::Vector2f(simplex[i].getX() + 500, simplex[i].getY() + 500));
      }
      shape.setFillColor(sf::Color::Blue);

      getOwner()->getOwner()->getWindow()->draw(shape);

      sf::CircleShape origin;
      origin.setPosition(500, 500);
      origin.setRadius(3);
      origin.setFillColor(sf::Color::Red);
      getOwner()->getOwner()->getWindow()->draw(origin);
    }

    if (nextSimplex(simplex, direction))
      return true;
  }
}

Vec3<float> Collider::getSupport(Collider* a, Collider* b, Vec3<float> direction)
{
  return a->findFurthestPoint(direction) - b->findFurthestPoint(direction * -1.0f);
}

bool Collider::nextSimplex(std::vector<Vec3<float>>& simplex, Vec3<float>& direction) {
  if (simplex.size() == 2)
    return line(simplex, direction);
  else if (simplex.size() == 3)
    return triangle(simplex, direction);

  return false;
}

bool Collider::line(std::vector<Vec3<float>>& simplex, Vec3<float>& direction)
{
  auto a = simplex[0];
  auto b = simplex[1];

  auto ab = b - a;
  auto ao = a * -1.0f;

  direction = ab.cross(ao).cross(ab);

  return false;
}

bool Collider::triangle(std::vector<Vec3<float>>& simplex, Vec3<float>& direction)
{
  auto a = simplex[0];
  auto b = simplex[1];
  auto c = simplex[2];

  auto ab = b - a;
  auto ac = c - a;
  auto ao = a * -1.0f;

  auto ABperp = ac.cross(ab).cross(ab);
  auto ACperp = ab.cross(ac).cross(ac);

  if (ABperp.dot(ao) > 0)
  {
    simplex.pop_back();
    direction = ABperp;
    return false;
  }

  if (ACperp.dot(ao) > 0)
  {
    simplex.erase(simplex.begin() + 1);
    direction = ACperp;
    return false;
  }

  return true;
}