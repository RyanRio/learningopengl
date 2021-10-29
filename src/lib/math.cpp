#include "lib/math.h"

namespace math {
    namespace {
        unsigned int m_curHandle = 0;
        std::vector<Mat4Handle*> matrixHandles;
    }

    Mat4Handle::Mat4Handle() : Mat4Handle(mat4(1.0f)) {}

    Mat4Handle::Mat4Handle(mat4 init) {
        matrixHandles.push_back(this);
        m_handle = ++m_curHandle;
        matrix = init;
    }

    Mat4Handle* applyTransformation(mat4& in, const Transformation& transform) {
        Mat4Handle* tmp = allocHandle(in);
        applyTransformation(in, transform, *tmp);
        return tmp;
    }

    void applyTransformation(mat4& in, const Transformation& transform, Mat4Handle& saveTo) {
        // want to first do scaling operations, then rotations and lastly translations
        saveTo.matrix = scale(rotate(translate(in, transform.translate), transform.rotateMagnitude, transform.rotateVec), transform.scale);
    }

    Mat4Handle* allocHandle() {
        return allocHandle(mat4(1.0f));
    }

    Mat4Handle* allocHandle(mat4 init) {
        return new Mat4Handle(init);
    }
}