#include "Math.hpp"

namespace Temp::Math
{
  // 2D vector struct
  Vec2::Vec2() : x(0.0f), y(0.0f) {}
  Vec2::Vec2(float x, float y) : x(x), y(y) {}
  Vec2::Vec2(__m128 data) : simdData(data) {}

  // Vector-vector addition
  Vec2 Vec2::operator+(const Vec2 &other) const
  {
    return _mm_add_ps(simdData, other.simdData);
  }

  // Vector-vector subtraction
  Vec2 Vec2::operator-(const Vec2 &other) const
  {
    return _mm_sub_ps(simdData, other.simdData);
  }

  // Vector-vector multiplication
  Vec2 Vec2::operator*(const Vec2 &other) const
  {
    return _mm_mul_ps(simdData, other.simdData);
  }

  // Vector-vector division
  Vec2 Vec2::operator/(const Vec2 &other) const
  {
    return _mm_div_ps(simdData, other.simdData);
  }

  // Vector-float addition
  Vec2 Vec2::operator+(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_add_ps(simdData, scalarData);
  }

  // Vector-float subtraction
  Vec2 Vec2::operator-(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_sub_ps(simdData, scalarData);
  }

  // Vector-float multiplication
  Vec2 Vec2::operator*(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_mul_ps(simdData, scalarData);
  }

  // Vector-float division
  Vec2 Vec2::operator/(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_div_ps(simdData, scalarData);
  }

  // Dot product of two vectors
  float Vec2::dot(const Vec2 &other) const
  {
    __m128 result = _mm_mul_ps(simdData, other.simdData);
    return result[0] + result[1];
  }

  // Magnitude (length) of the vector
  float Vec2::magnitude() const
  {
    return std::sqrt(dot(*this));
  }

  // Squared magnitude (length) of the vector
  float Vec2::magnitude2() const
  {
    return dot(*this);
  }

  // Normalize the vector
  Vec2 Vec2::normalize() const
  {
    __m128 magSquared = _mm_dp_ps(simdData, simdData, 0xFF);
    __m128 invMag = _mm_rsqrt_ps(magSquared);
    __m128 result = _mm_mul_ps(simdData, invMag);
    return result;
  }

  // 3D vector struct
  Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
  Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
  Vec3::Vec3(__m128 data) : simdData(data) {}

  // Vector-vector addition
  Vec3 Vec3::operator+(const Vec3 &other) const
  {
    return _mm_add_ps(simdData, other.simdData);
  }

  // Vector-vector subtraction
  Vec3 Vec3::operator-(const Vec3 &other) const
  {
    return _mm_sub_ps(simdData, other.simdData);
  }

  // Vector-vector multiplication
  Vec3 Vec3::operator*(const Vec3 &other) const
  {
    return _mm_mul_ps(simdData, other.simdData);
  }

  // Vector-vector division
  Vec3 Vec3::operator/(const Vec3 &other) const
  {
    return _mm_div_ps(simdData, other.simdData);
  }

  // Vector-float addition
  Vec3 Vec3::operator+(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_add_ps(simdData, scalarData);
  }

  // Vector-float subtraction
  Vec3 Vec3::operator-(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_sub_ps(simdData, scalarData);
  }

  // Vector-float multiplication
  Vec3 Vec3::operator*(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_mul_ps(simdData, scalarData);
  }

  // Vector-float division
  Vec3 Vec3::operator/(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_div_ps(simdData, scalarData);
  }

  // Dot product of two vectors
  float Vec3::dot(const Vec3 &other) const
  {
    __m128 xmm = _mm_mul_ps(simdData, other.simdData);
    return xmm[0] + xmm[1] + xmm[2];
  }

  // Cross product of two vectors
  Vec3 Vec3::cross(const Vec3 &other) const
  {
    __m128 xmm1 = _mm_set_ps(0.0f, x, z, y);
    __m128 xmm2 = _mm_set_ps(0.0f, other.y, other.x, other.z);

    __m128 xmm3 = _mm_set_ps(0.0f, y, x, z);
    __m128 xmm4 = _mm_set_ps(0.0f, other.x, other.z, other.y);

    __m128 xmm5 = _mm_sub_ps(_mm_mul_ps(xmm1, xmm2), _mm_mul_ps(xmm3, xmm4));

    return {xmm5[2], xmm5[1], xmm5[0]};
  }

  // Magnitude (length) of the vector
  float Vec3::magnitude() const
  {
    __m128 result = _mm_dp_ps(simdData, simdData, 0xFF);
    result = _mm_sqrt_ps(result);
    return result[0];
  }

  // Squared magnitude (length) of the vector
  float Vec3::magnitude2() const
  {
    __m128 result = _mm_dp_ps(simdData, simdData, 0xFF);
    return result[0];
  }

