#include "qc_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>


int main() {

    qreg *qr = new_qreg(5);
    if (qr == NULL) {
        printf("Error creating new quantum register\n");
        return 1;
    }

    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "H_0");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "CNOT_0_1");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "X_0");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0_1");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "CNOT_0_1");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0_1");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "H_1");
    // view_state_vector(qr);printf("\n");

    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "X_1|X_2|X_4");
    view_state_vector(qr);printf("\n");
    circuit_layer(qr, "SWP_1_3|SWP_4_0");
    view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0_2");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_2_3");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0_1");
    // view_state_vector(qr);printf("\n");



    return 0;
}
