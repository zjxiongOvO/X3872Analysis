#!/usr/bin/python
"""
Minimal example to run the package methods
"""
import sys
import os
import numpy as np
import pandas as pd
import xgboost as xgb
#import ROOT
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import label_binarize
import matplotlib.pyplot as plt

from hipe4ml.model_handler import ModelHandler
from hipe4ml.tree_handler import TreeHandler
from hipe4ml.analysis_utils import train_test_generator
from hipe4ml.analysis_utils import bdt_efficiency_array
#from hipe4ml import plot_utils

sys.path.append('.')
from utils import plot_utils

import argparse
import yaml
import shutil
from pathlib import Path
import itertools

# Define the function to extract all possible combinations of intervals
def extract_interval_combinations(*arrays):
    for array in arrays:
        if len(array) < 2:
            raise ValueError("Array must have at least 2 elements")
    
    intervals = [list(zip(arr[:-1], arr[1:])) for arr in arrays]
    all_combinations = list(itertools.product(*intervals))
    return all_combinations

parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('cfgFileName', metavar='text', default='cfgFileName.yml',
                    help='config file name with root input files')

analysis_type_parser=parser.add_mutually_exclusive_group(required=True)
analysis_type_parser.add_argument("--train", help="perform only training and testing", action="store_true")
analysis_type_parser.add_argument("--apply", help="perform only application", action="store_true")
args = parser.parse_args()

with open(args.cfgFileName, 'r') as ymlCfgFile:
    inputCfg = yaml.load(ymlCfgFile, yaml.FullLoader)


vars_to_sliced = inputCfg['vars_to_sliced']
arrays = inputCfg['sliced_arrays']
vars_to_read = inputCfg['vars_to_read']
vars_to_read_mc = inputCfg['vars_to_read_mc']
vars_to_learn = inputCfg['ml']['vars_to_learn']
output_path_train = inputCfg['output']['train']
output_path_apply = inputCfg['output']['apply']
applycolumns = inputCfg['applycolumns']

print("Configuration File: ",args.cfgFileName)
if args.train:
    print("It is training ML!")
    print("Output directory: ",output_path_train)
    conf_outputpath=output_path_train+'/config'
if args.apply:
    print("It is applying saved models!")
    print("Output directory: ",output_path_apply)
    conf_outputpath=output_path_apply+'/config'
if os.path.isdir(conf_outputpath):
    print((f'\033[93mWARNING: Output directory \'{conf_outputpath}\' already exists,'
        ' overwrites possibly ongoing!\033[0m'))
else:
    os.makedirs(conf_outputpath)

source_file = f"{args.cfgFileName}"
target_file = output_path_train+f"/{args.cfgFileName}"
target_file_path = Path(target_file)
if(target_file_path.exists()):
    print(f"* Deleting the old config file")
    os.remove(target_file_path)
    if(not target_file_path.exists()):
        print(f"* Have deleted the old config file")
print("* Whether have copied the config file:", target_file_path.exists())
shutil.copy(source_file, target_file)
print("* Whether have copied the config file:", target_file_path.exists())

