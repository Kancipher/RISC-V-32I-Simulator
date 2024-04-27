#include <iostream>
#include "RISCV_Instructions.h"
#include <iomanip>
#include <algorithm>
#include <map>
#include <cassert>
#include <string>
#include <bitset>

const int two_8 = 255;
const int two_16 = 65535;
using namespace std;
void convert_to_xbase_register(std::string &reg)
{
    if (reg == "zero")
    {
        reg = "x0";
    }
    else if (reg == "ra")
    {
        reg = "x1";
    }
    else if (reg == "sp")
    {
        reg = "x2";
    }
    else if (reg == "gp")
    {
        reg = "x3";
    }
    else if (reg == "tp")
    {
        reg = "x4";
    }
    else if (reg == "t0")
    {
        reg = "x5";
    }
    else if (reg == "t1")
    {
        reg = "x6";
    }
    else if (reg == "t2")
    {
        reg = "x7";
    }
    else if (reg == "t3")
    {
        reg = "x28";
    }
    else if (reg == "t4")
    {
        reg = "x29";
    }
    else if (reg == "t5")
    {
        reg = "x30";
    }
    else if (reg == "t6")
    {
        reg = "x31";
    }
    else if (reg == "s0" || reg == "fp")
    {
        reg = "x8";
    }
    else if (reg == "s1")
    {
        reg = "x9";
    }
    else if (reg == "s2")
    {
        reg = "x18";
    }
    else if (reg == "s3")
    {
        reg = "x19";
    }
    else if (reg == "s4")
    {
        reg = "x20";
    }
    else if (reg == "s5")
    {
        reg = "x21";
    }
    else if (reg == "s6")
    {
        reg = "x22";
    }
    else if (reg == "s7")
    {
        reg = "x23";
    }
    else if (reg == "s8")
    {
        reg = "x24";
    }
    else if (reg == "s9")
    {
        reg = "x25";
    }
    else if (reg == "s10")
    {
        reg = "x26";
    }
    else if (reg == "s11")
    {
        reg = "x27";
    }
    else if (reg == "a0")
    {
        reg = "x10";
    }
    else if (reg == "a1")
    {
        reg = "x11";
    }
    else if (reg == "a2")
    {
        reg = "x12";
    }
    else if (reg == "a3")
    {
        reg = "x13";
    }
    else if (reg == "a4")
    {
        reg = "x14";
    }
    else if (reg == "a5")
    {
        reg = "x15";
    }
    else if (reg == "a6")
    {
        reg = "x16";
    }
    else if (reg == "a7")
    {
        reg = "x17";
    }
}

string convert_to_binary(std::string &rs)
{
    std::string substring = rs.substr(1);
    int num = std::stoi(substring);
    std::bitset<5> binaryNum(num);

    return binaryNum.to_string();
}

string removeCommas(const string &input)
{
    string result = input;
    result.erase(remove(result.begin(), result.end(), ','), result.end());
    return result;
}

// Function to remove brackets from a string
string removeBrackets(const string &input)
{
    string result = input;
    result.erase(remove(result.begin(), result.end(), '('), result.end());
    result.erase(remove(result.begin(), result.end(), ')'), result.end());
    return result;
}
RISCV_Instructions::RISCV_Instructions()
{
    programCounter = 0;
}

RISCV_Instructions::~RISCV_Instructions()
{
}

void RISCV_Instructions::setProgramCounter(int pc)
{
    programCounter = pc;
}

constexpr uint64_t MAX_MEMORY_ADDRESS = 0xFFFFFFFF; // 4GB in bytes
map<unsigned int, int> memory;

void RISCV_Instructions::setDatainMemory(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    // Read the file line by line
    string line;
    while (getline(file, line))
    {
        // Parse the line
        istringstream iss(line);
        unsigned int key;
        int value;
        if (iss >> key >> value)
        {
            // Insert the values into the map
            memory[key] = value;
        }
        else
        {
            cerr << "Error parsing line: " << line << endl;
        }
    }

    // Close the file
    file.close();
}

