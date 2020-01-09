OPENQASM 2.0;
include "qelib1.inc";

qreg qubits[24];
ccx qubits[7],qubits[9],qubits[16];
ccx qubits[6],qubits[10],qubits[16];
ccx qubits[5],qubits[11],qubits[16];
ccx qubits[4],qubits[12],qubits[16];
ccx qubits[3],qubits[13],qubits[16];
ccx qubits[2],qubits[14],qubits[16];
ccx qubits[1],qubits[15],qubits[16];
ccx qubits[7],qubits[10],qubits[17];
ccx qubits[6],qubits[11],qubits[17];
ccx qubits[5],qubits[12],qubits[17];
ccx qubits[4],qubits[13],qubits[17];
ccx qubits[3],qubits[14],qubits[17];
ccx qubits[2],qubits[15],qubits[17];
ccx qubits[7],qubits[11],qubits[18];
ccx qubits[6],qubits[12],qubits[18];
ccx qubits[5],qubits[13],qubits[18];
ccx qubits[4],qubits[14],qubits[18];
ccx qubits[3],qubits[15],qubits[18];
ccx qubits[7],qubits[12],qubits[19];
ccx qubits[6],qubits[13],qubits[19];
ccx qubits[5],qubits[14],qubits[19];
ccx qubits[4],qubits[15],qubits[19];
ccx qubits[7],qubits[13],qubits[20];
ccx qubits[6],qubits[14],qubits[20];
ccx qubits[5],qubits[15],qubits[20];
ccx qubits[7],qubits[14],qubits[21];
ccx qubits[6],qubits[15],qubits[21];
ccx qubits[7],qubits[15],qubits[22];
cx qubits[22],qubits[18];
cx qubits[22],qubits[17];
cx qubits[22],qubits[16];
cx qubits[21],qubits[17];
cx qubits[21],qubits[16];
cx qubits[21],qubits[23];
cx qubits[20],qubits[16];
cx qubits[20],qubits[23];
cx qubits[20],qubits[22];
cx qubits[19],qubits[23];
cx qubits[19],qubits[22];
cx qubits[19],qubits[21];
cx qubits[18],qubits[22];
cx qubits[18],qubits[21];
cx qubits[18],qubits[20];
cx qubits[17],qubits[21];
cx qubits[17],qubits[20];
cx qubits[17],qubits[19];
cx qubits[16],qubits[20];
cx qubits[16],qubits[19];
cx qubits[16],qubits[18];
ccx qubits[7],qubits[8],qubits[23];
ccx qubits[6],qubits[9],qubits[23];
ccx qubits[5],qubits[10],qubits[23];
ccx qubits[4],qubits[11],qubits[23];
ccx qubits[3],qubits[12],qubits[23];
ccx qubits[2],qubits[13],qubits[23];
ccx qubits[1],qubits[14],qubits[23];
ccx qubits[0],qubits[15],qubits[23];
ccx qubits[6],qubits[8],qubits[22];
ccx qubits[5],qubits[9],qubits[22];
ccx qubits[4],qubits[10],qubits[22];
ccx qubits[3],qubits[11],qubits[22];
ccx qubits[2],qubits[12],qubits[22];
ccx qubits[1],qubits[13],qubits[22];
ccx qubits[0],qubits[14],qubits[22];
ccx qubits[5],qubits[8],qubits[21];
ccx qubits[4],qubits[9],qubits[21];
ccx qubits[3],qubits[10],qubits[21];
ccx qubits[2],qubits[11],qubits[21];
ccx qubits[1],qubits[12],qubits[21];
ccx qubits[0],qubits[13],qubits[21];
ccx qubits[4],qubits[8],qubits[20];
ccx qubits[3],qubits[9],qubits[20];
ccx qubits[2],qubits[10],qubits[20];
ccx qubits[1],qubits[11],qubits[20];
ccx qubits[0],qubits[12],qubits[20];
ccx qubits[3],qubits[8],qubits[19];
ccx qubits[2],qubits[9],qubits[19];
ccx qubits[1],qubits[10],qubits[19];
ccx qubits[0],qubits[11],qubits[19];
ccx qubits[2],qubits[8],qubits[18];
ccx qubits[1],qubits[9],qubits[18];
ccx qubits[0],qubits[10],qubits[18];
ccx qubits[1],qubits[8],qubits[17];
ccx qubits[0],qubits[9],qubits[17];
ccx qubits[0],qubits[8],qubits[16];
