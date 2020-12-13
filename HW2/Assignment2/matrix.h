#include "math.h"
#include "rotation.h"
#include "scaling.h"
#include "translation.h"
#include "vector4f.h"
#include "vector3f.h"
#include <iostream>

#ifndef MATRIX_H
#define MATRIX_H
typedef enum axis_type {
    X,
    Y,
    Z
} AXIS;

typedef enum trans_type {
    SCALE,
    TRANSLATE,
    ROTATE
} TRANSFORMATION;


class Matrix{
    
    public:
        void getTranslationMatrix(float tx, float ty, float tz) {
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = tx;

            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;
            m[1][3] = ty;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
            m[2][3] = tz;

            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }
        void getScaleMatrix(float sx, float sy, float sz) {
            m[0][0] = sx;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = 0;

            m[1][0] = 0;
            m[1][1] = sy;
            m[1][2] = 0;
            m[1][3] = 0;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = sz;
            m[2][3] = 0;

            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }

        void getRotationMatrix(float rx, float ry, float rz, float angle) {
            fst::math::Vector3f axis (rx, ry, rz);
            Matrix M = Matrix::getAltRot(axis);
            Matrix MInv = Matrix::getAltInvRot(axis);
            Matrix RxTheta = Matrix::getRx(angle);
            (*this) = MInv * RxTheta * M;
        }
        
        static Matrix getAltRot(fst::math::Vector3f& u) {
            u = fst::math::normalize(u);
            fst::math::Vector3f v = fst::math::getOrthogonal(u);
            fst::math::Vector3f w = fst::math::cross(u,v);
            v = fst::math::normalize(v);
            w = fst::math::normalize(w);
            Matrix m;
            m.m[0][0] = u.x;
            m.m[0][1] = u.y;
            m.m[0][2] = u.z;
            m.m[0][3] = 0;

            m.m[1][0] = v.x;
            m.m[1][1] = v.y;
            m.m[1][2] = v.z;
            m.m[1][3] = 0;

            m.m[2][0] = w.x;
            m.m[2][1] = w.y;
            m.m[2][2] = w.z;
            m.m[2][3] = 0;

            m.m[3][0] = 0;
            m.m[3][1] = 0;
            m.m[3][2] = 0;
            m.m[3][3] = 1;
            return m;
        }
        static Matrix getAltInvRot(fst::math::Vector3f& u) {
            u = fst::math::normalize(u);
            fst::math::Vector3f v = fst::math::getOrthogonal(u);
            fst::math::Vector3f w = fst::math::cross(u,v);
            v = fst::math::normalize(v);
            w = fst::math::normalize(w);
            Matrix m;

            m.m[0][0] = u.x;
            m.m[0][1] = v.x;
            m.m[0][2] = w.x;
            m.m[0][3] = 0;

            m.m[1][0] = u.y;
            m.m[1][1] = v.y;
            m.m[1][2] = w.y;
            m.m[1][3] = 0;

            m.m[2][0] = u.z;
            m.m[2][1] = v.z;
            m.m[2][2] = w.z;
            m.m[2][3] = 0;

            m.m[3][0] = 0;
            m.m[3][1] = 0;
            m.m[3][2] = 0;
            m.m[3][3] = 1;
            return m;
        }
        static Matrix getRx(float angle) {
            float theta = angle * M_PI / 180;
        
            Matrix m;

            m.m[0][0] = 1;
            m.m[0][1] = 0;
            m.m[0][2] = 0;
            m.m[0][3] = 0;

            m.m[1][0] = 0;
            m.m[1][1] = cos(theta);
            m.m[1][2] = -sin(theta);
            m.m[1][3] = 0;

            m.m[2][0] = 0;
            m.m[2][1] = sin(theta);
            m.m[2][2] = cos(theta);
            m.m[2][3] = 0;

            m.m[3][0] = 0;
            m.m[3][1] = 0;
            m.m[3][2] = 0;
            m.m[3][3] = 1;
            return m;
        }
    
    public:

        float m[4][4];
        Matrix() {}
        Matrix(float x, float y, float z,  TRANSFORMATION t, float angle = 0) {
            if ( t == SCALE ) {
                this -> getScaleMatrix(x, y ,z); 
            } else if ( t== ROTATE) {
                this -> getRotationMatrix(x, y, z, angle);
            }
            else {
                this -> getTranslationMatrix(x, y, z);
            }
        }
        Matrix (const Matrix& cpy) {
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++) {
                    this -> m[i][j] = cpy.m[i][j];
                }
            }
        }
        ~Matrix() {}

        Matrix operator*(const Matrix& rhs){
            Matrix output;
            
            output.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
            output.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
            output.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
            output.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];

            output.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
            output.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
            output.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
            output.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];

            output.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
            output.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
            output.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
            output.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];

            output.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
            output.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
            output.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
            output.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];
            return output;
        }

        void operator=(const Matrix& rhs) {
            Matrix out;
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++) {
                    this -> m[i][j] = rhs.m[i][j];
                }
            }
        }
        fst::math::Vector4f operator*(const fst::math::Vector4f& rhs){
            fst::math::Vector4f output;
            
            output.x = m[0][0] * rhs.x + m[0][1] * rhs.y + m[0][2] * rhs.z + m[0][3] * rhs.w;
            output.y = m[1][0] * rhs.x + m[1][1] * rhs.y + m[1][2] * rhs.z + m[1][3] * rhs.w;
            output.z = m[2][0] * rhs.x + m[2][1] * rhs.y + m[2][2] * rhs.z + m[2][3] * rhs.w;
            output.w = m[3][0] * rhs.x + m[3][1] * rhs.y + m[3][2] * rhs.z + m[3][3] * rhs.w;

            return output;
        }

        friend std::ostream& operator<<(std::ostream& os, const Matrix &m) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    os << m.m[i][j] << " ";
                }
                os << std::endl;
            }
            return os;
        }                                                                                                          
};

#endif