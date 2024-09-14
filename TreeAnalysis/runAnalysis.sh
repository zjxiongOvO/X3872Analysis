#!/bin/bash

print_help() {
    echo "Usage: $0 [--analysis] [--treemaker] [--eff] [--CutG] [--SigExt] -config <config.json>"
}

# check if the $X3872PATH is set
if [ -z "$X3872PATH" ]; then
    echo "Error: X3872PATH is not set"
    exit 1
fi

configpath=""

# options
treemaker=false
analysis=false
eff=false
CutG=false
SigExt=false

if [[ $# -eq 0 ]]; then
    print_help
    exit 1
fi

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        --analysis)
            analysis=true
            shift
            ;;
        --scan)
            scan=true
            shift
            ;;
        --treemaker)
            treemaker=true
            shift
            ;;
        --eff)
            eff=true
            shift
            ;;
        --CutG)
            CutG=true
            shift
            ;;
        --SigExt)
            SigExt=true
            shift
            ;;
        -config)
            shift
            configpath="$1"
            shift
            ;;
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            echo "Unknown option: $key"
            print_help
            exit 1
            ;;
    esac
done

# run tree-maker
if [ "$treemaker" = true ]; then
    if [ -z "$configpath" ]; then
        echo "Error: config path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/RunTreeMaker.cxx(\"$configpath\")"
fi

# run Analysis
if [ "$analysis" = true ]; then
    if [ -z "$configpath" ]; then
        echo "Error: config path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/RunAnalysis.cxx(\"$configpath\")"
fi

# run efficiency
if [ "$eff" = true ]; then
    if [ -z "$configpath" ]; then
        echo "Error: config path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/CalculateEff.cxx(\"$configpath\")"
fi

if [ "$CutG" = true ]; then
    if [ -z "$configpath" ]; then
        echo "Error: config path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/CutGCreater.cxx(\"$configpath\")"
fi

if [ "$SigExt" = true ]; then
    if [ -z "$configpath" ]; then
        echo "Error: config path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/RunSignalExtraction.cxx(\"$configpath\")"
fi