#include "WorldGeometry.h"

NearestSpace::NearestSpace(size_t index, size_t width, size_t height)
    : m_index(index)
    , m_width(width)
    , m_height(height)
{}

std::optional<size_t> NearestSpace::go(const Direction &dir) const {

    if (m_index >= m_width * m_height) {
        return std::nullopt;
    }

    int x = m_index % m_width + dir.offsetX();
    if (x < static_cast<int>(m_width) && x >= 0) {
        int y = m_index/m_width + dir.offsetY();
        if (y >= 0 && y < static_cast<int>(m_height)) {
            return y * m_width + x;
        }
    }
    return std::nullopt;
}

std::optional<size_t> NearestSpace::goBounce(Direction dir) const
{
    if (m_index >= m_width * m_height) {
        return std::nullopt;
    }

    int x = m_index % m_width + dir.offsetX();
    if (x < 0) {
        x +=  1;
    } else if (x >= static_cast<int>(m_width)) {
        x -= 1;
    }

    int y = m_index/m_width + dir.offsetY();
    if (y < 0) {
        y += 1;
    } else if (y >= static_cast<int>(m_height)) {
        y -= 1;
    }
    return y * m_width + x;
}

bool operator==(const Direction &d1, const Direction &d2)
{
    return d1.value % d1.maxNumber == d2.value % d2.maxNumber;
}
