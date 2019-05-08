# tasep-cpp
Monte carlo simulator for Totally Asymmetric Simple Exclusion Process (TASEP) on a 1D open lattice.

TASEP is an ideal model in nonequilibrium statistical mechanics. It is useful in modeling a wide range of transport phenomena including road traffic and translation of mRNA in protein synthesis. 
A 1D open ended lattice is considered here. The dynamics is governed by the following set of rules:
- Particles enter from the left end with and entry rate $\alpha$. 
- Particles can only move towards the right (totally asymmetric movement).
- Particles can hop to the adjacent site on the right only if it is empty (exclusion interaction).
- Particles leave from the site at the right end with an exit rate $\beta$.


## Input parameters

* lattice-size : Number of sites on the lattice
* $\alpha$ : particle entry rate (0 < $\alpha$ < 1)
* $\beta$ - particle exit rate (0 < $\beta < 1)
* $\gamma$ - particle hopping rate (0 < $\gamma < 1)
* simulation-time : Number of timesteps to run the simulation

## Outputs

* ```config.png``` : shows the monte carlo evolution of the lattice. Each horizontal line of pixel represent one lattice configuration at a timestep. 

* Particle current : The number of particles exiting the lattice per unit time (in simulation units)

## Three phases of the TASEP
Depending on the entry and exit rates, three phases are observed for the TASEP.
- Low density phase at low entry rate ($\alpha < 0.2$) and high exit rate ($\beta > 0.2$)
- High density phase at high entry rate ($\alpha > 0.2$) and low exit rate ($\beta < 0.2$)
- Maximal current phase at high entry rate ($\alpha > 0.3$) and high exit rate ($\beta > 0.3$)

![Typical-Configurations](/configs.png)

![Phase-diagram](/phase.png)

## Compilation

### Package requirements

* CMake (>2.6)
* libpng

### Steps to compile

* Create a new directory ```mkdir build```
* Move to the new directory ```cd build```
* Run ```ccmake ..```
* Configure ```c``` and then generate and exit ```g```
* Run ```make```


