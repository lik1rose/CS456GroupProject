#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Conversion.h"

using namespace std;

void pass1();
void pass2();
void create_optab();
string search_optab(string str);
string search_symtab(string str);
int directive(string str);
void expand(string &str , int length);
void generate_intermediateFile();
void generate_ObjFile();
void generate_Relocateablefile();
string registerNum(string r);


string line , word , start , name , locctr,temp , operand;
int commentFlag , startFlag , format4Flag , line_num = 0 , label_num = 0 , literal_num = 0;

string niFlag , xFlag , bFlag , pFlag , eFlag;
string B_register ;
	
string analysis [1000][7] , symtab[50][3] , optab[59][2] , littab[50][2];
std::stringstream out;
istringstream instruction;

Conversion* C = new Conversion();

ifstream file("input.txt");

int main()
{
	create_optab();
	pass1();
	pass2();
	
	generate_intermediateFile();
 	generate_ObjFile();
	generate_Relocateablefile();

	int i = 0;
	while(analysis[i][0] != "\0")
	{
		cout << analysis[i][0] << "\t" << analysis[i][1] << "\t" << analysis[i][2] << "\t" << analysis[i][3] << "\t" << analysis[i][4] << "\t" << analysis[i][5] << "\t" << analysis[i][6] << endl;
		i++;
	}

	i = 0;
	while(symtab[i][0] != "\0")
	{
		cout << symtab[i][0] << "\t" << symtab[i][1] << "\t" << symtab[i][2] << endl;
		i++;

	} 	
	

	return 0;
}

