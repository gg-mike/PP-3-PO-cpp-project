#pragma once
#include "Connection.h"

class FlightConnection :
    public Connection
{
public:
    FlightConnection(std::string start, std::string end);
    FlightConnection();
private:
    void FindAllConnections() override;
    void FindStartEndConnections() override;
};

