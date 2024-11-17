#include "qc_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// #define DEBUG_PRINTS

#ifdef DEBUG_PRINTS 
#define debug_printf(...) printf(__VA_ARGS__)
#else
#define debug_printf(...) ((void)0)
#endif

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

void apply_gate(qreg *qr, gate_list *gates);

// Helper function to allocate a 2D matrix of complex numbers
cnum **allocate_matrix(int size) {
    // Improvement: if there are allocation issues, first free previous allocations then return NULL
    if(size) {
        cnum **matrix = (cnum **)malloc(size * sizeof(cnum *));
        if (matrix == NULL) {
            fprintf(stderr, "Error allocating matrix in allocate_matrix\n");
            return NULL;
        }
        for (int i = 0; i < size; i++) {
            matrix[i] = (cnum *)calloc(size, sizeof(cnum)); // Zero-initialize each element
            if (matrix[i] == NULL) {
                fprintf(stderr, "Error zero-allocating each row of the new matrix\n");
                return NULL;
            }
        }
        return matrix;
    }
    else {
        fprintf(stderr, "Error trying to allocate a zero sized matrix\n");
        return NULL;
    }
}

// Free a matrix of complex numbers
void free_matrix(cnum **matrix, int dim) {
    if(dim != 0) {
        for (int i = 0; i < dim; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
    else {
        fprintf(stderr, "Error trying to free a matrix with an incorrect size size %d\n", dim);
    }
}

// Function to initialize the gate list
void init_gate_list(gate_list *gates) {
    gates->head = NULL;
}

// Function to add a gate to the gate list
void add_gate_to_list(gate_list *gates, qgate *gate, int *qubits) {
    gate_node *node = malloc(sizeof(gate_node));
    node->gate = gate;
    node->qubits = qubits;
    node->next = gates->head;
    gates->head = node;
}

// Function to clear the gate list
void clear_gate_list(gate_list *gates) {
    gate_node *current = gates->head;
    while (current != NULL) {
        free_matrix(current->gate->matrix, current->gate->size == 1 ? 2 : 4);
        free(current->gate);
        free(current->qubits);
        gate_node *next = current->next;
        free(current);
        current = next;
    }
    gates->head = NULL;
}

// Function to create a 2x2 identity matrix
cnum **create_identity_matrix() {
    cnum **matrix = allocate_matrix(2);
    if (matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for identity matrix\n");
        return NULL;
    }
    matrix[0][0] = (cnum){1, 0}; // 1 + 0i
    matrix[1][1] = (cnum){1, 0}; // 1 + 0i
    return matrix;
}

qgate *create_x_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for Y gate\n");
        return NULL;
    }
    strncpy(gate->type, "X", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for X gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][1] = (cnum){1, 0};
    gate->matrix[1][0] = (cnum){1, 0};
    return gate;
}

qgate *create_y_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for Y gate\n");
        return NULL;
    }
    strncpy(gate->type, "Y", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for Y gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][1] = (cnum){0, -1};
    gate->matrix[1][0] = (cnum){0, 1};
    return gate;
}

qgate *create_z_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for Z gate\n");
        return NULL;
    }
    strncpy(gate->type, "Z", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for Z gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][1] = (cnum){-1, 0};
    return gate;
}

qgate *create_h_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for H gate\n");
        return NULL;
    }
    strncpy(gate->type, "H", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for H gate's matrix\n");
        return NULL;
    }
    double inv_sqrt_2 = 1.0 / sqrt(2.0);
    gate->matrix[0][0] = (cnum){inv_sqrt_2, 0};
    gate->matrix[0][1] = (cnum){inv_sqrt_2, 0};
    gate->matrix[1][0] = (cnum){inv_sqrt_2, 0};
    gate->matrix[1][1] = (cnum){-inv_sqrt_2, 0};
    return gate;
}

qgate *create_s_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for S gate\n");
        return NULL;
    }
    strncpy(gate->type, "S", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for S gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][1] = (cnum){0, 1};
    return gate;
}

qgate *create_t_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for T gate\n");
        return NULL;
    }
    strncpy(gate->type, "T", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for T gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){1, 0};
    // I got the values below using Euler's formula, to avoid having to write a complex number power function for e^(i*M_PI/4)
    gate->matrix[1][1] = (cnum){cos(M_PI / 4), sin(M_PI / 4)};  
    return gate;
}

qgate *create_rx_gate(double angle) {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for RX gate\n");
        return NULL;
    }
    strncpy(gate->type, "RX", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for RX gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){cos(angle / 2), 0};
    gate->matrix[0][1] = (cnum){0, -sin(angle / 2)};
    gate->matrix[1][0] = (cnum){0, -sin(angle / 2)};
    gate->matrix[1][1] = (cnum){cos(angle / 2), 0};
    return gate;
}

qgate *create_ry_gate(double angle) {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for RY gate\n");
        return NULL;
    }
    strncpy(gate->type, "RY", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for RY gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){cos(angle / 2), 0};
    gate->matrix[0][1] = (cnum){-sin(angle / 2), 0};
    gate->matrix[1][0] = (cnum){sin(angle / 2), 0};
    gate->matrix[1][1] = (cnum){cos(angle / 2), 0};
    return gate;
}

