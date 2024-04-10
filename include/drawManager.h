#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H
#include <vector>

class DrawManager {
public:
    static DrawManager& getDrawManager();

    uint32_t initVAO() const;
    void drawLine(uint32_t& id);
    void drawTriangle(uint32_t& id);
    void drawQuad(uint32_t& id);
    void drawCube(uint32_t& id);
    void drawSphere(uint32_t& id);

private:
    DrawManager() = default;
    ~DrawManager();

    DrawManager(const DrawManager& other) = delete;
    DrawManager& operator=(const DrawManager& other) = delete;
    
    std::vector<uint32_t> vaoIds;
    std::vector<uint32_t> vboIds;
};


#endif