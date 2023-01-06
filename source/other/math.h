#pragma once

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

template <size_t N, typename T>
struct Vector {
    T m[N];  // 元素数组
    Vector() {
        for (size_t i = 0; i < N; i++) m[i] = T();
    }
    explicit Vector(const T *ptr) {
        for (size_t i = 0; i < N; i++) m[i] = ptr[i];
    }
    Vector(const Vector<N, T> &u) {
        for (size_t i = 0; i < N; i++) m[i] = u.m[i];
    }
    Vector(const std::initializer_list<T> &u) {
        auto it = u.begin();
        for (size_t i = 0; i < N; i++) m[i] = *it++;
    }
    const T &operator[](size_t i) const {
        assert(i < N);
        return m[i];
    }
    T &operator[](size_t i) {
        assert(i < N);
        return m[i];
    }
    void load(const T *ptr) {
        for (size_t i = 0; i < N; i++) m[i] = ptr[i];
    }
    void save(T *ptr) {
        for (size_t i = 0; i < N; i++) ptr[i] = m[i];
    }
};

// 特化二维矢量
template <typename T>
struct Vector<2, T> {
    union {
        struct {
            T x, y;
        };  // 元素别名
        struct {
            T u, v;
        };       // 元素别名
        T m[2];  // 元素数组
    };
    Vector() : x(T()), y(T()) {}
    Vector(T X, T Y) : x(X), y(Y) {}
    Vector(const Vector<2, T> &u) : x(u.x), y(u.y) {}
    explicit Vector(const T *ptr) : x(ptr[0]), y(ptr[1]) {}
    const T &operator[](size_t i) const {
        assert(i < 2);
        return m[i];
    }
    T &operator[](size_t i) {
        assert(i < 2);
        return m[i];
    }
    void load(const T *ptr) {
        for (size_t i = 0; i < 2; i++) m[i] = ptr[i];
    }
    void save(T *ptr) {
        for (size_t i = 0; i < 2; i++) ptr[i] = m[i];
    }
    Vector<2, T> xy() const { return *this; }
    Vector<3, T> xy1() const { return Vector<3, T>(x, y, 1); }
    Vector<4, T> xy11() const { return Vector<4, T>(x, y, 1, 1); }
};

// 特化三维矢量
template <typename T>
struct Vector<3, T> {
    union {
        struct {
            T x, y, z;
        };  // 元素别名
        struct {
            T r, g, b;
        };       // 元素别名
        T m[3];  // 元素数组
    };
    Vector() : x(T()), y(T()), z(T()) {}
    Vector(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    Vector(const Vector<3, T> &u) : x(u.x), y(u.y), z(u.z) {}
    explicit Vector(const T *ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]) {}
    const T &operator[](size_t i) const {
        assert(i < 3);
        return m[i];
    }
    T &operator[](size_t i) {
        assert(i < 3);
        return m[i];
    }
    void load(const T *ptr) {
        for (size_t i = 0; i < 3; i++) m[i] = ptr[i];
    }
    void save(T *ptr) {
        for (size_t i = 0; i < 3; i++) ptr[i] = m[i];
    }
    Vector<2, T> xy() const { return Vector<2, T>(x, y); }
    Vector<3, T> xyz() const { return *this; }
    Vector<4, T> xyz1() const { return Vector<4, T>(x, y, z, 1); }
};

