#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

#include <cmath>
#include <iostream>
#include <immintrin.h>
#include <cassert>

namespace Temp::Math
{
  struct Vec2
  {
    union
    {
      struct
      {
        float x;
        float y;
      };
      __m128 simdData;
    };

    Vec2();
    Vec2(float x, float y);
    Vec2(__m128 data);

    Vec2 operator+(const Vec2 &other) const;
    Vec2 operator-(const Vec2 &other) const;
    Vec2 operator*(const Vec2 &other) const;
    Vec2 operator/(const Vec2 &other) const;
    Vec2 operator+(float scalar) const;
    Vec2 operator-(float scalar) const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    float dot(const Vec2 &other) const;
    float magnitude() const;
    float magnitude2() const;
    Vec2 normalize() const;
  };

  struct Vec3
  {
    union
    {
      struct
      {
        float x;
        float y;
        float z;
      };
      __m128 simdData;
    };

    Vec3();
    Vec3(float x, float y, float z);
    Vec3(__m128 data);

    Vec3 operator+(const Vec3 &other) const;
    Vec3 operator-(const Vec3 &other) const;
    Vec3 operator*(const Vec3 &other) const;
    Vec3 operator/(const Vec3 &other) const;
    Vec3 operator+(float scalar) const;
    Vec3 operator-(float scalar) const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;

    float dot(const Vec3 &other) const;
    Vec3 cross(const Vec3 &other) const;
    float magnitude() const;
    float magnitude2() const;
    Vec3 normalize() const;
  };

  struct Vec4
  {
    union
    {
      struct
      {
        float x;
        float y;
        float z;
        float w;
      };
      __m128 simdData;
    };

    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4(__m128 data);

    Vec4 operator+(const Vec4 &other) const;
    Vec4 operator-(const Vec4 &other) const;
    Vec4 operator*(const Vec4 &other) const;
    Vec4 operator/(const Vec4 &other) const;
    Vec4 operator+(float scalar) const;
    Vec4 operator-(float scalar) const;
    Vec4 operator*(float scalar) const;
    Vec4 operator/(float scalar) const;

    float dot(const Vec4 &other) const;
    float magnitude() const;
    float magnitude2() const;
    Vec4 normalize() const;
  };

  struct Mat2
  {
    Vec2 rows[2];

    Mat2();
    Mat2(const Vec2 &col1, const Vec2 &col2);

    Mat2 operator+(const Mat2 &other) const;
    Mat2 operator-(const Mat2 &other) const;
    Mat2 operator*(const Mat2 &other) const;
    Mat2 operator*(float scalar) const;
    Vec2 operator*(const Vec2 &vec) const;

    Mat2 transpose() const;
    float determinant() const;
    Mat2 inverse() const;
  };

  // 3x3 matrix struct
  struct Mat3
  {
    Vec3 rows[3];

    Mat3();

    Mat3(const Vec3 &col1, const Vec3 &col2, const Vec3 &col3);

    Mat3 operator+(const Mat3 &other) const;
    Mat3 operator-(const Mat3 &other) const;
    Mat3 operator*(const Mat3 &other) const;
    Mat3 operator*(float scalar) const;
    Vec3 operator*(const Vec3 &vec) const;

    Mat3 transpose() const;
    float determinant() const;
    Mat3 inverse() const;
  };

  struct Mat4
  {
    Vec4 rows[4];

    Mat4();
    Mat4(const Vec4 &col1, const Vec4 &col2, const Vec4 &col3, const Vec4 &col4);

    Mat4 operator+(const Mat4 &other) const;
    Mat4 operator-(const Mat4 &other) const;
    Mat4 operator*(const Mat4 &other) const;
    Mat4 operator*(float scalar) const;
    Vec4 operator*(const Vec4 &vec) const;

    Mat4 transpose() const;
    float determinant() const;
    Mat4 inverse() const;

    // static Mat4 identity();
    // static Mat4 translate(const Vec3 &translation);
    // static Mat4 scale(const Vec3 &scale);
    // static Mat4 rotateX(float angle);
    // static Mat4 rotateY(float angle);
    // static Mat4 rotateZ(float angle);
    // static Mat4 perspective(float fov, float aspectRatio, float near, float far);
    // static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
  };

  namespace UnitTests
  {
    // Function to run the unit tests
    void Run();
  }
}

#pragma GCC diagnostic pop
