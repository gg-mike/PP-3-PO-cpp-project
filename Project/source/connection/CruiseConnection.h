#pragma once
#include "Connection.h"

// Interface between database and reservations for cruises only
class CruiseConnection :
	public Connection
{
public:
    // Initializers

    // Inits database
    CruiseConnection();
    
private:
    void InitDatabase() override;
};
