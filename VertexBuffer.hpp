#pragma once

#include <vector>
#include <glad/glad.h>

class VertexBuffer {
public:
    VertexBuffer()
    {
        glGenBuffers(1, &bufId);
    }

    void Activate() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufId);
    }

    void Write() const
    {
        Activate();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), (GLvoid*)(data.data()), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }

    void Append(const std::vector<float>& newData)
    {
        data.reserve(data.size() + newData.size());
        for (auto val : newData) {
            data.push_back(val);
        }
    }

    void Clear() {
        return data.clear();
    }

private:
    GLuint bufId = -1;
    std::vector<float> data;
};