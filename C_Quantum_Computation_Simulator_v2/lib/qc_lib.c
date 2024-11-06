#include "qc_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


// Helper function to allocate a 2D matrix of complex numbers
cnum **allocate_matrix(int size) {
    cnum **matrix = (cnum **)malloc(size * sizeof(cnum *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (cnum *)calloc(size, sizeof(cnum)); // Zero-initialize each element
    }
    return matrix;
}

// Free a matrix of complex numbers
void free_matrix(cnum **matrix, int dim) {
    for (int i = 0; i < dim; i++) {
        free(matrix[i]);
    }
    free(matrix);
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
    matrix[0][0] = (cnum){1, 0}; // 1 + 0i
    matrix[1][1] = (cnum){1, 0}; // 1 + 0i
    return matrix;
}

qgate *create_x_gate() {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "X", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    gate->matrix[0][1] = (cnum){1, 0};
    gate->matrix[1][0] = (cnum){1, 0};
    return gate;
}

qgate *create_y_gate() {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "Y", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    gate->matrix[0][1] = (cnum){0, -1};
    gate->matrix[1][0] = (cnum){0, 1};
    return gate;
}

qgate *create_z_gate() {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "Z", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][1] = (cnum){-1, 0};
    return gate;
}

qgate *create_hadamard_gate() {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "H", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    double inv_sqrt2 = 1.0 / sqrt(2);
    gate->matrix[0][0] = (cnum){inv_sqrt2, 0};
    gate->matrix[0][1] = (cnum){inv_sqrt2, 0};
    gate->matrix[1][0] = (cnum){inv_sqrt2, 0};
    gate->matrix[1][1] = (cnum){-inv_sqrt2, 0};
    return gate;
}

qgate *create_cnot_gate() {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "CNOT", sizeof(gate->type));
    gate->size = 2;
    gate->matrix = allocate_matrix(4);
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][1] = (cnum){1, 0};
    gate->matrix[2][3] = (cnum){1, 0};
    gate->matrix[3][2] = (cnum){1, 0};
    return gate;
}

qgate *create_swap_gate() {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "SWP", sizeof(gate->type));
    gate->size = 2;
    gate->matrix = allocate_matrix(4);
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][2] = (cnum){1, 0};
    gate->matrix[2][1] = (cnum){1, 0};
    gate->matrix[3][3] = (cnum){1, 0};
    return gate;
}

qgate *create_rx_gate(double theta) {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "RX", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    double cos_half_theta = cos(theta / 2);
    double sin_half_theta = sin(theta / 2);
    gate->matrix[0][0] = (cnum){cos_half_theta, 0};
    gate->matrix[0][1] = (cnum){0, -sin_half_theta};
    gate->matrix[1][0] = (cnum){0, -sin_half_theta};
    gate->matrix[1][1] = (cnum){cos_half_theta, 0};
    return gate;
}

qgate *create_ry_gate(double theta) {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "RY", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    double cos_half_theta = cos(theta / 2);
    double sin_half_theta = sin(theta / 2);
    gate->matrix[0][0] = (cnum){cos_half_theta, 0};
    gate->matrix[0][1] = (cnum){-sin_half_theta, 0};
    gate->matrix[1][0] = (cnum){sin_half_theta, 0};
    gate->matrix[1][1] = (cnum){cos_half_theta, 0};
    return gate;
}

qgate *create_rz_gate(double theta) {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "RZ", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    double half_theta = theta / 2;
    gate->matrix[0][0] = (cnum){cos(half_theta), -sin(half_theta)};
    gate->matrix[1][1] = (cnum){cos(half_theta), sin(half_theta)};
    return gate;
}

qgate *create_phase_gate(double phi) {
    qgate *gate = malloc(sizeof(qgate));
    strncpy(gate->type, "P", sizeof(gate->type));
    gate->size = 1;
    gate->matrix = allocate_matrix(2);
    gate->matrix[0][0] = (cnum){1, 0};
    gate->matrix[1][1] = (cnum){cos(phi), sin(phi)};
    return gate;
}