// 特化四维矢量
template <typename T>
struct Vector<4, T> {
    union {
        struct {
            T x, y, z, w;
        };  // 元素别名
        struct {
            T r, g, b, a;
        };       // 元素别名
        T m[4];  // 元素数组
    };
    Vector() : x(T()), y(T()), z(T()), w(T()) {}
    Vector(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
    Vector(const Vector<4, T> &u) : x(u.x), y(u.y), z(u.z), w(u.w) {}
    explicit Vector(const T *ptr)
        : x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {}
    const T &operator[](size_t i) const {
        assert(i < 4);
        return m[i];
    }
    T &operator[](size_t i) {
        assert(i < 4);
        return m[i];
    }
    void load(const T *ptr) {
        for (size_t i = 0; i < 4; i++) m[i] = ptr[i];
    }
    void save(T *ptr) {
        for (size_t i = 0; i < 4; i++) ptr[i] = m[i];
    }
    Vector<2, T> xy() const { return Vector<2, T>(x, y); }
    Vector<3, T> xyz() const { return Vector<3, T>(x, y, z); }
    Vector<4, T> xyzw() const { return *this; }
};

//---------------------------------------------------------------------
// 数学库：矢量运算
//---------------------------------------------------------------------

// = (+a)
template <size_t N, typename T>
Vector<N, T> operator+(const Vector<N, T> &a) {
    return a;
}

// = (-a)
template <size_t N, typename T>
Vector<N, T> operator-(const Vector<N, T> &a) {
    Vector<N, T> b;
    for (size_t i = 0; i < N; i++) b[i] = -a[i];
    return b;
}

// = (a == b) ? true : false
template <size_t N, typename T>
bool operator==(const Vector<N, T> &a, const Vector<N, T> &b) {
    for (size_t i = 0; i < N; i++)
        if (a[i] != b[i]) return false;
    return true;
}

// = (a != b)? true : false
template <size_t N, typename T>
bool operator!=(const Vector<N, T> &a, const Vector<N, T> &b) {
    return !(a == b);
}

// = a + b
template <size_t N, typename T>
Vector<N, T> operator+(const Vector<N, T> &a, const Vector<N, T> &b) {
    Vector<N, T> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] + b[i];
    return c;
}

// = a - b
template <size_t N, typename T>
Vector<N, T> operator-(const Vector<N, T> &a, const Vector<N, T> &b) {
    Vector<N, T> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] - b[i];
    return c;
}

// = a * b，不是点乘也不是叉乘，而是各个元素分别相乘，色彩计算时有用
template <size_t N, typename T>
Vector<N, T> operator*(const Vector<N, T> &a, const Vector<N, T> &b) {
    Vector<N, T> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] * b[i];
    return c;
}

// = a / b，各个元素相除
template <size_t N, typename T>
Vector<N, T> operator/(const Vector<N, T> &a, const Vector<N, T> &b) {
    Vector<N, T> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] / b[i];
    return c;
}

// = a * x
template <size_t N, typename T>
Vector<N, T> operator*(const Vector<N, T> &a, T x) {
    Vector<N, T> b;
    for (size_t i = 0; i < N; i++) b[i] = a[i] * x;
    return b;
}

// = x * a
template <size_t N, typename T>
Vector<N, T> operator*(T x, const Vector<N, T> &a) {
    Vector<N, T> b;
    for (size_t i = 0; i < N; i++) b[i] = a[i] * x;
    return b;
}

// = a / x
template <size_t N, typename T>
Vector<N, T> operator/(const Vector<N, T> &a, T x) {
    Vector<N, T> b;
    for (size_t i = 0; i < N; i++) b[i] = a[i] / x;
    return b;
}

// = x / a
template <size_t N, typename T>
Vector<N, T> operator/(T x, const Vector<N, T> &a) {
    Vector<N, T> b;
    for (size_t i = 0; i < N; i++) b[i] = x / a[i];
    return b;
}

// a += b
template <size_t N, typename T>
Vector<N, T> &operator+=(Vector<N, T> &a, const Vector<N, T> &b) {
    for (size_t i = 0; i < N; i++) a[i] += b[i];
    return a;
}

// a -= b
template <size_t N, typename T>
Vector<N, T> &operator-=(Vector<N, T> &a, const Vector<N, T> &b) {
    for (size_t i = 0; i < N; i++) a[i] -= b[i];
    return a;
}

// a *= b
template <size_t N, typename T>
Vector<N, T> &operator*=(Vector<N, T> &a, const Vector<N, T> &b) {
    for (size_t i = 0; i < N; i++) a[i] *= b[i];
    return a;
}

// a /= b
template <size_t N, typename T>
Vector<N, T> &operator/=(Vector<N, T> &a, const Vector<N, T> &b) {
    for (size_t i = 0; i < N; i++) a[i] /= b[i];
    return a;
}

