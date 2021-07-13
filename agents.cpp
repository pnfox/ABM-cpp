#include "agents.h"

Firms::Firms()
{

}

bool Firms::isDefaulted(int firm)
{
	return defaulted[firm] == 1 ? true : false;
}

Banks::Banks()
{

}

bool Banks::isDefaulted(int bank)
{
	return defaulted[bank] == 1 ? true : false;
}
