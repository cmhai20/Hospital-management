/* Class CarePeriod
 * ----------
 * COMP.CS.110 SPRING 2024
 * ----------
 * Class for describing a patient's care period in hospital.
 *
 * Note: Students need to implement almost all of this class by themselves.
 * */
#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include <string>
#include <set>

class CarePeriod
{
public:
    /**
      * @brief CarePeriod
      * @param start date given as a string (ddmmyyyy)
      * @param patient whose care period will be created
      */
    CarePeriod(const std::string& start, Person* patient);

    /**
      * @brief CarePeriod
      * @param start date given as a Date object
      * @param patient whose care period will be created
      */
    CarePeriod(const Date& start, Person* patient);

    /**
      * @brief destructor
      */
    ~CarePeriod();

    void mark_end_period (const Date& end_date);

    //Add staff member's ID to care period
    void set_staff(const std::string& staff);

    //Prints the details of the care period: the start and end dates, the staff assigned
    void print_patient_period_infos();

    //Prints the details of the care period focusing on the staff's perspective:
    //the duration of the care period and the patient's ID
    void print_staff_period_infos();

private:
    Person* patient_;
    Date start_;
    Date end_ = Date(); //Initialized to default Date object

    std::set<std::string> staff_ID; //Track staff in care period
                                    // avoid duplicate using set


};

#endif // CAREPERIOD_HH
