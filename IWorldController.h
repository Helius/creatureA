#pragma once

#include <memory>

class IWorldController {

public:
    enum ShowMode {
        ShowPredators = 0,
        ShowEnergy,
        ShowDivercity
    };

public:
    virtual ~IWorldController() = default;

    virtual void restart() = 0;
    virtual void setPause(bool ) = 0;
    virtual bool isPaused() const = 0;
    virtual void setSunLevel(int max) = 0;
    virtual void setMaxCreatureEnergy(int e) = 0;
    virtual void setMutationRate(uint e) = 0;
    virtual void setMoveEnergy(int e) = 0;
    virtual void setDivideEnergy(int e) = 0;
    virtual void setWorldSpeed(uint s) = 0;
    virtual void setShowMode(ShowMode) = 0;
    virtual ShowMode showMode() const = 0;
};

using IWorldControllerPtr = std::shared_ptr<IWorldController>;