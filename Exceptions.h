#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>

//------------------------------------- EXCEPTION CLASSES -------------------------------------//

class WorldCupException: public std::exception{};

class InvalidInput: public WorldCupException{};


class CompanyAlreadyExists: public EmployeeManagerException{};

class EmployeeAlreadyExists : public EmployeeManagerException{};

class CompanyDoesNotExist  : public EmployeeManagerException{};

class EmployeeDoesNotExist : public EmployeeManagerException{};

class ActiveCompany : public EmployeeManagerException{};

class InActiveCompany : public EmployeeManagerException{};

class FailedPurchase : public EmployeeManagerException{};

class NoEmployees : public EmployeeManagerException{};

class InvalidNumOfCompaniesMatching : public EmployeeManagerException{};


#endif