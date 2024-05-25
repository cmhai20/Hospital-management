#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::CarePeriod(const Date &start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

void CarePeriod::mark_end_period(const Date &end_date)
{
    end_ = end_date;
}

void CarePeriod::set_staff(const std::string& staff)
{
    staff_ID.insert(staff);
}

void CarePeriod::print_patient_period_infos()
{
    // Print the start date of the care period.
    std::cout << "* Care period: ";
    start_.print();
    std::cout << " -";

    if(!end_.is_default()) {
        std::cout << " ";
        end_.print();
    }
    std::cout << std::endl;

    // Print the staff members assigned to this care period.
    std::cout << "  - Staff:";
    if(staff_ID.empty()) {
        std::cout << " None";
    }
    else {
        for(const std::string& staff : staff_ID) {
            std::cout << " " << staff;
        }
    }
    std::cout << std::endl;
}

void CarePeriod::print_staff_period_infos()
{
    start_.print();
    std::cout << " -";
    if(!end_.is_default()) {
        std::cout << " ";
        end_.print();
    }
    std::cout << std::endl;

    // Print the ID of the patient associated with this care period.
    std::cout << "* Patient: " << patient_->get_id() << std::endl;
}
