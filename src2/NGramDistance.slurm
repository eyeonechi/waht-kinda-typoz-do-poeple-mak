#!/bin/bash
#SBATCH --job-name="kt1"
#SBATCH --time=0-03:30:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --partition=cloud
#SBATCH --output=NGramDistance.out

# Load required modules
module load Java/1.8.0_71

# Launch java code
echo "Processing Birkbeck"
javac *.java
time java NGramDistance
