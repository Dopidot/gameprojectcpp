#pragma once

#define JUMP_HEIGHT 50
#define GOD_TIME_IN_SEC 3
#define MAX_HEALTH_POINTS 10

class Configuration
{
public:
	Configuration();
	~Configuration();
	bool checkOrCreateConfigurationFile();
	void readConfigurationFile();

public:
	int jumpHeight;
	int invulnerabilityTimeInSec;
	int playerGodTimeInSec;
	int totalHealthPoints;

private:
	FILE * configFile;
	std::string filename;
};