/*This function is for implementing pass one and assign a location for each label in the code*/
void pass1()
{
	ifstream file("input.txt");
	
	if(!file.is_open())
	{
		cerr << "couldn't open file\n";
		exit(EXIT_FAILURE);	
	}
	else
	{	
 		while(!file.eof())
		{
			getline(file,line);
			istringstream instruction(line);

			commentFlag = line.find("!_!_!");
			startFlag = line.find("START");

			if(startFlag >= 0)//The START line 
			{		
				instruction >> name;

				symtab[label_num][0] = name;
				
				out.str("");
				out << (line_num+1);
				analysis[line_num][0] = out.str(); //line number

				instruction >> word;
				instruction >> locctr;
				start = locctr;
				symtab[label_num][1] = locctr;
				symtab[label_num][2] = "-";
				label_num++;

				analysis[line_num][1] = "-";//location
				analysis[line_num][2] = "-";//label name
				analysis[line_num][3] = "-";//opcode
				analysis[line_num][4] = "-";//flag
				analysis[line_num][5] = "-";//Displacement
				analysis[line_num][6] = "-";//Object code
				
				
			}
			else
			{
				if(commentFlag != 0)
				{
					instruction >> word;
					//This word could be a label name (label  instruction), a variabel name(variable BYTE/WORD ...), or a directive

					if(word == "*" || word == "EQU")
						instruction >> operand;

					if(directive(word))
					{
					}
					else
					{
						if(word.at(0) == '+')
						{
							temp = word.substr(1);
							eFlag = "1";
							pFlag = "0";
						}
						else
						{
							temp = word;
							eFlag = "0";
							pFlag = "1";
						}

						if(search_optab(temp) != "-")//This is an instruction 
						{
							out.str("");
							out << (line_num+1);
							analysis[line_num][0] = out.str(); //line number
							analysis[line_num][1] = locctr;//location

							if(eFlag == "1")//modify the location counter
							{
								locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 4)));
							}
							else
							{
								if(temp == "FIX" || temp == "FLOAT" || temp == "HIO" || temp == "NORM" || temp == "SIO" || temp == "TIO")
								{
									locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 1)));
								}
								else
								{
									if(temp == "ADDR" || temp == "CLEAR" || temp == "COMPR" || temp == "DIVR" || temp == "MULR" || temp == "RMO" || temp == "SHIFTL" || temp == "SHIFTL" || temp == "SUBR" || temp == "SVC" || temp == "TIXR")
									{
										locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 2)));
									}
									else
									{
										locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 3)));
									} 
								} 
							} 

							analysis[line_num][2] = "-";//label name
							analysis[line_num][3] = search_optab(temp);//opcode
						
							instruction >> word;
							int x = word.find(",X");
							if(x >= 0)
							{
								int size = word.size() - 2;

								word = word.substr(0,size);
								xFlag = "1";
							}
							else
							{
								xFlag = "0";
							} 

							if(word.at(0) == '#')
							{
								niFlag = "01";
								word = word.substr(1);
								pFlag = "0";
							} 
							else
							{
								if(word.at(0) == '@')
								{
									niFlag = "10";
									word = word.substr(1);
								} 
								else
								{
									niFlag = "11";
								} 
							} 
							bFlag = "0"; 

							analysis[line_num][4] = niFlag + xFlag + bFlag + pFlag + eFlag;//flag
							analysis[line_num][5] = word;//Displacement
							analysis[line_num][6] = "-";//Object code
						}
						else
						{
							instruction >> word;
							instruction >> operand;

							if(word.at(0) == '+')
							{
								word = word.substr(1);
								eFlag = "1";
								pFlag = "0";
							}
							else
							{
								eFlag = "0";
								pFlag = "1";
							} 

							if(directive(word))
							{

							}
							else
							{
								if(search_optab(word) != "-")
								{	
									out.str("");
									out << (line_num+1);
									analysis[line_num][0] = out.str(); //line number
									analysis[line_num][1] = locctr;//location

									symtab[label_num][0] = temp;
									symtab[label_num][1] = locctr;
									symtab[label_num][2] = "-";

									if(eFlag == "1")//modify the location counter
									{
										locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 4))); // format 4
									}
									else
									{
										if(word == "FIX" || word == "FLOAT" || word == "HIO" || word == "NORM" || word == "SIO" || word == "TIO")//format 1
										{
											locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 1)));
										}
										else
										{
											if(word == "ADDR" || word == "CLEAR" || word == "COMPR" || word == "DIVR" || word == "MULR" || word == "RMO" || word == "SHIFTL" || word == "SHIFTL" || word == "SUBR" || word == "SVC" || word == "TIXR")//format 2
											{
												locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 2)));
											}
											else
											{
												locctr= C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 3))); // format 3 											
											} 
										} 
									} 

									analysis[line_num][2] = temp;//label name
									analysis[line_num][3] = search_optab(word);//opcode

								
									instruction >> word;
									int x = word.find(",X");
									if(x >= 0)
									{
										int size = word.size() - 2;

										word = word.substr(0,size);
										xFlag = "1";
									}
									else
									{
										xFlag = "0";
									} 

									if(word.at(0) == '#')
									{
										niFlag = "01";
										word = word.substr(1);
										pFlag = "0";
									} 
									else
									{
										if(word.at(0) == '@')
										{
											niFlag = "10";
											word = word.substr(1);
										} 
										else
										{
											niFlag = "11";
										} 
									} 
									bFlag = "0"; 

									analysis[line_num][4] = niFlag + xFlag + bFlag + pFlag + eFlag;//flag
									analysis[line_num][5] = operand;//Displacement
									analysis[line_num][6] = "-";//Object code
								}
							} 
							label_num++;

						} 
					} 

				}
				else
				{
					out.str("");
					out << (line_num+1);
					analysis[line_num][0] = out.str(); //line number
					analysis[line_num][1] = "-";
					analysis[line_num][2] = "-";
					analysis[line_num][3] = "-";
					analysis[line_num][4] = "-";
					analysis[line_num][5] = "-";
					analysis[line_num][6] = "-";
				}
				}
				line_num++;
				
		}
	}
}

