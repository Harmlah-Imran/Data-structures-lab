#include <iostream>
using namespace std;
class  Employee {
public:
	virtual double calculateSalary() = 0;
};
class FullTimeEmployee :public Employee {
private:
	double salary;
public:
	FullTimeEmployee(double sal) {
		salary = sal;
	}
	double calculateSalary() {
		return salary;
	}
};
class PartTimeEmployee :public Employee {
private:
	double hourlyrate;
	double hourWorked;
public:
	PartTimeEmployee(double rateperhour, double hours) {
		hourlyrate = rateperhour;
		hourWorked = hours;
	}
	double calculateSalary() {
		return hourlyrate * hourWorked;
	}
};
int main() {
	FullTimeEmployee off(3456.89);
	PartTimeEmployee on(20.2, 60);
	cout << "Salary of Full Time Employee: " << off.calculateSalary() << endl;
	cout << "Salary of Part Time Employee: " << on.calculateSalary() << endl;

}
