#include "qc_lib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

// TODO: 
// - just mset the statically declared complex number matrixes to 0 after declaration
// - and maybe have a setter API to pass a value to a specific index & part (real or imaginary)


qbit X_gate(qbit in) {
    cnum gate_x[2][2];
    gate_x[0][0].real = 0;
    gate_x[0][0].imaginary = 0;

    gate_x[0][1].real = 1;
    gate_x[0][1].imaginary = 0;

    gate_x[1][0].real = 1;
    gate_x[1][0].imaginary = 0;

    gate_x[1][1].real = 0;
    gate_x[1][1].imaginary = 0;

    return matrix22_qbit_product(gate_x, in);
}

qbit Y_gate(qbit in) {
    cnum gate_y[2][2];
    gate_y[0][0].real = 0;
    gate_y[0][0].imaginary = 0;

    gate_y[0][1].real = 0;
    gate_y[0][1].imaginary = -1;

    gate_y[1][0].real = 0;
    gate_y[1][0].imaginary = 1;

    gate_y[1][1].real = 0;
    gate_y[1][1].imaginary = 0;

    return matrix22_qbit_product(gate_y, in);
}

qbit Z_gate(qbit in) {
    cnum gate_z[2][2];
    gate_z[0][0].real = 1;
    gate_z[0][0].imaginary = 0;

    gate_z[0][1].real = 0;
    gate_z[0][1].imaginary = 0;

    gate_z[1][0].real = 0;
    gate_z[1][0].imaginary = 0;

    gate_z[1][1].real = -1;
    gate_z[1][1].imaginary = 0;

    return matrix22_qbit_product(gate_z, in);
}

qbit H_gate(qbit in) {
    cnum gate_h[2][2];

    double scalar = 1.0 / sqrt(2);

    gate_h[0][0].real = scalar;
    gate_h[0][0].imaginary = 0;

    gate_h[0][1].real = scalar;
    gate_h[0][1].imaginary = 0;

    gate_h[1][0].real = scalar;
    gate_h[1][0].imaginary = 0;

    gate_h[1][1].real = -scalar;
    gate_h[1][1].imaginary = 0;

    return matrix22_qbit_product(gate_h, in);
}

qbit2 CNOT_gate(qbit ctrl_in, qbit target_in) {
    qbit out;
    cnum gate_cnot[4][4];

    gate_cnot[0][0].real = 1;
    gate_cnot[0][0].imaginary = 0;

    gate_cnot[0][1].real = 0;
    gate_cnot[0][1].imaginary = 0;

    gate_cnot[0][2].real = 0;
    gate_cnot[0][2].imaginary = 0;

    gate_cnot[0][3].real = 0;
    gate_cnot[0][3].imaginary = 0;

    gate_cnot[1][0].real = 0;
    gate_cnot[1][0].imaginary = 0;

    gate_cnot[1][1].real = 1;
    gate_cnot[1][1].imaginary = 0;

    gate_cnot[1][2].real = 0;
    gate_cnot[1][2].imaginary = 0;

    gate_cnot[1][3].real = 0;
    gate_cnot[1][3].imaginary = 0;

    gate_cnot[2][0].real = 0;
    gate_cnot[2][0].imaginary = 0;

    gate_cnot[2][1].real = 0;
    gate_cnot[2][1].imaginary = 0;

    gate_cnot[2][2].real = 0;
    gate_cnot[2][2].imaginary = 0;

    gate_cnot[2][3].real = 1;
    gate_cnot[2][3].imaginary = 0;

    gate_cnot[3][0].real = 0;
    gate_cnot[3][0].imaginary = 0;

    gate_cnot[3][1].real = 0;
    gate_cnot[3][1].imaginary = 0;

    gate_cnot[3][2].real = 1;
    gate_cnot[3][2].imaginary = 0;

    gate_cnot[3][3].real = 0;
    gate_cnot[3][3].imaginary = 0;

    // Calculate the tensor (Kronecker) product in-between the control & target qubits
    qbit2 ctrl_target_product;
    ctrl_target_product.amp[0] = cnum_product(ctrl_in.zero, target_in.zero);
    ctrl_target_product.amp[1] = cnum_product(ctrl_in.zero, target_in.one);
    ctrl_target_product.amp[2] = cnum_product(ctrl_in.one,  target_in.zero);
    ctrl_target_product.amp[3] = cnum_product(ctrl_in.one,  target_in.one);
    
    // printf("CNOT's control & target tensor product:\n"
    //     "(%.2f%s%.2fi)*|00>\n"
    //     "(%.2f%s%.2fi)*|01>\n"
    //     "(%.2f%s%.2fi)*|10>\n"
    //     "(%.2f%s%.2fi)*|11>\n\n",
    //     ctrl_target_product.amp[0].real, (ctrl_target_product.amp[0].imaginary>=0)?"+":"",ctrl_target_product.amp[0].imaginary,
    //     ctrl_target_product.amp[1].real, (ctrl_target_product.amp[1].imaginary>=0)?"+":"",ctrl_target_product.amp[1].imaginary,
    //     ctrl_target_product.amp[2].real, (ctrl_target_product.amp[2].imaginary>=0)?"+":"",ctrl_target_product.amp[2].imaginary,
    //     ctrl_target_product.amp[3].real, (ctrl_target_product.amp[3].imaginary>=0)?"+":"",ctrl_target_product.amp[3].imaginary);

    qbit2 computation_output = matrix44_qbit2_product(gate_cnot, ctrl_target_product);

    // Here would be a great place to differentiate in-between how to simulate this.
    // I can either just take the last 2 amplitudes that correspond to the target qubit which could get modified, and just return that as a qbit
    // or I could just return the whole qbit2 computation output, since these bits are probably entangled now (but I still have no clue about entanglement)
    // It should be easy to implement both, I'd just need to add a print_qbit2 and some helpers to take out either the first or 2nd qubit state of a 2 qubit state (Get either |a> or |b> from |ab>) 
    // printf("CNOT full output:\n"
    //     "(%.2f%s%.2fi)*|00>\n"
    //     "(%.2f%s%.2fi)*|01>\n"
    //     "(%.2f%s%.2fi)*|10>\n"
    //     "(%.2f%s%.2fi)*|11>\n\n",
    //     computation_output.amp[0].real, (computation_output.amp[0].imaginary>=0)?"+":"",computation_output.amp[0].imaginary,
    //     computation_output.amp[1].real, (computation_output.amp[1].imaginary>=0)?"+":"",computation_output.amp[1].imaginary,
    //     computation_output.amp[2].real, (computation_output.amp[2].imaginary>=0)?"+":"",computation_output.amp[2].imaginary,
    //     computation_output.amp[3].real, (computation_output.amp[3].imaginary>=0)?"+":"",computation_output.amp[3].imaginary);
       
    // // For now, let's take out just the target amplitude and return that
    // out.zero = cnum_copy(computation_output.amp[2]);
    // out.one = cnum_copy(computation_output.amp[3]);

    // return out;

    return computation_output;
}