def get_bkgH_signalH_for_training(dataTree_pm,dataTree_pp_mm,dataTree_mc,select_condition_bkg,select_condition_prompt,tag_from_unlike):

    if (tag_from_unlike):
        dataTree_bkg_select = dataTree_pm.get_subset(select_condition_bkg,rndm_state=inputCfg['data_prep']['seed_split'])
        bkgH_take_from = "unlike_sign"
        print("bkgH_take_from",bkgH_take_from)
    else:
        dataTree_bkg_select = dataTree_pp_mm.get_subset(select_condition_bkg,rndm_state=inputCfg['data_prep']['seed_split'])
        bkgH_take_from = "like_sign"
        print("bkgH_take_from",bkgH_take_from)
    print("select_condition_bkg == ",select_condition_bkg)

    dataTree_prompt_select    = dataTree_mc.get_subset(select_condition_prompt,rndm_state=inputCfg['data_prep']['seed_split'])

    n_cand = [dataTree_bkg_select.get_n_cand(),dataTree_prompt_select.get_n_cand()]
    n_cand_max = np.max(n_cand)
    n_cand_min = np.min(n_cand)
    print(type(n_cand_min))

    if (inputCfg['data_prep']['size_of_datasets_max'] == 0):
        if (inputCfg['data_prep']['dataset_opt'] == 'equal'):
            size_of_trainsets_testsets_bkg       = n_cand_min
            size_of_trainsets_testsets_prompt    = n_cand_min
        elif (inputCfg['data_prep']['dataset_opt'] == 'maxsignal'):
            size_of_trainsets_testsets_prompt    = n_cand[1]
            factor_bkg_mult=inputCfg['data_prep']['bkg_mult'][0]
            size_of_trainsets_testsets_bkg       = round(factor_bkg_mult*(n_cand[1]))
            if (size_of_trainsets_testsets_bkg >= n_cand[0]):
                size_of_trainsets_testsets_bkg = n_cand[0]
                print('Using all bkg candidates available!')
        elif (inputCfg['data_prep']['dataset_opt'] == 'ratio'):
            size_of_trainsets_testsets_bkg       = n_cand_min
            size_of_trainsets_testsets_prompt    = n_cand_min * inputCfg['data_prep']['dataset_ratio']
        else:
            print(f"\033[91mERROR: {inputCfg['data_prep']['dataset_opt']} is not a valid option!\033[0m")
            sys.exit()
    elif (inputCfg['data_prep']['size_of_datasets_max'] > 0):
        if (n_cand[0] >= inputCfg['data_prep']['size_of_datasets_max']):
            size_of_trainsets_testsets_bkg       = inputCfg['data_prep']['size_of_datasets_max']
        else:
            size_of_trainsets_testsets_bkg       = n_cand[0]
        if (n_cand[1] >= inputCfg['data_prep']['size_of_datasets_max']):
            size_of_trainsets_testsets_prompt    = inputCfg['data_prep']['size_of_datasets_max']
        else:
            size_of_trainsets_testsets_prompt    = n_cand[1]
        print((f"The size of bkg datasets for training was set to {size_of_trainsets_testsets_bkg}"))
        print((f"The size of prompt datasets for training was set to {size_of_trainsets_testsets_prompt}"))
    else:
        print("inputCfg['data_prep']['size_of_datasets_max'] can not be a negetive number!")
        sys.exit()

    print("dataTree_bkg_select_cand = ",dataTree_bkg_select.get_n_cand())
    print("dataTree_prompt_select_cand = ",dataTree_prompt_select.get_n_cand())
    print("n_cand_min = ", n_cand_min)
    print("size_of_sample_select_bkg = ",size_of_trainsets_testsets_bkg)
    print("size_of_sample_select_prompt = ",size_of_trainsets_testsets_prompt)

    bkgH       = dataTree_bkg_select.get_subset(size=size_of_trainsets_testsets_bkg,rndm_state=inputCfg['data_prep']['seed_split'])
    promptH    = dataTree_prompt_select.get_subset(size=size_of_trainsets_testsets_prompt,rndm_state=inputCfg['data_prep']['seed_split'])

    print("n_cand_bkg =",bkgH.get_n_cand())
    print("<<-----------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-------------------------->>")
    print(" ")

    del dataTree_bkg_select, dataTree_prompt_select

    return bkgH,promptH

