//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_TRIANGLE_HH
#define FAKE3DRENDERER_TRIANGLE_HH

#include "Renderable.hh"
#include "Buffer.hh"

namespace Graphic {

enum RenderType : unsigned {
    Triangles = GL_TRIANGLES,
    UnknownRenderType = 0x0
};

enum CountType : unsigned {
    Byte = GL_UNSIGNED_BYTE,
    Short = GL_UNSIGNED_SHORT,
    Integer = GL_UNSIGNED_INT,
    UnknownCountType = 0x0
};

struct /*interface*/ RendererInfo {
    RenderType type = RenderType::Triangles;
    unsigned elements = 0u;
    CountType countType = CountType::Integer;
    glm::vec3 position{};
};

struct /*interface*/ Renderable {
    virtual void initialize(std::shared_ptr<Buffer> buffer) = 0;
    virtual RendererInfo beginDraw() = 0;
    virtual void endDraw() = 0;
    virtual ~Renderable() = default;
};

class Triangle : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        buffer_->initialize({{-1, 0, 0},
                             {0,  1, 0},
                             {1,  0, 0}}, {0, 1, 2});
    }

    RendererInfo beginDraw() override {
        RendererInfo ri;
        ri.elements = 1;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
};

class TriangleInv : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        buffer_->initialize({{-1, 0,  0},
                             {0,  -1, 0},
                             {1,  0,  0}}, {0, 1, 2});
    }

    RendererInfo beginDraw() override {
        RendererInfo ri;
        ri.elements = 1;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
};

class Cube : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        auto sideLength = 0.5f;

        auto front_t_1 = glm::vec3{-sideLength, sideLength, sideLength};
        auto front_t_2 = glm::vec3{-sideLength, -sideLength, sideLength};
        auto front_t_3 = glm::vec3{sideLength, sideLength, sideLength};
        auto front_t_4 = glm::vec3{sideLength, -sideLength, sideLength};

        auto rear_t_1 = glm::vec3{-sideLength, sideLength, -sideLength};
        auto rear_t_2 = glm::vec3{-sideLength, -sideLength, -sideLength};
        auto rear_t_3 = glm::vec3{sideLength, sideLength, -sideLength};
        auto rear_t_4 = glm::vec3{sideLength, -sideLength, -sideLength};

        auto left_t_1 = glm::vec3{-sideLength, -sideLength, sideLength};
        auto left_t_2 = glm::vec3{-sideLength, -sideLength, -sideLength};
        auto left_t_3 = glm::vec3{-sideLength, sideLength, sideLength};
        auto left_t_4 = glm::vec3{-sideLength, sideLength, -sideLength};

        auto right_t_1 = glm::vec3{sideLength, -sideLength, sideLength};
        auto right_t_2 = glm::vec3{sideLength, -sideLength, -sideLength};
        auto right_t_3 = glm::vec3{sideLength, sideLength, sideLength};
        auto right_t_4 = glm::vec3{sideLength, sideLength, -sideLength};

        auto top_t_1 = glm::vec3{-sideLength, sideLength, sideLength};
        auto top_t_2 = glm::vec3{sideLength, sideLength, sideLength};
        auto top_t_3 = glm::vec3{-sideLength, sideLength, -sideLength};
        auto top_t_4 = glm::vec3{sideLength, sideLength, -sideLength};

        auto bottom_t_1 = glm::vec3{-sideLength, -sideLength, sideLength};
        auto bottom_t_2 = glm::vec3{sideLength, -sideLength, sideLength};
        auto bottom_t_3 = glm::vec3{-sideLength, -sideLength, -sideLength};
        auto bottom_t_4 = glm::vec3{sideLength, -sideLength, -sideLength};


        buffer_->initialize({front_t_1, front_t_2, front_t_3, front_t_4,
                             rear_t_1, rear_t_2, rear_t_3, rear_t_4,
                             left_t_1, left_t_2, left_t_3, left_t_4,
                             right_t_1, right_t_2, right_t_3, right_t_4,
                             top_t_1, top_t_2, top_t_3, top_t_4,
                             bottom_t_1, bottom_t_2, bottom_t_3, bottom_t_4},
                            {0, 1, 2, 1, 2, 3,
                             4, 5, 6, 6, 7, 5,
                             8, 10, 9, 10, 11, 9,
                             12, 13, 14, 13, 14, 15,
                             16, 17, 18, 17, 18, 19,
                             20, 21, 22, 21, 22, 23});
    }

    RendererInfo beginDraw() override {
        RendererInfo ri{};
        ri.elements = 6 * 2 * 3;
        ri.position = {-1, 0, 0};
        buffer_->bind();
        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_{nullptr};
};