/*This function is to generate the object code for each instruction*/
void pass2()
{
	int i = 0; 
	while(analysis[i][0] != "\0")
	{
		string instruction_binary = "";
		if(analysis[i][3] == "BASE")
		{
			B_register = C->hexToBin(search_symtab(analysis[i][5]));
		}
		else
		{
			if(analysis[i][3] != "-" && analysis[i][3] != "BYTE" && analysis[i][3] != "WORD" && analysis[i][3] != "RESB" && analysis[i][3] != "RESW" && analysis[i][3] != "END" && analysis[i][3] != "LTORG" && analysis[i][3] != "ORG")
			{
				if(analysis[i][3] == "4C")
				{
					analysis[i][6] = "4F0000";
				} 
				else
				{
				if(analysis[i][3] == "C4" || analysis[i][3] == "C0" || analysis[i][3] == "F4" || analysis[i][3] == "C8" || analysis[i][3] == "F0" || analysis[i][3] == "F8")
				{
					analysis[i][6] = analysis[i][3];
				}
				else
				{
					if(analysis[i][3] == "90" || analysis[i][3] == "B4" || analysis[i][3] == "A0" || analysis[i][3] == "9C" || analysis[i][3] == "98" || analysis[i][3] == "AC" || analysis[i][3] == "A8" || analysis[i][3] == "B0" || analysis[i][3] == "B8")
					{
						int r = analysis[i][5].find(",");
						if(r >= 0)
						{
							analysis[i][6] = analysis[i][3];
							analysis[i][6] += registerNum(analysis[i][5].substr(r-1,1));
							analysis[i][6] += registerNum(analysis[i][5].substr(r+1,1));
							
						}
						else
						{
							analysis[i][6] = analysis[i][3];
							analysis[i][6] += registerNum(analysis[i][5]);
							analysis[i][6] +="0";
						} 
					}
					else
					{
						if(analysis[i][4].at(5) == '1')//format 4
						{
							instruction_binary += C->hexToBin(analysis[i][3],8);
							instruction_binary.replace(6,2,analysis[i][4]);
							
							if(search_symtab(analysis[i][5]) == "-")
							{
								int num;
								string d = analysis[i][5];
								istringstream(d) >> num;
								instruction_binary += C->decToBin(num,20);
								analysis[i][6] = C->binToHex(instruction_binary);
							}
							else
							{
								string d = search_symtab(analysis[i][5]); 
								instruction_binary += C ->hexToBin(d,20);
								analysis[i][6] = C->binToHex(instruction_binary);
															
							}
						}
						else // format 3
						{
 							instruction_binary += C->hexToBin(analysis[i][3],8);
							instruction_binary.replace(6,2,analysis[i][4]);
							if(analysis[i][4].at(0) == '0' && analysis[i][4].at(1) == '1')
							{
								if(search_symtab(analysis[i][5]) == "-")
								{
									instruction_binary += C->hexToBin(analysis[i][5],12);
								}
								else
								{
									instruction_binary.replace(10,1,"1");
									string d = search_symtab(analysis[i][5]);
									d = d.substr(d.length()-3);
									int TA = C->binToDec(C->hexToBin(d)); int PC = 0;
									if(analysis[i+1][1] != "-")
									{
										PC = C->binToDec(C->hexToBin(analysis[i+1][1]));										
									}
									else
									{
										int j = 2;
										while(analysis[i+j][1] == "-")
										{
											j++;
										}
										PC = C->binToDec(C->hexToBin(analysis[i+j][1]));
									} 
									int disp = TA - PC;

									string str = C ->decToBin(disp,12);
									instruction_binary += str;
								}
							}
							else
							{
								int TA = C->binToDec(C->hexToBin(search_symtab(analysis[i][5])));
								int PC = C->binToDec(C->hexToBin(analysis[i+1][1]));
								int disp = TA - PC;
								if(abs(disp) < 4095)
								{
									string d = C ->decToBin(disp,12);
									instruction_binary += d;
								}
								else
								{
									instruction_binary.replace(9,2,"10");
									string d = C ->decToBin(0,12);
									instruction_binary += d;
								} 
							}
							analysis[i][6] = C->binToHex(instruction_binary);
						} 
					}
				}	
				} 
			}
		} 
		i++;
	}
}