// a *= x
template <size_t N, typename T>
Vector<N, T> &operator*=(Vector<N, T> &a, T x) {
    for (size_t i = 0; i < N; i++) a[i] *= x;
    return a;
}

// a /= x
template <size_t N, typename T>
Vector<N, T> &operator/=(Vector<N, T> &a, T x) {
    for (size_t i = 0; i < N; i++) a[i] /= x;
    return a;
}

//---------------------------------------------------------------------
// 数学库：矢量函数
//---------------------------------------------------------------------

// 不同维度的矢量转换
template <size_t N1, size_t N2, typename T>
Vector<N1, T> vector_convert(const Vector<N2, T> &a, T fill = 1) {
    Vector<N1, T> b;
    for (size_t i = 0; i < N1; i++) b[i] = (i < N2) ? a[i] : fill;
    return b;
}

// = |a| ^ 2
template <size_t N, typename T>
T vector_length_square(const Vector<N, T> &a) {
    T sum = 0;
    for (size_t i = 0; i < N; i++) sum += a[i] * a[i];
    return sum;
}

// = |a|
template <size_t N, typename T>
T vector_length(const Vector<N, T> &a) {
    return sqrt(vector_length_square(a));
}

// = |a| , 特化 float 类型，使用 sqrtf
template <size_t N>
float vector_length(const Vector<N, float> &a) {
    return sqrtf(vector_length_square(a));
}

// = a / |a|
template <size_t N, typename T>
Vector<N, T> vector_normalize(const Vector<N, T> &a) {
    return a / vector_length(a);
}

// 矢量点乘
template <size_t N, typename T>
T vector_dot(const Vector<N, T> &a, const Vector<N, T> &b) {
    T sum = 0;
    for (size_t i = 0; i < N; i++) sum += a[i] * b[i];
    return sum;
}

// 二维矢量叉乘，得到标量
template <typename T>
T vector_cross(const Vector<2, T> &a, const Vector<2, T> &b) {
    return a.x * b.y - a.y * b.x;
}

// 三维矢量叉乘，得到新矢量
template <typename T>
Vector<3, T> vector_cross(const Vector<3, T> &a, const Vector<3, T> &b) {
    return Vector<3, T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x);
}

// 四维矢量叉乘：前三维叉乘，后一位保留
template <typename T>
Vector<4, T> vector_cross(const Vector<4, T> &a, const Vector<4, T> &b) {
    return Vector<4, T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x, a.w);
}

// = a + (b - a) * t
template <size_t N, typename T>
Vector<N, T> vector_lerp(const Vector<N, T> &a, const Vector<N, T> &b,
                         float t) {
    return a + (b - a) * t;
}

// 各个元素取最大值
template <size_t N, typename T>
Vector<N, T> vector_max(const Vector<N, T> &a, const Vector<N, T> &b) {
    Vector<N, T> c;
    for (size_t i = 0; i < N; i++) c[i] = (a[i] > b[i]) ? a[i] : b[i];
    return c;
}

// 各个元素取最小值
template <size_t N, typename T>
Vector<N, T> vector_min(const Vector<N, T> &a, const Vector<N, T> &b) {
    Vector<N, T> c;
    for (size_t i = 0; i < N; i++) c[i] = (a[i] < b[i]) ? a[i] : b[i];
    return c;
}

// 将矢量的值控制在 minx/maxx 范围内
template <size_t N, typename T>
Vector<N, T> vector_between(const Vector<N, T> &minx, const Vector<N, T> &maxx,
                            const Vector<N, T> &x) {
    return vector_min(vector_max(minx, x), maxx);
}

// 判断矢量是否接近
template <size_t N, typename T>
bool vector_near(const Vector<N, T> &a, const Vector<N, T> &b, T dist) {
    return (vector_length_square(a - b) <= dist);
}

// 判断两个单精度矢量是否近似
template <size_t N>
bool vector_near_equal(const Vector<N, float> &a, const Vector<N, float> &b,
                       float e = 0.0001) {
    return vector_near(a, b, e);
}