qgate *create_rz_gate(double angle) {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for RZ gate\n");
        return NULL;
    }
    strncpy(gate->type, "RZ", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for RZ gate's matrix\n");
        return NULL;
    }
    // I got the values below using Euler's formula, to avoid having to write a complex number power function for e^(i*angle/2) & e^(-i*angle/2)
    gate->matrix[0][0] = (cnum){cos(angle / 2), -sin(angle / 2)};
    gate->matrix[1][1] = (cnum){cos(angle / 2), sin(angle / 2)};
    return gate;
}

qgate *create_phase_gate(double angle) {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for P gate\n");
        return NULL;
    }
    strncpy(gate->type, "P", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for P gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][1] = (cnum){cos(angle), sin(angle)};
    return gate;
}

qgate *create_cnot_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for CNOT gate\n");
        return NULL;
    }
    strncpy(gate->type, "CNOT", sizeof(gate->type));
    gate->size = 2;
    gate->matrix = allocate_matrix(4);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for CNOT gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][1] = (cnum){1, 0};
    gate->matrix[2][3] = (cnum){1, 0};
    gate->matrix[3][2] = (cnum){1, 0};
    return gate;
}

qgate *create_reverse_cnot_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for CNOT gate\n");
        return NULL;
    }
    strncpy(gate->type, "RCNOT", sizeof(gate->type));
    gate->size = 2;
    gate->matrix = allocate_matrix(4);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for CNOT gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][3] = (cnum){1, 0};
    gate->matrix[2][2] = (cnum){1, 0};
    gate->matrix[3][1] = (cnum){1, 0};
    return gate;
}

qgate *create_swap_gate() {
    qgate *gate = malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Failed to allocate memory for SWP gate\n");
        return NULL;
    }
    strncpy(gate->type, "SWP", sizeof(gate->type));
    gate->size = 2;
    gate->matrix = allocate_matrix(4);
    if (gate->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for SWP gate's matrix\n");
        return NULL;
    }
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][2] = (cnum){1, 0};
    gate->matrix[2][1] = (cnum){1, 0};
    gate->matrix[3][3] = (cnum){1, 0};
    return gate;
}

qgate *create_custom_gate(cnum **matrix, int num_qubits) {
    debug_printf("Creating custom gate for %d qubits\n", num_qubits);
    
    // Allocate memory for the gate
    qgate *gate = (qgate *)malloc(sizeof(qgate));
    if (gate == NULL) {
        fprintf(stderr, "Error allocating memory for custom gate.\n");
        return NULL;
    }

    // Set the number of qubits for the gate
    gate->size = num_qubits;

    // Assign the matrix to the gate (assuming the matrix is already allocated)
    gate->matrix = matrix;

    return gate;
}


qgate *generate_generic_swap(int num_qubits, int qubit_1, int qubit_2) {
    debug_printf("Generating generic SWAP gate for qubits %d and %d across %d qubits\n", qubit_1, qubit_2, num_qubits);

    int size = 1 << num_qubits;  // 2^num_qubits
    cnum **swap_matrix = allocate_matrix(size);
    if (swap_matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for generic SWAP matrix\n");
        return NULL;
    }

    // Initialize to identity matrix
    for (int i = 0; i < size; i++) {
        swap_matrix[i][i].re = 1.0;
        swap_matrix[i][i].im = 0.0;
    }

    // Adjust indices so that we can effectively work as if qubit_1 is at position 0
    int min_qubit = (qubit_1 < qubit_2) ? qubit_1 : qubit_2;
    int adjusted_qubit_1 = qubit_1 - min_qubit;
    int adjusted_qubit_2 = qubit_2 - min_qubit;
    debug_printf("Translates to a swap in-between %d and %d\n", adjusted_qubit_1, adjusted_qubit_2);

    // Apply swaps to construct the composite matrix
    for (int i = 0; i < size; i++) {
        int swapped_idx = i;
        if (((i >> adjusted_qubit_1) & 1) != ((i >> adjusted_qubit_2) & 1)) {
            swapped_idx ^= (1 << adjusted_qubit_1) | (1 << adjusted_qubit_2);
            swap_matrix[i][swapped_idx].re = 1.0;
            swap_matrix[i][i].re = 0.0; // Clear original entry if swapped
        }
    }

    qgate *swap_gate = create_custom_gate(swap_matrix, num_qubits);
    if (swap_gate == NULL) {
        fprintf(stderr, "Failed to create custom SWAP gate\n");
        free_matrix(swap_matrix, size);
        return NULL;
    }

    return swap_gate;
}

