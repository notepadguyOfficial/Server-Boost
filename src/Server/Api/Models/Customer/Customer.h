#pragma once
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <string>

class Customer {
private:
  static unsigned int num_instances;
  unsigned int id;
  std::string first_name;
  std::string last_name;
  std::string phone_number;
  std::string address;
  std::string gender;
  std::string username;
  std::string password;
  std::string address_long;
  std::string address_lat;
  std::string date_of_birth;

public:
  Customer(
    std::string fname,
    std::string lname,
    std::string phone, 
    std::string addr,
    std::string gender,
    std::string username, 
    std::string password,
    std::string addr_long,
    std::string addr_lat, 
    std::string dob
  );

  Customer* get() {
    return this;
  }

  unsigned int Id() const;
  std::string FirstName() const;
  std::string LastName() const;
  std::string PhoneNumber() const;
  std::string Address() const;
  std::string Gender() const;
  std::string Username() const;
  std::string Password() const;
  std::string AddressLong() const;
  std::string AddressLat() const;
  std::string DateOfBirth() const;
};
