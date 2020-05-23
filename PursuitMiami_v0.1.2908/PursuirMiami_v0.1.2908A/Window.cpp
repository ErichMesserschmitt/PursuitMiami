#define OLC_PGE_APPLICATION
//#include "pixelengine.h"
#include "positions.h"
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include "carsexample.h"
#include "lab_7.h"
#include "canvas_builder.h"

using namespace std;




class texture {
	int posx;
	int posy;
	int scale;
};



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
	void switchGear() {
		if(car_gears_current < car_gears_max)
		car_gears_current += 1;
	}
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
				key_rpm_counter += fElapsedTime * 0.2;
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
		return current_x - (car_speed_current / coef);
	}
};



class DLC_creator {
	Game game;
	DLC_Mercedes_1980series dlc_mercedes1980;
	DLC_BMW_1980series dlc_bmw1990;
	DLC_Ferr_1980series dlc_ferr1980;
	Garage* cc;
	Garage* cr;
	Garage* fe;
	
public :
	DLC_creator() {
		cc = game.createCars(dlc_mercedes1980);
		cr = game.createCars(dlc_bmw1990);
		fe = game.createCars(dlc_ferr1980);
	}
	void info() {
		std::cout << "DLC BMW:" << std::endl;
		cr->info();
		std::cout << "DLC Mercedes:" << std::endl;
		cc->info();
		std::cout << "DLC Ferrari Miami Vice Edition" << std::endl;
		fe->info();
	}
	vector<Garage*> getPacks() {
		vector<Garage*> vec = { cc, cr, fe };
		return vec;
	}
};


//////////////////////////////////////////////////



class CarHandle {
 public:
  virtual ~CarHandle() {}
  virtual double getRpm() const = 0;
  virtual double switchGear() const = 0;
};


class ConcreteCar : public CarHandle {
	PCar* car;
 public:
	 ConcreteCar(PCar* car_) {
		 car = car_;
	 }
	 double getRpm() const override {
		 return car->getRpm();
	}
	 double switchGear() {
		 car->switchGear();
	 }
};


class Decorator : public CarHandle {

 protected:
  CarHandle* component_;

 public:
  Decorator(CarHandle* component) : component_(component) {
  }
 
  double getRpm() const override {
    return this->component_->getRpm();
  }
};

class Car_HandleBot : public Decorator {
 
 public:
	Car_HandleBot(CarHandle* component) : Decorator(component) {
	}
	double getRpm() const override {
		double rpm = Decorator::getRpm();
		if (rpm + ((rand() % 20) / 1000) >= current_car::Instance()->getCurrentCar()->getMaxRpm()) {
			std::cout << "CarHandle::Car_HandleBot bot switched gear" << std::endl;
			return Decorator::component_->switchGear();
		}
		else {
			std::cout << "CarHandle::Car_HandleBot bot did not switch gear" << std::endl;
			return Decorator::component_->getRpm();
		}
	}
};

class Car_HandlePlayer : public Decorator {
 public:
  Car_HandlePlayer(CarHandle* component) : Decorator(component) {
  }
  double switchGear() const override = 0;
  double getRpm() const override {
	  std::cout << "CarHandle::Car_HandlePlayer RPM value" << std::endl;
	  std::cout << Decorator::getRpm() << std::endl;
	  return Decorator::getRpm();
  }
};

void ClientCode(CarHandle* component) {
  // ...
  std::cout << "RESULT: " << component->getRpm();
  // ...
}

int main() {
  
 
  return 0;
}











////////////////////////////////////////////////////

class MainWindow : public olc::PixelGameEngine {
private:
	PCar Car1;
	string str_rpm_current;
	string str_speed_current;
	olc::Sprite* roadlines = new olc::Sprite("textures/dynamic_roadlines.png");
	olc::Sprite* road = new olc::Sprite("textures/static_road.png");

