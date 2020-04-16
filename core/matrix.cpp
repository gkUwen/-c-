#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>

namespace rt {

Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4) {
    this->r1 = r1;
    this->r2 = r2;
    this->r3 = r3;
    this->r4 = r4;
}

Float4& Matrix::operator[](int idx) {
    if (idx == 0) return this->r1;
    if (idx == 1) return this->r2;
    if (idx == 2) return this->r3;
    if (idx == 3) return this->r4;
}

Float4 Matrix::operator[](int idx) const {
    if (idx == 0) return this->r1;
    if (idx == 1) return this->r2;
    if (idx == 2) return this->r3;
    if (idx == 3) return this->r4;
}

Matrix Matrix::operator+(const Matrix& b) const {
    return Matrix(Float4(this->r1 + b.r1),
                  Float4(this->r2 + b.r2),
                  Float4(this->r3 + b.r3),
                  Float4(this->r4 + b.r4));
}

Matrix Matrix::operator-(const Matrix& b) const {
    return Matrix(Float4(this->r1 - b.r1),
                  Float4(this->r2 - b.r2),
                  Float4(this->r3 - b.r3),
                  Float4(this->r4 - b.r4));
}

Matrix Matrix::transpose() const {
    return Matrix(Float4(r1.x, r2.x, r3.x, r4.x),
                  Float4(r1.y, r2.y, r3.y, r4.y),
                  Float4(r1.z, r2.z, r3.z, r4.z),
                  Float4(r1.w, r2.w, r3.w, r4.w));
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
    return (this->r1 == b.r1 && this->r2 == b.r2 && this->r3 == b.r3 && this->r4 == b.r4);
}

bool Matrix::operator!=(const Matrix& b) const {
    return (this->r1 != b.r1 || this->r2 != b.r2 || this->r3 != b.r3 || this->r4 != b.r4);
}

Matrix product(const Matrix& a, const Matrix& b) {
     
     Matrix __product = Matrix::zero();
     Matrix __b_transpose = b.transpose();

     for (int i = 0; i < 4; i++){
         for (int j = 0; j < 4; j++){
             __product[i][j] = dot(a[i], __b_transpose[j]);
         }
     }

     return __product;

}

Matrix translation(Point& t)
	{
		return Matrix(Float4(1.0f, 0.0f, 0.0f, t.x),
			Float4(0.0f, 1.0f, 0.0f, t.y),
			Float4(0.0f, 0.0f, 1.0f, t.z),
			Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}

Matrix operator*(const Matrix& a, float scalar) {
    return Matrix(Float4(a.r1*scalar),
                  Float4(a.r2*scalar),
                  Float4(a.r3*scalar),
                  Float4(a.r4*scalar));
}

Matrix operator*(float scalar, const Matrix& a) {
    return Matrix(Float4(scalar*a.r1),
                  Float4(scalar*a.r2),
                  Float4(scalar*a.r3),
                  Float4(scalar*a.r4));
}

Float4 Matrix::operator*(const Float4& b) const {
    return Float4(dot(this->r1, b), dot(this->r2, b), dot(this->r3, b), dot(this->r4, b));
}

Vector Matrix::operator*(const Vector& b) const { 
    return Vector(dot(this->r1, Float4(b)), dot(this->r2, Float4(b)), dot(this->r3, Float4(b)));
}

Point Matrix::operator*(const Point& b) const {
    Float4 __point = Float4(b);
    return Point(Float4(dot(this->r1, __point), dot(this->r2, __point), dot(this->r3, __point), dot(this->r4, __point)));
}

float Matrix::det() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix =))
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    
    return det;
}

Matrix Matrix::zero() {
    return Matrix(Float4(0.0f, 0.0f, 0.0f ,0.0f),
                  Float4(0.0f, 0.0f, 0.0f ,0.0f),
                  Float4(0.0f, 0.0f, 0.0f ,0.0f),
                  Float4(0.0f, 0.0f, 0.0f ,0.0f));
}

Matrix Matrix::identity() {

    return Matrix(Float4(1.0f, 0.0f, 0.0f ,0.0f),
                  Float4(0.0f, 1.0f, 0.0f ,0.0f),
                  Float4(0.0f, 0.0f, 1.0f ,0.0f),
                  Float4(0.0f, 0.0f, 0.0f ,1.0f));
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    return Matrix(Float4(e1),
                  Float4(e2),
                  Float4(e3),
                  Float4(0.0f, 0.0f,0.0f, 1.0f)).transpose();
}

}