/*This function is to create the opcode table */
void create_optab()
{
	optab[0][0] = "ADD";    optab[0][1] = "18";
	optab[1][0] = "ADDF";   optab[1][1] = "58";
	optab[2][0] = "ADDR";   optab[2][1] = "90";
	optab[3][0] = "AND";    optab[3][1] = "40";
	optab[4][0] = "CLEAR";  optab[4][1] = "B4";
	optab[5][0] = "COMP";   optab[5][1] = "28";
	optab[6][0] = "COMPF";  optab[6][1] = "88";
	optab[7][0] = "COMPR";  optab[7][1] = "A0";
	optab[8][0] = "DIV";    optab[8][1] = "24";
	optab[9][0] = "DIVF";   optab[9][1] = "64";
	optab[10][0] = "DIVR";  optab[10][1] = "9C";
	optab[11][0] = "FIX";   optab[11][1] = "C4";
	optab[12][0] = "FLOAT"; optab[12][1] = "C0";
	optab[13][0] = "HIO";   optab[13][1] = "F4";
	optab[14][0] = "J";     optab[14][1] = "3C";
	optab[15][0] = "JEQ";   optab[15][1] = "30";
	optab[16][0] = "JGT";   optab[16][1] = "34";
	optab[17][0] = "JLT";   optab[17][1] = "38";
	optab[18][0] = "JSUB";  optab[18][1] = "48";
	optab[19][0] = "LDA";   optab[19][1] = "00";
	optab[20][0] = "LDB";   optab[20][1] = "68";
	optab[21][0] = "LDCH";  optab[21][1] = "50";
	optab[22][0] = "LDF";   optab[22][1] = "70";
	optab[23][0] = "LDL";   optab[23][1] = "08";
	optab[24][0] = "LDS";   optab[24][1] = "6C";
	optab[25][0] = "LDT";   optab[25][1] = "74";
	optab[26][0] = "LDX";   optab[26][1] = "04";
	optab[27][0] = "LPS";   optab[27][1] = "D0";
	optab[28][0] = "MUL";   optab[28][1] = "20";
	optab[29][0] = "MULF";	optab[29][1] = "60";
	optab[30][0] = "MULR";  optab[30][1] = "98";
	optab[31][0] = "NORM";  optab[31][1] = "C8";
	optab[32][0] = "OR";    optab[32][1] = "44";
	optab[33][0] = "RD";    optab[33][1] = "D8";
	optab[34][0] = "RMO";   optab[34][1] = "AC";
	optab[35][0] = "RSUB";  optab[35][1] = "4C";
	optab[36][0] = "SHIFTL";optab[36][1] = "A4";
	optab[37][0] = "SHIFTR";optab[37][1] = "A8";
	optab[38][0] = "SIO";	optab[38][1] = "F0";
	optab[39][0] = "SSK";   optab[39][1] = "EC";
	optab[40][0] = "STA";   optab[40][1] = "0C";
	optab[41][0] = "STB";   optab[41][1] = "78";
	optab[42][0] = "STCH";  optab[42][1] = "54";
	optab[43][0] = "STF";   optab[43][1] = "80";
	optab[44][0] = "STI";   optab[44][1] = "D4";
	optab[45][0] = "STL";   optab[45][1] = "14";
	optab[46][0] = "STS";   optab[46][1] = "7C";
	optab[47][0] = "STSW";  optab[47][1] = "E8";
	optab[48][0] = "STT";   optab[48][1] = "84";
	optab[49][0] = "STX";	optab[49][1] = "10";
	optab[50][0] = "SUB";   optab[50][1] = "1C";
	optab[51][0] = "SUBF";  optab[51][1] = "5C";
	optab[52][0] = "SUBR";  optab[52][1] = "94";
	optab[53][0] = "SVC";   optab[53][1] = "B0";
	optab[54][0] = "TD";    optab[54][1] = "E0";
	optab[55][0] = "TIO";   optab[55][1] = "F8";
	optab[56][0] = "TIX";   optab[56][1] = "2C";
	optab[57][0] = "TIXR";  optab[57][1] = "B8";
	optab[58][0] = "WD";	optab[58][1] = "DC";
} 

