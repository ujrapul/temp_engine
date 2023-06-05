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

    inline Vec2() : x(0.0f), y(0.0f) {}
    inline Vec2(float x, float y) : x(x), y(y) {}
    inline Vec2(__m128 data) : simdData(data) {}

    inline Vec2 operator+(const Vec2 &other) const
    {
      return _mm_add_ps(simdData, other.simdData);
    }

    // Vector-vector subtraction
    inline Vec2 operator-(const Vec2 &other) const
    {
      return _mm_sub_ps(simdData, other.simdData);
    }

    // Vector-vector multiplication
    inline Vec2 operator*(const Vec2 &other) const
    {
      return _mm_mul_ps(simdData, other.simdData);
    }

    // Vector-vector division
    inline Vec2 operator/(const Vec2 &other) const
    {
      return _mm_div_ps(simdData, other.simdData);
    }

    // Vector-float addition
    inline Vec2 operator+(float scalar) const
    {
      return {x + scalar, y + scalar};
    }

    // Vector-float subtraction
    inline Vec2 operator-(float scalar) const
    {
      return {x - scalar, y - scalar};
    }

    // Vector-float multiplication
    inline Vec2 operator*(float scalar) const
    {
      return {x * scalar, y * scalar};
    }

    // Vector-float division
    inline Vec2 operator/(float scalar) const
    {
      return {x / scalar, y / scalar};
    }

    // Dot product of two vectors
    inline float dot(const Vec2 &other) const
    {
      return x * other.x + y * other.y;
    }

    // Magnitude (length) of the vector
    inline float magnitude() const
    {
      return std::sqrt(dot(*this));
    }

    // Squared magnitude (length) of the vector
    inline float magnitude2() const
    {
      return dot(*this);
    }

    // Normalize the vector
    inline Vec2 normalize() const
    {
      float mag = magnitude();
      float invMagnitude = 1.0f / (mag + (mag == 0.0f));
      return {x * invMagnitude, y * invMagnitude};
    }
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

    // 3D vector struct
    inline Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    inline Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline Vec3(__m128 data) : simdData(data) {}

    // Vector-vector addition
    inline Vec3 operator+(const Vec3 &other) const
    {
      return _mm_add_ps(simdData, other.simdData);
    }

    // Vector-vector subtraction
    inline Vec3 operator-(const Vec3 &other) const
    {
      return _mm_sub_ps(simdData, other.simdData);
    }

    // Vector-vector multiplication
    inline Vec3 operator*(const Vec3 &other) const
    {
      return _mm_mul_ps(simdData, other.simdData);
    }

    // Vector-vector division
    inline Vec3 operator/(const Vec3 &other) const
    {
      return _mm_div_ps(simdData, other.simdData);
    }

    // Vector-float addition
    inline Vec3 operator+(float scalar) const
    {
      return {x + scalar, y + scalar, z + scalar};
    }

    // Vector-float subtraction
    inline Vec3 operator-(float scalar) const
    {
      return {x - scalar, y - scalar, z - scalar};
    }

    // Vector-float multiplication
    inline Vec3 operator*(float scalar) const
    {
      return {x * scalar, y * scalar, z * scalar};
    }

    // Vector-float division
    inline Vec3 operator/(float scalar) const
    {
      return {x / scalar, y / scalar, z / scalar};
    }

    // Dot product of two vectors
    inline float dot(const Vec3 &other) const
    {
      return x * other.x + y * other.y + z * other.z;
    }

    // Cross product of two vectors
    inline Vec3 cross(const Vec3 &other) const
    {
      return {
          (y * other.z) - (z * other.y),
          (z * other.x) - (x * other.z),
          (x * other.y) - (y * other.x)};
    }

    // Magnitude (length) of the vector
    inline float magnitude() const
    {
      return std::sqrt(dot(*this));
    }

    // Squared magnitude (length) of the vector
    inline float magnitude2() const
    {
      return dot(*this);
    }

    // Normalize the vector
    inline Vec3 normalize() const
    {
      float mag = magnitude();
      float invMagnitude = 1.0f / (mag + (mag == 0.0f));
      return {x * invMagnitude, y * invMagnitude, z * invMagnitude};
    }
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

    // 4D vector struct
    Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4(__m128 data) : simdData(data) {}

    // Vector-vector addition
    inline Vec4 operator+(const Vec4 &other) const
    {
      return _mm_add_ps(simdData, other.simdData);
    }

    // Vector-vector subtraction
    inline Vec4 operator-(const Vec4 &other) const
    {
      return _mm_sub_ps(simdData, other.simdData);
    }

    // Vector-vector multiplication
    inline Vec4 operator*(const Vec4 &other) const
    {
      return _mm_mul_ps(simdData, other.simdData);
    }

    // Vector-vector division
    inline Vec4 operator/(const Vec4 &other) const
    {
      return _mm_div_ps(simdData, other.simdData);
    }

    // Vector-float addition
    inline Vec4 operator+(float scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_add_ps(simdData, scalarData);
    }

    // Vector-float subtraction
    inline Vec4 operator-(float scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_sub_ps(simdData, scalarData);
    }

    // Vector-float multiplication
    inline Vec4 operator*(float scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_mul_ps(simdData, scalarData);
    }

    // Vector-float division
    inline Vec4 operator/(float scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_div_ps(simdData, scalarData);
    }

    // Dot product of two vectors
    inline float dot(const Vec4 &other) const
    {
      return _mm_dp_ps(simdData, other.simdData, 0xFF)[0];
    }

    // Magnitude (length) of the vector
    inline float magnitude() const
    {
      return _mm_sqrt_ps(_mm_dp_ps(simdData, simdData, 0xFF))[0];
    }

    // Squared magnitude (length) of the vector
    inline float magnitude2() const
    {
      return dot(*this);
    }

    // Normalize the vector
    inline Vec4 normalize() const
    {
      return _mm_mul_ps(simdData, _mm_rsqrt_ps(_mm_dp_ps(simdData, simdData, 0xFF)));
    }
  };

  struct Mat2
  {
    Vec2 rows[2];

    // 2x2 matrix struct
    inline Mat2()
    {
      rows[0] = Vec2(1.0f, 0.0f);
      rows[1] = Vec2(0.0f, 1.0f);
    }

    inline Mat2(const Vec2 &col1, const Vec2 &col2)
    {
      rows[0] = col1;
      rows[1] = col2;
    }

    // Matrix-matrix addition
    inline Mat2 operator+(const Mat2 &other) const
    {
      return {rows[0] + other.rows[0], rows[1] + other.rows[1]};
    }

    // Matrix-matrix subtraction
    inline Mat2 operator-(const Mat2 &other) const
    {
      return {rows[0] - other.rows[0], rows[1] - other.rows[1]};
    }

    // Matrix-matrix multiplication using SIMD
    inline Mat2 operator*(const Mat2 &mat) const
    {
      // Mat2 result;

      // __m128 col0 = _mm_setr_ps(rows[0].x, rows[0].y, 0.0f, 0.0f);
      // __m128 col1 = _mm_setr_ps(rows[1].x, rows[1].y, 0.0f, 0.0f);
      // __m128 matCol0 = _mm_setr_ps(mat.rows[0].x, mat.rows[0].y, 0.0f, 0.0f);
      // __m128 matCol1 = _mm_setr_ps(mat.rows[1].x, mat.rows[1].y, 0.0f, 0.0f);

      // __m128 resCol0 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(col0, col0, _MM_SHUFFLE(0, 0, 0, 0)), matCol0),
      //                             _mm_mul_ps(_mm_shuffle_ps(col0, col0, _MM_SHUFFLE(1, 1, 1, 1)), matCol1));
      // __m128 resCol1 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(col1, col1, _MM_SHUFFLE(0, 0, 0, 0)), matCol0),
      //                             _mm_mul_ps(_mm_shuffle_ps(col1, col1, _MM_SHUFFLE(1, 1, 1, 1)), matCol1));

      // result.rows[0] = resCol0;
      // result.rows[1] = resCol1;

      // return result;

      Mat2 result{};

      result.rows[0] = Vec2(
          rows[0].x * mat.rows[0].x + rows[0].y * mat.rows[1].x,
          rows[0].x * mat.rows[0].y + rows[0].y * mat.rows[1].y);

      result.rows[1] = Vec2(
          rows[1].x * mat.rows[0].x + rows[1].y * mat.rows[1].x,
          rows[1].x * mat.rows[0].y + rows[1].y * mat.rows[1].y);

      return result;
    }

    // Matrix-float multiplication
    inline Mat2 operator*(float scalar) const
    {
      return {rows[0] * scalar, rows[1] * scalar};
    }

    // Matrix-vector multiplication
    inline Vec2 operator*(const Vec2 &vec) const
    {
      // Vector based dot product
      return _mm_add_ps(_mm_mul_ps(vec.simdData, rows[0].simdData), _mm_mul_ps(vec.simdData, rows[1].simdData));
    }

    // Matrix transposition
    inline Mat2 transpose() const
    {
      return {Vec2(rows[0].x, rows[1].x), Vec2(rows[0].y, rows[1].y)};
    }

    // Matrix determinant
    inline float determinant() const
    {
      // __m128 a = rows[0].simdData;
      // __m128 b = rows[1].simdData;

      // __m128 det = _mm_sub_ps(_mm_mul_ps(a, _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 2, 0, 1))),
      //                         _mm_mul_ps(b, _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1))));

      // float result;
      // _mm_store_ss(&result, det);
      // return result;
      return rows[0].x * rows[1].y - rows[0].y * rows[1].x;
    }

    // Matrix inverse
    inline Mat2 inverse() const
    {
      float det = determinant();
      float invDet = 1.0f / (det + (det == 0.f)) * !(det == 0.f);

      return {Vec2(rows[1].y, -rows[0].y) * invDet,
              Vec2(-rows[1].x, rows[0].x) * invDet};
    }
  };

  // 3x3 matrix struct
  struct Mat3
  {
    Vec3 rows[3];

    // 3x3 matrix struct
    inline Mat3()
    {
      rows[0] = Vec3(1.0f, 0.0f, 0.0f);
      rows[1] = Vec3(0.0f, 1.0f, 0.0f);
      rows[2] = Vec3(0.0f, 0.0f, 1.0f);
    }

    inline Mat3(const Vec3 &col1, const Vec3 &col2, const Vec3 &col3)
    {
      rows[0] = col1;
      rows[1] = col2;
      rows[2] = col3;
    }

    // Matrix-matrix addition
    inline Mat3 operator+(const Mat3 &other) const
    {
      return {rows[0] + other.rows[0], rows[1] + other.rows[1], rows[2] + other.rows[2]};
    }

    // Matrix-matrix subtraction
    inline Mat3 operator-(const Mat3 &other) const
    {
      return {rows[0] - other.rows[0], rows[1] - other.rows[1], rows[2] - other.rows[2]};
    }

    // Matrix-matrix multiplication using SIMD
    inline Mat3 operator*(const Mat3 &mat) const
    {
      Mat3 result;

      __m128 col0 = _mm_setr_ps(rows[0].x, rows[0].y, rows[0].z, 0.0f);
      __m128 col1 = _mm_setr_ps(rows[1].x, rows[1].y, rows[1].z, 0.0f);
      __m128 col2 = _mm_setr_ps(rows[2].x, rows[2].y, rows[2].z, 0.0f);
      __m128 matCol0 = _mm_setr_ps(mat.rows[0].x, mat.rows[0].y, mat.rows[0].z, 0.0f);
      __m128 matCol1 = _mm_setr_ps(mat.rows[1].x, mat.rows[1].y, mat.rows[1].z, 0.0f);
      __m128 matCol2 = _mm_setr_ps(mat.rows[2].x, mat.rows[2].y, mat.rows[2].z, 0.0f);

      __m128 resCol0 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(col0, col0, _MM_SHUFFLE(0, 0, 0, 0)), matCol0),
                                             _mm_mul_ps(_mm_shuffle_ps(col0, col0, _MM_SHUFFLE(1, 1, 1, 1)), matCol1)),
                                  _mm_mul_ps(_mm_shuffle_ps(col0, col0, _MM_SHUFFLE(2, 2, 2, 2)), matCol2));
      __m128 resCol1 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(col1, col1, _MM_SHUFFLE(0, 0, 0, 0)), matCol0),
                                             _mm_mul_ps(_mm_shuffle_ps(col1, col1, _MM_SHUFFLE(1, 1, 1, 1)), matCol1)),
                                  _mm_mul_ps(_mm_shuffle_ps(col1, col1, _MM_SHUFFLE(2, 2, 2, 2)), matCol2));
      __m128 resCol2 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(col2, col2, _MM_SHUFFLE(0, 0, 0, 0)), matCol0),
                                             _mm_mul_ps(_mm_shuffle_ps(col2, col2, _MM_SHUFFLE(1, 1, 1, 1)), matCol1)),
                                  _mm_mul_ps(_mm_shuffle_ps(col2, col2, _MM_SHUFFLE(2, 2, 2, 2)), matCol2));

      result.rows[0] = resCol0;
      result.rows[1] = resCol1;
      result.rows[2] = resCol2;

      return result;
    }

    // Matrix-float multiplication
    inline Mat3 operator*(float scalar) const
    {
      return {rows[0] * scalar, rows[1] * scalar, rows[2] * scalar};
    }

    // Matrix-vector multiplication
    inline Vec3 operator*(const Vec3 &vec) const
    {
      // Load the vector into SIMD register
      __m128 vecSimd = vec.simdData;

      // Transpose the matrix
      __m128 row1 = _mm_load_ps(&rows[0].x);
      __m128 row2 = _mm_load_ps(&rows[1].x);
      __m128 row3 = _mm_load_ps(&rows[2].x);
      __m128 row4 = _mm_setzero_ps();
      _MM_TRANSPOSE4_PS(row1, row2, row3, row4);

      // Multiply the transposed matrix with the vector
      return _mm_add_ps(
          _mm_add_ps(_mm_mul_ps(row1, _mm_shuffle_ps(vecSimd, vecSimd, _MM_SHUFFLE(0, 0, 0, 0))),
                     _mm_mul_ps(row2, _mm_shuffle_ps(vecSimd, vecSimd, _MM_SHUFFLE(1, 1, 1, 1)))),
          _mm_mul_ps(row3, _mm_shuffle_ps(vecSimd, vecSimd, _MM_SHUFFLE(2, 2, 2, 2))));
    }

    // Matrix transposition
    inline Mat3 transpose() const
    {
      return {{rows[0].x, rows[1].x, rows[2].x},
              {rows[0].y, rows[1].y, rows[2].y},
              {rows[0].z, rows[1].z, rows[2].z}};
    }

    // Matrix determinant
    inline float determinant() const
    {
      __m128 a = rows[0].simdData;
      __m128 b = rows[1].simdData;
      __m128 c = rows[2].simdData;

      __m128 det1 = _mm_mul_ps(_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 1, 0, 2)));
      __m128 det2 = _mm_mul_ps(_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1)));
      __m128 det = _mm_sub_ps(det1, det2);

      __m128 prod = _mm_mul_ps(a, det);
      float result;
      _mm_store_ss(&result, _mm_hadd_ps(prod, prod));

      return result;
    }

    // Matrix inverse
    inline Mat3 inverse() const
    {
      float det = determinant();
      float invDet = 1.0f / (det + (det == 0.f)) * !(det == 0.f);

      Mat3 result;

      result.rows[0] = Vec3((rows[1].y * rows[2].z - rows[1].z * rows[2].y) * invDet,
                            (rows[0].z * rows[2].y - rows[0].y * rows[2].z) * invDet,
                            (rows[0].y * rows[1].z - rows[0].z * rows[1].y) * invDet);

      result.rows[1] = Vec3((rows[1].z * rows[2].x - rows[1].x * rows[2].z) * invDet,
                            (rows[0].x * rows[2].z - rows[0].z * rows[2].x) * invDet,
                            (rows[0].z * rows[1].x - rows[0].x * rows[1].z) * invDet);

      result.rows[2] = Vec3((rows[1].x * rows[2].y - rows[1].y * rows[2].x) * invDet,
                            (rows[0].y * rows[2].x - rows[0].x * rows[2].y) * invDet,
                            (rows[0].x * rows[1].y - rows[0].y * rows[1].x) * invDet);

      return result;
    }

    inline void print(const Mat3 &mat3)
    {
      for (int i = 0; i < 3; ++i)
      {
        printf("[ %.4f %.4f %.4f ]\n", mat3.rows[i].x, mat3.rows[i].y, mat3.rows[i].z);
      }
    }
  };

  struct Mat4
  {
    Vec4 rows[4];

    // 4x4 matrix struct
    inline Mat4()
    {
      rows[0] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
      rows[1] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
      rows[2] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
      rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    inline Mat4(const Vec4 &col1, const Vec4 &col2, const Vec4 &col3, const Vec4 &col4)
    {
      rows[0] = col1;
      rows[1] = col2;
      rows[2] = col3;
      rows[3] = col4;
    }

    // Matrix-matrix addition
    inline Mat4 operator+(const Mat4 &other) const
    {
      return {rows[0] + other.rows[0],
              rows[1] + other.rows[1],
              rows[2] + other.rows[2],
              rows[3] + other.rows[3]};
    }

    // Matrix-matrix subtraction
    inline Mat4 operator-(const Mat4 &other) const
    {
      return {rows[0] - other.rows[0],
              rows[1] - other.rows[1],
              rows[2] - other.rows[2],
              rows[3] - other.rows[3]};
    }

    // Matrix-matrix multiplication using SIMD
    inline Mat4 operator*(const Mat4 &mat) const
    {
      Mat4 result;

      // for (int i = 0; i < 4; ++i)
      // {
      //   __m128 row0 = _mm_set1_ps(rows[i].x);
      //   __m128 row1 = _mm_set1_ps(rows[i].y);
      //   __m128 row2 = _mm_set1_ps(rows[i].z);
      //   __m128 row3 = _mm_set1_ps(rows[i].w);

      //   __m128 prod0 = _mm_mul_ps(row0, mat.rows[0].simdData);
      //   __m128 prod1 = _mm_mul_ps(row1, mat.rows[1].simdData);
      //   __m128 prod2 = _mm_mul_ps(row2, mat.rows[2].simdData);
      //   __m128 prod3 = _mm_mul_ps(row3, mat.rows[3].simdData);

      //   __m128 sum0 = _mm_add_ps(prod0, prod1);
      //   __m128 sum1 = _mm_add_ps(prod2, prod3);
      //   __m128 sum2 = _mm_add_ps(sum0, sum1);

      //   result.rows[i].simdData = sum2;
      // }

      for (int i = 0; i < 4; ++i)
      {
        result.rows[i].x = rows[i].x * mat.rows[0].x +
                           rows[i].y * mat.rows[1].x +
                           rows[i].z * mat.rows[2].x +
                           rows[i].w * mat.rows[3].x;

        result.rows[i].y = rows[i].x * mat.rows[0].y +
                           rows[i].y * mat.rows[1].y +
                           rows[i].z * mat.rows[2].y +
                           rows[i].w * mat.rows[3].y;

        result.rows[i].z = rows[i].x * mat.rows[0].z +
                           rows[i].y * mat.rows[1].z +
                           rows[i].z * mat.rows[2].z +
                           rows[i].w * mat.rows[3].z;

        result.rows[i].w = rows[i].x * mat.rows[0].w +
                           rows[i].y * mat.rows[1].w +
                           rows[i].z * mat.rows[2].w +
                           rows[i].w * mat.rows[3].w;
      }

      return result;
    }

    // Matrix-float multiplication
    inline Mat4 operator*(float scalar) const
    {
      return {rows[0] * scalar, rows[1] * scalar, rows[2] * scalar, rows[3] * scalar};
    }

    // Matrix-vector multiplication
    inline Vec4 operator*(const Vec4 &vec) const
    {
      Vec4 result;

      __m128 dot0 = _mm_mul_ps(rows[0].simdData, vec.simdData);
      __m128 dot1 = _mm_mul_ps(rows[1].simdData, vec.simdData);
      __m128 dot2 = _mm_mul_ps(rows[2].simdData, vec.simdData);
      __m128 dot3 = _mm_mul_ps(rows[3].simdData, vec.simdData);

      __m128 xy = _mm_hadd_ps(dot0, dot1);
      __m128 zw = _mm_hadd_ps(dot2, dot3);

      return _mm_hadd_ps(xy, zw);
    }

    // Matrix transposition
    inline Mat4 transpose() const
    {
      return {{rows[0].x, rows[1].x, rows[2].x, rows[3].x},
              {rows[0].y, rows[1].y, rows[2].y, rows[3].y},
              {rows[0].z, rows[1].z, rows[2].z, rows[3].z},
              {rows[0].w, rows[1].w, rows[2].w, rows[3].w}};
    }

    // Matrix determinant
    inline float determinant() const
    {
      float a11 = rows[0].x, a12 = rows[0].y, a13 = rows[0].z, a14 = rows[0].w;
      float a21 = rows[1].x, a22 = rows[1].y, a23 = rows[1].z, a24 = rows[1].w;
      float a31 = rows[2].x, a32 = rows[2].y, a33 = rows[2].z, a34 = rows[2].w;
      float a41 = rows[3].x, a42 = rows[3].y, a43 = rows[3].z, a44 = rows[3].w;

      float det =
          a11 * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42)) -
          a12 * (a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41)) +
          a13 * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41)) -
          a14 * (a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41));

      return det;
    }

    // // Matrix determinant
    // inline float determinant() const
    // {
    //   // float a11 = rows[0].x, a12 = rows[0].y, a13 = rows[0].z, a14 = rows[0].w;
    //   // float a21 = rows[1].x, a22 = rows[1].y, a23 = rows[1].z, a24 = rows[1].w;
    //   // float a31 = rows[2].x, a32 = rows[2].y, a33 = rows[2].z, a34 = rows[2].w;
    //   // float a41 = rows[3].x, a42 = rows[3].y, a43 = rows[3].z, a44 = rows[3].w;

    //   __m128 a3332 = _mm_set_ps(rows[2].y, rows[2].y, rows[2].z, rows[2].z);
    //   __m128 a4443 = _mm_set_ps(rows[3].z, rows[3].w, rows[3].w, rows[3].w);
    //   __m128 a3433 = _mm_set_ps(rows[2].z, rows[2].w, rows[2].w, rows[2].w);
    //   __m128 a4342 = _mm_set_ps(rows[3].y, rows[3].y, rows[3].z, rows[3].z);
    //   __m128 a3231 = _mm_set_ps(rows[2].x, rows[2].x, rows[2].x, rows[2].y);
    //   __m128 a4241 = _mm_set_ps(rows[3].x, rows[3].x, rows[3].x, rows[3].y);

    //   __m128 detM0 = _mm_sub_ps(_mm_mul_ps(a3332, a4443), _mm_mul_ps(a3433, a4342));
    //   __m128 detM1 = _mm_set_ps(rows[1].x, rows[1].x, rows[1].x, rows[1].y);
    //   __m128 detM10 = _mm_mul_ps(detM0, detM1);

    //   __m128 detM02 = _mm_sub_ps(_mm_mul_ps(a3231, a4443), _mm_mul_ps(a3433, a4241));
    //   __m128 detM2 = _mm_set_ps(rows[1].y, rows[1].y, rows[1].z, rows[1].z);
    //   __m128 detM20 = _mm_mul_ps(detM02, detM2);

    //   __m128 detM03 = _mm_sub_ps(_mm_mul_ps(a3231, a4342), _mm_mul_ps(a3332, a4241));
    //   __m128 detM3 = _mm_set_ps(rows[1].z, rows[1].w, rows[1].w, rows[1].w);
    //   __m128 detM30 = _mm_mul_ps(detM03, detM3);

    //   __m128 detM40 = _mm_add_ps(_mm_sub_ps(detM10, detM20), detM30);

    //   __m128 xmm0 = _mm_mul_ps(rows[0].simdData, detM40);

    //   __m128 det = _mm_hadd_ps(_mm_hsub_ps(xmm0, xmm0), _mm_hsub_ps(xmm0, xmm0));

    //   // float det =
    //   //     a11 * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42)) -
    //   //     a12 * (a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41)) +
    //   //     a13 * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41)) -
    //   //     a14 * (a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41));

    //   float result;
    //   _mm_store_ss(&result, det);
    //   return result;
    // }

    // Matrix inverse
    inline Mat4 inverse() const
    {
      // Calculate the matrix of minors
      float m11 = rows[1].y * (rows[2].z * rows[3].w - rows[2].w * rows[3].z) -
                  rows[1].z * (rows[2].y * rows[3].w - rows[2].w * rows[3].y) +
                  rows[1].w * (rows[2].y * rows[3].z - rows[2].z * rows[3].y);

      float m12 = rows[1].x * (rows[2].z * rows[3].w - rows[2].w * rows[3].z) -
                  rows[1].z * (rows[2].x * rows[3].w - rows[2].w * rows[3].x) +
                  rows[1].w * (rows[2].x * rows[3].z - rows[2].z * rows[3].x);

      float m13 = rows[1].x * (rows[2].y * rows[3].w - rows[2].w * rows[3].y) -
                  rows[1].y * (rows[2].x * rows[3].w - rows[2].w * rows[3].x) +
                  rows[1].w * (rows[2].x * rows[3].y - rows[2].y * rows[3].x);

      float m14 = rows[1].x * (rows[2].y * rows[3].z - rows[2].z * rows[3].y) -
                  rows[1].y * (rows[2].x * rows[3].z - rows[2].z * rows[3].x) +
                  rows[1].z * (rows[2].x * rows[3].y - rows[2].y * rows[3].x);

      float m21 = rows[0].y * (rows[2].z * rows[3].w - rows[2].w * rows[3].z) -
                  rows[0].z * (rows[2].y * rows[3].w - rows[2].w * rows[3].y) +
                  rows[0].w * (rows[2].y * rows[3].z - rows[2].z * rows[3].y);

      float m22 = rows[0].x * (rows[2].z * rows[3].w - rows[2].w * rows[3].z) -
                  rows[0].z * (rows[2].x * rows[3].w - rows[2].w * rows[3].x) +
                  rows[0].w * (rows[2].x * rows[3].z - rows[2].z * rows[3].x);

      float m23 = rows[0].x * (rows[2].y * rows[3].w - rows[2].w * rows[3].y) -
                  rows[0].y * (rows[2].x * rows[3].w - rows[2].w * rows[3].x) +
                  rows[0].w * (rows[2].x * rows[3].y - rows[2].y * rows[3].x);

      float m24 = rows[0].x * (rows[2].y * rows[3].z - rows[2].z * rows[3].y) -
                  rows[0].y * (rows[2].x * rows[3].z - rows[2].z * rows[3].x) +
                  rows[0].z * (rows[2].x * rows[3].y - rows[2].y * rows[3].x);

      float m31 = rows[0].y * (rows[1].z * rows[3].w - rows[1].w * rows[3].z) -
                  rows[0].z * (rows[1].y * rows[3].w - rows[1].w * rows[3].y) +
                  rows[0].w * (rows[1].y * rows[3].z - rows[1].z * rows[3].y);

      float m32 = rows[0].x * (rows[1].z * rows[3].w - rows[1].w * rows[3].z) -
                  rows[0].z * (rows[1].x * rows[3].w - rows[1].w * rows[3].x) +
                  rows[0].w * (rows[1].x * rows[3].z - rows[1].z * rows[3].x);

      float m33 = rows[0].x * (rows[1].y * rows[3].w - rows[1].w * rows[3].y) -
                  rows[0].y * (rows[1].x * rows[3].w - rows[1].w * rows[3].x) +
                  rows[0].w * (rows[1].x * rows[3].y - rows[1].y * rows[3].x);

      float m34 = rows[0].x * (rows[1].y * rows[3].z - rows[1].z * rows[3].y) -
                  rows[0].y * (rows[1].x * rows[3].z - rows[1].z * rows[3].x) +
                  rows[0].z * (rows[1].x * rows[3].y - rows[1].y * rows[3].x);

      float m41 = rows[0].y * (rows[1].z * rows[2].w - rows[1].w * rows[2].z) -
                  rows[0].z * (rows[1].y * rows[2].w - rows[1].w * rows[2].y) +
                  rows[0].w * (rows[1].y * rows[2].z - rows[1].z * rows[2].y);

      float m42 = rows[0].x * (rows[1].z * rows[2].w - rows[1].w * rows[2].z) -
                  rows[0].z * (rows[1].x * rows[2].w - rows[1].w * rows[2].x) +
                  rows[0].w * (rows[1].x * rows[2].z - rows[1].z * rows[2].x);

      float m43 = rows[0].x * (rows[1].y * rows[2].w - rows[1].w * rows[2].y) -
                  rows[0].y * (rows[1].x * rows[2].w - rows[1].w * rows[2].x) +
                  rows[0].w * (rows[1].x * rows[2].y - rows[1].y * rows[2].x);

      float m44 = rows[0].x * (rows[1].y * rows[2].z - rows[1].z * rows[2].y) -
                  rows[0].y * (rows[1].x * rows[2].z - rows[1].z * rows[2].x) +
                  rows[0].z * (rows[1].x * rows[2].y - rows[1].y * rows[2].x);

      // Calculate the determinant of the matrix
      float det = rows[0].x * m11 - rows[0].y * m12 + rows[0].z * m13 - rows[0].w * m14;
      float invDet = 1.0f / (det + (fabs(det) < 0.0001f)) * !(fabs(det) < 0.0001f);

      // Calculate the adjugate matrix
      Mat4 adjugate(
          Vec4(m11, -m21, m31, -m41),
          Vec4(-m12, m22, -m32, m42),
          Vec4(m13, -m23, m33, -m43),
          Vec4(-m14, m24, -m34, m44));

      // Multiply the adjugate matrix by the inverse determinant to get the inverse matrix
      return adjugate * invDet;
    }

    inline void translate(const Vec3 &translation)
    {
      rows[0].w += translation.x;
      rows[1].w += translation.y;
      rows[2].w += translation.z;
    }

    inline void scale(const Vec3 &scale)
    {
      rows[0].x *= scale.x;
      rows[1].y *= scale.y;
      rows[2].z *= scale.z;
    }

    // inline void rotateX(float angle)
    // {
    // }

    // inline void rotateY(float angle)
    // {
    // }

    // inline void rotateZ(float angle)
    // {
    // }

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
