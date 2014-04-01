#include "Conversion.h"
#include <cmath>


Conversion::Conversion(void)
{
}


Conversion::~Conversion(void)
{
}

//To convert from hexadecimal into decimal 
int Conversion::hexToDec ( std::string h)
{
	return binToDec(hexToBin(h));
} 


//To convert from hexadecimal into binary 
std::string Conversion::hexToBin(std::string h)
{
	std::string b = "";
		for ( size_t i=0; i < h.length(); i++)//convert each hex digit in the h string to it's 4 corresponding  binary digits
		{ 
			switch(h.at(i))
			{
			case'0':
				b += "0000";
				break; 
			case'1':
				b += "0001";
				break; 
			case'2':
				b += "0010";
				break;
			case'3':
				b += "0011";
				break;
			case'4':
				b += "0100";
				break;
			case'5':
				b += "0101";
				break;
			case'6':
				b += "0110";
				break;
			case'7':
				b += "0111";
				break;
			case'8':
				b += "1000";
				break;
			case'9':
				b += "1001";
				break;
			case'A':
			case'a':
				b += "1010";
				break; 
			case'B':
			case'b':
				b += "1011";
				break;
			case'C':
			case'c':
				b += "1100";
				break;
			case'D':
			case'd':
				b += "1101";
				break;
			case'E':
			case'e':
				b += "1110";
				break;
			case'F':
			case'f':
				b += "1111";
				break;
			} 
		} 

		return b;  //the 4 binary digits for each string are added to the b variable and the bis returned as a binary of string 
} 

//To convert from binary into decimal 
long long int Conversion::binToDec(std::string b)
{
		long long int d = 0;
		std::string str = "";
		for ( int i= (b.length() -1 ); i >= 0 ; i--)
		{
			str += b.at(i);
		} 

		for (int i = 0 ; i < str.length(); i++)
		{
			char c = str.at(i);
			int one = c - '0';
			d += one * pow(2.0,i);
		} 
		return d; 
	

} 

//to convert from dec to binary for unsigned int values
std::string Conversion::decToBin(int dec)
{
	std::string bin_inv = "", bin = "" , ss ="";
	std::stringstream s;

	if(dec==0)
		return "0";

	else {while(dec != 0)
	{
		s << dec % 2;
		ss = s.str();
		dec = dec/2; 
	} 

	bin_inv = ss; 

	for (int i = (bin_inv.length()) - 1; i >= 0 ; i--)
	{
		bin += bin_inv.at(i); 
	} 

	return bin;}
} 

//to convert from decimal into binary for signed & unsigned floating numbers 
std::string Conversion :: decToBinFloat(double f , int l)
{
	std::string sign="" , exp ="", fra="" , rDot ="", lDot="";
	int vL ,e;
	double vR ;
	std::string result = "";

	if ( f == 0 )// a value of zero is represented by setting all bits including sign, exponent and fraction to 0
	{
		while (result.length() < 36)
		{
			result += '0';
		} 
	}
	else
	{ 

		if ( f < 0) // for negative values make the sign bit = 1 
		{
			sign = '1';
			f = -1 * f;
		}
		else  // for positive values sign = 0 
		{
			sign = '0';
		} 

		//keep the decimal int part inside vL & the fraction decimal part inside vR
		vL = floor(f);
		vR = f - floor(f);

		lDot = decToBin(vL);//the decimal int part in simply converted from decimal to binary

		//to convert the decimal fraction part from decimal into binary 
		while (rDot.length() < l-lDot.length())//to make sure that the length of the strength is as specified 
		{
			vR *= 2;
			if (vR >= 1)//The int part is 1 
			{
				rDot += "1";
				vR = vR - 1;
			}
			else//the int part is 0
			{
				rDot += "0";
			}
		} 
	 
		//to determine the exponent and fraction for each float number 

		if( vL > 0)//for positive numbers
		{
			e = 1024 + lDot.length();
			exp = decToBin(e,11);
			fra = lDot + rDot;
			while ( fra.length() < 36)
			{
				fra += '0';
			}
		}
		else
		{
			int i = 0;
			while (rDot.at(i) != '1')
			{
				i++;
			}
			e = 1024 - i;
			exp = decToBin(e,11);
			fra = rDot.substr(i,rDot.length()-i);
			while ( fra.length() < 36)
			{
				fra += '0';
			}
		} 

		result = sign + exp + fra.substr(0,36); //the float number is returned as a string of binary numbers 
	} 

	return result;
} 

