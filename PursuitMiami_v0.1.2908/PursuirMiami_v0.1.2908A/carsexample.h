#include <iostream>
#include <vector>





class car_stats {
	double car_rpm_max;// = 0.6000;
	std::vector<double> car_gears_coef;// [6] = { 0, 1.0, 0.2, 0.05, 0.02, 0.005 };
	std::vector<double> car_speed_coef;// [6] = { 0, 60, 120, 190, 210, 280 };
	bool car_gearbox_isauto;
	double key_rpm_counter;// = 0.005;
public:
	car_stats(double car_rpm_max, std::vector<double> car_gears_coef, std::vector<double> car_speed_coef, bool car_gearbox_isauto, double key_rpm_counter) {
		this->car_rpm_max = car_rpm_max;
		this->car_gears_coef = car_gears_coef;
		this->car_speed_coef = car_speed_coef;
		this->car_gearbox_isauto = car_gearbox_isauto;
		this->key_rpm_counter = key_rpm_counter;
	}
	void printStats();
	double getMaxRpm() {
		return car_rpm_max;
	}
};




class current_car {
private:
	static current_car* curr_car;
	current_car() {
		stats = nullptr;
	}
	car_stats* stats;
public:
	static current_car* Instance();
	car_stats* getCurrentCar();
	void select(const car_stats& stats);
};





class Car_racer {
public:
	virtual void printInfo() = 0;
	virtual car_stats* getStats() = 0;

};

class Car_chaser {
public:
	virtual void printInfo() = 0;
	virtual car_stats* getStats() = 0;
};


class Merlo190 : public Car_racer {
public:
	
	car_stats* getStats() override { return nullptr; };
	void printInfo() override;
};

class Bmw220e30 : public Car_racer {
public:
	car_stats* getStats() override { return nullptr; };
	void printInfo() override;
};

class Merlo123 : public Car_chaser {
public:
	car_stats* getStats() override { return nullptr; };
	void printInfo() override;
};

class Bmw530e34 : public Car_chaser {
public:
	car_stats* getStats() override { return nullptr; };
	void printInfo() override;
};

class FerrDaytona : public Car_racer {
private:
	car_stats* car_stat;
public:
	FerrDaytona() {
		if(this->car_stat == nullptr)
		car_stat = new car_stats(0.6000, { 0, 1.0, 0.2, 0.05, 0.02, 0.005 }, { 0,60,120,190,210,280 }, false, 0.005);
	}
public:
	car_stats* getStats() override;
	void printInfo() override;
};
class FerrTestarossa : public Car_chaser {
private:
	car_stats* car_stat;
public:
	FerrTestarossa() {
		if (this->car_stat == nullptr)
			car_stat = new car_stats(0.8000, { 0, 1.0, 0.3, 0.09, 0.05, 0.006 }, { 0,65,130,200,240,310 }, false, 0.005);
	}
public:
	car_stats* getStats() override;
	void printInfo() override;
};


class GarageFactory {
public: 
	virtual Car_racer* createRacerCar() = 0;
	virtual Car_chaser* createChaserCar() = 0;
};

class DLC_Mercedes_1980series : public GarageFactory {
public:
	Car_racer* createRacerCar();
	Car_chaser* createChaserCar();
};

class DLC_BMW_1980series : public GarageFactory {
public:
	Car_racer* createRacerCar();
	Car_chaser* createChaserCar();
};
class DLC_Ferr_1980series : public GarageFactory {
public:
	Car_racer* createRacerCar();
	Car_chaser* createChaserCar();
};


class Garage
{
public:
	
	void info() {
		int i;
		std::cout << "Racing: \n";
		for (i = 0; i < cr.size(); ++i)  cr[i]->printInfo();
		std::cout << "Chasing: \n";
		for (i = 0; i < cc.size(); ++i)  cc[i]->printInfo();
	}
	std::vector<Car_racer*> cr;
	std::vector<Car_chaser*> cc;
};

class Game
{
public:
	Garage* createCars(GarageFactory& factory) {
		Garage* p = new Garage;
		p->cr.push_back(factory.createRacerCar());
		p->cc.push_back(factory.createChaserCar());
		return p;
	}
};


