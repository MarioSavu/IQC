#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>

void build_figure_4_3() {
    // Build & show output of figure 4.3 on a 4 qubit register
    qreg *qr = new_qreg(4);
    circuit_layer(qr, "H_0");
    circuit_layer(qr, "CNOT_0_1");
    circuit_layer(qr, "X_0");
    circuit_layer(qr, "SWP_0_1");
    circuit_layer(qr, "CNOT_0_1");
    circuit_layer(qr, "SWP_0_1");
    circuit_layer(qr, "H_1");

    view_state_vector(qr);

    free_qreg(qr);
}

int main() {
    build_figure_4_3();

    return 0;
}

