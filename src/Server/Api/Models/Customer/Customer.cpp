#include "Customer.h"

unsigned int Customer::num_instances = 0;

Customer::Customer(
    std::string fname,
    std::string lname,
    std::string phone,
    std::string addr,
    std::string gender,
    std::string username,
    std::string password,
    std::string addr_long,
    std::string addr_lat,
    std::string dob) : first_name(fname),
                       last_name(lname),
                       phone_number(phone),
                       address(addr),
                       gender(gender),
                       username(username),
                       password(password),
                       address_long(addr_long),
                       address_lat(addr_lat),
                       date_of_birth(dob)
{
}

unsigned int Customer::Id() const 
{ 
    return id;
}

std::string Customer::FirstName() const
{
    return first_name;
}

std::string Customer::LastName() const
{
    return last_name;
}

std::string Customer::PhoneNumber() const
{
    return phone_number;
}

std::string Customer::Address() const
{
    return address;
}

std::string Customer::Gender() const
{
    return gender;
}

std::string Customer::Username() const
{
    return username;
}

std::string Customer::Password() const
{
    return password;
}

std::string Customer::AddressLong() const
{
    return address_long;
}

std::string Customer::AddressLat() const
{
    return address_lat;
 }

std::string Customer::DateOfBirth() const
{
    return date_of_birth;
}
