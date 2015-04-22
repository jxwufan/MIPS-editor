#include "disassembler.h"
#include <QString>
#include <QDebug>

Disassembler::Disassembler()
{
    type1.clear();
    type1["ADD"]	 = 0x20;
    type1["ADDU"]	 = 0x21;
    type1["AND"]	 = 0x24;
    type1["NOR"]	 = 0x27;
    type1["OR"]		 = 0x25;
    type1["SUB"]	 = 0x22;
    type1["SUBU"]	 = 0x23;
    type1["XOR"]	 = 0x26;
    type1["SLT"]	 = 0x2a;
    type1["SLTU"]	 = 0x2b;

    type2.clear();
    type2["LW"]		= 0x23;
    type2["LB"]		= 0x20;
    type2["LBU"]	= 0x24;
    type2["LH"]		= 0x21;
    type2["LHU"]	= 0x25;
    type2["SW"]		= 0x2b;
    type2["SB"]		= 0x28;
    type2["SH"]		= 0x29;

    type3.clear();
    type3["ADDI"]	=	8;
    type3["ADDIU"]	=	9;
    type3["ANDI"]	=	0xc;
    type3["ORI"]	=	0xd;
    type3["XORI"]	=	0xe;
    type3["SLTI"]	=	0xa;
    type3["SLTIU"]	=	0xb;

    type4.clear();
    type4["SLL"]	=	0;
    type4["SRL"]	=	2;
    type4["SRA"]	=	3;

    type5.clear();
    type5["MULT"]	=	0x18;
    type5["MULTU"]	=	0x19;
    type5["DIV"]	=	0x1a;
    type5["DIVU"]	=	0x1b;

    registerTable.clear();
    registerTable[0]	=	"$zero";
    registerTable[1]	=	"$at";
    registerTable[2]	=	"$v0";
    registerTable[3]	=	"$v1";
    registerTable[4]	=	"$a0";
    registerTable[5]	=	"$a1";
    registerTable[6]	=	"$a2";
    registerTable[7]	=	"$a3";
    registerTable[8]	=	"$t0";
    registerTable[9]	=	"$t1";
    registerTable[10]	=	"$t2";
    registerTable[11]	=	"$t3";
    registerTable[12]	=	"$t4";
    registerTable[13]	=	"$t5";
    registerTable[14]	=	"$t6";
    registerTable[15]	=	"$t7";
    registerTable[16]	=	"$s0";
    registerTable[17]	=	"$s1";
    registerTable[18]	=	"$s2";
    registerTable[19]	=	"$s3";
    registerTable[20]	=	"$s4";
    registerTable[21]	=	"$s5";
    registerTable[22]	=	"$s6";
    registerTable[23]	=	"$s7";
    registerTable[24]	=	"$t8";
    registerTable[25]	=	"$t9";
    registerTable[26]	=	"$k0";
    registerTable[27]	=	"$k1";
    registerTable[28]	=	"$gp";
    registerTable[29]	=	"$sp";
    registerTable[30]	=	"$fp";
    registerTable[31]	=	"$ra";


    opTable.clear();

    opTable[0x2	] = "j";
    opTable[0x3	] = "jal";
    opTable[0x4	] = "beq";
    opTable[0x5	] = "bne";
    opTable[0x6	] = "blez";
    opTable[0x7	] = "bgtz";
    opTable[0x8	] = "addi";
    opTable[0x9	] = "addiu";
    opTable[0xa	] = "slti";
    opTable[0xb	] = "sltiu";
    opTable[0xc	] = "andi";
    opTable[0xd	] = "ori";
    opTable[0xe	] = "xori";
    opTable[0xf	] = "lui";
    opTable[0x20] = "lb";
    opTable[0x21] = "lh";
    opTable[0x22] = "lwl";
    opTable[0x23] = "lw";
    opTable[0x24] = "lbu";
    opTable[0x25] = "lhu";
    opTable[0x26] = "lwr";
    opTable[0x28] = "sb";
    opTable[0x29] = "sh";
    opTable[0x2a] = "swl";
    opTable[0x2b] = "sw";

    functTable.clear();
    functTable[0	] = "sll";
    functTable[2	] = "srl";
    functTable[3	] = "sra";
    functTable[4	] = "sllv";
    functTable[6	] = "srlv";
    functTable[7	] = "srav";
    functTable[8	] = "jr";
    functTable[9	] = "jalr";
    functTable[12	] = "syscall";
    functTable[13	] = "break";
    functTable[16	] = "mfhi";
    functTable[17	] = "mthi";
    functTable[18	] = "mflo";
    functTable[19	] = "mtlo";
    functTable[24	] = "mult";
    functTable[25	] = "multu";
    functTable[26	] = "div";
    functTable[27	] = "divu";
    functTable[32	] = "add";
    functTable[33	] = "addu";
    functTable[34	] = "sub";
    functTable[35	] = "subu";
    functTable[36	] = "and";
    functTable[37	] = "or";
    functTable[38	] = "xor";
    functTable[39	] = "nor";
    functTable[42	] = "slt";
    functTable[43	] = "sltu";

    rtTable.clear();
    rtTable[0] = "bltz";
    rtTable[1] = "bgez";
    rtTable[2] = "bltzl";
    rtTable[3] = "bgezl";
    //eret
    //mfc0
    //mtc0
}