qgate *generate_dynamic_ccnot(int num_qubits, int control_1, int control_2, int target) {
    debug_printf("Generating CCNOT gate for control qubits %d, %d and target qubit %d across %d qubits\n", control_1, control_2, target, num_qubits);

    int size = 1 << num_qubits;  // 2^num_qubits
    cnum **ccnot_matrix = allocate_matrix(size);
    if (ccnot_matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for CCNOT matrix\n");
        return NULL;
    }

    // Initialize to identity matrix
    for (int i = 0; i < size; i++) {
        ccnot_matrix[i][i].re = 1.0;
        ccnot_matrix[i][i].im = 0.0;
    }

    // Adjust indices so that we can effectively work as if control_1, control_2, and target are relative positions
    int min_qubit = (control_1 < control_2) ? (control_1 < target ? control_1 : target) : (control_2 < target ? control_2 : target);
    int adjusted_control_1 = control_1 - min_qubit;
    int adjusted_control_2 = control_2 - min_qubit;
    int adjusted_target = target - min_qubit;

    debug_printf("Adjusted control_1 = %d, ctrl 2 = %d, target = %d\n", adjusted_control_1, adjusted_control_2, adjusted_target);

    // Apply Toffoli logic to construct the matrix
    for (int i = 0; i < size; i++) {
        // Check if both control qubits are set to 1
        if (((i >> (num_qubits-1-adjusted_control_1)) & 1) == 1 && ((i >> (num_qubits-1-adjusted_control_2)) & 1) == 1) {
            debug_printf("Hit! i=%d\n", i);
            // Flip the target qubit only if both control qubits are 1
            int flipped_idx = i ^ (1<< (num_qubits-1-adjusted_target));
            if (flipped_idx != i) {
                debug_printf("Hit hit, flipped_idx=%d\n", flipped_idx);
                ccnot_matrix[flipped_idx][i].re = 1.0;
                ccnot_matrix[i][i].re = 0.0;  // Clear original entry if swapped
            }
        }
    }

    qgate *ccnot_gate = create_custom_gate(ccnot_matrix, num_qubits);
    if (ccnot_gate == NULL) {
        fprintf(stderr, "Failed to create custom CCNOT gate\n");
        free_matrix(ccnot_matrix, size);
        return NULL;
    }

    return ccnot_gate;
}



