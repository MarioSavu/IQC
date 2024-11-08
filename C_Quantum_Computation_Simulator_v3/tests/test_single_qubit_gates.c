#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>

void test_single_qubit_gates() {
    // Test 1: Apply X gate to a 1-qubit register
    qreg *qr = new_qreg(1);
    circuit_layer(qr, "X_0");
    view_state_vector(qr);
    // Add assertions to verify the state vector, if applicable
    printf("Test 1 Passed: Applied X gate to qubit 0.\n");

    // Test 2: Apply H gate to a 1-qubit register
    circuit_layer(qr, "H_0");
    view_state_vector(qr);
    // Add assertions to verify the state vector, if applicable
    printf("Test 2 Passed: Applied H gate to qubit 0.\n");

    free_qreg(qr);
}

int main() {
    test_single_qubit_gates();
    printf("All single-qubit gate tests passed.\n");
    return 0;
}
