#!/bin/bash

print_help() {
    echo "Usage: $0 [--skim] -config <config.json>"
    echo "          [--treemaker] -config <config.json>"
    echo "          [--scan] -i <input.root> -start <start> -end <end> -Nstep <step>"
}

# check if the $X3872PATH is set
if [ -z "$X3872PATH" ]; then
    echo "Error: X3872PATH is not set"
    exit 1
fi

configpath=""
inputpath=""
start=0
end=1
Nstep=1

# options
skim=false
scan=false
treemaker=false

if [[ $# -eq 0 ]]; then
    print_help
    exit 1
fi

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        --skim)
            skim=true
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
        -config)
            shift
            configpath="$1"
            shift
            ;;
        -i)
            shift
            inputpath="$1"
            shift
            ;;
        -start)
            shift
            start="$1"
            shift
            ;;
        -end)
            shift
            end="$1"
            shift
            ;;
        -Nstep)
            shift
            Nstep="$1"
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

# run the tree maker
if [ "$treemaker" = true ]; then
    if [ -z "$configpath" ]; then
        echo "Error: config path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/RunTreeMaker.cxx(\"$configpath\")"
fi

# run Analysis
if [ "$skim" = true ]; then
    if [ -z "$configpath" ]; then
        echo "Error: config path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/RunAnalysis.cxx(\"$configpath\")"
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/DrawAnalysisResults.cxx(\"$configpath\")"
fi

# run BDT scan
if [ "$scan" = true ]; then
    if [ -z "$inputpath" ]; then
        echo "Error: input path is not set"
        exit 1
    fi
    root -l -b -q "$X3872PATH/TreeAnalysis/tasks/ScanBDT.cxx(\"$inputpath\", $start, $end, $Nstep)"
fi