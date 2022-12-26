#pragma once

#include <memory>

class IWorldController {
public:
    virtual ~IWorldController() = default;

    virtual void setPause(bool ) = 0;
    virtual bool isPaused() const = 0;
    virtual void setSunLevel(uint maxValue) = 0;
    virtual uint getSunLevel() = 0;
};

using IWorldControllerPtr = std::shared_ptr<IWorldController>;