	olc::Sprite* car = new olc::Sprite("textures/car.png");
	olc::Sprite* city_front1 = new olc::Sprite("textures/dynamic_city_front_1.png");
	olc::Sprite* city_front2 = new olc::Sprite("textures/dynamic_city_front_2.png");
	olc::Sprite* city_front3 = new olc::Sprite("textures/dynamic_city_front_3.png");
	olc::Sprite* city_front4 = new olc::Sprite("textures/dynamic_city_front_4.png");
	olc::Sprite* city_front5 = new olc::Sprite("textures/dynamic_city_front_5.png");
	olc::Sprite* road_hq_bot = new olc::Sprite("textures/dynamic_road_bot_hq.png");
	olc::Sprite* road_hq_mid = new olc::Sprite("textures/dynamic_road_middle_hq.png");
	olc::Sprite* road_hq_top = new olc::Sprite("textures/dynamic_road_top_hq.png");
	olc::Sprite* city_back1 = new olc::Sprite("textures/lowdynamic_city_back_1.png");
	olc::Sprite* city_back2 = new olc::Sprite("textures/lowdynamic_city_back_2.png");
	olc::Sprite* city_back3 = new olc::Sprite("textures/lowdynamic_city_back_3.png");
	olc::Sprite* city_back4 = new olc::Sprite("textures/lowdynamic_city_back_4.png");
	olc::Sprite* sun = new olc::Sprite("textures/lowdynamic_sun.png");
	olc::Sprite* palm1 = new olc::Sprite("textures/palm_1.png");
	olc::Sprite* palm2 = new olc::Sprite("textures/palm_2.png");
	olc::Sprite* palm3 = new olc::Sprite("textures/palm_3.png");
	olc::Sprite* palm4 = new olc::Sprite("textures/palm_4.png");
	olc::Sprite* palm5 = new olc::Sprite("textures/palm_5.png");
	olc::Sprite* palm6 = new olc::Sprite("textures/palm_6.png");
	olc::Sprite* field = new olc::Sprite("textures/static_field.png");
	olc::Sprite* sky = new olc::Sprite("textures/static_sky.png");
	olc::Sprite* wheels1 = new olc::Sprite("textures/wheels1.png");
	olc::Sprite* wheels2 = new olc::Sprite("textures/wheels2.png");
private:
	double roadx1 = 0;
	double roadx2 = roadlines->width;
	double roadx3 = roadx2 + roadx2;
public:
	bool OnUserCreate() override {
		//Clear(olc::BLACK);
		
		Director dir;
		SpritesStaticBuilder builder;

		Spriter* sp = dir.createArmy(builder);
		sp->sprite();


		CarHandle* simple = new ConcreteCar(PCar());
		std::cout << "Client: I've got a simple component:\n";
		ClientCode(simple);
		std::cout << "\n\n";

		CarHandle* decorator1 = new Car_HandleBot(simple);
		CarHandle* decorator2 = new Car_HandlePlayer(decorator1);
		std::cout << "Client: Now I've got a decorated component:\n";
		ClientCode(decorator2);
		std::cout << "\n";

		delete simple;
		delete decorator1;
		delete decorator2;


		
		// ...

		SetPixelMode(olc::Pixel::MASK);
		DrawSprite(0, SKY, sky, 1);
		DrawSprite(0, ROAD, road, 1);
		DrawSprite(0, CITY_BACK, city_back1, 1);
		DrawSprite(0 + city_back1->width, CITY_BACK, city_back2, 1);
		DrawSprite(0 + city_back1->width + city_back2->width, CITY_BACK, city_back3, 1);
		DrawSprite(0 + city_back1->width + city_back2->width + city_back3->width, CITY_BACK, city_back4, 1);

		DrawSprite(0, CITY, city_front1, 1);
		DrawSprite(0 + city_front1->width, CITY, city_front2, 1);
		DrawSprite(0 + city_front1->width + city_front2->width, CITY, city_front3, 1);
		DrawSprite(0 + city_front1->width + city_front2->width + city_front3->width, CITY, city_front4, 1);

		DrawSprite(0, BUSH_FIELD, field, 1);
		DrawSprite(401, SUN, sun, 1);





		//fe->cc[0]->getStats()->printStats();
		DLC_creator* container = new DLC_creator();
		container->info();
		vector<Garage*> packs = container->getPacks();
		car_stats stats = *packs[2]->cc[0]->getStats();

		current_car::Instance()->select(stats);
		current_car::Instance()->getCurrentCar()->printStats();


		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {
		if (!(Car1.GearboxPCar(GetKey(olc::Key::SPACE).bPressed, GetKey(olc::Key::CTRL).bPressed)))
			Car1.EnginePCar(GetKey(olc::Key::W).bPressed, GetKey(olc::Key::W).bHeld, GetKey(olc::Key::W).bReleased, fElapsedTime);
		
		roadx1 = Car1.GetBackgroundSpeed(roadx1, 20);
		if (roadx1 > 0) {
			DrawSprite(roadx1, ROAD_LINES, roadlines, 1);
			//DrawBackgroud(road, roadlines, 0, ROAD_LINES);
			
		}
		else {
			if (roadx1 < -(roadlines->width)) {
				roadx1 = 1280 - roadlines->width;
			}
			DrawPartialSprite(0, ROAD_LINES, roadlines, -roadx1, 0, roadlines->width, roadlines->height, 1);
			DrawPartialSprite(1280+roadx1, ROAD_LINES, roadlines, 0, 0, -roadx1, roadlines->height, 1);
		}
		return true;
	}
	void DrawBackgroud(olc::Sprite* bckgr, olc::Sprite* texture, int tx, int ty) {
		DrawPartialSprite(tx, ty, bckgr, tx, ty, texture->width, texture->height, 1);
	}


};

int main() {
	MainWindow screen;
	screen.sAppName.assign("PursuitMiami");
	if (screen.Construct(1280, 720, 1, 1)) {
		screen.Start();
	}
	return 0;
}



