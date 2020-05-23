#include <iostream>
#include <vector>
#include "carsexample.h"

//class Car_stats {
//private:
//	static Car_stats* Obj;
//	Info_car* stats;
//	Car_stats(const Info_car& info) {
//		stats = new Info_car(info);
//	}
//	Car_stats() {
//	}
//protected:
//	static void setCarStats(const Info_car& info) {
//		if (Obj == nullptr) {
//			Obj = new Car_stats(info);
//		}
//		else {
//			Obj->stats = new Info_car(info);
//		}
//	}
//public:
//	static Car_stats* Instance() {
//		if (Obj == nullptr) {
//			Obj = new Car_stats();
//		}
//		return Obj;
//	}
//	Info_car* getStats() {
//		return this->stats;
//	}
//
//};
//
//class Info_car {
//	Info_car(double car_rpm_max, std::vector<double> car_gears_coef, std::vector<double> car_speed_coef, bool car_gearbox_isauto, double key_rpm_counter) {
//		this->car_rpm_max = car_rpm_max;
//		this->car_gears_coef = car_gears_coef;
//		this->car_speed_coef = car_speed_coef;
//		this->car_gearbox_isauto = car_gearbox_isauto;
//		this->key_rpm_counter = key_rpm_counter;
//	}
//	double car_rpm_max;// = 0.6000;
//	std::vector<double> car_gears_coef;// [6] = { 0, 1.0, 0.2, 0.05, 0.02, 0.005 };
//	std::vector<double> car_speed_coef;// [6] = { 0, 60, 120, 190, 210, 280 };
//	bool car_gearbox_isauto;
//	double key_rpm_counter;// = 0.005;
//};


void car_stats::printStats() {
	std::cout <<
		"Gearbox auto\t" << this->car_gearbox_isauto << std::endl <<
		"RPM key\t" << this->key_rpm_counter << std::endl <<
		"RPM MAX\t" << this->car_rpm_max << std::endl;
	std::cout << "Gear coef" << std::endl;
	for (auto& i : this->car_gears_coef) {
		std::cout << i << std::endl;
	}
	std::cout << "Speed coef" << std::endl;
	for (auto& i : this->car_speed_coef) {
		std::cout << i << std::endl;
	}
}

void Merlo190::printInfo() {
		std::cout << "Mercedes 190 1988 2.3 16 235 PS" << std::endl;
}

void Merlo123::printInfo() {
	std::cout << "Mercedes w123 1979 3.0 TD 160 PS" << std::endl;
}
void Bmw530e34::printInfo() {
	std::cout << "BMW 530 e34 1991 2.0 T 225 PS CIA edition" << std::endl;
}
void Bmw220e30::printInfo() {
	std::cout << "BMW 220 e30 1990 2.2 I 230 PS" << std::endl;
}
void FerrDaytona::printInfo() {
	std::cout << "Ferrari Daytona Spider Miami Vice Edition" << std::endl;
}
void FerrTestarossa::printInfo() {
	std::cout << "Ferrari Testarossa Miami Vice Edition" << std::endl;
}

car_stats* FerrDaytona::getStats() {
	return this->car_stat;
}
car_stats* FerrTestarossa::getStats() {
	return this->car_stat;
}


Car_racer* DLC_Mercedes_1980series::createRacerCar() {
	return new Merlo190();
}
Car_chaser* DLC_Mercedes_1980series::createChaserCar() {
	return new Merlo123();
}

Car_racer* DLC_BMW_1980series::createRacerCar() {
	return new Bmw220e30();
}
Car_chaser* DLC_BMW_1980series::createChaserCar() {
	return new Bmw530e34();
}

Car_racer* DLC_Ferr_1980series::createRacerCar() {
	return new FerrDaytona();
}
Car_chaser* DLC_Ferr_1980series::createChaserCar() {
	return new FerrTestarossa();
}






current_car* current_car::curr_car = 0;

current_car* current_car::Instance() {
	if (curr_car == nullptr) {
		curr_car = new current_car();
	}
	return curr_car;
}
car_stats* current_car::getCurrentCar() {
	return stats;
}

void current_car::select(const car_stats& stats) {
	this->stats = new car_stats(stats);
}