//To convert from binary into hexadecimal 
std::string Conversion::binToHex(std::string b)
{
	//This is to make sure that the binary string has multiple of 4 bits
	while ( b.length() % 4 != 0 )
       {
               b = "0" + b;
       }

	std::string c = "" , l ="" , hex_inv = "" , hex= ""; 
	int x = b.length();
	for (int i = b.length() -1 ; i >= 0;)
	{
		int j = 4;
		l = ""; c = ""; //Empty these for a new hex digit!!
		while ( j != 0)
		{
			c += b.at(i);
			i--; 
			j--; 
		} 

		
		for (int ii = (c.length()) - 1; ii >= 0 ; ii--)
		{ 
			l += c.at(ii);
		} 

		if ( l == "0000" ) //convert each four binary bits to their corresponding hex digit
		{
			hex_inv += "0";
		} 
		else
		{
			if ( l == "0001" )
			{ 
				hex_inv += "1";
			} 
			else
			{
				if ( l == "0010") 
				{
					hex_inv += "2";
				} 
				else
				{
					if ( l == "0011" )
					{
						hex_inv += "3";
					}
					else
					{
						if (l == "0100")
						{
							hex_inv += "4";
						} 
						else
						{
							if ( l == "0101")
							{
								hex_inv += "5";
							} 
							else
							{
								if( l == "0110")
								{
									hex_inv += "6";
								} 
								else
								{
									if ( l == "0111")
									{
										hex_inv += "7";
									}
									else
									{
										if ( l == "1000")
										{
											hex_inv += "8";
										} 
										else
										{
											if( l == "1001")
											{
												hex_inv += "9";
											}
											else
											{
												if ( l == "1010")
												{
													hex_inv += "A";
												}
												else
												{
													if ( l == "1011")
													{
														hex_inv += "B";
													}
													else
													{
														if ( l == "1100")
														{
															hex_inv += "C";
														}
														else
														{
															if ( l == "1101")
															{ 
																hex_inv += "D";
															} 
															else
															{
																if (l == "1110")
																{
																	hex_inv += "E";
																}
																else
																{
																	if(l == "1111")
																	{
																		hex_inv += "F";
																	} 
																} 
															} 
														}

													} 
												} 
											}
									} 

								} 
							} 
						} 
					} 
				} 
			} 
		} 

	} 
	} 

	for (int i = (hex_inv.length()) - 1; i >= 0 ; i--) //have the inverse to get the correct value 
	{ 
		hex += hex_inv.at(i);
	}

	return hex; //the result is represented as a string of hex digits
} 

//To convert from binary into decimal for signed int values
int Conversion::binToDecSigned(std::string b)
{
	b=hexToBin(Expantion(binToHex(b)));
	if (b.at(0) == '1')//for negative numbers 
	{
		int value = binToDec(b);
		int power = b.length();
		return (value - pow(2.0,power));
	} 
	else // for positive numbers 
	{
		//normal conversion 
		return binToDec(b);
	} }
 
//Expantion Hex number
std::string Conversion::Expantion(std::string sp){
	if(sp=="")
	{
		sp="000000";
		return sp;
	}
	else 
		if(sp.length()<6)
		{//expantion
		    int nx=sp.length();
		    for (int l=0;l<5-nx;l++)
				sp+="0";
		    std::string aa =sp.substr(nx);
			sp=aa+sp.substr(0,nx);
		}

	return sp;
}

//To convert hex numbers into a binary string with a specified length 
std::string Conversion::hexToBin(std::string h,int length)
{
	std::string s = hexToBin(h);
	while(s.length() <length)//expansion until the specified length is reached
	{
		s = '0' + s;
	}

	return s;
} 

//To convert decimal numbers into a binary string with a specified length 
std::string Conversion::decToBin(int d,int length)
{
	if (d < 0)
		d = pow(2.0,length) + d;

	std::string s = decToBin(d);
	while(s.length() <length)//expansion until the specified length is reached 
	{
		s = '0' + s;
	}

	return s;
} 

//To convert from bin to decimcal for signed and unsigned float numbers 
double Conversion::binToDecFloat(std::string f)
{
	std::string exp = "" , fra = "";
	int sign , value , e;

	if(f.at(0) == '0')
		sign = 1;
	else
		sign = -1; 

	exp = f.substr(1,11);
	fra = '0' + f.substr(12,36);

	e = binToDec(exp);
	value = binToDec(fra);
	value *= pow(2.0,(e-1060));
	value *= sign;

	return value; 

} 