void parse_circuit_layer(qreg *qr, const char *operations) {
    debug_printf("Parsing circuit layer: %s\n", operations);
    const char *op_ptr = operations;
    char gate_type[16];
    int qubit_1, qubit_2, qubit_3;
    double angle;

    gate_list swap_gates;
    gate_list current_gates;

    init_gate_list(&swap_gates);
    init_gate_list(&current_gates);

    while (*op_ptr != '\0') {
        // Parse the gate type
        if (sscanf(op_ptr, "%15[^_]_", gate_type) != 1) {
            fprintf(stderr, "Error parsing gate type\n");
            return;
        }

        op_ptr += strlen(gate_type) + 1;

        if (strcmp(gate_type, "CCNOT") == 0) {
            // Parse three qubit indices for CCNOT gate
            if (sscanf(op_ptr, "%d_%d_%d", &qubit_1, &qubit_2, &qubit_3) != 3) {
                fprintf(stderr, "Error parsing %s qubits\n", gate_type);
                return;
            }
            if(qubit_1 > qr->size || qubit_2 > qr->size || qubit_3 > qr->size) {
                fprintf(stderr, "Error: specified qubit in gate %s is greater than the circuit size: %d %d %d\n", gate_type, qubit_1, qubit_2, qubit_3);
                return;
            }
            if(qubit_1 < 0 || qubit_2 < 0 || qubit_3 < 0) {
                fprintf(stderr, "Error: specified qubit in gate %s less than zero: %d %d %d\n", gate_type, qubit_1, qubit_2, qubit_3);
                return;
            }
            if(qubit_1 == qubit_2 || qubit_1 == qubit_3 || qubit_2 == qubit_3) {
                fprintf(stderr, "Error: specified qubits in gate %s are equal: %d %d %d\n", gate_type, qubit_1, qubit_2, qubit_3);
                return;
            }
            debug_printf("Parsed CCNOT gate for control qubits %d, %d and target qubit %d\n", qubit_1, qubit_2, qubit_3);

            // Re-order qubit index
            qubit_1 = (qr->size - 1) - qubit_1;
            qubit_2 = (qr->size - 1) - qubit_2;
            qubit_3 = (qr->size - 1) - qubit_3;

            // Generate dynamic CCNOT gate
            int max_gate_qbit, min_gate_qbit;
            max_gate_qbit = (qubit_1 > qubit_2) ? ((qubit_1 > qubit_3) ? qubit_1 : qubit_3) : ((qubit_2 > qubit_3) ? qubit_2 : qubit_3); 
            min_gate_qbit = (qubit_1 < qubit_2) ? ((qubit_1 < qubit_3) ? qubit_1 : qubit_3) : ((qubit_2 < qubit_3) ? qubit_2 : qubit_3); 

            qgate *ccnot_gate = generate_dynamic_ccnot((max_gate_qbit - min_gate_qbit) + 1, qubit_1, qubit_2, qubit_3);
            if (ccnot_gate == NULL) {
                fprintf(stderr, "Failed to generate dynamic CCNOT gate\n");
                return;
            }
            int *qubits = malloc(3 * sizeof(int));
            // Must assign these in increasing order
            qubits[0] = qubit_1 < qubit_2 ? (qubit_1 < qubit_3? qubit_1 : qubit_3) : (qubit_2 < qubit_3? qubit_2 : qubit_3);
            qubits[1] = qubit_1 < qubit_2 ? (qubit_1 < qubit_3 ? (qubit_2 < qubit_3 ? qubit_2: qubit_3) : qubit_1) : (qubit_1 > qubit_3? (qubit_2 > qubit_3 ? qubit_2 : qubit_3) : qubit_1);
            qubits[2] = qubit_1 > qubit_2 ? (qubit_1 > qubit_3? qubit_1 : qubit_3) : (qubit_2 > qubit_3? qubit_2 : qubit_3);
            add_gate_to_list(&current_gates, ccnot_gate, qubits);
        }
        else if (strcmp(gate_type, "SWP") == 0) {
            // Parse two qubit indices for SWP gate
            if (sscanf(op_ptr, "%d_%d", &qubit_1, &qubit_2) != 2) {
                fprintf(stderr, "Error parsing SWP qubits\n");
                return;
            }
            if(qubit_1 > qr->size || qubit_2 > qr->size) {
                fprintf(stderr, "Error: specified qubit in gate %s is greater than the circuit size: %d %d\n", gate_type, qubit_1, qubit_2);
                return;
            }
            if(qubit_1 < 0 || qubit_2 < 0) {
                fprintf(stderr, "Error: specified qubit in gate %s less than zero: %d %d\n", gate_type, qubit_1, qubit_2);
                return;
            }
            if(qubit_1 == qubit_2) {
                fprintf(stderr, "Error: specified qubits in gate %s are equal: %d %d\n", gate_type, qubit_1, qubit_2);
                return;
            }

            debug_printf("Parsed SWP gate for qubits %d and %d\n", qubit_1, qubit_2);

            // Re-order qubit index
            qubit_1 = (qr->size - 1) - qubit_1;
            qubit_2 = (qr->size - 1) - qubit_2;

            // Normalize qubit order to ensure qubit_1 < qubit_2
            if (qubit_1 > qubit_2) {
                int temp = qubit_1;
                qubit_1 = qubit_2;
                qubit_2 = temp;
            }

            // Check if qubits are adjacent
            if (abs(qubit_1 - qubit_2) == 1) {
                // Add adjacent SWAP gate to the gate list as before
                // Create SWP gate and add to circuit
                qgate *swap_gate = create_swap_gate();
                int *qubits = malloc(2 * sizeof(int));
                qubits[0] = qubit_1;
                qubits[1] = qubit_2;
                add_gate_to_list(&current_gates, swap_gate, qubits);
            } else {
                // Generate a generic SWAP matrix and add it to the gate list
                qgate *swap_gate = generate_generic_swap(abs(qubit_1-qubit_2)+1, qubit_1, qubit_2);
                if (swap_gate != NULL) {
                    int *qubits = malloc(2 * sizeof(int));
                    qubits[0] = qubit_1;
                    qubits[1] = qubit_2;
                    add_gate_to_list(&current_gates, swap_gate, qubits);
                }
            }
        }
        else if (strcmp(gate_type, "CNOT") == 0) {
            // Parse two qubit indices for CNOT gate
            if (sscanf(op_ptr, "%d_%d", &qubit_1, &qubit_2) != 2) {
                fprintf(stderr, "Error parsing %s qubits\n", gate_type);
                return;
            }
            if(qubit_1 > qr->size || qubit_2 > qr->size) {
                fprintf(stderr, "Error: specified qubit in gate %s is greater than the circuit size: %d %d\n", gate_type, qubit_1, qubit_2);
                return;
            }
            if(qubit_1 < 0 || qubit_2 < 0) {
                fprintf(stderr, "Error: specified qubit in gate %s less than zero: %d %d\n", gate_type, qubit_1, qubit_2);
                return;
            }
            if(qubit_1 == qubit_2) {
                fprintf(stderr, "Error: specified qubits in gate %s are equal: %d %d\n", gate_type, qubit_1, qubit_2);
                return;
            }

            debug_printf("Parsed CNOT gate for qubits %d and %d\n", qubit_1, qubit_2);

            // Re-order qubit index
            qubit_1 = (qr->size - 1) - qubit_1;
            qubit_2 = (qr->size - 1) - qubit_2;

            // Handle non-adjacent or reverse-ordered CNOTs
            if (abs(qubit_1 - qubit_2) != 1 || qubit_1 > qubit_2) {
                if (qubit_1 > qubit_2) {
                    if(qubit_1 == qubit_2+1) {
                        // Use the reverse CNOT matrix for non-ordered control and target
                        qgate *reverse_cnot_gate = create_reverse_cnot_gate();
                        if (reverse_cnot_gate == NULL) {
                            fprintf(stderr, "Failed to generate reverse CNOT gate\n");
                            return;
                        }
                        int *qubits = malloc(2 * sizeof(int));
                        qubits[1] = qubit_1;
                        qubits[0] = qubit_2;
                        add_gate_to_list(&current_gates, reverse_cnot_gate, qubits);
                    }
                    else {
                        // Swap control and target to make them adjacent and correctly ordered
                        int swap_target = qubit_2;
                        int swap_to = qubit_1 - 1;
                        qgate *swap_gate;
                        debug_printf("Qubits %d and %d require swapping\n", swap_target, swap_to);

                        // Check if qubits are adjacent
                        if (abs(swap_target - swap_to) == 1) {
                            swap_gate = create_swap_gate();
                        } else {
                            // Generate a generic SWAP matrix and add it to the gate list
                            swap_gate = generate_generic_swap(abs(swap_target-swap_to)+1, swap_target, swap_to);
                        }
                        if (swap_gate == NULL) {
                            fprintf(stderr, "Failed to generate swap gate\n");
                            return;
                        }
                        int *qubits = malloc(2 * sizeof(int));
                        qubits[0] = swap_target;
                        qubits[1] = swap_to;
                        add_gate_to_list(&swap_gates, swap_gate, qubits);

                        // Use the reverse CNOT matrix for non-ordered control and target
                        qgate *reverse_cnot_gate = create_reverse_cnot_gate();
                        if (reverse_cnot_gate == NULL) {
                            fprintf(stderr, "Failed to generate reverse CNOT gate\n");
                            return;
                        }
                        qubits = malloc(2 * sizeof(int));
                        qubits[1] = qubit_1;
                        qubits[0] = swap_to;
                        add_gate_to_list(&current_gates, reverse_cnot_gate, qubits);
                    }
                } else {
                    // Move target qubit adjacent to control
                    int swap_target = qubit_2;
                    int swap_to = qubit_1 + 1;
                    qgate *swap_gate;

                    debug_printf("Qubits %d and %d require swapping\n", swap_target, swap_to);

                    // Check if qubits are adjacent
                    if (abs(swap_target - swap_to) == 1) {
                        swap_gate = create_swap_gate();
                    } else {
                        // Generate a generic SWAP matrix and add it to the gate list
                        swap_gate = generate_generic_swap(abs(swap_target-swap_to)+1, swap_target, swap_to);
                    }
                    if (swap_gate == NULL) {
                        fprintf(stderr, "Failed to generate swap gate\n");
                        return;
                    }
                    int *qubits = malloc(2 * sizeof(int));
                    qubits[1] = swap_target;
                    qubits[0] = swap_to;
                    add_gate_to_list(&swap_gates, swap_gate, qubits);

                    // Generate standard CNOT gate
                    qgate *cnot_gate = create_cnot_gate();
                    if (cnot_gate == NULL) {
                        fprintf(stderr, "Failed to generate CNOT gate\n");
                        return;
                    }
                    qubits = malloc(2 * sizeof(int));
                    qubits[0] = qubit_1;
                    qubits[1] = swap_to;
                    add_gate_to_list(&current_gates, cnot_gate, qubits);
                }
            } else {
                // Add standard CNOT gate to current gates if adjacent and in correct order
                qgate *cnot_gate = create_cnot_gate();
                if (cnot_gate == NULL) {
                    fprintf(stderr, "Failed to generate CNOT gate\n");
                    return;
                }
                int *qubits = malloc(2 * sizeof(int));
                qubits[0] = qubit_1;
                qubits[1] = qubit_2;
                add_gate_to_list(&current_gates, cnot_gate, qubits);
            }
        } else if (strcmp(gate_type, "X") == 0 || strcmp(gate_type, "Y") == 0 || strcmp(gate_type, "Z") == 0 ||
                   strcmp(gate_type, "H") == 0 || strcmp(gate_type, "S") == 0 || strcmp(gate_type, "T") == 0) {
            // Parse single qubit index for 1-qubit gates
            if (sscanf(op_ptr, "%d", &qubit_1) != 1) {
                fprintf(stderr, "Error parsing %s qubit\n", gate_type);
                return;
            }
            if(qubit_1 > qr->size) {
                fprintf(stderr, "Error: specified qubit in gate is greater than the circuit size: %d\n", qubit_1);
            }
            // Validate qubit index is non-negative
            if (qubit_1 < 0) {
                fprintf(stderr, "Invalid qubit index for %s gate: must be non-negative\n", gate_type);
                return;
            }

            // Re-order qubit index
            qubit_1 = (qr->size - 1) - qubit_1;

            // Create gate and add to circuit
            qgate *gate;
            if (strcmp(gate_type, "X") == 0) {
                gate = create_x_gate();
            } else if (strcmp(gate_type, "Y") == 0) {
                gate = create_y_gate();
            } else if (strcmp(gate_type, "Z") == 0) {
                gate = create_z_gate();
            } else if (strcmp(gate_type, "H") == 0) {
                gate = create_h_gate();
            } else if (strcmp(gate_type, "S") == 0) {
                gate = create_s_gate();
            } else { // T gate
                gate = create_t_gate();
            }
            if (gate == NULL) {
                fprintf(stderr, "Failed to generate %s gate\n", gate_type);
                return;
            }
            
            int *qubits = malloc(sizeof(int));
            qubits[0] = qubit_1;

            add_gate_to_list(&current_gates, gate, qubits);
        } else if (strcmp(gate_type, "RX") == 0 || strcmp(gate_type, "RY") == 0 || strcmp(gate_type, "RZ") == 0 || strcmp(gate_type, "P") == 0) {
            // Parse single qubit index and angle for rotation or phase gates
            if (sscanf(op_ptr, "%d_%lf", &qubit_1, &angle) != 2) {
                fprintf(stderr, "Error parsing %s gate\n", gate_type);
                return;
            }
            if(qubit_1 > qr->size) {
                fprintf(stderr, "Error: specified qubit in gate is greater than the circuit size: %d\n", qubit_1);
            }
            // Validate qubit index is non-negative
            if (qubit_1 < 0) {
                fprintf(stderr, "Invalid qubit index for %s gate: must be non-negative\n", gate_type);
                return;
            }

            // Re-order qubit index
            qubit_1 = (qr->size - 1) - qubit_1;

            // Create gate and add to circuit
            qgate *gate;
            if (strcmp(gate_type, "RX") == 0) {
                gate = create_rx_gate(angle);
            } else if (strcmp(gate_type, "RY") == 0) {
                gate = create_ry_gate(angle);
            } else if (strcmp(gate_type, "RZ") == 0) {
                gate = create_rz_gate(angle);
            } else { // Phase gate
                gate = create_phase_gate(angle);
            }
            if (gate == NULL) {
                fprintf(stderr, "Failed to generate %s gate\n", gate_type);
                return;
            }

            int *qubits = malloc(sizeof(int));
            qubits[0] = qubit_1;

            add_gate_to_list(&current_gates, gate, qubits);
        } else {
            fprintf(stderr, "Unsupported gate type: %s\n", gate_type);
            return;
        }

        // Move pointer to next operation
        while (*op_ptr != '\0' && *op_ptr != '|') op_ptr++;
        if (*op_ptr == '|') op_ptr++;
    }

    // Apply the gates: apply the SWAP gates if they exist, then the current gates, then the SWAP gates again
    if (swap_gates.head != NULL) {
        debug_printf("Applying swap gates before main gates:\n");
        apply_gate(qr, &swap_gates);
    }

    if (current_gates.head != NULL) {
        debug_printf("Applying main gates:\n");
        apply_gate(qr, &current_gates);
    }

    if (swap_gates.head != NULL) {
        debug_printf("Re-applying swap gates after main gates:\n");
        apply_gate(qr, &swap_gates);
    }

    // Clean up
    clear_gate_list(&swap_gates);
    clear_gate_list(&current_gates);
}

