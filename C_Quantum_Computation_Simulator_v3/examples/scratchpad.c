#include "qc_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>


int main() {

    qreg *qr = new_qreg(8);
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

    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "X_1|X_2|X_4");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_1_2|X_0|H_6|CNOT_5_3|");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0_2");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_2_3");
    // view_state_vector(qr);printf("\n");
    // circuit_layer(qr, "SWP_0_1");
    // view_state_vector(qr);printf("\n");

    printf("Initial state vector:\n");
    view_state_vector(qr);printf("\n");
    char gate_step_0[] = "H_3|H_1";
    circuit_layer(qr, gate_step_0);
    printf("State vector after applying the following gates: %s\n", gate_step_0);
    view_state_vector(qr);printf("\n");
    
    free_qreg(qr);

    return 0;
}