class Sphere : public Renderable {
    // Code is borrowed and adjust from the site:
    // http://www.songho.ca/opengl/gl_sphere.html
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {

        set(1.0f, 36, 18, true);
        buffer_ = std::move(buffer);

        std::vector<glm::vec3> points{};
        for (size_t i = 0; i < interleavedVertices.size(); i += 3) {
            points.push_back({interleavedVertices[i], interleavedVertices[i + 1], interleavedVertices[i + 2]});
        }
        indiciesCount = indices.size();
        buffer_->initialize(std::move(points), std::move(indices));
    }

    RendererInfo beginDraw() override {
        RendererInfo ri;
        ri.elements = indiciesCount;
        ri.position = {1, 0, 0};

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:

// constants //////////////////////////////////////////////////////////////////
    const int MIN_SECTOR_COUNT = 3;
    const int MIN_STACK_COUNT = 2;


///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
    Sphere(float radius, int sectors, int stacks, bool smooth) : interleavedStride(32) {
        set(radius, sectors, stacks, smooth);
    }


///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
    void set(float radius, int sectors, int stacks, bool smooth) {
        this->radius = radius;
        this->sectorCount = sectors;
        if (sectors < MIN_SECTOR_COUNT)
            this->sectorCount = MIN_SECTOR_COUNT;
        this->stackCount = stacks;
        if (sectors < MIN_STACK_COUNT)
            this->sectorCount = MIN_STACK_COUNT;
        this->smooth = smooth;

        if (smooth)
            buildVerticesSmooth();
        else
            buildVerticesFlat();
    }

    void setRadius(float radius) {
        this->radius = radius;
        updateRadius();
    }

    void setSectorCount(int sectors) {
        set(radius, sectors, stackCount, smooth);
    }

    void setStackCount(int stacks) {
        set(radius, sectorCount, stacks, smooth);
    }

    void setSmooth(bool smooth) {
        if (this->smooth == smooth)
            return;

        this->smooth = smooth;
        if (smooth)
            buildVerticesSmooth();
        else
            buildVerticesFlat();
    }


///////////////////////////////////////////////////////////////////////////////
// draw a sphere in VertexArray mode
// OpenGL RC must be set before calling it
///////////////////////////////////////////////////////////////////////////////
    void draw() const {
//        // interleaved array
//        glEnableClientState(GL_VERTEX_ARRAY);
//        glEnableClientState(GL_NORMAL_ARRAY);
//        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//        glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0]);
//        glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[3]);
//        glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[6]);
//
//        glDrawElements(GL_TRIANGLES, (unsigned int) indices.size(), GL_UNSIGNED_INT, indices.data());
//
//        glDisableClientState(GL_VERTEX_ARRAY);
//        glDisableClientState(GL_NORMAL_ARRAY);
//        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }


///////////////////////////////////////////////////////////////////////////////
// draw lines only
// the caller must set the line width before call this
///////////////////////////////////////////////////////////////////////////////
    void drawLines(const float lineColor[4]) const {
        // set line colour
        glColor4fv(lineColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, lineColor);

        // draw lines with VA
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices.data());

        glDrawElements(GL_LINES, (unsigned int) lineIndices.size(), GL_UNSIGNED_INT, lineIndices.data());

        glDisableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    }


///////////////////////////////////////////////////////////////////////////////
// draw a sphere surfaces and lines on top of it
// the caller must set the line width before call this
///////////////////////////////////////////////////////////////////////////////
    void drawWithLines(const float lineColor[4]) const {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0f); // move polygon backward
        this->draw();
        glDisable(GL_POLYGON_OFFSET_FILL);

        // draw lines with VA
        drawLines(lineColor);
    }


///////////////////////////////////////////////////////////////////////////////
// update vertex positions only
///////////////////////////////////////////////////////////////////////////////
    void updateRadius() {
        float scale = sqrtf(
                radius * radius / (vertices[0] * vertices[0] + vertices[1] * vertices[1] + vertices[2] * vertices[2]));

        std::size_t i, j;
        std::size_t count = vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 8) {
            vertices[i] *= scale;
            vertices[i + 1] *= scale;
            vertices[i + 2] *= scale;

            // for interleaved array
            interleavedVertices[j] *= scale;
            interleavedVertices[j + 1] *= scale;
            interleavedVertices[j + 2] *= scale;
        }
    }


