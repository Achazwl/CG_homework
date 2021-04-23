#ifndef VECIO_H
#define VECIO_H

#include "Vector2f.h"
#include "Vector3f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"
#include <ostream>
#include <istream>

std::ostream& operator<<(std::ostream& os, const Vector2f& v);
std::ostream& operator<<(std::ostream& os, const Vector3f& v);

std::ostream& operator<<(std::ostream& os, const Matrix3f& v);
std::ostream& operator<<(std::ostream& os, const Matrix4f& v);

#endif //VECIO_H
