#pragma once
#include "Connection.h"

// Interface between database and reservations for flights only
class FlightConnection :
    public Connection
{
public:
    // Initializers

    // Inits database
    FlightConnection();

private:
    void InitDatabase() override;
};
