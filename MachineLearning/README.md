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

Please run the following command to train the models:

```bash
    python3 task/RunModelTraining.py --train config/configML_example.yml
```
and if you want apply this model to a tree, you can run the following command:

```bash
    python3 task/RunModelTraining.py --apply config/configML_example.yml
```

The configuration file is in the `config` directory. You can modify the configuration file to change the training parameters.

About the yml file:
```yaml
vars_to_sliced: list of variables need to be sliced, e.g. ['Centrality', 'Pt']
sliced_arrays: list of arrays need to be sliced, e.g.
        - [0, 30, 60, 90] # Centrality
        - [0, 1, 2, 3] # Pt
output: 
            train: output path for training model, e.g. /data/ML_models/
            apply: output path for applying model, e.g. /data/ML_models/

applycolumns: list of columns need to be applied, e.g. ['Centrality', 'Pt', 'model_output']

input: 
            train: 
                sim: 
                    path: path to the simulation file
                    treename: name of the tree in the simulation file
                data_unlike: 
                    path: path to the data unlike-sign file
                    treename: name of the tree in the data unlike-sign file
                data_like: 
                    path: path to the data like-sign file
                    treename: name of the tree in the data like-sign file
            apply: 
                sim: 
                    path: path to the simulation file
                    treename: name of the tree in the simulation file
                data_unlike: 
                    path: path to the data unlike-sign file
                    treename: name of the tree in the data unlike-sign file
                data_like:
                    path: path to the data like-sign file
                    treename: name of the tree in the data like-sign file
        
vars_to_read: list of variables need to be read, e.g. ['Mass', 'Pt', 'Q', 'DeltaR', 'fkDeltaPhiPiPi', 'fDiTracksPt']
vars_to_read_mc: list of variables need to be read for MC, e.g. ['Mass', 'Pt', 'Q', 'DeltaR', 'fkDeltaPhiPiPi', 'fDiTracksPt']

data_prep: 
    event_type: 
        same_event: Is the data from the same event? True/False
        mixed_event: Is the data from the mixed event? True/False
    sign_type:
        unlikesign_sideband: should we use unlike-sign sideband as background? True/False
        filt_bkg_mass_unlike: pandas query to select sideband bkg candidates, e.g. '(3.81 < Mass < 3.84 or 3.90 < Mass < 3.93)'
        likesign: should we use like-sign as background? True/False
        filt_bkg_mass_like: pandas query to select like-sign bkg candidates, e.g. '(3.84 < Mass < 3.90)'
    size_of_datasets_max: 0
    dataset_opt: choose how the dataset is built, options available: 'equal', 'max_signal', 'ratio'
                    # 'equal' -> same number of prompt/FD/bkg (not using all the signal available)
                    # 'maxsignal' -> try to use all the signal (prompt and FD) + add n_bkg = 2 * (n_prompt + n_FD)
                    # 'ratio' -> control the ratio of signal and bkg
    dataset_ratio: ratio of signal and bkg in the dataset, e.g. 0.5
    seed_split: seed used for train_test_split(...), e.g. 42
    test_fraction: fraction of data used for test set and efficiencies, e.g. 0.5

ml:
    raw_output: False # use raw_output (True) of probability (False) as output of the model
    roc_auc_average: 'macro' # 'macro' or 'weighted'
    roc_auc_approach: 'roc_auc_ovo'  # 'ovo' or 'ovr'
    vars_to_learn: list of variables to learn, e.g. ['Q', 'DeltaR', 'fkDeltaPhiPiPi']

    hyper_par: 
        do_hyp_opt: True 
        njobs: -1 # number of parallel jobs used in hyper-parameter optimization, -1. to use all
        nfolds: 5 # number of folds used in cross validation
        initpoints: 10 # steps of random exploration you want to perform
        niter: 25 # steps for bayesian optimization
        bayes_opt_config: {'max_depth': !!python/tuple [2, 4], 
                        'learning_rate': !!python/tuple [0.01, 0.2],
                        'n_estimators': !!python/tuple [2, 380], 
                        'min_child_weight': !!python/tuple [1, 10],
                        'subsample': !!python/tuple [0.8, 1.], 
                        'colsample_bytree': !!python/tuple [0.8, 1.]}
                        # configuration dictionary for optimize_params_bayes()

        hyper_pars_optimized: [{'max_depth':3, 'learning_rate':0.023, 'n_estimators':1028, 'min_child_weight':5, 'colsample':0.9, 'n_jobs':4, 'tree_method':hist}
               ] # list of dicts of hyperparameters (one for each pT bin) bk=n_estimators1028

apply:
    columns_to_save: ['Mass', 'Pt', 'model_output'] # list of columns to save in the output file
```

## ONNX model
Sometimes you may want to convert the trained model to ONNX format. Because ONNX is a standard format for machine learning models, it allows you to use the model in different frameworks. For example, you can use the ONNX model in O2Physics. (upload it to CCDB, and use it in the analysis)

To convert the model to ONNX format, you can run the following command:
    
```bash
    python3 task/RunModelTraining.py --onnx config/configML_example.yml
```