#include "qc_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>


int main() {

    qreg *qr = new_qreg(4);
    if (qr == NULL) {
        printf("Error creating new quantum register\n");
        return 1;
    }

    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "H_0");
    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "CNOT_0_1");
    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "X_0");
    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "SWP_0_1");
    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "CNOT_0_1");
    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "SWP_0_1");
    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "H_1");
    view_state_vector(qr);printf("\n");

    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "X_0");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0");
    // view_state_vector(qr);printf("\n");



    return 0;
}