Disassembler::~Disassembler()
{

}

QString Disassembler::bin2ass(QString input)
{
    if (input.size() % 32 != 0) {
        //QMessageBox invalid
        return "";
    }
    QString output = "";
    int address = 0;

    int sign = 0;

    while (!input.isEmpty()) {
        output += "0x" + QString::number(address, 16) + "\t:\t";
        QString instruction = input.mid(0, 32);
        if (instruction[16] == '1') sign = 1; else sign = 0;
        input.remove(0, 32);
        QString opcode = instruction.mid(0, 6);
        if (opcode == "000000" && instruction.mid(32 - 6 - 5, 5) == "00000" && type1.contains(functTable[instruction.mid(32 - 6, 6).toInt(NULL, 2)].toUpper())) {
            output += functTable[instruction.mid(32 - 6, 6).toInt(NULL, 2)];
            output += " " + registerTable[instruction.mid(6 + 5 + 5, 5).toInt(NULL, 2)] + ", " + registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", " +
                    registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)];
        } else if (type2.contains(opTable[instruction.mid(0, 6).toInt(NULL, 2)].toUpper())) {
            output += opTable[instruction.mid(0, 6).toInt(NULL, 2)];
            output += " " + registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)] + ", " + QString::number(instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15), 10) + "(" + registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ")";
        } else if (type3.contains(opTable[instruction.mid(0, 6).toInt(NULL, 2)].toUpper())) {
            output += opTable[instruction.mid(0, 6).toInt(NULL, 2)];
            output += " " + registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)] + ", " + registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", " + QString::number(instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15), 10);
        } else if (opcode == "001111") {
            output += "lui ";
            output += registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)] + ", " + QString::number(instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15), 10);
        } else if (opcode == "000000" && type4.contains(functTable[instruction.mid(32 - 6, 6).toInt(NULL, 2)].toUpper())) {
            output += functTable[instruction.mid(32 - 6, 6).toInt(NULL, 2)];
            output += " " + registerTable[instruction.mid(6 + 5 + 5, 5).toInt(NULL, 2)] + ", ";
            output += registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)] + ", ";
            output += QString::number(instruction.mid(32 - 6 - 5, 5).toInt(NULL, 2), 10);
        } else if (opcode == "000000" && instruction.mid(6 + 5 + 5, 10) == "0000000000" && type5.contains(functTable[instruction.mid(32 - 6, 6).toInt(NULL, 2)].toUpper())) {
            output += functTable[instruction.mid(32 - 6, 6).toInt(NULL, 2)];
            output += " " + registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", ";
            output += registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)];
        } else if (opcode == "000101") {
            output += "bne ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", ";
            output += registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)] + ", ";
            output += "0x" + QString::number(address + 4 * (instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15)), 16);
        } else if (opcode == "000100") {
            output += "beq ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", ";
            output += registerTable[instruction.mid(6 + 5, 5).toInt(NULL, 2)] + ", ";
            output += "0x" + QString::number(address + 4 * (instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15)), 16);
        } else if (opcode == "000001" && instruction.mid(11, 5) == "00001") {
            output += "bgez ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", ";
            output += "0x" + QString::number(address + 4 * (instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15)), 16);
        } else if (opcode == "000111" && instruction.mid(11, 5) == "00000") {
            output += "bgtz ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", ";
            output += "0x" + QString::number(address + 4 * (instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15)), 16);
        } else if (opcode == "000001" && instruction.mid(11, 5) == "00000") {
            output += "bltz ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", ";
            output += "0x" + QString::number(address + 4 * (instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15)), 16);
        } else if (opcode == "000110" && instruction.mid(11, 5) == "00000") {
            output += "blez ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)] + ", ";
            output += "0x" + QString::number(address + 4 * (instruction.mid(32 - 15, 15).toInt(NULL, 2) - sign * (1 << 15)), 16);
        } else if (opcode == "000010") {
            output += "j ";
            output += "0x" + QString::number(qlonglong(address & 0xf0000000) + instruction.mid(6, 26).toLongLong(NULL, 2) * 4, 16);
        } else if (opcode == "000011") {
            output += "jal ";
            output += "0x" + QString::number(qlonglong(address & 0xf0000000) + instruction.mid(6, 26).toLongLong(NULL, 2) * 4, 16);
        } else if (opcode == "000000" && instruction.mid(32 - 6, 6) == "001000" && instruction.mid(6 + 5, 15) == "000000000000000") {
            output += "jr ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)];
        } else if (opcode == "000000" && instruction.mid(32 - 6, 6) == "001001" && instruction.mid(6 + 5, 15) == "000001111100000") {
            output += "jalr ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)];
        } else if (instruction.mid(0, 16) == "0000000000000000" && instruction.mid(32 - 11, 11) == "00000010000") {
            output += "mfhi ";
            output += registerTable[instruction.mid(16, 5).toInt(NULL, 2)];
        } else if (instruction.mid(0, 16) == "0000000000000000" && instruction.mid(32 - 11, 11) == "00000010010") {
            output += "mflo ";
            output += registerTable[instruction.mid(16, 5).toInt(NULL, 2)];
        } else if (instruction.mid(0, 6) == "000000" && instruction.mid(11, 21) == "000000000000000010001") {
            output += "mthi ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)];
        } else if (instruction.mid(0, 6) == "000000" && instruction.mid(11, 21) == "000000000000000010011") {
            output += "mthi ";
            output += registerTable[instruction.mid(6, 5).toInt(NULL, 2)];
        } else if (instruction.mid(0, 11) == "01000000000" && instruction.mid(32 - 11, 11) == "00000000000") {
            output += "mfc0 ";
            output += registerTable[instruction.mid(11, 5).toInt(NULL, 2)] + ", ";
            output += registerTable[instruction.mid(16, 5).toInt(NULL, 2)];
        } else if (instruction.mid(0, 11) == "01000000100" && instruction.mid(32 - 11, 11) == "00000000000") {
            output += "mtc0 ";
            output += registerTable[instruction.mid(11, 5).toInt(NULL, 2)] + ", ";
            output += registerTable[instruction.mid(16, 5).toInt(NULL, 2)];
        } else if (instruction == "01000010000000000000000000011000") {
            output += "eret";
        } else if (instruction == "00000000000000000000000000001100") {
            output += "syscall";
        } else if (instruction.mid(0, 6) == "000000" && instruction.mid(32 - 6, 6) == "001100") {
            output += "break ";
            output += QString::number(instruction.mid(6, 20).toInt(NULL, 2), 10);
        }
        output += ";\n";
        address += 4;
        qDebug() << instruction;
    }
    return output;
}

