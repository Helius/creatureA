#pragma once

#include <optional>

// increment clockwise, 0 means Up

class Direction {
public:
    Direction() = default;
    static constexpr unsigned int maxNumber = 8;
    int offsetX() const {
        switch (value % maxNumber) {
        case 1:
        case 2:
        case 3:
            return 1;
        case 5:
        case 6:
        case 7:
            return -1;
        default:
            return 0;
        }
    }

    int offsetY() const {
        switch (value % maxNumber) {
        case 7:
        case 0:
        case 1:
            return -1;
        case 3:
        case 4:
        case 5:
            return 1;
        default:
            return 0;
        }
    }

    void turnRight() {
        ++value;
    }

    void turnLeft() {
        --value;
    }

    void set(uint someValue) {
        value = someValue % maxNumber;
    }

//    void reverseX() {
//        value = ;
//    }
//    void reverseY() {
//        value = ;
//    }

    friend bool operator== (const Direction& d1, const Direction& d2);

private:
   unsigned char value = 0;
};

bool operator== (const Direction& d1, const Direction& d2);

class NearestSpace {
public:
    NearestSpace() = delete;
    NearestSpace(size_t index, size_t width, size_t height);
    std::optional<size_t> go(const Direction & dir) const;
    std::optional<size_t> goBounce(Direction  dir) const;

private:
    size_t m_index;
    size_t m_width;
    size_t m_height;
};