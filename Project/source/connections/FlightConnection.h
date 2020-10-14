#pragma once
#include "Connection.h"

class FlightConnection :
    public Connection
{
public:
    FlightConnection();
    void UpdateInfo(std::string start, std::string end) override;
private:
    void FindAllConnections() override;
    void FindStartEndConnections() override;
};

