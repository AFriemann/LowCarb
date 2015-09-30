LowCarb
=======

This is a re-implemenation of the program *REACH* written in Fortran by Kei Moritsugu and Jeremy C. Smith and based and
their paper *Coarse-Grained Biomolecular Simulation with REACH* [#]_.

This re-implementation was issued by *Prof. Dr. Hamacher* and the research group for *Computational Biology and
Simulation* [#]_ at TU Darmstadt and executed by us as a university assignment.
Due to many existing problems in the *REACH* source code and further requests by said working group, several changes have
been introduced. Here we will document these problems and our workarounds, as well as additional functionality.
For detailed information on implementation, please refer to the provided source code and the documentation created via
doxygen.

REACH
-----

The vanilla state of the provided sources only works with the examples that come with the source package.
*REACH* has major issues, including errors in it's calculations, which render it practically unsuseable.
We created git patchfiles for mathemetical mistakes, however most problems could not be solved that easily.

Calculations
~~~~~~~~~~~~

* Instead of the average covariance matrix *REACH* uses the matrix of the last ensemble [#]_.
* A typo in the calculation for residue interaction resulted in faulty results.

There might be more implementation errors in parts we left out, see the section on implementation for more information.

User experience
~~~~~~~~~~~~~~~

* Input files have to be renamed to the given examples, any other filename result in an error.
* PDB specification is not completely met, which results in errors with files differing from the given example.
* DCD files can not be larger than about 2.5GB.
* Multiple given DCD files have to be of equal size.
* The amount of frames [#]_ in a DCD file is calculated instead of beeing read from the file header.

Implementation
--------------

We created a mostly object oriented Model in C++.

Calculations
~~~~~~~~~~~~

While *REACH* uses ublas calls for linear algebra, we chose Eigen [#]_. This resulted in some parts giving different
results during calculations. For example, *REACH* used a self written implementation of the Jacobi eigenvalue algorithm
which we chose to ignore due to iconsistencies in that specific part of the code.

DCD
~~~

We re-implemented read capability for DCD files according the the provided specification. Due to the lack of easy to
include C/C++ libraries this was a completely new attempt. It was tested with the provided examples.
Unfortunately there was no test data available for files in big endian. So while the capability is there, it should be
considered experimental.

Output
~~~~~~

For output creation during runtime we use Plog [#]_. This could potentially be configured to write logfiles but will
only print to stdout for now.

REACH writes several file during execution. Some of these have been deemed unnecessary by the research group so we left
them out, others have been added. We changed naming where we could to make interpretation of results easier and tried to
stay as consistent as possible. Among the ones left out were arrow files.

Output format is csv with *,* as delimiter and *"* as quote char.

New Features
------------

Some additional features have been requested by the research group.

XTC and TRR
~~~~~~~~~~~

Support for additional trajectory formats exists thanks to the Gromacs xdrfile [#]_ library.
When reading XTC or TRR files the libraries results turned out to differ from what gromacs' gmx dump read from the file
by less than 10%. This is most likely due to the reduced type size in XDR files.

NMA covariance input
~~~~~~~~~~~~~~~~~~~~

**TODO**

Model Reduction
~~~~~~~~~~~~~~~

It is possible [#]_ to reduce the protein model through projecting all residues onto a limited set.
This reduction will take place for the hessian matrix computed for the meansquare fluctuation for the entire protein.
The reduction scheme can be provided via a file, please refer to the README for specific information.

Multithreading
~~~~~~~~~~~~~~

The Eigen library uses multithreading via OpenMP. Enabling openmp will therefore run some calculations in parallel. We
tried parallelizing some for loops, however the results were rather underwhelming. Typically execution time was lower
when using only one thread and there was a linear increase in system time with increasing threads. This was most likely
due to the short life-time of single threads and could very well be done better.

Tests
-----

Unfortunately we mostly had the examples provided by *REACH* avilable for testing. We used the *REACH* source code to
create input and output data for key parts of the code and tested our implementation against those results.
While this approach is faulty at heart it was required of us to achieve a precision of 10^8 compared to *REACH* and seemed
like the best way to achieve reliable results at the time. Several issues came up once we got ahold of more test files
but since *REACH* is not able to work with other files it was hard to verify the correctness of any calculations.

Footnotes
=========

.. [#] K. Moritsugu and J.C. Smith. 2007. Coarse-Grained Biomolecular Simulation with *REACH*: Realistic Extension
   Algorithm via Covariance Hessian. Biophysical Journal, Vol 3, Nov. 2007.

.. [#] Computational Biology and Simulation: http://www.bio.tu-darmstadt.de/ag/professuren/ag_hamacher/Hamacher.en.jsp

.. [#] An ensemble is a collection of frames.

.. [#] A frame is a single data point in the protein trajectory.

.. [#] http://eigen.tuxfamily.org/

.. [#] https://github.com/SergiusTheBest/plog

.. [#] http://www.gromacs.org/Developer_Zone/Programming_Guide/XTC_Library

.. [#] J. Kim, S. Na, K. Eom. 2010. Domain Decomposition-Based Structural Condensation of Large Protein Structures
   for Understanding Their Conformational Dynamics. Wiley Online Library 2010.