///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
    void clearArrays() {
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(normals);
        std::vector<float>().swap(texCoords);
        std::vector<unsigned int>().swap(indices);
        std::vector<unsigned int>().swap(lineIndices);
    }


///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
    void buildVerticesSmooth() {
        const float PI = acos(-1);

        // clear memory of prev arrays
        clearArrays();

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
        float s, t;                                     // texCoord

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                addVertex(x, y, z);

                // normalized vertex normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                addNormal(nx, ny, nz);

                // vertex tex coord between [0, 1]
                s = (float) j / sectorCount;
                t = (float) i / stackCount;
                addTexCoord(s, t);
            }
        }

        // indices
        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1
        unsigned int k1, k2;
        for (int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                // 2 triangles per sector excluding 1st and last stacks
                if (i != 0) {
                    addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
                }

                if (i != (stackCount - 1)) {
                    addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
                }

                // vertical lines for all stacks
                lineIndices.push_back(k1);
                lineIndices.push_back(k2);
                if (i != 0)  // horizontal lines except 1st stack
                {
                    lineIndices.push_back(k1);
                    lineIndices.push_back(k1 + 1);
                }
            }
        }

        // generate interleaved vertex array as well
        buildInterleavedVertices();
    }


///////////////////////////////////////////////////////////////////////////////
// generate vertices with flat shading
// each triangle is independent (no shared vertices)
///////////////////////////////////////////////////////////////////////////////
    void buildVerticesFlat() {
        const float PI = acos(-1);

        // tmp vertex definition (x,y,z,s,t)
        struct Vertex {
            float x, y, z, s, t;
        };
        std::vector<Vertex> tmpVertices;

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            float xy = radius * cosf(stackAngle);       // r * cos(u)
            float z = radius * sinf(stackAngle);        // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                Vertex vertex;
                vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
                vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
                vertex.z = z;                           // z = r * sin(u)
                vertex.s = (float) j / sectorCount;        // s
                vertex.t = (float) i / stackCount;         // t
                tmpVertices.push_back(vertex);
            }
        }

        // clear memory of prev arrays
        clearArrays();

        Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
        std::vector<float> n;                           // 1 face normal

        int i, j, k, vi1, vi2;
        int index = 0;                                  // index for vertex
        for (i = 0; i < stackCount; ++i) {
            vi1 = i * (sectorCount + 1);                // index of tmpVertices
            vi2 = (i + 1) * (sectorCount + 1);

            for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2) {
                // get 4 vertices per sector
                //  v1--v3
                //  |    |
                //  v2--v4
                v1 = tmpVertices[vi1];
                v2 = tmpVertices[vi2];
                v3 = tmpVertices[vi1 + 1];
                v4 = tmpVertices[vi2 + 1];

                // if 1st stack and last stack, store only 1 triangle per sector
                // otherwise, store 2 triangles (quad) per sector
                if (i == 0) // a triangle for first stack ==========================
                {
                    // put a triangle
                    addVertex(v1.x, v1.y, v1.z);
                    addVertex(v2.x, v2.y, v2.z);
                    addVertex(v4.x, v4.y, v4.z);

                    // put tex coords of triangle
                    addTexCoord(v1.s, v1.t);
                    addTexCoord(v2.s, v2.t);
                    addTexCoord(v4.s, v4.t);

                    // put normal
                    n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                    for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                    {
                        addNormal(n[0], n[1], n[2]);
                    }

                    // put indices of 1 triangle
                    addIndices(index, index + 1, index + 2);

                    // indices for line (first stack requires only vertical line)
                    lineIndices.push_back(index);
                    lineIndices.push_back(index + 1);

                    index += 3;     // for next
                } else if (i == (stackCount - 1)) // a triangle for last stack =========
                {
                    // put a triangle
                    addVertex(v1.x, v1.y, v1.z);
                    addVertex(v2.x, v2.y, v2.z);
                    addVertex(v3.x, v3.y, v3.z);

                    // put tex coords of triangle
                    addTexCoord(v1.s, v1.t);
                    addTexCoord(v2.s, v2.t);
                    addTexCoord(v3.s, v3.t);

                    // put normal
                    n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                    for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                    {
                        addNormal(n[0], n[1], n[2]);
                    }

                    // put indices of 1 triangle
                    addIndices(index, index + 1, index + 2);

                    // indices for lines (last stack requires both vert/hori lines)
                    lineIndices.push_back(index);
                    lineIndices.push_back(index + 1);
                    lineIndices.push_back(index);
                    lineIndices.push_back(index + 2);

                    index += 3;     // for next
                } else // 2 triangles for others ====================================
                {
                    // put quad vertices: v1-v2-v3-v4
                    addVertex(v1.x, v1.y, v1.z);
                    addVertex(v2.x, v2.y, v2.z);
                    addVertex(v3.x, v3.y, v3.z);
                    addVertex(v4.x, v4.y, v4.z);

                    // put tex coords of quad
                    addTexCoord(v1.s, v1.t);
                    addTexCoord(v2.s, v2.t);
                    addTexCoord(v3.s, v3.t);
                    addTexCoord(v4.s, v4.t);

                    // put normal
                    n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                    for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                    {
                        addNormal(n[0], n[1], n[2]);
                    }

                    // put indices of quad (2 triangles)
                    addIndices(index, index + 1, index + 2);
                    addIndices(index + 2, index + 1, index + 3);

                    // indices for lines
                    lineIndices.push_back(index);
                    lineIndices.push_back(index + 1);
                    lineIndices.push_back(index);
                    lineIndices.push_back(index + 2);

                    index += 4;     // for next
                }
            }
        }

        // generate interleaved vertex array as well
        buildInterleavedVertices();
    }


