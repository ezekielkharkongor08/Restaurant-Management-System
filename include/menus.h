#ifndef MENUS_H
#define MENUS_H

#include <string>

class Restaurant;

// Small input helpers used by the menus.
int readInt(const std::string &prompt);
double readDouble(const std::string &prompt);
std::string readString(const std::string &prompt);

// Sub-menus for staff and menu CRUD.
void staffMenu(Restaurant &restaurant);
void menuMenu(Restaurant &restaurant);

#endif
