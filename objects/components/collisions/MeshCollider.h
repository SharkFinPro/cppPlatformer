#ifndef PLATFORMER_MESHCOLLIDER_H
#define PLATFORMER_MESHCOLLIDER_H

#include "Collider.h"

class MeshCollider : public Collider
{
private:
  Vec3<float> findFurthestPoint(const Vec3<float>& direction) override;
};


#endif //PLATFORMER_MESHCOLLIDER_H
