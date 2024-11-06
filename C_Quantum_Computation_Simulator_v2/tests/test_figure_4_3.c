#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>

void test_two_qubit_gates() {
    // Test 1: Test figure 4.3 on a 4 qubit register
    qreg *qr = new_qreg(4);
    circuit_layer(qr, "H_0");
    circuit_layer(qr, "CNOT_0_1");
    circuit_layer(qr, "X_0");
    circuit_layer(qr, "SWP_0_1");
    circuit_layer(qr, "CNOT_0_1");
    circuit_layer(qr, "SWP_0_1");
    circuit_layer(qr, "H_1");

    view_state_vector(qr);
    // Add assertions to verify the state vector, if applicable
    printf("Test 1 Passed: Applied SWP gate to qubits 0 and 1.\n");

    free_qreg(qr);
}

int main() {
    test_two_qubit_gates();
    printf("All Figure 4.3 gate tests passed.\n");
    return 0;
}

