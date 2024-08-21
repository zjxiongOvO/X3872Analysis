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

### tasks
The first step is to convert the Evtgen simulation tree and the reduced X(3872) table into a skimmed tree. The skimmed tree is used for ML training and Analysis.

We have a task named `RunTreeMaker.cxx` which is used for this purpose. The task is defined in the `tasks` directory. For the Evtgen simulation tree, It will convert the tree into a skimmed tree. For the reduced X(3872) table, it will do a pre-filter to reduce the size of the tree.

After the conversion, you can do additional filter and analysis using the `RunTreeAnalysis.cxx` task. And calculate the Acceptance, draw comparison plots, Signal-Extraction, etc. By using the `DrawAnalysisResults.cxx` task.

It will also produce a `Skimmed Tree` which is used for ML training.

### scripts
We use the 'runAnalysis.sh' script to run the analysis. The script will run the tasks in the `tasks` directory.
If you are alreadly alias the `Aliash.sh` script, you can run the analysis by:

```bash
runX3872 --help
```