#include <iostream>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "drawManager.h"

DrawManager& DrawManager::getDrawManager() {
    static DrawManager drawManager;
    return drawManager;
}

DrawManager::~DrawManager() {
    for (auto& id : vaoIds) {
        glDeleteVertexArrays(1, &id);
    }
    for (auto& id : vboIds) {
        glDeleteBuffers(1, &id);
    }
}

uint32_t DrawManager::initVAO() const {
    return 0;
}

void DrawManager::drawTriangle(uint32_t& id) {
    if (id == 0) {
        float trianglev[] = {
        // positions         // colors
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
        };

        glGenVertexArrays(1, &id);

        uint32_t vbo;
        glGenBuffers(1, &vbo);

        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trianglev), trianglev, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        vaoIds.push_back(id);
        vboIds.push_back(vbo);
    }

    glBindVertexArray(id);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void DrawManager::drawQuad(uint32_t& id) {
    if (id == 0) {
        float quadV[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        glGenVertexArrays(1, &id);

        uint32_t vbo;
        glGenBuffers(1, &vbo);

        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadV), quadV, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        vaoIds.push_back(id);
        vboIds.push_back(vbo);
    }

    glBindVertexArray(id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawManager::drawCube(uint32_t& id) {
    if (id == 0) {
        float cubev[] = {
            -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    
            -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
            -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    
            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
             1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    
            -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f, 1.0f
        };
        
        glGenVertexArrays(1, &id);

        uint32_t vbo;
        glGenBuffers(1, &vbo);

        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubev), cubev, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        vaoIds.push_back(id);
        vboIds.push_back(vbo);
    }

    glBindVertexArray(id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void DrawManager::drawSphere(uint32_t& id) {
    static uint32_t indexCount = 0;
    if (id == 0) {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;
        std::vector<uint32_t> indices;

        uint32_t xSegments = 64, ySegments = 64;
        float pi = 3.14159265359f;
        for (uint32_t x = 0; x <= xSegments; ++x) {
            for (uint32_t y = 0; y <= ySegments; ++y) {
                float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
                float ySegment = static_cast<float>(y) / static_cast<float>(ySegments);
                float xPos = std::cos(xSegment * 2.0 * pi) * std::sin(ySegment * pi);
                float yPos = std::cos(ySegment * pi);
                float zPos = std::sin(xSegment * 2.0 * pi) * std::sin(ySegment * pi);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
            }
        }
        bool oddRow = false;
        for (uint32_t y = 0; y < ySegments; ++y) {
            if (!oddRow) {
                for (uint32_t x = 0; x <= xSegments; ++x) {
                    indices.push_back(y * (xSegments + 1) + x);
                    indices.push_back((y + 1) * (xSegments + 1) + x);
                }
            }
            else {
                for (int x = xSegments; x >= 0; --x) {
                    indices.push_back((y + 1) * (xSegments + 1) + x);
                    indices.push_back(y * (xSegments + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();
        std::vector<float> data;
        uint32_t size = positions.size();
        for (uint32_t i = 0; i < size; ++i) {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (!normals.empty()) {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (!uv.empty()) {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }

        glGenVertexArrays(1, &id);

        uint32_t vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
        uint32_t stride = (3 + 3 + 2) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));

        vaoIds.push_back(id);
        vboIds.push_back(vbo);
    }

    glBindVertexArray(id);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