void RISCV_Instructions::LB(std::string rd, std::string rs1, int imm)
{
    if (rd == "x0")
        return;

    if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
    {
        int k = registers[rs1];
        int r = (registers[rs1] + imm) % 4;
        int address = registers[rs1] + imm - r;

        // Check if the address is already in memory, if not, initialize it
        if (memory.find(address) == memory.end())
        {
            std::cerr << "Error: Memory access out of bounds - LB" << std::endl;
            exit(100);
        }
        else
            registers[rd] = (memory[address] << (8 * (3 - r)));
    }
    else
    {
        cout << "Immediate can't be more than 12 bit - LB" << endl;
        exit(1001);
    }
    programCounter += 4;
};
void RISCV_Instructions::LH(std::string rd, std::string rs1, int imm)
{
    if (rd == "x0")
        return;
    if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
    {
        int r = (registers[rs1] + imm) % 4;
        int address = registers[rs1] + imm - r;

        if (memory.find(address) == memory.end())
        {
            std::cerr << "Error: Memory access out of bounds - LH" << std::endl;
            exit(101);
        }
        else
            registers[rd] = (memory[address] << (8 * (2 - r))) >> 16;
    }
    else
    {
        cout << "Immediate can't be more than 12 bit - LH" << endl;
        exit(1002);
    }
    programCounter += 4;
};
void RISCV_Instructions::LW(std::string rd, std::string rs1, int imm)
{
    if (rd == "x0")
        return;
    if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
    {
        int address = registers[rs1] + imm;
        // Check if the address is within the valid memory range
        if (memory.find(address) == memory.end())
        {
            std::cerr << "Error: Memory access out of bounds -LW " << std::endl;
            exit(102);
        }
        else
            registers[rd] = memory[address];
    }
    else
    {
        cout << "Immediate can't be more than 12 bit - LW" << endl;
        exit(1003);
    }
    programCounter += 4;
}
void RISCV_Instructions::LBU(std::string rd, std::string rs1, int imm)
{
    if (rd == "x0")
        return;
    if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
    {
        int r = (registers[rs1] + imm) % 4;
        int address = registers[rs1] + imm - r;
        if (memory.find(address) == memory.end())
        {
            std::cerr << "Error: Memory access out of bounds - LBU" << std::endl;
            exit(103);
        }
        else
            registers[rd] = (memory[address] << (8 * (3 - r))) >> 24;
    }
    else
    {
        cout << "Immediate can't be more than 12 bit - LBU" << endl;
        exit(1004);
    }
    programCounter += 4;
};

void RISCV_Instructions::LHU(std::string rd, std::string rs1, int imm)
{
    if (rd == "x0")
        return;
    if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
    {
        int r = (registers[rs1] + imm) % 4;
        int address = registers[rs1] + imm - r;

        if (memory.find(address) == memory.end())
        {
            std::cerr << "Error: Memory access out of bounds - LHU" << std::endl;
            exit(104);
        }
        registers[rd] = (unsigned int)(memory[address] << (8 * (2 - r))) >> 16;
    }
    else
    {
        cout << "Immediate can't be more than 12 bit - LHU" << endl;
        exit(1005);
    }
    programCounter += 4;
};
void RISCV_Instructions::SB(std::string rs1, std::string rs2, int imm)
{
    int address = registers[rs1] + imm;
    int r = address % 4;
    int final_address = address - r;

    // if (memory.find(final_address) == memory.end())
    // {
    //     std::cerr << "Error: Memory access out of bounds - SB" << std::endl;
    //     exit(105);
    // }

    unsigned int byte = registers[rs2] & 0xFF; // Extract the least significant byte

    // Shift the byte to the correct position based on the remainder
    byte <<= (8 * r);

    // Mask out the existing byte at the memory location
    unsigned int mask = ~(0xFF << (8 * r));
    memory[final_address] = (memory[final_address] & mask) | byte;
    programCounter += 4;
};
void RISCV_Instructions::SH(std::string rs1, std::string rs2, int imm)
{
    int address = registers[rs1] + imm;
    int r = address % 4;
    int final_address = address - r;

    // if (memory.find(final_address) == memory.end())
    // {
    //     std::cerr << "Error: Memory access out of bounds - SH" << std::endl;
    //     exit(106);
    // }

    unsigned int half = registers[rs2] & 0xFFFF; // Extract the least significant halfword

    // Shift the halfword to the correct position based on the remainder
    half <<= (8 * r);

    // Mask out the existing halfword at the memory location
    unsigned int mask = ~(0xFFFF << (8 * r));
    memory[final_address] = (memory[final_address] & mask) | half;
    programCounter += 4;
};