  // Normalize the vector
  Vec3 Vec3::normalize() const
  {
    __m128 magSquared = _mm_dp_ps(simdData, simdData, 0xFF);
    __m128 invMag = _mm_rsqrt_ps(magSquared);
    __m128 result = _mm_mul_ps(simdData, invMag);
    return result;
  }

  // 4D vector struct
  Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
  Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  Vec4::Vec4(__m128 data) : simdData(data) {}

  // Vector-vector addition
  Vec4 Vec4::operator+(const Vec4 &other) const
  {
    return _mm_add_ps(simdData, other.simdData);
  }

  // Vector-vector subtraction
  Vec4 Vec4::operator-(const Vec4 &other) const
  {
    return _mm_sub_ps(simdData, other.simdData);
  }

  // Vector-vector multiplication
  Vec4 Vec4::operator*(const Vec4 &other) const
  {
    return _mm_mul_ps(simdData, other.simdData);
  }

  // Vector-vector division
  Vec4 Vec4::operator/(const Vec4 &other) const
  {
    return _mm_div_ps(simdData, other.simdData);
  }

  // Vector-float addition
  Vec4 Vec4::operator+(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_add_ps(simdData, scalarData);
  }

  // Vector-float subtraction
  Vec4 Vec4::operator-(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_sub_ps(simdData, scalarData);
  }

  // Vector-float multiplication
  Vec4 Vec4::operator*(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_mul_ps(simdData, scalarData);
  }

  // Vector-float division
  Vec4 Vec4::operator/(float scalar) const
  {
    __m128 scalarData = _mm_set1_ps(scalar);
    return _mm_div_ps(simdData, scalarData);
  }

  // Dot product of two vectors
  float Vec4::dot(const Vec4 &other) const
  {
    __m128 result = _mm_dp_ps(simdData, other.simdData, 0xFF);
    return result[0];
  }

  // Magnitude (length) of the vector
  float Vec4::magnitude() const
  {
    __m128 result = _mm_dp_ps(simdData, simdData, 0xFF);
    result = _mm_sqrt_ps(result);
    return result[0];
  }

  // Squared magnitude (length) of the vector
  float Vec4::magnitude2() const
  {
    __m128 result = _mm_dp_ps(simdData, simdData, 0xFF);
    return result[0];
  }

  // Normalize the vector
  Vec4 Vec4::normalize() const
  {
    __m128 magSquared = _mm_dp_ps(simdData, simdData, 0xFF);
    __m128 invMag = _mm_rsqrt_ps(magSquared);
    __m128 result = _mm_mul_ps(simdData, invMag);
    return result;
  }

  // 2x2 matrix struct
  Mat2::Mat2()
  {
    rows[0] = Vec2(1.0f, 0.0f);
    rows[1] = Vec2(0.0f, 1.0f);
  }

  Mat2::Mat2(const Vec2 &col1, const Vec2 &col2)
  {
    rows[0] = col1;
    rows[1] = col2;
  }

  // Matrix-matrix addition
  Mat2 Mat2::operator+(const Mat2 &other) const
  {
    return {rows[0] + other.rows[0], rows[1] + other.rows[1]};
  }

  // Matrix-matrix subtraction
  Mat2 Mat2::operator-(const Mat2 &other) const
  {
    return {rows[0] - other.rows[0], rows[1] - other.rows[1]};
  }

