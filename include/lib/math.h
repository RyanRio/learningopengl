#ifndef LIB_MATH_H
#define LIB_MATH_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

using namespace glm;

namespace math {

    struct Transformation {
        float rotateMagnitude{ 0 };
        vec3 rotateVec{ 0 };
        vec3 scale{ 1 };
        vec3 translate{ 0 };
    };

    class Mat4Handle {
    public:
        mat4 matrix;
        bool filled{ false };
        Mat4Handle();
        Mat4Handle(mat4 init);

    private:
        unsigned int m_handle;
    };

    // saves the result of the matrix to the new handle
    Mat4Handle* applyTransformation(mat4& in, const Transformation& transform);

    // updates the given handle with the resulting transformation
    void applyTransformation(mat4& in, const Transformation& transform, Mat4Handle& saveTo);

    // allocates a handle with a default initial matrix
    Mat4Handle* allocHandle();

    // allocates a handle with a given initial matrix
    Mat4Handle* allocHandle(mat4 init);
}
#endif // !LIB_MATH_H