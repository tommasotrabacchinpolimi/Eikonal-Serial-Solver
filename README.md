# Eikonal 
Eikonal implementation for the Advanced Methods for Scientific Computing (AMSC) Course @Polimi

**Students**:
- [Sabrina Cesaroni](https://github.com/SabrinaCesaroni)
- [Melanie Tonarelli](https://github.com/melanie-t27)
- [Tommaso Trabacchin](https://github.com/tommasotrabacchinpolimi) 

## Introduction
An Eikonal equation is a non-linear first-order partial differential equation 
that is encountered in problems of wave propagation. <br>

An Eikonal equation is one of the form:

$$\begin{cases} 
H(x, \nabla u(x)) = 1 & \quad x \in \Omega \subset \mathbb{R}^d \\  
u(x) = g(x) & \quad x \in \Gamma \subset \partial\Omega 
\end{cases} $$

where 
- $d$ is the dimension of the problem, either 2 or 3;
- $u$ is the eikonal function, representing the travel time of a wave;
- $\nabla u(x)$ is the gradient of $u$, a vector that points in the direction of the wavefront;
- $H$ is the Hamiltonian, which is a function of the spatial coordinates $x$ and the gradient $\nabla u$;
- $\Gamma$ is a set smooth boundary conditions.

In most cases, 
$$H(x, \nabla u(x)) = |\nabla u(x)|_{M} = \sqrt{(\nabla u(x))^{T} M(x) \nabla u(x)}$$
where $M(x)$ is a symmetric positive definite function encoding the speed information on $\Omega$. <br> 
In the simplest cases, $M(x) = c^2 I$ therefore the equation becomes:

$$\begin{cases}
|\nabla u(x)| = \frac{1}{c} & \quad x \in \Omega \\  
u(x) = g(x) & \quad x \in \Gamma \subset \partial\Omega
\end{cases}$$

where $c$ represents the celerity of the wave.

## Description

`src` is a library for the computation of the numerical solution of the Eikonal equation described in the introduction paragraph. <br>
The library contains:
- `Mesh` which is a class that represents a mesh in N-D (where N is eihter 2 or 3). The class is inherited by two classes: `TriangularMesh` and `TetrahedricalMesh`;
- `EikonalSolver` is an interface for the two implementation of the solver: `SerialEikonalSolver` and `ParallelEikonalSolver`.

The `SerialEikonalSolver` and `ParallelEikonalSolver` are based on the algorithms described in the [Fu et al. 2011 paper](bibliography/FuEtAl-2011-AFastIterativeMethodForSolvingTheEikonalEqOnTriangulatedSurfaces.pdf) and [Fu et al. 2013 paper](bibliography/FuEtAl-2013-AFastIterativeMethodForSolvingTheEikonalEqOnTethrahedralMeshes.pdf). <br>

The parallelisation was carried out using OpenMP.

## Usage
To build the executable, from the root folder run the following commands:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
An executable for each test will be created into `/build`, and can be executed through:
```bash
$ ./test_name input-filename num-threads output_type output-filename
```
where:
- `test_name` is either `triangular`, `triangulated` or `tetrahedrical` depending on the physical dimensione of the mesh and its type;
- `input-filename` is the input file path where the mesh will be retrieved. The program only accepts file in vtk format.
- `num-threads` is the number of threads used in the parallel algorithm.
- `output_type` should be set to `s` to save the serial solver output or to `p` to save the parallel solver output into the output file.
- `output-filename` is the name of the output file. The file will be located in the folder `test/output_meshes`.

However, these are only examples. To fully exploit our library, it should be directly used in code to access further 
features, such as the possibility to modify the velocity matrix and the boundary conditions (which in our example are 
defaulted respectively to the identity matrix and the vertex nearest to the origin).

We have already provided some meshes in the folder `test/input_meshes`.<br> 
One example is:
```bash
$ ./triangulated ../test/input_meshes/triangulated/bunny.vtk 4 s output-bunny
```
will execute both the serial and the parallel algorithm on the well-known Stanford Bunny test model and will save the serial 
output into the file `output-bunny`.

## Results
After extensive testing, we have been able to conclude that the output from the serial solver is correct as well as the
parallel solver. The parallel solver has shown excellent scaling properties. <br>

The following are some examples visualized using Paraview:
<figure>
    <img src="images/square-two.png" width="400"
         alt="Square, two sources">
    <figcaption>2D square with two wave sources.</figcaption>
</figure>

<figure>
    <img src="images/bunny.png" width="400"
         alt="Square, two sources">
    <figcaption>Stanford Bunny model.</figcaption>
</figure>

<figure>
    <img src="images/lucy.png" width="700"
         alt="Square, two sources">
    <figcaption> Lucy model with one wave source.</figcaption>
</figure>

<figure>
    <img src="images/cube.png" width="400"
         alt="Cube, two sources">
    <figcaption>3D cube with two wave sources.</figcaption>
</figure>

<br>
