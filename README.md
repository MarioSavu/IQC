This is a project for our Introduction to Quantum Computing project, where we have to implement either:
- a quantum simulation library (in a language of our choice)
- or define our own quantum circuit language (i.e. syntax) along with an interpreter that can convert it and simulate the language
---
In this repository I am writing from (almost) scratch a quantum simulation library, in C, with a fixed API and support for at least some standard quantum gates, such as:
- Hadamard gate H: e.g. "H_0"
- Pauli-X, Pauli-Y and Pauli-Z gates X,Y,Z: e.g. "X_1","Y_2","Z_3"
- Controlled-Not gate CNOT: e.g. "CNOT_4_5" where 4 is the control qubit and 5 is the target qubit
- CCNOT gate (Toffoli): e.g. "CCNOT_8_7_6" where 8 and 7 are are the control qubits, and 6 is the target qubit
- SWAP gate: "e.g. SWP_9_10"
- Rotation around x,y,z gates Rx, Ry, Rz: e.g. "RX_11_1.8" where 11 is the qubit, and 1.8 is the rotation value measured in radians, and further divided by two, same for RY and RZ
- Phase gate Ph: e.g. "P_12_4.2" where 12 is the qubit, and 4.2 is angle value for the desired phase change
- T and S gates (rotation around z-axis by Pi/4 and Pi/2 radians): e.g. "T_12","S_13"
---
The quantum simulation library has APIs for:
- Initializing an quantum register formed of N qubits (2^N complex numbers forming a state vector):
  - example: qreg *qr = new_qreg(8); (& free_qreg(qr); for when we're done with this register)
- Defining & evaluating the transformation after applying 1,2,..,n gates (in parallel), as a simulation "step"/"layer"/"level", over the state vector:
  - example: circuit_layer(qr, "SWP_1_2|X_0|H_6|CNOT_5_3|");
- "Measuring" the final (or really any intermediary) state:
  - example: view_state_vector(qr);

The API could provide multiple ways of visualizing the states, right now it just supports this notation:
(0.71+0.00i)*|00>
(0.00+0.00i)*|01>
(0.00+0.00i)*|10>
(0.71+0.00i)*|11>
The library assumed that the right-most qubit is qubit-0, and as a consequence:
- the output values can only be viewed with the least significant qubit being positioned on the right-most position
- indexing qubits will be done as such when applying gates: for example an X gate on qubit 0 in a 2 qubit register, initialized from |00>, will lead to |01>.

Note: Quantum states that have zero amplitude will not be shown when viewing the state of a register.

---
To build & execute this library & the examples, use the Makefile's following commands:
make clean - cleans the /build directory
make all - builds the library, as well as all the the example and test sources found in examples/ & tests/ by creating .elf files next to the sources  
make test - builds & runs the tests (useful for manual regression testing)

After running "make all", you can find .elf files under /tests & /examples for all the examples & tests that are currently written. 
Simply calling "make all" followed by "./examples/bell_state" will for example build & run the bell_state example.

For a quick test, just modify the contents of examples/scratchpad.c, call "make all", and run ./examples/scratchpad to run your test.

---
Current examples:
- bell_state.c - simplest example that shows how declare a quantum register, how to use the API to apply a Hadamard gate followed by a Controlled-Not Gate to generate a bell state, and how to view the final state
- grover_search.c - the most complicated example that shows a grover's search on 2 qubits, for state |01>, demonstrating both sequencial and parallel gate application & intermediary simulator state outputting
- figure_4_1.c & figure_4_3.c - example circuits from the first laboratories
- scratchpad.c - a file to used for verifying anything & developing everything

---
TODO:
- Update the build system as needed, based on the evolving folder structures (Maybe a root make file and individual Makefiles in each project/example/test?)
- Maybe play around with the C preprocessor to define a more "natural" syntax for using this API, (ab)using the preprocessing as a parser & interpreter, exposing full API functionality in a simple language for people that don't like C

