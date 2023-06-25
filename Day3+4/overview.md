# Computational Physics and Molecular Simulation

## Computational Physics

### Session 1 (Keynote):
- **Physics Assisted Digital Twins for Complex Systems by** [Santosh Ansumali](https://www.jncasr.ac.in/faculty/ansumali)
- Molecular Simulation is interdisciplinary
- Tracking slow manifold (also look at Fast Manifold, only briefly mentioned)
    - Tracks extrapolation as opposed to interpolation
    - "Implicit method solver" employ Linear Algebra and hence slow
    - "Explicit method solver" employ solutions like First Order Euler Solution
    - " *HIRES: High Irradiance Response Model From Plant Physiology* "
- Computational Challenges in Fluid Dynamics(Sir's field)
    - How to parallelize the non linear computations?
    - How to scale down to smaller scale(more precise/granular particles/units) from a larger scale.
        
|Reynolds Number|Computational Power Needed|Example of System|
|---|---|---|
|10^3|TeraFLOPS|Pipe flow(as in liquid in pipes)|
|10^6|PetaFLOPS|Blood Flow in arteries and veins|
|10^9|ExaFLOPS|Automobiles|
    
    - What is currently done for the previous challenge?
        - Compute large scale "nodes"(less fine granularity of particle chosen)
        - Crude, but good enough for some practical applications
        - Important regardless, to make it better finer and smaller scale nodes 
- Current work done at SankhyaSutra:
    - Calculating trajectory and hence deployment parameters for a UAV in a cradle launched from a C-130
        - Re = 100000
        - Mach = 0.2
        - Payload -> Cuboid Shaped = 7200Kg
    - Virtual Anagram Solver
        - Digital Twin for Cardiovascular Diagnosis Protocol.
    
|Current method|Drawback|
|---|---|
|ECG & TMT|Limited Info|
|CT-Scan|Only structural information, not Functional information|
|Angiogram|Functional, but invasive|
|Invasive FFR|Finding FFR Pressure functional but invasice|
        
        - Digital twin would allow for simulating clot in artery by reconstructing the structural informatoin from CT-Scans
    - Pseudo Random Generator (?Merceny Twister?)
    - Paper on *"Molecular Dice"* which talks about Maxwell's Demon being a RNG. Mentioned some "computational trick" provided in paper.

### Session 2 & 3:
- Theory of Computational Physics by [Dr.Vishwas V](https://old.iitpkd.ac.in/people/vishwas)
- Numerical Methods:
    - Mathematical schemes to solve number based problems
    - Integratoin
    - Root Finding
    - LA
    - ...
- Numerical Simulations:
    - Physical experiments simulated In-Silico (entirely on computers)
- Some problems to simulate:
    |Brownian Motion|Single Core|10-100MB RAM|Minutes to Hours|
    |Nucleation|10-100 cores| 100MB to few GBs | Hours to Years|
    |Membrane Engulfment|100 cores|100MB|Days to weeks|
    |Mechanical Properties| 100-500 Cores|100MB to few GBs|Days to Months|
    |Rupturing Dynamics|100 Cores|100MB to few GBs| Hours to Days|
    |High Memory Jobs(eg:Hessian Calculation)|100GB to few TBs|Days|

> Note: Values for memory and time are rudimentary, just for an idea.
- Idea behind Molecular Dynamics Simulation
    - Mapping physical experiements to counterparts in computer simulations to perform in-silico experiments/simulatoins
    - **Modelling:** Identifying a *Lenscale* and *Timescale* for the simualation and what constitutes the System, State, Evolution and Behaviour in a computer simulations
        - *System:* Identifyin the system we are trying to simulate
        - *State:* Discussed "Ensemble theory", Microstates, Ensemble Average, and Initialization of these states.
            - Heavy stuff, refer to [Prof Notes](./Prof_Notes/)
        - *Evolution:* More heavy and fancy mathematical stuff(Verlet Algorithm), refer [Prof Notes](./Prof_Notes/)
        - *Behaviour:* Finally running the simuation and understanding the evolution of the System.
- Reduced Units:
    - Ties in with definig and Lenscale and Timescale for the simulation.Reduced units for the system would be:
        - Mass = Mass of particle
        - Length = Particle diameter 
        - Energy = Interaction energy

> Physics Is Hard. Math is Harder :skull:

### Session 4:
- Hands On 
    - All Code avaialble in [MD](./MD/) directory in current directory.

        