# def train_and_test_at_centrality_and_pt(centrality_cut_low,centrality_cut_high,pt_cut_low,pt_cut_high):
def train_and_test_sliced(vars_to_sliced, combo):

    # --------------------------------------------
    SlicedTAG = ""
    for i, var in enumerate(vars_to_sliced):
        if i == 0:
            SlicedTAG += f"{combo[i][0]} < {var} < {combo[i][1]}"
        else:
            SlicedTAG += " and "
            SlicedTAG += f"{combo[i][0]} < {var} < {combo[i][1]}"
    SavedTAG = ""
    for i, var in enumerate(vars_to_sliced):
        if i == 0:
            SavedTAG += f"{var}_{combo[i][0]}_{combo[i][1]}"
        else:
            SavedTAG += f"_{var}_{combo[i][0]}_{combo[i][1]}"
    print("SlicedTAG = ",SlicedTAG)
    print("SavedTAG = ",SavedTAG)


    # DATA PREPARATION (load data from sklearn digits dataset)
    path_to_unlike = inputCfg['input']['train']['data_unlike']['path']
    path_to_like   = inputCfg['input']['train']['data_like']['path']
    path_to_sim = inputCfg['input']['train']['sim']['path']
    print("Input DATA Unlike file: ",path_to_unlike)
    print("Input DATA Like file: ",path_to_like)
    print("Input MC file: ",path_to_sim)
    # --------------------------------------------


    # --------------------------------------------
    dataH_PP_MM_input = TreeHandler(path_to_like,inputCfg['input']['train']['data_like']['treename'],vars_to_read)
    dataH_PM_input    = TreeHandler(path_to_unlike,inputCfg['input']['train']['data_unlike']['treename'],vars_to_read)
    simulationH_input = TreeHandler(path_to_sim ,inputCfg['input']['train']['sim']['treename'],vars_to_read_mc)
    # --------------------------------------------# --------------------------------------------

    if (inputCfg['data_prep']['sign_type']['unlikesign_sideband']):
        if (inputCfg['data_prep']['sign_type']['likesign']):
            print("It can not use the unlike sign side band and like sign at the same time!!")
            sys.exit()
        else:
            mass_cut_bkg_name =inputCfg['data_prep']['sign_type']['filt_bkg_mass_unlike']

    elif (inputCfg['data_prep']['sign_type']['likesign']):
        mass_cut_bkg_name =inputCfg['data_prep']['sign_type']['filt_bkg_mass_like']

    else:
        print("Both the unlike sign side band and like sign are not used. At least one of the two need to set to be True!!")
        sys.exit()

    print("mass_cut_bkg_name = ",mass_cut_bkg_name)
    # --------------------------------------------


    fig_outputpath=output_path_train+'/fig/'
    if os.path.isdir(fig_outputpath):
        print((f'\033[93mWARNING: Output directory \'{fig_outputpath}\' already exists,'
            ' overwrites possibly ongoing!\033[0m'))
    else:
        os.makedirs(fig_outputpath)

        # --------------------------------------------
    select_condition_bkg       = SlicedTAG+" and "+mass_cut_bkg_name
    select_condition_prompt    = SlicedTAG
    # --------------------------------------------

    bkgH, promptH = get_bkgH_signalH_for_training(dataH_PM_input,dataH_PP_MM_input,simulationH_input,select_condition_bkg,select_condition_prompt,inputCfg['data_prep']['sign_type']['unlikesign_sideband'])

    del dataH_PM_input, dataH_PP_MM_input, simulationH_input

    train_test_data = train_test_generator([bkgH,promptH],[0,1],test_size=inputCfg['data_prep']['test_fraction'],random_state=inputCfg['data_prep']['seed_split'])

    leg_labels = ['Background',r'X(3872) signal']
    vars_to_draw = vars_to_read;

    plot_utils.plot_distr([bkgH, promptH], vars_to_draw, bins=100, labels=leg_labels, log=True, density=True, figsize=(12, 12), alpha=0.3, grid=False, histtype='step', stacked=True) ##density means normalization open log
    plt.savefig(fig_outputpath+'QA1'+SavedTAG+'.pdf',dpi=1000)

    plt.subplots_adjust(left=0.06, bottom=0.06, right=0.99, top=0.96, hspace=0.55, wspace=0.55)
    features_correlation = plot_utils.plot_corr([bkgH, promptH], vars_to_draw, leg_labels)
    plt.xticks(fontsize=15,fontweight=500)
    plt.yticks(fontsize=15,fontweight=500)
    features_correlation[0].savefig(fig_outputpath+'correlation_bkg'+SavedTAG+'.pdf',dpi=1000)
    features_correlation[1].savefig(fig_outputpath+'correlation_signal'+SavedTAG+'.pdf',dpi=1000)

    # --------------------------------------------

    # TRAINING AND TESTING
    # --------------------------------------------
    hyper_pars_optimized = inputCfg['ml']['hyper_par']['hyper_pars_optimized'][0]
    model_clf = xgb.XGBClassifier()
