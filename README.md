# Brach-Predictor Evauation

For our project, we utilized the Champsim simulator, a powerful and versatile tool, which allowed us to accurately model and analyze various system behaviors, contributing significantly to our research objectives and findings, and enhancing our overall understanding of the subject matter.
ChampSim is a trace-based simulator for a microarchitecture study.

# Download Champsim
To download Champsim, please navigate to the following link: https://github.com/ChampSim/ChampSim
Once there, you can clone the repository to obtain the necessary files.

# Download dependencies for champsim

ChampSim uses [vcpkg](https://vcpkg.io) to manage its dependencies. In this repository, vcpkg is included as a submodule. You can download the dependencies with
```
git submodule update --init
vcpkg/bootstrap-vcpkg.sh
vcpkg/vcpkg install
```

# Compile

ChampSim takes a JSON configuration script. Examine `champsim_config.json` for a fully-specified example. All options described in this file are optional and will be replaced with defaults if not specified. The configuration scrip can also be run without input, in which case an empty file is assumed.
```
$ ./config.sh champsim_config.json
$ make
```

# Download DPC-3 trace

Traces used for the 3rd Data Prefetching Championship (DPC-3) can be found here. (https://dpc3.compas.cs.stonybrook.edu/champsim-traces/speccpu/)

# Run simulation

Execute the binary directly.
```
$ script -c "bin/champsim --warmup_instructions 200000000 --simulation_instructions 500000000 ~/Desktop/Projects/Brach-Predictor/tracer/621.wrf_s-8065B.champsimtrace.xz" ~/Desktop/Projects/Brach-Predictor/results/Original_621.wrf_s-8065B_$(date "+%Y.%m.%d-%H.%M.%S").txt

```

The number of warmup and simulation instructions provided corresponds to the count of retired instructions. It's important to note that the statistics displayed at the end of the simulation and include only the simulation phase. Additionally, these statistics are saved in a text file with an appropriate name within the 'results' folder.

# Add your own branch predictor
**Copy an empty template**
```
$ mkdir brach/mybranch
$ cp branch/bimodal/bimodal.cc branch/mybranch/mybranch.cc
```

**Work on your algorithms with your favorite text editor**
```
$ vim branch/mypref/mypref.cc
```

**Compile and test**
Add your branch predictor to the configuration file.
```
{
    "ooo_cpu": {
        "branch_predictor": "mybranch"
    }
}
```
# Compile and Run

```
$ ./config.sh champsim_config.json
$ make
$ script -c "bin/champsim --warmup_instructions 200000000 --simulation_instructions 500000000 ~/Desktop/Projects/Brach-Predictor/tracer/621.wrf_s-8065B.champsimtrace.xz" ~/Desktop/Projects/Brach-Predictor/results/Original_621.wrf_s-8065B_$(date "+%Y.%m.%d-%H.%M.%S").txt
```

# How to copy traces

Download the appropriate traces and place them within the 'tracer' folder.

