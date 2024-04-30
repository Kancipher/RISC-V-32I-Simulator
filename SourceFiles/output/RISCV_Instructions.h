#pragma once
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
using namespace std;

class RISCV_Instructions
{
public:
	RISCV_Instructions();
	~RISCV_Instructions();
	void ADD(string rd,string rs1,string rs2);
	void SUB(string rd,string rs1,string rs2);
	void SLL(string rd,string rs1,string rs2);
	void SLT(string rd,string rs1,string rs2);
	void SLTU(string rd,string rs1,string rs2);
	void XOR(string rd,string rs1,string rs2);
	void SRL(string rd,string rs1,string rs2);
	void SRA(string rd,string rs1,string rs2);
	void OR(string rd,string rs1,string rs2);
	void AND(string rd,string rs1,string rs2);
	void ADDI(string rd,string rs1, int imm);
	void SLTI(string rd,string rs1, int imm);
	void SLTIU(string rd,string rs1, int imm);
	void XORI(string rd,string rs1, int imm);
	void ORI(string rd,string rs1, int imm);
	void ANDI(string rd,string rs1, int imm);
	void SLLI(string rd,string rs1, int imm);
	void SRLI(string rd,string rs1, int imm);
	void SRAI(string rd,string rs1, int imm);
	void BEQ(string rs1, string rs2,string label);
	void BNE(string rs1, string rs2,string label);
	void BGE(string rs1, string rs2,string label);
	void BLT(string rs1, string rs2,string label);
	void BLTU(string rs1, string rs2,string label);
	void BGEU(string rs1, string rs2,string label);
	void LB(string rd,string rs1,int imm);
	void LH(string rd,string rs1,int imm);
	void LBU(string rd,string rs1,int imm);
	void LW(string rd,string rs1,int imm);
	void LHU(string rd,string rs1,int imm);
	void SB(string rs1, string rs2, int imm);
	void SH(string rs1, string rs2, int imm);
	void SW(string rs1,string rs2,int imm);
	void JALR(string rd,string rs1,int imm);
	void JAL(string rd,string label);
	void LUI(string rd,int imm);
	void AUIPC(string rd,int imm);
	void RunProrgam(string filenameAssembly, string filenameData,int pc);
	void setProgramCounter(int pc);
private:
	map<std::string, int> registers; //registers
	map<std::string, uint8_t> opcodes; //opcodes
	map<string,int> labelMap; //labels
	struct Instruction { //struct for instructions
		string opcode;
		string name;
    	string rs1;
    	string rs2;
    	int imm;
    	string rd;
		string label;
		int pc;
	};
	void execute(vector<Instruction> &instr); //execute instructions
	void findLabels(string filename, map<string,int> &labelMap); //stores labels and their program counter in a map
	void processOpcode(map<std::string, uint8_t>& opcodes);  // process opcodes given by a txt file
	void parsingAssemblyCode(string filename, vector<Instruction> &instructions, map<string,int> &labelMap); //parsing assembly code
    int programCounter;
    vector<Instruction> instructions;
	void simulation();
	void setDatainMemory(string filename);
	
};