// 判断两个双精度矢量是否近似
template <size_t N>
bool vector_near_equal(const Vector<N, double> &a, const Vector<N, double> &b,
                       double e = 0.0000001) {
    return vector_near(a, b, e);
}

// 矢量值元素范围裁剪
template <size_t N, typename T>
Vector<N, T> vector_clamp(const Vector<N, T> &a, T minx = 0, T maxx = 1) {
    Vector<N, T> b;
    for (size_t i = 0; i < N; i++) {
        T x = (a[i] < minx) ? minx : a[i];
        b[i] = (x > maxx) ? maxx : x;
    }
    return b;
}

// 输出到文本流
template <size_t N, typename T>
std::ostream &operator<<(std::ostream &os, const Vector<N, T> &a) {
    os << "[";
    for (size_t i = 0; i < N; i++) {
        os << a[i];
        if (i < N - 1) os << ", ";
    }
    os << "]";
    return os;
}

// 输出成字符串
template <size_t N, typename T>
std::string vector_repr(const Vector<N, T> &a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
}

//---------------------------------------------------------------------
// 数学库：矩阵
//---------------------------------------------------------------------
template <size_t ROW, size_t COL, typename T>
struct Matrix {
    T m[ROW][COL];

    Matrix() = default;

    Matrix(const Matrix<ROW, COL, T> &src) {
        for (size_t r = 0; r < ROW; r++) {
            for (size_t c = 0; c < COL; c++) m[r][c] = src.m[r][c];
        }
    }

    Matrix(const std::initializer_list<Vector<COL, T>> &u) {
        auto it = u.begin();
        for (size_t i = 0; i < ROW; i++) SetRow(i, *it++);
    }

    const T *operator[](size_t row) const {
        assert(row < ROW);
        return m[row];
    }
    T *operator[](size_t row) {
        assert(row < ROW);
        return m[row];
    }

    // 取一行
    Vector<COL, T> Row(size_t row) const {
        assert(row < ROW);
        Vector<COL, T> a;
        for (size_t i = 0; i < COL; i++) a[i] = m[row][i];
        return a;
    }

    // 取一列
    Vector<ROW, T> Col(size_t col) const {
        assert(col < COL);
        Vector<ROW, T> a;
        for (size_t i = 0; i < ROW; i++) a[i] = m[i][col];
        return a;
    }

    // 设置一行
    void SetRow(size_t row, const Vector<COL, T> &a) {
        assert(row < ROW);
        for (size_t i = 0; i < COL; i++) m[row][i] = a[i];
    }

    // 设置一列
    void SetCol(size_t col, const Vector<ROW, T> &a) {
        assert(col < COL);
        for (size_t i = 0; i < ROW; i++) m[i][col] = a[i];
    }

    // 取得删除某行和某列的子矩阵：子式
    Matrix<ROW - 1, COL - 1, T> GetMinor(size_t row, size_t col) const {
        Matrix<ROW - 1, COL - 1, T> ret;
        for (size_t r = 0; r < ROW - 1; r++) {
            for (size_t c = 0; c < COL - 1; c++) {
                ret.m[r][c] = m[r < row ? r : r + 1][c < col ? c : c + 1];
            }
        }
        return ret;
    }

    // 取得转置矩阵
    Matrix<COL, ROW, T> Transpose() const {
        Matrix<COL, ROW, T> ret;
        for (size_t r = 0; r < ROW; r++) {
            for (size_t c = 0; c < COL; c++) ret.m[c][r] = m[r][c];
        }
        return ret;
    }

    // 取得 0 矩阵
    static Matrix<ROW, COL, T> GetZero() {
        Matrix<ROW, COL, T> ret;
        for (size_t r = 0; r < ROW; r++) {
            for (size_t c = 0; c < COL; c++) ret.m[r][c] = 0;
        }
        return ret;
    }

    // 取得单位矩阵
    static Matrix<ROW, COL, T> GetIdentity() {
        Matrix<ROW, COL, T> ret;
        for (size_t r = 0; r < ROW; r++) {
            for (size_t c = 0; c < COL; c++) ret.m[r][c] = (r == c) ? 1 : 0;
        }
        return ret;
    }
};

