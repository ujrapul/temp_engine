#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

#include <cmath>
#include <iostream>
#include <immintrin.h>
#include <cassert>

namespace Temp::Math
{
  constexpr float PI = 3.141592653589793238f;

  template <typename T>
  struct Vec2
  {
    union
    {
      struct
      {
        T x;
        T y;
      };
      T data[2];
      __m128 simdData;
    };

    constexpr Vec2() : x(0.0f), y(0.0f) {}
    constexpr Vec2(T x, T y) : x(x), y(y) {}
    constexpr Vec2(__m128 data) : simdData(data) {}

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
    constexpr Vec2 operator+(T scalar) const
    {
      return {x + scalar, y + scalar};
    }

    // Vector-float subtraction
    constexpr Vec2 operator-(T scalar) const
    {
      return {x - scalar, y - scalar};
    }

    // Vector-float multiplication
    constexpr Vec2 operator*(T scalar) const
    {
      return {x * scalar, y * scalar};
    }

    // Vector-float division
    constexpr Vec2 operator/(T scalar) const
    {
      return {x / scalar, y / scalar};
    }

    // Dot product of two vectors
    constexpr T dot(const Vec2 &other) const
    {
      return x * other.x + y * other.y;
    }

    // Magnitude (length) of the vector
    constexpr T magnitude() const
    {
      return std::sqrt(dot(*this));
    }

    // Squared magnitude (length) of the vector
    constexpr T magnitude2() const
    {
      return dot(*this);
    }

    // Normalize the vector
    constexpr Vec2 normalize() const
    {
      float mag = magnitude();
      float invMagnitude = 1.0f / (mag + (mag == 0.0f));
      return {x * invMagnitude, y * invMagnitude};
    }
  };

  using Vec2f  = Vec2<float>;
  using Vec2ui = Vec2<unsigned int>;
  using Vec2i = Vec2<int>;

  template <typename T>
  struct Vec3
  {
    union
    {
      struct
      {
        T x;
        T y;
        T z;
      };
      T data[3];
      __m128 simdData;
    };

    // 3D vector struct
    constexpr Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    constexpr Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    constexpr Vec3(__m128 data) : simdData(data) {}

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
    constexpr Vec3 operator+(T scalar) const
    {
      return {x + scalar, y + scalar, z + scalar};
    }

    // Vector-float subtraction
    constexpr Vec3 operator-(T scalar) const
    {
      return {x - scalar, y - scalar, z - scalar};
    }

    // Vector-float multiplication
    constexpr Vec3 operator*(T scalar) const
    {
      return {x * scalar, y * scalar, z * scalar};
    }

    // Vector-float division
    constexpr Vec3 operator/(T scalar) const
    {
      return {x / scalar, y / scalar, z / scalar};
    }

    constexpr T &operator[](int i)
    {
      return data[i];
    }

    // Dot product of two vectors
    constexpr T dot(const Vec3 &other) const
    {
      return x * other.x + y * other.y + z * other.z;
    }

    // Cross product of two vectors
    constexpr Vec3 cross(const Vec3 &other) const
    {
      return {
          (y * other.z) - (z * other.y),
          (z * other.x) - (x * other.z),
          (x * other.y) - (y * other.x)};
    }

    // Magnitude (length) of the vector
    constexpr T magnitude() const
    {
      return std::sqrt(dot(*this));
    }

    // Squared magnitude (length) of the vector
    constexpr T magnitude2() const
    {
      return dot(*this);
    }

    // Normalize the vector
    constexpr Vec3 normalize() const
    {
      float mag = magnitude();
      float invMagnitude = 1.0f / (mag + (mag == 0.0f));
      return {x * invMagnitude, y * invMagnitude, z * invMagnitude};
    }
  };

  using Vec3f = Vec3<float>;
  using Vec3i = Vec3<unsigned char>;

  template <typename T>
  struct Vec4
  {
    union
    {
      struct
      {
        T x;
        T y;
        T z;
        T w;
      };
      T data[4];
      __m128 simdData;
    };

    // 4D vector struct
    constexpr Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    constexpr Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    constexpr Vec4(__m128 data) : simdData(data) {}

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
    inline Vec4 operator+(T scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_add_ps(simdData, scalarData);
    }

    // Vector-float subtraction
    inline Vec4 operator-(T scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_sub_ps(simdData, scalarData);
    }

    // Vector-float multiplication
    inline Vec4 operator*(T scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_mul_ps(simdData, scalarData);
    }

