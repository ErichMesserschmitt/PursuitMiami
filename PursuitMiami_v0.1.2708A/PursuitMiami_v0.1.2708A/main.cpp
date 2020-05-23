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
	double car_gears_coef[6] = { 0, 1.0, 0.2, 0.05, 0.02, 0.005 };
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
	void EnginePCar(bool bPressed, bool bHeld, bool bReleased, float fElapsedTime) {
		if (bHeld) {
			if (car_rpm_curr <= car_rpm_max) {
				key_rpm_counter += fElapsedTime*0.2;
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
				car_rpm_curr = car_gears_coef[car_gears_current] * pow(key_rpm_counter, 1.9);
			}
			else {
				car_rpm_curr = 4 * key_rpm_counter;
			}
		if (car_rpm_curr <= 0) {
			car_rpm_curr = 0;
		}
		if (car_gears_current)
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

		double key_predict = pow((rpm / car_gears_coef[car_gears_current]), (1 / 1.9));
		return key_predict;
	}

	double GetBackgroundSpeed(double current_x, double coef) {
		return current_x - (car_speed_current/coef);
	}
};
class MainWindow : public olc::PixelGameEngine {
private:
	PCar Car1;
	string str_rpm_current;
	string str_speed_current;
	double road_line1_x = 495;
	double road_line2_x = 1280;
	double backgr_x = 495;
	int prev_linepos1;
	int prev_linepos2;
	int positionx = 40;
	olc::Sprite* road_backgr1 = new olc::Sprite("test_img/road_bckgr_asph_01.png");
	olc::Sprite* road_lines1 = new olc::Sprite("test_img/road_line_white_01.png");
	olc::Sprite* road_linefield1 = new olc::Sprite("test_img/road_linefield_asph_02.png");
	olc::Sprite* car = new olc::Sprite("test_img/car_crockett.png");
	olc::Sprite* lines_backgr = new olc::Sprite("test_img/lines.png");
	olc::Sprite* black = new olc::Sprite("test_img/Black.png");
	olc::Sprite* road_path = new olc::Sprite("test_img/road_path.png");
	olc::Sprite* road_path_cl = new olc::Sprite("test_img/road_path_cl.png");

	int i = 0;
public:
	bool OnUserCreate() override {
		Clear(olc::BLACK);
		DrawSprite(0, 520, road_backgr1, 1);
		SetPixelMode(olc::Pixel::MASK);
		DrawSprite(40, 450, car, 1);
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {
		if(!(Car1.GearboxPCar(GetKey(olc::Key::SPACE).bPressed, GetKey(olc::Key::CTRL).bPressed)))
		Car1.EnginePCar(GetKey(olc::Key::W).bPressed, GetKey(olc::Key::W).bHeld, GetKey(olc::Key::W).bReleased, fElapsedTime);		
		str_rpm_current = to_string(Car1.getRpm()*10000);
		str_speed_current = to_string(Car1.getSpeed());
		DrawSprite(0, 0, black, 1);
		if (Car1.getRpm() <= 0.6)
			DrawString(4, 4, "RPM " + str_rpm_current, olc::WHITE, 2);
		if (Car1.getRpm() > 0.6)
			DrawString(4, 4, "RPM " + str_rpm_current, olc::RED, 2);
		DrawString(4, 40, "SPEED " + str_speed_current, olc::WHITE, 2);
		DrawString(4, 84, "GEAR " + to_string(Car1.getGear()), olc::WHITE, 2);
		DrawSprite(prev_linepos1, 600, road_linefield1, 1);	
		DrawSprite(prev_linepos2, 600, road_linefield1, 1);
		DrawSprite(prev_linepos1, 700, road_path_cl, 1);
		DrawSprite(prev_linepos1 = road_line1_x = Car1.GetBackgroundSpeed(road_line1_x, 10), 600, road_lines1, 1);
		DrawSprite(prev_linepos2 = road_line2_x = Car1.GetBackgroundSpeed(road_line2_x, 10), 600, road_lines1, 1);
		DrawSprite(prev_linepos1, 700, road_path, 1);
		if (GetKey(olc::Key::RIGHT).bHeld) {
			++positionx;
			DrawSprite(positionx, 450, car, 1);
		}
		if (GetKey(olc::Key::LEFT).bHeld) {
			--positionx;
			DrawSprite(positionx, 450, car, 1);
		}
		if (road_line1_x < -313)
			road_line1_x = 1260;
		if (road_line2_x < -313)
			road_line2_x = 1260;
		if (backgr_x < 0)
			backgr_x = 600;
		return true;
	}

};

int main() {
	MainWindow screen;
	if (screen.Construct(1280, 720, 1, 1)) {
		screen.Start();
	}
	return 0;
}