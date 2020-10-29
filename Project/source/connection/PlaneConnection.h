#pragma once
#include "Connection.h"

// Interface between database and reservations for flights only
class PlaneConnection :
    public Connection
{
public:
    // Initializers

    // Inits database
    PlaneConnection();

private:
    void InitDatabase() override;
};