//---------------------------------------------------------------------
// 数学库：矩阵运算
//---------------------------------------------------------------------
template <size_t ROW, size_t COL, typename T>
bool operator==(const Matrix<ROW, COL, T> &a, const Matrix<ROW, COL, T> &b) {
    for (size_t r = 0; r < ROW; r++) {
        for (size_t c = 0; c < COL; c++) {
            if (a.m[r][c] != b.m[r][c]) return false;
        }
    }
    return true;
}

template <size_t ROW, size_t COL, typename T>
bool operator!=(const Matrix<ROW, COL, T> &a, const Matrix<ROW, COL, T> &b) {
    return !(a == b);
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T> &src) {
    return src;
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T> &src) {
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++) {
        for (size_t i = 0; i < COL; i++) out.m[j][i] = -src.m[j][i];
    }
    return out;
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T> &a,
                              const Matrix<ROW, COL, T> &b) {
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++) {
        for (size_t i = 0; i < COL; i++) out.m[j][i] = a.m[j][i] + b.m[j][i];
    }
    return out;
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T> &a,
                              const Matrix<ROW, COL, T> &b) {
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++) {
        for (size_t i = 0; i < COL; i++) out.m[j][i] = a.m[j][i] - b.m[j][i];
    }
    return out;
}

template <size_t ROW, size_t COL, size_t NEWCOL, typename T>
Matrix<ROW, NEWCOL, T> operator*(const Matrix<ROW, COL, T> &a,
                                 const Matrix<COL, NEWCOL, T> &b) {
    Matrix<ROW, NEWCOL, T> out;
    for (size_t j = 0; j < ROW; j++) {
        for (size_t i = 0; i < NEWCOL; i++) {
            out.m[j][i] = vector_dot(a.Row(j), b.Col(i));
        }
    }
    return out;
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator*(const Matrix<ROW, COL, T> &a, T x) {
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++) {
        for (size_t i = 0; i < COL; i++) {
            out.m[j][i] = a.m[j][i] * x;
        }
    }
    return out;
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator/(const Matrix<ROW, COL, T> &a, T x) {
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++) {
        for (size_t i = 0; i < COL; i++) {
            out.m[j][i] = a.m[j][i] / x;
        }
    }
    return out;
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator*(T x, const Matrix<ROW, COL, T> &a) {
    return (a * x);
}

template <size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator/(T x, const Matrix<ROW, COL, T> &a) {
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++) {
        for (size_t i = 0; i < COL; i++) {
            out.m[j][i] = x / a.m[j][i];
        }
    }
    return out;
}

template <size_t ROW, size_t COL, typename T>
Vector<COL, T> operator*(const Vector<ROW, T> &a,
                         const Matrix<ROW, COL, T> &m) {
    Vector<COL, T> b;
    for (size_t i = 0; i < COL; i++) b[i] = vector_dot(a, m.Col(i));
    return b;
}

template <size_t ROW, size_t COL, typename T>
Vector<ROW, T> operator*(const Matrix<ROW, COL, T> &m,
                         const Vector<COL, T> &a) {
    Vector<ROW, T> b;
    for (size_t i = 0; i < ROW; i++) b[i] = vector_dot(a, m.Row(i));
    return b;
}

//---------------------------------------------------------------------
// 数学库：行列式和逆矩阵等，光照计算有用
//---------------------------------------------------------------------

// 行列式求值：一阶
template <typename T>
T matrix_det(const Matrix<1, 1, T> &m) {
    return m[0][0];
}