    // Vector-float division
    inline Vec4 operator/(T scalar) const
    {
      __m128 scalarData = _mm_set1_ps(scalar);
      return _mm_div_ps(simdData, scalarData);
    }

    constexpr T &operator[](int i)
    {
      return data[i];
    }

    // Dot product of two vectors
    inline T dot(const Vec4 &other) const
    {
      return _mm_dp_ps(simdData, other.simdData, 0xFF)[0];
    }

    // Magnitude (length) of the vector
    inline T magnitude() const
    {
      return _mm_sqrt_ps(_mm_dp_ps(simdData, simdData, 0xFF))[0];
    }

    // Squared magnitude (length) of the vector
    inline T magnitude2() const
    {
      return dot(*this);
    }

    // Normalize the vector
    inline Vec4 normalize() const
    {
      return _mm_mul_ps(simdData, _mm_rsqrt_ps(_mm_dp_ps(simdData, simdData, 0xFF)));
    }
  };

  using Vec4f = Vec4<float>;
  using Vec4i = Vec4<unsigned char>;

  struct Mat2
  {
    Vec2f rows[2];

    // 2x2 matrix struct
    constexpr Mat2() : rows{{1.0f, 0.0f}, {0.0f, 1.0f}} {}
    constexpr Mat2(const Vec2f &col1, const Vec2f &col2) : rows{col1, col2} {}

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
    inline Vec2f operator*(const Vec2f &vec) const
    {
      // Vector based dot product
      return _mm_add_ps(_mm_mul_ps(vec.simdData, rows[0].simdData), _mm_mul_ps(vec.simdData, rows[1].simdData));
    }

    // Matrix transposition
    constexpr Mat2 transpose() const
    {
      return {Vec2f(rows[0].x, rows[1].x), Vec2f(rows[0].y, rows[1].y)};
    }

    // Matrix determinant
    constexpr float determinant() const
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

