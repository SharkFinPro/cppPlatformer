#include "BoxCollider.h"
#include "Transform.h"
#include "../GameObject.h"
#include <cmath>

BoxCollider::BoxCollider()
  : Component{ComponentType::boxCollider}, transform{nullptr}
{}

bool BoxCollider::collidesWith(BoundingRectangle r2)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return false;

  auto r1 = getBoundingRectangle(transform->getMesh());

  double mdX =  r1.left - r2.right;
  double mdY = r1.top - r2.bottom;
  double mdW = r1.width() + r2.width();
  double mdH = r1.height() + r2.height();

  return mdX <= 0 && mdX + mdW >= 0 && mdY <= 0 && mdY + mdH >= 0;
}

Vec2<double> BoxCollider::getPenetrationVector(const std::vector<GameObject *>& objects)
{
  Vec2<double> finalPenetrationVector = {0, 0};
  double xCollisions = 0;
  double yCollisions = 0;

  for (auto& object : objects)
  {
    auto penetrationVector = getPenetration(object);

    if (penetrationVector.getX() != 0)
    {
      xCollisions++;
      finalPenetrationVector.setX(finalPenetrationVector.getX() + penetrationVector.getX());
    }

    if (penetrationVector.getY() != 0)
    {
      yCollisions++;
      finalPenetrationVector.setY(finalPenetrationVector.getY() + penetrationVector.getY());
    }
  }

  if (finalPenetrationVector.getX() != 0 && finalPenetrationVector.getY() != 0)
  {
    if (std::fabs(finalPenetrationVector.getX()) > std::fabs(finalPenetrationVector.getY()))
    {
      auto theoreticalRectangle = getBoundingRectangle(transform->getMesh());
      theoreticalRectangle.left -= finalPenetrationVector.getX();
      theoreticalRectangle.right -= finalPenetrationVector.getX();

      finalPenetrationVector.setY(0);

      yCollisions = 0;
      for (auto& object : objects)
      {
        auto penetrationVector = getTheoreticalPenetration(theoreticalRectangle, object);

        if (penetrationVector.getY() != 0)
        {
          yCollisions++;
          finalPenetrationVector.setY(finalPenetrationVector.getY() + penetrationVector.getY());
        }
      }
    }
    else
    {
      auto theoreticalRectangle = getBoundingRectangle(transform->getMesh());
      theoreticalRectangle.top -= finalPenetrationVector.getY();
      theoreticalRectangle.bottom -= finalPenetrationVector.getY();

      finalPenetrationVector.setX(0);

      xCollisions = 0;
      for (auto& object : objects)
      {
        auto penetrationVector = getTheoreticalPenetration(theoreticalRectangle, object);

        if (penetrationVector.getX() != 0)
        {
          xCollisions++;
          finalPenetrationVector.setX(finalPenetrationVector.getX() + penetrationVector.getX());
        }
      }
    }
  }

  if (xCollisions != 0)
    finalPenetrationVector.setX(finalPenetrationVector.getX() / xCollisions);

  if (yCollisions != 0)
    finalPenetrationVector.setY(finalPenetrationVector.getY() / yCollisions);

  return finalPenetrationVector;
}

Vec2<double> BoxCollider::getPenetration(GameObject* object)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return { 0, 0 };

  auto otherTransform = dynamic_cast<Transform*>(object->getComponent(ComponentType::transform));

  if (!transform || !otherTransform)
    return { 0, 0 };

  auto r1 = getBoundingRectangle(transform->getMesh());
  auto r2 = getBoundingRectangle(otherTransform->getMesh());

  return getActualPenetration(r1, r2);
}

Vec2<double> BoxCollider::getTheoreticalPenetration(BoundingRectangle boundingRectangle, GameObject* object)
{
  auto otherTransform = dynamic_cast<Transform*>(object->getComponent(ComponentType::transform));

  if (!otherTransform)
    return { 0, 0 };

  auto r2 = getBoundingRectangle(otherTransform->getMesh());

  return getActualPenetration(boundingRectangle, r2);
}

Vec2<double> BoxCollider::getActualPenetration(BoundingRectangle r1, BoundingRectangle r2)
{
  double mdX =  r1.left - r2.right;
  double mdY = r1.top - r2.bottom;
  double mdW = r1.width() + r2.width();
  double mdH = r1.height() + r2.height();

  Vec2<double> min{mdX, mdY};
  Vec2<double> max{mdX + mdW, mdY + mdH};

  double minDist = std::fabs(min.getX());
  Vec2<double> penetrationVector{min.getX(), 0};

  if (std::fabs(max.getX()) < minDist)
  {
    minDist = std::fabs(max.getX());
    penetrationVector.setX(max.getX());
  }

  if (std::fabs(min.getY()) < minDist)
  {
    minDist = std::fabs(min.getY());
    penetrationVector.setX(0);
    penetrationVector.setY(min.getY());
  }

  if (std::fabs(max.getY()) < minDist)
  {
    penetrationVector.setX(0);
    penetrationVector.setY(max.getY());
  }

  return penetrationVector;
}

BoundingRectangle BoxCollider::getBoundingRectangle(const std::vector<Vec2<double>>& mesh)
{
  Vec2<double> bl{0, 0}, tr{0, 0};

  double maxDot = -10000;
  for (auto& m : mesh)
  {
    if (m.dot({-1, 1}) > maxDot)
    {
      maxDot = m.dot({-1, 1});
      bl = m;
    }
  }

  maxDot = -10000;
  for (auto& m : mesh)
  {
    if (m.dot({1, -1}) > maxDot)
    {
      maxDot = m.dot({1, -1});
      tr = m;
    }
  }

  return BoundingRectangle{bl.getX(), tr.getX(), tr.getY(), bl.getY()};
}