// 行列式求值：二阶
template <typename T>
T matrix_det(const Matrix<2, 2, T> &m) {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

// 行列式求值：多阶行列式，即第一行同他们的余子式相乘求和
template <size_t N, typename T>
T matrix_det(const Matrix<N, N, T> &m) {
    T sum = 0;
    for (size_t i = 0; i < N; i++) sum += m[0][i] * matrix_cofactor(m, 0, i);
    return sum;
}

// 余子式：一阶
template <typename T>
T matrix_cofactor(const Matrix<1, 1, T> &m, size_t row, size_t col) {
    return 0;
}

// 多阶余子式：即删除特定行列的子式的行列式值
template <size_t N, typename T>
T matrix_cofactor(const Matrix<N, N, T> &m, size_t row, size_t col) {
    return matrix_det(m.GetMinor(row, col)) * (((row + col) % 2) ? -1 : 1);
}

// 伴随矩阵：即余子式矩阵的转置
template <size_t N, typename T>
Matrix<N, N, T> matrix_adjoint(const Matrix<N, N, T> &m) {
    Matrix<N, N, T> ret;
    for (size_t j = 0; j < N; j++) {
        for (size_t i = 0; i < N; i++) ret[j][i] = matrix_cofactor(m, i, j);
    }
    return ret;
}

// 求逆矩阵：使用伴随矩阵除以行列式的值得到
template <size_t N, typename T>
Matrix<N, N, T> matrix_invert(const Matrix<N, N, T> &m) {
    Matrix<N, N, T> ret = matrix_adjoint(m);
    T det = vector_dot(m.Row(0), ret.Col(0));
    return ret / det;
}

// 输出到文本流
template <size_t ROW, size_t COL, typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<ROW, COL, T> &m) {
    for (size_t r = 0; r < ROW; r++) {
        Vector<COL, T> row = m.Row(r);
        os << row << std::endl;
    }
    return os;
}

//---------------------------------------------------------------------
// 工具函数
//---------------------------------------------------------------------
template <typename T>
T Abs(T x) {
    return (x < 0) ? (-x) : x;
}
template <typename T>
T Max(T x, T y) {
    return (x < y) ? y : x;
}
template <typename T>
T Min(T x, T y) {
    return (x > y) ? y : x;
}

template <typename T>
bool NearEqual(T x, T y, T error) {
    return (Abs(x - y) < error);
}

template <typename T>
T Between(T xmin, T xmax, T x) {
    return Min(Max(xmin, x), xmax);
}

// 截取 [0, 1] 的范围
template <typename T>
T Saturate(T x) {
    return Between<T>(0, 1, x);
}

// 类型别名
typedef Vector<2, float> Vec2f;
typedef Vector<2, double> Vec2d;
typedef Vector<2, int> Vec2i;
typedef Vector<3, float> Vec3f;
typedef Vector<3, double> Vec3d;
typedef Vector<3, int> Vec3i;
typedef Vector<4, float> Vec4f;
typedef Vector<4, double> Vec4d;
typedef Vector<4, int> Vec4i;

typedef Matrix<4, 4, float> Mat4x4f;
typedef Matrix<3, 3, float> Mat3x3f;
typedef Matrix<4, 3, float> Mat4x3f;
typedef Matrix<3, 4, float> Mat3x4f;

//---------------------------------------------------------------------
// 3D 数学运算
//---------------------------------------------------------------------

// 矢量转整数颜色
static uint32_t vector_to_color(const Vec4f &color) {
    uint32_t r = (uint32_t)Between(0, 255, (int)(color.r * 255.0f));
    uint32_t g = (uint32_t)Between(0, 255, (int)(color.g * 255.0f));
    uint32_t b = (uint32_t)Between(0, 255, (int)(color.b * 255.0f));
    uint32_t a = (uint32_t)Between(0, 255, (int)(color.a * 255.0f));
    return (r << 16) | (g << 8) | b | (a << 24);
}

// 矢量转换整数颜色
static uint32_t vector_to_color(const Vec3f &color) {
    return vector_to_color(color.xyz1());
}

// 整数颜色到矢量
static Vec4f vector_from_color(uint32_t rgba) {
    Vec4f out;
    out.r = ((rgba >> 16) & 0xff) / 255.0f;
    out.g = ((rgba >> 8) & 0xff) / 255.0f;
    out.b = ((rgba >> 0) & 0xff) / 255.0f;
    out.a = ((rgba >> 24) & 0xff) / 255.0f;
    return out;
}

