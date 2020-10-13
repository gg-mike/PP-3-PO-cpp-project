#include "pch.h"
#include "Connection.h"

void Connection::UpdateInfo(std::string start, std::string end)
{
	this->start = start;
	this->end = end;
}

Log Connection::ChooseConnection(int ID)
{
	for (const auto& log : allConnections.info)
		if (ID == log.ID) {
			return log;
		}
	return Log();
}
