#include "MeshCollider.h"
#include "../../Object.h"
#include "../Transform.h"
#include <cfloat>
#include <iostream>

Vec3<float> MeshCollider::findFurthestPoint(Vec3<float> direction, Vec3<float> translation)
{
  if (!transform)
  {
    transform = std::dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (!transform)
      throw std::runtime_error("MeshCollider::findFurthestPoint::Missing transform component");
  }

  float furthestDistance = -FLT_MAX;
  Vec3<float> furthestVertex;

  for (auto& vertex : transform->getMesh())
  {
    auto vert = vertex + translation;
    float distance = vert.dot(direction);

    if (distance > furthestDistance)
    {
      furthestDistance = distance;
      furthestVertex = vert;
    }
  }

  return furthestVertex;
}
