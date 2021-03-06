#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "Setting.h"
#include <iostream>
#include <string>
using namespace std;

class Display : public Setting {

public:
	Display();
	~Display() override;
	void inputInfo() override;
	void outputInfo() override;
	int getLightLevel() const;
	int getScreenLightLevel() const;
	int getTaploLightLevel() const;
	void setLightLevel(int lightLevel);
	void setScreenLightLevel(int lightLevel);
	void setTaploLightLevel(int lightLevel);
private:
	int _lightLevel;
	int _screenLightLevel;
	int _taploLightLevel;
};

#endif /* DISPLAY_H_ */