std::string Conversion::asciiToHex(std::string b){

	switch(b.at(0))
			{
			case'A':
				b= "41";
				break; 
			case'B':
				b= "42";
				break; 
			case'C':
				b= "43";
				break;
			case'D':
				b= "44";
				break;
			case'E':
				b= "45";
				break;
			case'F':
				b= "46";
				break;
			case'G':
				b= "47";
				break;
			case'H':
			    b= "48";
				break;
			case'I':
				b= "49";
				break;
			case'J':
				b= "4A";
				break;
			case'K':
				b= "4B";
				break; 
			case'L':
			    b= "4C";				
				break;
			case'M':
			   b= "4D";
				break;
			case'N':
			   b= "4E";
				break;
			case'O':
			   b= "4F";
				break;
			case'P':
				b= "50";
				break;
				case'Q':
			    b= "51";				
				break;
			case'R':
			   b= "52";
				break;
			case'S':
			   b= "53";
				break;
			case'T':
			   b= "54";
				break;
			case'U':
				b= "55";
				break;

					case'V':
			    b= "56";				
				break;
			case'W':
			   b= "57";
				break;
			case'X':
			   b= "58";
				break;
			case'Y':
			   b= "59";
				break;
			case'Z':
				b= "5A";
				break;

				
					case'0':
			    b= "30";				
				break;
			case'1':
			   b= "31";
				break;
			case'2':
			   b= "32";
				break;
			case'3':
			   b= "33";
				break;
			case'4':
				b= "34";
				break;

				case'5':
			   b= "35";
				break;
			case'6':
			   b= "36";
				break;
			case'7':
				b= "37";
				break;
				case'8':
			   b= "38";
				break;
			case'9':
			   b= "39";
				break;
			case' ':
				b= "00";
				break;

				case'a':
				b= "61";
				break; 
			case'b':
				b= "62";
				break; 
			case'c':
				b= "63";
				break;
			case'd':
				b= "64";
				break;
			case'e':
				b= "65";
				break;
			case'f':
				b= "66";
				break;
			case'g':
				b= "67";
				break;
			case'h':
			    b= "68";
				break;
			case'i':
				b= "69";
				break;
			case'j':
				b= "6A";
				break;
			case'k':
				b= "6B";
				break; 
			case'l':
			    b= "6C";				
				break;
			case'm':
			   b= "6D";
				break;
			case'n':
			   b= "6E";
				break;
			case'o':
			   b= "6F";
				break;
			case'p':
				b= "70";
				break;
				case'q':
			    b= "71";				
				break;
			case'r':
			   b= "72";
				break;
			case's':
			   b= "73";
				break;
			case't':
			   b= "74";
				break;
			case'u':
				b= "75";
				break;

					case'v':
			    b= "76";				
				break;
			case'w':
			   b= "77";
				break;
			case'x':
			   b= "78";
				break;
			case'y':
			   b= "79";
				break;
			case'z':
				b= "7A";
				break;


					} 
	return b;


}


std::string Conversion::hexToAscii(std::string b){

	
			if (b=="41")
				b= "A";
			else if (b=="42")
				b= "B";
		    else if (b=="43")	 
				b= "C";
			 else if (b=="44")
				b= "D";
			 else if (b=="45")
				b= "E";
		     else if (b=="46")
				b= "F";
			 else if (b=="47")
				b= "G";
			 else if (b=="48")
			    b= "H";
			 else if (b=="49")
				b= "K";
			 else if (b=="4A")
				b= "J";
			 else if (b=="4B")
				b= "K";
			 else if (b=="4C")
			    b= "L";				
			 else if (b=="4D")
			   b= "M";
			 else if (b=="4E")
			   b= "N";
			 else if (b=="4F")
			   b= "O";
			 else if (b=="50")
				b= "P";
			else if (b=="51")
			    b= "Q";				
			else if (b=="52")
			   b= "R";
			else if (b=="53")
			   b= "S";
			else if (b=="54")
			   b= "T";
			else if (b=="55")
				b= "U";
			else if (b=="56")
			    b= "V";				
			else if (b=="57")
			   b= "W";
			else if (b=="58")
			   b= "X";
			else if (b=="59")
			   b= "Y";
			else if (b=="5A")
				b= "Z";
			else if (b=="30")
				b= "0";
			else if (b=="31")
			    b= "1";				
			else if (b=="32")
			   b= "2";
			else if (b=="33")
			   b= "3";
			else if (b=="34")
			   b= "4";
			else if (b=="35")
				b= "5";
			else if (b=="36")
			    b= "6";				
			else if (b=="37")
			   b= "7";
			else if (b=="38")
			   b= "8";
			else if (b=="39")
			   b= "9";
			else if (b=="00")
				b= " ";
			
			else if (b=="61")
				b= "a";
			else if (b=="62")
				b= "b";
		    else if (b=="63")	 
				b= "c";
			 else if (b=="64")
				b= "d";
			 else if (b=="65")
				b= "e";
		     else if (b=="66")
				b= "f";
			 else if (b=="67")
				b= "g";
			 else if (b=="68")
			    b= "h";
			 else if (b=="69")
				b= "k";
			 else if (b=="6A")
				b= "j";
			 else if (b=="6B")
				b= "k";
			 else if (b=="6C")
			    b= "l";				
			 else if (b=="6D")
			   b= "m";
			 else if (b=="6E")
			   b= "n";
			 else if (b=="6F")
			   b= "o";
			 else if (b=="70")
				b= "p";
			else if (b=="71")
			    b= "q";				
			else if (b=="72")
			   b= "r";
			else if (b=="73")
			   b= "s";
			else if (b=="74")
			   b= "t";
			else if (b=="75")
				b= "u";
			else if (b=="76")
			    b= "v";				
			else if (b=="77")
			   b= "w";
			else if (b=="78")
			   b= "x";
			else if (b=="79")
			   b= "y";
			else if (b=="7A")
				b= "z";			

												
	return b;


}