#include "Object.h"
#include "components/Component.h"

void Object::update(float dt)
{
  for (auto [componentType, component] : components)
    component->update(dt);
}

void Object::fixedUpdate(float dt)
{
  for (auto [componentType, component] : components)
    component->fixedUpdate(dt);
}

void Object::addComponent(std::shared_ptr<Component> component)
{
  component->setOwner(this);
  components.insert({ component->getType(), std::move(component) });
}

std::shared_ptr<Component> Object::getComponent(const ComponentType type) const
{
  for (auto [componentType, component] : components)
    if (componentType == type)
      return component;

  return nullptr;
}

void Object::setOwner(ObjectManager* objectOwner)
{
  owner = objectOwner;
}

ObjectManager* Object::getOwner() const
{
  return owner;
}
