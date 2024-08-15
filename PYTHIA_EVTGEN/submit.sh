njob=1000
container_name=evtgen_sandbox
outpurfilename=evtgen_0327_spintest6
common_dir=/ustcfs/HICUser/zjxiong/output/simulation
outputdir=$common_dir/$outpurfilename
subfilename=output

echo "Universe     = vanilla
Priority     =

Notification = never
GetEnv       = true
should_transfer_files= YES

Initialdir   = ./
Executable   = ./run.sh
arguments    = \$(ipath) t\$(ijob)
max_materialize = 500

Log          = ./submit.run/log/run\$(Process).log
Output       = ./submit.run/out/run\$(Process).out
Error        = ./submit.run/err/run\$(Process).err
Queue ipath from more all_part.txt |
Queue ijob from 1 to $njob
" > submit.condor

echo "#!/bin/bash
path=\$1
job=\$2
singularity shell -B /ustcfs:/ustcfs /ustcfs/HICUser/zjxiong/tool/container/$container_name << eof
source /ustcfs/HICUser/zjxiong/tool/container/$container_name/evtgen/setup.sh
cd \$path
echo \$(date) >> log_test
./maker \$job >> log_test
echo \$(date) >> log_test
eof
" > run.sh
chmod +x run.sh


for i in `seq 1 $njob`
do
mkdir -p $outputdir/$subfilename$i
cp build/maker $outputdir/$subfilename$i/.
echo $outputdir"/"$subfilename$i >> all_part.txt
done

cp -r Basefile/ $outputdir/.
cp -r build/ $outputdir/.
mv run.sh $outputdir/.
mv submit.condor $outputdir/.
mv all_part.txt $outputdir/.
mkdir -p $outputdir/submit.run/log
mkdir -p $outputdir/submit.run/out
mkdir -p $outputdir/submit.run/err

echo $outputdir
