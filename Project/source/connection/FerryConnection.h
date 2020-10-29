#pragma once
#include "Connection.h"

// Interface between database and reservations for cruises only
class FerryConnection :
	public Connection
{
public:
    // Initializers

    // Inits database
    FerryConnection();
    
private:
    void InitDatabase() override;
};
