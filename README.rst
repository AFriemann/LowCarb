LowCarb
=======

With little carbo-hydrates and lots of protein!

PREREQUISITES
-------------

This project requires:

* Cross-platform Make (CMake) v2.6.2+
* GNU Make or equivalent.
* GCC or an alternative, reasonably conformant C++ compiler.

BUILDING
--------

The olden way::

    $ ./configure && make

Possible configure options are::

    DEBUG
    PROFILE
    COVERAGE
    OPTIMIZE
    SSE2
    OPENMP

of which optimization, sse and openmp are set to **ON** by default.

To make use of the python3 build script you might have to install certain
requirements::

    $ pip3 install --user -r requirements.txt

Compilation::

    $ python3 cc clean compile # use 'cc' instead of 'clean compile'

Testing::

    $ ./sync-test-data down # data required for unittests
    $ python3 cc --check-unittests --check-coupling compile

To run the coverage tests, make sure to run unittests!

RUNNING
-------

Allowed options::

    -h [ --help ]           produce help message
    -c [ --config ] PATH    config file path
    -o [ --output ] PATH    output file path
    -d [ --debug ]          print debug output

Example::

    ./bin/low-carb --config test/resources/integration/config.ini -o output

CONFIGURATION
-------------

There are several configuration options available. Example::

    [General]
    ENSEMBLE_SIZE = 2000
    SEGMENT_SIZE  = 20
    TEMPERATURE   = 120.0

    [Fitting]
    AVERAGE_BIN_LENGTH = 1.0
    MINIMUM_LENGTH = 6.0
    MAXIMUM_LENGTH = 12.0
    USE_SLOW_FITTING = true
    SLOW_MINIMUM_LENGTH = 11.0
    SLOW_MAXIMUM_LENGTH = 20.0

    [Files]
    DSSP = 1a6g.dssp
    PROTEIN = mb_ca.pdb
    TRAJECTORIES = mb_ca-1.dcd,mb_ca-2.dcd

Some values have defaults, others are not required. Please refer to the
following list for more information.

Section: General
~~~~~~~~~~~~~~~~

ENSEMBLE_SIZE::

    Type: Integer
    Default: 2000
    Required: No
    Description: Maximum number of frames per ensemble.

SEGMENT_SIZE::

    Type: Integer
    Default: 20
    Required: No
    Description: Maximum number of residues per segment.

TEMPERATURE::

    Type: Double
    Required: Yes
    Description: Temperature at which the trajectory data was accquired.

CRYSTAL_INFORMATION::

    Type: Boolean
    Default: false
    Required: Yes, if true.
    Description: If trajectory files contain crystal information.

Section: Files
~~~~~~~~~~~~~~

PROTEIN::

    Type: String
    Allowed formats: pdb
    Required: Yes
    Description: Path to the protein file (allowed formats: pdb)

TRAJECTORIES::

    Type: String
    Allowed formats: dcd, xtc
    Required: Yes, if NMA_COVARIANCE is absent.
    Description: Comma seperated list of paths to files containing trajectory
                 data.

NMA_COVARIANCE::

    Type: String
    Allowed formats: csv
    Required: No
    Description: Path to covariance input file to be used instead of trajectory
                 file(s). Please refer to nma covariance section for more
                 information.

REDUCTION::

    Type: String
    Required: No
    Description: Path to a file containing a reduction scheme. For more
                 information refer to section about reduction.

SECONDARY_STRUCTURE::

    Type: String
    Allowed formats: dssp
    Required: No
    Description: Path to a file containing secondary structure information.

Section: Fitting
~~~~~~~~~~~~~~~~

AVERAGE_BIN_LENGTH::

    Type: Double
    Default: 1.0
    Required: No
    Description: **TODO**

MINIMUM_LENGTH::

    Type: Double
    Default: 6.0
    Required: No
    Description: **TODO**

MAXIMUM_LENGTH::

    Type: Double
    Default: 12.0
    Required: No
    Description: **TODO**

SLOW_MINIMUM_LENGTH::

    Type: Double
    Default: 11.0
    Required: No
    Description: **TODO**

SLOW_MAXIMUM_LENGTH::

    Type: Double
    Default: 20.0
    Required: No
    Description: **TODO**

Section: Threading
~~~~~~~~~~~~~~~~~~

THREADS::

    Type: Integer
    Default: 0
    Required: No
    Description: Maximum number of threads where 0 is unlimited.

DYNAMIC::

    Type: Boolean
    Default: true
    Required: No
    Description: Whether to enable openmp dynamic teams.
                 Deactivating will fix the number of threads used by openmp.

EIGEN_THREADS::

    Type: Integer
    Default: 0
    Required: No
    Description: Maximum number of threads to be used by Eigen where 0 is unlimited.

Section: Logging
~~~~~~~~~~~~~~~~

LEVEL::

    Type: Integer
    Default: 4
    Choices: 0-6
    Required: No
    Description: Log output level, choose from 0-6 where 0 is no output at all
                 and 6 prints everything.

Reduction
---------

It is possible to reduce the Protein Model through projecting all Residua on
the remaining Residua. You specify the remaining Residua in a textfile.

Example::

    1,2,3,5-7,10-end

NMA Covariance input
--------------------

**TODO**

Known Issues
------------

* CSVWriter class will not show any errors if it fails to write anything
  (e.g. into a non existing folder). This will be caught by the ConfigParser.


External Libraries
------------------

* _Plog  1.0.0: https://github.com/SergiusTheBest/plog
* _Boost 1.59.0: http://www.boost.org/
* _xdrfile 1.1.4: http://www.gromacs.org/Developer_Zone/Programming_Guide/XTC_Library
* _fast_cpp_csv_parser: https://github.com/ben-strasser/fast-cpp-csv-parser

