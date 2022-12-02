#pragma once

#include "VecN.hpp"
#include <vector>

class Triangle {
public:
    Triangle() = default;
    Triangle(const std::vector<Vertex>& points)
    {
        AddVertex(points);
    }

    void AddVertex(const Vertex& point)
    {
        m_vertexes.push_back(point.x);
        m_vertexes.push_back(point.y);
        m_vertexes.push_back(point.z);
    }

    void AddVertex(const std::vector<Vertex>& points)
    {
        if (points.empty()) {
            return;
        }
        m_vertexes.reserve(points.size() * points.front().Dim());
        for (const Vertex &vertex : points) {
            m_vertexes.push_back(vertex.x);
            m_vertexes.push_back(vertex.y);
            m_vertexes.push_back(vertex.z);
        }
    }

    const std::vector<float> GetData() const {
        return m_vertexes;
    }

private:
    std::vector<float> m_vertexes;
};