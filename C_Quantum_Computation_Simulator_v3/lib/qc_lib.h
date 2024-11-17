#ifndef QC_LIB_H // Include guard 
#define QC_LIB_H

// Public API
#define QUBIT_REGISTER_LIMIT 12
#define PRINT_MSB_LEFT

typedef struct complex_number {
    double re, im; 
} cnum;

typedef struct quantum_register {
    int size; // Number of qubits in the register

    // State vector
    cnum *amp; /* 2^size number of amplitudes, dynamically allocated & freed,
     corresponding to the probability of the register being in a particular state */
} qreg;

qreg *new_qreg(int size);
void free_qreg(qreg *qr);
void circuit_layer(qreg *qr, const char *operations);
void view_state_vector(qreg *qr);

#endif // End of include guard