void RISCV_Instructions::SW(std::string rs1, std::string rs2, int imm)
{
    int final_address = registers[rs1] + imm;
    // if (memory.find(final_address) == memory.end())
    // {
    //     std::cerr << "Error: Memory access out of bounds - SW" << std::endl;
    //     exit(107);
    // }
    memory[final_address] = registers[rs2];
    programCounter += 4;
};

void RISCV_Instructions::LUI(string rd, int imm)
{
    if (rd == "x0")
        return;
    registers[rd] = (imm << 12);
    programCounter += 4;
}

void RISCV_Instructions::ADD(string rd, string rs1, string rs2)
{
    if (rd != "X0")
        registers[rd] = registers[rs1] + registers[rs2];
    else
    {
        cout << "Can't add to x0" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::ADDI(string rd, string rs1, int imm)
{
    if (rd != "x0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
            registers[rd] = registers[rs1] + imm;
        else
        {
            cout << "The Immediate is more than 12 bit - ADDI" << endl;
            exit(1000);
        }
    }
    else
    {
        cout << "Can't add to x0" << endl;
        exit(1);
    }
    programCounter += 4;
}

void RISCV_Instructions::AND(string rd, string rs1, string rs2)
{
    if (rd != "X0")
        registers[rd] = registers[rs1] & registers[rs2];
    else
    {
        cout << "Can't and to x0" << endl;
        exit(2);
    }
    programCounter += 4;
}

void RISCV_Instructions::ANDI(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
            registers[rd] = registers[rs1] & imm;
        else
        {
            cout << "The Immediate is more than 12 bit - ANDI" << endl;
            exit(1006);
        }
    }
    else
    {
        cout << "Can't and to x0" << endl;
        exit(2);
    }
    programCounter += 4;
}

void RISCV_Instructions::SUB(string rd, string rs1, string rs2)
{
    if (rd != "X0")
        registers[rd] = registers[rs1] - registers[rs2];
    else
    {
        cout << "Can't change x0 - SUB" << endl;
        exit(0);
    }
    programCounter += 4;
}
void RISCV_Instructions::SLL(string rd, string rs1, string rs2)
{
    if (rd != "X0")
        registers[rd] = registers[rs1] << registers[rs2];
    else
    {
        cout << "Can't change x0 - SLL" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::SRL(string rd, string rs1, string rs2)
{
    if (rd != "X0")
        registers[rd] = registers[rs1] >> registers[rs2];
    else
    {
        cout << "Can't change x0 - SLL" << endl;
        exit(0);
    }
    programCounter += 4;
}
void RISCV_Instructions::SLT(string rd, string rs1, string rs2)
{
    if (rd != "X0")
    {
        if (registers[rs1] < registers[rs2])
            registers[rd] = 1;
        else
            registers[rd] = 0;
    }
    else
    {
        cout << "Can't change x0 - SLT" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::SLTU(string rd, string rs1, string rs2)
{
    if (rd != "X0")
    {
        if ((unsigned int)registers[rs1] < (unsigned int)registers[rs2])
            registers[rd] = 1;
        else
            registers[rd] = 0;
    }
    else
    {
        cout << "Can't change x0 - SLTU" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::XOR(string rd, string rs1, string rs2)
{
    if (rd != "X0")
    {
        registers[rd] = registers[rs1] ^ registers[rs2];
    }
    else
    {
        cout << "Can't change x0 - SLT" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::SRA(string rd, string rs1, string rs2)
{
    if (rd != "X0")
        registers[rd] = registers[rs1] << registers[rs2];
    else
    {
        cout << "Can't change x0 - SRA" << endl;
        exit(0);
    }
    programCounter += 4;
}

// void RISCV_Instructions::SRL(string rd, string rs1, string rs2)
//{
//  if (rd != "X0")
//   registers[rd] = registers[rs1] << registers[rs2];
// else
//{
// cout << "Can't change x0 - SRL" << endl;
//   exit(0);
// }
//  programCounter += 4;
//}

void RISCV_Instructions::OR(string rd, string rs1, string rs2)
{
    if (rd != "X0")
        registers[rd] = registers[rs1] | registers[rs2];
    else
    {
        cout << "Can't change x0 - OR" << endl;
        exit(0);
    }
    programCounter += 4;
}
void RISCV_Instructions::SLTI(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
        {
            if (registers[rs1] < imm)
                registers[rd] = 1;
            else
                registers[rd] = 0;
        }
        else
        {
            cout << "The Immediate is more than 12 bit - SLTI" << endl;
            exit(1007);
        }
    }
    else
    {
        cout << "Can't change x0 - SLTI" << endl;
        exit(0);
    }
    programCounter += 4;
}
void RISCV_Instructions::SLTIU(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
        {
            if ((unsigned int)registers[rs1] < imm)
                registers[rd] = 1;
            else
                registers[rd] = 0;
        }
        else
        {
            cout << "The Immediate is more than 12 bit - SLTIU" << endl;
            exit(1008);
        }
    }
    else
    {
        cout << "Can't change x0 - SLTIU" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::XORI(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
        {
            registers[rd] = registers[rs1] ^ imm;
        }
        else
        {
            cout << "The Immediate is more than 12 bit - XORI" << endl;
            exit(1009);
        }
    }
    else
    {
        cout << "Can't change x0 - XORI" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::ORI(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
        {
            registers[rd] = registers[rs1] | imm;
        }
        else
        {
            cout << "The Immediate is more than 12 bit - ORI" << endl;
            exit(1010);
        }
    }
    else
    {
        cout << "Can't change x0 - ORI" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::SLLI(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
            registers[rd] = registers[rs1] << imm;
        else
        {
            cout << "The Immediate is more than 12 bit - SLLI" << endl;
            exit(1011);
        }
    }
    else
    {
        cout << "Can't change x0 - SLLI" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::SRLI(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
            registers[rd] = registers[rs1] >> imm;
        else
        {
            cout << "The Immediate is more than 12 bit - SRLI" << endl;
            exit(1012);
        }
    }
    else
    {
        cout << "Can't change x0 - SRLI" << endl;
        exit(0);
    }
    programCounter += 4;
}

void RISCV_Instructions::SRAI(string rd, string rs1, int imm)
{
    if (rd != "X0")
    {
        if (!(imm > (1 << 11) - 1 || imm < -1 * (1 << 11)))
            registers[rd] = registers[rs1] >> imm;

        else
        {
            cout << "The Immediate is more than 12 bit - SRAI" << endl;
            exit(1013);
        }
    }
    else
    {
        cout << "Can't change x0 - SRAI" << endl;
        exit(0);
    }
    programCounter += 4;
}
void RISCV_Instructions::BEQ(string rs1, string rs2, string label)
{
    if (!labelMap.count(label))
    {
        cerr << "Trying to jump to a non existing label- BEQ " << endl;
        exit(200);
    }
    if (registers[rs1] == registers[rs2])
        programCounter = labelMap[label];
    else
        programCounter += 4;
}

void RISCV_Instructions::BGE(string rs1, string rs2, string label)
{
    if (!labelMap.count(label))
    {
        cerr << "Trying to jump to a non existing label - BGE " << endl;
        exit(201);
    }
    if (registers[rs1] >= registers[rs2])
        programCounter = labelMap[label];
    else
        programCounter += 4;
}

void RISCV_Instructions::BNE(string rs1, string rs2, string label)
{
    if (!labelMap.count(label))
    {
        cerr << "Trying to jump to a non existing label- BGE " << endl;
        exit(201);
    }
    if (registers[rs1] != registers[rs2])
        programCounter = labelMap[label];
    else
        programCounter += 4;
}
void RISCV_Instructions::BLT(string rs1, string rs2, string label)
{
    if (!labelMap.count(label))
    {
        cerr << "Trying to jump to a non existing label- BLT" << endl;
        exit(202);
    }
    if (registers[rs1] < registers[rs2])
        programCounter = labelMap[label];
    else
        programCounter += 4;
}
void RISCV_Instructions::BGEU(string rs1, string rs2, string label)
{
    if (!labelMap.count(label))
    {
        cerr << "Trying to jump to a non existing label - BGEU" << endl;
        exit(202);
    }
    if ((unsigned int)(registers[rs1]) >= (unsigned int)(registers[rs2]))
        programCounter = labelMap[label];
    else
        programCounter += 4;
}

void RISCV_Instructions::BLTU(string rs1, string rs2, string label)
{
    if (!labelMap.count(label))
    {
        cerr << "Trying to jump to a non existing label- BGEU" << endl;
        exit(203);
    }
    if ((unsigned int)(registers[rs1]) < (unsigned int)(registers[rs2]))
        programCounter = labelMap[label];
    else
        programCounter += 4;
}

void RISCV_Instructions::JALR(string rd, string rs1, int imm) // Still need to check the restrictions on changing X0
{
    registers[rd] = programCounter + 4; // We removed four from here
    programCounter = registers[rs1] + imm;
    if (rd == "x0")
        registers[rd] = 0;
}

void RISCV_Instructions::JAL(string rd, string label)
{
    if (!labelMap.count(label))
    {
        cerr << "Trying to jump to a non existing label- jal" << endl;
        exit(205);
    }
    if (rd != "x0")
    {
        registers[rd] = programCounter + 4;
        programCounter = labelMap[label];
    }
    else
    {
        cout << "Can't Change X0 - JAL " << endl;
        exit(210);
    }
}

void RISCV_Instructions::AUIPC(string rd, int imm)
{
    if (rd != "X0")
    {
        registers[rd] = programCounter + (imm << 12);
        programCounter += 4;
    }
    else
    {
        cout << "Can't Change X0 - AUIPC" << endl;
        exit(211);
    }
}
void RISCV_Instructions::processOpcode(map<std::string, uint8_t> &opcodes)
{
    string filename = "../TestCases/Opcodes.txt";
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string instruction;
        int opcode;
        if (getline(iss, instruction, ',') && (iss >> opcode))
        {
            opcodes[instruction] = opcode;
        }
        else
        {
            cerr << "Invalid line: " << line << endl;
        }
    }

    file.close();
}

void RISCV_Instructions::findLabels(string filename, map<string, int> &labelMap)
{
    int pc = programCounter;
    int i = 0;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        // Trim leading and trailing whitespaces from the line
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        if (!line.empty())
        {

            // Check if the line contains a label
            if (line.find(':') != string::npos)
            {
                line.erase(remove(line.begin(), line.end(), ' '), line.end());
                int position = -1;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] == ':')
                    {
                        position = i;
                    }
                }
                // if (line.at(position + 1) == '\n' || line.at(position + 1) == '\t')
                // {
                // cerr << "Cannot write Insturction after Label, please rewrite the syntex error" << endl;
                // exit(51);
                //}
                // Extract label and its memory address
                istringstream lineStream(line);
                string label;
                getline(lineStream, label, ':');
                if (isdigit(label[0]))
                {
                    cerr << "First character of label cannot be a digit " << label << endl;
                    return;
                }
                // Store label and its memory address
                labelMap[label] = pc - (4 * i);
                i++;
            }
        }
        pc += 4;
    }
    file.close();
}
void RISCV_Instructions::execute(vector<Instruction> &instruction)
{
    int programCounterNew = instruction[instruction.size() - 1].pc;
    int x = 0;
    bool CheckBreak = false;
    while ((programCounter <= programCounterNew) && programCounter > 0) // Max program counter
    {
        // cout << "kjsdfhksdhfkjs   " << programCounter << endl;
        for (auto &instr : instruction)
        {
            if (programCounter == instr.pc)
            {
                if (instr.name == "add")
                    ADD(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "sub")
                    SUB(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "sll")
                    SLL(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "slt")
                    SLT(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "sltu")
                    SLTU(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "xor")
                    XOR(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "srl")
                    SRL(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "sra")
                    SRA(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "or")
                    OR(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "and")
                    AND(instr.rd, instr.rs1, instr.rs2);
                else if (instr.name == "addi")
                    ADDI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "slti")
                    SLTI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "sltiu")
                    SLTIU(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "xori")
                    XORI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "ori")
                    ORI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "andi")
                    ANDI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "slli")
                    SLLI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "srli")
                    SRLI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "srai")
                    SRAI(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "sw")
                    SW(instr.rs1, instr.rs2, instr.imm);
                else if (instr.name == "sb")
                    SB(instr.rs1, instr.rs2, instr.imm);
                else if (instr.name == "sh")
                    SH(instr.rs1, instr.rs2, instr.imm);
                else if (instr.name == "lw")
                    LW(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "lb")
                    LB(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "lbu")
                    LBU(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "lh")
                    LH(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "lhu")
                    LHU(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "beq")
                    BEQ(instr.rs1, instr.rs2, instr.label);
                else if (instr.name == "blt")
                    BLT(instr.rs1, instr.rs2, instr.label);
                else if (instr.name == "bne")
                    BNE(instr.rs1, instr.rs2, instr.label);
                else if (instr.name == "bge")
                    BGE(instr.rs1, instr.rs2, instr.label);
                else if (instr.name == "bltu")
                    BLTU(instr.rs1, instr.rs2, instr.label);
                else if (instr.name == "bgeu")
                    BGEU(instr.rs1, instr.rs2, instr.label);
                else if (instr.name == "jalr")
                    JALR(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "jal")
                    JAL(instr.rd, instr.label);
                else if (instr.name == "auipc")
                    AUIPC(instr.rd, instr.imm);
                else if (instr.name == "jalr")
                    JALR(instr.rd, instr.rs1, instr.imm);
                else if (instr.name == "lui")
                    LUI(instr.rd, instr.imm);
                else if (instr.name == "ebreak" || instr.name == "ecall" || instr.name == "fence")
                    CheckBreak = true;
            }
            // cout << "Program Counter: " << programCounter << endl;
        }
        if (CheckBreak)
            break;
    }
}
void RISCV_Instructions::parsingAssemblyCode(string filename, vector<Instruction> &instructions, map<string, int> &labelMap)
{
    processOpcode(opcodes);
    findLabels(filename, labelMap);
    int programCounterInitial = programCounter;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        // Trim leading and trailing whitespaces from the line
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        if (!line.empty())
        {

            Instruction instr;
            // No label, just store the instruction and update memory address
            cout << "Instruction: " << line << endl;
            istringstream iss(line);
            string instructionName;
            if (iss >> instructionName)
            {
                if (line.find(':') != string::npos)
                {
                    continue;
                }
                else
                {
                    // Converting the instruction name into lowercase to avoid any error while reading from file
                    transform(instructionName.begin(), instructionName.end(), instructionName.begin(), [](unsigned char c)
                              { return tolower(c); });
                    cout << "Instruction Name: " << instructionName << " Program Counter: " << programCounter << endl;
                    instr.name = instructionName;
                    int opcode = opcodes[instructionName];
                    switch (opcode)
                    {
                    case 0x33: // R-type instructions
                        // Parse the operands for R-type instructions
                        // Example: "add x10, x11, x12"
                        if (iss >> instr.rd && iss.ignore() && iss >> instr.rs1 && iss.ignore() && iss >> instr.rs2)
                        {
                            // Set the immediate field to 0 for R-type instructions
                            instr.imm = 0;
                            // Remove commas from the operands
                            instr.rd = removeCommas(instr.rd);
                            instr.rs1 = removeCommas(instr.rs1);
                            instr.rs2 = removeCommas(instr.rs2);

                            // Converting all registers used to X based
                            // Example a0 will be mapped to x0
                            convert_to_xbase_register(instr.rd);
                            convert_to_xbase_register(instr.rs1);
                            convert_to_xbase_register(instr.rs2);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid R-type instruction line: " << line << endl;
                        }
                        break;
                        // Add cases for other instruction types
                    case 0x13: // I-type instructions
                        // Parse the operands for I-type instructions
                        // Example: "addi x10, x11, 10"
                        if (iss >> instr.rd && iss.ignore() && iss >> instr.rs1 && iss.ignore() && iss >> instr.imm)
                        {
                            // Set the rs2 field to 0 for I-type instructions
                            instr.rs2 = "";
                            // Remove commas from the operands
                            instr.rd = removeCommas(instr.rd);
                            instr.rs1 = removeCommas(instr.rs1);

                            convert_to_xbase_register(instr.rd);
                            convert_to_xbase_register(instr.rs1);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid I-type instruction line: " << line << endl;
                        }
                        break;
                    case 0x23: // S-type instructions
                        // Parse the operands for S-type instructions
                        // Example: "sw x10, 10(x11)"
                        char comma;
                        int offset;
                        if (iss >> instr.rs2 >> offset >> comma >> instr.rs1)
                        {
                            // Extract the immediate value from the offset
                            instr.imm = offset;
                            // Set the rd field to 0 for S-type instructions
                            instr.rd = "";
                            // Remove commas from the operands
                            instr.rs2 = removeCommas(instr.rs2);
                            instr.rs1 = removeCommas(instr.rs1);
                            // Remove brackets from the operands
                            instr.rs2 = removeBrackets(instr.rs2);
                            instr.rs1 = removeBrackets(instr.rs1);

                            convert_to_xbase_register(instr.rs2);
                            convert_to_xbase_register(instr.rs1);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid S-type instruction line: " << line << endl;
                        }
                        break;
                    case 0x3: // Load Instructions
                              // Parse the operands for Load instructions
                              // Example: "lw x10, 10(x11)"
                        if (iss >> instr.rd >> offset >> comma >> instr.rs1)
                        {
                            // Extract the immediate value from the offset
                            instr.imm = offset;
                            // Set the rd field to 0 for S-type instructions
                            instr.rs2 = "";
                            // Remove commas from the operands
                            instr.rd = removeCommas(instr.rd);
                            instr.rs1 = removeCommas(instr.rs1);
                            // Remove brackets from the operands
                            instr.rd = removeBrackets(instr.rd);
                            instr.rs1 = removeBrackets(instr.rs1);

                            convert_to_xbase_register(instr.rs1);
                            convert_to_xbase_register(instr.rd);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid Load instruction line: " << line << endl;
                        }
                        break;
                    case 0x63: // Branch Instructions
                        // Parse the operands for Branch instructions
                        // Example: "beq x10, x11, label"
                        if (iss >> instr.rs1 >> instr.rs2 >> instr.label)
                        {
                            // Set the rd field to 0 for Branch instructions
                            instr.rd = "";
                            // Remove commas from the operands
                            instr.rs1 = removeCommas(instr.rs1);
                            instr.rs2 = removeCommas(instr.rs2);

                            convert_to_xbase_register(instr.rs2);
                            convert_to_xbase_register(instr.rs1);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid Branch instruction line: " << line << endl;
                        }
                        break;
                    case 0x67: // JALR instruction
                        // Parse the operands for jalr instruction
                        // Example: "jalr rd, offset(rs1)"
                        if (iss >> instr.rd >> offset >> comma >> instr.rs1)
                        {
                            // Extract the immediate value from the offset
                            instr.imm = offset;
                            // Set the rd field to 0 for S-type instructions
                            instr.rs2 = "";
                            // Remove commas from the operands
                            instr.rd = removeCommas(instr.rd);
                            instr.rs1 = removeCommas(instr.rs1);
                            // Remove brackets from the operands
                            instr.rd = removeBrackets(instr.rd);
                            instr.rs1 = removeBrackets(instr.rs1);

                            convert_to_xbase_register(instr.rd);
                            convert_to_xbase_register(instr.rs1);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid jalr instruction line: " << line << endl;
                        }
                        break;
                    case 0x6F: // JAL instruction
                        // Parse the operands for jal instruction
                        // Example: "jal x10, label"
                        if (iss >> instr.rd >> instr.label)
                        {
                            // Remove commas from the operands
                            instr.rd = removeCommas(instr.rd);
                            convert_to_xbase_register(instr.rd);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid jal instruction line: " << line << endl;
                        }
                        break;
                    case 0x17: // AUIPC instruction
                        // Parse the operands for auipc instruction
                        // Example: "auipc x10, 100"
                        if (iss >> instr.rd >> instr.imm)
                        {
                            // Remove commas from the operands
                            instr.rd = removeCommas(instr.rd);

                            convert_to_xbase_register(instr.rd);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid auipc instruction line: " << line << endl;
                        }
                        break;
                    case 0x37: // LUI instruction
                        // Parse the operands for lui instruction
                        // Example: "lui x10, 100"
                        if (iss >> instr.rd >> instr.imm)
                        {
                            // Remove commas from the operands
                            instr.rd = removeCommas(instr.rd);

                            convert_to_xbase_register(instr.rd);
                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invalid lui instruction line: " << line << endl;
                        }
                        break;
                    case 0x73: // Ecall and Ebreak instructions
                        instr.rd = "";
                        instr.rs1 = "";
                        instr.rs2 = "";
                        instr.pc = programCounter;
                        break;
                    case 0xF:
                        if (iss >> instr.rd >> instr.rs1)
                        {
                            instr.rd = removeCommas(instr.rd);
                            instr.rs1 = removeCommas(instr.rs1);

                            convert_to_xbase_register(instr.rd);
                            convert_to_xbase_register(instr.rs1);

                            instr.pc = programCounter;
                        }
                        else
                        {
                            cerr << "Invaled Fence instruction line " << line << endl;
                        }
                        break;
                    default:
                        cerr << "Wrong instruction, Unsupported opcode: " << opcode << endl;
                        break;
                    }
                }
            }
            else
            {
                cerr << "Invalid instruction line: " << line << endl;
            }

            programCounter += 4; // Assuming each instruction takes 4 bytes in memory
            instructions.push_back(instr);
        }
    }
    programCounter = programCounterInitial;
    file.close();
    execute(instructions);
}
void RISCV_Instructions::simulation()
{
    std::cout << "Register"
              << "\t"
              << "Decimal"
              << "\t"
              << "Hexadecimal"
              << "\t"
              << "Binary" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;

    for (auto it = registers.begin(); it != registers.end(); ++it)
    {
        std::cout << it->first << "\t   | \t" << it->second << "\t|" << std::hex << it->second << "\t\t| " << std::bitset<32>(it->second) << std::dec << std::endl;
    }
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Memory Address"
              << "\t"
              << "Decimal"
              << "\t"
              << "Hexadecimal"
              << "\t"
              << "Binary" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    for (auto it = memory.begin(); it != memory.end(); ++it)
    {
        std::cout << it->first << "\t   | \t" << it->second << "\t|" << "0x"<<std::hex << it->second << "\t\t| " << std::bitset<32>(it->second) << std::dec << std::endl;
    }
}
void RISCV_Instructions::RunProrgam(string filenameAssembly, string filenameData,int programcounter)
{ // Set the data memory
    setDatainMemory(filenameData);
    setProgramCounter(programcounter);
    parsingAssemblyCode(filenameAssembly, instructions, labelMap);

    // Perform simulation
    simulation();
}
/*
int main(int argc, char *argv[]) {
    system("gui.py");
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file.txt>" << std::endl;
        return 1;
    }


    std::string filenameAssembly = argv[1];
    std::string filenameData = argv[2];
    RISCV_Instructions riscv;
    riscv.RunProrgam(filenameAssembly,filenameData);

}
*/
// int main()
// {
//     RISCV_Instructions riscv;
//     riscv.RunProrgam("TestCases/sumarraycode.txt", "TestCases/sumarraymem.txt");
//     return 0;
// }

int main()
{

    std::string input1, input2, input3;
    std::getline(std::cin, input1); 
    std::getline(std::cin, input2);
    std::getline(std::cin, input3);
    int pc=stoi(input3);
    RISCV_Instructions riscv;
    riscv.RunProrgam(input1,input2,pc);
}