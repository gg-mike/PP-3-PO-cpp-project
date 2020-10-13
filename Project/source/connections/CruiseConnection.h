#pragma once
#include "Connection.h"

class CruiseConnection :
	public Connection
{
public:
    CruiseConnection(std::string start, std::string end);
    CruiseConnection();
private:
    void FindAllConnections() override;
    void FindStartEndConnections() override;
};

