// Copyright 2017 The VGC Developers
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/vgc.io/vgc/blob/master/COPYRIGHT
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef VGC_WIDGETS_OPENGLVIEWER_H
#define VGC_WIDGETS_OPENGLVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <vector>

namespace vgc {
namespace widgets {

class OpenGLViewer : public QOpenGLWidget
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(OpenGLViewer)

public:
    /// This function must be called before creating the first
    /// OpenGLViewer. Its sets the appropriate Qt OpenGLFormat.
    ///
    static void init();

    OpenGLViewer(QWidget * parent = nullptr);
    ~OpenGLViewer();

private:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    using OpenGLFunctions = QOpenGLFunctions_3_2_Core;
    OpenGLFunctions * openGLFunctions() const;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void cleanupGL();

private:
    QPointF computeNormal_(const QPoint & p, const QPoint & q);
    void computeGLVertices_();

private:
    // Projection and view matrix
    QMatrix4x4 projMatrix_;
    QMatrix4x4 viewMatrix_;

    // Input mouse data
    using Curve = std::vector<QPoint>;
    std::vector<Curve> curves_;

    // RAM resources synced with GL resources
    struct GLVertex {
        float x, y;
        GLVertex(float x, float y) : x(x), y(y) {}
    };
    std::vector<GLVertex> glVertices_;

    // GL resources
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLBuffer vbo_;
    QOpenGLVertexArrayObject vao_;

    // Shader locations
    int vertexLoc_;
    int projMatrixLoc_;
    int viewMatrixLoc_;
};

} // namespace widgets
} // namespace vgc

#endif // VGC_WIDGETS_OPENGLVIEWER_H
