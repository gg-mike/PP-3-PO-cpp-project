#pragma once
#include "Connection.h"

class CombinedConnection :
    public Connection
{
public:
    CombinedConnection(std::string start, std::string end);
    CombinedConnection();
private:
    void FindAllConnections() override;
    void FindStartEndConnections() override;
};