// qbit CCNOT_gate(qbit ctrl_0_in, qbit ctrl_1_in, qbit target_in) {

// }

qbit q_zero() {
    qbit out;
    out.zero.real = 1;
    out.zero.imaginary = 0;
    out.one.real = 0;
    out.one.imaginary = 0;

    return out;
}

qbit q_one() {
    qbit out;
    out.zero.real = 0;
    out.zero.imaginary = 0;
    out.one.real = 1;
    out.one.imaginary = 0;

    return out;
}

void print_qbit_short(qbit in) {
    printf("(%.2f%s%.2fi)*|0> + (%.2f%s%.2fi)*|1>", 
        in.zero.real, (in.zero.imaginary>=0)?"+":"", in.zero.imaginary,
        in.one.real, (in.one.imaginary>=0)?"+":"",in.one.imaginary);
}

void print_qbit2_short(qbit2 in) {
    printf("(%.2f%s%.2fi)*|00>\n"
        "(%.2f%s%.2fi)*|01>\n"
        "(%.2f%s%.2fi)*|10>\n"
        "(%.2f%s%.2fi)*|11>\n\n",
        in.amp[0].real, (in.amp[0].imaginary>=0)?"+":"",in.amp[0].imaginary,
        in.amp[1].real, (in.amp[1].imaginary>=0)?"+":"",in.amp[1].imaginary,
        in.amp[2].real, (in.amp[2].imaginary>=0)?"+":"",in.amp[2].imaginary,
        in.amp[3].real, (in.amp[3].imaginary>=0)?"+":"",in.amp[3].imaginary);       
}

void print_qbit(qbit in) {
    printf("QBit state: (%.2f%s%.2fi)*|0> + (%.2f%s%.2fi)*|1>\n", 
        in.zero.real, (in.zero.imaginary>=0)?"+":"", in.zero.imaginary,
        in.one.real, (in.one.imaginary>=0)?"+":"",in.one.imaginary);
}

static cnum cnum_copy(cnum in) {
    cnum out;

    out.real = in.real;
    out.imaginary = in.imaginary;

    return out;
} 

static cnum cnum_sum2(cnum one, cnum two) {
    cnum out;

    out.real = one.real + two.real;
    out.imaginary = one.imaginary + two.imaginary;

    return out;
}

static cnum cnum_sum4(cnum one, cnum two, cnum three, cnum four) {
    cnum out;

    out.real = one.real + two.real + three.real + four.real;
    out.imaginary = one.imaginary + two.imaginary + three.imaginary + four.imaginary;

    return out;
}

static cnum cnum_product(cnum one, cnum two) {
    cnum out;

    out.real = (one.real * two.real) - (one.imaginary * two.imaginary);
    out.imaginary = (one.real * two.imaginary) + (one.imaginary * two.real);
    
    return out;
}

static qbit matrix22_qbit_product(cnum gate[2][2], qbit in) {
    qbit out;

    out.zero = cnum_sum2(cnum_product(gate[0][0], in.zero), cnum_product(gate[0][1], in.one));
    out.one = cnum_sum2(cnum_product(gate[1][0], in.zero), cnum_product(gate[1][1], in.one));

    return out;
}

static qbit2 matrix44_qbit2_product(cnum gate[4][4], qbit2 in) {
    qbit2 out;

    for (int i = 0; i < 4; i++) {
        out.amp[i] = cnum_sum4(
            cnum_product(gate[i][0], in.amp[0]),
            cnum_product(gate[i][1], in.amp[1]),
            cnum_product(gate[i][2], in.amp[2]),
            cnum_product(gate[i][3], in.amp[3]));
    }

    return out;
}
