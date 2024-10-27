#include "qc_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

// #define TEST_SUITE_0
// #define TEST_SUITE_1
#define CIRCUIT_FIGURE_4_3

void main() {
    qbit zero = q_zero();
    qbit one = q_one();

#ifdef TEST_SUITE_0
    printf("Initial states for qubit zero and one:\n");
    print_qbit(zero);
    print_qbit(one);

    printf("\nTesting X gate:\n");
    print_qbit(X_gate(zero));
    print_qbit(X_gate(one));

    printf("\nTesting Y gate:\n");
    print_qbit(Y_gate(zero));
    print_qbit(Y_gate(one));

    printf("\nTesting Z gate:\n");
    print_qbit(Z_gate(zero));
    print_qbit(Z_gate(one));

    printf("\nTesting H gate:\n");
    print_qbit(H_gate(zero));
    print_qbit(H_gate(one));

    printf("\n\n");
#endif

#ifdef TEST_SUITE_1
    printf("|0> = ");print_qbit_short(zero);printf("\n");
    printf("|1> = ");print_qbit_short(one);printf("\n");

    printf("\nTesting X gate:\n");
    printf("X gate after |0> = ");print_qbit_short(X_gate(zero));printf("\n");
    printf("X gate after |1> = ");print_qbit_short(X_gate(one));printf("\n");

    printf("\nTesting Y gate:\n");
    printf("Y gate after |0> = ");print_qbit_short(Y_gate(zero));printf("\n");
    printf("Y gate after |1> = ");print_qbit_short(Y_gate(one));printf("\n");

    printf("\nTesting Z gate:\n");
    printf("Z gate after |0> = ");print_qbit_short(Z_gate(zero));printf("\n");
    printf("Z gate after |1> = ");print_qbit_short(Z_gate(one));printf("\n");

    printf("\nTesting H gate:\n");
    printf("H gate after |0> = ");print_qbit_short(H_gate(zero));printf("\n");
    printf("H gate after |1> = ");print_qbit_short(H_gate(one));printf("\n");

    printf("\nTesting CNOT gate:\n");
    printf("Control is |0> and Target is |0>, applying CNOT results in the following Target: ");
    print_qbit_short(CNOT_gate(zero, zero)); printf("\n");
    printf("Control is |0> and Target is |1>, applying CNOT results in the following Target: ");
    print_qbit_short(CNOT_gate(zero, one)); printf("\n");
    printf("Control is |1> and Target is |0>, applying CNOT results in the following Target: ");
    print_qbit_short(CNOT_gate(one, zero)); printf("\n");
    printf("Control is |1> and Target is |1>, applying CNOT results in the following Target: ");
    print_qbit_short(CNOT_gate(one, one)); printf("\n");

    printf("\n\n");
#endif


#ifdef CIRCUIT_FIGURE_4_3
    // Initialize system states for 4 qbits
    qbit q0 = q_zero();
    qbit q1 = q_zero();
    qbit q2 = q_zero();
    qbit q3 = q_zero();

    printf("Initial 4 Qubit System state is:\n");
    printf("Q0: ");print_qbit_short(q0);printf("\n");
    printf("Q1: ");print_qbit_short(q1);printf("\n");
    printf("Q2: ");print_qbit_short(q2);printf("\n");
    printf("Q3: ");print_qbit_short(q3);printf("\n\n");

    // Apply Hadamard to obtain state psi0
    q0 = H_gate(q0);
    printf("4 Qubit System state at PSI0:\n");
    printf("Q0: ");print_qbit_short(q0);printf("\n");
    printf("Q1: ");print_qbit_short(q1);printf("\n");
    printf("Q2: ");print_qbit_short(q2);printf("\n");
    printf("Q3: ");print_qbit_short(q3);printf("\n\n");

    // Apply CNOT gate to obtain state psi1
    q1 = CNOT_gate(q0 /* Control qubit*/, q1 /* Target qubit*/);
    printf("4 Qubit System state at PSI1:\n");
    printf("Q0: ");print_qbit_short(q0);printf("\n");
    printf("Q1: ");print_qbit_short(q1);printf("\n");
    printf("Q2: ");print_qbit_short(q2);printf("\n");
    printf("Q3: ");print_qbit_short(q3);printf("\n\n");

    // Apply X gate to obtain state psi2
    q0 = X_gate(q0);
    printf("4 Qubit System state at PSI2:\n");
    printf("Q0: ");print_qbit_short(q0);printf("\n");
    printf("Q1: ");print_qbit_short(q1);printf("\n");
    printf("Q2: ");print_qbit_short(q2);printf("\n");
    printf("Q3: ");print_qbit_short(q3);printf("\n\n");

    // Apply CNOT gate to obtain state psi3
    q0 = CNOT_gate(q1, q0);
    printf("4 Qubit System state at PSI3:\n");
    printf("Q0: ");print_qbit_short(q0);printf("\n");
    printf("Q1: ");print_qbit_short(q1);printf("\n");
    printf("Q2: ");print_qbit_short(q2);printf("\n");
    printf("Q3: ");print_qbit_short(q3);printf("\n\n");

    // Finally, apply Hadamard to obtain state psi4, then "measure"/print all states
    q1 = H_gate(q1);
    printf("4 Qubit System state at PSI4, after circuit 4.3 is:\n");
    printf("Q0: ");print_qbit_short(q0);printf("\n");
    printf("Q1: ");print_qbit_short(q1);printf("\n");
    printf("Q2: ");print_qbit_short(q2);printf("\n");
    printf("Q3: ");print_qbit_short(q3);printf("\n\n");

#endif

    return;
}
