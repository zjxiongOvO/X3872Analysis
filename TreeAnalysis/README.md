# Tree Analysis and Skimming

This part of the repository contains the code to perform the tree analysis and skimming.

## Tree Analysis
### header files
You can find the header files in the `header` directory. The header files contain the class definitions from MakeClass.
```
EvtgenSim.h: Contains the class definition for the Evtgen Simulation
Quadplet.h: Contains the class definition for reduced X(3872) table which produced by O2Physics
SkimmedTree.h: Contains the class definition for the skimmed tree, which is used for ML training, which is produced by this code
```

When you update the structure of the tree, you need to update the header files accordingly.
```bash
root -l file.root
root [0] T->MakeClass("EvtgenSim")
```
or
```bash
root -l file.root
root [0] T->MakeClass("Quadplet")
```

### scripts
We use the 'runAnalysis.sh' script to run the analysis. The script will run the tasks in the `tasks` directory.
If you are alreadly alias the `Aliash.sh` script, you can run the analysis by:

```bash
runX3872 --help
```

if you want to run the full analysis, you can run the following command:
```bash
runX3872 --analysis --treemaker --SigExt -config config.json
```
and some options are available available:
```bash
--eff  # Calculate the efficiency
--CutG  # Create the cut graph
```