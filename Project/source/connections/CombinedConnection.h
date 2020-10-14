#pragma once
#include "Connection.h"

class CombinedConnection :
    public Connection
{
public:
    CombinedConnection();
    void UpdateInfo(std::string start, std::string end) override;
private:
    void FindAllConnections() override;
    void FindStartEndConnections() override;
};

