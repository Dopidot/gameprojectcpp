#include "pch.h"
#include "Configuration.h"


Configuration::Configuration()
{
	jumpHeight = JUMP_HEIGHT;
	invulnerabilityTimeInSec = GOD_TIME_IN_SEC;
	playerGodTimeInSec = 0;
	totalHealthPoints = MAX_HEALTH_POINTS;
}


Configuration::~Configuration()
{
}

bool Configuration::checkOrCreateConfigurationFile()
{
	bool result = false;
	filename = "Donkey_kong.properties";

	fopen_s(&configFile, filename.c_str(), "r");

	if (!configFile)
	{
		std::cout << "Configuration file not found." << std::endl;

		fopen_s(&configFile, filename.c_str(), "w");

		if (!configFile)
		{
			std::perror("Error : unable to create configuration file.");
		}
		else
		{
			std::cout << "New configuration file created !" << std::endl;

			fprintf(configFile, "# Donkey kong properties\n");
			fprintf(configFile, "# Height of the jump. Min = 0, Max = 60\n");
			fprintf(configFile, "jump_height=50\n");
			fprintf(configFile, "\n");
			fprintf(configFile, "# Invulnerability time in sec when Mario collides with an enemy. Min = 0\n");
			fprintf(configFile, "invulnerability_time=3\n");
			fprintf(configFile, "\n");
			fprintf(configFile, "# Change to god mode. Min = 0, Max = 1\n");
			fprintf(configFile, "set_god_mode=0\n");
			fprintf(configFile, "\n");
			fprintf(configFile, "# Number of health points. Min = 1, Max = 10\n");
			fprintf(configFile, "health_points=5\n");

			fclose(configFile);
			
			result = true;
		}
	}
	else
	{
		result = true;
	}

	return result;
}


void Configuration::readConfigurationFile()
{
	bool fileAvailable = checkOrCreateConfigurationFile();

	if (!fileAvailable)
	{
		return;
	}

	fclose(configFile);

	fopen_s(&configFile, filename.c_str(), "r");

	if (configFile)
	{
		char szBuffer[2048];
		std::string line;

		while (fgets(szBuffer, 2048, configFile))
		{
			line = szBuffer;

			if (line.find("jump_height=") != std::string::npos)
			{
				std::size_t index = line.find("=");
				int length = line.length() - 1;

				if (index < length)
				{
					index++;

					std::string temp = line.substr(index, length);
					int newValue = std::stoi(temp);

					if (newValue >= 0 && newValue <= 60)
					{
						jumpHeight = newValue;
					}

				}
			}
			else if (line.find("invulnerability_time=") != std::string::npos)
			{
				std::size_t index = line.find("=");
				int length = line.length() - 1;

				if (index < length)
				{
					index++;

					std::string temp = line.substr(index, length);
					int newValue = std::stoi(temp);

					if (newValue >= 0)
					{
						invulnerabilityTimeInSec = newValue;
					}

				}
			}
			else if (line.find("set_god_mode=") != std::string::npos)
			{
				std::size_t index = line.find("=");
				int length = line.length() - 1;

				if (index < length)
				{
					index++;

					std::string temp = line.substr(index, length);
					int newValue = std::stoi(temp);

					if (newValue == 1)
					{
						playerGodTimeInSec = -1;
					}
				}
			}
			else if (line.find("health_points=") != std::string::npos)
			{
				std::size_t index = line.find("=");
				int length = line.length() - 1;

				if (index < length)
				{
					index++;

					std::string temp = line.substr(index, length);
					int newValue = std::stoi(temp);

					if (newValue >= 1 && newValue <= 10)
					{
						totalHealthPoints = newValue;
					}

				}
			}

		}


		fclose(configFile);
	}
}