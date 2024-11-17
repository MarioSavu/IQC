#include "qc_lib.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>


int state_to_index(const char *state, int num_qubits) {
    int index = 0;
    int state_it;
    for (int i = 0; i < num_qubits; i++) {
        state_it = i+1; // Skip the '|' at the beginning of the state string
        if (state[state_it] == '1') {
            index |= (1 << (num_qubits - i - 1));
        }
    }
    return index;
}

// Helper function to assert amplitude of specific state
void assert_amplitude_re(qreg *qr, const char *state, double expected_real) {
    int index = state_to_index(state, qr->size);
    assert(fabs(qr->amp[index].re - expected_real) < 1e-6);
}


// Helper function to assert complex amplitude of specific state
void assert_complex_amplitude(qreg *qr, const char *state, double expected_real, double expected_imag) {
    int index = state_to_index(state, qr->size);
    // printf("Expected re %f im %f, actual re %f, im %f\n", expected_real, expected_imag, qr->amp[index].re, qr->amp[index].im);
    assert(fabs(qr->amp[index].re - expected_real) < 1e-6);
    assert(fabs(qr->amp[index].im - expected_imag) < 1e-6);
}

// Single simple qubit gate tests
void test_simple_single_qubit_gates() {
    qreg *qr = new_qreg(3);

    // Apply X gate to qubit 0
    circuit_layer(qr, "X_0");
    assert_amplitude_re(qr, "|001>", 1.0);

    // Apply H gate to qubit 1
    circuit_layer(qr, "H_1");
    assert_amplitude_re(qr, "|001>", 1/sqrt(2));
    assert_amplitude_re(qr, "|011>", 1/sqrt(2));

    // Apply Y gate to qubit 2
    circuit_layer(qr, "Y_2");
    assert_complex_amplitude(qr, "|101>", 0.0, 1/sqrt(2));
    assert_complex_amplitude(qr, "|111>", 0.0, 1/sqrt(2));

    // Apply Z gate to qubit 1
    circuit_layer(qr, "Z_1");
    assert_complex_amplitude(qr, "|101>", 0.0, 1/sqrt(2));
    assert_complex_amplitude(qr, "|111>", 0.0, -1/sqrt(2));

    free_qreg(qr);

    printf("Simple single qubit gates pass\n");
}

// Rotation gates
void test_rotation_gates() {
    qreg *qr = new_qreg(3);

    // Set a known initial state for the qubit
    // Apply X gate to prepare qubit 0 in the |1> state
    circuit_layer(qr, "X_0");

    // Apply S gate to qubit 0, which should now add a phase
    circuit_layer(qr, "S_0");
    assert_complex_amplitude(qr, "|001>", 0.0, 1.0); // Should add a 90 degree phase to |1> state

    // // Apply T gate to qubit 1
    circuit_layer(qr, "T_0");
    assert_complex_amplitude(qr, "|001>", -1/sqrt(2), 1/sqrt(2));

    // Apply RX gate to qubit 0 with pi/2 rotation
    double radians = M_PI/2;
    char gates_string[64];
    snprintf(gates_string, sizeof(gates_string), "RX_%d_%f",
        0 /* Qubit index*/, radians /* Rotation value */);
    circuit_layer(qr, gates_string); // RX_0_Pi/2
    assert_complex_amplitude(qr, "|000>", 0.5, 0.5);
    assert_complex_amplitude(qr, "|001>", -0.5, 0.5);

    // // Apply RY gate to qubit 1 with pi/2 rotation
    radians = M_PI/2;
    snprintf(gates_string, sizeof(gates_string), "RY_%d_%f",
        0 /* Qubit index*/, radians /* Rotation value */);
    circuit_layer(qr, gates_string); // RY_0_Pi/2
    assert_complex_amplitude(qr, "|000>", 1/sqrt(2), 0.0);
    assert_complex_amplitude(qr, "|001>", 0.0, 1/sqrt(2));

    // Apply RZ gate to qubit 2 with pi/2 rotation
    radians = M_PI/2;
    snprintf(gates_string, sizeof(gates_string), "RZ_%d_%f",
        0 /* Qubit index*/, radians /* Rotation value */);
    circuit_layer(qr, gates_string); // RZ_0_Pi/2 
    assert_complex_amplitude(qr, "|000>", 0.5, -0.5);
    assert_complex_amplitude(qr, "|001>", -0.5, 0.5);

    // // Apply Phase gate to qubit 0 with pi/4 phase
    radians = M_PI/4;
    snprintf(gates_string, sizeof(gates_string), "P_%d_%f",
        0 /* Qubit index*/, radians /* Rotation value */);
    circuit_layer(qr, gates_string); // P_0_Pi/4
    assert_complex_amplitude(qr, "|000>", 0.5, -0.5);
    assert_complex_amplitude(qr, "|001>", -1/sqrt(2), 0.0);

    free_qreg(qr);

    printf("Rotation qubit gates pass\n");
}

