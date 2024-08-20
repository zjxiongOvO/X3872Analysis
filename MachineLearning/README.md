# Machine Learning for X(3872) analysis

This part of the repository contains the code to train the machine learning models for the X(3872) analysis. The code is based on the python package HIPE4ML. The input data is the output of the Tree Analysis and Skimming part.

## Environment setup

You rather use the Singularity container or Conda to setup the environment.

Make sure you have these packages installed in your system:

```bash
    apt-get -y update
    apt-get -y install python3.6
    apt-get -y install python3-pip
    pip3 install hipe4ml==0.0.12
    pip3 install uproot
    pip3 install xxhash
    pip3 install lz4
```

## Train the models


## ONNX model