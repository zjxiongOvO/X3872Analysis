#!/bin/bash
export PYTHIA8DATA="/ustcfs/HICUser/zjxiong/tool/container/evtgen_sandbox/evtgen/software/share/Pythia8/xmldoc"
if [ -d "$PYTHIA8DATA" ]; then
    export PYTHIA8DATA
else
    echo "Warning: Pythia8 data directory not found: $PYTHIA8DATA"
fi

PYTHIA8="/ustcfs/HICUser/zjxiong/tool/container/evtgen_sandbox/evtgen/software/share/Pythia8"
if [ -d "$PYTHIA8" ]; then
    export PYTHIA8
else
    echo "Warning: Pythia8 root directory not found: $PYTHIA8"
fi

EVTGEN_ROOT="/ustcfs/HICUser/zjxiong/tool/container/evtgen_sandbox/evtgen/software/"
if [ -d "$EVTGEN_ROOT" ]; then
    export EVTGEN_ROOT
else
    echo "Warning: EvtGen root directory not found: $EVTGEN_ROOT"
fi

# if [ -d "${PYTHIA8}/bin" ] && [ -d "${EVTGEN_ROOT}/bin" ]; then
#     export PATH="${PYTHIA8}/bin:${EVTGEN_ROOT}/bin:$PATH"
# else
#     echo "Warning: Pythia8 or EvtGen bin directory not found."
# fi

# if [ -d "${PYTHIA8}/lib" ] && [ -d "${EVTGEN_ROOT}/lib" ]; then
#     export LD_LIBRARY_PATH="${PYTHIA8}/lib:${EVTGEN_ROOT}/lib:$LD_LIBRARY_PATH"
# else
#     echo "Warning: Pythia8 or EvtGen lib directory not found."
# fi

ROOT_SETUP_SCRIPT="/ustcfs/HICUser/zjxiong/tool/container/evtgen_sandbox/ROOT/root/bin/thisroot.sh"
if [ -f "$ROOT_SETUP_SCRIPT" ]; then
    source "$ROOT_SETUP_SCRIPT"
else
    echo "Warning: ROOT setup script not found: $ROOT_SETUP_SCRIPT"
fi

source $ROOT_SETUP_SCRIPT
mkdir -p build