#include <iostream>
using namespace std;
class Course {
public:
	virtual int duration() = 0;
};
class OnlineCourse :public Course {
private:
	int weeks;
	int hoursPerWeek;
public:
	OnlineCourse(int noOfweeks, int hour) {
		weeks = noOfweeks;
		hoursPerWeek = hour;
	}
	int duration() {
		return weeks * hoursPerWeek;
	}
};
class OfflineCourse :public Course {
private:
	int months;
	int hoursPerDay;
public:
	OfflineCourse(int noOfMonths, int hours) {
		months = noOfMonths;
		hoursPerDay = hours;
	}
	int duration() {
		return months * hoursPerDay * 30;
	}
};
int main() {
	OfflineCourse off(3, 83);
	OnlineCourse on(2, 60);
	cout << "Duratin of offline course: " << off.duration() << endl;
	cout << "Duratin of online course: " << on.duration() << endl;

}
