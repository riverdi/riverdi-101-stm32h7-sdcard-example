#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <cstdint>

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}

    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void sdStatusChanged(uint8_t status) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
