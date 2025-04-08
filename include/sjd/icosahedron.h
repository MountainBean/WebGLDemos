#ifndef SPHERE_H
#define SPHERE_H

/* Sphere class 
 * taking instruction from icosphere:
 * https://www.songho.ca/opengl/gl_sphere.htm
 */
#include "glm/ext/matrix_common.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace constants {
    const float pi {3.141592654f};
    const float hAngle {(pi / 180) * 72};
    const float vAngle {atanf(1.0f / 2.0f)};
}
namespace sjd {
class Icosahedron {
public:
    Icosahedron(float radius = 0.5f) 
    : m_radius {radius}
    {
        float hAngle1 {-constants::pi / 2 - constants::hAngle / 2};
        float hAngle2 {-constants::pi / 2};
        int i1;
        int i2;
        float y {m_radius * sinf(constants::vAngle)};
        float xz {m_radius * cosf(constants::vAngle)};

        m_vertices[0] = 0;
        m_vertices[1] = m_radius;
        m_vertices[2] = 0;

        for(int i = 1; i <= 5; ++i) {
            i1 = i * 3;
            i2 = (i + 5) * 3;

            m_vertices[i1] = xz * cosf(hAngle1);      // x
            m_vertices[i2] = xz * cosf(hAngle2);
            m_vertices[i1 + 1] = y;                   // y
            m_vertices[i2 + 1] = -y;
            m_vertices[i1 + 2] = xz * sinf(hAngle1);  // z
            m_vertices[i2 + 2] = xz * sinf(hAngle2);

            // next horizontal angles
            hAngle1 += constants::hAngle;
            hAngle2 += constants::hAngle;
        }
        i1 = 11 * 3;
        m_vertices[i1] = 0.0f;
        m_vertices[i1 + 1] = -m_radius;
        m_vertices[i1 + 2] = 0.0f;
        
        m_indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 1,
            1, 2, 6,
            2, 3, 7,
            3, 4, 8,
            4, 5, 9,
            5, 1, 10,
            6, 7, 2,
            7, 8, 3,
            8, 9, 4,
            9, 10, 5,
            10, 6, 1,
            6, 7, 11,
            7, 8, 11,
            8, 9, 11,
            9, 10, 11,
            10, 6, 11
        };
        m_line_indices = {
            0, 1, 0, 2, 0, 3, 0, 4, 0, 5,
            1, 2, 2, 3, 3, 4, 4, 5, 5, 1,
            1, 10, 1, 6, 2, 6, 2, 7, 3, 7,
            3, 8, 4, 8, 4, 9, 5, 9, 5, 10,
            6, 7, 7, 8, 8, 9, 9, 10, 10, 6,
            6, 11, 7, 11, 8, 11, 9, 11, 10, 11,
        };
    }

    const std::array<float, 36>& getVertices() const {
        const std::array<float, 36>& vertref {m_vertices};
        return vertref;
    }

    const std::array<uint16_t, 60>& getIndices() const {
        const std::array<uint16_t, 60>& indref {m_indices};
        return indref;
    }

    const std::array<uint16_t, 60>& getLineIndices() const {
        const std::array<uint16_t, 60>& indref {m_line_indices};
        return indref;
    }

    const std::array<float, 180> getPrimVertices() const {
        std::array<float, 180> primVertices {};
        int i {0};
        for (uint32_t index : m_indices) {
            primVertices[i] = (m_vertices[3*index]);
            primVertices[i + 1] = (m_vertices[3*index+1]);
            primVertices[i + 2] = (m_vertices[3*index+2]);
            i += 3;
        }
        return primVertices;
    }

    const std::array<float, 360> getPrimVerticesNorms() const {
        std::array<float, 360> primVerticesNorms {};
        for (int i = 0; i < 20; i++) {
            // i is the row in the indices list
            primVerticesNorms[18*i +  0] = m_vertices[3 * m_indices[3*i+0]+0];  // 0x
            primVerticesNorms[18*i +  1] = m_vertices[3 * m_indices[3*i+0]+1];  // 0y
            primVerticesNorms[18*i +  2] = m_vertices[3 * m_indices[3*i+0]+2];  // 0z
            primVerticesNorms[18*i +  6] = m_vertices[3 * m_indices[3*i+1]+0];  // 1x
            primVerticesNorms[18*i +  7] = m_vertices[3 * m_indices[3*i+1]+1];  // 1y
            primVerticesNorms[18*i +  8] = m_vertices[3 * m_indices[3*i+1]+2];  // 1z
            primVerticesNorms[18*i + 12] = m_vertices[3 * m_indices[3*i+2]+0];  // 2x
            primVerticesNorms[18*i + 13] = m_vertices[3 * m_indices[3*i+2]+1];  // 2y
            primVerticesNorms[18*i + 14] = m_vertices[3 * m_indices[3*i+2]+2];  // 2z

            // We're drawing a crude sphere so all prims should be facing the 
            // origin this means taking the average of each vertex of a prim
            // should produce a vector for the centre of the prim which will
            // also be the prim's Normal.
            float aveX = (primVerticesNorms[18*i +  0] +
                          primVerticesNorms[18*i +  6] +
                          primVerticesNorms[18*i + 12]) / 3;
            float aveY = (primVerticesNorms[18*i +  1] +
                          primVerticesNorms[18*i +  7] +
                          primVerticesNorms[18*i + 13]) / 3;
            float aveZ = (primVerticesNorms[18*i +  2] +
                          primVerticesNorms[18*i +  8] +
                          primVerticesNorms[18*i + 14]) / 3;
            glm::vec3 normVec = glm::normalize(glm::vec3(aveX, aveY, aveZ));

            primVerticesNorms[18*i +  3] = normVec.x;
            primVerticesNorms[18*i +  4] = normVec.y;
            primVerticesNorms[18*i +  5] = normVec.z;
            primVerticesNorms[18*i +  9] = normVec.x;
            primVerticesNorms[18*i + 10] = normVec.y;
            primVerticesNorms[18*i + 11] = normVec.z;
            primVerticesNorms[18*i + 15] = normVec.x;
            primVerticesNorms[18*i + 16] = normVec.y;
            primVerticesNorms[18*i + 17] = normVec.z;
        }
        return primVerticesNorms;
        
    }

private:
    float m_radius;
    std::array<float, 36> m_vertices;
    std::array<uint16_t, 60> m_indices;
    std::array<uint16_t, 60> m_line_indices;

};
}
#endif
