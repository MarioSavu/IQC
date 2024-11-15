#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>

void grover_search_second_object() {
    // Quantum register formed of 3 qubits: first 2 qubits are our search space (which form 2^2=4 possible positions/objects), and the third one is an ancillary qubit
    qreg *qr = new_qreg(3);
    
    // Set the third qubit to 1, by applying X gate
    circuit_layer(qr, "X_2");
    
    // Then the first proper step is to prepare the states by putting everything in an equally weighted superposition
    // Hadamard gate on all qubits in the circuit
    circuit_layer(qr, "H_0|H_1|H_2");
    // View the state (for the homework "computation" exercise)
    printf("State |psi0>:\n");
    view_state_vector(qr);printf("\n");

    // Then we begin our sqrt(4)=2 iterations to find our desired qubit
    for(int it = 0; it < 2; it++) {
        // Let's build and apply our chosen oracle, for the 2nd object:
        circuit_layer(qr, "X_0");
        circuit_layer(qr, "CCNOT_0_1_2");
        circuit_layer(qr, "X_0");
        // View the state (for the homework "computation" exercise)
        printf("State |psi1>, %d time:\n", it);
        view_state_vector(qr);printf("\n");

        // And now let's apply our diffusion operator
        circuit_layer(qr, "H_0|H_1");
        // View the state (for the homework "computation" exercise)
        printf("State |psi2>, %d time:\n", it);
        view_state_vector(qr);printf("\n");
        circuit_layer(qr, "X_0|X_1");
        // View the state (for the homework "computation" exercise)
        printf("State |psi3>, %d time:\n", it);
        view_state_vector(qr);printf("\n");
        circuit_layer(qr, "H_1");
        circuit_layer(qr, "CNOT_0_1");
        circuit_layer(qr, "H_1");
        circuit_layer(qr, "X_0|X_1");
        circuit_layer(qr, "H_0|H_1|H_2");

        printf("State after finishing the diffusion operator %d times:\n", it);
        view_state_vector(qr);printf("\n");
    }

    printf("Final state:\n");
    view_state_vector(qr);printf("\n");

    free_qreg(qr);
}

int main() {
    grover_search_second_object();

    return 0;
}

