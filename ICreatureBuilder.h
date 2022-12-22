#pragma once

#include <memory>

class ICreatureBuilder {
public:
    ~ICreatureBuilder() = default;

    virtual void buildDefaultCreatures(size_t amount) = 0;
};

using ICreatureBuilderPtr = std::shared_ptr<ICreatureBuilder>;