qreg *new_qreg(int size) {
    if (size > QUBIT_REGISTER_LIMIT) {
        printf("Cannot support more than %d qubits currently, attempted %d\n", QUBIT_REGISTER_LIMIT, size);
        return NULL;
    }

    // Allocate memory for the quantum register
    qreg *qr = (qreg *)malloc(sizeof(qreg));
    if (qr == NULL) {
        fprintf(stderr, "Error allocating memory for quantum register.\n");
        return NULL;
    }

    // Set the size
    qr->size = size;

    // Allocate memory for the state vector (2^size complex amplitudes)
    int num_states = 1 << size; // 2^size
    qr->amp = (cnum *)malloc(num_states * sizeof(cnum));
    if (qr->amp == NULL) {
        fprintf(stderr, "Error allocating memory for state vector.\n");
        free(qr);
        return NULL;
    }

    // Initialize all amplitudes to 0 except for the first state |00...0>
    qr->amp[0].re = 1.0;
    qr->amp[0].im = 0.0;
    for (int i = 1; i < num_states; i++) {
        qr->amp[i].re = 0.0;
        qr->amp[i].im = 0.0;
    }

    return qr;
}

void free_qreg(qreg *qr) {
    if (qr != NULL) {
        // Free the state vector
        if (qr->amp != NULL) {
            free(qr->amp);
        }
        // Free the quantum register structure itself
        free(qr);
    }
}

