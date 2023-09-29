#include <vector>
#include <string>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string> SplitString(std::string InString, char delimiter);

void Write2Log(std::string Message);