///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
    void buildInterleavedVertices() {
        std::vector<float>().swap(interleavedVertices);

        std::size_t i, j;
        std::size_t count = vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2) {
            interleavedVertices.push_back(vertices[i]);
            interleavedVertices.push_back(vertices[i + 1]);
            interleavedVertices.push_back(vertices[i + 2]);

            interleavedVertices.push_back(normals[i]);
            interleavedVertices.push_back(normals[i + 1]);
            interleavedVertices.push_back(normals[i + 2]);

            interleavedVertices.push_back(texCoords[j]);
            interleavedVertices.push_back(texCoords[j + 1]);
        }
    }


///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
    void addVertex(float x, float y, float z) {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }


///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
    void addNormal(float nx, float ny, float nz) {
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
    }


///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
    void addTexCoord(float s, float t) {
        texCoords.push_back(s);
        texCoords.push_back(t);
    }


///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }


///////////////////////////////////////////////////////////////////////////////
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,  // v1
                                         float x2, float y2, float z2,  // v2
                                         float x3, float y3, float z3)  // v3
    {
        const float EPSILON = 0.000001f;

        std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
        float nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        float ex1 = x2 - x1;
        float ey1 = y2 - y1;
        float ez1 = z2 - z1;
        float ex2 = x3 - x1;
        float ey2 = y3 - y1;
        float ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        float length = sqrtf(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON) {
            // normalize
            float lengthInv = 1.0f / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
    std::vector<float> vertices{};
    std::vector<float> normals{};
    std::vector<float> texCoords{};
    std::vector<unsigned int> indices{};
    unsigned indiciesCount = 0;
    std::vector<unsigned int> lineIndices{};

    // interleaved
    std::vector<float> interleavedVertices{};
    int interleavedStride{};                  // # of bytes to hop to the next vertex (should be 32 bytes)


    std::shared_ptr<Buffer> buffer_;
};


} // namespace Graphic
#endif //FAKE3DRENDERER_TRIANGLE_HH
