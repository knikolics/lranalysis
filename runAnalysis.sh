if [ ! $# == 2 ]; then 
	echo "USAGE: ./runAnalysis.sh runNumber dataPath"
	exit
fi 

root -l -q CreateConfigFile.C\($1\)

../bin/LRAnalysis $2 $1
