#pragma once
#include "Connection.h"

// Interface between database and reservations for flights only
class TrainConnection :
    public Connection
{
public:
    // Initializers

    // Inits database
    TrainConnection();

private:
    void InitDatabase() override;
};
