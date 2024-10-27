#ifndef QC_LIB_H // Include guard 
#define QC_LIB_H

// Public API
typedef struct complex_number {
    double real, imaginary; 
} cnum;

typedef struct quantum_bit {
    cnum zero, one; // Probabilities of being in either |0> or |1> states 
} qbit;

typedef struct quantum_bit_2 {
    cnum amp[4]; // Amplitudes for each possible 2 qubit states (00, 01, 10, 11)
} qbit2;

qbit X_gate(qbit in);
qbit Y_gate(qbit in);
qbit Z_gate(qbit in);
qbit H_gate(qbit in);
qbit CNOT_gate(qbit ctrl_in, qbit target_in);
// qbit CCNOT_gate(qbit ctrl_0_in, qbit ctrl_1_in, qbit target_in);

qbit q_zero();
qbit q_one();

void print_qbit(qbit in);
void print_qbit_short(qbit in);

// Private helper functions
static cnum cnum_copy(cnum one);
static cnum cnum_sum2(cnum one, cnum two);
static cnum cnum_sum4(cnum one, cnum two, cnum three, cnum four);
static cnum cnum_product(cnum one, cnum two);
static qbit matrix22_qbit_product(cnum gate[2][2], qbit in);
static qbit2 matrix44_qbit2_product(cnum gate[4][4], qbit2 in);

#endif // End of include guard