// Helper function to print binary representation of a basis state
static void print_binary(int num, int bits) {
#ifdef PRINT_MSB_LEFT
    for (int i = bits - 1; i >= 0; i--) {
#else
    for (int i = 0; i <= bits - 1; i++) {
#endif
        printf("%d", (num >> i) & 1);
    }
}

void view_state_vector(qreg *qr) {
    if (qr) {
        int num_states = 1 << qr->size; // 2^size

        for (int i = 0; i < num_states; i++) {
            // Get real and imaginary parts of the amplitude
            double re = qr->amp[i].re;
            double im = qr->amp[i].im;

            // Skip printing if the amplitude is zero
            if (fabs(re) < 1e-6 && fabs(im) < 1e-6) {
                continue;
            }

            // Print the amplitude with Dirac notation
            printf("(%.2f%s%.2fi)*|", re, (im >= 0) ? "+" : "", im);
            print_binary(i, qr->size); // Print binary representation of the state
            printf(">\n");
        }
    }
    else {
        fprintf(stderr, "Error trying to view state vector on a null quantum register!\n");
    }
}

// Compute the tensor product of two matrices A (dimA x dimA) and B (dimB x dimB)
cnum **tensor_product(cnum **A, int dimA, cnum **B, int dimB) {
    if(A == NULL || B == NULL) {
        fprintf(stderr, "Error trying to do tensor product with at least 1 NULL matrix\n");
        return NULL;
    }
    
    int dim_out = dimA * dimB;
    cnum **result = allocate_matrix(dim_out); // Allocate a dim_out x dim_out matrix
    if(result == NULL) {
        fprintf(stderr, "Error allocating matrix for tensor product\n");
        return NULL;
    }

    debug_printf("Computing tensor product: %dx%d ⊗ %dx%d = %dx%d\n", dimA, dimA, dimB, dimB, dim_out, dim_out);

    for (int i = 0; i < dimA; i++) {
        for (int j = 0; j < dimA; j++) {
            for (int k = 0; k < dimB; k++) {
                for (int l = 0; l < dimB; l++) {
                    int row = i * dimB + k;
                    int col = j * dimB + l;

                    // Ensure that each row and column are allocated properly
                    // printf("Setting result[%d][%d]\n", row, col);

                    result[row][col].re = A[i][j].re * B[k][l].re - A[i][j].im * B[k][l].im;
                    result[row][col].im = A[i][j].re * B[k][l].im + A[i][j].im * B[k][l].re;
                }
            }
        }
    }
    return result;
}


void print_gate_matrix(cnum **matrix, int size) {
    if(matrix == NULL) {
        fprintf(stderr, "Error printing a NULL matrix\n");
        return;
    }
#ifdef DEBUG_PRINTS
    printf("Gate matrix (%d x %d):\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // printf("(%.2f + %.2fi) ", matrix[i][j].re, matrix[i][j].im);
            // printf("%.2f ", matrix[i][j].re);
            printf("%.0f ", matrix[i][j].re);

        }
        printf("\n");
    }
#endif
}


