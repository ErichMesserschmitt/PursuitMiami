#define OLC_PGE_APPLICATION
#include "pixelengine.h"
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>

using namespace std;

class PCar {
private:
	double car_rpm_idle;
	double car_rpm_curr;
	double car_rpm_max = 0.6000;
	double car_hp;
	double car_speed_current;
	int car_gears_max = 5;
	double car_gears_coef[6] = { 0, 1.0, 0.2, 0.05, 0.01, 0.008 };
	double car_speed_coef[6] = { 0, 60, 120, 190, 210, 280 };
	int car_gears_current = 0;
	bool car_gearbox_isauto;
	double key_rpm_counter = 0.005;
public:
	double getRpm() {
		return car_rpm_curr;
	}
	double getSpeed() {
		return car_speed_current;
	}
	int getGear() {
		return car_gears_current;
	}
	bool GearboxPCar(bool bUpPressed, bool bDowPressed) {
		if (bUpPressed) {
			if (car_gears_current < car_gears_max) {
				++car_gears_current;
				key_rpm_counter = KeyRpmGearReturn(car_rpm_curr, car_gears_coef[car_gears_current]);
				return true;
			}
		}
		if (bDowPressed) {
			if (car_gears_current > 0) {
				--car_gears_current;
				key_rpm_counter = KeyRpmGearReturn(car_rpm_curr, car_gears_coef[car_gears_current]);
				return true;
			}
		}
		return false;
		
	}
	void EnginePCar(bool bPressed, bool bHeld, bool bReleased) {
		if (bHeld) {
			if (car_rpm_curr <= car_rpm_max) {
				key_rpm_counter += 0.001;
			}
			else {
				key_rpm_counter -= 0.0007;
			}
		}
		else {
			if (key_rpm_counter > 0.0010)
				key_rpm_counter -= 0.0008;
			if (!car_gears_current) {
				if (car_speed_current >= 0.01)
				car_speed_current -= 0.01;
			}
		}
		if (key_rpm_counter)
			if (car_gears_current) {
				car_rpm_curr = (cbrt((car_gears_coef[car_gears_current] * key_rpm_counter) - 0.3) + 0.67);//(2*cbrt(key_rpm_counter - 0.5) +1.58) * 10000;//2 * cbrt(key_rpm_counter - 1.92) + 0.5; //
			}
			else {
				car_rpm_curr = 4 * key_rpm_counter;
			}
		if (car_rpm_curr <= 0) {
			car_rpm_curr = 0;
		}
		if(car_gears_current)
		car_speed_current = car_rpm_curr * car_speed_coef[car_gears_current];

	}
	double KeyRpmGearReturn(float car_rpm, float key_rpm_gear_koef) {
		double rpm;
		if (!car_gears_current) {
			rpm = car_rpm_curr / 4;
			return rpm;
		}
		rpm = car_speed_current / car_speed_coef[car_gears_current];
		double speed_predict = rpm * car_speed_coef[car_gears_current];
		
		double x = (rpm - 0.67 + cbrt(0.3)) / car_gears_coef[car_gears_current];
		return x;


		/*double rpm = car_rpm / car_gears_coef[car_gears_current];
		double x = (0.67 - rpm - cbrt(0.3)) / cbrt(key_rpm_gear_koef);
		x *= -1;
		return x;*/
	}
};
class MainWindow : public olc::PixelGameEngine {
private:
	PCar Car1;
	string str_rpm_current;
	string str_speed_current;
	olc::Sprite *car = new olc::Sprite("lines_png.png");
	int i = 0;
public:
	bool OnUserCreate() override {
		Clear(olc::BLACK);
		DrawSprite(0, 100, car, 1);
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {
		//if(!(Car1.GearboxPCar(GetKey(olc::Key::SPACE).bPressed, GetKey(olc::Key::CTRL).bPressed)))
		//Car1.EnginePCar(GetKey(olc::Key::W).bPressed, GetKey(olc::Key::W).bHeld, GetKey(olc::Key::W).bReleased);		
		//str_rpm_current = to_string(Car1.getRpm()*10000);
		//str_speed_current = to_string(Car1.getSpeed());
		/*Clear(olc::BLACK);
		DrawString(4, 4, str_rpm_current, olc::WHITE, 2);
		DrawString(4, 40, str_speed_current, olc::WHITE, 2);
		DrawString(4, 84, to_string(Car1.getGear()), olc::WHITE, 2);*/
		if (GetKey(olc::Key::RIGHT).bHeld) {
			i+=5;
			Clear(olc::BLACK);
			DrawSprite(i, 100, car, 1);
			if (i > 100)
				i = 0;
		}
		return true;
	}

};

int main() {
	MainWindow screen;
	if (screen.Construct(600, 400, 5, 5)) {
		screen.Start();
	}
	return 0;
}