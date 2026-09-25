#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "sd_task.h"

Model::Model() : modelListener(0), lastSdStatus(0xFF)
{

}

void Model::tick()
{
    uint8_t status = sd_write_status;
    if (status != lastSdStatus)
    {
        lastSdStatus = status;
        modelListener->sdStatusChanged(status);
    }
}
