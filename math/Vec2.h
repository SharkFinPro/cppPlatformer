#ifndef PLATFORMER_VEC2_H
#define PLATFORMER_VEC2_H

#include <cmath>

template<typename T>
class Vec2
{
public:
  Vec2();
  Vec2(T x, T y);

  T getX() const;
  T getY() const;

  void setX(T x);
  void setY(T y);

  T dot(Vec2<T> other) const;

  T length() const;

  Vec2<T> operator+(Vec2<T> const& other) const;
  Vec2<T> operator-(Vec2<T> const& other) const;
  Vec2<T> operator*(float scalar) const;

  Vec2<T> operator+=(Vec2<T> const& other);
  Vec2<T> operator-=(Vec2<T> const& other);
  Vec2<T> operator*=(float scalar);

private:
  T x;
  T y;
};

template<typename T>
Vec2<T>::Vec2()
  : x{0}, y{0}
{}

template<typename T>
Vec2<T>::Vec2(T x_, T y_)
  : x{x_}, y{y_}
{}

template<typename T>
T Vec2<T>::getX() const
{
  return x;
}

template<typename T>
T Vec2<T>::getY() const
{
  return y;
}

template<typename T>
void Vec2<T>::setX(T x_)
{
  x = x_;
}

template<typename T>
void Vec2<T>::setY(T y_)
{
  y = y_;
}

template<typename T>
T Vec2<T>::dot(Vec2<T> other) const
{
  return x * other.x + y * other.y;
}

template<typename T>
T Vec2<T>::length() const
{
  return std::sqrt(dot(this));
}

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& other) const
{
  return { x + other.x, y + other.y };
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& other) const
{
  return { x - other.x, y - other.y };
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const float scalar) const
{
  return { x * scalar, y * scalar };
}

template<typename T>
Vec2<T> Vec2<T>::operator+=(const Vec2<T>& other)
{
  auto newVector = *this + other;
  x = newVector.x;
  y = newVector.y;

  return *this;
}

template<typename T>
Vec2<T> Vec2<T>::operator-=(const Vec2<T>& other)
{
  auto newVector = *this - other;
  x = newVector.x;
  y = newVector.y;

  return *this;
}

template<typename T>
Vec2<T> Vec2<T>::operator*=(float scalar)
{
  auto newVector = *this * scalar;
  x = newVector.x;
  y = newVector.y;

  return *this;
}

#endif //PLATFORMER_VEC2_H
