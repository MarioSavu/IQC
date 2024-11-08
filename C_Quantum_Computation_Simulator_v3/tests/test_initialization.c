#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>

void test_initialization() {
    // Test 1: Initialize a quantum register with 1 qubit
    qreg *qr = new_qreg(1);
    assert(qr != NULL);
    printf("Test 1 Passed: Initialized quantum register with 1 qubit.\n");

    // Test 2: Initialize a quantum register with 3 qubits
    qreg *qr3 = new_qreg(3);
    assert(qr3 != NULL);
    printf("Test 2 Passed: Initialized quantum register with 3 qubits.\n");

    // Test 3: Free quantum registers
    free_qreg(qr);
    free_qreg(qr3);
    printf("Test 3 Passed: Freed quantum registers.\n");
}

int main() {
    test_initialization();
    printf("All initialization tests passed.\n");
    return 0;
}
