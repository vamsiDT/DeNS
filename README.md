# DeNS : Deterministic Networks simulator 

DeNS is a **De**terministic **N**etworks **S**imulator, based on [OMNET++](http://omnetpp.org/) discrete event simulator, [INET](https://inet.omnetpp.org/) framework and depends on [NeSTiNg](https://omnetpp.org/download-items/NeSTiNg.html) TSN package for clock and transmission selection modules at link-layer.

# Getting Started

* Download and install OMNeT++ version 5.5.1. ([Installation guide](https://omnetpp.org/documentation/))
* ``cd`` to your OMNeT++ directory and ``$ source setenv`` to add omnetpp to ``PATH``
* Download [INET version 4.1.0](https://inet.omnetpp.org/Download.html). Alternatively, clone the INET repository and checkout tag v4.1.0 ``$ git clone --branch v4.1.0 --depth 1 https://github.com/inet-framework/inet.git``
* Clone NeSTiNg resposity ``$ git clone https://gitlab.com/ipvs/nesting.git``
* Clone DeNS reposity ``$ git clone https://github.com/vamsiDT/DeNS.git``
Finally your directory should like:
```
<workspace>
|___ INET
|___ detnetmod
|___ nesting
```

# Import
* Launch OMNet++ IDE and set the workspace to the directory where packages have been downloaded in the previous step.
* Import ``DeNS``, ``INET`` and ``NeSTiNg`` packages into your workspace.
    * ``File -> Import``
    * select ``Existing Projects into Workspace`` option and ``Next>``
    * under ``Select root directory`` add your previously chosen workspace directory
    * select INET, detnetmod and nesting projects and ``Finish``

All the three projects should now appear in the Project Explorer

# Build
* First, right click detnetmod project and click on ``Properties``
* Under ``Project References``, select INET and nesting, ``Apply and Close`` (__*Important*__)
* Each project can be set to ``release`` or ``debug`` build by right click and ``Build Configurations -> Set Active``
* Right click each project (in the order INET, NeSTiNg, DeNS) and ``Build Project``

# Examples
* Right click ``detnetmod/simulations/experiments_l3/dens_sc2/paper2.ini`` [paper2.ini](https://github.com/vamsiDT/DeNS/blob/master/simulations/experiments_l3/dens_sc2/paper2.ini) and ``Run As -> OMNeT++ Simulation``. This opens the graphical interface for the simulation
* [paper2.ini](https://github.com/vamsiDT/DeNS/blob/master/simulations/experiments_l3/dens_sc2/paper2.ini) contains the configurations used in the paper. Certain knowledge about ini configuration in OMNeT++ is required and helps in easy understanding of paper2.ini and navation into different modules used in configuration. Further details on modules and configuration files will be opened soon through Wiki page in this repository.