// matrix set to zero
static Mat4x4f matrix_set_zero() {
    Mat4x4f m;
    m.m[0][0] = m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
    m.m[1][0] = m.m[1][1] = m.m[1][2] = m.m[1][3] = 0.0f;
    m.m[2][0] = m.m[2][1] = m.m[2][2] = m.m[2][3] = 0.0f;
    m.m[3][0] = m.m[3][1] = m.m[3][2] = m.m[3][3] = 0.0f;
    return m;
}

// set to identity
static Mat4x4f matrix_set_identity() {
    Mat4x4f m;
    m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f;
    m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
    m.m[1][0] = m.m[1][2] = m.m[1][3] = 0.0f;
    m.m[2][0] = m.m[2][1] = m.m[2][3] = 0.0f;
    m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
    return m;
}

// 平移变换
static Mat4x4f matrix_set_translate(float x, float y, float z) {
    Mat4x4f m = matrix_set_identity();
    m.m[3][0] = x;
    m.m[3][1] = y;
    m.m[3][2] = z;
    return m;
}

// 缩放变换
static Mat4x4f matrix_set_scale(float x, float y, float z) {
    Mat4x4f m = matrix_set_identity();
    m.m[0][0] = x;
    m.m[1][1] = y;
    m.m[2][2] = z;
    return m;
}

// 旋转变换，围绕 (x, y, z) 矢量旋转 theta 角度
static Mat4x4f matrix_set_rotate(float x, float y, float z, float theta) {
    float qsin = (float)sin(theta * 0.5f);
    float qcos = (float)cos(theta * 0.5f);
    float w = qcos;
    Vec3f vec = vector_normalize(Vec3f(x, y, z));
    x = vec.x * qsin;
    y = vec.y * qsin;
    z = vec.z * qsin;
    Mat4x4f m;
    m.m[0][0] = 1 - 2 * y * y - 2 * z * z;
    m.m[1][0] = 2 * x * y - 2 * w * z;
    m.m[2][0] = 2 * x * z + 2 * w * y;
    m.m[0][1] = 2 * x * y + 2 * w * z;
    m.m[1][1] = 1 - 2 * x * x - 2 * z * z;
    m.m[2][1] = 2 * y * z - 2 * w * x;
    m.m[0][2] = 2 * x * z - 2 * w * y;
    m.m[1][2] = 2 * y * z + 2 * w * x;
    m.m[2][2] = 1 - 2 * x * x - 2 * y * y;
    m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
    m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
    m.m[3][3] = 1.0f;
    return m;
}

// 摄影机变换矩阵：eye/视点位置，at/看向哪里，up/指向上方的矢量
static Mat4x4f matrix_set_lookat(const Vec3f &eye, const Vec3f &at,
                                 const Vec3f &up) {
    Vec3f zaxis = vector_normalize(at - eye);
    Vec3f xaxis = vector_normalize(vector_cross(up, zaxis));
    Vec3f yaxis = vector_cross(zaxis, xaxis);
    Mat4x4f m;
    m.SetCol(0, Vec4f(xaxis.x, xaxis.y, xaxis.z, -vector_dot(eye, xaxis)));
    m.SetCol(1, Vec4f(yaxis.x, yaxis.y, yaxis.z, -vector_dot(eye, yaxis)));
    m.SetCol(2, Vec4f(zaxis.x, zaxis.y, zaxis.z, -vector_dot(eye, zaxis)));
    m.SetCol(3, Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    return m;
}

// D3DXMatrixPerspectiveFovLH
static Mat4x4f matrix_set_perspective(float fovy, float aspect, float zn,
                                      float zf) {
    float fax = 1.0f / (float)tan(fovy * 0.5f);
    Mat4x4f m = matrix_set_zero();
    m.m[0][0] = (float)(fax / aspect);
    m.m[1][1] = (float)(fax);
    m.m[2][2] = zf / (zf - zn);
    m.m[3][2] = -zn * zf / (zf - zn);
    m.m[2][3] = 1;
    return m;
}