      return {Vec2f(rows[1].y, -rows[0].y) * invDet,
              Vec2f(-rows[1].x, rows[0].x) * invDet};
    }
  };

  // 3x3 matrix struct
  struct Mat3
  {
    Vec3f rows[3];

    // 3x3 matrix struct
    constexpr Mat3() : rows{{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}} {}
    constexpr Mat3(const Vec3f &col1, const Vec3f &col2, const Vec3f &col3) : rows{col1, col2, col3} {}

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
    inline Vec3f operator*(const Vec3f &vec) const
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
    constexpr Mat3 transpose() const
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

    constexpr void print(const Mat3 &mat3)
    {
      for (int i = 0; i < 3; ++i)
      {
        printf("[ %.4f %.4f %.4f ]\n", mat3.rows[i].x, mat3.rows[i].y, mat3.rows[i].z);
      }
    }
  };

  struct Mat4
  {
    Vec4f rows[4];

    // 4x4 matrix struct
    constexpr Mat4() : rows{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}} {}
    constexpr Mat4(const Vec4f &col1, const Vec4f &col2, const Vec4f &col3, const Vec4f &col4) : rows{col1, col2, col3, col4} {}

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
    constexpr Mat4 operator*(const Mat4 &mat) const
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
    inline Vec4f operator*(const Vec4f &vec) const
    {
      Vec4f result;

      __m128 dot0 = _mm_mul_ps(rows[0].simdData, vec.simdData);
      __m128 dot1 = _mm_mul_ps(rows[1].simdData, vec.simdData);
      __m128 dot2 = _mm_mul_ps(rows[2].simdData, vec.simdData);
      __m128 dot3 = _mm_mul_ps(rows[3].simdData, vec.simdData);

      __m128 xy = _mm_hadd_ps(dot0, dot1);
      __m128 zw = _mm_hadd_ps(dot2, dot3);

      return _mm_hadd_ps(xy, zw);
    }

    constexpr Vec4f &operator[](int i)
    {
      return rows[i];
    }

    // Matrix transposition
    constexpr Mat4 transpose() const
    {
      return {{rows[0].x, rows[1].x, rows[2].x, rows[3].x},
              {rows[0].y, rows[1].y, rows[2].y, rows[3].y},
              {rows[0].z, rows[1].z, rows[2].z, rows[3].z},
              {rows[0].w, rows[1].w, rows[2].w, rows[3].w}};
    }

    // Matrix determinant
    constexpr float determinant() const
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

    constexpr Mat4 translate(const Vec3f &translation)
    {
      Mat4 translateMatrix;
      translateMatrix[0].w = translation.x;
      translateMatrix[1].w = translation.y;
      translateMatrix[2].w = translation.z;

      return *this * translateMatrix;
    }

    constexpr Mat4 scale(const Vec3f &scale)
    {
      Mat4 scaleMatrix;
      scaleMatrix[0].x = scale.x;
      scaleMatrix[1].y = scale.y;
      scaleMatrix[2].z = scale.z;

      return *this * scaleMatrix;
    }

    // Rotations take radians!

    constexpr Mat4 rotateX(float angle)
    {
      float cosAngle = cosf(angle);
      float sinAngle = sinf(angle);

      Mat4 rotationMatrix;
      rotationMatrix[1][1] = cosAngle;
      rotationMatrix[1][2] = -sinAngle;
      rotationMatrix[2][1] = sinAngle;
      rotationMatrix[2][2] = cosAngle;

      return *this * rotationMatrix;
    }

    constexpr Mat4 rotateY(float angle)
    {
      float cosAngle = cosf(angle);
      float sinAngle = sinf(angle);

      Mat4 rotationMatrix;
      rotationMatrix[0][0] = cosAngle;
      rotationMatrix[0][2] = sinAngle;
      rotationMatrix[2][0] = -sinAngle;
      rotationMatrix[2][2] = cosAngle;

      return *this * rotationMatrix;
    }

    constexpr Mat4 rotateZ(float angle)
    {
      float cosAngle = cosf(angle);
      float sinAngle = sinf(angle);

      Mat4 rotationMatrix;
      rotationMatrix[0][0] = cosAngle;
      rotationMatrix[0][1] = -sinAngle;
      rotationMatrix[1][0] = sinAngle;
      rotationMatrix[1][1] = cosAngle;

      return *this * rotationMatrix;
    }

    constexpr Mat4 rotate(float angle, const Vec3f &axis)
    {
      Mat4 mat;
      float c = std::cos(angle);
      float s = std::sin(angle);
      float t = 1.0f - c;

      float x = axis.x;
      float y = axis.y;
      float z = axis.z;

      // Normalize the axis
      float length = std::sqrt(x * x + y * y + z * z);
      if (length != 0.0f)
      {
        x /= length;
        y /= length;
        z /= length;
      }

      // Calculate the rotation matrix elements
      float xt = x * t;
      float yt = y * t;
      float zt = z * t;
      float xs = x * s;
      float ys = y * s;
      float zs = z * s;

      // Set the rotation matrix elements
      mat[0][0] = xt * x + c;
      mat[0][1] = xt * y + zs;
      mat[0][2] = xt * z - ys;
      mat[0][3] = 0.0f;

      mat[1][0] = xt * y - zs;
      mat[1][1] = yt * y + c;
      mat[1][2] = yt * z + xs;
      mat[1][3] = 0.0f;

      mat[2][0] = xt * z + ys;
      mat[2][1] = yt * z - xs;
      mat[2][2] = zt * z + c;
      mat[2][3] = 0.0f;

      mat[3][0] = 0.0f;
      mat[3][1] = 0.0f;
      mat[3][2] = 0.0f;
      mat[3][3] = 1.0f;

      return *this * mat;
    }

    static inline Mat4 perspective(float fov, float aspectRatio, float near, float far)
    {
      Mat4 result;

      float tanHalfFOV = tanf(fov / 2.0f);
      float range = near - far;

      result[0][0] = 1.0f / (aspectRatio * tanHalfFOV);
      result[1][1] = 1.0f / tanHalfFOV;
      result[2][2] = (-near - far) / range;
      result[2][3] = 2.0f * far * near / range;
      result[3][2] = 1.0f;

      return result;
    }

    static constexpr Mat4 orthographic(float left, float right, float bottom, float top, float near, float far)
    {
      Mat4 result;

      float width = right - left;
      float height = top - bottom;
      float depth = far - near;

      result[0][0] = 2.0f / width;
      result[1][1] = 2.0f / height;
      result[2][2] = -2.0f / depth;
      result[0][3] = -(right + left) / width;
      result[1][3] = -(top + bottom) / height;
      result[2][3] = -(far + near) / depth;
      result[3][3] = 1.0f;

      return result;
    }
  };

  constexpr float ToRadians(float degrees)
  {
    return degrees * PI / 180.f;
  }

  namespace UnitTests
  {
    // Function to run the unit tests
    void Run();
  }
}

#pragma GCC diagnostic pop