qreg *new_qreg(int size) {
    // Allocate memory for the quantum register
    qreg *qr = (qreg *)malloc(sizeof(qreg));
    if (qr == NULL) {
        printf("Error allocating memory for quantum register.\n");
        return NULL;
    }

    // Set the size
    qr->size = size;

    // Allocate memory for the state vector (2^size complex amplitudes)
    int num_states = 1 << size; // 2^size
    qr->amp = (cnum *)malloc(num_states * sizeof(cnum));
    if (qr->amp == NULL) {
        printf("Error allocating memory for state vector.\n");
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
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

void view_state_vector(qreg *qr) {
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

// Compute the tensor product of two matrices A (dimA x dimA) and B (dimB x dimB)
cnum **tensor_product(cnum **A, int dimA, cnum **B, int dimB) {
    int dim_out = dimA * dimB;
    cnum **result = allocate_matrix(dim_out); // Allocate a dim_out x dim_out matrix

    // printf("Computing tensor product: %dx%d ⊗ %dx%d = %dx%d\n", dimA, dimA, dimB, dimB, dim_out, dim_out);

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


void parse_circuit_layer(gate_list *gates, const char *operations) {
    char *ops_copy = strdup(operations);
    char *token = strtok(ops_copy, "|");

    while (token != NULL) {
        char gate_type[10];
        int angle = 0;
        int qubit1 = 0, qubit2 = -1;

        if (sscanf(token, "CNOT_%d_%d", &qubit1, &qubit2) == 2) {
            qgate *gate = create_cnot_gate();
            int *qubits = malloc(2 * sizeof(int));
            qubits[0] = qubit1;
            qubits[1] = qubit2;
            add_gate_to_list(gates, gate, qubits);

        } else if (sscanf(token, "SWP_%d", &qubit1) == 1) {
            qgate *gate = create_swap_gate();
            int *qubits = malloc(2 * sizeof(int));
            qubits[0] = qubit1;
            qubits[1] = qubit1 + 1;
            add_gate_to_list(gates, gate, qubits);

        } else if (sscanf(token, "%[^_]_%d", gate_type, &qubit1) == 2) {
            qgate *gate = NULL;
            if (strcmp(gate_type, "X") == 0) gate = create_x_gate();
            else if (strcmp(gate_type, "Y") == 0) gate = create_y_gate();
            else if (strcmp(gate_type, "Z") == 0) gate = create_z_gate();
            else if (strcmp(gate_type, "H") == 0) gate = create_hadamard_gate();
            if (gate != NULL) {
                int *qubits = malloc(sizeof(int));
                qubits[0] = qubit1;
                add_gate_to_list(gates, gate, qubits);
            }
        } else if (sscanf(token, "RX_%d_%d", &angle, &qubit1) == 2) {
            qgate *gate = create_rx_gate(angle * M_PI / 180.0);
            int *qubits = malloc(sizeof(int));
            qubits[0] = qubit1;
            add_gate_to_list(gates, gate, qubits);

        } else if (sscanf(token, "RY_%d_%d", &angle, &qubit1) == 2) {
            qgate *gate = create_ry_gate(angle * M_PI / 180.0);
            int *qubits = malloc(sizeof(int));
            qubits[0] = qubit1;
            add_gate_to_list(gates, gate, qubits);

        } else if (sscanf(token, "RZ_%d_%d", &angle, &qubit1) == 2) {
            qgate *gate = create_rz_gate(angle * M_PI / 180.0);
            int *qubits = malloc(sizeof(int));
            qubits[0] = qubit1;
            add_gate_to_list(gates, gate, qubits);

        } else if (sscanf(token, "P_%d_%d", &angle, &qubit1) == 2) {
            qgate *gate = create_phase_gate(angle * M_PI / 180.0);
            int *qubits = malloc(sizeof(int));
            qubits[0] = qubit1;
            add_gate_to_list(gates, gate, qubits);
        }

        token = strtok(NULL, "|");
    }

    free(ops_copy);
}


// Function to build the full operator matrix for the circuit layer
cnum **build_full_operator_matrix(gate_list *gates, int num_qubits) {
    // Start with a 1x1 identity matrix
    cnum **full_matrix = allocate_matrix(1);
    full_matrix[0][0] = (cnum){1, 0};
    int full_dim = 1; // Start with 1x1

    for (int q = 0; q < num_qubits; q++) {
        cnum **current_matrix = NULL;
        int matrix_dim = 2; // Default to 2x2 for single-qubit gates

        // printf("Processing qubit %d\n", q);

        // Check if there's a gate for this qubit or a two-qubit gate involving this and the next qubit
        gate_node *node = gates->head;
        while (node != NULL) {
            if (node->gate->size == 1 && node->qubits[0] == q) {
                current_matrix = node->gate->matrix;
                matrix_dim = 2;
                // printf("Single-qubit gate found on qubit %d\n", q);
                break;
            } else if (node->gate->size == 2 && node->qubits[0] == q && node->qubits[1] == q + 1) {
                current_matrix = node->gate->matrix;
                matrix_dim = 4;
                // printf("Two-qubit gate found on qubits %d and %d\n", q, q + 1);
                q++; // Skip the next qubit as it's part of this two-qubit gate
                break;
            }
            node = node->next;
        }

        // If no gate found, use an identity matrix for this qubit
        if (current_matrix == NULL) {
            // printf("No gate found on qubit %d, using identity\n", q);
            current_matrix = create_identity_matrix();
            matrix_dim = 2;
        }

        // printf("Tensor product of dimensions %dx%d and %dx%d\n", full_dim, full_dim, matrix_dim, matrix_dim);
        cnum **new_full_matrix = tensor_product(full_matrix, full_dim, current_matrix, matrix_dim);

        // Free the old full matrix and update with the new one
        free_matrix(full_matrix, full_dim);
        full_matrix = new_full_matrix;
        full_dim *= matrix_dim; // Correctly update full_dim for new size
    }

    // printf("Final full_matrix dimension: %dx%d\n", full_dim, full_dim); // Expected to be 16x16 for a 4-qubit system
    return full_matrix;
}


void apply_operator_to_state(qreg *qr, cnum **operator_matrix) {
    int num_states = 1 << qr->size; // 2^N for N qubits
    cnum *new_state = (cnum *)calloc(num_states, sizeof(cnum)); // Zero-initialize new state vector

    // printf("Applying operator to state vector (size: %d)\n", num_states);
    // printf("Address of operator_matrix: %p\n", (void *)operator_matrix);
    // printf("Address of qr->amp: %p\n", (void *)qr->amp);
    // printf("Address of new_state: %p\n", (void *)new_state);

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

    // printf("Operator application complete\n");
}



void apply_gate(qreg *qr, gate_list *gates) {
    // Build the full operator matrix for this circuit layer

    cnum **operator_matrix = build_full_operator_matrix(gates, qr->size);

    // Apply the full operator matrix to the quantum register's state vector
    apply_operator_to_state(qr, operator_matrix);

    // Free the full operator matrix after application
    free_matrix(operator_matrix, 1 << qr->size);
}


void circuit_layer(qreg *qr, const char *operations) {
    gate_list gates;
    init_gate_list(&gates);

    // Parse the operation string and populate the gate list
    parse_circuit_layer(&gates, operations);

    // Apply the gates to the quantum register
    apply_gate(qr, &gates);

    // Clear the gate list to free memory
    clear_gate_list(&gates);
}
