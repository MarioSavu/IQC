This is a project for our Introduction to Quantum Computing project, where we have to implement either:
- a quantum simulation library (in a language of our choice)
- or define our own quantum circuit language (i.e. syntax) along with an interpreter that can convert it and simulate the language

In this repository I am writing from (almost) scratch a quantum simulation library, in C, with a fixed API and support for at least some standard quantum gates, such as:
- Identity gate I
- Hadamard gate H
- Pauli-X, Pauli-Y and Pauli-Z gates X,Y,Z
- Controlled-Not gate CNOT
- Rotation around x,y,z gates Rx, Ry, Rz
- Phase gate Ph
It might also, at one point, implement support for the following gates:
- Possibly T and S gates (rotation around z-axis by Pi/4 and Pi/2 radians)
- Possibly CCNOT gate (or other n-qubit Toffoli gates?)

The quantum simulation library will have APIs for:
- Initializing an quantum system formed of N qubits, giving a 2*n state vector, with each element being a complex number, which represents the probability (amplitude) of the system being in that particular state (possibly through a C preprocessor define that created a static structure with 2n elements + size reference) 
- Defining & evaluating the transformation after applying 1,2,..,n gates (in parallel), as a simulation "step", over the state vector (variadic argument C function, that takes the state vector as an input, alongside 1 and up to n elements of the form "GateSymbol_qbitPos0_.._qbitPosX", such as "CX_2_1", internally does a tensor product with any missing identity matrices, and outputs a new state vector)
- Measuring the final (or really any intermediary) state (probably like shown below)

The API might provide one or more ways of visualizing the states, right now it just supports this notation:

(0.71+0.00i)*|00>

(0.00+0.00i)*|01>

(0.00+0.00i)*|10>

(0.71+0.00i)*|11>


To build & execute this project use the (TODO) Makefile's following commands:
make              <- builds & runs the project
make all          <- builds everything
make clean        <- cleans everythings
make run          <- runs the already built project (or builds & runs?)

TODO:
- Document how to build & run things, and how to use the API
- Add more test cases, and a way to automatically run and test things on new builds (configurable)
- Prepare some "How-To" examples and showcase in the readme
- Do proper input validation (non repeating values in multi qubit gates, numbers that fit within the circuit, etc)
- Add simple examples & organize the project into proper folders
- Add examples for some of our lab circuits, maybe even some of the more complex algorithms (e.g. Grover, Shor)
- Maybe play around with the C preprocessor to define a more "natural" syntax for using this API, (ab)using the preprocessing as a parser & interpreter, exposing (hopefully) full API functionality in a simple language for people that don't like C

Current version, v3, supports all the gates mentioned above (TODO: test the rotation & phase gates), plus SWP_Qn_Qm.
It should be stable and should be used.