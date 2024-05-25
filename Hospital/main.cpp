/* Hospital
 *
 * Desc:
 * This program implements an application for hospital to
 * store information and track it when needed. It can help to read data
 * from a file and store data about date, recruitment, staffs and patients,
 * medicine, care period,...
 * At first, the program will print the prompt and the user is expected
 * to give input. List of inputs can be shown if user types in command
 * "HELP". Each command has a predefined number of parameters.
 * There are some commands such as "enter" or "leave" or
 * "assign_staff", etc. The program can store show the information
 * about both staff and patient. The program will continue until
 * the command "QUIT" is given
 *
 *
 * Program author ( Fill with your own info )
 * Name: Hai Chu
 * Student number: 152251723
 * UserID: bfhach
 * E-Mail: hai.m.chu@tuni.fi
 *
 * */

#include "cli.hh"
#include "hospital.hh"
#include <string>

const std::string PROMPT = "Hosp> ";


int main()
{
    Hospital* hospital = new Hospital();
    Cli cli(hospital, PROMPT);
    while ( cli.exec() ){}

    delete hospital;
    return EXIT_SUCCESS;
}