  // Matrix-matrix multiplication using SIMD
  Mat2 Mat2::operator*(const Mat2 &mat) const
  {
    Mat2 result;

    __m128 col0 = _mm_setr_ps(rows[0].x, rows[0].y, 0.0f, 0.0f);
    __m128 col1 = _mm_setr_ps(rows[1].x, rows[1].y, 0.0f, 0.0f);
    __m128 matCol0 = _mm_setr_ps(mat.rows[0].x, mat.rows[0].y, 0.0f, 0.0f);
    __m128 matCol1 = _mm_setr_ps(mat.rows[1].x, mat.rows[1].y, 0.0f, 0.0f);

    __m128 resCol0 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(col0, col0, _MM_SHUFFLE(0, 0, 0, 0)), matCol0),
                                _mm_mul_ps(_mm_shuffle_ps(col0, col0, _MM_SHUFFLE(1, 1, 1, 1)), matCol1));
    __m128 resCol1 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(col1, col1, _MM_SHUFFLE(0, 0, 0, 0)), matCol0),
                                _mm_mul_ps(_mm_shuffle_ps(col1, col1, _MM_SHUFFLE(1, 1, 1, 1)), matCol1));

    result.rows[0] = Vec2(resCol0);
    result.rows[1] = Vec2(resCol1);

    return result;
  }

  // Matrix-float multiplication
  Mat2 Mat2::operator*(float scalar) const
  {
    return {rows[0] * scalar, rows[1] * scalar};
  }

  // Matrix-vector multiplication
  Vec2 Mat2::operator*(const Vec2 &vec) const
  {
    __m128 vecData = vec.simdData;
    __m128 colData0 = rows[0].simdData;
    __m128 colData1 = rows[1].simdData;
    __m128 dotProduct = _mm_add_ps(_mm_mul_ps(vecData, colData0), _mm_mul_ps(vecData, colData1));
    return dotProduct;
  }

  // Matrix transposition
  Mat2 Mat2::transpose() const
  {
    return {Vec2(rows[0].x, rows[1].x), Vec2(rows[0].y, rows[1].y)};
  }

  // Matrix determinant
  float Mat2::determinant() const
  {
    __m128 a = rows[0].simdData;
    __m128 b = rows[1].simdData;

    __m128 det = _mm_sub_ps(_mm_mul_ps(a, _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 2, 0, 1))),
                            _mm_mul_ps(b, _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1))));

    float result;
    _mm_store_ss(&result, det);
    return result;
  }

  // Matrix inverse
  Mat2 Mat2::inverse() const
  {
    float det = determinant();
    if (det != 0.0f)
    {
      float invDet = 1.0f / det;

      return {Vec2(rows[1].y, -rows[0].y) * invDet,
              Vec2(-rows[1].x, rows[0].x) * invDet};
    }
    return Mat2();
  }

  // 3x3 matrix struct
  Mat3::Mat3()
  {
    rows[0] = Vec3(1.0f, 0.0f, 0.0f);
    rows[1] = Vec3(0.0f, 1.0f, 0.0f);
    rows[2] = Vec3(0.0f, 0.0f, 1.0f);
  }

  Mat3::Mat3(const Vec3 &col1, const Vec3 &col2, const Vec3 &col3)
  {
    rows[0] = col1;
    rows[1] = col2;
    rows[2] = col3;
  }

  // Matrix-matrix addition
  Mat3 Mat3::operator+(const Mat3 &other) const
  {
    return {rows[0] + other.rows[0], rows[1] + other.rows[1], rows[2] + other.rows[2]};
  }

  // Matrix-matrix subtraction
  Mat3 Mat3::operator-(const Mat3 &other) const
  {
    return {rows[0] - other.rows[0], rows[1] - other.rows[1], rows[2] - other.rows[2]};
  }

  // Matrix-matrix multiplication using SIMD
  Mat3 Mat3::operator*(const Mat3 &mat) const
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

    result.rows[0] = Vec3(resCol0);
    result.rows[1] = Vec3(resCol1);
    result.rows[2] = Vec3(resCol2);

    return result;
  }

  // Matrix-float multiplication
  Mat3 Mat3::operator*(float scalar) const
  {
    return {rows[0] * scalar, rows[1] * scalar, rows[2] * scalar};
  }

  // Matrix-vector multiplication
  Vec3 Mat3::operator*(const Vec3 &vec) const
  {
    // Load the vector into SIMD register
    __m128 vecSimd = _mm_setr_ps(vec.x, vec.y, vec.z, 0.0f);

    // Transpose the matrix
    __m128 row1 = _mm_load_ps(&rows[0].x);
    __m128 row2 = _mm_load_ps(&rows[1].x);
    __m128 row3 = _mm_load_ps(&rows[2].x);
    __m128 row4 = _mm_setzero_ps();
    _MM_TRANSPOSE4_PS(row1, row2, row3, row4);

    // Multiply the transposed matrix with the vector
    __m128 resultSimd = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(row1, _mm_shuffle_ps(vecSimd, vecSimd, _MM_SHUFFLE(0, 0, 0, 0))),
                   _mm_mul_ps(row2, _mm_shuffle_ps(vecSimd, vecSimd, _MM_SHUFFLE(1, 1, 1, 1)))),
        _mm_mul_ps(row3, _mm_shuffle_ps(vecSimd, vecSimd, _MM_SHUFFLE(2, 2, 2, 2))));

    // Store the result into a temporary array
    float temp[4];
    _mm_store_ps(temp, resultSimd);

    // Create and return the resulting Vec3
    return Vec3(temp[0], temp[1], temp[2]);
  }

  // Matrix transposition
  Mat3 Mat3::transpose() const
  {
    return {{rows[0].x, rows[1].x, rows[2].x},
            {rows[0].y, rows[1].y, rows[2].y},
            {rows[0].z, rows[1].z, rows[2].z}};
  }

  // Matrix determinant
  float Mat3::determinant() const
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
  Mat3 Mat3::inverse() const
  {
    float det = determinant();

    if (det == 0.0f)
    {
      // Matrix is not invertible, return identity matrix
      return Mat3();
    }

    float invDet = 1.0f / det;

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

  void print(const Mat3 &mat3)
  {
    for (int i = 0; i < 3; ++i)
    {
      printf("[ %.4f %.4f %.4f ]\n", mat3.rows[i].x, mat3.rows[i].y, mat3.rows[i].z);
    }
  }

  // 4x4 matrix struct
  Mat4::Mat4()
  {
    rows[0] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
    rows[1] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
    rows[2] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
    rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

  Mat4::Mat4(const Vec4 &col1, const Vec4 &col2, const Vec4 &col3, const Vec4 &col4)
  {
    rows[0] = col1;
    rows[1] = col2;
    rows[2] = col3;
    rows[3] = col4;
  }

  // Matrix-matrix addition
  Mat4 Mat4::operator+(const Mat4 &other) const
  {
    return {rows[0] + other.rows[0],
            rows[1] + other.rows[1],
            rows[2] + other.rows[2],
            rows[3] + other.rows[3]};
  }

  // Matrix-matrix subtraction
  Mat4 Mat4::operator-(const Mat4 &other) const
  {
    return {rows[0] - other.rows[0],
            rows[1] - other.rows[1],
            rows[2] - other.rows[2],
            rows[3] - other.rows[3]};
  }

  // Matrix-matrix multiplication using SIMD
  Mat4 Mat4::operator*(const Mat4 &mat) const
  {
    Mat4 result;

    for (int i = 0; i < 4; ++i)
    {
      __m128 row0 = _mm_set1_ps(rows[i].x);
      __m128 row1 = _mm_set1_ps(rows[i].y);
      __m128 row2 = _mm_set1_ps(rows[i].z);
      __m128 row3 = _mm_set1_ps(rows[i].w);

      __m128 col0 = mat.rows[0].simdData;
      __m128 col1 = mat.rows[1].simdData;
      __m128 col2 = mat.rows[2].simdData;
      __m128 col3 = mat.rows[3].simdData;

      __m128 prod0 = _mm_mul_ps(row0, col0);
      __m128 prod1 = _mm_mul_ps(row1, col1);
      __m128 prod2 = _mm_mul_ps(row2, col2);
      __m128 prod3 = _mm_mul_ps(row3, col3);

      __m128 sum0 = _mm_add_ps(prod0, prod1);
      __m128 sum1 = _mm_add_ps(prod2, prod3);
      __m128 sum2 = _mm_add_ps(sum0, sum1);

      result.rows[i].simdData = sum2;
    }

    return result;
  }

  // Matrix-float multiplication
  Mat4 Mat4::operator*(float scalar) const
  {
    return {rows[0] * scalar, rows[1] * scalar, rows[2] * scalar, rows[3] * scalar};
  }

  // Matrix-vector multiplication
  Vec4 Mat4::operator*(const Vec4 &vec) const
  {
    Vec4 result;

    __m128 dot0 = _mm_mul_ps(rows[0].simdData, vec.simdData);
    __m128 dot1 = _mm_mul_ps(rows[1].simdData, vec.simdData);
    __m128 dot2 = _mm_mul_ps(rows[2].simdData, vec.simdData);
    __m128 dot3 = _mm_mul_ps(rows[3].simdData, vec.simdData);

    __m128 xy = _mm_hadd_ps(dot0, dot1);
    __m128 zw = _mm_hadd_ps(dot2, dot3);

    result.simdData = _mm_hadd_ps(xy, zw);

    return result;
  }

  // Matrix transposition
  Mat4 Mat4::transpose() const
  {
    return {{rows[0].x, rows[1].x, rows[2].x, rows[3].x},
            {rows[0].y, rows[1].y, rows[2].y, rows[3].y},
            {rows[0].z, rows[1].z, rows[2].z, rows[3].z},
            {rows[0].w, rows[1].w, rows[2].w, rows[3].w}};
  }

  // Matrix determinant
  float Mat4::determinant() const
  {
    // float a11 = rows[0].x, a12 = rows[0].y, a13 = rows[0].z, a14 = rows[0].w;
    // float a21 = rows[1].x, a22 = rows[1].y, a23 = rows[1].z, a24 = rows[1].w;
    // float a31 = rows[2].x, a32 = rows[2].y, a33 = rows[2].z, a34 = rows[2].w;
    // float a41 = rows[3].x, a42 = rows[3].y, a43 = rows[3].z, a44 = rows[3].w;

    __m128 a3332 = _mm_set_ps(rows[2].y, rows[2].y, rows[2].z, rows[2].z);
    __m128 a4443 = _mm_set_ps(rows[3].z, rows[3].w, rows[3].w, rows[3].w);
    __m128 a3433 = _mm_set_ps(rows[2].z, rows[2].w, rows[2].w, rows[2].w);
    __m128 a4342 = _mm_set_ps(rows[3].y, rows[3].y, rows[3].z, rows[3].z);
    __m128 a3231 = _mm_set_ps(rows[2].x, rows[2].x, rows[2].x, rows[2].y);
    __m128 a4241 = _mm_set_ps(rows[3].x, rows[3].x, rows[3].x, rows[3].y);

    __m128 detM0 = _mm_sub_ps(_mm_mul_ps(a3332, a4443), _mm_mul_ps(a3433, a4342));
    __m128 detM1 = _mm_set_ps(rows[1].x, rows[1].x, rows[1].x, rows[1].y);
    __m128 detM10 = _mm_mul_ps(detM0, detM1);

    __m128 detM02 = _mm_sub_ps(_mm_mul_ps(a3231, a4443), _mm_mul_ps(a3433, a4241));
    __m128 detM2 = _mm_set_ps(rows[1].y, rows[1].y, rows[1].z, rows[1].z);
    __m128 detM20 = _mm_mul_ps(detM02, detM2);

    __m128 detM03 = _mm_sub_ps(_mm_mul_ps(a3231, a4342), _mm_mul_ps(a3332, a4241));
    __m128 detM3 = _mm_set_ps(rows[1].z, rows[1].w, rows[1].w, rows[1].w);
    __m128 detM30 = _mm_mul_ps(detM03, detM3);

    __m128 detM40 = _mm_add_ps(_mm_sub_ps(detM10, detM20), detM30);

    __m128 xmm0 = _mm_mul_ps(rows[0].simdData, detM40);

    __m128 det = _mm_hadd_ps(_mm_hsub_ps(xmm0, xmm0), _mm_hsub_ps(xmm0, xmm0));

    // float det =
    //     a11 * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42)) -
    //     a12 * (a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41)) +
    //     a13 * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41)) -
    //     a14 * (a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41));

    float result;
    _mm_store_ss(&result, det);
    return result;
  }

  // Matrix inverse
  Mat4 Mat4::inverse() const
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

    // Check if the determinant is zero
    if (fabs(det) < 0.0001f)
    {
      // Matrix is not invertible, return an identity matrix
      return Mat4();
    }

    // Calculate the adjugate matrix
    float invDet = 1.0f / det;
    Mat4 adjugate(
        Vec4(m11, -m21, m31, -m41),
        Vec4(-m12, m22, -m32, m42),
        Vec4(m13, -m23, m33, -m43),
        Vec4(-m14, m24, -m34, m44));

    // Multiply the adjugate matrix by the inverse determinant to get the inverse matrix
    return adjugate * invDet;
  }

  namespace UnitTests
  {

    // Function to compare floating-point values with a small epsilon
    bool floatEquals(float a, float b, float epsilon = 0.0001f)
    {
      return std::abs(a - b) < epsilon;
    }

    // Function to compare Vec2 objects
    bool vec2Equals(const Vec2 &v1, const Vec2 &v2)
    {
      return floatEquals(v1.x, v2.x) && floatEquals(v1.y, v2.y);
    }

    // Function to compare Vec3 objects
    bool vec3Equals(const Vec3 &v1, const Vec3 &v2)
    {
      return floatEquals(v1.x, v2.x) && floatEquals(v1.y, v2.y) && floatEquals(v1.z, v2.z);
    }

    // Function to compare Vec4 objects
    bool vec4Equals(const Vec4 &v1, const Vec4 &v2)
    {
      return floatEquals(v1.x, v2.x) && floatEquals(v1.y, v2.y) && floatEquals(v1.z, v2.z) && floatEquals(v1.w, v2.w);
    }

    // Function to compare Mat2 objects
    bool mat2Equals(const Mat2 &m1, const Mat2 &m2)
    {
      return vec2Equals(m1.rows[0], m2.rows[0]) && vec2Equals(m1.rows[1], m2.rows[1]);
    }

    // Function to compare Mat3 objects
    bool mat3Equals(const Mat3 &m1, const Mat3 &m2)
    {
      return vec3Equals(m1.rows[0], m2.rows[0]) && vec3Equals(m1.rows[1], m2.rows[1]) && vec3Equals(m1.rows[2], m2.rows[2]);
    }

    // Function to compare Mat4 objects
    bool mat4Equals(const Mat4 &m1, const Mat4 &m2)
    {
      return vec4Equals(m1.rows[0], m2.rows[0]) && vec4Equals(m1.rows[1], m2.rows[1]) && vec4Equals(m1.rows[2], m2.rows[2]) && vec4Equals(m1.rows[3], m2.rows[3]);
    }

    // Function to run the unit tests
    void Run()
    {
      // Test Vec2 operations
      Vec2 v2a(2.0f, 3.0f);
      Vec2 v2b(4.0f, 5.0f);

      // Addition
      Vec2 v2add = v2a + v2b;
      assert(vec2Equals(v2add, Vec2(6.0f, 8.0f)));

      // Subtraction
      Vec2 v2sub = v2a - v2b;
      assert(vec2Equals(v2sub, Vec2(-2.0f, -2.0f)));

      // Multiplication
      Vec2 v2mul = v2a * v2b;
      assert(vec2Equals(v2mul, Vec2(8.0f, 15.0f)));

      // Division
      Vec2 v2div = v2a / v2b;
      assert(vec2Equals(v2div, Vec2(0.5f, 0.6f)));

      // Dot Product
      float dotProduct2 = v2a.dot(v2b);
      assert(std::abs(dotProduct2 - 23.0f) < 0.0001f);

      // Magnitude
      float magnitude2 = v2a.magnitude();
      assert(std::abs(magnitude2 - 3.6056f) < 0.0001f);

      // Magnitude2
      float magnitude22 = v2a.magnitude2();
      assert(std::abs(magnitude22 - 13) < 0.0001f);

      // Normalization
      assert(vec2Equals(v2a.normalize(), {0.5547f, 0.832f}));

      // Test Vec3 operations
      Vec3 v3a(2.0f, 3.0f, 4.0f);
      Vec3 v3b(4.0f, 5.0f, 6.0f);

      // Addition
      Vec3 v3add = v3a + v3b;
      assert(vec3Equals(v3add, Vec3(6.0f, 8.0f, 10.0f)));

      // Subtraction
      Vec3 v3sub = v3a - v3b;
      assert(vec3Equals(v3sub, Vec3(-2.0f, -2.0f, -2.0f)));

      // Multiplication
      Vec3 v3mul = v3a * v3b;
      assert(vec3Equals(v3mul, Vec3(8.0f, 15.0f, 24.0f)));

      // Division
      Vec3 v3div = v3a / v3b;
      assert(vec3Equals(v3div, Vec3(0.5f, 0.6f, 0.6667f)));

      // Dot Product
      float dotProduct3 = v3a.dot(v3b);
      assert(std::abs(dotProduct3 - 47.0f) < 0.0001f);

      // Cross Product
      Vec3 crossProduct3 = v3a.cross(v3b);
      Vec3 expectedCrossProduct(-2.0f, 4.0f, -2.0f);
      assert(vec3Equals(crossProduct3, expectedCrossProduct));

      // Magnitude
      float magnitude3 = v3a.magnitude();
      assert(std::abs(magnitude3 - 5.38516f) < 0.0001f);

      // Magnitude2
      float magnitude32 = v3a.magnitude2();
      assert(std::abs(magnitude32 - (5.38516f * 5.38516f)) < 0.0001f);

      // Normalization
      assert(vec3Equals(v3a.normalize(), {0.3714f, 0.5571f, 0.7428f}));

      // Test Vec4 operations
      Vec4 v4a(2.0f, 3.0f, 4.0f, 5.0f);
      Vec4 v4b(4.0f, 5.0f, 6.0f, 7.0f);

      // Addition
      Vec4 v4add = v4a + v4b;
      assert(vec4Equals(v4add, Vec4(6.0f, 8.0f, 10.0f, 12.0f)));

      // Subtraction
      Vec4 v4sub = v4a - v4b;
      assert(vec4Equals(v4sub, Vec4(-2.0f, -2.0f, -2.0f, -2.0f)));

      // Multiplication
      Vec4 v4mul = v4a * v4b;
      assert(vec4Equals(v4mul, Vec4(8.0f, 15.0f, 24.0f, 35.0f)));

      // Division
      Vec4 v4div = v4a / v4b;
      assert(vec4Equals(v4div, Vec4(0.5f, 0.6f, 0.6667f, 0.7143f)));

      // Dot Product
      float dotProduct4 = v4a.dot(v4b);
      assert(std::abs(dotProduct4 - 82.f) < 0.0001f);

      // Magnitude
      float magnitude4 = v4a.magnitude();
      assert(std::abs(magnitude4 - 7.34847f) < 0.0001f);

      // Magnitude2
      float magnitude42 = v4a.magnitude2();
      assert(std::abs(magnitude42 - (7.34847f * 7.34847f)) < 0.0001f);

      // Normalization
      assert(vec4Equals(v4a.normalize(), {0.272165f, 0.408248f, 0.544331f, 0.680414f}));

      // Test Mat2 operations
      Mat2 m2a(Vec2(1.0f, 2.0f), Vec2(3.0f, 4.0f));
      Mat2 m2b(Vec2(5.0f, 6.0f), Vec2(7.0f, 8.0f));

      // Addition
      Mat2 m2add = m2a + m2b;
      assert(mat2Equals(m2add, Mat2(Vec2(6.0f, 8.0f), Vec2(10.0f, 12.0f))));

      // Subtraction
      Mat2 m2sub = m2a - m2b;
      assert(mat2Equals(m2sub, Mat2(Vec2(-4.0f, -4.0f), Vec2(-4.0f, -4.0f))));

      // Multiplication
      Mat2 m2mul = m2a * m2b;
      assert(mat2Equals(m2mul, Mat2(Vec2(19.0f, 22.0f), Vec2(43.0f, 50.0f))));

      // Scalar multiplication
      Mat2 m2scalar = m2a * 2.0f;
      assert(mat2Equals(m2scalar, Mat2(Vec2(2.0f, 4.0f), Vec2(6.0f, 8.0f))));

      // Matrix-vector multiplication
      Vec2 v2(2.0f, 3.0f);
      Vec2 m2vec = m2a * v2;
      assert(vec2Equals(m2vec, Vec2(8.0f, 18.0f)));

      // Transpose
      Mat2 m2transpose = m2a.transpose();
      assert(mat2Equals(m2transpose, Mat2(Vec2(1.0f, 3.0f), Vec2(2.0f, 4.0f))));

      // Determinant
      float m2det = m2a.determinant();
      assert(floatEquals(m2det, -2.0f));

      // Inverse
      Mat2 m2inverse = m2a.inverse();
      assert(mat2Equals(m2inverse, Mat2(Vec2(-2.0f, 1.0f), Vec2(1.5f, -0.5f))));

      // Test Mat3 operations
      Mat3 m3a(Vec3(1.0f, 2.0f, 3.0f), Vec3(4.0f, 5.0f, 6.0f), Vec3(7.0f, 8.0f, 9.0f));
      Mat3 m3b(Vec3(10.0f, 11.0f, 12.0f), Vec3(13.0f, 14.0f, 15.0f), Vec3(16.0f, 17.0f, 18.0f));
      Mat3 m3c(Vec3(1.0f, 2.0f, 0.0f), Vec3(0.0f, 1.0f, 2.0f), Vec3(2.0f, 0.0f, 1.0f));

      // Addition
      Mat3 m3add = m3a + m3b;
      assert(mat3Equals(m3add, Mat3(Vec3(11.0f, 13.0f, 15.0f), Vec3(17.0f, 19.0f, 21.0f), Vec3(23.0f, 25.0f, 27.0f))));

      // Subtraction
      Mat3 m3sub = m3a - m3b;
      assert(mat3Equals(m3sub, Mat3(Vec3(-9.0f, -9.0f, -9.0f), Vec3(-9.0f, -9.0f, -9.0f), Vec3(-9.0f, -9.0f, -9.0f))));

      // Multiplication
      Mat3 m3mul = m3a * m3b;
      assert(mat3Equals(m3mul, Mat3(Vec3(84.0f, 90.0f, 96.0f), Vec3(201.0f, 216.0f, 231.0f), Vec3(318.0f, 342.0f, 366.0f))));

      // Scalar multiplication
      Mat3 m3scalar = m3a * 2.0f;
      assert(mat3Equals(m3scalar, Mat3(Vec3(2.0f, 4.0f, 6.0f), Vec3(8.0f, 10.0f, 12.0f), Vec3(14.0f, 16.0f, 18.0f))));

      // Matrix-vector multiplication
      Vec3 v3(1.0f, 2.0f, 3.0f);
      Vec3 m3vecmul = m3a * v3;
      assert(vec3Equals(m3vecmul, Vec3(14.0f, 32.0f, 50.0f)));

      // Transpose
      Mat3 m3transpose = m3a.transpose();
      assert(mat3Equals(m3transpose, Mat3(Vec3(1.0f, 4.0f, 7.0f), Vec3(2.0f, 5.0f, 8.0f), Vec3(3.0f, 6.0f, 9.0f))));

      // Determinant
      float det = m3c.determinant();
      assert(floatEquals(det, 9.0f)); // Example determinant value, adjust accordingly

      // Inverse
      Mat3 m3inverse = m3c.inverse();
      assert(mat3Equals(m3inverse, Mat3(Vec3(0.11111111111111111112f, -0.2222222222222222223f, 0.44444444444444444444f),
                                        Vec3(0.44444444444444444444f, 0.11111111111111111112f, -0.22222222222222222222f),
                                        Vec3(-0.22222222222222222222f, 0.44444444444444444444f, 0.11111111111111111111f))));

      // Test Mat4 operations
      Mat4 m4a(Vec4(1.0f, 2.0f, 3.0f, 4.0f),
               Vec4(5.0f, 6.0f, 7.0f, 8.0f),
               Vec4(9.0f, 10.0f, 11.0f, 12.0f),
               Vec4(13.0f, 14.0f, 15.0f, 16.0f));
      Mat4 m4b(Vec4(17.0f, 18.0f, 19.0f, 20.0f),
               Vec4(21.0f, 22.0f, 23.0f, 24.0f),
               Vec4(25.0f, 26.0f, 27.0f, 28.0f),
               Vec4(29.0f, 30.0f, 31.0f, 32.0f));
      Mat4 m4c(Vec4(0.0f, 2.0f, 3.0f, 4.0f),
               Vec4(5.0f, -6.0f, 7.0f, 8.0f),
               Vec4(9.0f, 10.0f, 11.0f, 12.0f),
               Vec4(13.0f, 7.0f, 15.0f, 17.0f));
      // Addition
      Mat4 m4add = m4a + m4b;
      assert(mat4Equals(m4add, Mat4(Vec4(18.0f, 20.0f, 22.0f, 24.0f), Vec4(26.0f, 28.0f, 30.0f, 32.0f), Vec4(34.0f, 36.0f, 38.0f, 40.0f), Vec4(42.0f, 44.0f, 46.0f, 48.0f))));

      // Subtraction
      Mat4 m4sub = m4a - m4b;
      assert(mat4Equals(m4sub, Mat4(Vec4(-16.0f, -16.0f, -16.0f, -16.0f), Vec4(-16.0f, -16.0f, -16.0f, -16.0f), Vec4(-16.0f, -16.0f, -16.0f, -16.0f), Vec4(-16.0f, -16.0f, -16.0f, -16.0f))));

      // Multiplication
      Mat4 m4mul = m4a * m4b;
      assert(mat4Equals(m4mul, Mat4(Vec4(250.0f, 260.0f, 270.0f, 280.0f), Vec4(618.0f, 644.0f, 670.0f, 696.0f), Vec4(986.0f, 1028.0f, 1070.0f, 1112.0f), Vec4(1354.0f, 1412.0f, 1470.0f, 1528.0f))));

      // Test scalar multiplication
      float scalar = 2.0f;
      Mat4 mScalarMul = m4a * scalar;
      assert(mat4Equals(mScalarMul, {Vec4(2.0f, 4.0f, 6.0f, 8.0f),
                                     Vec4(10.0f, 12.0f, 14.0f, 16.0f),
                                     Vec4(18.0f, 20.0f, 22.0f, 24.0f),
                                     Vec4(26.0f, 28.0f, 30.0f, 32.0f)}));

      // Test matrix-vector multiplication
      Vec4 vec4(1.0f, 2.0f, 3.0f, 4.0f);
      Vec4 m4vecmul = m4a * vec4;
      assert(vec4Equals(m4vecmul, Vec4(30.0f, 70.0f, 110.0f, 150.0f)));

      // Test matrix transpose
      Mat4 mTranspose4 = m4a.transpose();
      assert(mat4Equals(mTranspose4, {Vec4(1.0f, 5.0f, 9.0f, 13.0f),
                                      Vec4(2.0f, 6.0f, 10.0f, 14.0f),
                                      Vec4(3.0f, 7.0f, 11.0f, 15.0f),
                                      Vec4(4.0f, 8.0f, 12.0f, 16.0f)}));

      // Test matrix determinant
      float det4 = m4a.determinant();
      assert(det4 == 0.0f);
      float det42 = m4c.determinant();
      assert(det42 == 252.0f);

      // Test matrix inverse
      Mat4 mInverse4 = m4a.inverse();
      assert(mat4Equals(mInverse4, {}));
      Mat4 mInverse42 = m4c.inverse();
      assert(mat4Equals(mInverse42, {Vec4(-0.23809523809523809478f, -0.19047619047619047692f, -0.33333333333333333472f, 0.38095238095238095362f),
                                     Vec4(0.031746031746031746008f, -0.09126984126984126981f, 0.027777777777777777841f, 0.015873015873015872959f),
                                     Vec4(-0.49206349206349206618f, 1.0396825396825396868f, 1.9444444444444444526f, -1.746031746031746039f),
                                     Vec4(0.60317460317460317664, -0.73412698412698413022, -1.4722222222222222284, 1.3015873015873015928f)}));
      std::cout << "All unit tests passed!\n";
    }
  }
}

#pragma GCC diagnostic pop