#    model_hdl = ModelHandler(model_clf,vars_to_learn,hyper_pars_optimized)
    model_hdl = ModelHandler(model_clf,vars_to_learn)

    print("vars_to_learn =",vars_to_learn)

    if inputCfg['ml']['hyper_par']['do_hyp_opt']:
        model_hdl.set_model_params({'tree_method':'hist'})
        hyper_pars_ranges = inputCfg['ml']['hyper_par']['bayes_opt_config']
        print("hyper_pars_ranges:",hyper_pars_ranges)
        model_hdl.optimize_params_optuna(train_test_data,hyper_pars_ranges, cross_val_scoring='roc_auc', timeout=120, n_jobs = inputCfg['ml']['hyper_par']['njobs'], n_trials = inputCfg['ml']['hyper_par']['niter'], direction='maximize')
    else:
        model_hdl.set_model_params(hyper_pars_optimized)

    if (inputCfg['ClassificationMethod'] == "3classification"):
        model_hdl.train_test_model(train_test_data,False,False,average='macro',multi_class_opt='ovo')
        #model_hdl.train_test_model(train_test_data,False,False,average='weighted',multi_class_opt='ovo')
    else:
        model_hdl.train_test_model(train_test_data,False,False,average='macro')

    # save model handler in pickle
    model_outputpath=output_path_train+'/TrainedModels'
    if os.path.isdir(model_outputpath):
        print((f'\033[93mWARNING: Output directory \'{model_outputpath}\' already exists,'
            ' overwrites possibly ongoing!\033[0m'))
    else:
        os.makedirs(model_outputpath)
    model_hdl.dump_model_handler(f'{model_outputpath}/ModelHandler{SavedTAG}.pickle')
    model_hdl.dump_original_model(f'{model_outputpath}/XGBoostModel{SavedTAG}.model', True)

    hyperparams_after_optimization = model_hdl.get_model_params()
    #print(hyperparams_after_optimization)


    model_hdl.get_training_columns()
    print("mytest")
    print("training_columns:",model_hdl.get_training_columns())
    with open(output_path_train+'/TrainedModels/'+'machine_learning_parameters'+SavedTAG+'.txt',"w") as file:
        temp = "\n"
        file.write(str(hyperparams_after_optimization)+temp)
        file.write(str(model_hdl.get_training_columns()))


    learning_curve = plot_utils.plot_learning_curves(model_hdl,train_test_data,10)
    learning_curve.savefig(fig_outputpath+'learning_rate.pdf')


    y_pred_train = model_hdl.predict(train_test_data[0],False)
    y_pred_test  = model_hdl.predict(train_test_data[2],False)

    plot_utils.plot_precision_recall(train_test_data[1],y_pred_train)
    plt.savefig(fig_outputpath+'precision_recall'+SavedTAG+'.pdf',dpi=1000)


    temp1 = 'ALICE'

    ## Train and Test BDT output
    ml_out_fig = plot_utils.plot_output_train_test(model_hdl,train_test_data,100,False,leg_labels,logscale=True,density=True)
    plt.rcParams["figure.figsize"] = (20,15)
    plt.xticks(fontsize=12,fontweight=500)
    plt.yticks(fontsize=12,fontweight=500)
    plt.xlabel('BDT Output',fontsize=20,fontweight=500)
    plt.ylabel('Counts',fontsize=20,fontweight=500)
    plt.ylim(8.e-3,3.e3)
    plt.legend(frameon=False,loc=1,fontsize=13)
    ml_out_fig.savefig(fig_outputpath+'train_and_test'+SavedTAG+'.pdf',dpi=1000)

    ## ROC AUC
    roc_train_test_fig = plot_utils.plot_roc_train_test(train_test_data[3], y_pred_test,train_test_data[1], y_pred_train, None, leg_labels)
    plt.rcParams["figure.figsize"] = (15,10)
    plt.xticks(fontsize=12,fontweight=300)
    plt.yticks(fontsize=12,fontweight=300)
    plt.xlabel('False Positive Rate',fontsize=18,fontweight=500)
    plt.ylabel('True Positive Rate',fontsize=18,fontweight=500)
    plt.legend(frameon=False,fontsize=15)
    plt.text(0.5,0.9,f'{temp1}',fontsize=18,fontfamily='DejaVu Sans',fontweight='heavy')
    roc_train_test_fig.savefig(fig_outputpath+'roc'+SavedTAG+'.pdf',dpi=1000)

    ## Feature Importance
    features_importance = plot_utils.plot_feature_imp(train_test_data[2],train_test_data[3],model_hdl)
    features_importance[0].savefig(fig_outputpath+'feature_imp_0'+SavedTAG+'.pdf',dpi=1000)
    features_importance[1].savefig(fig_outputpath+'feature_imp_1'+SavedTAG+'.pdf',dpi=1000)


    print("Training finished!!!!")
    print("<<-----------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-------------------------->>")