// Two qubit gate tests
void test_two_qubit_gates() {
    qreg *qr = new_qreg(4);

    // Apply CNOT with control 0 and target 1
    circuit_layer(qr, "CNOT_0_1");
    assert_amplitude_re(qr, "|0000>", 1.0);

    // Apply X gate to qubit 0, to prepare for CNOT
    circuit_layer(qr, "X_0");

    // Apply CNOT with control 0 and target 1
    circuit_layer(qr, "CNOT_0_1");
    assert_amplitude_re(qr, "|0011>", 1.0);

    // Apply CNOT with non-adjacent qubits
    circuit_layer(qr, "CNOT_1_3");
    assert_amplitude_re(qr, "|1011>", 1.0);

    // Apply CNOT in reverse order (control > target)
    circuit_layer(qr, "CNOT_3_1");
    assert_amplitude_re(qr, "|1001>", 1.0);

    // Apply SWAP gate between qubits 0 and 3
    circuit_layer(qr, "SWP_0_2");
    assert_amplitude_re(qr, "|1100>", 1.0);

    free_qreg(qr);

    printf("Two qubit gates pass\n");
}

// Three qubit gate tests
void test_three_qubit_gates() {
    qreg *qr = new_qreg(3);

    // Apply CCNOT with control qubits 0 and 1, target qubit 2
    circuit_layer(qr, "CCNOT_0_1_2");
    assert_amplitude_re(qr, "|000>", 1.0);

    // Apply X gate to qubit 0, to prepare for CCNOT
    circuit_layer(qr, "X_0");
    circuit_layer(qr, "X_1");

    // Apply CCNOT with control qubits 0 and 1, target qubit 2
    circuit_layer(qr, "CCNOT_0_1_2");
    assert_amplitude_re(qr, "|111>", 1.0);

    // Apply CCNOT with different qubit combinations
    circuit_layer(qr, "CCNOT_1_2_0");
    assert_amplitude_re(qr, "|110>", 1.0);

    free_qreg(qr);

    printf("Three qubit gates pass\n");
}

// Parallel gate tests
void test_parallel_gates() {
    qreg *qr = new_qreg(3);

    // Apply X gate in parallel to all qubits
    circuit_layer(qr, "X_0|X_1|X_2");
    assert_amplitude_re(qr, "|111>", 1.0);
    // Reset
    circuit_layer(qr, "X_0|X_1|X_2");

    // Apply H in parallel to all qubits
    circuit_layer(qr, "H_0|H_1|H_2");
    // Expected to be in superposition
    assert_amplitude_re(qr, "|000>", 1/sqrt(8));
    assert_amplitude_re(qr, "|001>", 1/sqrt(8));
    assert_amplitude_re(qr, "|010>", 1/sqrt(8));
    assert_amplitude_re(qr, "|011>", 1/sqrt(8));
    assert_amplitude_re(qr, "|100>", 1/sqrt(8));
    assert_amplitude_re(qr, "|101>", 1/sqrt(8));
    assert_amplitude_re(qr, "|110>", 1/sqrt(8));
    assert_amplitude_re(qr, "|111>", 1/sqrt(8));
    // Reset
    circuit_layer(qr, "H_0|H_1|H_2");

    // Apply X gate to the first qubit, nothing on the 2nd, and an H gate on the third
    circuit_layer(qr, "X_2|H_0");
    assert_amplitude_re(qr, "|100>", 1/sqrt(2));
    assert_amplitude_re(qr, "|101>", 1/sqrt(2));


    free_qreg(qr);

    printf("Parallel qubit gates pass\n");
}

int main() {
    test_simple_single_qubit_gates();
    test_two_qubit_gates();
    test_three_qubit_gates();
    test_parallel_gates();
    test_rotation_gates();

    printf("All simulator gate tests passed successfully.\n");
    return 0;
}