cnum **expand_gate_matrix(qgate *gate, int num_qubits, int *target_qubits) {
    if (gate == NULL) {
        fprintf(stderr, "Error expanding a NULL gate\n");
        return NULL;
    }

    int gate_size = gate->size;  // Number of qubits the gate operates on
    cnum **expanded_matrix = NULL;

    // Start with an identity matrix for the expansion
    expanded_matrix = allocate_matrix(1);
    if (expanded_matrix == NULL) {
        fprintf(stderr, "Error allocating the first expansion matrix\n");
        return NULL;
    }
    expanded_matrix[0][0].re = 1.0; // Start with a 1x1 identity element

    int target_index = 0;
    int expanded_qubits = 0;  // Tracks the number of qubits incorporated into expanded_matrix

    for (int i = 0; i < num_qubits; i++) {
        cnum **current_matrix;
        int current_matrix_size;

        // Check if the current qubit is part of the target qubits for this gate
        if (target_index < gate_size && i == target_qubits[target_index]) {
            // Use the gate matrix for the targeted qubits
            current_matrix = gate->matrix;
            current_matrix_size = 1 << gate_size;

            // Move to the next target qubit
            target_index++;

            // Skip the rest of the target qubits covered by this multi-qubit gate
            i += (gate_size - 1);
        } else {
            // Use a 2x2 identity matrix for non-target qubits
            current_matrix_size = 2;
            current_matrix = allocate_matrix(current_matrix_size);
            if (current_matrix == NULL) {
                fprintf(stderr, "Failed to allocate identity matrix for expansion\n");
                if (expanded_matrix != NULL) {
                    free_matrix(expanded_matrix, 1 << expanded_qubits);
                }
                return NULL;
            }

            // Initialize as a 2x2 identity matrix
            current_matrix[0][0].re = 1.0;
            current_matrix[1][1].re = 1.0;
        }

        // Compute the tensor product with the current expanded matrix so far
        int expanded_size = 1 << expanded_qubits;  // Size of the expanded matrix so far
        cnum **temp_matrix = tensor_product(expanded_matrix, expanded_size, current_matrix, current_matrix_size);
        if(temp_matrix == NULL) {
            fprintf(stderr, "Error calculating tensor product\n");
            return NULL;
        }

        // Free the old expanded matrix if it wasn't the gate itself
        if (expanded_matrix != gate->matrix) {
            free_matrix(expanded_matrix, expanded_size);
        }

        // If the current matrix was an identity matrix, we need to free it
        if (current_matrix != gate->matrix) {
            free_matrix(current_matrix, current_matrix_size);
        }

        // Update the expanded matrix with the newly computed tensor product
        expanded_matrix = temp_matrix;

        // Update the number of qubits incorporated into expanded_matrix
        expanded_qubits += (current_matrix_size == 1 << gate_size) ? gate_size : 1;
    }

    return expanded_matrix;
}



cnum **multiply_matrices(cnum **A, cnum **B, int size) {
    if (A == NULL || B == NULL) {
        fprintf(stderr, "Error mulplitying matrices with at least 1 NULL matrix\n");
        return NULL;
    }

    debug_printf("Multiplying matrices of size %d x %d\n", size, size);
    cnum **result = allocate_matrix(size);
    if (result == NULL) {
        fprintf(stderr, "Failed to allocate memory for result matrix\n");
        return NULL;
    }

    // Initialize the result matrix with zero values
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j].re = 0.0;
            result[i][j].im = 0.0;
        }
    }

    // Perform matrix multiplication
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                // Multiply corresponding elements and accumulate
                double re_part = A[i][k].re * B[k][j].re - A[i][k].im * B[k][j].im;
                double im_part = A[i][k].re * B[k][j].im + A[i][k].im * B[k][j].re;

                result[i][j].re += re_part;
                result[i][j].im += im_part;
            }
        }
    }

    return result;
}



