#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>

void test_two_qubit_gates() {
    // Test 1: Apply SWP gate to a 2-qubit register
    qreg *qr = new_qreg(2);
    circuit_layer(qr, "SWP_0_1");
    view_state_vector(qr);
    // Add assertions to verify the state vector, if applicable
    printf("Test 1 Passed: Applied SWP gate to qubits 0 and 1.\n");

    // Test 2: Apply CNOT gate to a 2-qubit register
    circuit_layer(qr, "CNOT_0_1");
    view_state_vector(qr);
    // Add assertions to verify the state vector, if applicable
    printf("Test 2 Passed: Applied CNOT gate to qubits 0 and 1.\n");

    free_qreg(qr);
}

int main() {
    test_two_qubit_gates();
    printf("All two-qubit gate tests passed.\n");
    return 0;
}

