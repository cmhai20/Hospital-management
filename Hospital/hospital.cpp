#include "hospital.hh"
#include "utils.hh"
#include <iostream>
#include <set>

Hospital::Hospital()
{
}

Hospital::~Hospital()
{
    // Deallocating staff
    for( std::map<std::string, Person*>::iterator
         iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Deallocating patients
    for(std::map<std::string, Person*>::iterator
        iter = total_patients_data.begin();
        iter != total_patients_data.end();
        ++iter)
        {
            delete iter->second;
        }

     // Deallocating CarePeriod
     for(std::map<std::string, std::vector<CarePeriod*>>::iterator
         iter = patients_care_record.begin();
         iter != patients_care_record.end();
         ++iter)
     {
         for(CarePeriod* care_period : iter->second) {
            delete care_period;
         }
     }

    // Remember to deallocate patients also
}

void Hospital::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not utils::is_numeric(day, false) or
        not utils::is_numeric(month, false) or
        not utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utils::today.print();
    std::cout << std::endl;
}

void Hospital::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    utils::today.print();
    std::cout << std::endl;
}

void Hospital::recruit(Params params)
{
    std::string specialist_id = params.at(0);
    if( staff_.find(specialist_id) != staff_.end() )
    {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }
    Person* new_specialist = new Person(specialist_id);
    staff_.insert({specialist_id, new_specialist});
    std::cout << STAFF_RECRUITED << std::endl;
}

void Hospital::print_all_staff(Params)
{
    if( staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }
    for( std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        std::cout << iter->first << std::endl;
    }
}

void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);
    if( not utils::is_numeric(strength, true) or
        not utils::is_numeric(dosage, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;
}

void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

void Hospital::enter(Params params)
{
    const std::string& patientID = params.at(0);

    //Check if patient is admitted
    if (current_patients_.find(patientID) != current_patients_.end()) {
        std::cout << ALREADY_EXISTS << patientID << std::endl;
        return;
    }

    Person* patientPtr = nullptr;
    auto patientIterator = total_patients_data.find(patientID);

    //Find patient in total_patients_data
    //If not found, create and insert a new one
    if (patientIterator == total_patients_data.end()) {
        patientPtr = new Person(patientID);
        total_patients_data[patientID] = patientPtr;
    } else {
        //Reuse the existing patient object
        patientPtr = patientIterator->second;
    }

    current_patients_.insert({patientID, patientPtr});

    //Initialize new care period
    CarePeriod* care_period = new CarePeriod(utils::today, patientPtr);
    patients_care_record[patientID].push_back(care_period);

    std::cout << PATIENT_ENTERED << std::endl;
}

void Hospital::leave(Params params)
{
    const std::string& patientID = params.at(0);

    //Search for the patient in the current_patients_ map to
    //check if they're currently admitted.
    auto patientIterator = current_patients_.find(patientID);

    if (patientIterator == current_patients_.end()) {
        std::cout << CANT_FIND << patientID << std::endl;
        return;
    }

    // Remove the patient from the current_patients_ map
    current_patients_.erase(patientID);
    auto& carePeriods = patients_care_record.at(patientID);

    //Check if there are any care periods for the patient.
    //If so, mark the end date of the latest one.
    if (!carePeriods.empty()) {
        carePeriods.back()->mark_end_period(utils::today);
    }

    std::cout << PATIENT_LEFT << std::endl;
}

void Hospital::assign_staff(Params params)
{
    const std::string& staffID = params.at(0);
    const std::string& patientID = params.at(1);

    auto it1 = staff_.find(staffID);
    auto it2 = current_patients_.find(patientID);

    //Check staff and patient existence
    if (it1 == staff_.end() || it2 == current_patients_.end()) {
        std::cout << CANT_FIND <<
                (it1 == staff_.end() ? staffID : patientID) << std::endl;
        return;
    }

    //Assign staff for latest period
    CarePeriod* latestCarePeriod = patients_care_record.at(patientID).back();
    latestCarePeriod->set_staff(staffID);

    //Update staff care period
    auto it = staffs_care_record.find(staffID);

    if (it == staffs_care_record.end()) {
        std::vector<CarePeriod*> new_set {latestCarePeriod};
        staffs_care_record.insert({staffID, new_set});
    } else {
        it->second.push_back(latestCarePeriod);
    }

    std::cout << STAFF_ASSIGNED << patientID << std::endl;
}

void Hospital::print_patient_info(Params params)
{
    const std::string& patientID = params.at(0);

    // Check if the patient exists in the care record map
    if (patients_care_record.find(patientID) == patients_care_record.end()) {
        std::cout << CANT_FIND << patientID << std::endl;
        return;
    }

    // Retrieve and iterate over all care periods of the patient
    std::vector<CarePeriod*> care_period = patients_care_record.find(patientID)->second;
    for (CarePeriod* period : care_period) {
        period->print_patient_period_infos();
    }
    std::cout << "* Medicines:";
    total_patients_data.at(patientID)->print_medicines("  - ");
}

void Hospital::print_care_periods(Params params)
{
    const std::string& staffID = params.at(0);

    // Check if the staff member exists
    if (staff_.find(staffID) == staff_.end()) {
        std::cout << CANT_FIND << staffID << std::endl;
        return;
    }

    // Find care periods associated with the staff member
    auto staff_care_it = staffs_care_record.find(staffID);
    if (staff_care_it == staffs_care_record.end()) {
        std::cout << "None" << std::endl;
    } else {
        for (CarePeriod* staff_period : staff_care_it->second) {
            staff_period->print_staff_period_infos();
        }
    }
}

// Constructs a map of medicines to the list of patients prescribed those medicines
std::map<std::string, std::vector<std::string>> Hospital::prescribed_medicine() {
    std::map<std::string, std::vector<std::string>> medicines;

     // Iterate over all patients
    for (const auto& patient_data : total_patients_data) {
        const std::string& patientID = patient_data.first;
        Person* patient = patient_data.second;
        const std::vector<std::string>& patient_med = patient->get_medicines();

        // Iterate over medicines prescribed to the patient
        for (const std::string& medicine : patient_med) {
            if (medicines.find(medicine) == medicines.end()) {
                std::vector<std::string> newPatientList = {patientID};
                medicines.insert({medicine, newPatientList});
            } else {
                medicines.find(medicine)->second.push_back(patientID);
            }

        }
    }

    return medicines;
}

void Hospital::print_all_medicines(Params)
{
    std::map<std::string,std::vector<std::string>>
            medicines = prescribed_medicine();

    if (medicines.empty()) {
        std::cout << "None" << std::endl;
        return;
    }

    for (const auto& [medicine, patients] : medicines) {
        std::cout << medicine << " prescribed for" << std::endl;
        for (const auto& patientID : patients) {
            std::cout << "* " << patientID << std::endl;
        }
    }
}

void Hospital::print_all_patients(Params)
{
    if (total_patients_data.empty()) {
        std::cout << "None" << std::endl;
        return;
    }

    for (const auto& [patientID, person] : total_patients_data) {
        std::cout << patientID << std::endl;
        print_patient_info({patientID});
    }
}

void Hospital::print_current_patients(Params)
{
    if (current_patients_.empty()) {
        std::cout << "None" << std::endl;
        return;
    }

    for (const auto& [patientID, person] : current_patients_) {
        std::cout << patientID << std::endl;
        print_patient_info({patientID});
    }
}

