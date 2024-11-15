#ifndef QC_LIB_H // Include guard 
#define QC_LIB_H

// Public API
#define QUBIT_REGISTER_LIMIT 12
// #define PRINT_LSB_LEFT

typedef struct complex_number {
    double re, im; 
} cnum;

typedef struct quantum_register {
    int size; // Number of qubits in the register

    // State vector
    cnum *amp; /* 2^size number of amplitudes, dynamically allocated & freed,
     corresponding to the probability of the register being in a particular state */
} qreg;

typedef struct quantum_gate {
    char type[10];    // Type of the gate (e.g., "X", "H", "CNOT", "SWP")
    int size;         // Number of qubits it’s applied to (1 or 2)
    cnum **matrix;    // Matrix operator for the particular gate
} qgate;


typedef struct gate_node {
    qgate *gate;
    int *qubits; // List of qubits this gate acts on
    struct gate_node *next;
} gate_node;

typedef struct gate_list {
    gate_node *head;
} gate_list;

qreg *new_qreg(int size);
void free_qreg(qreg *qr);
void circuit_layer(qreg *qr, const char *operations);
void view_state_vector(qreg *qr);

#endif // End of include guard