def apply_sliced(vars_to_sliced, combo):

    SliceTAG = ""
    for i, var in enumerate(vars_to_sliced):
        if i == 0:
            SliceTAG += f"{combo[i][0]} < {var} < {combo[i][1]}"
        else:
            SliceTAG += " and "
            SliceTAG += f"{combo[i][0]} < {var} < {combo[i][1]}"
    SavedTAG = ""
    for i, var in enumerate(vars_to_sliced):
        if i == 0:
            SavedTAG += f"{var}_{combo[i][0]}_{combo[i][1]}"
        else:
            SavedTAG += f"_{var}_{combo[i][0]}_{combo[i][1]}"
    print("SliceTAG = ",SliceTAG)
    print("SavedTAG = ",SavedTAG)

    ## DATA PREPARATION (load data from sklearn digits dataset)
    path_to_unlike = inputCfg['input']['apply']['data_unlike']['path']
    path_to_like   = inputCfg['input']['apply']['data_like']['path']
    path_to_sim = inputCfg['input']['apply']['sim']['path']
    print("Input DATA Unlike file: ",path_to_unlike)
    print("Input DATA Like file: ",path_to_like)
    print("Input MC file: ",path_to_sim)
    # --------------------------------------------


    # --------------------------------------------
    dataH_PP_MM_input = TreeHandler(path_to_like,inputCfg['input']['apply']['data_like']['treename'],vars_to_read)
    dataH_PM_input    = TreeHandler(path_to_unlike,inputCfg['input']['apply']['data_unlike']['treename'],vars_to_read)
    simulationH_input = TreeHandler(path_to_sim ,inputCfg['input']['apply']['sim']['treename'],vars_to_read_mc)
    # --------------------------------------------# --------------------------------------------

    # --------------------------------------------
    dataH_PP_MM = dataH_PP_MM_input.get_subset(SliceTAG)
    dataH_PM    = dataH_PM_input.get_subset(SliceTAG)

    select_condition_prompt    =  SliceTAG
    promptH    = simulationH_input.get_subset(select_condition_prompt,rndm_state=inputCfg['data_prep']['seed_split'])
    # --------------------------------------------

    del dataH_PM_input, dataH_PP_MM_input, simulationH_input


    ModelPath = os.path.expanduser(f'{output_path_train}/TrainedModels/ModelHandler{SavedTAG}.pickle')
    print(f'Loaded saved model: {ModelPath}')
    model_hdl = ModelHandler()
    model_hdl.load_model_handler(ModelPath)


    # --------------------------------------------
    dataH_PM.apply_model_handler(model_hdl, False)
    dataH_PP_MM.apply_model_handler(model_hdl, False)
    promptH.apply_model_handler(model_hdl,False)

    columns_to_save = inputCfg['apply']['columns_to_save']

    output_file_name_temp_pm = 'Sliced_'+SavedTAG+'_PM'
    output_file_name_temp_pp_mm = 'Sliced_'+SavedTAG+'_PP_MM'

    file_outputpath=output_path_apply+'/AppliedROOTFiles'
    print(type(file_outputpath))
    if os.path.isdir(file_outputpath):
        print((f'\033[93mWARNING: Output directory \'{file_outputpath}\' already exists,'
            ' overwrites possibly ongoing!\033[0m'))
    else:
        os.makedirs(file_outputpath)
    dataH_PM.write_df_to_root_files(output_file_name_temp_pm,'fTreePairsSPM',file_outputpath)
    dataH_PP_MM.write_df_to_root_files(output_file_name_temp_pp_mm,'fTreePairsSPP_MM',file_outputpath)
    promptH.write_df_to_root_files(output_file_name_temp_pm+'_signal','fTreePairsSPM', file_outputpath)


    print("Applying finished!!!!")
    print("<<-----------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-------------------------->>")

combinations = extract_interval_combinations(*arrays)
for combo in combinations:
    print("Machine Learning for the following intervals:")
    for i, var in enumerate(vars_to_sliced):
        print(f"{var}: {combo[i]}")
    if args.train: 
        train_and_test_sliced(vars_to_sliced, combo)
    if args.apply:
        apply_sliced(vars_to_sliced, combo)
    
