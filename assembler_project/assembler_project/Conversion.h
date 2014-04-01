//This class will include everything needs to convert instructions ,Data or addresses from one numbering system into another

#ifndef CONVERSION_H
#define CONVERSION_H

#pragma once
#include <string> 
#include <cmath> 
#include <sstream> 

class Conversion
{
public:
	Conversion(void);
	~Conversion(void);

	//Conversion between hexadecimal and decimal numbering systems 
	int hexToDec (std::string);

	//Conversion between hexadecimal and binary numbering systems
	std::string hexToBin(std::string);
	std::string hexToBin(std::string,int);
	std::string binToHex(std::string); 

	//Conversion between binary and decimal numbering systems including unsigned, signed and float numbers 
	long long int binToDec(std::string);
	int binToDecSigned(std::string); 
	std::string decToBin(int);
	std::string decToBin(int,int);
	std::string decToBinFloat(double,int);
	double binToDecFloat(std::string);

	//Conversion between hex and ASCII code 
	std::string asciiToHex(std::string);
	std::string hexToAscii(std::string);

	//to expand the string for a certain number of characters
	std::string Expantion(std::string);
	
	 
};

#endif 


