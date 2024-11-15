This is a project for our Introduction to Quantum Computing project, where we have to implement either:
- a quantum simulation library (in a language of our choice)
- or define our own quantum circuit language (i.e. syntax) along with an interpreter that can convert it and simulate the language

In this repository I am writing from (almost) scratch a quantum simulation library, in C, with a fixed API and support for at least some standard quantum gates, such as:
- Hadamard gate H: e.g. "H_0"
- Pauli-X, Pauli-Y and Pauli-Z gates X,Y,Z: e.g. "X_1","Y_2","Z_3"
- Controlled-Not gate CNOT: e.g. "CNOT_4_5" where 4 is the control qubit and 5 is the target qubit
- CCNOT gate (Toffoli): e.g. "CCNOT_8_7_6" where 8 and 7 are are the control qubits, and 6 is the target qubit
- SWAP gate: "e.g. SWP_9_10"
- Rotation around x,y,z gates Rx, Ry, Rz: e.g. "RX_11_1.8" where 11 is the qubit, and 1.8 is the rotation value measured in radians, and further divided by two, same for RY and RZ
- Phase gate Ph: e.g. "P_12_4.2" where 12 is the qubit, and 4.2 is angle value for the desired phase change
- T and S gates (rotation around z-axis by Pi/4 and Pi/2 radians): e.g. "T_12","S_13"

The quantum simulation library has APIs for:
- Initializing an quantum register formed of N qubits (2^N complex numbers forming a state vector):
  -- example: qreg *qr = new_qreg(8);
- Defining & evaluating the transformation after applying 1,2,..,n gates (in parallel), as a simulation "step"/"layer"/"level", over the state vector:
  -- example: circuit_layer(qr, "SWP_1_2|X_0|H_6|CNOT_5_3|");
- "Measuring" the final (or really any intermediary) state:
  -- example: view_state_vector(qr);

The API could provide multiple ways of visualizing the states, right now it just supports this notation:
(0.71+0.00i)*|00>
(0.00+0.00i)*|01>
(0.00+0.00i)*|10>
(0.71+0.00i)*|11>
The output values can be viewed with the least significant qubit being either positioned on the right or left, and this is controllable through a define in qc_lib.h called PRINT_LSB_LEFT that can be added to the project.

To build & execute this project use the (TODO) Makefile's following commands:
----

TODO:
- Document how to build & run things, and how to use the API
- Add more test cases, and a way to automatically run and test things on new builds (configurable)
- Prepare some "How-To" examples and showcase in the readme
- Do proper input validation (non repeating values in multi qubit gates, numbers that fit within the circuit, etc)
- Add simple examples & organize the project into proper folders
- Add examples for some of our lab circuits, maybe even some of the more complex algorithms (e.g. Grover, Shor)
- Maybe play around with the C preprocessor to define a more "natural" syntax for using this API, (ab)using the preprocessing as a parser & interpreter, exposing (hopefully) full API functionality in a simple language for people that don't like C

Current version, v3, supports all the gates mentioned above (TODO: test the rotation, s, t & phase gates), plus SWP_Qn_Qm.
It should be stable and can be used.