# PYTHIA and EVTGEN for X(3872) analysis

This part of the repository contains the code to generate the Monte Carlo samples for the X(3872) analysis. The code is based on the PYTHIA event generator and the EVTGEN decay package. But the detector simulation is not included in this part. Which means that the output of this code can only be used for Acceptance level studies.

## Environment setup

the dependences of EVTGEN are include in this link: https://cernbox.cern.ch/s/s9aLhda6ydnqgAo

You need to download it to your local by using this command:

```bash
wget https://cernbox.cern.ch/remote.php/dav/public-files/s9aLhda6ydnqgAo/dependence.tar.gz

tar -xzvf dependence.tar.gz
```

Then you need to build the EVTGEN library, before you run the setupEvtGen.sh script, you need to modify the path of the dependence in the script. And make sure you have installed some dependences(like: gcc, gfortran, cmake, etc.) in your system.:

```bash
./setupEvtGen.sh
```
If you have any problem with the installation, please let me know.

## Generate the MC samples

After you have installed the EVTGEN library, you need write a Init.sh script to setup the environment variables. Such as the file in the PYTHIA_EVTGEN/evtgen/Init_xzj_pc.sh.

And then you can build the code by using the following command:

```bash
cd /path/to/PYTHIA_EVTGEN/
mkdir build
cd build
cmake ..
make -j4
```

Then run the code you want.
If you want to submit the jobs by using HTCondor, you can use the following command:

```bash
cd /path/to/PYTHIA_EVTGEN/
./submit.sh
```
It will create a file for jobs submission, but you need modity it according to your own requirement.