/*This function take the appropriate action for each directive*/
int directive(string str)
{
	if (str == "BASE")
	{
		out.str("");
		out << (line_num+1);
		analysis[line_num][0] = out.str();

		analysis[line_num][1] = "-";
		analysis[line_num][2] = "-";
		analysis[line_num][3] = "BASE";
		analysis[line_num][4] = "-";
		//instruction >> word;
		analysis[line_num][5] = operand;//Displacement 
		analysis[line_num][6] = "-";
		
		return 1;
	}
	else
	{
		if(str == "BYTE")
		{
			out.str("");
			out << (line_num+1);
			analysis[line_num][0] = out.str();
			analysis[line_num][1] = locctr;

			
			symtab[label_num][0] = temp;
			symtab[label_num][1] = locctr;
			

			instruction >> word;
			int size = operand.size();
			int f = operand.find("C'");
											
			if(f == 0) 
				size -= 3;
			else 
				size = 1;

			locctr = C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + size)));
			analysis[line_num][2] = "-";
			analysis[line_num][3] = "BYTE";
			analysis[line_num][4] = "-";
			analysis[line_num][5] = "-";
			
			if(operand.at(0) == 'C')
			{
				int length = operand.length() - 3; int j = 2 ;
				string str = "";

				while (length > 0)
				{					
					str += C->asciiToHex(operand.substr(j,1));
					j++; length--;
				}
				analysis[line_num][6] = str;
			}
			else
			{
				if(operand.at(0) == 'X')
				{
					string str = "";
					str += operand.substr(2,1);
					str += operand.substr(3,1);
					analysis[line_num][6] = str;
				}
			} 

			symtab[label_num][2] = analysis[line_num][6];

			return 1;
		}
		else
		{
			if(str == "WORD")
			{
				out.str("");
				out << (line_num+1);
				analysis[line_num][0] = out.str();
				analysis[line_num][1] = locctr;

				
				symtab[label_num][0] = temp;
				symtab[label_num][1] = locctr;

				instruction >> word;
				
				locctr = C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 3)));
				analysis[line_num][2] = "-";
				analysis[line_num][3] = "WORD";
				analysis[line_num][4] = "-";
				analysis[line_num][5] = "-";
				
				int num;
				istringstream(operand) >> num;
				analysis[line_num][6] = C->binToHex(C -> decToBin(num));
				expand(analysis[line_num][6],6);

				symtab[label_num][2] = analysis[line_num][6];
			
				return 1;
			}
			else
			{
				if(str == "RESB")
				{
					out.str("");
					out << (line_num+1);
					analysis[line_num][0] = out.str();
					analysis[line_num][1] = locctr;
					
					symtab[label_num][0] = temp;
					symtab[label_num][1] = locctr;

					instruction >> word;
					int num;
					istringstream(operand) >> num;
					locctr = C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + num)));

					analysis[line_num][2] = "-";
					analysis[line_num][3] = "RESB";
					analysis[line_num][4] = "-";
					analysis[line_num][5] = "-";
					analysis[line_num][6] = "        ";

					return 1;
				}
				else
				{
					if(str == "RESW")
					{
						out.str("");
						out << (line_num+1);
						analysis[line_num][0] = out.str();
						analysis[line_num][1] = locctr;
						
						
						symtab[label_num][0] = temp;
						symtab[label_num][1] = locctr;

						instruction >> word;
						
						int num;
						istringstream(operand) >> num;
						locctr = C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + num * 3)));

						analysis[line_num][2] = "-";
						analysis[line_num][3] = "RESW";
						analysis[line_num][4] = "-";
						analysis[line_num][5] = "-";
						analysis[line_num][6] = "        ";

						return 1;
					}
					else
					{
						if(str == "*")
						{
							out.str("");
							out << (line_num+1);
							analysis[line_num][0] = out.str();
							analysis[line_num][1] = locctr;

							instruction >> word;

							symtab[label_num][0] = operand;
							symtab[label_num][1] = locctr;
							label_num++;

							int size;
							if(operand.at(0) == '=' && operand.at(1) == 'C')
							{
								size = operand.length() - 4;
							}
							else
							{
								size = 2;
							} 
							locctr = C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + size)));

							analysis[line_num][2] = "-";
							analysis[line_num][3] = "-";
							analysis[line_num][4] = "-";
							analysis[line_num][5] = "-";

							if(operand.at(0) == '=' && operand.at(1) == 'C')
							{
								int length = operand.length() - 4; int j = 3 ;
								string str = "";

								while (length > 0)
								{					
									str += C->asciiToHex(operand.substr(j,1));
									j++; length--;
								}
								analysis[line_num][6] = str;
							}
							else
							{
								if(operand.at(0) == '=' && operand.at(1) == 'X')
								{
									string str = "";
									str += C->asciiToHex(operand.substr(3,1));
									str += C->asciiToHex(operand.substr(4,1));
									analysis[line_num][6] = str;
								}
							} 

							return 1;
						}
						else
						{
							if( str == "EQU" )
							{
								out.str("");
								out << (line_num+1);
								analysis[line_num][0] = out.str();
								analysis[line_num][1] = locctr;

				
								symtab[label_num][0] = temp;
								symtab[label_num][1] = locctr;

								int expression = operand.find("-");
								if(expression >= 0)
								{
									int d = operand.length();
									int operand1 = C->hexToDec(search_symtab(operand.substr(0,expression)));
									int operand2 = C->hexToDec(search_symtab(operand.substr(expression+1))); 
									 
									int result = operand1 - operand2;

									symtab[label_num][2] = C->binToHex(C->decToBin(result));

								}
								else
								{
									symtab[label_num][2] = search_symtab(operand);
								} 

								label_num++;
				
								//locctr = C->Expantion(C->binToHex(C->decToBin(C->hexToDec(locctr) + 3)));
								analysis[line_num][2] = temp;
								analysis[line_num][3] = "-";
								analysis[line_num][4] = "-";
								analysis[line_num][5] = "-";
				
								int num;
								istringstream(operand) >> num;
								analysis[line_num][6] = C->binToHex(C -> decToBin(num));
								expand(analysis[line_num][6],6);
			
								return 1;
							}
							else
							{
								if( str == "END" )
								{
									out.str("");
									out << (line_num+1);
									analysis[line_num][0] = out.str();

									analysis[line_num][1] = "-";
									analysis[line_num][2] = "-";
									analysis[line_num][3] = "END";
									analysis[line_num][4] = "-";
									analysis[line_num][5] = "-";//Displacement 
									analysis[line_num][6] = "-";

									return 1;

								}
								else
								{
									if( str == "LTORG" )
									{
										out.str("");
										out << (line_num+1);
										analysis[line_num][0] = out.str();

										analysis[line_num][1] = "-";
										analysis[line_num][2] = "-";
										analysis[line_num][3] = "LTORG";
										analysis[line_num][4] = "-";
										analysis[line_num][5] = "-";//Displacement 
										analysis[line_num][6] = "-";

										return 1;
									} 
									else
									{
										if( str == "ORG" )
										{
											if(search_symtab(operand) == "-") //if the operand is a contant value
											{
												int num;
												istringstream(operand) >> num;
												locctr = C->binToHex(C->decToBin(num));
											}
											else 
											{
												locctr = search_symtab(operand);
											} 

											out.str("");
											out << (line_num+1);
											analysis[line_num][0] = out.str();

											analysis[line_num][1] = "-";
											analysis[line_num][2] = "-";
											analysis[line_num][3] = "ORG";
											analysis[line_num][4] = "-";
											analysis[line_num][5] = "-";//Displacement 
											analysis[line_num][6] = "-";
											return 1;
										} 
										else
										{
											return 0;
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

/*This function search inside the opcode table & return the correct opcode for each mnemonic*/
string search_optab(string str)
{
	for (int i = 0; i <=58 ; i++)
	{ 
		if(str.compare(optab[i][0]) == 0)
		{
			return optab[i][1];
		}
	} 
	return "-"; //if the returned value was - means str didn't match any entry in the OPTAB 
} 
 
/*This function is to expand a string for the specified length*/ 
void expand(string &str , int length)
{
	while(str.length() < length)
	{
		str = "0" + str;
	}
}

/*This function is to search inside the symtab and returns - if this isn't a lable*/
string search_symtab(string str)
{
	int i = 0;
	while(symtab[i][0] != "\0")
	{
		if(symtab[i][0] == str)
		{
			return symtab[i][1];
		}
		i++;
	}
	return "-";
}

/*This function is to generate the intermediate file*/
void generate_intermediateFile()
{
	ifstream file;
	ofstream output;
	output.open("intermediate_file.txt");
	file.open("input.txt");

	string line = "";

	if(!file.is_open())
	{
		cerr << "couldn't open file\n";
		exit(EXIT_FAILURE);	
	}
	else
	{
		if(!output.is_open())
		{
			cerr << "couldn't open file\n";
			exit(EXIT_FAILURE);	
		}
		else
		{
			int i = 0 ;
			while(analysis[i][0] != "\0")
			{
				getline(file,line);
				output << analysis[i][0] << "\t" << analysis[i][1] << "\t" << line << "\t\t" << analysis[i][6] << endl;
				i++;
			} 
		}
	}
}

/*This function is to generate the object file*/
void generate_ObjFile()
{
	int i = 0 , gapFlag = 0 , end = 0 , lFlag = 0;
	ofstream output;
	output.open("Object_file.txt");
	string programLength = C->binToHex(C->decToBin(C ->hexToDec(locctr) - C ->hexToDec(start)));
	string temp = "" , start_record , length_record;

	while(name.length() < 6)
	{
		name =name + " " ;
	}

	expand(start , 6);
	expand(programLength , 6);

	if(!output.is_open())
	{
		cerr << "couldn't open file\n";
		exit(EXIT_FAILURE);	
	}
	else
	{
		output << "H" << name << start << programLength << endl;

		while(analysis[i][0] != "\0")
		{
			if(analysis[i][6] == "-")
			{
				i++;
				continue;
			}
			else
			{
				if(temp == "")
					start_record = analysis[i][1];

				if((temp.length() + analysis[i][6].length()) <= 60)
				{
					if(analysis[i][6] == "        ")
					{
						gapFlag = 1;
						lFlag = 1;
						i++;
						continue;
					}
					else
					{
						if(gapFlag == 1)
						{
							end = 1;
						}

						if(end != 1)
						{
							temp += analysis[i][6];
							i++;
						}
						else
						{
							while(temp.length() != 60)
							{
								temp += " ";
							}
							
							end = 0; gapFlag = 0;
						} 
					}
				}
				else
				{
					if (lFlag == 0)
					{
						length_record = C->binToHex(C->decToBin(C->hexToDec(analysis[i][1]) - C->hexToDec(start_record)));
					}
					else
					{
						length_record = C->binToHex(C->decToBin(temp.length() / 2));
					} 

					expand(start_record , 6);
					expand(length_record , 2);
				
					output << "T" << start_record << length_record << temp << endl;
					temp = "";
				}
			} 
		}

		
		
		if (lFlag == 0)
		{
			length_record = C->binToHex(C->decToBin(C->hexToDec(analysis[i][1]) - C->hexToDec(start_record)));
		}
		else
		{
			length_record = C->binToHex(C->decToBin(temp.length() / 2));
		} 
		
		expand(start_record , 6);
		expand(length_record , 2);
				
		output << "T" << start_record << length_record << temp << endl;
		temp = "";

		output << "E" << start; 
	} 
}	

/*This function is to generate the relocatable object file*/
void generate_Relocateablefile()
{
	int i = 0 , gapFlag = 0 , end = 0 , lFlag = 0;
	ofstream output;
	output.open("relocateable_file.txt");
	string programLength = C->binToHex(C->decToBin(C ->hexToDec(locctr) - C ->hexToDec(start)));
	string temp = "" , start_record , length_record;
	string modification[20] ; int m = 0 , size = 0;

	while(name.length() < 6)
	{
		name =name + " " ;
	}

	expand(start , 6);
	expand(programLength , 6);

	if(!output.is_open())
	{
		cerr << "couldn't open file\n";
		exit(EXIT_FAILURE);	
	}
	else
	{
		output << "H" << name << start << programLength << endl;

		while(analysis[i][0] != "\0")//for each line in the analysis table
		{
			if(analysis[i][6] == "-") //if this is a directive 
			{
				i++;
				continue;
			}
			else
			{
				if(temp == "") //To start a new text record
					start_record = analysis[i][1];

				if((temp.length() + analysis[i][6].length()) <= 60)
				{
					if(analysis[i][6] == "        ") //for RESB & RESW directives 
					{
						gapFlag = 1;
						lFlag = 1;
						i++;
						continue;
					}
					else
					{
						if(gapFlag == 1)
						{
							end = 1;
						}

						if(end != 1)
						{
							if(analysis[i][6].length() == 8) // if this is a format 4 instruction
							{
								int location = size / 2 + (temp.length() + 2) / 2;
								string l = C->binToHex(C->decToBin(location));
								expand(l,6);
								modification[m] = "M" + l + "05";
								m++;
							} 
							temp += analysis[i][6];
							i++;
						}
						else
						{
							while(temp.length() != 60)
							{
								temp += " ";
							}
							
							end = 0; gapFlag = 0;
						} 
					}
				}
				else
				{
					if (lFlag == 0) //calculate the length of the record 
					{
						length_record = C->binToHex(C->decToBin(C->hexToDec(analysis[i][1]) - C->hexToDec(start_record)));
					}
					else
					{
						length_record = C->binToHex(C->decToBin(temp.length() / 2));
					} 

					expand(start_record , 6);
					expand(length_record , 2);
				
					output << "T" << start_record << length_record << temp << endl;
					temp = "";
				}
			} 
		}

				
		if (lFlag == 0)
		{
			length_record = C->binToHex(C->decToBin(C->hexToDec(analysis[i][1]) - C->hexToDec(start_record)));
		}
		else
		{
			length_record = C->binToHex(C->decToBin(temp.length() / 2));
		} 
		
		expand(start_record , 6);
		expand(length_record , 2);
				
		output << "T" << start_record << length_record << temp << endl;
		temp = "";

		int i = 0;
		while (modification[i] != "")
		{
			output << modification[i] << endl;
			i++;
		} 

		output << "E" << start; 
	}
} 

/*This function is to get the number of each register*/
string registerNum(string r)
{
	if(r == "A")
        return "0";

    else if (r == "X")
        return "1";
                                   
    else if (r == "L")                                   
        return "2";
                                   
    else if (r == "B")                                   
        return "3";
                                   
    else if (r == "S")
        return "4";

    else if (r == "T")
        return "5";

	else if (r == "F")
		return "6";

	else if (r == "PC")
		return "8";

	else if (r== "SW")
		return "9";
}
