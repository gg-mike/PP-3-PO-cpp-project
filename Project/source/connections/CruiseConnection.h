#pragma once
#include "Connection.h"

class CruiseConnection :
	public Connection
{
public:
    CruiseConnection();
    void UpdateInfo(std::string start, std::string end) override;
private:
    void FindAllConnections() override;
    void FindStartEndConnections() override;
};