cnum **build_full_operator_matrix(gate_list *gates, int num_qubits) {
    if (gates == NULL) {
        fprintf(stderr, "Error building full operator matrix for NULL gates list\n");
        return NULL;
    }

    debug_printf("Building full operator matrix for %d qubits\n", num_qubits);
    int full_size = 1 << num_qubits;  // 2^num_qubits
    cnum **full_matrix = allocate_matrix(full_size);

    if (full_matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for full operator matrix\n");
        return NULL;
    }

    // Initialize full_matrix to be an identity matrix
    for (int i = 0; i < full_size; i++) {
        for (int j = 0; j < full_size; j++) {
            full_matrix[i][j].re = (i == j) ? 1.0 : 0.0;
            full_matrix[i][j].im = 0.0;
        }
    }

    // Iterate over all gates in the gate list
    for (gate_node *node = gates->head; node != NULL; node = node->next) {
        debug_printf("Expanding gate matrix for gate with %d qubits:\n", node->gate->size);
        print_gate_matrix(node->gate->matrix, 1 << node->gate->size);

        // Expand the gate matrix to the full system size
        cnum **expanded_matrix = expand_gate_matrix(node->gate, num_qubits, node->qubits);
        if (expanded_matrix == NULL) {
            fprintf(stderr, "Failed to expand gate matrix\n");
            free_matrix(full_matrix, full_size);
            return NULL;
        }

        debug_printf("Expanded matrix\n");
        print_gate_matrix(expanded_matrix, full_size);


        // Multiply the current full matrix with the expanded gate matrix
        cnum **temp_matrix = multiply_matrices(full_matrix, expanded_matrix, full_size);
        if (temp_matrix == NULL) {
            fprintf(stderr, "Failed to multiply matrices\n");
            free_matrix(full_matrix, full_size);
            free_matrix(expanded_matrix, full_size);
            return NULL;
        }

        // Free the old matrices after multiplication
        free_matrix(full_matrix, full_size);
        free_matrix(expanded_matrix, full_size);

        // Update the full matrix
        full_matrix = temp_matrix;
    }

    return full_matrix;
}



void apply_operator_to_state(qreg *qr, cnum **operator_matrix) {
    if (qr == NULL || operator_matrix == NULL) {
        fprintf(stderr, "Error trying to apply operator to state, with NULL inputs\n");
        return;
    }

    int num_states = 1 << qr->size; // 2^N for N qubits
    cnum *new_state = (cnum *)calloc(num_states, sizeof(cnum)); // Zero-initialize new state vector

    debug_printf("Applying operator to state vector (size: %d)\n", num_states);
    debug_printf("Address of operator_matrix: %p\n", (void *)operator_matrix);
    debug_printf("Address of qr->amp: %p\n", (void *)qr->amp);
    debug_printf("Address of new_state: %p\n", (void *)new_state);

    // Perform matrix-vector multiplication to apply the operator
    for (int i = 0; i < num_states; i++) {
        // printf("Processing new_state[%d]\n", i);  // Debug: Track each element
        for (int j = 0; j < num_states; j++) {
            // Check bounds to ensure no invalid access
            if (operator_matrix[i] == NULL || qr->amp == NULL) {
                fprintf(stderr, "Null pointer access at operator_matrix[%d] or qr->amp\n", i);
                exit(EXIT_FAILURE);
            }
            if (j >= num_states) {
                fprintf(stderr, "Out-of-bounds access at operator_matrix[%d][%d] or qr->amp[%d]\n", i, j, j);
                exit(EXIT_FAILURE);
            }

            new_state[i].re += operator_matrix[i][j].re * qr->amp[j].re - operator_matrix[i][j].im * qr->amp[j].im;
            new_state[i].im += operator_matrix[i][j].re * qr->amp[j].im + operator_matrix[i][j].im * qr->amp[j].re;
        }
        // printf("new_state[%d]: (%.2f, %.2f)\n", i, new_state[i].re, new_state[i].im); // Debug: Print each new_state entry
    }

    // Free the old state vector and update the quantum register with the new state vector
    free(qr->amp);
    qr->amp = new_state;

    debug_printf("Operator application complete\n");
}



void apply_gate(qreg *qr, gate_list *gates) {
    if (qr == NULL || gates == NULL) {
        fprintf(stderr, "Error applying gate with NULL inputs\n");
        return;
    }

    // Build the full operator matrix for this circuit layer

    cnum **operator_matrix = build_full_operator_matrix(gates, qr->size);
    if (operator_matrix == NULL) {
        fprintf(stderr, "Error obtaining full operator matrix\n");
        return;
    }
    debug_printf("Built following full matrix:\n");
    print_gate_matrix(operator_matrix, 1<<(qr->size));

    // Apply the full operator matrix to the quantum register's state vector
    apply_operator_to_state(qr, operator_matrix);

    debug_printf("State vector afterwards:\n");
#ifdef DEBUG_PRINTS
    view_state_vector(qr);printf("\n");
#endif

    // Free the full operator matrix after application
    free_matrix(operator_matrix, 1 << qr->size);
}


void circuit_layer(qreg *qr, const char *operations) {
    if (!qr) {
        fprintf(stderr, "Error trying to evaluate a circuit layer on a null quantum register\n");
        return;
    }
    if (!operations) {
        fprintf(stderr, "Error trying to evaluate a circuit layer with a null operations list\n");
        return;
    }
    // Parse the operation string and populate the gate list
    parse_circuit_layer(qr, operations);
}
