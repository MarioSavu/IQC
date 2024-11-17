#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>

int main() {
    qreg *qr = new_qreg(2);
    circuit_layer(qr, "H_0");
    circuit_layer(qr, "CNOT_0_1");

    printf("A bell state generated by a Hadamard on Qubit 0 followed by a control not on Qubits 0 & 1:\n");
    view_state_vector(qr);printf("\n");

    free_qreg(qr);

    return 0;
}
